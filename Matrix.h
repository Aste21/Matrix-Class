#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#pragma once

class Matrix
{

    friend Matrix operator+(const Matrix &lhs, const Matrix &rhs);
    friend Matrix operator-(const Matrix &lhs, const Matrix &rhs);
    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs);
    friend bool operator==(const Matrix &lhs, const Matrix &rhs);
    friend bool operator!=(const Matrix &lhs, const Matrix &rhs);

private:
    struct MatrixData;
    MatrixData *data;

public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const Matrix &other);
    ~Matrix();

    int getRows() const;
    int getColumns() const;
    int getReferenceCount() const;

    Matrix &operator=(const Matrix &other);
    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator*=(const Matrix &other);

    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
    friend std::istream &operator>>(std::istream &is, Matrix &matrix);

    Matrix &readMatrixFromFile(const std::string fileName);

    int operator()(int row, int col) const;
    int &operator()(int row, int col);

    void checkSizeOfMatrix(const Matrix &other) const;
    void checkSizeOfMatrixMultiplication(const Matrix &other) const;
    void checkIndex(int col, int row) const;

    class DifferentSizesOfMatrixesException
    {
    };
    class IndexOutOfRangeException
    {
    };
    class ReadFromFileErrorException
    {
    };
};

class WrongInputTypeException
{
};
