#include <iostream>
#include <iomanip>
#include <cmath>

const double PI = 3.141592653589793238463;

double Pi1(int nTerm)  
{
    double Pi = 0;
    if (nTerm < 0)
    {
        throw std::invalid_argument("Invalid number");
    }
    // base case
    if (nTerm == 0)
    {
        Pi = 4*(pow(-1,nTerm) / (2*nTerm + 1));
    }
    else
    {
        Pi = 4*(pow(-1,nTerm) / (2*nTerm + 1)) + Pi1(nTerm - 1);
    }
    // alt solution
    // Pi = nTerm == 0 ? 4*(pow(-1,nTerm) / (2*nTerm + 1)) : 4*(pow(-1,nTerm) / (2*nTerm + 1)) + Pi1(nTerm - 1);
    return Pi;
}

double Pi2(int nTerm)
{
    double value = 0;
    if (nTerm < 0)
    {
        throw std::invalid_argument("Invalid number");
    }
    while (nTerm != 0)
    {
        value += (pow(-1,nTerm - 1) / (2*nTerm * (2*nTerm + 1) * (2*nTerm+2)));
        nTerm -= 1;
    }
    return 3+4*value;
}

double arctan(int nTerm, double x)
{
    double value = 0;
    int k = 0;
    do
    {
        value += ((pow(-1, k) * pow(x, 2*k + 1)) / (2*k+ 1));
        k += 1;
    }
    while (k <= nTerm);
    return value;
}

double Pi3(int nTerm)
{
    if (nTerm < 0)
    {
        throw std::invalid_argument("Invalid number");
    }
    return 4*(4*arctan(nTerm, 1/(double)5) - arctan(nTerm, 1/(double)239));
}

int main()
{
    int nTerm;
    std::cout << "Please enter number of terms:" << std::endl;
    std::cin >> nTerm;
    std::cout << std::setprecision(21);
    std::cout << "Pi1 approximation error: " << Pi1(nTerm) - PI << std::endl;
    std::cout << "Pi2 approximation error: " << Pi2(nTerm) - PI << std::endl;
    std::cout << "Pi3 approximation error: " << Pi3(nTerm) - PI << std::endl;
    return 0;
}