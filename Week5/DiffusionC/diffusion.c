/*
 * Note: this code is intentionally very badly written, it has a
 * a number of issues, both conceptually and in the implementation.
 * Do not use this for any other purpose than an exercise in bug
 * fixing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <err.h>

#define INPUT_ERR 11

double *create_temp(int n[]);
void init_temp(double *temp, int coords[],
               int n_global[], int n_local[]);
void update_temp(MPI_Comm comm, double *temp_new, double *temp, int n_local[],
                 int n_global[]);
void print_temp(double *temp, int n_x, int n_y);

int main(int argc, char *argv[]) {
    if ( argc != 4 )
    {
        errx(INPUT_ERR, "The provided number of input parameters is different from 3. Please run the program with correct number of inputs.\n");
    }
    const int nr_dims = 2;
    MPI_Init(NULL, NULL);
    int n_global[] = {atoi(argv[1]), atoi(argv[2])};
    int t_max = atoi(argv[3]);
    int my_size;
    MPI_Comm_size(MPI_COMM_WORLD, &my_size);
    /* potentially mess up further by initializing to
     * {1, my_size} */
    int dims[] = {0, 0};
    MPI_Dims_create(my_size, nr_dims, dims);
    int n_local[] = {
        n_global[0]/dims[0], n_global[1]/dims[1]
    };
    /* potentially mess up further by not using a Carthesian
     * communicator */
    MPI_Comm comm;
    int periods[] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, nr_dims, dims, periods,
                    1, &comm);
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int coords[nr_dims];
    /* potentially mess up further by reusing rank in
     * MPI_COMM_WORLD */
    MPI_Cart_coords(comm, my_rank, nr_dims, coords);

    double *temp = create_temp(n_local);
    double *temp_new = create_temp(n_local);

    init_temp(temp, coords, n_global, n_local);
    printf("/// Initial temperature on rank %d:\n", my_rank);
    print_temp(temp, n_local[0], n_local[1]);

    for (int t = 1; t <= t_max; t++) {
        update_temp(comm, temp_new, temp, n_local, n_global);

        double *tmp = temp;
        temp = temp_new;
        temp_new = tmp;
    }
    printf("/// Final on rank %d:\n", my_rank);
    print_temp(temp, n_local[0], n_local[1]);

    /* remove to create a memory leak */
    free(temp);
    free(temp_new);
    MPI_Finalize();
    return EXIT_SUCCESS;
}

double *create_temp(int n[]) {
    double *temp = (double *) malloc( ( n[0] + 2 ) * // +2: reserve space also for halos on left and right
                                      ( n[1] + 2 ) * sizeof(double) );
    for (int i = 0; i < n[0]+2; i++)
        for (int j = 0; j < n[1]+2; j++)
            temp[i*(n[1]+2) + j] = 0.0;
    return temp;
}

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

void init_temp(double *temp, int coords[],
               int n_global[], int n_local[]) {
    int n_heated[] = {n_global[0]/10, n_global[1]/10};
    int center[] = {n_global[0]/2, n_global[1]/2};
    int offset[] = {
        coords[0]*n_local[0], coords[1]*n_local[1]
    };
    int lower_left[] = {
        center[0] - n_heated[0]/2, center[1] - n_heated[1]/2
    };
    int upper_right[] = {
        center[0] + n_heated[0]/2, center[1] + n_heated[1]/2
    };
    if (lower_left[0] < offset[0] + n_local[0] &&
            offset[0] <= upper_right[0] &&
        lower_left[1] < offset[1] + n_local[1] &&
            offset[1] <= upper_right[1]) {
        int i_min = lower_left[0] - offset[0] + 1;
        int i_max = MIN(n_local[0] + 1,
                        upper_right[0] - offset[0] + 1);
        if ( i_min == i_max ) i_max += 1;
        int j_min = lower_left[1] - offset[1] + 1;
        int j_max = MIN(n_local[1] + 1,
                        upper_right[1] - offset[1] + 1);
        if ( j_min == j_max ) j_max += 1;
        for (int i = i_min; i < i_max; i++)
            for (int j = j_min; j < j_max; j++)
                temp[i * ( n_local[1] + 2 ) + j] = 1.0;
    }
}

