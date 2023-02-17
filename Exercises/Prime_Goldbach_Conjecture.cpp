#include <iostream>

bool isPrime(int m) 
{
    // check if int is less than 2
    if (m <= 1) {
        return false;
    }
    // from 2 to m-1, check if m is divisible by any number other than 1 or itself
    for (int i = 2; i < m; i++) {
        if (m % i == 0) {
            return false;
        }
    }
    return true;
}

// goldbach conjecture function definition
void goldbach(int m) 
{
    int i;
    int counter = 0;
    // check if int is less than 2
    if ((m < 1) || (m % 2 != 0)) {
        // std::cout << "Please enter a positive even integer greater than 2" << std::endl;
        throw std::invalid_argument("Invalid number");
    }
    else {
        if ((m > 0) && (m % 2 == 0)) {
            for (i = 2; i <= (m/2); i++) {
                if ((isPrime(i) == true) && (isPrime(m-i) == true)) {
                    counter += 1;
                    std::cout << m << "=" << i << "+" << (m-i) << std::endl;
                }
            }
            // if m cannot be expressed as a sum of primes
            if (counter == 0)
            {
                std::cout << "Countermeasure found" << std::endl;
            }
        }    
    }
}

int main() 
{
    int m = 0;
    std::cout << "Please enter a positive even integer greater than 2:" << std::endl;
    std::cin >> m;

    goldbach(m);
}
