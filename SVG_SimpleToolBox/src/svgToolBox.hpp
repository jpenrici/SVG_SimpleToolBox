#ifndef _SMALLTOOLBOX_H_
#define _SMALLTOOLBOX_H_

#include <algorithm>
#include <array>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <string>
#include <vector>

namespace smalltoolbox {

#define Numbers std::vector<double>
#define Point2D smalltoolbox::Point
#define Points  std::vector<smalltoolbox::Point>
#define Polygon smalltoolbox::RegularPolygon
#define Strings std::vector<std::string>

#define CRITICALNUMBER std::numeric_limits<double>::max()

using std::string;
using std::vector;

class Point;
class Base;
class Line;
class Triangles;
class Rectangle;
class Circle;
class Ellipse;
class RegularPolygon;
class IrregularPolygon;

class SVG;
class Sketch;
class Interpreter;

Point Total(Points points);
Points Organize(Points points);
Points Round(Points points, int decimalPlaces = -1);
Points Sort(Points points, bool X_axis = true);
Points Sum(Points group, Point point);
Points Sum(Points group, double value);

Line Perpendicular(Line line, Point point);

Numbers Round(Numbers values, int decimalPlaces = -1);
Numbers Sort(Numbers numbers, bool ascendingOrder = true);

bool Average(Points points, Point &point);
bool LineIntersect(Line line1, Line line2, Point &point);
bool LineIntersect(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, double &X, double &Y);
bool Save(const string &text, string path = "");
template<typename T> bool Equal(vector<T> group1, vector<T> group2, bool compareOrder = false);

double Angle(Point origin, Point first, Point second, bool signal = false);
double Angle(double radians);
double Angle(double x0, double y0, double x1, double y1);
double Cos(double value, double radius, double angle);
double Distance(double x0, double y0, double x1, double y1);
double Radians(double angle);
double Round(double value, int decimalPlaces = -1);
double Sin(double value, double radius, double angle);
double SumDistances(Points points);
double TriangleArea(Point p1, Point p2, Point p3);
double TriangleArea(double x0, double y0, double x1, double y1, double x2, double y2);
double TriangleHeight(Point p1, Point p2, Point p3);
double TriangleHeight(double x0, double y0, double x1, double y1, double x2, double y2);

Strings Split(string str, const char delimiter);
Strings Trim(Strings vStr, const char trimmer);

string Load(string path, string filenameExtension = "");

string Replace(string str, char character, string replace);
string Replace(string str, char character, char replace);
string LTrim(string str, const char trimmer);
string RTrim(string str, const char trimmer);
string Trim(string str, const char trimmer);
string Join(Strings vStr, const char delimiter);

void View(Point point);
void View(Points points);
void View(double value);
void View(string str);
void View(Strings values);
template<std::size_t SIZE> void View(std::array<double, SIZE> arr);
template<typename T> void View(vector<T> values);

// Point 2D (x,y)
class Point {

    struct Coordinate {
        double value;

        string toStr()
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
        return Angle(X.value, Y.value, point.X.value, point.Y.value);
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
        return Point(Cos(X.value, radius, angle), Sin(Y.value, radius, angle));
    }

    // Returns the rounded current coordinates.
    Point round(int decimalPlaces = 2)
    {
        return Point(Round(X.value, decimalPlaces), Round(Y.value, decimalPlaces));
    }

    // Returns Vector[2] with X and Y values.
    Numbers XY()
    {
        return {X.value, Y.value};
    }

    string toStr(bool trimmed = false)
    {
        if (trimmed) {
            return RTrim(X.toStr(), '0') + "," + RTrim(Y.toStr(), '0');
        }

        return X.toStr() + "," + Y.toStr();
    }

};

// Special point.
static const Point Origin = Point(0, 0);
static const Point Zero   = Point(0, 0);

class Base {

    // Store the last configuration.
    Points vertices;

    Point last_first, last_second, last_third, last_fourth;

    void update(Point first, Point second, Point third, Point fourth)
    {
        last_first = first;
        last_second = second;
        last_third = third;
        last_fourth = fourth;

        if (!vertices.empty()) {
            vertices[0] = first;
        }
        if (vertices.size() > 1) {
            vertices[1] = second;
        }
        if (vertices.size() > 2) {
            vertices[2] = third;
        }
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

    Base() {};
    ~Base() {};

    Points setup(Points points)
    {
        if (points.size() < 2) {
            vertices.clear();
            return vertices;
        }

        vertices = points;
        first  = vertices[0];
        second = vertices[1];
        third  = points.size() > 2 ? vertices[2] : Origin;
        fourth = points.size() > 3 ? vertices[3] : Origin;

        last_first  = first;
        last_second = second;
        last_third  = third;
        last_fourth = fourth;

        return vertices;
    }

    bool operator==(Base polygon)
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
        for (unsigned i = 0; i < sides; i++) {
            auto product = crossProduct(vertices[i],
                                        vertices[(i + 1) % sides],
                                        vertices[(i + 2) % sides]);
            if (product != 0) {
                if (product * previous < 0) {
                    return false;
                }
                else {
                    previous = product;
                }
            }
        }

        return true;
    }

