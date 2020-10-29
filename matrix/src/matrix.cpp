#include "matrix.h"
#include "vector"

using namespace task;

bool check_bounds(const size_t &source_rows, const size_t &source_cols, const size_t &query_row,
                  const size_t &query_col) {
  return (query_row < source_rows && query_col < source_cols);
}

bool check_size(const size_t &source_rows, const size_t &source_cols, const size_t &target_rows,
                const size_t &target_cols) {
  return (source_rows == target_rows && source_cols == target_cols);
}

size_t Matrix::n_rows() const {
  return this->rows;
}

size_t Matrix::n_cols() const {
  return this->cols;
}

Matrix::Matrix() {
  this->rows = 1;
  this->cols = 1;
  this->array = new double *[1];
  *(this->array + 0) = new double{1};
}

Matrix::Matrix(size_t row, size_t col) {
  this->rows = row;
  this->cols = col;
  this->array = new double *[this->rows];
  for (size_t i = 0; i < this->rows; ++i) {
    *(this->array + i) = new double[this->cols];
    for (size_t j = 0; j < this->cols; ++j) {
      if (i == j) {
        *(*(this->array + i) + j) = 1;
      } else {
        *(*(this->array + i) + j) = 0;
      }
    }
  }
}

Matrix::~Matrix() {
  for (size_t i = 0; i < this->rows; ++i) {
    delete[] *(this->array + i);
  }
  delete[] this->array;
}

Matrix::Matrix(const Matrix &copy) {
  this->rows = copy.rows;
  this->cols = copy.cols;
  this->array = new double *[this->rows];
  for (size_t i = 0; i < this->rows; ++i) {
    *(this->array + i) = new double[this->cols];
    for (size_t j = 0; j < this->cols; ++j) {
      *(*(this->array + i) + j) = *(*(copy.array + i) + j);
    }
  }
}

Matrix &Matrix::operator=(const Matrix &a) {
  if (this == &a) {
    return *this;
  }
  for (size_t i = 0; i < this->rows; ++i) {
    delete[] *(this->array + i);
  }
  delete[] this->array;
  this->rows = a.rows;
  this->cols = a.cols;
  this->array = new double *[this->rows];
  for (size_t i = 0; i < this->rows; ++i) {
    *(this->array + i) = new double[this->cols];
    for (size_t j = 0; j < this->cols; ++j) {
      *(*(this->array + i) + j) = *(*(a.array + i) + j);
    }
  }
  return *this;
}

double &Matrix::get(size_t row, size_t col) {
  if (!check_bounds(this->rows, this->cols, row, col)) {
    throw OutOfBoundsException();
  }
  return (*this)[row][col];
}

const double &Matrix::get(size_t row, size_t col) const {
  if (!check_bounds(this->rows, this->cols, row, col)) {
    throw OutOfBoundsException();
  }
  auto *tmp = (*this)[row];
  auto val = new double[1]{*(tmp + col)};
  delete[] tmp;
  return *val;
}

void Matrix::set(size_t row, size_t col, const double &value) {
  if (!check_bounds(this->rows, this->cols, row, col)) {
    throw OutOfBoundsException();
  }
  (*this)[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  auto **new_array = new double *[new_rows];
  size_t cur_i;
  size_t cur_j;
  for (size_t i = 0; i < new_rows; ++i) {
    *(new_array + i) = new double[new_cols];
    for (size_t j = 0; j < new_cols; ++j) {
      cur_i = (i * new_cols + j) / this->cols;
      cur_j = (i * new_cols + j) % this->cols;
      if (cur_i < this->rows) {
        *(*(new_array + i) + j) = *(*(this->array + cur_i) + cur_j);
      } else {
        *(*(new_array + i) + j) = 0;
      }
    }
  }
  for (size_t i = 0; i < this->rows; ++i) {
    delete[] *(this->array + i);
  }
  delete[] this->array;
  this->array = new_array;
  this->rows = new_rows;
  this->cols = new_cols;
}

double *Matrix::operator[](size_t row) {
  return *(this->array + row);
}

double *Matrix::operator[](size_t row) const {
  auto *row_copy = new double[this->cols];
  for (size_t i = 0; i < this->cols; ++i) {
    *(row_copy + i) = *(*(this->array + row) + i);
  }
  return row_copy;
}

Matrix &Matrix::operator+=(const Matrix &a) {
  if (!check_size(this->rows, this->cols, a.rows, a.cols)) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      *(*(this->array + i) + j) += *(*(a.array + i) + j);
    }
  }
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &a) {
  return *this += -a;
}

Matrix &Matrix::operator*=(const Matrix &a) {
  if (this->cols != a.rows) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < this->rows; ++i) {
    auto *new_row = new double[a.cols];
    for (size_t j = 0; j < a.cols; ++j) {
      double value = 0;
      for (size_t k = 0; k < this->cols; ++k) {
        value += this->get(i, k) * a.get(k, j);
      }
      *(new_row + j) = value;
    }
    delete[] *(this->array + i);
    *(this->array + i) = new_row;
  }
  this->cols = a.cols;
  return *this;
}

