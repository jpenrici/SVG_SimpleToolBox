#ifndef SMALLTOOLBOX_H_
#define SMALLTOOLBOX_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numbers>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace smalltoolbox {

#define Numbers     std::vector<double>
#define Point2D     smalltoolbox::Point
#define Points      std::vector<smalltoolbox::Point>
#define Polygon     smalltoolbox::RegularPolygon
#define Strings     std::vector<std::string>
#define PI          std::numbers::pi
#define MAXDOUBLE   std::numeric_limits<double>::max()

using std::array;
using std::cerr;
using std::cout;
using std::exception;
using std::greater;
using std::ifstream;
using std::ios;
using std::less;
using std::map;
using std::max;
using std::min;
using std::move;
using std::ofstream;
using std::string;
using std::to_string;
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

auto Organize(Points points) -> Points;
auto Round(Points points, int decimalPlaces = -1) -> Points;
auto Sort(Points points, bool X_axis = true) -> Points;
auto Sum(Points group, Point point) -> Points;
auto Sum(Points group, double value) -> Points;
auto Total(Points const &points) -> Point;

auto Average(Points const &points, Point &point) -> bool;

auto Perpendicular(Line line, Point point) -> Line;

auto LineIntersect(Line line1, const Line &line2, Point &point) -> bool;
auto LineIntersect(double x0, double y0, double x1, double y1, double x2,
                   double y2, double x3, double y3, Point &point) -> bool;

auto TriangleArea(Point point1, Point point2, Point point3) -> double;
auto TriangleArea(double x0, double y0, double x1, double y1, double x2, double y2) -> double;
auto TriangleHeight(Point point1, Point point2, Point point3) -> double;
auto TriangleHeight(double x0, double y0, double x1, double y1, double x2, double y2) -> double;

auto Angle(Point origin, Point first, Point second, bool signal = false) -> double;
auto Angle(double radians) -> double;
auto Angle(double x0, double y0, double x1, double y1) -> double;
auto Cos(double value, double radius, double angle) -> double;
auto Sin(double value, double radius, double angle) -> double;
auto Radians(double angle) -> double;
auto Distance(double x0, double y0, double x1, double y1) -> double;
auto SumDistances(Points points) -> double;

template<typename T>
auto Equal(vector<T> group1, vector<T> group2, bool compareOrder = false) -> bool;

auto Sort(Numbers numbers, bool ascendingOrder = true) -> Numbers;
auto Round(Numbers values, int decimalPlaces = -1)  -> Numbers;
auto Round(double value, int decimalPlaces = -1) -> double;

// String
auto Split(const string &str, char delimiter) -> Strings;
auto Trim(Strings vStr, char trimmer) -> Strings;

auto Replace(const string &str, char character, const string &replace) -> string;
auto Replace(const string &str, char character, char replace) -> string;
auto LTrim(string str, char trimmer) -> string;
auto RTrim(string str, char trimmer) -> string;
auto Trim(string str, char trimmer) -> string;
auto Join(Strings vStr, char delimiter) -> string;

// View
void View(Point point);
void View(Points points);
void View(Strings values);
void View(double value);
void View(const string &str);

template<typename T>
void View(vector<T> values);

template<size_t SIZE>
void View(array<double, SIZE> arr);

// I/O
auto Save(const string &text, string filePath = "") -> bool;
auto Load(const string &filePath, string filenameExtension = "") -> string;

// Point 2D (x,y)
class Point {

    struct Coordinate {
        double value{0};

        auto toStr() const -> string
        {
            return to_string(value);
        }
    };

public:

    // Axis
    Coordinate X, Y;

    // Point (0, 0)
    Point() : X{0}, Y{0} {};

    // Point (x, y)
    Point(double x, double y) : X{x}, Y{y} {};

    ~Point() = default;

    auto operator+(Point point) const -> Point
    {
        return {X.value + point.X.value, Y.value + point.Y.value};
    }

    auto operator+(double value) const -> Point
    {
        return {X.value + value, Y.value + value};
    }

    void operator+=(Point point)
    {
        sum(point.X.value, point.Y.value);
    }

    void operator+=(double value)
    {
        sum(value, value);
    }

    auto operator-(const Point point) const -> Point
    {
        return {X.value - point.X.value, Y.value - point.Y.value};
    }

    auto operator-(double value) const -> Point
    {
        return {X.value - value, Y.value - value};
    }

    void operator-=(const Point point)
    {
        sum(-point.X.value, -point.Y.value);
    }

    void operator-=(double value)
    {
        sum(-value, -value);
    }

    auto operator*(const Point point) const -> Point
    {
        return {X.value * point.X.value, Y.value * point.Y.value};
    }

    auto operator*(double value) const -> Point
    {
        return {X.value * value, Y.value * value};
    }

    void operator*=(const Point point)
    {
        multiply(point.X.value, point.Y.value);
    }

    void operator*=(double value)
    {
        multiply(value, value);
    }

    auto operator==(const Point point) const -> bool
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
    auto equal(Point point) const -> bool
    {
        return X.value == point.X.value && Y.value == point.Y.value;
    }

    // Angle of the imaginary line between the current point and the other.
    auto angle(Point point) const -> double
    {
        return Angle(X.value, Y.value, point.X.value, point.Y.value);
    }

    // Distance between the current point and another.
    auto distance(Point point) const -> double
    {
        return sqrt(pow(X.value - point.X.value, 2) +
                    pow(Y.value - point.Y.value, 2));
    }

    // Position from angle and radius.
    // Current point as origin.
    auto position(double angle, double radius) const -> Point
    {
        return {Cos(X.value, radius, angle), Sin(Y.value, radius, angle)};
    }

    // Returns the rounded current coordinates.
    auto round(int decimalPlaces = 2) const -> Point
    {
        return {Round(X.value, decimalPlaces), Round(Y.value, decimalPlaces)};
    }

    // Returns Vector[2] with X and Y values.
    auto XY() -> Numbers
    {
        return {X.value, Y.value};
    }

    auto toStr(bool trimmed = false) const -> string
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

    auto state() -> bool
    {
        return (!first.equal(last_first) || !second.equal(last_second) ||
                !third.equal(last_third) || !fourth.equal(last_fourth));
    }

public:

    Point first, second, third, fourth;

    Base() = default;
    ~Base() = default;

    auto setup(const Points &points) -> Points
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

    auto operator==(const Base &polygon) -> bool
    {
        return equal(polygon);
    }