    // Rearrange the polygon points.
    Points organize()
    {
        return Organize(vertices);
    }

    // Calculates the Area by triangular subdivisions.
    double area()
    {
        if (vertices.size() < 3) {
            return 0;
        }

        double result = 0;
        if (vertices.size() == 3) {
            result = TriangleArea(vertices[0], vertices[1], vertices[2]);
        }
        else if (isConvex()) {
            for (unsigned i = 2; i < vertices.size(); ++i) {
                result += TriangleArea(vertices[0], vertices[i - 1], vertices[i]);
            }
        }
        else {
            // TO DO
            // Concave
        }

        return result;
    }

    double perimeter()
    {
        double perimeter = SumDistances(vertices);

        if (vertices.size() > 2) {
            perimeter += vertices.back().distance(vertices.front());
        }

        return perimeter;
    }

    bool equal(Base polygon)
    {
        return Equal(points(), polygon.points());
    }

    Base round(unsigned decimalPlaces = 2)
    {
        Base polygon;
        polygon.vertices = Round(vertices, decimalPlaces);

        return polygon;
    }

    // Return the length of each side.
    Numbers lengthOfSides()
    {
        if (vertices.size() < 2) {
            return {0};
        }

        if (vertices.size() == 2) {
            return {perimeter()};
        }

        Numbers lengths;
        for (unsigned i = 0; i < vertices.size(); i++) {
            lengths.push_back(vertices[i].distance(vertices[(i + 1) % vertices.size()]));
        }

        return lengths;
    }

    // Average length.
    double averageLength()
    {
        if (!vertices.empty()) {
            return perimeter() / vertices.size();
        }

        return 0;
    }

    // Returns the current vertices.
    Points points()
    {
        if (state()) {
            update(first, second, third, fourth);
        }

        return vertices;
    }

};

// Line (x1,y1)(x2,y2)
class Line : public Base {

public:

    Line() {};

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    Line(Point first, Point second)
    {
        setup(first, second);
    };

