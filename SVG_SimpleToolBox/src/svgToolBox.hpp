#ifndef _SMALLTOOLBOX_H_
#define _SMALLTOOLBOX_H_

#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <string>
#include <vector>

#define CRITICALNUMBER std::numeric_limits<double>::max()

namespace smalltoolbox {

constexpr auto WHITE = "#FFFFFF";
constexpr auto BLACK = "#000000";
constexpr auto RED   = "#FF0000";
constexpr auto GREEN = "#00FF00";
constexpr auto BLUE  = "#0000FF";

// Math
double radians(double angle)
{
    return angle * std::numbers::pi / 180.0;
}

double angle(double radians)
{
    return radians * 180.0 / std::numbers::pi;
}

// Returns the angle of the line (x0,y0)(x1,y1).
double angle(double x0, double y0, double x1, double y1)
{
    double result = angle(std::atan((y1 - y0) / (x1 - x0)));

    if (x0 == x1 && y0 == y1) {
        result = 0;
    }
    if (x0 <  x1 && y0 == y1) {
        result = 0;
    }
    if (x0 == x1 && y0 <  y1) {
        result = 90;
    }
    if (x0 >  x1 && y0 == y1) {
        result = 180;
    }
    if (x0 == x1 && y0 >  y1) {
        result = 270;
    }
    if (x0 >  x1 && y0 <  y1) {
        result += 180;
    }
    if (x0 >  x1 && y0 >  y1) {
        result += 180;
    }
    if (x0 <  x1 && y0 >  y1) {
        result += 360;
    }

    return result;
}

// Return: value + radius * cos(angle).
double cos(double value, double radius, double angle)
{
    return value + radius * std::cos(radians(angle));
}

// Return: value + radius * sin(angle).
double sin(double value, double radius, double angle)
{
    return value + radius * std::sin(radians(angle));
}

// Returns the distance between two points.
double distance(double x0, double y0, double x1, double y1)
{
    return std::sqrt(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2));
}

// Rounds value to N digits after decimal point.
double round(double value, unsigned digits = 2)
{
    digits = digits > 10 ? 10 : digits;

    return std::round(value * std::pow(10, digits)) / std::pow(10, digits);
}

// Round values
std::vector<double> round(std::vector<double> values, unsigned digits = 2)
{
    for(unsigned i = 0; i < values.size(); i++) {
        values[i] = smalltoolbox::round(values[i], digits);
    }

    return values;
}