    auto isConvex() -> bool
    {
        auto sides = vertices.size();
        if (sides < 3) {
            return false;
        }

        // Cross product of two vectors.
        auto crossProduct = [](Point origin, Point first, Point second) {
            auto x1 = first.X.value  - origin.X.value;
            auto y1 = first.Y.value  - origin.Y.value;
            auto x2 = second.X.value - origin.X.value;
            auto y2 = second.Y.value - origin.Y.value;
            return x1 * y2 - y1 * x2;
        };

        double previous = 0;
        for (unsigned i = 0; i < sides; i++) {
            auto product = crossProduct(vertices[i],
                                        vertices[(i + 1) % sides],
                                        vertices[(i + 2) % sides]);
            if (product != 0) {
                if (product * previous >= 0) {
                    previous = product;
                }
                else {
                    return false;
                }
            }
        }

        return true;
    }

    // Rearrange the polygon points.
    auto organize() -> Points
    {
        return Organize(vertices);
    }

    // Calculates the Area by triangular subdivisions.
    auto area() -> double
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

    auto perimeter() -> double
    {
        auto perimeter = SumDistances(vertices);

        if (vertices.size() > 2) {
            perimeter += vertices.back().distance(vertices.front());
        }

        return perimeter;
    }

    auto equal(Base polygon) -> bool
    {
        return Equal(points(), polygon.points());
    }

    auto round(int decimalPlaces = 2) -> Base
    {
        Base polygon;
        polygon.vertices = Round(vertices, decimalPlaces);

        return polygon;
    }

    // Return the length of each side.
    auto lengthOfSides() -> Numbers
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
    auto averageLength() -> double
    {
        if (!vertices.empty()) {
            return perimeter() / static_cast<double>(vertices.size());
        }

        return 0;
    }

    // Returns the current vertices.
    auto points() -> Points
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

    Line() = default;

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

    ~Line() = default;

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    auto setup(Point first, Point second = Origin) -> Points
    {
        Base::setup({first, second});

        return points();
    }

    // Line : Point (x,y), angle and length.
    // Returns vertices.
    auto setup(Point origin, double angle, double length) -> Points
    {
        return setup(origin, origin.position(angle, length));
    }

    // Line : Point 1 (x1,y1) to Point 2 (x2,y2).
    // Returns vertices.
    auto setup(double x1, double y1, double x2, double y2) -> Points
    {
        return setup(Point(x1, y1), Point(x2, y2));
    }

    // Distance between the two points.
    auto length() -> double
    {
        return perimeter();
    }

    // Line angle.
    // Direction: first point to second point.
    auto angle() -> double
    {
        // Update
        auto vertices = points();

        if (vertices.size() < 2) {
            return 0;
        }

        return first.angle(second);
    }

    // Midpoint
    auto middle() -> Point
    {
        // Update
        points();

        return first.position(angle(), length() / 2);
    }

    // Returns the intersection point with another line.
    auto intersection(const Line &line, Point &point) -> bool
    {
        return LineIntersect(first.X.value, first.Y.value,
                             second.X.value, second.Y.value,
                             line.first.X.value, line.first.Y.value,
                             line.second.X.value, line.second.Y.value,
                             point);
    }

    // Perpendicular line passing through the point.
    auto perpendicular(Point point) -> Line
    {
        // Dummy triangle area.
        double area = TriangleArea(first.X.value, first.Y.value,
                                   second.X.value, second.Y.value,
                                   point.X.value, point.Y.value);
        // Pythagorean theorem : a^2 + b^2 = c^2
        double c = first.distance(point);               // hypotenuse
        double b = 2 * area / first.distance(second);   // area = base * height / 2
        double a = sqrt(pow(c, 2) - pow(b, 2));

        // Line with base slope.
        return {first.position(angle(), a), point};
    }

};

// Triangle (x1,y1)(x2,y2)(x3,y3)
class Triangle : public Base {

public:

    Triangle() = default;

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

    ~Triangle() = default;

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    // Returns vertices.
    auto setup(Point first, Point second, Point third) -> Points
    {
        Base::setup({first, second, third});

        return points();
    }

    // Triangle: Line and height.
    // Returns vertices.
    auto setup(Line side, double height) -> Points
    {
        return setup(side.first,
                     side.second,
                     Point(side.middle()).position(90 + side.angle(), height));
    }

    // Triangle: Points (x1,y1),(x2,y2) and height.
    // Returns vertices.
    auto setup(Point first, Point second, double height) -> Points
    {
        return setup(Line(first, second), height);
    }

    auto height() -> double
    {
        return TriangleHeight(first, second, third);
    }

};

// Rectangle (x1,y1)(x2,y2)(x3,y3)(x4,y4)
class Rectangle : public Base {

public:

    Rectangle() = default;

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

    ~Rectangle() = default;

    // Rectangle: Points (x1,y1),(x2,y2),(x3,y3),(x4,y4)
    // Returns vertices.
    auto setup(Point first, Point second, Point third, Point fourth) -> Points
    {
        Base::setup({first, second, third, fourth});

        return points();
    }

    // Rectangle : Point (x,y), width and heigth.
    // Returns vertices.
    auto setup(Point origin, double width, double heigth) -> Points
    {
        return setup(origin,
                     origin + Point(width, 0),
                     origin + Point(width, heigth),
                     origin + Point(0, heigth));
    }

};

// Regular Polygon (x,y)...(xN,yN)
class RegularPolygon : public Base {

    Point last_center{0, 0};
    double last_angle{0};
    double last_horizontalRadius{0};
    double last_verticalRadius{0};
    unsigned last_sides{0};

    void update()
    {
        // If there has been modification in the parameters.
        if (state()) {
            setup(center, horizontalRadius, verticalRadius, angle, sides);
        }
    }

public:

    Point center{0, 0};
    double angle{0};
    double horizontalRadius{0};
    double verticalRadius{0};
    unsigned sides{0};

    // Returns true if data changes.
    auto state() -> bool
    {
        return !(center == last_center &&
                 angle == last_angle &&
                 horizontalRadius == last_horizontalRadius &&
                 verticalRadius == last_verticalRadius &&
                 sides == last_sides);
    }

    RegularPolygon() = default;

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

    ~RegularPolygon() = default;

    // Center : polygon center point (x,y),
    // horizontalRadius : distance from the center on X axis (>= 1),
    // verticalRadius   : distance from the center on Y axis (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    // Returns vertices.
    auto setup(Point center, double horizontalRadius, double verticalRadius,
               double angle, unsigned sides) -> Points
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
        int begin = static_cast<int>(angle);
        int end = 360 + static_cast<int>(angle);
        int step = 360 / static_cast<int>(sides);
        for (auto a = begin; a < end; a += step) {
            points.emplace_back(Point(Cos(center.X.value, horizontalRadius, a),
                                      Sin(center.Y.value, verticalRadius, a)));
        }

