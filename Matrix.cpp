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

    MatrixData(int numcols, int numrows) : cols(numcols), rows(numrows), referenceCount(1)
    {
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
        this->~MatrixData();
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
    data = new MatrixData(*other.data);
}

Matrix::~Matrix()
{
    if (--data->referenceCount == 0)
        delete data;
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
        other.data->referenceCount++;
        MatrixData *newData = new MatrixData(*(other.data));
        if((data->referenceCount-1) == 0)
        {
            delete data;
        }
        data = newData;
    }
    return *this;
}

Matrix operator+(const Matrix &lhs, const Matrix &rhs)
{
    if (!lhs.data->isEqualSize(*rhs.data))
    {
        throw WrongSizeOfMatrixException();
    }
    Matrix result = lhs;
    for (int i = 0; i < result.data->cols; i++)
    {
        for (int j = 0; j < result.data->rows; j++)
        {
            result.data->elements[i][j] += rhs.data->elements[i][j];
        }
    }
    return result;
}

Matrix operator-(const Matrix &lhs, const Matrix &rhs)
{
    if (!lhs.data->isEqualSize(*rhs.data))
    {
        throw WrongSizeOfMatrixException();
    }
    Matrix result = lhs;
    for (int i = 0; i < result.data->cols; i++)
    {
        for (int j = 0; j < result.data->rows; j++)
        {
            result.data->elements[i][j] -= rhs.data->elements[i][j];
        }
    }
    return result;
}

Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
    if (lhs.getColumns() != rhs.getRows())
    {
        throw WrongSizeOfMatrixException();
    }

    Matrix result(lhs.getRows(), rhs.getColumns());

    for (int i = 0; i < lhs.getRows(); ++i)
    {
        for (int j = 0; j < rhs.getColumns(); ++j)
        {
            for (int k = 0; k < lhs.getColumns(); ++k)
            {
                result.data->elements[i][j] += lhs.data->elements[i][k] * rhs.data->elements[k][j];
            }
        }
    }

    return result;
}


Matrix &Matrix::operator+=(const Matrix &other)
{
    if (!data->isEqualSize(*other.data))
    {
        throw WrongSizeOfMatrixException();
    }
    for (int i = 0; i < other.data->cols; i++)
    {
        for (int j = 0; j < other.data->rows; j++)
        {
            data->elements[i][j] += other.data->elements[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    if (!data->isEqualSize(*other.data))
    {
        throw WrongSizeOfMatrixException();
    }
    for (int i = 0; i < other.data->cols; i++)
    {
        for (int j = 0; j < other.data->rows; j++)
        {
            data->elements[i][j] -= other.data->elements[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
    if (getColumns() != other.getRows())
    {
        throw WrongSizeOfMatrixException();
    }

    Matrix result(getRows(), other.getColumns());

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < other.getColumns(); ++j)
        {
            for (int k = 0; k < getColumns(); ++k)
            {
                result.data->elements[i][j] += this->data->elements[i][k] * other.data->elements[k][j];
            }
        }
    }

    *this = result;

    return *this;
}


bool operator==(const Matrix& lhs, const Matrix& rhs)
{
    if(!lhs.data->isEqualSize(*rhs.data))
    {
        return false;
    }
    for(int i =0;i < lhs.data->cols;i++)
    {
        for(int j = 0;j < lhs.data->rows;j++)
        {
            if(lhs.data->elements[i][j] != rhs.data->elements[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
bool operator!=(const Matrix& lhs, const Matrix& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (int i = 0; i < matrix.data->cols; ++i) {
        for (int j = 0; j < matrix.data->rows; ++j) {
            os << matrix.data->elements[i][j] << ' ';
        }
        os << '\n';
    }
    return os;
}

std::istream &operator>>(std::istream &is, Matrix &matrix)
{
    int cols, rows;

    std::cout<<"Input number of rows: ";
    is >> rows;
    std::cout<<"\nInput number of columns: ";
    is >> cols;

    Matrix temp(cols, rows);

    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            is >> temp.data->elements[i][j];
        }
    }

    matrix = temp;

    return is;
}


int Matrix::operator()(int row, int col) const
{
    return data->elements[row-1][col-1];
}

int &Matrix::operator()(int row, int col)
{
    return data->elements[row-1][col-1];
}

int Matrix::getRows() const
{
    return data->cols;
}

int Matrix::getColumns() const
{
    return data->rows;
}