void halo_temp(MPI_Comm comm, double *temp, int n_local[]) {
    const int tag = 17;
    MPI_Datatype row_type, col_type;
    MPI_Type_contiguous(n_local[1], MPI_DOUBLE, &row_type);
    MPI_Type_commit(&row_type);
    MPI_Type_vector(n_local[0], 1, n_local[1]+2, MPI_DOUBLE,
                    &col_type);
    MPI_Type_commit(&col_type);
    int row_source, row_dest;
    MPI_Cart_shift(comm, 0, 1, &row_source, &row_dest);

    int col_source, col_dest;
    MPI_Cart_shift(comm, 1, 1, &col_source, &col_dest);

    MPI_Request left_req, right_req, up_req, down_req;
    MPI_Irecv(temp + (n_local[1] + 2), // receive in left halo.
              1, col_type, col_source, tag, comm,
              &left_req);
    MPI_Irecv(temp + 2 * ( n_local[1] + 2 ) - 1, // receive in the right halo.
              1, col_type, col_dest, tag, comm,
              &right_req);
    MPI_Irecv(temp + 1, // receive in the top halo.
              1, row_type, row_source, tag, comm,
              &up_req);
    MPI_Irecv(temp + 1 + (n_local[0] + 1)* (n_local[1] + 2), // receive in the bottom halo
              1, row_type, row_dest, tag, comm,
              &down_req);

    MPI_Send(temp + 2 * (n_local[1] + 2) - 2, 1, col_type, col_dest, tag, comm);
    MPI_Send(temp + ( n_local[1] + 2) + 1, 1, col_type, col_source,
             tag, comm);
    MPI_Send(temp + 1 + n_local[0] * (n_local[1] + 2), 1, row_type, row_dest, tag, comm);
    MPI_Send(temp + 1 + ( n_local[1] + 2 ), 1,
             row_type, row_source, tag, comm);

    MPI_Wait(&left_req, MPI_STATUS_IGNORE);
    MPI_Wait(&right_req, MPI_STATUS_IGNORE);
    MPI_Wait(&up_req, MPI_STATUS_IGNORE);
    MPI_Wait(&down_req, MPI_STATUS_IGNORE);
}

void update_temp(MPI_Comm comm, double *temp_new, double *temp, int n_local[],
                 int n_global[]) {
    halo_temp(comm, temp, n_local);
    double dx2 = 1.0/(n_global[0] * n_global[0]);
    double dy2 = 1.0/(n_global[1] * n_global[1]);
    double d = 1/(2*(dx2 + dy2));
    //double dx = 0.25/n_global[0];
    //double dy = 0.25/n_global[1];
    for (int i = 1; i <= n_local[0]; i++)
        for (int j = 1; j <= n_local[1]; j++) {
            int idx = i * ( n_local[1] + 2 ) + j;
            int up = (i - 1) * ( n_local[1] + 2 ) + j;
            int down = (i + 1) * ( n_local[1] + 2 ) + j;
            int left = i * ( n_local[1] + 2 ) + j - 1;
            int right = i * ( n_local[1] + 2 ) + j + 1;
            temp_new[idx] = temp[idx] +
                d * ( ( temp[left] - 2*temp[idx] + temp[right] ) * dx2 +
                      ( temp[up] - 2*temp[idx] + temp[down] ) * dy2 );
        }
    int row_source, row_dest;
    MPI_Cart_shift(comm, 0, 1, &row_source, &row_dest);

    int col_source, col_dest;
    MPI_Cart_shift(comm, 1, 1, &col_source, &col_dest);

    // Clear up boundaries (temperature is prescribed there):
    if ( row_source == MPI_PROC_NULL ) { // global top row
        for ( int j = 1; j <= n_local[1]; j++ )
            temp_new[ ( n_local[1] + 2 ) + j] = 0;
    }
    if (row_dest == MPI_PROC_NULL){ // global bottom row
        for ( int j = 1; j <= n_local[1]; j++ )
            temp_new[ n_local[0] * ( n_local[1] + 2 ) + j  ] = 0;
    }
    if ( col_source == MPI_PROC_NULL ) { // global left column
        for ( int i = 1; i <= n_local[0]; i++ )
            temp_new[i * ( n_local[1] + 2 ) + 1] = 0;
    }
    if ( col_dest == MPI_PROC_NULL ){ // global right column
        for ( int i = 1; i <= n_local[0]; i++ )
            temp_new[i * ( n_local[1] + 2 ) + n_local[1]  ] = 0;
    }

}

void print_temp(double *temp, int n_x, int n_y) {
    for (int i = 1; i <= n_x; i++)
        for (int j = 1; j <= n_y; j++)
            printf("%.4e%c", temp[i*(n_y+2) + j], j == n_y ? '\n' : ' ');
}
