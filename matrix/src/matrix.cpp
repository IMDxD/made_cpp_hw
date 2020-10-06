#include "matrix.h"
#include "vector"

using namespace task;

bool check_bounds(const size_t &source_rows, const size_t &source_cols, const size_t &query_row, 
                         const size_t &query_col){
  return (source_rows < query_row & source_cols < query_col);
}

bool check_size(const size_t &source_rows, const size_t &source_cols, const size_t &target_rows,
                         const size_t &target_cols){
  return (source_rows == target_rows & source_cols == target_cols);
}

Matrix::Matrix() {
  rows = 1;
  cols = 1;
  delete[] array;
  array = new double[1]{1};
}

Matrix::Matrix(size_t row, size_t col) {
  rows = row;
  cols = col;
  delete[] array;
  array = new double[rows * cols];
}

Matrix::Matrix(const Matrix &copy) {
  rows = copy.rows;
  cols = copy.cols;
  delete[] array;
  array = new double[rows * cols];
  for (size_t i = 0; i < rows * cols; ++i) {
    *(array + i) = *(copy.array + i);
  }
}

Matrix & Matrix::operator=(const Matrix &a) {
  if (a == *this) {
    return *this;
  }
  rows = a.rows;
  cols = a.cols;
  delete[] array;
  array = new double[rows * cols];
  for (size_t i = 0; i < rows * cols; ++i) {
    *(array + i) = *(a.array + i);
  }
  return *this;
}

double &Matrix::get(size_t row, size_t col) {
  return *(array + row * cols + col);
}

const double &Matrix::get(size_t row, size_t col) const {
  if (check_bounds(rows, cols, row, col)){
    throw OutOfBoundsException();
  }
  auto * value = new double[1];
  *(value) = *(array + row * cols + col);
  return *(value);
}

void Matrix::set(size_t row, size_t col, const double &value) {
  if (check_bounds(rows, cols, row, col)){
    throw OutOfBoundsException();
  }
  *(array + row * cols + col) = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  auto * new_array = new double [new_rows * new_cols];
  size_t min_size = new_rows * new_cols;
  if (min_size > cols * rows){
    min_size = cols * rows;
  }
  for (size_t i = 0; i < min_size; ++i){
    *(new_array + i) = *(array + i);
  }
  if (new_rows * new_cols > cols * rows){
    for (size_t i = min_size; i < new_rows * new_cols; ++i){
      *(new_array + i) = 0;
    }
  }
  rows = new_rows;
  cols = new_cols;
  delete [] array;
  array = new_array;
}

double * Matrix::operator[](size_t row) {
  return array + row * cols;
}

double * Matrix::operator[](size_t row) const {
  auto *row_copy = new double[cols];
  for (size_t i = 0; i < cols; ++i) {
    *(row_copy + i) = *(array + row * cols + i);
  }
  return row_copy;
}

Matrix & Matrix::operator+=(const Matrix &a) {
  if (check_size(this -> rows, this -> cols, a.rows, a.cols)){
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < a.rows * a.cols; ++i){
    *(this -> array + i) += *(a.array + i);
  }
  return *this;
}

Matrix & Matrix::operator-=(const Matrix &a) {
  return *this += -a;
}

Matrix & Matrix::operator*=(const Matrix &a) {
  if (check_size(this -> rows, this -> cols, a.rows, a.cols)){
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < a.rows * a.cols; ++i){
    *(this -> array + i) *= *(a.array + i);
  }
  return *this;
}

Matrix & Matrix::operator*=(const double &number) {
  for (size_t i = 0; i < rows * cols; ++i){
    *(array + i) *= number;
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
  Matrix new_matrix = *this;
  for (size_t i = 0; i < new_matrix.rows * new_matrix.cols; ++i){
    *(new_matrix.array + i) = -*(this->array + i);
  }
  return new_matrix;
}

bool Matrix::operator==(const Matrix &a) const {

  return true;
}
