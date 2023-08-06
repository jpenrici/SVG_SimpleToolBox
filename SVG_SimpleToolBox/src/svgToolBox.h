#ifndef _SMALLTOOLBOX_H_
#define _SMALLTOOLBOX_H_

#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <string>
#include <vector>

namespace smalltoolbox {

constexpr auto WHITE = "#FFFFFF";
constexpr auto BLACK = "#000000";
constexpr auto RED   = "#FF0000";
constexpr auto GREEN = "#00FF00";
constexpr auto BLUE  = "#0000FF";

// (x,y)
class Point {

    struct Coordinate {
        double value;

        double operator+(double value);
        void operator+=(double value);

        double operator-(double value);
        void operator-=(double value);

        double operator*(double value);
        void operator*=(double value);

        bool operator==(Coordinate coordinate);
        bool operator>(Coordinate coordinate);
        bool operator>=(Coordinate coordinate);
        bool operator<(Coordinate coordinate);
        bool operator<=(Coordinate coordinate);

        std::string toStr();
    };

public:

    // Axis
    Coordinate X, Y;

    // Point (0, 0)
    Point() : X{0}, Y{0} {};
    // Point (x, y)
    Point(double x, double y) : X{x}, Y{y} {};

    ~Point() {};

    Point operator+(Point point);
    Point operator+(double value);
    void operator+=(Point point);
    void operator+=(double value);

    Point operator-(const Point point);
    Point operator-(double value);
    void operator-=(const Point point);
    void operator-=(double value);

    Point operator*(const Point point);
    Point operator*(double value);
    void operator*=(const Point point);
    void operator*=(double value);

    bool operator==(const Point point);

    void sum(double x, double y);
    void multiply(double x, double y);

    bool equal(Point point);
    double distance(Point point);

    std::array<double, 2> XY();

    std::string toStr();

};

class Line {

public:

    Point first, second;

    // Line : Origin (0,0) to Point (x,y)
    Line(Point point) : first(Point()), second(point) {};
    // Line : Point 1 (x1,y1) to Point 2 (x,y)
    Line(Point first, Point second) : first(first), second(second) {};

    ~Line() {};

    double length();
    std::vector<Point> points();
};

class Polygon {

    Point center;
    double angle;
    double radius;
    unsigned sides;
    std::vector<Point> vertices;

public:

    Polygon() {};
    ~Polygon() {};

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    std::vector<Point> setup(Point center, double radius, double angle, unsigned sides);

    // Return last vertices.
    std::vector<Point> points();

    double sideLength();
    double diagonalLength();

};

// SVG
class SVG {

public:

    struct Metadata {
        std::string creator = "SVG created automatically by algorithm in C++.";
        std::string title = "SVG";
        std::string publisherAgentTitle = "";
        std::string date = "";

        Metadata() {};
        Metadata(std::string creator, std::string title, std::string publisher)
            : creator(creator), title(title), publisherAgentTitle(publisher) {}
    };

    struct Shape {
        std::string name, fill, stroke;
        double strokeWidth;
        double fillOpacity, strokeOpacity; // 0.0 = 0 = 0%; 255 = 1.0 = 100%
        std::vector<Point> points;

        Shape()
            : name("Shape"), fill(WHITE), stroke(BLACK), strokeWidth(1.0),
              fillOpacity(255.0), strokeOpacity(255.0), points({}) {}

        Shape(std::string name,  std::string fill, std::string stroke, double strokeWidth)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
              fillOpacity(255.0), strokeOpacity(255.0), points({}) {}
        Shape(std::string name,  std::string fill, std::string stroke, double strokeWidth,
              std::vector<Point> points)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
              fillOpacity(255.0), strokeOpacity(255.0), points(points) {}

        Shape(std::string name,  std::string fill, std::string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
              fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), points({}) {}
        Shape(std::string name,  std::string fill, std::string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity, std::vector<Point> points)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
              fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), points(points) {}
    };

    static const std::string INT2HEX(unsigned value);
    static const std::string RGB2HEX(unsigned R, unsigned G, unsigned B);
    static const std::string RGBA2HEX(unsigned R, unsigned G, unsigned B, unsigned A);

    static const std::string group(std::string id, std::string elements);
    static const std::string polyline(Shape shape);
    static const std::string polygon(Shape shape);
    static const std::string svg(int width, int height, const std::string &xml,
                                 Metadata metadata);
};

// Generic
double radians(double angle);
double angle(double radians);
double angle(double x0, double y0, double x1, double y1);
double cos(double value, double radius, double angle);
double sin(double value, double radius, double angle);
double distance(double x0, double y0, double x1, double y1);

void view(double value);
void view(Point point);
void view(std::array<double, 2> arr);
void view(std::vector<Point> points);
void view(std::string str);

bool save(const std::string &text, std::string path = "");

} // namespace

#endif // _SMALLTOOLBOX_H_