    // Line : Point (x,y), angle and length.
    Line(Point origin, double angle, double length)
    {
        setup(origin, angle, length);
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    Line(double x1, double y1, double x2, double y2)
    {
        setup(Point(x1, y1), Point(x2, y2));
    };

    ~Line() {};

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    Points setup(Point first, Point second = Origin)
    {
        Base::setup({first, second});

        return points();
    }

    // Line : Point (x,y), angle and length.
    // Returns vertices.
    Points setup(Point origin, double angle, double length)
    {
        return setup(origin, origin.position(angle, length));
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    Points setup(double x1, double y1, double x2, double y2)
    {
        return setup(Point(x1, y1), Point(x2, y2));
    }

    // Distance between the two points.
    double length()
    {
        return perimeter();
    }

    // Line angle.
    // Direction: first point to second point.
    double angle()
    {
        // Update
        auto vertices = points();

        if (vertices.size() < 2) {
            return 0;
        }

        return first.angle(second);
    }

    // Midpoint
    Point middle()
    {
        // Update
        points();

        return first.position(angle(), length() / 2);
    }

    // Returns the intersection point with another line.
    bool intersection(Line line, Point &point)
    {
        return LineIntersect(first.X.value, first.Y.value,
                             second.X.value, second.Y.value,
                             line.first.X.value, line.first.Y.value,
                             line.second.X.value, line.second.Y.value,
                             point.X.value, point.Y.value);
    }

    // Perpendicular line passing through the point.
    Line perpendicular(Point point)
    {
        // Dummy triangle area.
        double area = TriangleArea(first.X.value, first.Y.value,
                                   second.X.value, second.Y.value,
                                   point.X.value, point.Y.value);
        // Pythagorean theorem : a^2 + b^2 = c^2
        double c = first.distance(point);               // hypotenuse
        double b = 2 * area / first.distance(second);   // area = base * height / 2
        double a = std::sqrt(std::pow(c, 2) - std::pow(b, 2));

        // Line with base slope.
        return Line(first.position(angle(), a), point);
    }

};

// Triangle (x1,y1)(x2,y2)(x3,y3)
class Triangle : public Base {

public:

    Triangle() {};

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    Triangle(Point first, Point second, Point third)
    {
        setup(first, second, third);
    };

    // Triangle: Points (x1,y1),(x2,y2) and height.
    // Returns vertices.
    Triangle(Point first, Point second, double height)
    {
        setup(Line(first, second), height);
    }

    ~Triangle() {};

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    // Returns vertices.
    Points setup(Point first, Point second, Point third)
    {
        Base::setup({first, second, third});

        return points();
    }

    // Triangle: Line and height.
    // Returns vertices.
    Points setup(Line side, double height)
    {
        return setup(side.first,
                     side.second,
                     Point(side.middle()).position(90 + side.angle(), height));
    }

    // Triangle: Points (x1,y1),(x2,y2) and height.
    // Returns vertices.
    Points setup(Point first, Point second, double height)
    {
        return setup(Line(first, second), height);
    }

    double height()
    {
        return TriangleHeight(first, second, third);
    }

};

// Rectangle (x1,y1)(x2,y2)(x3,y3)(x4,y4)
class Rectangle : public Base {

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
    Points setup(Point first, Point second, Point third, Point fourth)
    {
        Base::setup({first, second, third, fourth});

        return points();
    }

    // Rectangle : Point (x,y), width and heigth.
    // Returns vertices.
    Points setup(Point origin, double width, double heigth)
    {
        return setup(origin,
                     origin + Point(width, 0),
                     origin + Point(width, heigth),
                     origin + Point(0, heigth));
    }

};

// Regular Polygon (x,y)...(xN,yN)
class RegularPolygon : public Base {

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
    RegularPolygon(Point center, double horizontalRadius, double verticalRadius,
                   double angle, unsigned sides)
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
    Points setup(Point center, double horizontalRadius, double verticalRadius,
                 double angle, unsigned sides)
    {
        // Update.
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

        // Check.
        sides = sides > 360 ? 360 : sides;
        if (horizontalRadius < 1 || verticalRadius < 1 || sides < 3) {
            return Base::setup({});
        }

        Points points;
        for (unsigned a = angle; a < 360 + angle; a += (360 / sides)) {
            points.push_back(Point(Cos(center.X.value, horizontalRadius, a),
                                   Sin(center.Y.value, verticalRadius, a)));
        }

        return Base::setup(points);
    }

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    Points setup(Point center, double radius, double angle, unsigned sides)
    {
        return setup(center, radius, radius, angle, sides);
    }

    double sideLength(int decimalPlaces = 2)
    {
        if (sides < 2) {
            return 0;
        }

        auto result = first.distance(second);

        return decimalPlaces < 0 ? result : Round(result, decimalPlaces);
    }

    // Returns the current vertices.
    Points points()
    {
        update();

        return Base::points();
    }

};

class Ellipse : public RegularPolygon {

public:

    Ellipse(){};

    ~Ellipse() {};

    Ellipse(Point center, double horizontalRadius, double verticalRadius)
    {
        setup(center, horizontalRadius, verticalRadius);
    }

    Points setup(Point center, double horizontalRadius, double verticalRadius)
    {
        RegularPolygon::setup(center, horizontalRadius, verticalRadius, 0, 360);

        return points();
    }

    double area()
    {
        return horizontalRadius * verticalRadius * std::numbers::pi;
    }

    double perimeter()
    {
        // TO DO
        return -1;
    }
};

class Circle : public Ellipse {

public:

    Circle(){};

    ~Circle() {};

    Circle(Point center, double radius)
    {
        setup(center, radius);
    }

    Points setup(Point center, double radius)
    {
        Ellipse::setup(center, radius, radius);

        return points();
    }

};

// Irregular Polygon (x,y)...(xN,yN)
class IrregularPolygon : public Base {

public:

    IrregularPolygon() {};

    IrregularPolygon(Points points)
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

    static constexpr const char* WHITE = "#FFFFFF";
    static constexpr const char* BLACK = "#000000";
    static constexpr const char* RED   = "#FF0000";
    static constexpr const char* GREEN = "#00FF00";
    static constexpr const char* BLUE  = "#0000FF";

    // Metadata setup.
    // creator             : string with the name of the creator or developer,
    // title               : string with title,
    // publisherAgentTitle : string with the name of the publisher,
    // date                : string with creation date, if empty use current date.
    struct Metadata {
        string creator = "SVG created automatically by algorithm in C++.";
        string title = "SVG";
        string publisherAgentTitle = "";
        string date = "";

        Metadata() {};
        Metadata(string creator, string title, string publisher)
            : creator(creator), title(title), publisherAgentTitle(publisher) {}
    };

    // Drawing setup.
    // name          : ID used in SVG element,
    // fill          : fill color in hexadecimal string format (#FFFFFF),
    // stroke        : stroke color in hexadecimal string format (#FFFFFF),
    // strokeWidth   : line width,
    // fillOpacity   : fill opacity or alpha value from 0 to 255.
    // strokeOpacity : stroke opacity or alpha value from 0 to 255.
    // points        : Points vector (x,y).
    struct Shape {
        string name, fill, stroke;
        double strokeWidth;
        double fillOpacity, strokeOpacity; // 0.0 = 0 = 0%; 255 = 1.0 = 100%
        Points points;

        Shape()
            : name("Shape"), fill(WHITE), stroke(BLACK), strokeWidth(1.0),
            fillOpacity(255.0), strokeOpacity(255.0), points({}) {}

        Shape(string name,  string fill, string stroke, double strokeWidth)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
            fillOpacity(255.0), strokeOpacity(255.0), points({}) {}
        Shape(string name,  string fill, string stroke, double strokeWidth,
              Points points)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
            fillOpacity(255.0), strokeOpacity(255.0), points(points) {}

        Shape(string name,  string fill, string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
            fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), points({}) {}
        Shape(string name,  string fill, string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity, Points points)
            : name(name), fill(fill), stroke(stroke), strokeWidth(strokeWidth),
            fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), points(points) {}
    };

    // Converts decimal value to hexadecimal.
    static const string INT2HEX(unsigned value)
    {
        string digits = "0123456789ABCDEF";
        string result = "";
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

    // Converts hexadecimal value to decimal
    static const int HEX2INT(string value)
    {
        if (value.starts_with('#')) {
            value = value.substr(1);
        }

        if (value.empty()) {
            return 0;
        }

        unsigned result = 0;
        try {
            result = std::stoul(value, nullptr, 16);
        } catch (...) {
            // pass
        }

        return result;
    }

    // Return vector unsigned {Red, Green, Blue, Alpha}
    static const vector<unsigned> HEX2RGB(string value)
    {
        if (value.starts_with('#')) {
            value = value.substr(1);
        }

        if (value.empty()) {
            return {};
        }

        if (value.size() > 8) {
            value = value.substr(0, 8);
        }

        vector<unsigned> result;
        if (value.size() % 2 == 0) {
            while (!value.empty()) {
                string num = value.substr(0, 2);
                value = value.substr(2);
                result.push_back(HEX2INT(num));
            }
        }

        return result;
    }

    // Formats values (Red, Green, Blue) to "#RRGGBB" hexadecimal.
    static const string RGB2HEX(unsigned R, unsigned G, unsigned B)
    {
        return "#" + INT2HEX(R) + INT2HEX(G) + INT2HEX(B);
    }

    // Formats values (Red, Green, Blue, Alpha) to "#RRGGBBAA" hexadecimal.
    static const string RGBA2HEX(unsigned R, unsigned G, unsigned B, unsigned A)
    {
        return RGB2HEX(R, G, B) + INT2HEX(A);
    }

    // Returns SVG: <g> Elements </g>
    static const string group(string id, string elements)
    {
        id = id.empty() ? "<g>\n" : "<g id=\"" + id + "\" >\n";
        return elements.empty() ? "" : id + elements + "</g>\n";
    }

private:

    // Validates and formats entries.
    static const void check(Shape &shape, string name)
    {
        shape.name = name.empty() ? "Shape" : name;
        shape.stroke = shape.stroke.empty() ? "#000000" : shape.stroke;
        shape.fillOpacity = shape.fillOpacity < 0 ? 0 : std::min(shape.fillOpacity / 255, 1.0);
        shape.strokeOpacity = shape.strokeOpacity < 0 ? 0 : std::min(shape.strokeOpacity / 255, 1.0);
    }

public:

    // Returns SVG: <polyline ... />
    static const string polyline(Shape shape)
    {

        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        string values = "";
        for (unsigned i = 0; i < shape.points.size(); i++) {
            values += shape.points[i].toStr() + " ";
        }

        check(shape, shape.name);
        string opacity = std::to_string(shape.fillOpacity);
        string strokeOpacity = std::to_string(shape.strokeOpacity);
        string strokeWidth = std::to_string(shape.strokeWidth);

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

    // Returns SVG: <path ... />
    static const string polygon(Shape shape)
    {
        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        string values = "";
        for (unsigned i = 0; i < shape.points.size() - 1; i++) {
            values += shape.points[i].toStr() + " L ";
        }
        values += shape.points[shape.points.size() - 1].toStr();

        check(shape, shape.name);
        string opacity = std::to_string(shape.fillOpacity);
        string strokeOpacity = std::to_string(shape.strokeOpacity);
        string strokeWidth = std::to_string(shape.strokeWidth);

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

    // Returns full SVG.
    static const string svg(int width, int height, const string &xml,
                            Metadata metadata)
    {
        string now = "";
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

// Color
class Color {

public:

    struct RGBA {
        int R{0}, G{0}, B{0}, A{0};

        RGBA() {};

        RGBA(int r, int g, int b, int a)
            : RGBA(vector<int>{r, g, b, a}) {}

        RGBA(vector<int> rgba)
        {
            R = 0; G = 0; B = 0; A = 0;

            switch (rgba.size()) {
            case 4:
                A = rgba[3];
            case 3:
                B = rgba[2];
            case 2:
                G = rgba[1];
            case 1:
                R = rgba[0];
                break;
            default:
                break;
            }

            R = R < 0 ? 0 : R % 256;
            G = G < 0 ? 0 : G % 256;
            B = B < 0 ? 0 : B % 256;
            A = A < 0 ? 0 : A % 256;
        }

        bool operator==(RGBA rgba)
        {
            return equal(rgba);
        }

        bool empty()
        {
            return R == 0 && G == 0 && B == 0 && A == 0;
        }

        bool equal(RGBA rgba)
        {
            return R == rgba.R && G == rgba.G && B == rgba.B && A == rgba.A;
        }

        std::string toStr(bool alpha = true)
        {
            return {
                std::to_string(R) + "," + std::to_string(G) + "," +
                std::to_string(B) + (alpha ? "," + std::to_string(A) : "")
            };
        }
    };

};

// Sketch
// Organizes SVG development.
class Sketch : public SVG, public Color {

public:

    // Returns basic SVG::Shape with Polygon base.
    static const SVG::Shape Shape(Base base, std::string label)
    {
        SVG::Shape shape;
        shape.name = label;
        shape.points = base.points();

        return shape;
    }

    // Returns SVG::polyline with Polygon base.
    static const string SvgPolyline(Base base,std::string label)
    {

        return SVG::polygon(Shape(base, label));
    }

    // Returns SVG::polyline with Polygon base.
    static const string SvgPolyline(Base base,std::string label, RGBA fill, RGBA stroke)
    {

        return SVG::polyline(SVG::Shape(label,
                                        SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                        SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                        1.0, base.points()));
    }

    // Returns SVG::polygon with Polygon base.
    static const string SvgPolygon(Base base,std::string label)
    {

        return SVG::polygon(Shape(base, label));
    }

    // Returns SVG::polygon with Polygon base.
    static const string SvgPolygon(Base base,std::string label, RGBA fill, RGBA stroke)
    {

        return SVG::polygon(SVG::Shape(label,
                                       SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                       SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                       1.0, base.points()));
    }

    // Returns SVG Elements.
    static const string Join(vector<Base> bases, string label = "")
    {
        std::string strShape{};
        for (auto item : bases) {
            strShape += Sketch::SvgPolygon(item, label);
        }

        return strShape;
    }

};

// Interpreter
// Conversion of text into commands for SVG construction.
class Interpreter {

    enum elementType {
        EMPTY, POINTS, LINE, TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, POLYGON, BASE
    };

    Strings labelType {
        "EMPTY", "POINTS", "LINE", "TRIANGLE", "RECTANGLE",
        "CIRCLE", "ELLIPSE", "POLYGON", "BASE"
    };

    const char SPACE = char(32);
    const string ERROR   = "\tERROR";
    const string WARNING = "\tWARNING";

public:

    Interpreter(){};
    ~Interpreter(){};

    string svg (string line, string& error)
    {
        if (line.empty()) {
            return {};
        }

        Points points;
        double angle;
        double sides;
        double width, height, length;
        double horizontalRadius;
        double verticalRadius;
        string label;

        string result{};
        string bkp = line;

        line = Trim(line, SPACE); // Remove spaces from the ends.
        std::transform(line.begin(), line.end(), line.begin(), ::toupper); // Format.

        // Check command.
        string command{};
        int element = EMPTY;
        for (unsigned i = 1; i < labelType.size(); i++) {
            command = labelType[i] + ":";   // Syntax.
            if (line.starts_with(command)) {
                element = i;
                break;
            }
        }

        if (element < POINTS || element > POLYGON) {
            error = bkp + WARNING + "[Ignore]";
            return result;
        }
        line = line.substr(command.size());

        // Check format.
        auto counter = std::count_if(line.begin(), line.end(),
                                     [](char c){ return c == '{'; });
        if (counter > 1) {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }

        counter -= std::count_if(line.begin(), line.end(),
                                 [](char c){ return c == '}'; });
        if (counter != 0) {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }

        counter = std::count_if(line.begin(), line.end(),
                                [](char c){ return c == '('; }) -
                  std::count_if(line.begin(), line.end(),
                                [](char c){ return c == ')'; });
        if (counter != 0) {
            error = bkp + ERROR + "[Parentheses]\n";
            return result;
        }

        // Check content between braces.
        // Expected: {(x0,y1)(xN,yN)}
        points.clear();
        line = Trim(line, SPACE);
        if (line.starts_with('{')) {
            line = LTrim(line, '{');
            auto arg = Split(line, '}');
            arg[0] = Replace(arg[0], '(', " (");
            for (auto str : Split(arg[0], SPACE)) {
                str = Trim(str, SPACE);
                if (str.starts_with('(')) {
                    str = Trim(Trim(str, '('), ')');
                    try {
                        // Convert to Point.
                        auto values = Split(str, ',');
                        if (values.size() == 2) {
                            points.push_back(Point(std::stod(values[0]),
                                                   std::stod(values[1])));
                        }
                    } catch (...) {
                        error = bkp + ERROR + "[Invalid numeric formatting!]\n";
                        return result;
                    }
                }
            }
        }

        // Check other arguments.
        for (auto str : Split(line, SPACE)) {
            vector<string> complements {
                "ANGLE", "SIDES", "WIDTH", "HEIGHT", "LENGTH", "HRADIUS", "VRADIUS"
            };
            for (unsigned i = 0; i < complements.size(); i++) {
                if (str.starts_with(complements[i])) {
                    try {
                        auto value = std::stod(str.substr(complements[i].size()));
                        switch (i) {
                        case 0:
                            angle = value;
                            break;
                        case 1:
                            sides = value;
                            break;
                        case 2:
                            width = value;
                            break;
                        case 3:
                            height = value;
                            break;
                        case 4:
                            length = value;
                            break;
                        case 5:
                            horizontalRadius = value;
                            break;
                        case 6:
                            verticalRadius = value;
                            break;
                        default:
                            break;
                        }
                    } catch (...) {
                        // pass
                    }
                }
            }
            if (str.starts_with("LABEL=")) {
                try {
                    label = str.substr(6);
                } catch (...) {
                    // pass
                }
            }
        }

        // TO DO

        label = label.empty() ? labelType[element] : label;
        switch (element) {
        case POINTS:
            if (points.size() > 1) {
                result = Sketch::SvgPolygon(IrregularPolygon(points), label);
            }
            break;
        case LINE:
            if (points.size() == 1) {
                result = Sketch::SvgPolygon(Line(points.front(), angle, length), label);
            }
            if (points.size() == 2) {
                result = Sketch::SvgPolygon(IrregularPolygon(points), label);
            }
            break;
        case TRIANGLE:
            if (points.size() == 3) {
                result = Sketch::SvgPolygon(IrregularPolygon(points), label);
            }
            break;
        case RECTANGLE:
            if (points.size() == 1) {
                result = Sketch::SvgPolygon(Rectangle(points.front(), width, height), label);
            } else if (points.size() == 4) {
                result = Sketch::SvgPolygon(IrregularPolygon(points), label);
            }
            break;
        default:
            error = bkp + WARNING + "[Incomplete or wrong syntax!]";
            result = {};
            break;
        }

        return result;
    }

};

// Generic

double Radians(double angle)
{
    return angle * std::numbers::pi / 180.0;
}

double Angle(double radians)
{
    return radians * 180.0 / std::numbers::pi;
}

// Returns the angle of the line (x0,y0)(x1,y1).
double Angle(double x0, double y0, double x1, double y1)
{
    double result = Angle(std::atan((y1 - y0) / (x1 - x0)));

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

// Angle between three points.
// Sign : True, respects the order of the vectors.
double Angle(Point origin, Point first, Point second, bool signal)
{
    auto angle1 = origin.angle(first);
    auto angle2 = origin.angle(second);

    if (signal) {
        return angle1 - angle2;
    }

    return std::max(angle1, angle2) - std::min(angle1, angle2);
}

// Return: value + radius * cos(angle).
double Cos(double value, double radius, double angle)
{
    return value + radius * std::cos(Radians(angle));
}

// Return: value + radius * sin(angle).
double Sin(double value, double radius, double angle)
{
    return value + radius * std::sin(Radians(angle));
}

// Returns the distance between two points.
double Distance(double x0, double y0, double x1, double y1)
{
    return std::sqrt(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2));
}

// Rounds value to N digits after decimal point.
// Number of Decimal Places < 0, returns the same value.
double Round(double value, int decimalPlaces)
{
    if (decimalPlaces < 0) {
        return value;
    }

    if (decimalPlaces == 0) {
        return static_cast<int>(value);
    }

    decimalPlaces = decimalPlaces > 10 ? 10 : decimalPlaces;

    return std::round(value * std::pow(10, decimalPlaces)) / std::pow(10, decimalPlaces);
}

// Round values.
// Number of Decimal Places < 0, returns the same value.
Numbers Round(Numbers values, int decimalPlaces)
{
    for (unsigned i = 0; i < values.size(); i++) {
        values[i] = Round(values[i], decimalPlaces);
    }

    return values;
}

// Round points.
// Number of Decimal Places < 0, returns the same value.
Points Round(Points points, int decimalPlaces)
{
    for (unsigned i = 0; i < points.size(); i++) {
        points[i] = points[i].round(decimalPlaces);
    }

    return points;
}

// Triangle area.
double TriangleArea(double x0, double y0, double x1, double y1, double x2, double y2)
{
    // Heron's formula
    auto a = Distance(x0, y0, x1, y1);
    auto b = Distance(x1, y1, x2, y2);
    auto c = Distance(x2, y2, x0, y0);
    auto s = (a + b + c) / 2.0;

    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

// Triangle area using points.
double TriangleArea(Point p1, Point p2, Point p3)
{
    return TriangleArea(p1.X.value, p1.Y.value,
                        p2.X.value, p2.Y.value,
                        p3.X.value, p3.Y.value);
}

// Triangle height.
double TriangleHeight(double x0, double y0, double x1, double y1, double x2, double y2)
{
    auto area = TriangleArea(x0, y0, x1, y1, x2, y2);
    auto h0 = 2 * area / Distance(x0, y0, x1, y1);
    auto h1 = 2 * area / Distance(x1, y1, x2, y2);
    auto h2 = 2 * area / Distance(x2, y2, x0, y0);

    return std::max(std::max(h0, h1), h2);
}

// Triangle height using points.
double TriangleHeight(Point p1, Point p2, Point p3)
{
    return TriangleHeight(p1.X.value, p1.Y.value,
                          p2.X.value, p2.Y.value,
                          p3.X.value, p3.Y.value);
}

// Calculates the point of intersection between two lines.
// Returns false if the lines are parallel or coincident.
// Line 1 (x0, y0) - (x1, y1),
// Line 2 (x2, y2) - (x3, y4).
bool LineIntersect(double x0, double y0, double x1, double y1,
                   double x2, double y2, double x3, double y3,
                   double &X, double &Y)
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

    // Lines do not intersect.
    return false;
}

// Calculates the point of intersection between two lines.
// Returns false if the lines are parallel or coincident.
bool LineIntersect(Line line1, Line line2, Point &point)
{
    return line1.intersection(line2, point);
}

// Perpendicular line passing through the point.
Line Perpendicular(Line line, Point point)
{
    return line.perpendicular(point);
}

// Compare groups.
template<typename T>
bool Equal(vector<T> group1, vector<T> group2, bool compareOrder)
{
    if (group1.size() != group2.size()) {
        return false;
    }

    if (compareOrder) {
        for (unsigned i = 0; i < group1.size(); i++) {
            if (!(group1[i] == group2[i])) {
                return false;
            }
        }
    }

    for (auto v1 : group1) {
        bool differentFromEveryone = true;
        for (auto v2 : group2) {
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

// Returns Points updated with the sum.
Points Sum(Points group, double value)
{
    for (unsigned i = 0; i < group.size(); i++) {
        group[i] += value;
    }

    return group;
}

// Returns Points updated with the sum.
Points Sum(Points group, Point point)
{
    for (unsigned i = 0; i < group.size(); i++) {
        group[i] += point;
    }

    return group;
}

// Sum all the distances, point by point.
double SumDistances(Points points)
{
    if (points.empty()) {
        return 0;
    }

    if (points.size() == 1) {
        return 0;
    }

    if (points.size() == 2) {
        return points.front().distance(points.back());
    }

    double sum = 0;
    for (unsigned i = 1; i < points.size(); i++) {
        sum += points[i].distance(points[i - 1]);
    }

    return sum;
}

// Sum : Point (Total X axis, Total Y axis).
Point Total(Points points)
{
    Point sum;
    for (auto p : points) {
        sum += p;
    }

    return sum;
}

// Average : Point (Total X axis / Points, Total Y axis / Points).
bool Average(Points points, Point &point)
{
    if (points.empty()) {
        point = Origin;
        return false;
    }

    point = Total(points) * (1.0 / points.size()) ;

    return true;
}

// Sort numbers.
Numbers Sort(Numbers numbers, bool ascendingOrder)
{
    if (ascendingOrder) {
        std::sort(numbers.begin(), numbers.end(), std::less<double>());
    }
    else {
        std::sort(numbers.begin(), numbers.end(), std::greater<double>());
    }

    return numbers;
}

// Sort points by X or Y axis.
Points Sort(Points points, bool X_axis)
{
    if (points.empty()) {
        return {};
    }

    if (points.size() == 1) {
        return points;
    }

    std::map<double, Numbers > mapPoint;

    for (auto p : points) {
        auto key = X_axis ? p.X.value : p.Y.value;
        auto value = X_axis ? p.Y.value : p.X.value;
        if (mapPoint.find(key) == mapPoint.end()) {
            mapPoint.insert({key, {value}});
        }
        else {
            mapPoint[key].push_back(value);
        }
    }

    Points result;
    for (auto item : mapPoint) {
        if (X_axis) {
            for (auto value : item.second) { // Y
                result.push_back(Point(item.first, value));
            }
        }
        else {
            auto values = Sort(item.second);
            for (auto value : values) {     // X
                result.push_back(Point(value, item.first));
            }
        }
    }

    return result;
}

// Sort the Points clockwise.
Points Organize(Points points)
{
    if (points.size() < 2) {
        return points;
    }

    // Map : Angle x Point.
    std::map<float, Points > mapPoint;

    for (auto value : points) {
        float key = Point(0, 0).angle(value);
        if (mapPoint.find(key) == mapPoint.end()) {
            mapPoint.insert(std::make_pair(key, Points{value}));
        }
        else {
            mapPoint[key].push_back(value);
        }
    }

    Points result;
    for (auto item : mapPoint) {
        for (auto vPoints : item.second) {
            result.push_back(vPoints);
        }
    }

    return result;
}

// Join strings.
string Join(Strings vStr, const char delimiter)
{
    string result{};
    for (unsigned i = 0; i < vStr.size(); i++) {
        result += vStr[i] + (i < vStr.size() - 1 ? string{delimiter} : "");
    }

    return result;
}

// Trim string : Remove characters to the left.
string LTrim(string str, const char trimmer)
{
    int left = 0;
    int right = str.size() - 1;
    right = right < 0 ? 0 : right;
    while (left < str.size()) {
        if (str[left] != trimmer) {
            break;
        }
        left++;
    }

    return str.substr(left, 1 + right - left);
}

// Trim string : Remove characters to the right.
string RTrim(string str, const char trimmer)
{
    int left = 0;
    int right = str.size() - 1;
    right = right < 0 ? 0 : right;
    while (right >= 0) {
        if (str[right] != trimmer) {
            break;
        }
        right--;
    }

    return str.substr(left, 1 + right - left);

}

// Trim string.
string Trim(string str, const char trimmer)
{
    return RTrim(LTrim(str, trimmer), trimmer);
}

// Trim strings.
Strings Trim(Strings vStr, const char trimmer)
{
    for (unsigned i = 0; i < vStr.size(); ++i) {
        vStr[i] = Trim(vStr[i], trimmer);
    }

    return vStr;
}

// Replace all occurrences.
string Replace(string str, char character, string replace)
{
    string result{};
    for (unsigned i = 0; i < str.size(); ++i) {
        result += (str[i] == character ? replace : string{str[i]});
    }

    return result;
}

// Replace all occurrences.
string Replace(string str, char character, char replace)
{
    return Replace(str, character, string{replace});
}

// Split string.
Strings Split(string str, const char delimiter)
{
    Strings result;
    string s{};
    for (unsigned i = 0; i < str.size(); ++i) {
        if (str[i] == delimiter) {
            result.push_back(s);
            s = "";
        }
        else {
            s += str[i];
        }
    }
    if (!s.empty()) {
        result.push_back(s);
    }

    return result;
}

// Input/Output

// Std::cout : double.
void View(double value)
{
    std::cout << std::to_string(value) << '\n';
}

// Std::cout : Point(x,y).
void View(Point point)
{
    std::cout << "(" << point.toStr() << ")" << '\n';
}

// Std::cout : Array of double.
template<std::size_t SIZE>
void View(std::array<double, SIZE> arr)
{
    unsigned i = 0;
    for (const auto &value : arr) {
        std::cout << "[" << i << "]: " << Round(value) << '\n';
        i++;
    }
}

// Std::cout : Vector of Point(x,y).
void View(Points points)
{
    string str{};
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

// Std::cout : Vector of numbers.
template<typename T>
void View(vector<T> values)
{
    string str{};
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

// Std::cout : Vector of strings.
void View(Strings values)
{
    string str{};
    for (unsigned i = 0; i < values.size(); i++) {
        str += values[i] + (i < values.size() - 1 ? "," : "");
    }

    if (str.empty()) {
        std::cout << "Empty\n";
    }
    else {
        std::cout << str << '\n';
    }
}

// Std::cout : string.
void View(string str)
{
    std::cout << str << '\n';
}

// Load text file.
string Load(string path, string filenameExtension)
{
    if (path.empty()) {
        return {};
    }

    // Check extension.
    if (!filenameExtension.empty()) {
        const std::filesystem::path p(path);
        if (!std::filesystem::exists(p)) {
            std::cerr << "File not found!\n";
            return {};
        }

        std::string extension{p.extension()};
        std::transform(extension.begin(), extension.end(),
                       extension.begin(), ::toupper);
        std::transform(filenameExtension.begin(), filenameExtension.end(),
                       filenameExtension.begin(), ::toupper);
        if (!extension.ends_with(filenameExtension)) {
            std::cerr << "Invalid file!\n";
            return {};
        }
    }

    string str{};
    try {
        std::ifstream fileIn(path, std::ios::in);
        if (fileIn.is_open()) {
            string line{};
            while (getline(fileIn, line)) {
                str += line + "\n";
            }
            fileIn.close();
        }
    }
    catch (...) {
        // pass
    }

    return str;
}

// Save text file.
bool Save(const string &text, string path)
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
