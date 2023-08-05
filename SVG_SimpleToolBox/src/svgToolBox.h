#ifndef _SMALLTOOLBOX_H_
#define _SMALLTOOLBOX_H_

#include <array>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

namespace smalltoolbox {

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

    Coordinate X, Y;

    // Point(0, 0)
    Point() : X{0}, Y{0} {};
    // Point(x, y)
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

class Polygon {

    Point center;
    unsigned sides;

    std::vector<Point> points;

public:

    Polygon(Point center, unsigned sides);
    ~Polygon() {};
};

class Line {

public:

    Point first, second;

    Line() : first(Point()), second(Point()) {};
    Line(Point first, Point second) : first(first), second(second) {};
    ~Line() {};

    double length();
};

double radians(double angle);
double angle(double radians);
double angle(double x0, double y0, double x1, double y1);
double cos(double value, double radius, int angle);
double sin(double value, double radius, int angle);
double distance(double x0, double y0, double x1, double y1);

void view(double value);
void view(Point point);
void view(std::array<double, 2> arr);

} // namespace

#endif // _SMALLTOOLBOX_H_
