#include "Matrix.h"

int main()
{
    
    Matrix a;
    Matrix b;
    std::cout << "Input matrix a: \n";
    std::cin >> a;
    std::cout << "\nInput matrix b: \n";
    std::cin >> b;

    b(2, 1) = 123;

    std::cout << "\nMatrix a: \n";
    std::cout << a << std::endl;
    std::cout << "\nMatrix b: \n";
    std::cout << b << std::endl;

    std::cout<< std::endl << std::endl;

    std::cout << a*b;

    
}