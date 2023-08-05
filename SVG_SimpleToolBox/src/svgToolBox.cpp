#include "svgToolBox.h"

// Coordinate

double smalltoolbox::Point::Coordinate::operator+(double value)
{
    return this->value + value;
}

void smalltoolbox::Point::Coordinate::operator+=(double value)
{
    this->value += value;
}

double smalltoolbox::Point::Coordinate::operator-(double value)
{
    return this->value - value;
}

void smalltoolbox::Point::Coordinate::operator-=(double value)
{
    this->value -= value;
}

double smalltoolbox::Point::Coordinate::operator*(double value)
{
    return this->value * value;
}

void smalltoolbox::Point::Coordinate::operator*=(double value)
{
    this->value *= value;
}

bool smalltoolbox::Point::Coordinate::operator==(Coordinate coordinate)
{
    return value == coordinate.value;
}

bool smalltoolbox::Point::Coordinate::operator>(Coordinate coordinate)
{
    return value > coordinate.value;
}

bool smalltoolbox::Point::Coordinate::operator>=(Coordinate coordinate)
{
    return value >= coordinate.value;
}

bool smalltoolbox::Point::Coordinate::operator<(Coordinate coordinate)
{
    return value < coordinate.value;
}

bool smalltoolbox::Point::Coordinate::operator<=(Coordinate coordinate)
{
    return value <= coordinate.value;
}

std::string smalltoolbox::Point::Coordinate::toStr()
{
    return std::to_string(value);
}

// Point

smalltoolbox::Point smalltoolbox::Point::operator+(Point point)
{
    return Point(X.value + point.X.value, Y.value + point.Y.value);
}

void smalltoolbox::Point::operator+=(Point point)
{
    sum(point.X.value, point.Y.value);
}

smalltoolbox::Point smalltoolbox::Point::operator+(double value)
{
    return Point(X.value + value, Y.value + value);
}
void smalltoolbox::Point::operator+=(double value)
{
    sum(value, value);
}

smalltoolbox::Point smalltoolbox::Point::operator-(const Point point)
{
    return Point(X.value - point.X.value, Y.value - point.Y.value);
}

void smalltoolbox::Point::operator-=(const Point point)
{
    sum(-point.X.value, -point.Y.value);
}
smalltoolbox::Point smalltoolbox::Point::operator-(double value)
{
    return Point(X.value - value, Y.value - value);
}
void smalltoolbox::Point::operator-=(double value)
{
    sum(-value, -value);
}

smalltoolbox::Point smalltoolbox::Point::operator*(const Point point)
{
    return Point(X.value * point.X.value, Y.value * point.Y.value);
}

void smalltoolbox::Point::operator*=(const Point point)
{
    multiply(point.X.value, point.Y.value);
}

bool smalltoolbox::Point::operator==(const Point point)
{
    return equal(point);
}

smalltoolbox::Point smalltoolbox::Point::operator*(double value)
{
    return Point(X.value * value, Y.value * value);
}

void smalltoolbox::Point::operator*=(double value)
{
    multiply(value, value);
}

void smalltoolbox::Point::sum(double x, double y)
{
    X.value += x;
    Y.value += y;
}

void smalltoolbox::Point::multiply(double x, double y)
{
    X.value *= x;
    Y.value *= y;
}

bool smalltoolbox::Point::equal(Point point)
{
    return X.value == point.X.value &&
           Y.value == point.Y.value;
}

double smalltoolbox::Point::distance(Point point)
{
    return std::sqrt(std::pow(X.value - point.X.value, 2) +
                     std::pow(Y.value - point.Y.value, 2));
}

std::array<double, 2> smalltoolbox::Point::XY()
{
    return {{X.value, Y.value}};
}

std::string smalltoolbox::Point::toStr()
{
    return X.toStr() + "," + Y.toStr();
}

// Polygon

smalltoolbox::Polygon::Polygon(Point center, unsigned int sides)
    : center(center), sides(sides)
{
    // TO DO
}

// Line

double smalltoolbox::Line::length()
{
    return first.distance(second);
}

// Generic

double smalltoolbox::radians(double angle)
{
    return angle * std::numbers::pi / 180.0;
}

double smalltoolbox::angle(double radians)
{
    return radians * 180.0 / std::numbers::pi;
}

double smalltoolbox::angle(double x0, double y0,
                           double x1, double y1)
{
    double angle = atan((y1 - y0) / (x1 - x0)) * 180.0 / std::numbers::pi;
    if (x0 >  x1 && y0 == y1) { angle = 0; }
    if (x0 == x1 && y0 <  y1) { angle = 90; }
    if (x0 <  x1 && y0 == y1) { angle = 180; }
    if (x0 == x1 && y0 <  y1) { angle = 270; }
    if (x0 <  x1 && y0 >  y1) { angle += 180; }
    if (x0 <  x1 && y0 <  y1) { angle += 180; }
    if (x0 >  x1 && y0 <  y1) { angle += 360; }

    return angle;
}

double smalltoolbox::cos(double value, double radius, int angle)
{
    return value + radius * std::cos(radians(angle));
}

double smalltoolbox::sin(double value, double radius, int angle)
{
    return value + radius * std::sin(radians(angle));
}

double smalltoolbox::distance(double x0, double y0,
                              double x1, double y1)
{
    return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}

void smalltoolbox::view(Point point)
{
    std::cout << point.toStr() << '\n';
}

void smalltoolbox::view(double value)
{
    std::cout << std::to_string(value) << '\n';
}

void smalltoolbox::view(std::array<double, 2> arr)
{
    std::cout << std::to_string(arr[0]) << ","
              << std::to_string(arr[1]) << '\n';
}
