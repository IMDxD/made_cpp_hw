#pragma once
#include <vector>
#include <iostream>
#include "algorithm"
#include "cmath"

using std::vector;

namespace task {

const double TOL = 1e-12;

vector<double> operator+(const vector<double> &source) {
  vector<double> result = source;
  return result;
}

vector<double> operator-(const vector<double> &source) {
  vector<double> result(source.size());
  for (int i = 0; i < source.size(); ++i) {
    result[i] = -source[i];
  }
  return result;
}

vector<double> operator+(const vector<double> &left, const vector<double> &right) {
  vector<double> result(left.size());
  for (int i = 0; i < left.size(); ++i) {
    result[i] = left[i] + right[i];
  }
  return result;
}

vector<double> operator-(const vector<double> &left, const vector<double> &right) {
  vector<double> result(left.size());
  for (int i = 0; i < left.size(); ++i) {
    result[i] = left[i] - right[i];
  }
  return result;
}

double operator*(const vector<double> &left, const vector<double> &right) {
  double result = 0;
  for (int i = 0; i < right.size(); ++i) {
    result += left[i] * right[i];
  }
  return result;
}

vector<double> operator%(const vector<double> &left, const vector<double> &right) {
  vector<double> result(3);
  result[0] = left[1] * right[2] - left[2] * right[1];
  result[1] = left[2] * right[0] - left[0] * right[2];
  result[2] = left[0] * right[1] - left[1] * right[0];
  return result;
}

bool operator||(const vector<double> &left, const vector<double> &right) {
  bool flag = false;
  double fraction;
  for (size_t i = 0; i < left.size(); ++i) {
    if (left[i] && right[i]) {
      double fraction_dif = left[i] / right[i] - fraction;
      if (flag && (fabs(fraction_dif) > TOL)) {
        return false;
      } else if (!flag) {
        fraction = left[i] / right[i];
        flag = true;
      }
    } else if (left[i] || right[i]) {
      return false;
    }
  }
  return true;
}

bool operator&&(const vector<double> &left, const vector<double> &right) {
  return (left || right) && (left * right > 0);
}

std::istream &operator>>(std::istream &in, vector<double> &source) {
  size_t size;
  in >> size;
  source.resize(size);
  for (size_t i = 0; i < size; ++i) {
    in >> source[i];
  }
  return in;
}

std::ostream &operator<<(std::ostream &out, const vector<double> &source) {
  for (size_t i = 0; i < source.size(); ++i) {
    out << source[i];
    if (i < source.size() - 1) {
      out << ' ';
    }
  }
  out << std::endl;
  return out;
}

void reverse(vector<double> &source) {
  for (size_t i = 0; i < source.size() / 2; ++i) {
    double temp = source[i];
    source[i] = source[source.size() - i - 1];
    source[source.size() - i - 1] = temp;
  }
}

vector<int> operator|(const vector<int> &left, const vector<int> &right) {
  vector<int> result(left.size());
  for (int i = 0; i < left.size(); ++i) {
    result[i] = left[i] | right[i];
  }
  return result;
}

vector<int> operator&(const vector<int> &left, const vector<int> &right) {
  vector<int> result(left.size());
  for (int i = 0; i < left.size(); ++i) {
    result[i] = left[i] & right[i];
  }
  return result;
}
}