// Triangle area
double triangleArea(double x0, double y0, double x1, double y1, double x2, double y2)
{
    // Heron's formula
    auto a = smalltoolbox::distance(x0, y0, x1, y1);
    auto b = smalltoolbox::distance(x1, y1, x2, y2);
    auto c = smalltoolbox::distance(x2, y2, x0, y0);
    auto s = (a + b + c) / 2.0;

    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

// Triangle height
double triangleHeight(double x0, double y0, double x1, double y1, double x2, double y2)
{
    auto area = triangleArea(x0, y0, x1, y1, x2, y2);
    auto h0 = 2 * area / smalltoolbox::distance(x0, y0, x1, y1);
    auto h1 = 2 * area / smalltoolbox::distance(x1, y1, x2, y2);
    auto h2 = 2 * area / smalltoolbox::distance(x2, y2, x0, y0);

    return std::max(std::max(h0, h1), h2);
}

// Calculates the point of intersection between two lines.
// Returns false if the lines are parallel or coincident.
// Line 1 (x0, y0) - (x1, y1),
// Line 2 (x2, y2) - (x3, y4).
bool lineIntersect(double x0, double y0, double x1, double y1,
                   double x2, double y2, double x3, double y3,
                   double& X, double& Y)
{
    double d = (y3 - y2) * (x1 - x0) - (x3 - x2) * (y1 - y0);
    if (d == 0) {   // Two lines are parallel or coincident ...
        X = CRITICALNUMBER;
        Y = CRITICALNUMBER;
        return false;
    }

    double t = ((x3 - x2) * (y0 - y2) - (y3 - y2) * (x0 - x2)) / d;
    double u = ((x1 - x0) * (y0 - y2) - (y1 - y0) * (x0 - x2)) / d;

    if (t >= 0.0 && t <= 1.0 && u >= 0 && u <= 1.0) {
        X = x0 + t * (x1 - x0);
        Y = y0 + t * (y1 - y0);
        return true;
    }

    // Lines do not intersect
    return false;
}

// Sort numbers
std::vector<double> sort(std::vector<double> numbers, bool ascendingOrder = true)
{
    if (ascendingOrder) {
        std::sort(numbers.begin(), numbers.end(), std::less<double>());
    } else {
        std::sort(numbers.begin(), numbers.end(), std::greater<double>());
    }

    return numbers;
}

// (x,y)
class Point {

    struct Coordinate {
        double value;

        std::string toStr()
        {
            return std::to_string(value);
        }
    };

public:

    // Axis
    Coordinate X, Y;

    // Point (0, 0)
    Point() : X{0}, Y{0} {};

    // Point (x, y)
    Point(double x, double y) : X{x}, Y{y} {};

    ~Point() {};

    Point operator+(Point point)
    {
        return Point(X.value + point.X.value, Y.value + point.Y.value);
    }

    Point operator+(double value)
    {
        return Point(X.value + value, Y.value + value);
    }

    void operator+=(Point point)
    {
        sum(point.X.value, point.Y.value);
    }

    void operator+=(double value)
    {
        sum(value, value);
    }

    Point operator-(const Point point)
    {
        return Point(X.value - point.X.value, Y.value - point.Y.value);
    }

    Point operator-(double value)
    {
        return Point(X.value - value, Y.value - value);
    }

    void operator-=(const Point point)
    {
        sum(-point.X.value, -point.Y.value);
    }

    void operator-=(double value)
    {
        sum(-value, -value);
    }

    Point operator*(const Point point)
    {
        return Point(X.value * point.X.value, Y.value * point.Y.value);
    }

    Point operator*(double value)
    {
        return Point(X.value * value, Y.value * value);
    }

    void operator*=(const Point point)
    {
        multiply(point.X.value, point.Y.value);
    }

    void operator*=(double value)
    {
        multiply(value, value);
    }

    bool operator==(const Point point)
    {
        return equal(point);
    }

    // X += x, Y += y
    void sum(double x, double y)
    {
        X.value += x;
        Y.value += y;
    }

    // X *= x; Y *= y
    void multiply(double x, double y)
    {
        X.value *= x;
        Y.value *= y;
    }

    // X = 0; Y = 0
    void reset()
    {
        X.value = 0;
        Y.value = 0;
    }

    // Checks if coordinates are equal.
    bool equal(Point point)
    {
        return X.value == point.X.value && Y.value == point.Y.value;
    }

    // Angle of the imaginary line between the current point and the other.
    double angle(Point point)
    {
        return smalltoolbox::angle(X.value, Y.value, point.X.value, point.Y.value);
    }

    // Distance between the current point and another.
    double distance(Point point)
    {
        return std::sqrt(std::pow(X.value - point.X.value, 2) +
                         std::pow(Y.value - point.Y.value, 2));
    }

    // Position from angle and radius.
    // Current point as origin.
    Point position(double angle, double radius)
    {
        return Point(smalltoolbox::cos(X.value, radius, angle),
                     smalltoolbox::sin(Y.value, radius, angle));
    }

    // Returns the rounded current coordinates.
    Point round(unsigned digits = 2)
    {
        return Point(smalltoolbox::round(X.value, digits),
                     smalltoolbox::round(Y.value, digits));
    }

    // Returns Vector[2] with X and Y values.
    std::vector<double> XY()
    {
        return {X.value, Y.value};
    }

    std::string toStr()
    {
        return X.toStr() + "," + Y.toStr();
    }

};

// Special point.
const Point Origin = Point(0, 0);

// Angle between three points.
// Sign : True, respects the order of the vectors.
double angle(Point origin, Point first, Point second, bool signal = false)
{
    auto angle1 = origin.angle(first);
    auto angle2 = origin.angle(second);

    if (signal) {
        return angle1 - angle2;
    }

    return std::max(angle1, angle2) - std::min(angle1, angle2);
}

// Compare groups
template<typename T>
bool equal(std::vector<T> group1, std::vector<T> group2, bool compareOrder = false)
{
    if (group1.size() != group2.size()) {
        return false;
    }

    if (compareOrder) {
        for(unsigned i = 0; i < group1.size(); i++) {
            if (!(group1[i] == group2[i])) {
                return false;
            }
        }
    }

    for(auto v1 : group1) {
        bool differentFromEveryone = true;
        for(auto v2 : group2) {
            if (v1 == v2) {
                differentFromEveryone = false;
                break;
            }
        }
        if (differentFromEveryone) {
            return false;
        }
    }

    return true;
}

// Round points
std::vector<Point> round(std::vector<Point> points, unsigned digits = 2)
{
    for(unsigned i = 0; i < points.size(); i++) {
        points[i] = points[i].round(digits);
    }

    return points;
}

// Sum all the distances, point by point.
double sumDistances(std::vector<Point> points)
{
    if (points.empty()) {
        return 0;
    }

    if (points.size() == 1)
    {
        return 0;
    }

    if (points.size() == 2)
    {
        return points.front().distance(points.back());
    }

    double sum = 0;
    for (unsigned i = 1; i < points.size(); i++) {
        sum += points[i].distance(points[i - 1]);
    }

    return sum;
}

// Sort points by X or Y axis.
std::vector<Point> sort(std::vector<Point> points, bool X_axis = true)
{
    if (points.empty()) {
        return {};
    }

    if (points.size() == 1) {
        return points;
    }

    std::map<double, std::vector<double> > mapPoint;

    for(auto p : points) {
        auto key = X_axis ? p.X.value : p.Y.value;
        auto value = X_axis ? p.Y.value : p.X.value;
        if (mapPoint.find(key) == mapPoint.end()) {
            mapPoint.insert({key, {value}});
        } else {
            mapPoint[key].push_back(value);
        }
    }

    std::vector<Point> result;
    for(auto item : mapPoint) {
        if (X_axis) {
            for(auto value : item.second) { // Y
                result.push_back(Point(item.first, value));
            }
        } else {
            auto values = smalltoolbox::sort(item.second);
            for(auto value : values) {      // X
                result.push_back(Point(value, item.first));
            }
        }
    }

    return result;
}

// Sum : Point (Total X axis, Total Y axis)
Point total(std::vector<Point> points)
{
    Point sum;
    for(auto p : points) {
        sum += p;
    }

    return sum;
}

// Average : Point (Total X axis / Points, Total Y axis / Points)
bool average(std::vector<Point> points, Point& point)
{
    if (points.empty()) {
        point = Origin;
        return false;
    }

    point = smalltoolbox::total(points) * (1.0 / points.size()) ;

    return true;
}

// Organize points
std::vector<Point> organize(std::vector<Point> points)
{
    if (points.size() < 2) {
        return points;
    }

    // Map : Angle x Point
    std::map<float, std::vector<Point> > mapPoint;

    for(auto value : points) {
        float key = Point(0,0).angle(value);
        if (mapPoint.find(key) == mapPoint.end()) {
            mapPoint.insert(std::make_pair(key, std::vector<Point>{value}));
        } else {
            mapPoint[key].push_back(value);
        }
    }

    std::vector<Point> result;
    for(auto item : mapPoint) {
        for (auto vPoints : item.second) {
            result.push_back(vPoints);
        }
    }

    return result;
}

// Triangle area using points.
double triangleArea(Point p1, Point p2, Point p3)
{
    return smalltoolbox::triangleArea(p1.X.value, p1.Y.value,
                                      p2.X.value, p2.Y.value,
                                      p3.X.value, p3.Y.value);
}

// Triangle height using points.
double triangleHeight(Point p1, Point p2, Point p3)
{
    return smalltoolbox::triangleHeight(p1.X.value, p1.Y.value,
                                        p2.X.value, p2.Y.value,
                                        p3.X.value, p3.Y.value);
}

// (x1,y1)(x2,y2)
class Line {

    // Store the last configuration.
    std::vector<Point> vertices;

    void update()
    {
        vertices = {first, second};
    }

public:

    Point first, second;

    Line() {};

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    Line(Point first, Point second)
        : first(first), second(second) {};

    // Line : Point (x,y), angle and length.
    Line(Point origin, double angle, double length)
    {
        setup(origin, angle, length);
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    Line(double x1, double y1, double x2, double y2)
        : first(Point(x1, y1)), second(Point(x2, y2)) {};

    ~Line() {};

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    std::vector<Point> setup(Point first, Point second = Origin)
    {
        this->first = first;
        this->second = second;

        return points();
    }

    // Line : Point (x,y), angle and length.
    // Returns vertices.
    std::vector<Point> setup(Point origin, double angle, double length)
    {
        first = origin;
        second = origin.position(angle, length);

        return points();
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    std::vector<Point> setup(double x1, double y1, double x2, double y2)
    {
        return setup(Point(x1, y1), Point(x2, y2));
    }

    bool operator==(Line line)
    {
        return equal(line);
    }

    // Distance between the two points.
    double length()
    {
        return first.distance(second);
    }

    // Line angle.
    // Direction: first point to second point.
    double angle()
    {
        return first.angle(second);
    }

    // Midpoint
    Point middle()
    {
        return first.position(angle(), length() / 2);
    }

    // Returns the intersection point with another line.
    bool intersection(Line line, Point &point)
    {
        std::array<double, 2> xy;
        auto result = smalltoolbox::lineIntersect(first.X.value, first.Y.value,
                                                  second.X.value, second.Y.value,
                                                  line.first.X.value, line.first.Y.value,
                                                  line.second.X.value, line.second.Y.value,
                                                  point.X.value, point.Y.value);

        return result;
    }

    // Perpendicular line passing through the point.
    Line perpendicular(Point point)
    {
        // Dummy triangle area.
        double area = smalltoolbox::triangleArea(first.X.value, first.Y.value,
                                                 second.X.value, second.Y.value,
                                                 point.X.value, point.Y.value);
        // Pythagorean theorem : a^2 + b^2 = c^2
        double c = first.distance(point);               // hypotenuse
        double b = 2 * area / first.distance(second);   // area = base * height / 2
        double a = std::sqrt(std::pow(c, 2) - std::pow(b, 2));

        // Line with base slope.
        return Line(first.position(angle(), a), point);
    }

    // Line 1 == Line 2
    bool equal(Line line)
    {
        return smalltoolbox::equal(points(), line.points());
    }

    // Returns the rounded current coordinates.
    Line round(unsigned digits = 2)
    {
        return Line(first.round(digits), second.round(digits));
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        update();

        return vertices;
    }
};

// Calculates the point of intersection between two lines.
// Returns false if the lines are parallel or coincident.
bool lineIntersect(Line line1, Line line2, Point &point)
{
    return line1.intersection(line2, point);
}

// Perpendicular line passing through the point.
Line perpendicular(Line line, Point point)
{
    return line.perpendicular(point);
}

// Polygon
class Polygon {

    // Store the last configuration.
    std::vector<Point> vertices;

    Point last_first, last_second, last_third, last_fourth;

    void update(Point first, Point second, Point third, Point fourth)
    {
        last_first = first;
        last_second = second;
        last_third = third;
        last_fourth = fourth;

        vertices[0] = first;
        vertices[1] = second;
        vertices[2] = third;
        if (vertices.size() > 3) {
            vertices[3] = fourth;
        }
    }

    bool state()
    {
        return (!first.equal(last_first) || !second.equal(last_second) ||
                !third.equal(last_third) || !fourth.equal(last_fourth));
    }

public:

    Point first, second, third, fourth;

    Polygon() {};

    Polygon(std::vector<Point> points)
    {
        setup(points);
    }

    ~Polygon() {};

    std::vector<Point> setup(std::vector<Point> points)
    {
        if (points.size() < 3) {
            vertices.clear();
            return vertices;
        }

        vertices = points;
        this->first = vertices[0];
        this->second = vertices[1];
        this->third = vertices[2];
        this->fourth = points.size() > 3 ? vertices[3] : Origin;

        last_first = first;
        last_second = second;
        last_third = third;
        last_fourth = fourth;

        return vertices;
    }

    bool operator==(Polygon polygon)
    {
        return equal(polygon);
    }

    bool isConvex()
    {
        auto sides = vertices.size();
        if (sides < 3) {
            return false;
        }

        // Cross product of two vectors.
        auto crossProduct = [](Point origin, Point first, Point second) {
            int x1 = first.X.value  - origin.X.value;
            int y1 = first.Y.value  - origin.Y.value;
            int x2 = second.X.value - origin.X.value;
            int y2 = second.Y.value - origin.Y.value;
            return x1 * y2 - y1 * x2;
        };

        int previous = 0;
        for(unsigned i = 0; i < sides; i++) {
            auto product = crossProduct(vertices[i],
                                        vertices[(i + 1) % sides],
                                        vertices[(i + 2) % sides]);
            if (product != 0) {
                if (product * previous < 0) {
                    return false;
                } else {
                    previous = product;
                }
            }
        }

        return true;
    }

    // Rearrange the polygon points.
    std::vector<Point> organize()
    {
        return smalltoolbox::organize(vertices);
    }

    // Calculates the Area by triangular subdivisions.
    double area()
    {
        if (vertices.size() < 3) {
            return 0;
        }

        if (vertices.size() == 3) {
            return smalltoolbox::triangleArea(vertices[0], vertices[1], vertices[2]);
        }

        double area = 0;
        if (isConvex()) {
            for (unsigned i = 2; i < vertices.size(); ++i) {
                area += smalltoolbox::triangleArea(vertices[0], vertices[i - 1], vertices[i]);
            }
        } else {
            // TO DO
            // Concave
        }

        return area;
    }

    double perimeter()
    {
        double perimeter = smalltoolbox::sumDistances(vertices);
        perimeter += vertices.back().distance(vertices.front());

        return perimeter;
    }

    bool equal(Polygon polygon)
    {
        return smalltoolbox::equal(points(), polygon.points());
    }

    Polygon round(unsigned digits = 2)
    {
        Polygon polygon(vertices);
        polygon.vertices = smalltoolbox::round(vertices, digits);

        return polygon;
    }

    // Return the length of each side
    std::vector<double> lengthOfSides()
    {
        std::vector<double> sides;
        for(unsigned i = 0; i < vertices.size(); i++) {
            sides.push_back(vertices[i].distance(vertices[i % vertices.size()]));
        }

        return sides;
    }

    // Average length
    double averageLength()
    {
        if (!vertices.empty()) {
            return perimeter() / vertices.size();
        }

        return 0;
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        if (state()) {
            update(first, second, third, fourth);
        }

        return vertices;
    }

};

// (x1,y1)(x2,y2)(x3,y3)
class Triangle : public Polygon {

public:

    Triangle() {};

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    Triangle(Point first, Point second, Point third)
    {
        setup(first, second, third);
    };

    ~Triangle() {};

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    // Returns vertices.
    std::vector<Point> setup(Point first, Point second, Point third)
    {
        Polygon::setup({first, second, third});

        return points();
    }

    // Triangle: Line and height
    // Returns vertices.
    std::vector<Point> setup(Line side, double height)
    {
        return setup(side.first,
                     side.second,
                     Point(side.middle()).position(90 + side.angle(), height));
    }

    // Triangle: Points (x1,y1),(x2,y2) and height
    // Returns vertices.
    std::vector<Point> setup(Point first, Point second, double height)
    {
        return setup(Line(first, second), height);
    }

    double height()
    {
        return smalltoolbox::triangleHeight(first, second, third);
    }

};

// (x1,y1)(x2,y2)(x3,y3)(x4,y4)
class Rectangle : public Polygon {

public:

    Rectangle() {};

    // Rectangle: Points (x1,y1),(x2,y2),(x3,y3),(x4,y4)
    Rectangle(Point first, Point second, Point third, Point fourth)
    {
        setup(first, second, third, fourth);
    };

    // Rectangle : Point (x,y), width and heigth.
    Rectangle(Point origin, double width, double heigth)
    {
        setup(origin, width, heigth);
    }

    ~Rectangle() {};

    // Rectangle: Points (x1,y1),(x2,y2),(x3,y3),(x4,y4)
    // Returns vertices.
    std::vector<Point> setup(Point first, Point second, Point third, Point fourth)
    {
        Polygon::setup({first, second, third, fourth});

        return points();
    }

    // Rectangle : Point (x,y), width and heigth.
    // Returns vertices.
    std::vector<Point> setup(Point origin, double width, double heigth)
    {
        return setup(origin,
                     origin + Point(width, 0),
                     origin + Point(width, heigth),
                     origin + Point(0, heigth));
    }

};

class RegularPolygon : public Polygon {

    Point last_center;
    double last_angle;
    double last_horizontalRadius;
    double last_verticalRadius;
    unsigned last_sides;

    void update()
    {
        // If there has been modification in the parameters.
        if (state()) {
            setup(center, horizontalRadius, verticalRadius, angle, sides);
        }
    }

public:

    Point center;
    double angle;
    double horizontalRadius;
    double verticalRadius;
    unsigned sides;

    // Returns true if data changes.
    bool state()
    {
        return !(center == last_center &&
                 angle == last_angle &&
                 horizontalRadius == last_horizontalRadius &&
                 verticalRadius == last_verticalRadius &&
                 sides == last_sides);
    }

    RegularPolygon() {};

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    RegularPolygon(Point center, double radius, double angle, unsigned sides)
    {
        setup(center, radius, radius, angle, sides);
    }

    // Center : polygon center point (x,y),
    // horizontalRadius : distance from the center on X axis (>= 1),
    // verticalRadius   : distance from the center on Y axis (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    RegularPolygon(Point center, double horizontalRadius, double verticalRadius, double angle,
                   unsigned sides)
    {
        setup(center, horizontalRadius, verticalRadius, angle, sides);
    }

    ~RegularPolygon() {};

    // Center : polygon center point (x,y),
    // horizontalRadius : distance from the center on X axis (>= 1),
    // verticalRadius   : distance from the center on Y axis (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    // Returns vertices.
    std::vector<Point> setup(Point center, double horizontalRadius, double verticalRadius,
                             double angle, unsigned sides)
    {
        // Update
        this->center = center;
        this->angle = angle;
        this->horizontalRadius = horizontalRadius;
        this->verticalRadius = verticalRadius;
        this->sides = sides;

        last_center = center;
        last_angle = angle;
        last_horizontalRadius = horizontalRadius;
        last_verticalRadius = verticalRadius;
        last_sides = sides;

        // Check
        sides = sides > 360 ? 360 : sides;
        if (horizontalRadius < 1 || verticalRadius < 1 || sides < 3) {
            return Polygon::setup({});
        }

        std::vector<Point> points;
        for (unsigned a = angle; a < 360 + angle; a += (360 / sides)) {
            points.push_back(Point(smalltoolbox::cos(center.X.value, horizontalRadius, a),
                                   smalltoolbox::sin(center.Y.value, verticalRadius, a)));
        }

        return Polygon::setup(points);
    }

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    std::vector<Point> setup(Point center, double radius, double angle, unsigned sides)
    {
        return setup(center, radius, radius, angle, sides);
    }

    double sideLength()
    {
        return first.distance(second);
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        update();

        return Polygon::points();
    }

};

class IrregularPolygon : public Polygon {

public:

    IrregularPolygon(){};

    IrregularPolygon(std::vector<Point> points)
    {
        setup(points);
    }

    IrregularPolygon(Triangle triangle)
    {
        setup(triangle.points());
    }

    IrregularPolygon(Rectangle rectangle)
    {
        setup(rectangle.points());
    }

    IrregularPolygon(RegularPolygon polygon)
    {
        setup(polygon.points());
    }

    ~IrregularPolygon() {};
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

    static const std::string INT2HEX(unsigned value)
    {
        std::string digits = "0123456789ABCDEF";
        std::string result = "";
        if (value < 16) {
            result.push_back('0');
            result.push_back(digits[value % 16]);
        }
        else {
            while (value != 0) {
                result = digits[value % 16] + result;
                value /= 16;
            }
        }
        return result;
    }

    static const std::string RGB2HEX(unsigned R, unsigned G, unsigned B)
    {
        return "#" + INT2HEX(R) + INT2HEX(G) + INT2HEX(B);
    }

    static const std::string RGBA2HEX(unsigned R, unsigned G, unsigned B, unsigned A)
    {
        return RGB2HEX(R, G, B) + INT2HEX(A);
    }

    static const std::string group(std::string id, std::string elements)
    {
        id = id.empty() ? "<g>\n" : "<g id=\"" + id + "\" >\n";
        return elements.empty() ? "" : id + elements + "</g>\n";
    }

private:

    static const void check(Shape &shape, std::string name)
    {
        shape.name = name.empty() ? "Shape" : name;
        shape.stroke = shape.stroke.empty() ? "#000000" : shape.stroke;
        shape.fillOpacity = shape.fillOpacity < 0 ? 0 : std::min(shape.fillOpacity / 255, 1.0);
        shape.strokeOpacity = shape.strokeOpacity < 0 ? 0 : std::min(shape.strokeOpacity / 255, 1.0);
    }

public:

    static const std::string polyline(Shape shape)
    {

        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        std::string values = "";
        for (unsigned i = 0; i < shape.points.size(); i++) {
            values += shape.points[i].toStr() + " ";
        }

        check(shape, "polyline");
        std::string opacity = std::to_string(shape.fillOpacity);
        std::string strokeOpacity = std::to_string(shape.strokeOpacity);
        std::string strokeWidth = std::to_string(shape.strokeWidth);

        return {
            "     <polyline\n"
            "        id=\"" + shape.name + "\"\n" +
            "        style=\"" +
            "opacity:" + opacity + ";fill:" + shape.fill +
            ";stroke:" + shape.stroke + ";stroke-width:" + strokeWidth +
            ";stroke-opacity:" + strokeOpacity +
            ";stroke-linejoin:round;stroke-linecap:round\"\n" +
            "        points=\"" + values + "\" />\n"
        };
    }

    static const std::string polygon(Shape shape)
    {
        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        std::string values = "";
        for (unsigned i = 0; i < shape.points.size() - 1; i++) {
            values += shape.points[i].toStr() + " L ";
        }
        values += shape.points[shape.points.size() - 1].toStr();

        check(shape, "polygon");
        std::string opacity = std::to_string(shape.fillOpacity);
        std::string strokeOpacity = std::to_string(shape.strokeOpacity);
        std::string strokeWidth = std::to_string(shape.strokeWidth);

        return {
            "     <path\n"
            "        id=\"" + shape.name + "\"\n" +
            "        style=\"" +
            "opacity:" + opacity + ";fill:" + shape.fill +
            ";stroke:" + shape.stroke + ";stroke-width:" + strokeWidth +
            ";stroke-opacity:" + strokeOpacity +
            ";stroke-linejoin:round;stroke-linecap:round\"\n" +
            "        d=\"M " + values + " Z\" />\n"
        };
    }

    static const std::string svg(int width, int height, const std::string &xml,
                                 Metadata metadata)
    {
        std::string now = "";
        try {
            std::time_t t = std::time(nullptr);
            std::tm *const pTm = std::localtime(&t);
            now = std::to_string(1900 + pTm->tm_year);
        }
        catch (...) {
            // pass
        }

        metadata.date = metadata.date == "" ? now : metadata.date;

        return {
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
            "<svg\n"
            "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
            "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
            "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
            "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
            "   xmlns=\"http://www.w3.org/2000/svg\"\n"
            "   xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
            "   width=\"" + std::to_string(width) + "\"\n" +
            "   height=\"" + std::to_string(height) + "\"\n" +
            "   viewBox= \"0 0 " + std::to_string(width) + " " + std::to_string(height) + "\"\n" +
            "   version=\"1.1\"\n" +
            "   id=\"svg8\">\n" +
            "  <title\n" +
            "     id=\"title1\">" + metadata.title + "</title>\n" +
            "  <defs\n" +
            "     id=\"defs1\" />\n" +
            "  <metadata\n" +
            "     id=\"metadata1\">\n" +
            "    <rdf:RDF>\n" +
            "      <cc:Work\n" +
            "         rdf:about=\"\">\n" +
            "        <dc:format>image/svg+xml</dc:format>\n" +
            "        <dc:type\n" +
            "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n" +
            "        <dc:title>" + metadata.title + "</dc:title>\n" +
            "        <dc:date>" + metadata.date + "</dc:date>\n" +
            "        <dc:publisher>\n" +
            "          <cc:Agent>\n" +
            "            <dc:title>" + metadata.publisherAgentTitle + "</dc:title>\n" +
            "          </cc:Agent>\n" +
            "        </dc:publisher>\n" +
            "        <dc:subject>\n" +
            "          <rdf:Bag>\n" +
            "            <rdf:li></rdf:li>\n" +
            "            <rdf:li></rdf:li>\n" +
            "            <rdf:li></rdf:li>\n" +
            "            <rdf:li></rdf:li>\n" +
            "          </rdf:Bag>\n" +
            "        </dc:subject>\n" +
            "        <dc:creator>\n" +
            "          <cc:Agent>\n" +
            "            <dc:title>" + metadata.creator + "</dc:title>\n" +
            "          </cc:Agent>\n" +
            "        </dc:creator>\n" +
            "        <cc:license\n" +
            "           rdf:resource=\"http://creativecommons.org/publicdomain/zero/1.0/\" />\n" +
            "        <dc:description>SVG created automatically by algorithm in C++.</dc:description>\n" +
            "      </cc:Work>\n" +
            "      <cc:License\n" +
            "         rdf:about=\"http://creativecommons.org/publicdomain/zero/1.0/\">\n" +
            "        <cc:permits\n" +
            "           rdf:resource=\"http://creativecommons.org/ns#Reproduction\" />\n" +
            "        <cc:permits\n" +
            "           rdf:resource=\"http://creativecommons.org/ns#Distribution\" />\n" +
            "        <cc:permits\n" +
            "           rdf:resource=\"http://creativecommons.org/ns#DerivativeWorks\" />\n" +
            "      </cc:License>\n" +
            "    </rdf:RDF>\n" +
            "  </metadata>\n" +
            "  <!--      Created in C++ algorithm       -->\n" +
            "  <!-- Attention: do not modify this code. -->\n" +
            "\n"
            "" + xml + "" +
            "\n" +
            "  <!-- Attention: do not modify this code. -->\n" +
            "</svg>"
        };
    }
};

// Generic
void view(double value)
{
    std::cout << std::to_string(value) << '\n';
}

void view(Point point)
{
    std::cout << "(" << point.toStr() << ")" << '\n';
}

template<std::size_t SIZE>
void view(std::array<double, SIZE> arr)
{
    unsigned i = 0;
    for (const auto &value : arr) {
        std::cout << "[" << i << "]: " << smalltoolbox::round(value) << '\n';
        i++;
    }
}

void view(std::vector<Point> points)
{
    std::string str{};
    for (unsigned i = 0; i < points.size(); i++) {
        str += "(" + points[i].toStr() + ")" + (i < points.size() - 1 ? "," : "");
    }

    if (str.empty()) {
        std::cout << "Empty\n";
    }
    else {
        std::cout << str << '\n';
    }
}

void view(std::vector<double> values)
{
    std::string str{};
    for (unsigned i = 0; i < values.size(); i++) {
        str += std::to_string(values[i]) + (i < values.size() - 1 ? "," : "");
    }

    if (str.empty()) {
        std::cout << "Empty\n";
    }
    else {
        std::cout << str << '\n';
    }
}

void view(std::string str)
{
    std::cout << str << '\n';
}

bool save(const std::string &text, std::string path = "")
{
    if (text.empty()) {
        std::cerr << "Empty text! Export failed!\n";
        return false;
    }

    if (path.empty()) {
        path = "output.txt";
    }

    try {
        std::ofstream file(path, std::ios::out);
        file << text;
        file.close();
    }
    catch (const std::exception &e) {
        std::cout << "Error handling file writing.\n";
        std::cerr << e.what() << "\n";
        return false;
    }

    return true;
}

} // namespace

#endif // _SMALLTOOLBOX_H_
