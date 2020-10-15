#pragma once
#include "cmath"
#include <utility>

#include "tr1/cmath"
#include "vector"

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

  bool operator==(const Point &right) const {
    return this->x == right.x && this->y == right.y;
  }

  bool operator!=(const Point &right) const {
    return !((*this) == right);
  }

  Point operator+(const Point &other) const {
    return {this->x + other.x, this->y + other.y};
  }

  Point operator*(double a) const {
    return {this->x * a, this->y * a};
  }

  Point reflex(const Point &target) const {
    double new_x = 2 * this->x - target.x;
    double new_y = 2 * this->y - target.y;
    return {new_x, new_y};
  }

  double distance(const Point &target) const {
    return std::sqrt((this->x - target.x) * (this->x - target.x) + (this->y - target.y) * (this->y - target.y));
  }

  Point rotate(const Point &target, double angle) const {
    double new_x = this->x * std::cos(angle) - this->y * std::sin(angle) + this->x;
    double new_y = this->x * std::sin(angle) + this->y * std::cos(angle) + this->y;
    return {new_x, new_y};
  }

  Point scale(const Point &target, double coefficient) const {
    double new_x = coefficient * (target.x - this->x) + this->x;
    double new_y = coefficient * (target.y - this->y) + this->y;
    return {new_x, new_y};
  }
};

class Line {

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

  bool operator==(const Line &right) const {
    return this->angle == right.angle && this->bias == right.bias;
  }

  bool operator!=(const Line &right) const {
    return !((*this) == right);
  }

  Point reflex(const Point &target) const {
    double center_x = ((target.x / this->angle + target.y - this->bias) / (this->angle - 1 / this->angle));
    double center_y = this->angle * center_x + this->bias;
    Point center = Point(center_x, center_y);
    return center.reflex(target);
  }

 private:
  double angle;
  double bias;
};

class Shape {
 public:
  virtual double perimeter() const = 0;
  virtual double area() const = 0;
  virtual bool operator==(const Shape &another) const = 0;
  virtual bool operator!=(const Shape &another) const = 0;
  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;
};

class Polygon : public Shape {
 public:

  explicit Polygon(std::vector<Point> new_vertices) : vertices(std::move(new_vertices)) {};

  Polygon(const Polygon& copy){
    for (auto& p: copy.vertices){
      this->vertices.push_back(p);
    }
  };

  unsigned verticesCount() { return this->vertices.size(); }

  std::vector<Point> getVertices() {
    std::vector<Point> v = this->vertices;
    return v;
  }

  bool operator==(const Polygon &another) const {
    if (this->vertices.size() != another.vertices.size()) {
      return false;
    }
    size_t i = 0;
    while (i < this->vertices.size()) {
      if (this->vertices[i] == another.vertices[0]) {
        break;
      }
      ++i;
    }
    if (i == this->vertices.size()) {
      return false;
    }
    for (size_t j = 0; j < another.vertices.size(); ++j) {
      if (another.vertices[j] != this->vertices[(i + j) % this->vertices.size()]) {
        return false;
      }
    }
    return true;
  }

  bool operator==(const Shape &another) const override {
    return false;
  }

  bool operator!=(const Shape &another) const override {
    return !(*this == another);
  }

  double perimeter() const override {
    double p = 0;
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      double length = this->vertices[i].distance(this->vertices[(i + 1) % this->vertices.size()]);
      p += length;
    }
    return p;
  };

  double area() const override {
    double s = 0;
    for (size_t i = 0; i < this->vertices.size() - 1; ++i) {
      s += (this->vertices[i].x * this->vertices[(i + 1) % this->vertices.size()].y -
          this->vertices[i].y * this->vertices[(i + 1) % this->vertices.size()].x);
    }
    return fabs(s) / 2;
  };

  void reflex(Point center) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = center.reflex(vertices[i]);
    }
  }

  void reflex(Line axis) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = axis.reflex(vertices[i]);
    }
  }

  void rotate(Point center, double angle) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = center.rotate(vertices[i], angle);
    }
  }

  void scale(Point center, double coefficient) override {
    for (size_t i = 0; i < this->vertices.size(); ++i) {
      vertices[i] = center.scale(vertices[i], coefficient);
    }
  }

 protected:
  std::vector<Point> vertices;
};

class Ellipse : public Shape {
 public:

  Ellipse(Point &first_focus, Point &second_focus, double ellipse_distance) :
      first_focus(first_focus),
      second_focus(second_focus),
      ellipse_distance(ellipse_distance) {};

  std::pair<Point, Point> focuses() const {
    return {this->first_focus, this->second_focus};
  }

  Point center() const {
    return (this->first_focus + this->second_focus) * 0.5;
  }

  double eccentricity() const {
    Point center = this->center();
    double center_focus_dist = center.distance(first_focus);
    return center_focus_dist * 2 / this->ellipse_distance;
  }

  bool operator==(const Shape &another) const override {
    return false;
  }

  bool operator!=(const Shape &another) const override {
    return !(*this == another);
  }

  bool operator==(const Ellipse &another) const {
    if (this->ellipse_distance != another.ellipse_distance) {
      return false;
    } else if (this->first_focus != another.first_focus) {
      return this->first_focus == another.second_focus && this->second_focus == another.first_focus;
    } else {
      return this->first_focus == another.first_focus && this->second_focus == another.second_focus;
    }
  }

  double perimeter() const override {
    double ecs = this->eccentricity();
    return 2 * this->ellipse_distance * std::tr1::comp_ellint_2(ecs);
  }

  double area() const override {
    Point center = this->center();
    double c = center.distance(this->first_focus);
    double a = this->ellipse_distance / 2;
    double b = std::sqrt(a * a - c * c);
    return M_PI * a * b;
  };

  void reflex(Point center) override {
    this->first_focus = center.reflex(this->first_focus);
    this->second_focus = center.reflex(this->second_focus);
  }

  void reflex(Line axis) override {
    this->first_focus = axis.reflex(this->first_focus);
    this->second_focus = axis.reflex(this->second_focus);
  }

  void rotate(Point center, double angle) override {
    this->first_focus = center.rotate(this->first_focus, angle);
    this->second_focus = center.rotate(this->second_focus, angle);
  }

  void scale(Point center, double coefficient) override {
    this->first_focus = center.scale(this->first_focus, coefficient);
    this->second_focus = center.scale(this->second_focus, coefficient);
    this->ellipse_distance = this->ellipse_distance * coefficient;
  }

 protected:
  Point first_focus;
  Point second_focus;
  double ellipse_distance;
};


class Circle: public Ellipse{

 public:

  Circle(Point &center, double radius): Ellipse(center, center, radius*2) {};

  double radius(){
    return this->ellipse_distance/2;
  }
};