        return Base::setup(points);
    }

    // Center : polygon center point (x,y),
    // radius : distance from the center (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    auto setup(Point center, double radius, double angle, unsigned sides) -> Points
    {
        return setup(center, radius, radius, angle, sides);
    }

    auto sideLength(int decimalPlaces = 2) -> double
    {
        if (sides < 2) {
            return 0;
        }

        auto result = first.distance(second);

        return decimalPlaces < 0 ? result : Round(result, decimalPlaces);
    }

    // Returns the current vertices.
    auto points() -> Points
    {
        update();

        return Base::points();
    }

};

class Ellipse : public RegularPolygon {

public:

    Ellipse() = default;

    Ellipse(Point center, double horizontalRadius, double verticalRadius)
    {
        setup(center, horizontalRadius, verticalRadius);
    }

    ~Ellipse() = default;

    auto setup(Point center, double horizontalRadius, double verticalRadius) -> Points
    {
        RegularPolygon::setup(center, horizontalRadius, verticalRadius, 0, 360);

        return points();
    }

    auto area() -> double
    {
        return horizontalRadius * verticalRadius * PI;
    }

    auto perimeter() -> double
    {
        // TO DO
        return -1;
    }
};

class Circle : public Ellipse {

public:

    Circle() = default;

    Circle(Point center, double radius)
    {
        setup(center, radius);
    }

    ~Circle() = default;

    auto setup(Point center, double radius) -> Points
    {
        Ellipse::setup(center, radius, radius);

        return points();
    }

};

// Irregular Polygon (x,y)...(xN,yN)
class IrregularPolygon : public Base {

public:

    IrregularPolygon() = default;

    explicit IrregularPolygon(const Points &points)
    {
        setup(points);
    }

    explicit IrregularPolygon(Triangle triangle)
    {
        setup(triangle.points());
    }

    explicit IrregularPolygon(Rectangle rectangle)
    {
        setup(rectangle.points());
    }

    explicit IrregularPolygon(RegularPolygon polygon)
    {
        setup(polygon.points());
    }

    ~IrregularPolygon() = default;
};

// SVG
class SVG {

public:

    static constexpr const char *WHITE = "#FFFFFF";
    static constexpr const char *BLACK = "#000000";
    static constexpr const char *RED   = "#FF0000";
    static constexpr const char *GREEN = "#00FF00";
    static constexpr const char *BLUE  = "#0000FF";

    // Metadata setup.
    // creator             : String with the name of the creator or developer,
    // title               : String with title,
    // publisherAgentTitle : String with the name of the publisher,
    // date                : String with creation date, if empty use current date.
    struct Metadata {
        string creator = "SVG created automatically by algorithm in C++.";
        string title = "SVG";
        string publisherAgentTitle;
        string date;

        Metadata() = default;
        Metadata(string creator, string title, string publisher)
            : creator{move(creator)}, title{move(title)}, publisherAgentTitle{move(publisher)} {}
    };

    // Drawing setup.
    // name          : ID used in SVG element,
    // fill          : Fill color in hexadecimal string format (#FFFFFF),
    // stroke        : Stroke color in hexadecimal string format (#FFFFFF),
    // strokeWidth   : Line width,
    // fillOpacity   : Fill opacity or alpha value from 0 to 255.
    // strokeOpacity : Stroke opacity or alpha value from 0 to 255.
    struct Style {
        string name{"Shape"}, fill{WHITE}, stroke{BLACK};
        double strokeWidth{1.0};
        double fillOpacity{255.0}, strokeOpacity{255.0}; // 0.0 = 0%; 255 = 1.0 = 100%

        Style() = default;

        Style(string name,  string fill, string stroke, double strokeWidth)
            : name{move(name)}, fill{move(fill)}, stroke{move(stroke)}, strokeWidth{strokeWidth} {}
        Style(string name,  string fill, string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity)
            : name{move(name)}, fill{move(fill)}, stroke{move(stroke)}, strokeWidth{strokeWidth},
            fillOpacity{fillOpacity}, strokeOpacity{strokeOpacity} {}
    };

    // Polygon and Polyline.
    // points : Points vector (x,y).
    struct Shape : Style {
        Points points{};

        Shape() = default;

        Shape(string name,  string fill, string stroke, double strokeWidth)
            : Style(move(name), move(fill), move(stroke), strokeWidth) {}
        Shape(string name,  string fill, string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity)
            :  Style(move(name),  move(fill), move(stroke), strokeWidth, fillOpacity, strokeOpacity) {}

        Shape(string name,  string fill, string stroke, double strokeWidth, Points points)
            :  Style(move(name),  move(fill), move(stroke), strokeWidth), points{move(points)} {}
        Shape(string name,  string fill, string stroke, double strokeWidth,
              double fillOpacity, double strokeOpacity, Points points)
            :  Style(move(name),  move(fill), move(stroke), strokeWidth, fillOpacity, strokeOpacity),
            points{move(points)} {}
    };

    // Circle and Ellipse.
    // center           : Central point, (x,y)
    // horizontalRadius : Horizontal radius of circle, rx
    // verticalRadius   : Vertical radius of circle, ry
    struct CircleShape : Style {
        Point center{Origin};
        double horizontalRadius{1};
        double verticalRadius{1};

        CircleShape() = default;

        CircleShape(string name,  string fill, string stroke, double strokeWidth)
            : Style(move(name),  move(fill), move(stroke), strokeWidth) {}
        CircleShape(string name,  string fill, string stroke, double strokeWidth,
                    double fillOpacity, double strokeOpacity)
            :  Style(move(name),  move(fill), move(stroke), strokeWidth, fillOpacity, strokeOpacity) {}

        CircleShape(string name,  string fill, string stroke, double strokeWidth,
                    Point center, double horizontalRadius, double verticalRadius)
            :  Style(move(name),  move(fill), move(stroke), strokeWidth), center(center),
            horizontalRadius(horizontalRadius), verticalRadius(verticalRadius) {}
        CircleShape(string name,  string fill, string stroke, double strokeWidth,
                    double fillOpacity, double strokeOpacity,
                    Point center, double horizontalRadius, double verticalRadius)
            :  Style(move(name),  move(fill), move(stroke), strokeWidth, fillOpacity, strokeOpacity),
            center(center), horizontalRadius(horizontalRadius), verticalRadius(verticalRadius) {}
    };

