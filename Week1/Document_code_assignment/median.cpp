/*!
  \mainpage median program

  This is a program that computes median value from those provided by user at runtime using CLI.

  \section install_sec Compilation

  `g++ -std=c++14 median.cpp -o median`

  \section Usage

  Run the program and start entering numbers separated by whitespace (blanks, tabs, and newlines).
  Enter any non-numeric symbol to finnish entering. Program will print the median value out.

  \file
  \brief Implementation of the median functionality.
*/

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using Data = std::unique_ptr<std::vector<double>>;

Data read_data(std::istream& istream);
double median(const std::vector<double>* values);

/*!
  \brief Calls functions to read input data and compute median value.
         Prints output to screen.
   \return Zero upon successful completion.
*/
int main() {
    Data values = read_data(std::cin);
    std::cout << median(values.get());
    return 0;
}

/*!
  \brief Reads numeric data from input stream until non-numeric value is entered.
  \param istream Reference to input stream object, from which the entered by user
                 symbol is read.
  \return `std::vector` of entered values in double precision.
*/
Data read_data(std::istream& istream) {
    auto values = std::make_unique<std::vector<double>>();
    double value;
    while (istream >> value)
        values->push_back(value);
    return values;
}

/*!
  \brief Implementation of algorithm to compute median value of a sequence of numbers.
  \param values Pointer to constant `std::vector` that holds sequence of double precision data.
  \return Median value.
 */
double median(const std::vector<double>* values) {
    std::vector<double> tmp;
    std::copy(values->cbegin(), values->cend(), std::back_inserter(tmp));
    std::sort(tmp.begin(), tmp.end());
    size_t index = tmp.size()/2;
    if (tmp.size() % 2 == 0)
        return (tmp[index - 1] + tmp[index])/2.0;
    else
        return tmp[index];
}

/*!
  \brief Prints xdata to output stream.
  \param values Pointer to constant `std::vector` that holds sequence of double precision data.
  \return Void.
*/
void print_data(const std::vector<double>* values) {
    for (auto it = values->cbegin(); it != values->cend(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}
