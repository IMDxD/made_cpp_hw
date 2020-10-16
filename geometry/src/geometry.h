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

const double EPS = 1e-9;

struct Point {

 public:
  double x;
  double y;

  Point() : x(0), y(0) {};

  Point(double new_x, double new_y) : x(new_x), y(new_y) {};

  bool operator==(const Point &right) const {
    return fabs(this->x - right.x) < EPS && fabs(this->y == right.y) < EPS;
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
    angle /= 180;
    angle *= M_PI;
    Point shifted = {target.x - this->x, target.y - this->y};
    double new_x = shifted.x * std::cos(angle) - shifted.y * std::sin(angle) + this->x;
    double new_y = shifted.x * std::sin(angle) + shifted.y * std::cos(angle) + this->y;
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

  Line(Point first, Point second) {
    this->angle = (first.y - second.y) / (first.x - second.x);
    this->bias = (first.y - first.x * this->angle);
  };

  Line(Point new_point, double new_angle) {
    this->angle = new_angle;
    this->bias = (new_point.y - new_point.x * this->angle);
  };

  double get_angle() const {
    return this->angle;
  }

  bool operator==(const Line &right) const {
    return fabs(this->angle - right.angle) < EPS && fabs(this->bias - right.bias) < EPS;
  }

  bool operator!=(const Line &right) const {
    return !((*this) == right);
  }

  Line perpendicular(Point &target) const {
    double per_angle = -1 / this->angle;
    return {target, per_angle};
  }

  Point reflex(Point &target) const {
    Line per = this->perpendicular(target);
    Point center = this->intersection(per);
    return center.reflex(target);
  }

  Point intersection(const Line &target) const {
    if (this->angle == target.angle) {
      return {0, 0};
    }
    double new_x = (target.bias - this->bias) / (this->angle - target.angle);
    double new_y = this->angle * new_x + this->bias;
    return {new_x, new_y};
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

  Polygon() : vertices(std::vector<Point>(4)) {};

  explicit Polygon(std::vector<Point> new_vertices) : vertices(std::move(new_vertices)) {};

  unsigned verticesCount() { return this->vertices.size(); }

  std::vector<Point> getVertices() {
    std::vector<Point> v = this->vertices;
    return v;
  }

  bool operator==(const Polygon &another) const {
    if (this->vertices.size() != another.vertices.size()) {
      return false;
    } else if (this->vertices.empty()) {
      return true;
    }
    size_t i = 0;
    while (i < this->vertices.size()) {
      if (this->vertices[i] == another.vertices[0]) {
        break;
      }
      ++i;
    }
    if (this->vertices[(i + 1) % this->vertices.size()] == another.vertices[1]) {
      for (size_t j = 0; j < another.vertices.size(); ++j) {
        if (another.vertices[j] != this->vertices[(i + j) % this->vertices.size()]) {
          return false;
        }
      }
      return true;
    } else {
      for (size_t j = 0; j < another.vertices.size(); ++j) {
        size_t this_index;
        if (j > i) {
          this_index = this->vertices.size() - j + i;
        } else {
          this_index = (i - j) % this->vertices.size();
        }
        if (another.vertices[j] != this->vertices[this_index]) {
          return false;
        }
      }
      return true;
    }
  }

  bool operator==(const Shape &another) const override {
    return false;
  }

  bool operator!=(const Shape &another) const override {
    return !(*this == another);
  }

  bool operator!=(const Polygon &another) const {
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
    for (size_t i = 0; i < this->vertices.size(); ++i) {
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
    if (fabs(this->ellipse_distance - another.ellipse_distance) > EPS) {
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

class Circle : public Ellipse {

 public:

  Circle(Point &center, double radius) : Ellipse(center, center, radius * 2) {};

  double radius() {
    return this->ellipse_distance / 2;
  }
};

class Rectangle : public Polygon {
 public:

  Point center() {
    return (this->vertices[0] + this->vertices[2]) * 0.5;
  }

  std::pair<Line, Line> diagonals() {
    Line first(this->vertices[0], this->vertices[2]);
    Line second(this->vertices[1], this->vertices[3]);
    return {first, second};
  }

  Rectangle(Point first, Point third, double fraction) : Polygon() {
    Point center = (first + third) * 0.5;
    if (fraction > 1) {
      fraction = 1 / fraction;
    }
    double alpha = M_PI - 2 * std::atan(fraction);
    alpha *= 180;
    alpha /= M_PI;
    Point second = center.rotate(first, alpha);
    Point fourth = center.reflex(second);
    this->vertices = {first, second, third, fourth};
  };
};

class Square : public Rectangle {

 public:
  Circle circumscribedCircle() {
    Point center = this->center();
    double radius = center.distance(this->vertices[0]);
    return {center, radius};
  }

  Circle inscribedCircle() {
    Point center = this->center();
    Point line_center = (this->vertices[0] + this->vertices[1]) * 0.5;
    double radius = center.distance(line_center);
    return {center, radius};
  }

  Square(Point first, Point third) : Rectangle(first, third, 1) {};
};

class Triangle : public Polygon {
 public:

  Triangle(Point first, Point second, Point third) : Polygon({first, second, third}) {};

  Circle circumscribedCircle() {
    Line ab = Line(this->vertices[0], this->vertices[1]);
    Line bc = Line(this->vertices[1], this->vertices[2]);
    Line ac = Line(this->vertices[0], this->vertices[2]);
    Point d = (this->vertices[0] + this->vertices[1]) * 0.5;
    Point e = (this->vertices[1] + this->vertices[2]) * 0.5;
    Point f = (this->vertices[0] + this->vertices[2]) * 0.5;
    Point center;
    if (ab.get_angle() == 0) {
      Line ac_per = ab.perpendicular(d);
      Line bc_per = bc.perpendicular(e);
      center = ac_per.intersection(bc_per);
    } else if (bc.get_angle() == 0) {
      Line ab_per = ab.perpendicular(d);
      Line ac_per = ac.perpendicular(f);
      center = ab_per.intersection(ac_per);
    } else {
      Line ab_per = ab.perpendicular(d);
      Line bc_per = bc.perpendicular(e);
      center = ab_per.intersection(bc_per);
    }
    double radius = center.distance(this->vertices[0]);
    return {center, radius};
  }

  Circle inscribedCircle() {
    double ab = this->vertices[1].distance(this->vertices[0]);
    double bc = this->vertices[1].distance(this->vertices[2]);
    double ac = this->vertices[2].distance(this->vertices[0]);
    double center_x = (ac * this->vertices[1].x + bc * this->vertices[0].x + ab * this->vertices[2].x) / (ab + ac + bc);
    double center_y = (ac * this->vertices[1].y + bc * this->vertices[0].y + ab * this->vertices[2].y) / (ab + ac + bc);
    Point center = {center_x, center_y};
    Line ab_line = Line(this->vertices[0], this->vertices[1]);
    Line bc_line = Line(this->vertices[1], this->vertices[2]);
    double radius;
    if (ab_line.get_angle() == 0) {
      Line per_bc = bc_line.perpendicular(center);
      Point radius_inter = per_bc.intersection(bc_line);
      radius = radius_inter.distance(center);
    } else {
      Line per_ab = ab_line.perpendicular(center);
      Point radius_inter = per_ab.intersection(ab_line);
      radius = radius_inter.distance(center);
    }
    return {center, radius};
  }

  Point centroid() {
    Point center_ab = (this->vertices[0] + this->vertices[1]) * 0.5;
    Point center_bc = (this->vertices[1] + this->vertices[2]) * 0.5;
    Line ab_med = Line(this->vertices[2], center_ab);
    Line bc_med = Line(this->vertices[0], center_bc);
    return ab_med.intersection(bc_med);
  }

  Point orthocenter() {

    Line ab = Line(this->vertices[0], this->vertices[1]);
    Line bc = Line(this->vertices[1], this->vertices[2]);
    Line ac = Line(this->vertices[0], this->vertices[2]);
    if (ab.get_angle() == 0) {
      Line ac_per = ac.perpendicular(this->vertices[1]);
      Line bc_per = bc.perpendicular(this->vertices[0]);
      return ac_per.intersection(bc_per);
    } else if (bc.get_angle() == 0) {
      Line ac_per = ac.perpendicular(this->vertices[1]);
      Line ab_per = ab.perpendicular(this->vertices[2]);
      return ac_per.intersection(ab_per);
    } else {
      Line ab_per = ab.perpendicular(this->vertices[2]);
      Line bc_per = bc.perpendicular(this->vertices[0]);
      return ab_per.intersection(bc_per);
    }
  }

  Line EulerLine() {
    Point centroid = this->centroid();
    Point ortho_center = this->orthocenter();
    return {centroid, ortho_center};
  }

  Circle ninePointsCircle() {
    Point ortho_center = this->orthocenter();
    Point circum_center = this->circumscribedCircle().center();
    Point center = (ortho_center + circum_center) * 0.5;
    Point center_ab = (this->vertices[0] + this->vertices[1]) * 0.5;
    double radius = center.distance(center_ab);
    return {center, radius};
  }
};