    // Converts decimal value to hexadecimal.
    static auto INT2HEX(unsigned value) -> string
    {
        string digits = "0123456789ABCDEF";
        string result;
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
    static auto HEX2INT(string value) -> unsigned
    {
        if (value.starts_with('#')) {
            value = value.substr(1);
        }

        if (value.empty()) {
            return 0;
        }

        unsigned result = 0;
        try {
            result = stoul(value, nullptr, 16);
        }
        catch (...) {
            // pass
        }

        return result;
    }

    // Return vector unsigned {Red, Green, Blue, Alpha}
    static auto HEX2RGB(string value) -> vector<unsigned>
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
                result.push_back(static_cast<unsigned>(HEX2INT(num)));
            }
        }

        return result;
    }

    // Formats values (Red, Green, Blue) to "#RRGGBB" hexadecimal.
    static auto RGB2HEX(unsigned R, unsigned G, unsigned B) -> string
    {
        return "#" + INT2HEX(R) + INT2HEX(G) + INT2HEX(B);
    }

    // Formats values (Red, Green, Blue, Alpha) to "#RRGGBBAA" hexadecimal.
    static auto RGBA2HEX(unsigned R, unsigned G, unsigned B, unsigned A) -> string
    {
        return RGB2HEX(R, G, B) + INT2HEX(A);
    }

    // Returns SVG: <g> Elements </g>
    static auto group(string id, const string &elements) -> string
    {
        id = id.empty() ? "<g>\n" : "<g id=\"" + id + "\" >\n";
        return elements.empty() ? "" : id + elements + "</g>\n";
    }

private:

    // Validates and formats entries.
    static auto style(Style style, const string &name) -> string
    {
        style.name = name.empty() ? "Shape" : name;
        style.stroke = style.stroke.empty() ? "#000000" : style.stroke;
        style.fillOpacity = style.fillOpacity < 0 ? 0 : min(style.fillOpacity / 255, 1.0);
        style.strokeOpacity = style.strokeOpacity < 0 ? 0 : min(style.strokeOpacity / 255, 1.0);

        return {
                "id=\"" + style.name + "\"\nstyle=\"" +
                "opacity:" + to_string(style.fillOpacity) + ";fill:" + style.fill +
                ";stroke:" + style.stroke + ";stroke-width:" + to_string(style.strokeWidth) +
                ";stroke-opacity:" + to_string(style.strokeOpacity) +
                ";stroke-linejoin:round;stroke-linecap:round\"\n" };
    }

public:

    // Return SVG: <polyline ... />
    static auto polyline(const Shape &shape) -> string
    {

        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        string values;
        for (const auto &point : shape.points) {
            values += point.toStr() + " ";
        }

        return {
            "<polyline\n" + style(shape, shape.name) + "points=\"" + values + "\" />\n"
        };
    }

    // Return SVG: <path ... />
    static auto polygon(Shape shape) -> string
    {
        if (shape.points.empty()) {
            return "<!-- Empty -->\n";
        }

        string values;
        for (unsigned i = 0; i < shape.points.size() - 1; i++) {
            values += shape.points[i].toStr() + " L ";
        }
        values += shape.points[shape.points.size() - 1].toStr();

        return {
            "<path\n" + style(shape, shape.name) + "d=\"M " + values + " Z\" />\n"
        };
    }

    // Return SVG : <ellipse ... />
    static auto circle(const CircleShape &circle) -> string
    {
        if (circle.horizontalRadius < 1 || circle.verticalRadius < 1) {
            return "<!-- Empty -->\n";
        }
        return {
            "<ellipse\n" + style(circle, circle.name) +
            "cx=\"" + circle.center.X.toStr() + "\" " +
            "cy=\"" + circle.center.Y.toStr() + "\" " +
            "rx=\"" + to_string(circle.horizontalRadius) + "\" " +
            "ry=\"" + to_string(circle.verticalRadius) + "\" />\n"
        };
    }

    // Return full SVG.
    static auto svg(int width, int height, const string &xml,
                    Metadata metadata) -> string
    {
        string now;
        try {
            time_t t = time(nullptr);
            tm *const pTm = localtime(&t);
            now = to_string(1900 + pTm->tm_year);
        }
        catch (...) {
            // pass
        }

        metadata.date = metadata.date.empty() ? now : metadata.date;

        return {
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
            "<svg\n"
            "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
            "xmlns:cc=\"http://creativecommons.org/ns#\"\n"
            "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
            "xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
            "xmlns=\"http://www.w3.org/2000/svg\"\n"
            "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
            "width=\"" + to_string(width) + "\"\n" +
            "height=\"" + to_string(height) + "\"\n" +
            "viewBox= \"0 0 " + to_string(width) + " " + to_string(height) + "\"\n" +
            "version=\"1.1\"\n" +
            "id=\"svg8\">\n" +
            "<title\n" +
            "id=\"title1\">" + metadata.title + "</title>\n" +
            "<defs\n" +
            "id=\"defs1\" />\n" +
            "<metadata\n" +
            "id=\"metadata1\">\n" +
            "<rdf:RDF>\n" +
            "<cc:Work\n" +
            "rdf:about=\"\">\n" +
            "<dc:format>image/svg+xml</dc:format>\n" +
            "<dc:type\n" +
            "rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n" +
            "<dc:title>" + metadata.title + "</dc:title>\n" +
            "<dc:date>" + metadata.date + "</dc:date>\n" +
            "<dc:publisher>\n" +
            "<cc:Agent>\n" +
            "<dc:title>" + metadata.publisherAgentTitle + "</dc:title>\n" +
            "</cc:Agent>\n" +
            "</dc:publisher>\n" +
            "<dc:subject>\n" +
            "<rdf:Bag>\n" +
            "<rdf:li></rdf:li>\n" +
            "<rdf:li></rdf:li>\n" +
            "<rdf:li></rdf:li>\n" +
            "<rdf:li></rdf:li>\n" +
            "</rdf:Bag>\n" +
            "</dc:subject>\n" +
            "<dc:creator>\n" +
            "<cc:Agent>\n" +
            "<dc:title>" + metadata.creator + "</dc:title>\n" +
            "</cc:Agent>\n" +
            "</dc:creator>\n" +
            "<cc:license\n" +
            "rdf:resource=\"http://creativecommons.org/publicdomain/zero/1.0/\" />\n" +
            "<dc:description>SVG created automatically by algorithm in C++.</dc:description>\n" +
            "</cc:Work>\n" +
            "<cc:License\n" +
            "rdf:about=\"http://creativecommons.org/publicdomain/zero/1.0/\">\n" +
            "<cc:permits\n" +
            "rdf:resource=\"http://creativecommons.org/ns#Reproduction\" />\n" +
            "<cc:permits\n" +
            "rdf:resource=\"http://creativecommons.org/ns#Distribution\" />\n" +
            "<cc:permits\n" +
            "rdf:resource=\"http://creativecommons.org/ns#DerivativeWorks\" />\n" +
            "</cc:License>\n" +
            "</rdf:RDF>\n" +
            "</metadata>\n" +
            "<!--      Created in C++ algorithm       -->\n" +
            "<!-- Attention: do not modify this code. -->\n" +
            "\n" + xml + "\n" +
            "<!-- Attention: do not modify this code. -->\n" +
            "</svg>"
        };
    }
};

