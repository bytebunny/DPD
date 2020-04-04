#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using Data = std::unique_ptr<std::vector<double>>;

Data read_data(std::istream& istream);
double median(const std::vector<double>* values);

int main() {
    Data values = read_data(std::cin);
    std::cout << median(values.get());
    return 0;
}

Data read_data(std::istream& istream) {
    auto values = std::make_unique<std::vector<double>>();
    double value;
    while (istream >> value)
        values->push_back(value);
    return values;
}

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

void print_data(const std::vector<double>* values) {
    for (auto it = values->cbegin(); it != values->cend(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}
