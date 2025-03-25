#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
class Matrix
{
  public:
    Matrix() : rows_(0), cols_(0)
    {
    }
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows * cols)
    {
    }

    Matrix(size_t rows, size_t cols, const T& initial_value)
        : rows_(rows), cols_(cols), data_(rows * cols, initial_value)
    {
    }

    size_t Rows() const
    {
        return rows_;
    }

    size_t Cols() const
    {
        return cols_;
    }

    const T& GetClamped(int x, int y) const
    {
        x = std::clamp(x, 0, static_cast<int>(rows_) - 1);
        y = std::clamp(y, 0, static_cast<int>(cols_) - 1);
        return operator()(x, y);
    }

    T& GetClamped(int x, int y)
    {
        x = std::clamp(x, 0, static_cast<int>(rows_) - 1);
        y = std::clamp(y, 0, static_cast<int>(cols_) - 1);
        return operator()(x, y);
    }

    T& operator()(size_t row, size_t col)
    {
        CheckIndex(row, col);
        return data_[row * cols_ + col];
    }

    const T& operator()(size_t row, size_t col) const
    {
        CheckIndex(row, col);
        return data_[row * cols_ + col];
    }

    void Resize(size_t new_rows, size_t new_cols, const T& fill_value = T())
    {
        std::vector<T> new_data(new_rows * new_cols, fill_value);
        size_t min_rows = std::min(rows_, new_rows);
        size_t min_cols = std::min(cols_, new_cols);
        for (size_t row = 0; row < min_rows; ++row)
        {
            for (size_t col = 0; col < min_cols; ++col)
            {
                new_data[row * new_cols + col] = operator()(row, col);
            }
        }
        rows_ = new_rows;
        cols_ = new_cols;
        data_.swap(new_data);
    }

  private:
    size_t rows_, cols_;
    std::vector<T> data_;

    void CheckIndex(size_t row, size_t col) const
    {
        if (row >= rows_ || col >= cols_)
        {
            throw std::runtime_error("Matrix subscript out of range");
        }
    }
};