// Color
class Color {

public:

    struct RGBA {
        int R{0}, G{0}, B{0}, A{0};

        RGBA() = default;

        RGBA(int r, int g, int b, int a)
            : RGBA(vector<int>
                   {
                       r, g, b, a
                   }) {}

        explicit RGBA(vector<int> rgba)
            : RGBA()
        {
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

        auto operator==(RGBA rgba) const -> bool
        {
            return equal(rgba);
        }

        auto empty() const -> bool
        {
            return R == 0 && G == 0 && B == 0 && A == 0;
        }

        auto equal(RGBA rgba) const  ->bool
        {
            return R == rgba.R && G == rgba.G && B == rgba.B && A == rgba.A;
        }

        auto toStr(bool alpha = true) const  ->string
        {
            return {
                to_string(R) + "," + to_string(G) + "," +
                to_string(B) + (alpha ? "," + to_string(A) : "")
            };
        }
    };

};

// Sketch
// Organizes SVG development.
class Sketch : public SVG, public Color {

public:

    // Return basic SVG::Shape with Polygon base.
    static auto Shape(Base base, string label) -> SVG::Shape
    {
        SVG::Shape shape;
        shape.name = move(label);
        shape.points = base.points();

        return shape;
    }

    // Return basic SVG::CircleShape with Ellipse base.
    static auto CircleShape(const Ellipse &ellipse, string label) -> SVG::CircleShape
    {
        SVG::CircleShape shape;
        shape.name = move(label);
        shape.center = ellipse.center;
        shape.horizontalRadius = ellipse.horizontalRadius;
        shape.verticalRadius = ellipse.verticalRadius;

        return shape;
    }

    // Return SVG::polyline with Polygon base.
    static auto SvgPolyline(const Base &base, string label) -> string
    {

        return SVG::polygon(Shape(base, move(label)));
    }

    // Return SVG::polyline with Polygon base.
    static auto SvgPolyline(Base base, string label, RGBA fill, RGBA stroke) -> string
    {

        return SVG::polyline(SVG::Shape(move(label),
                                        SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                        SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                        1.0,    // strokeWidth
                                        base.points()));
    }

    // Return SVG::polyline with Polygon base.
    static auto SvgPolyline(Base base, string label, RGBA fill, RGBA stroke,
                            double fillOpacity, double strokeOpacity) -> string
    {

        return SVG::polyline(SVG::Shape(move(label),
                                        SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                        SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                        1.0,    // strokeWidth
                                        fillOpacity,
                                        strokeOpacity,
                                        base.points()));
    }

    // Return SVG::polyline with Polygon base.
    static auto SvgPolyline(Base base, string label, RGBA fill, RGBA stroke,
                            double fillOpacity, double strokeOpacity,
                            double strokeWidth) -> string
    {

        return SVG::polyline(SVG::Shape(move(label),
                                        SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                        SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                        strokeWidth,
                                        fillOpacity,
                                        strokeOpacity,
                                        base.points()));
    }

    // Return SVG::polygon with Polygon base.
    static auto SvgPolygon(const Base &base, string label) -> string
    {

        return SVG::polygon(Shape(base, move(label)));
    }

    // Return SVG::polygon with Polygon base.
    static auto SvgPolygon(Base base, string label, RGBA fill, RGBA stroke) -> string
    {

        return SVG::polygon(SVG::Shape(move(label),
                                       SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                       SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                       1.0, // strokeWidth
                                       base.points()));
    }

    // Return SVG::polygon with Polygon base.
    static auto SvgPolygon(Base base, string label, RGBA fill, RGBA stroke,
                           double fillOpacity, double strokeOpacity) -> string
    {

        return SVG::polygon(SVG::Shape(move(label),
                                       SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                       SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                       1.0, // strokeWidth
                                       fillOpacity,
                                       strokeOpacity,
                                       base.points()));
    }

    // Return SVG::polygon with Polygon base.
    static auto SvgPolygon(Base base, string label, RGBA fill, RGBA stroke,
                           double fillOpacity, double strokeOpacity,
                           double strokeWidth) -> string
    {

        return SVG::polygon(SVG::Shape(move(label),
                                       SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                       SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                       strokeWidth,
                                       fillOpacity,
                                       strokeOpacity,
                                       base.points()));
    }

    // Return SVG::circle with Ellipse base.
    static auto SvgCircle(const Ellipse &ellipse, string label) -> string
    {
        return SVG::circle(CircleShape(ellipse, move(label)));
    }

    // Return SVG::circle with Ellipse base.
    static auto SvgCircle(const Ellipse &ellipse, string label, RGBA fill, RGBA stroke) -> string
    {
        return SVG::circle(SVG::CircleShape(move(label),
                                            SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                            SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                            1.0,    // strokeWidth
                                            255,    // fillOpacity
                                            255,    // strokeOpacity
                                            ellipse.center,
                                            ellipse.horizontalRadius,
                                            ellipse.verticalRadius));
    }

    // Return SVG::circle with Ellipse base.
    static auto SvgCircle(const Ellipse &ellipse, string label, RGBA fill, RGBA stroke,
                          double fillOpacity, double strokeOpacity) -> string
    {
        return SVG::circle(SVG::CircleShape(move(label),
                                            SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                            SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                            1.0,    // strokeWidth
                                            fillOpacity,
                                            strokeOpacity,
                                            ellipse.center,
                                            ellipse.horizontalRadius,
                                            ellipse.verticalRadius));
    }

    // Return SVG::circle with Ellipse base.
    static auto SvgCircle(const Ellipse &ellipse, string label, RGBA fill, RGBA stroke,
                          double fillOpacity, double strokeOpacity,
                          double strokeWidth) -> string
    {
        return SVG::circle(SVG::CircleShape(move(label),
                                            SVG::RGB2HEX(fill.R, fill.G, fill.B),
                                            SVG::RGB2HEX(stroke.R, stroke.G, stroke.B),
                                            strokeWidth,
                                            fillOpacity,
                                            strokeOpacity,
                                            ellipse.center,
                                            ellipse.horizontalRadius,
                                            ellipse.verticalRadius));
    }