Matrix &Matrix::operator*=(const double &number) {
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      *(*(this->array + i) + j) *= number;
    }
  }
  return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
  Matrix tmp_matrix = *this;
  return tmp_matrix += a;
}

Matrix Matrix::operator-(const Matrix &a) const {
  Matrix tmp_matrix = *this;
  return tmp_matrix -= a;
}

Matrix Matrix::operator*(const Matrix &a) const {
  Matrix tmp_matrix = *this;
  return tmp_matrix *= a;
}

Matrix Matrix::operator*(const double &a) const {
  Matrix tmp_matrix = *this;
  return tmp_matrix *= a;
}

Matrix Matrix::operator-() const {
  Matrix tmp_matrix = *this;
  tmp_matrix *= -1.0;
  return tmp_matrix;
}

Matrix Matrix::operator+() const {
  Matrix tmp_matrix = *this;
  return tmp_matrix;
}

Matrix Matrix::get_minor(size_t row, size_t col) const {
  Matrix tmp_matrix(this->rows - 1, this->cols - 1);
  size_t new_i;
  size_t new_j;
  for (size_t i = 0; i < this->rows; ++i) {
    if (i == row) {
      continue;
    }
    for (size_t j = 0; j < this->cols; ++j) {
      if (j == col) {
        continue;
      }
      new_i = i;
      new_j = j;
      if (i > row) {
        --new_i;
      }
      if (j > col) {
        --new_j;
      }
      tmp_matrix.set(new_i, new_j, this->get(i, j));
    }
  }
  return tmp_matrix;
}

double Matrix::det() const {
  if (this->rows != this->cols) {
    throw SizeMismatchException();
  }
  if (this->rows == 1) {
    return this->get(0, 0);
  } else if (this->rows == 2) {
    return this->get(0, 0) * this->get(1, 1) - this->get(1, 0) * this->get(0, 1);
  } else {
    double d = 0;
    for (size_t i = 0; i < this->cols; ++i) {
      Matrix tmp_matrix = get_minor(0, i);
      if (i % 2 == 0) {
        d += this->get(0, i) * tmp_matrix.det();
      } else {
        d -= this->get(0, i) * tmp_matrix.det();
      }
    }
    return d;
  }
}

void Matrix::transpose() {
  auto new_array = new double *[this->cols];
  for (size_t j = 0; j < this->cols; ++j) {
    *(new_array + j) = new double[this->rows];
    for (size_t i = 0; i < this->rows; ++i) {
      *(*(new_array + j) + i) = *(*(this->array + i) + j);
    }
  }
  for (size_t i = 0; i < this->rows; ++i) {
    delete[] *(this->array + i);
  }
  delete[] this->array;
  this->array = new_array;
  size_t tmp = this->rows;
  this->rows = this->cols;
  this->cols = tmp;
}

Matrix Matrix::transposed() const {
  Matrix tmp_matrix = *this;
  tmp_matrix.transpose();
  return tmp_matrix;
}

double Matrix::trace() const {
  if (this->rows != this->cols) {
    throw SizeMismatchException();
  }
  double t = 0;
  for (size_t i = 0; i < this->rows; i++) {
    t += (*this).get(i, i);
  }
  return t;
}

std::vector<double> Matrix::getRow(size_t row) {
  std::vector<double> row_vector(*(this->array + row), *(this->array + row) + this->cols);
  return row_vector;
}

std::vector<double> Matrix::getColumn(size_t column) {
  std::vector<double> col_vector(this->rows);
  for (size_t i = 0; i < this->rows; ++i) {
    col_vector.push_back((*this).get(i, column));
  }
  return col_vector;
}

std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
  size_t rows;
  size_t cols;
  double value;
  input >> rows >> cols;
  matrix.resize(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      input >> value;
      matrix.set(i, j, value);
    }
  }
  return input;
}

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
  output.precision(9);
  for (size_t i = 0; i < matrix.n_rows(); ++i) {
    for (size_t j = 0; j < matrix.n_cols(); ++j) {
      output << matrix.get(i, j);
      if (j < matrix.n_cols() - 1) {
        output << " ";
      }
    }
    output << "\n";
  }
  return output;
}

bool Matrix::operator==(const Matrix &a) const {
  if (!check_size(this->rows, this->cols, a.rows, a.cols)) {
    return false;
  }
  double diff;
  for (size_t i = 0; i < this->rows; ++i) {
    for (size_t j = 0; j < this->cols; ++j) {
      diff = this->get(i, j) - a.get(i, j);
      if (diff > EPS || -diff > EPS) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix::operator!=(const Matrix &a) const {
  return !(*this == a);
}

Matrix task::operator*(const double &a, const Matrix &b) {
  return b * a;
}