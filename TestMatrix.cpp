#include "TestMatrix.h"

void testInputFromStream()
{
    Matrix a;
    std::cout << "Enter values for matrix 'a':\n";
    std::cin >> a;
    std::cout << "Matrix a after input:\n"
              << a << std::endl;
}

int testCopyConstructor()
{
    Matrix a;
    a.readMatrixFromFile("ExampleMatrixA.txt");

    Matrix b(a);
    if (b != a)
    {
        std::cout << "Copy constructor test failed.\n";
        return 1;
    }
    return 0;
}

int testAssignmentOperator()
{
    Matrix a(3, 3);
    a.readMatrixFromFile("ExampleMatrixA.txt");

    Matrix b = a;
    if (b != a)
    {
        std::cout << "Copy constructor test failed.\n";
        return 1;
    }
    return 0;
}

int testAdditionOperator()
{
    Matrix a, b, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    if (a + b != result.readMatrixFromFile("ResultOfAddition.txt"))
    {
        std::cout << "Addition operator test failed.\n";
        return 1;
    }
    return 0;
}

int testSubstractionOperator()
{
    Matrix a, b, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    if (a - b != result.readMatrixFromFile("ResultOfSubstraction.txt"))
    {
        std::cout << "Substraction operator test failed.\n";
        return 1;
    }
    return 0;
}

int testMultiplicationOperator()
{
    Matrix a, b, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    if (a * b != result.readMatrixFromFile("ResultOfMultiplication.txt"))
    {
        std::cout << "Multiplication operator test failed.\n";
        return 1;
    }
    return 0;
}

int testComparisonOperators()
{
    Matrix a, b;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    if (a == b || !(a != b))
    {
        std::cout << "Comparison operator test failed.\n";
        return 1;
    }

    return 0;
}

int testAdditionAssignmentOperators()
{
    Matrix a, b, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    a += b;
    if (a != result.readMatrixFromFile("ResultOfAddition.txt"))
    {
        std::cout << "Addition assignment operator test failed.\n";
        return 1;
    }
    return 0;
}

int testSubstracionAssignmentOperators()
{
    Matrix a, b, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    a -= b;
    if (a != result.readMatrixFromFile("ResultOfSubstraction.txt"))
    {
        std::cout << "Substraction assignment operator test failed.\n";
        return 1;
    }
    return 0;
}

int testMultiplicationAssignmentOperators()
{
    Matrix a, b, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    b.readMatrixFromFile("ExampleMatrixB.txt");

    a *= b;
    if (a != result.readMatrixFromFile("ResultOfMultiplication.txt"))
    {
        std::cout << "Multiplication assignment operator test failed.\n";
        return 1;
    }
    return 0;
}

int testReferenceCounting()
{
    Matrix a(2, 2);
    if (a.getReferenceCount() != 1)
    {
        std::cout << "Reference counting test failed.\n";
        return 1;
    }
    Matrix b(a);

    if (a.getReferenceCount() != 2)
    {
        std::cout << "Reference counting test failed.\n";
        return 1;
    }

    Matrix c;
    c = a;

    if (a.getReferenceCount() != 3)
    {
        std::cout << "Reference counting test failed.\n";
        return 1;
    }
    a += b;
    if (a.getReferenceCount() != 1 || b.getReferenceCount() != 2 || c.getReferenceCount() != 2)
    {
        std::cout << "Reference counting test failed.\n";
        return 1;
    }
    return 0;
}

int testSingleElementAccess()
{
    Matrix a, result;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    a(2, 2) = 9;
    a(3, 1) = 69;
    if (a != result.readMatrixFromFile("ResultOfSingleElementAccess.txt"))
    {
        std::cout << "Single element access test failed.\n";
        return 1;
    }
    return 0;
}

int testIndexOutOfRangeException()
{
    Matrix a;
    a.readMatrixFromFile("ExampleMatrixA.txt");
    try
    {
        a(10, 20) = 9;
        std::cout << "IndexOutOfRangeException test failed.\n";
        return 1;
    }
    catch (const Matrix::IndexOutOfRangeException &e)
    {
        return 0;
    }
}

int testWrongInputTypeException()
{
    try
    {
        Matrix c;
        c.readMatrixFromFile("ExampleMatrixC.txt");
        std::cout << "WrongInputTypeException test failed.\n";
        return 1;
    }
    catch (const WrongInputTypeException &e)
    {
        return 0;
    }
}

int testDifferentSizesOfMatrixesException()
{
    try
    {
        Matrix a, d;
        a.readMatrixFromFile("ExampleMatrixA.txt");
        d.readMatrixFromFile("ExampleMatrixD.txt");
        a = a + d;
        std::cout << "DifferentSizesOfMatrixesException test failed.\n";
        return 1;
    }
    catch (const Matrix::DifferentSizesOfMatrixesException &e)
    {
        return 0;
    }
}
