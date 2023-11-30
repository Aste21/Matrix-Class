#include "TestMatrix.h"

int main()
{
    int numberOfFailedTests = 0;
    try
    {
        testInputFromStream();
        numberOfFailedTests += testCopyConstructor();
        numberOfFailedTests += testAssignmentOperator();
        numberOfFailedTests += testAdditionOperator();
        numberOfFailedTests += testSubstractionOperator();
        numberOfFailedTests += testMultiplicationOperator();
        numberOfFailedTests += testComparisonOperators();
        numberOfFailedTests += testAdditionAssignmentOperators();
        numberOfFailedTests += testSubstracionAssignmentOperators();
        numberOfFailedTests += testMultiplicationAssignmentOperators();
        numberOfFailedTests += testReferenceCounting();
        numberOfFailedTests += testSingleElementAccess();
        numberOfFailedTests += testIndexOutOfRangeException();
        numberOfFailedTests += testWrongInputTypeException();
        numberOfFailedTests += testDifferentSizesOfMatrixesException();
        if (numberOfFailedTests > 0)
        {
            std::cout << "The tests were unsuccesful! - Number of failed tests: " << numberOfFailedTests;
        }
        else
        {
            std::cout << "The tests were succesful!";
        }
    }
    catch (const Matrix::DifferentSizesOfMatrixesException &e)
    {
        std::cerr << "Error: Different sizes of matrices.\n";
    }
    catch (const Matrix::IndexOutOfRangeException &e)
    {
        std::cerr << "Error: Index out of range.\n";
    }
    catch (const WrongInputTypeException &e)
    {
        std::cerr << "Error: Wrong input type.\n";
    }
    catch (const Matrix::ReadFromFileErrorException &e)
    {
        std::cerr << "Error: Read from file failed.\n";
    }

    return 0;
}
