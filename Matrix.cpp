#include "Matrix.h"

struct Matrix::MatrixData
{
public:
    int **elements;
    int cols;
    int rows;
    int referenceCount;

    MatrixData(int value) : cols(1), rows(1), referenceCount(1)
    {
        elements = new int *[1];
        elements[0] = new int[1];
        elements[0][0] = value;
    }

    MatrixData(int numcols, int numrows) : cols(numcols), rows(numrows), referenceCount(0)
    {
        if (numcols > 0 && numrows > 0)
        {
            referenceCount += 1;
            elements = new int *[cols];
            for (int i = 0; i < cols; ++i)
            {
                elements[i] = new int[rows];
                for (int j = 0; j < rows; ++j)
                {
                    elements[i][j] = 0;
                }
            }
        }
        else
        {
            elements = NULL;
        }
    }

    MatrixData(int numcols, int numrows, int secondReferenceCount, int **otherElements) : cols(numcols), rows(numrows), referenceCount(secondReferenceCount)
    {
        elements = new int *[cols];
        for (int i = 0; i < cols; ++i)
        {
            elements[i] = new int[rows];
            for (int j = 0; j < rows; ++j)
            {
                elements[i][j] = otherElements[i][j];
            }
        }
    }

    MatrixData(const MatrixData &other) : cols(other.cols), rows(other.rows), referenceCount(other.referenceCount)
    {
        elements = new int *[cols];
        for (int i = 0; i < cols; ++i)
        {
            elements[i] = new int[rows];
            for (int j = 0; j < rows; ++j)
            {
                elements[i][j] = other.elements[i][j];
            }
        }
    }

    ~MatrixData()
    {
        for (int i = 0; i < cols; ++i)
        {
            delete[] elements[i];
        }
        delete[] elements;
    }

    MatrixData &operator=(const MatrixData &other)
    {
        if (this == &other)
        {
            return *this;
        }
        delete this;
        cols = other.cols;
        rows = other.rows;
        elements = new int *[cols];
        for (int i = 0; i < cols; ++i)
        {
            elements[i] = new int[rows];
            for (int j = 0; j < rows; ++j)
            {
                elements[i][j] = other.elements[i][j];
            }
        }
        return *this;
    }

    bool isEqualSize(const MatrixData &other)
    {
        if (cols != other.cols || rows != other.rows)
        {
            return false;
        }
        return true;
    }

    MatrixData *detach()
    {
        if (referenceCount == 1)
            return this;
        MatrixData *t = new MatrixData(cols, rows);
        referenceCount--;
        return t;
    }

private:
};

Matrix::CrefMatrix::CrefMatrix(Matrix *matrixNew, int xNew, int yNew) : matrix(matrixNew), x(xNew), y(yNew){};

Matrix::CrefMatrix::operator double()
{
    matrix->data = matrix->data->detach();
    return matrix->data->elements[x - 1][y - 1];
}

Matrix::CrefMatrix &Matrix::CrefMatrix::operator=(double value)
{
    if (matrix->data->referenceCount > 1)
    {
        matrix->data = matrix->data->detach();
    }
    matrix->data->elements[x - 1][y - 1] = value;
    return *this;
}
Matrix::Matrix()
{
    data = new MatrixData(0, 0);
}

Matrix::Matrix(int cols, int rows)
{
    data = new MatrixData(cols, rows);
}

Matrix::Matrix(const Matrix &other)
{
    other.data->referenceCount++;
    data = other.data;
}

Matrix::~Matrix()
{
    if (--data->referenceCount <= 0)
        delete data;
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
        other.data->referenceCount++;
        if (data->referenceCount == 0)
        {
            delete data;
        }
        else if (--data->referenceCount == 0)
        {
            delete data;
        }
        data = other.data;
    }
    return *this;
}

Matrix operator+(const Matrix &lhs, const Matrix &rhs)
{
    return Matrix(lhs) += rhs;
}