    // Returns SVG Elements.
    static auto Join(const vector<Base> &bases, const string &label = "") -> string
    {
        string strShape{};
        for (const auto &item : bases) {
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

    const char SPACE = static_cast<char>(32);
    const string ERROR   = "\tERROR";
    const string WARNING = "\tWARNING";

public:

    Interpreter() = default;
    ~Interpreter() = default;

    auto svg(string line, string &error) -> string
    {
        if (line.empty()) {
            error = "[LINE EMPTY]\n";
            return {};
        }

        string result{};
        string bkp = line;

        // Prepare
        line = Trim(line, SPACE); // Remove spaces from the ends.
        transform(line.begin(), line.end(), line.begin(), ::toupper); // Format.

        // Check command.
        string command{};
        unsigned element = EMPTY;
        for (unsigned i = 1; i < labelType.size(); i++) {
            command = labelType[i] + ":";   // Syntax.
            if (line.starts_with(command)) {
                element = i;
                break;
            }
        }
        if (element < POINTS || element > POLYGON) {
            error = bkp + WARNING + "[Ignore]\n";
            return result;
        }

        // Prepare
        line = line.substr(command.size()); // Remove command word.

        // Check points container: { }
        auto counter = count_if(line.begin(), line.end(),
                                [](char c) {
                                    return c == '{';
                                });
        if (counter > 1) {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }
        counter -= count_if(line.begin(), line.end(),
                            [](char c) {
                                return c == '}';
                            });
        if (counter != 0) {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }

        // Check coordinates container. ( )
        counter = count_if(line.begin(), line.end(),
                           [](char c) {
                               return c == '(';
                           }) -
                  count_if(line.begin(), line.end(),
                           [](char c) {
                               return c == ')';
                           });
        if (counter != 0) {
            error = bkp + ERROR + "[Parentheses]\n";
            return result;
        }

        // Reset
        Points points{};
        double angle{0};
        double width{0};
        double height{0};
        double length{0};
        double horizontalRadius{0};
        double verticalRadius{0};
        double fillOpacity{255};
        double strokeOpacity{255};
        double strokeWidth{1.0};
        unsigned sides{0};
        string label{};
        Color::RGBA fillColor(255, 255, 255, 255);
        Color::RGBA strokeColor(0, 0, 0, 255);

        // Check content between braces.
        // Expected: {(x0,y1)(xN,yN)}
        string content{};
        unsigned first = line.find_first_of('{');
        unsigned second = line.find_first_of('}');
        if (second < first) {   // } {
            error = bkp + ERROR + "[Curly braces]\n";
            return result;
        }
        if (first < second) {   // { ... } or {}
            content = line.substr(first + 1, second - first - 1);
            line = line.substr(0, first) + line.substr(second + 1);
        }
        if (!content.empty()) {
            content = Replace(content, SPACE, "");  // (x0, y0)( x1,y1) to (x0,y0)(x1,y1)
            content = Replace(content, '(', " (");  // (x0,y0)(x1,y1) to (x0,y0) (x1,y1)
            for (auto str : Split(content, SPACE)) {
                str = Trim(str, SPACE);
                if (str.starts_with('(') && str.ends_with(')')) { // (x,y)
                    str = Trim(Trim(str, '('), ')');              //  x,y
                    try {
                        auto values = Split(str, ',');            //  x y
                        if (values.size() == 2) {
                            // Convert to Point.
                            points.emplace_back(Point(stod(values[0]),
                                                      stod(values[1])));
                        }
                    }
                    catch (...) {
                        error = bkp + ERROR + "[Invalid numeric formatting!]\n";
                        return result;
                    }
                }
            }
        }

        if (points.empty()) {
            error = bkp + WARNING + "[Ignore]\n";
            return result;
        }

        // Check other arguments.
        for (const auto &str : Split(line, SPACE)) {
            vector<string> complements {
                "ANGLE", "SIDES", "WIDTH", "HEIGHT", "LENGTH", "RADIUS", "HRADIUS", "VRADIUS",
                "STROKEW"
            };
            for (unsigned i = 0; i < complements.size(); i++) {
                auto arg = complements[i] + "=";
                if (str.starts_with(arg)) {
                    try {
                        auto value = stod(str.substr(arg.size()));
                        switch (i) {
                        case 0:
                            angle = value;
                            break;
                        case 1:
                            sides = static_cast<unsigned>(value);
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
                        // radius = horizontalRadius
                        case 6:
                            horizontalRadius = value;
                            break;
                        case 7:
                            verticalRadius = value;
                            break;
                        case 8:
                            strokeWidth = value;
                            break;
                        default:
                            break;
                        }
                    }
                    catch (...) {
                        // pass
                    }
                }
            }
            string arg = "LABEL=";
            if (str.starts_with(arg)) {
                try {
                    label = str.substr(arg.size());
                }
                catch (...) {
                    // pass
                }
            }
            arg = "FILL=";
            if (str.starts_with("FILL=")) {
                try {
                    auto numbers = Split(str.substr(arg.size()), ',');
                    fillColor = Color::RGBA(stoi(numbers[0]),
                                            stoi(numbers[1]),
                                            stoi(numbers[2]),
                                            255);
                    fillOpacity = (numbers.size() == 4 ? stoi(numbers[3]) : 255);
                }
                catch (...) {
                    // pass
                }
            }
            arg = "STROKE=";
            if (str.starts_with(arg)) {
                try {
                    auto numbers = Split(str.substr(arg.size()), ',');
                    strokeColor = Color::RGBA(stoi(numbers[0]),
                                              stoi(numbers[1]),
                                              stoi(numbers[2]),
                                              255);
                    strokeOpacity = (numbers.size() == 4 ? stoi(numbers[3]) : 255);
                }
                catch (...) {
                    // pass
                }
            }
        }

        // Converter
        label = label.empty() ? labelType[element] : label;
        switch (element) {
        case POINTS:
            if (points.size() == 1) {
                // Isolated points in SVG are not recommended.
                error = bkp + WARNING + "[Ignore - A minimum of two points is expected!]\n";
                return {};
            }
            if (points.size() > 1) {
                result = Sketch::SvgPolygon(IrregularPolygon(points),
                                            label, fillColor, strokeColor);
            }
            break;
        case LINE:
            if (points.size() == 1 && angle > 0 && length > 0) {
                result = Sketch::SvgPolygon(Line(points.front(), angle, length),
                                            label, fillColor, strokeColor);
            }
            else if (points.size() == 2) {
                result = Sketch::SvgPolygon(IrregularPolygon(points),
                                            label, fillColor, strokeColor);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case TRIANGLE:
            if (points.size() == 2) {
                result = Sketch::SvgPolygon(Triangle(points[0], points[1], height),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else if (points.size() == 3) {
                result = Sketch::SvgPolygon(IrregularPolygon(points),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case RECTANGLE:
            if (points.size() == 1 && width > 0 && height > 0) {
                result = Sketch::SvgPolygon(Rectangle(points.front(), width, height),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else if (points.size() == 4) {
                result = Sketch::SvgPolygon(IrregularPolygon(points),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case CIRCLE:
            if (points.size() == 1 && horizontalRadius > 0) {
                result = Sketch::SvgCircle(Circle(points.front(), horizontalRadius),
                                           label, fillColor, strokeColor,
                                           fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case ELLIPSE:
            if (points.size() == 1 && horizontalRadius > 0 && verticalRadius > 0) {
                result = Sketch::SvgCircle(Ellipse(points.front(),
                                                   horizontalRadius,
                                                   verticalRadius),
                                           label, fillColor, strokeColor,
                                           fillOpacity, strokeOpacity, strokeWidth);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        case POLYGON:
            if (points.size() == 1 && horizontalRadius > 0 && angle > 0 && sides > 2) {
                result = Sketch::SvgPolygon(RegularPolygon(points.front(),
                                                           horizontalRadius, angle, sides),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity, strokeWidth);
            }
            else if (points.size() > 1) {
                result = Sketch::SvgPolygon(IrregularPolygon(points),
                                            label, fillColor, strokeColor,
                                            fillOpacity, strokeOpacity);
            }
            else {
                error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
                return {};
            }
            break;
        default:
            error = bkp + WARNING + "[Incomplete or wrong syntax!]\n";
            result = {};
            break;
        }

        return result;
    }

    auto load(const string &path, string &errors) -> string
    {
        auto text = Load(path, ".txt");
        if (text.empty()) {
            errors = path + ERROR + "[FILE EMPTY]\n";
            return {};
        }

        string result{};
        errors.clear();
        auto lines = Split(text, '\n');
        for (unsigned i = 0; i < lines.size(); i++) {
            auto line = lines[i];
            if (line.starts_with('#')) {
                errors += to_string(i + 1) + ": [COMMENT LINE]\n";
                continue;
            }
            string error{};
            auto res = svg(line, error);
            if (!res.empty()) {
                result += res;
            }
            else {
                errors += to_string(i + 1) + ": " + error;
            }
        }

        return result;
    }

};

// Generic

auto Radians(double angle) -> double
{
    return angle * PI / 180.0;
}

auto Angle(double radians) -> double
{
    return radians * 180.0 / PI;
}

// Returns the angle of the line (x0,y0)(x1,y1).
auto Angle(double x0, double y0, double x1, double y1) -> double
{
    double result = Angle(atan((y1 - y0) / (x1 - x0)));

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
auto Angle(Point origin, Point first, Point second, bool signal) -> double
{
    auto angle1 = origin.angle(first);
    auto angle2 = origin.angle(second);

    if (signal) {
        return angle1 - angle2;
    }

    return max(angle1, angle2) - min(angle1, angle2);
}

// Return: value + radius * cos(angle).
auto Cos(double value, double radius, double angle) -> double
{
    return value + radius * cos(Radians(angle));
}

// Return: value + radius * sin(angle).
auto Sin(double value, double radius, double angle) -> double
{
    return value + radius * sin(Radians(angle));
}

// Returns the distance between two points.
auto Distance(double x0, double y0, double x1, double y1) -> double
{
    return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}

// Rounds value to N digits after decimal point.
// Number of Decimal Places < 0, returns the same value.
auto Round(double value, int decimalPlaces) -> double
{
    if (decimalPlaces < 0) {
        return value;
    }

    if (decimalPlaces == 0) {
        return static_cast<int>(value);
    }

    const int limit = 10;
    decimalPlaces = decimalPlaces > limit ? limit : decimalPlaces;

    const int base = 10;
    return round(value * pow(base, decimalPlaces)) / pow(base, decimalPlaces);
}

// Round values.
// Number of Decimal Places < 0, returns the same value.
auto Round(Numbers values, int decimalPlaces) -> Numbers
{
    for (double &value : values) {
        values = Round(values, decimalPlaces);
    }

    return values;
}

// Round points.
// Number of Decimal Places < 0, returns the same value.
auto Round(Points points, int decimalPlaces) -> Points
{
    for (auto &point : points) {
        point = point.round(decimalPlaces);
    }

    return points;
}

// Triangle area.
auto TriangleArea(double x0, double y0, double x1, double y1, double x2, double y2) -> double
{
    // Heron's formula
    auto a = Distance(x0, y0, x1, y1);
    auto b = Distance(x1, y1, x2, y2);
    auto c = Distance(x2, y2, x0, y0);
    auto s = (a + b + c) / 2.0;

    return sqrt(s * (s - a) * (s - b) * (s - c));
}

// Triangle area using points.
auto TriangleArea(Point point1, Point point2, Point point3) -> double
{
    return TriangleArea(point1.X.value, point1.Y.value,
                        point2.X.value, point2.Y.value,
                        point3.X.value, point3.Y.value);
}

// Triangle height.
auto TriangleHeight(double x0, double y0, double x1, double y1, double x2, double y2) -> double
{
    auto area = TriangleArea(x0, y0, x1, y1, x2, y2);
    auto height0 = 2 * area / Distance(x0, y0, x1, y1);
    auto height1 = 2 * area / Distance(x1, y1, x2, y2);
    auto height2 = 2 * area / Distance(x2, y2, x0, y0);

    return max(max(height0, height1), height2);
}

// Triangle height using points.
auto TriangleHeight(Point point1, Point point2, Point point3) -> double
{
    return TriangleHeight(point1.X.value, point1.Y.value,
                          point2.X.value, point2.Y.value,
                          point3.X.value, point3.Y.value);
}

// Calculates the point of intersection between two lines.
// Returns false if the lines are parallel or coincident.
// Line 1 (x0, y0) - (x1, y1),
// Line 2 (x2, y2) - (x3, y4).
auto LineIntersect(double x0, double y0, double x1, double y1,
                   double x2, double y2, double x3, double y3,
                   Point &point) -> bool
{
    double d = (y3 - y2) * (x1 - x0) - (x3 - x2) * (y1 - y0);
    if (d == 0) {   // Two lines are parallel or coincident ...
        point.X.value = MAXDOUBLE;
        point.Y.value = MAXDOUBLE;
        return false;
    }

    double t = ((x3 - x2) * (y0 - y2) - (y3 - y2) * (x0 - x2)) / d;
    double u = ((x1 - x0) * (y0 - y2) - (y1 - y0) * (x0 - x2)) / d;

    if (t >= 0.0 && t <= 1.0 && u >= 0 && u <= 1.0) {
        point.X.value = x0 + t * (x1 - x0);
        point.Y.value = y0 + t * (y1 - y0);
        return true;
    }

    // Lines do not intersect.
    return false;
}

// Calculates the point of intersection between two lines.
// Returns false if the lines are parallel or coincident.
auto LineIntersect(Line line1, const Line &line2, Point &point) -> bool
{
    return line1.intersection(line2, point);
}

// Perpendicular line passing through the point.
auto Perpendicular(Line line, Point point) -> Line
{
    return line.perpendicular(point);
}

// Compare groups.
template<typename T>
auto Equal(vector<T> group1, vector<T> group2, bool compareOrder) -> bool
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

    for (auto value1 : group1) {
        bool differentFromEveryone = true;
        for (auto value2 : group2) {
            if (value1 == value2) {
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
auto Sum(Points group, double value) -> Points
{
    for (auto &item : group) {
        item += value;
    }

    return group;
}

// Returns Points updated with the sum.
auto Sum(Points group, Point point) -> Points
{
    for (auto &item : group) {
        item += point;
    }

    return group;
}

// Sum all the distances, point by point.
auto SumDistances(Points points) -> double
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
auto Total(const Points &points) -> Point
{
    Point sum;
    for (auto point : points) {
        sum += point;
    }

    return sum;
}

// Average : Point (Total X axis / Points, Total Y axis / Points).
auto  Average(const Points &points, Point &point) -> bool
{
    if (points.empty()) {
        point = Origin;
        return false;
    }

    point = Total(points) * (1.0 / static_cast<double>(points.size())) ;

    return true;
}

// Sort numbers.
auto Sort(Numbers numbers, bool ascendingOrder) -> Numbers
{
    if (ascendingOrder) {
        sort(numbers.begin(), numbers.end(), less<>());
    }
    else {
        sort(numbers.begin(), numbers.end(), greater<>());
    }

    return numbers;
}

// Sort points by X or Y axis.
auto Sort(Points points, bool X_axis) -> Points
{
    if (points.empty()) {
        return {};
    }

    if (points.size() == 1) {
        return points;
    }

    map<double, Numbers > mapPoint;

    for (auto point : points) {
        auto key = X_axis ? point.X.value : point.Y.value;
        auto value = X_axis ? point.Y.value : point.X.value;
        if (mapPoint.find(key) == mapPoint.end()) {
            mapPoint.insert({key, {value}});
        }
        else {
            mapPoint[key].push_back(value);
        }
    }

    Points result;
    for (const auto &item : mapPoint) {
        if (X_axis) {
            for (auto value : item.second) { // Y
                result.emplace_back(Point(item.first, value));
            }
        }
        else {
            auto values = Sort(item.second);
            for (auto value : values) {     // X
                result.emplace_back(Point(value, item.first));
            }
        }
    }

    return result;
}

// Sort the Points clockwise.
auto Organize(Points points) -> Points
{
    if (points.size() < 2) {
        return points;
    }

    // Map : Angle x Point.
    map<double, Points > mapPoint;

    for (auto value : points) {
        auto key = Point(0, 0).angle(value);
        if (mapPoint.find(key) == mapPoint.end()) {
            mapPoint.insert(make_pair(key, Points{value}));
        }
        else {
            mapPoint[key].push_back(value);
        }
    }

    Points result;
    for (const auto &item : mapPoint) {
        for (auto vPoints : item.second) {
            result.push_back(vPoints);
        }
    }

    return result;
}

// Join strings.
auto Join(Strings vStr, char delimiter) -> string
{
    string result{};
    for (unsigned i = 0; i < vStr.size(); i++) {
        result += vStr[i] + (i < vStr.size() - 1 ? string{delimiter} : "");
    }

    return result;
}

// Trim string : Remove characters to the left.
auto LTrim(string str, char trimmer) -> string
{
    int left = 0;
    auto right = str.size() - 1;
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
auto RTrim(string str, char trimmer) -> string
{
    int left = 0;
    auto right = str.size() - 1;
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
auto Trim(string str, char trimmer) -> string
{
    return RTrim(LTrim(move(str), trimmer), trimmer);
}

// Trim strings.
auto Trim(Strings vStr, char trimmer) -> Strings
{
    for (auto &item : vStr) {
        item = Trim(item, trimmer);
    }

    return vStr;
}

// Replace all occurrences.
auto Replace(const string &str, char character, const string &replace) -> string
{
    string result{};
    for (char characterTemp : str) {
        result += (characterTemp == character ? replace : string{characterTemp});
    }

    return result;
}

// Replace all occurrences.
auto Replace(const string &str, char character, char replace) -> string
{
    return Replace(str, character, string{replace});
}

// Split string.
auto Split(const string &str, char delimiter) -> Strings
{
    Strings result;
    string strTemp{};
    for (char character : str) {
        if (character == delimiter) {
            result.push_back(strTemp);
            strTemp = "";
        }
        else {
            strTemp += character;
        }
    }
    if (!strTemp.empty()) {
        result.push_back(strTemp);
    }

    return result;
}

// Input/Output

// Std::cout : double.
void View(double value)
{
    cout << to_string(value) << '\n';
}

// Std::cout : Point(x,y).
void View(Point point)
{
    cout << "(" << point.toStr() << ")" << '\n';
}

// Std::cout : Array of double.
template<size_t SIZE>
void View(array<double, SIZE> arr)
{
    unsigned counter = 0;
    for (const auto &value : arr) {
        cout << "[" << counter << "]: " << Round(value) << '\n';
        counter++;
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
        cout << "Empty\n";
    }
    else {
        cout << str << '\n';
    }
}

// Std::cout : Vector of numbers.
template<typename T>
void View(vector<T> values)
{
    string str{};
    for (unsigned i = 0; i < values.size(); i++) {
        str += to_string(values[i]) + (i < values.size() - 1 ? "," : "");
    }

    if (str.empty()) {
        cout << "Empty\n";
    }
    else {
        cout << str << '\n';
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
        cout << "Empty\n";
    }
    else {
        cout << str << '\n';
    }
}

// Std::cout : string.
void View(const string &str)
{
    cout << str << '\n';
}

// Load text file.
auto Load(const string &filePath, string filenameExtension) -> string
{
    if (filePath.empty()) {
        return {};
    }

    // Check extension.
    if (!filenameExtension.empty()) {
        const auto fpath = std::filesystem::path(filePath);
        if (!exists(fpath)) {
            cerr << "File not found!\n";
            return {};
        }

        string extension{fpath.extension()};
        transform(extension.begin(), extension.end(),
                  extension.begin(), ::toupper);
        transform(filenameExtension.begin(), filenameExtension.end(),
                  filenameExtension.begin(), ::toupper);

        if (!extension.ends_with(filenameExtension)) {
            cerr << "Invalid file!\n";
            return {};
        }
    }

    string str{};
    try {
        ifstream fileIn(filePath, ios::in);
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
auto Save(const string &text, string filePath) -> bool
{
    if (text.empty()) {
        cerr << "Empty text! Export failed!\n";
        return false;
    }

    if (filePath.empty()) {
        filePath = "output.txt";
    }

    try {
        ofstream file(filePath, ios::out);
        file << text;
        file.close();
    }
    catch (const exception &e) {
        cout << "Error handling file writing.\n";
        cerr << e.what() << "\n";
        return false;
    }

    return true;
}

} // namespace smalltoolbox

#endif // SMALLTOOLBOX_H_
