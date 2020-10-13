#pragma once
#include "vector"
#include "cmath"

struct Point;
struct Line;
class Shape;
class Polygon;
class Ellipse;
class Circle;
class Rectangle;
class Square;

struct Point {

 public:
  double x;
  double y;

  Point(double new_x, double new_y) : x(new_x), y(new_y) {};

  bool operator==(const Point &right) {
    return this->x == right.x && this->y == right.y;
  }
  bool operator!=(const Point &right) {
    return !((*this) == right);
  }

  Point reflex(Point &target) {
    double new_x = 2 * this->x - target.x;
    double new_y = 2 * this->y - target.y;
    return Point(new_x, new_y);
  }

  double distance(Point &target) {
    return std::sqrt((this->x - target.x) * (this->x - target.x) + (this->y - target.y) * (this->y - target.y));
  }

  Point rotate(Point &target, double angle) {
    double new_x = this->x * std::cos(angle) - this->y * std::sin(angle) + this->x;
    double new_y = this->x * std::sin(angle) + this->y * std::cos(angle) + this->y;
    return Point(new_x, new_y);
  }

  Point scale(Point &target, double coefficient) {
    double new_x = coefficient * (target.x - this->x) + this->x;
    double new_y = coefficient * (target.y - this->y) + this->y;
    return Point(new_x, new_y);
  }
};

class Line {

 private:
  double angle;
  double bias;

 public:
  Line(double new_angle, double new_bias) : angle(new_angle), bias(new_bias) {};
  Line(Point &first, Point &second) {

    this->angle = (first.y - second.y) / (first.x - second.x);
    this->bias = (first.y - first.x * this->angle);
  };
  Line(Point &new_point, double new_angle) {
    this->angle = new_angle;
    this->bias = (new_point.y - new_point.x * this->angle);
  };

  bool operator==(const Line &right) {
    return this->angle == right.angle && this->bias == right.bias;
  }
  bool operator!=(const Line &right) {
    return !((*this) == right);
  }

  Point reflex(Point &target) {
    double center_x = ((target.x / this->angle + target.y - this->bias) / (this->angle - 1 / this->angle));
    double center_y = this->angle * center_x + this->bias;
    Point center = Point(center_x, center_y);
    return center.reflex(target);
  }
};

class Shape {
 public:
  virtual double perimeter() = 0;
  virtual double area() = 0;
  virtual bool operator==(const Shape &another) = 0;
  virtual void rotate(Point &center, double angle) = 0;
  virtual void reflex(Point &center) = 0;
  virtual void reflex(Line &axis) = 0;
  virtual void scale(Point &center, double coefficient) = 0;
};

class Polygon : public Shape {
 private:
  std::vector<Point> vertices;
 public:

  Polygon(std::vector<Point> new_vertices) : vertices(new_vertices) {};

  unsigned verticesCount() { return this->vertices.size(); }

  const std::vector<Point> getVertices() {
    return this->vertices;
  }

  bool operator==(const Polygon &another) {
    return this->vertices == another.vertices;
  }

  bool virtual operator==(const Shape &another) override {
    return false;
  }

  double perimeter() override {
    double p = 0;
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      double length = this->vertices[i].distance(this->vertices[(i + 1) % this->vertices.size()]);
      p += length;
    }
    return p;
  };

  double area() override {
    double s = 0;
    for (size_t i = 0; i < this->vertices.size() - 1; ++i) {
      s += (this->vertices[i].x * this->vertices[(i + 1) % this->vertices.size()].y -
          this->vertices[i].y * this->vertices[(i + 1) % this->vertices.size()].x);
    }
    return fabs(s) / 2;
  };

  void reflex(Point &center) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = center.reflex(vertices[i]);
    }
  }

  void reflex(Line &axis) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = axis.reflex(vertices[i]);
    }
  }

  void rotate(Point &center, double angle) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = center.rotate(vertices[i], angle);
    }
  }

  void scale(Point &center, double coefficient) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = center.scale(vertices[i], coefficient);
    }
  }
};