Matrix operator-(const Matrix &lhs, const Matrix &rhs)
{
    return Matrix(lhs) -= rhs;
}

Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
    return Matrix(lhs) *= rhs;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    checkSizeOfMatrix(other);
    MatrixData *result = new MatrixData(getRows(), getColumns(), 1, data->elements);
    for (int i = 0; i < other.data->cols; i++)
    {
        for (int j = 0; j < other.data->rows; j++)
        {
            (*result).elements[i][j] += other.data->elements[i][j];
        }
    }
    if (--data->referenceCount == 0)
    {
        delete data;
    }
    data = result;
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    checkSizeOfMatrix(other);
    MatrixData *result = new MatrixData(getRows(), getColumns(), 1, data->elements);
    for (int i = 0; i < other.data->cols; i++)
    {
        for (int j = 0; j < other.data->rows; j++)
        {
            (*result).elements[i][j] -= other.data->elements[i][j];
        }
    }
    if (--data->referenceCount == 0)
    {
        delete data;
    }
    data = result;
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other)
{
    checkSizeOfMatrix(other);

    MatrixData *result = new MatrixData(getRows(), getColumns(), 1, data->elements);

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < other.getColumns(); ++j)
        {
            for (int k = 0; k < getColumns(); ++k)
            {
                (*result).elements[i][j] += this->data->elements[i][k] * other.data->elements[k][j];
            }
        }
    }

    if (--data->referenceCount == 0)
    {
        delete data;
    }

    data = result;

    return *this;
}

bool operator==(const Matrix &lhs, const Matrix &rhs)
{
    if (!lhs.data->isEqualSize(*rhs.data))
    {
        return false;
    }
    for (int i = 0; i < lhs.data->cols; i++)
    {
        for (int j = 0; j < lhs.data->rows; j++)
        {
            if (lhs.data->elements[i][j] != rhs.data->elements[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Matrix &lhs, const Matrix &rhs)
{
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    for (int i = 0; i < matrix.data->cols; ++i)
    {
        for (int j = 0; j < matrix.data->rows; ++j)
        {
            os << matrix.data->elements[i][j] << ' ';
        }
        os << '\n';
    }
    return os;
}

std::istream &operator>>(std::istream &is, Matrix &matrix)
{
    int cols, rows;

    std::cout << "Input number of rows: ";
    if (!(is >> rows))
    {
        throw WrongInputTypeException();
    }
    std::cout << "Input number of columns: ";
    if (!(is >> cols))
    {
        throw WrongInputTypeException();
    }

    Matrix temp(cols, rows);

    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            if (!(is >> temp.data->elements[i][j]))
            {
                throw WrongInputTypeException();
            }
        }
    }

    matrix = temp;

    return is;
}

Matrix &Matrix::readMatrixFromFile(const std::string fileName)
{
    std::ifstream inputFile(fileName);
    int cols, rows;

    if (!inputFile.is_open())
    {
        throw ReadFromFileErrorException();
    }

    if (!(inputFile >> rows >> cols))
    {
        throw WrongInputTypeException();
    }

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (!(inputFile >> result.data->elements[i][j]))
            {
                throw WrongInputTypeException();
            }
        }
    }

    inputFile.close();

    *this = result;

    return *this;
}

int Matrix::operator()(int row, int col) const
{
    checkIndex(col, row);
    return data->elements[row - 1][col - 1];
}

Matrix::CrefMatrix Matrix::operator()(int row, int col)
{
    checkIndex(col, row);
    return CrefMatrix(this, row, col);
}

int Matrix::getRows() const
{
    return data->cols;
}

int Matrix::getColumns() const
{
    return data->rows;
}

int Matrix::getReferenceCount() const
{
    return data->referenceCount;
}

void Matrix::checkSizeOfMatrix(const Matrix &other) const
{
    if (getColumns() != other.getColumns() || getRows() != other.getRows())
    {
        throw DifferentSizesOfMatrixesException();
    }
}

void Matrix::checkSizeOfMatrixMultiplication(const Matrix &other) const
{
    if (getColumns() != other.getRows())
    {
        throw DifferentSizesOfMatrixesException();
    }
}

void Matrix::checkIndex(int col, int row) const
{
    if (row > data->rows || row <= 0 || col > data->cols || col <= 0)
    {
        throw IndexOutOfRangeException();
    }
}
