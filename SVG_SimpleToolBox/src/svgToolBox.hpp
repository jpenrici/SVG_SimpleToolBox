#ifndef _SMALLTOOLBOX_H_
#define _SMALLTOOLBOX_H_

#include <array>
#include <ctime>
#include <fstream>
#include <iomanip>
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
                   std::array<double, 2> &xy)
{
    double d = (y3 - y2) * (x1 - x0) - (x3 - x2) * (y1 - y0);
    if (d == 0) {   // Two lines are parallel or coincident ...
        xy = {std::numeric_limits<double>::max(),
              std::numeric_limits<double>::max()
        };
        return false;
    }

    double t = ((x3 - x2) * (y0 - y2) - (y3 - y2) * (x0 - x2)) / d;
    double u = ((x1 - x0) * (y0 - y2) - (y1 - y0) * (x0 - x2)) / d;

    if (t >= 0.0 && t <= 1.0 && u >= 0 && u <= 1.0) {
        xy = {(x0 + t * (x1 - x0)), (y0 + t * (y1 - y0))};
        return true;
    }

    // Lines do not intersect
    return false;
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
    Point round()
    {
        return Point(smalltoolbox::round(X.value),
                     smalltoolbox::round(Y.value));
    }

    // Returns Array[2] with X and Y values.
    std::array<double, 2> XY()
    {
        return {{X.value, Y.value}};
    }

    std::string toStr()
    {
        return X.toStr() + "," + Y.toStr();
    }

};

// Special point.
const Point Origin = Point(0, 0);

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

    Line(Point first, Point second)
        : first(first), second(second) {};

    Line(Point origin, double angle, double length)
    {
        setup(origin, angle, length);
    }

    ~Line() {};

    bool operator==(Line line)
    {
        return equal(line);
    }

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

    // Distance between the two points.
    double length()
    {
        return first.distance(second);
    }

    // Line angle.
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
                      xy);
        point.X.value = xy[0];
        point.Y.value = xy[1];

        return result;
    }

    // Perpendicular line passing through the point.
    Line perpendicular(Point point)
    {
        // TO DO
        return {};
    }

    // Line 1 == Line 2
    bool equal(Line line)
    {
        if (!first.equal(line.first) && !first.equal(line.second)) {
            return false;
        }
        if (!second.equal(line.first) && !second.equal(line.second)) {
            return false;
        }
        return true;
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        update();
        return vertices;
    }
};

// (x1,y1)(x2,y2)(x3,y3)
class Triangle {

    // Store the last configuration.
    std::vector<Point> vertices;

    void update()
    {
        vertices = {first, second, third};
    }

public:
    Point first, second, third;

    Triangle() {};
    ~Triangle() {};

    // Triangle: Points (x1,y1),(x2,y2),(x3,y3)
    std::vector<Point> setup(Point first, Point second, Point third)
    {
        this->first = first;
        this->second = second;
        this->third = third;

        return points();
    }

    // Triangle: Line and height
    std::vector<Point> setup(Line side, double height)
    {
        return setup(side.first,
                     side.second,
                     Point(side.middle()).position(90 + side.angle(), height));
    }

    // Triangle: Points (x1,y1),(x2,y2) and height
    std::vector<Point> setup(Point first, Point second, double height)
    {
        return setup(Line(first, second), height);
    }

    double height()
    {
        return smalltoolbox::triangleHeight(first.X.value, first.Y.value,
                                            second.X.value, second.Y.value,
                                            third.X.value, third.Y.value);
    }

    double area()
    {
        return smalltoolbox::triangleArea(first.X.value, first.Y.value,
                                          second.X.value, second.Y.value,
                                          third.X.value, third.Y.value);
    }

    double perimeter()
    {
        return first.distance(second) + second.distance(third) + third.distance(first);
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        update();
        return vertices;
    }
};

// (x1,y1)(x2,y2)(x3,y3)(x4,y4)
class Rectangle {

    // Store the last configuration.
    std::vector<Point> vertices;

public:

    Point first, second, third, fourth;

    Rectangle() {};
    ~Rectangle() {};

    std::vector<Point> setup(Point first, Point second, Point third, Point fourth)
    {
        this->first = first;
        this->second = second;
        this->third = third;
        this->fourth = fourth;

        return points();
    }

    std::vector<Point> setup(Point first, double width, double heigth)
    {
        return setup(first,
                     first + Point(width, 0),
                     first + Point(width, heigth),
                     first + Point(0, heigth));
    }

    double area()
    {
        // TO DO
        return -1;
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        return {first, second, third, fourth};
    }
};

class Polygon {

    // Store the last configuration.
    Point center;
    double angle;
    double horizontalRadius;
    double verticalRadius;
    unsigned sides;

    std::vector<Point> vertices;

public:

    Polygon() {};
    ~Polygon() {};

    // Center : polygon center point (x,y),
    // horizontalRadius : distance from the center on X axis (>= 1),
    // verticalRadius   : distance from the center on Y axis (>= 1),
    // angle  : starting point angle,
    // sides  : divisions of a circle (>= 3).
    std::vector<Point> setup(Point center, double radius, double angle, unsigned sides)
    {
        // Check
        sides = sides > 360 ? 360 : sides;
        if (radius < 1 || sides < 3) {
            return {};
        }

        // Update
        this->center = center;
        this->angle = angle;
        this->horizontalRadius = radius;
        this->verticalRadius = radius;
        this->sides = sides;

        vertices.clear();
        for (unsigned a = angle; a < 360 + angle; a += (360 / sides)) {
            vertices.push_back(Point(smalltoolbox::cos(center.X.value, radius, a),
                                     smalltoolbox::sin(center.Y.value, radius, a)));
        }

        return vertices;
    }

    // Returns the current vertices.
    std::vector<Point> points()
    {
        return vertices;
    }

    double sideLength()
    {
        return vertices.size() < 2 ? 0 : vertices[0].distance(vertices[1]);
    }

    double diagonalLength()
    {
        // TO DO
        return -1;
    }

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

void view(std::array<double, 2> arr)
{
    std::cout << "x: " << std::to_string(arr[0]) << '\n'
              << "y: " << std::to_string(arr[1]) << '\n';
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
