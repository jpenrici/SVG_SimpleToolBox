#include "svgToolBox.hpp"

#include <assert.h>
#include <iostream>
#include <vector>

using namespace smalltoolbox;

// Tests
void basic();
void point();
void line();
void triangle();
void rectangle();
void polygons();
void svg();

int main()
{
    basic();
    point();
    line();
    triangle();
    //    rectangle();
    //    polygons();
    //    svg();

    return 0;
}

void basic()
{
    // Math
    for (auto &a : {0, 15, 30, 45, 60, 90, 120, 135, 150, 180, 225, 270, 315, 360}) {
        assert(std::sin(a * std::numbers::pi / 180.0) == smalltoolbox::sin(0, 1, a));
        assert(std::cos(a * std::numbers::pi / 180.0) == smalltoolbox::cos(0, 1, a));
    }

    assert(smalltoolbox::radians(360) == 2 * std::numbers::pi);
    assert(smalltoolbox::angle(2 * std::numbers::pi) == 360);

    assert(smalltoolbox::angle(0.0, 0.0,  0.0,  0.0) == 0);
    assert(smalltoolbox::angle(0.0, 0.0,  1.0,  0.0) == 0);
    assert(smalltoolbox::angle(0.0, 0.0,  0.0,  1.0) == 90);
    assert(smalltoolbox::angle(0.0, 0.0, -1.0,  0.0) == 180);
    assert(smalltoolbox::angle(0.0, 0.0,  0.0, -1.0) == 270);
    assert(smalltoolbox::angle(0.0, 0.0,  1.0,  1.0) == 45);
    assert(smalltoolbox::angle(0.0, 0.0, -1.0,  1.0) == 90 + 45);
    assert(smalltoolbox::angle(0.0, 0.0, -1.0, -1.0) == 180 + 45);
    assert(smalltoolbox::angle(0.0, 0.0,  1.0, -1.0) == 270 + 45);
    assert(smalltoolbox::angle(1.0, 1.0,  0.0,  0.0) == 180 + 45);

    assert(smalltoolbox::cos(5, 10,  0) == (5 + 10 * 1));
    assert(smalltoolbox::sin(5, 10,  0) == (5 + 10 * 0));
    assert(smalltoolbox::sin(5, 10, 90) == (5 + 10 * 1));

    assert(smalltoolbox::distance(0.0, 0.0, 5.0, 0.0) == 5);
    assert(smalltoolbox::distance(0.0, 0.0, 0.0, 5.0) == 5);
    assert(smalltoolbox::distance(0.0, 0.0, 5.0, 5.0) == std::sqrt(50));

    assert(smalltoolbox::round(0.011) == 0.01);
    assert(smalltoolbox::round(0.019) == 0.02);
    assert(smalltoolbox::round(0.0101, 3) == 0.010);
    assert(smalltoolbox::round(0.0109, 3) == 0.011);

    std::cout << "Basic test finished!\n";
}

void point()
{
    Point p1, p2; // (0, 0)

    assert(p1.equal(p2));
    assert(p1 == Point(0, 0));
    assert(p1 + Point(1, 1) == Point(1,  1));
    assert(p1 - Point(1, 1) == Point(-1, -1));
    assert(p1 * Point(2, 2) == Point(0,  0));
    assert(Point(5, 5) * Point(0.5, 1 / 2)   == Point(2.5, 0));
    assert(Point(5, 5) * Point(0.5, 1 / 2.0) == Point(2.5, 2.5));

    p2 = Point(1, 1);
    p2 *= 3;
    p2 += 2;
    p2 -= 4;
    assert(p2 == Point(1, 1));

    p1 += Point(1, 1);
    assert(p1 == p2);

    p1 = Origin;
    assert(p1 == Point(0, 0));
    assert(p1.angle(Point(0, 1)) == 90);
    assert(p1.distance(Point(1, 0)) == 1.0);

    p1.sum(1, 1);
    assert(p1 == Point(1, 1));

    p1.multiply(0.5, 0.5);
    assert(p1 == Point(0.5, 0.5));

    p1.reset();
    assert(p1 == Origin);

    // Output
    view(1);
    view(0.5);
    view(1.5f);

    view(p1);
    view(p2);
    view(p1.XY());

    std::cout << "Point test finished!\n";
}

void line()
{
    Line line;

    assert(line.first == Origin);
    assert(line.second.equal(Origin));

    view(line.setup(Point(1, 1), Point(2, 2)));
    assert(line.first == Point(1, 1));
    assert(line.second.equal(Point(2, 2)));

    line.setup(Origin, Point(1, 0));
    assert(line.angle() == 0.0);
    assert(line.length() == 1.0);
    assert(line.middle() == Point(0.5, 0));

    line.setup(Origin, Point(1, 1));
    assert(line.angle() == 45);
    assert(line.length() == std::sqrt(2));
    assert(line.middle().round().equal(Point(0.5, 0.5)));

    std::cout << "Line test finished!\n";
}

void triangle()
{
    Triangle triangle;

    assert(triangle.first == Origin);
    assert(triangle.second == Origin);
    assert(triangle.third == Origin);

    std::cout << "Triangle test finished!\n";
}

void rectangle()
{
    Rectangle rectangle;

    assert(rectangle.first == Origin);
    assert(rectangle.second == Origin);
    assert(rectangle.third == Origin);
    assert(rectangle.fourth == Origin);

    std::cout << "Rectangle test finished!\n";
}

void polygons()
{
    // Polygon
    Polygon polygon;
    view(polygon.setup(Point(0, 0), -1,  0,  0)); // Return empty
    view(polygon.setup(Point(0, 0),  0,  0, -1)); // Return empty
    view(polygon.setup(Point(1, 1),  0,  0,  0)); // Return empty
    view(polygon.setup(Point(0, 0),  1,  0,  1)); // Return empty
    view(polygon.setup(Point(0, 0),  1,  0,  3)); // Triangle
    view(polygon.setup(Point(0, 0),  1,  0,  4)); // Rectangle
    view(polygon.setup(Point(0, 0),  1, 45,  4)); // Rectangle
    view(polygon.setup(Point(0, 0),  2, 90,  4)); // Rectangle
    view(polygon.setup(Point(0, 0),  1,  0,  5)); // Pentagon
    view(polygon.points()); // returns the last configuration points

    // Rectangle (1,0)(0,1)(-1,0)(0,-1)
    polygon.setup(Point(0, 0),  1,  0,  4);
    auto a = polygon.sideLength();
    auto b = Point(1, 0).distance(Point(0, 1));
    view(std::to_string(a) == std::to_string(b) ? "true" : "false"); // true
    view(a == b ? "true" : "false");                                 // false
    view(polygon.diagonalLength() == 2.0 ? "true" : "false");        // true
    view(polygon.points());

    std::cout << "Polygon test finished!\n";
}

void svg()
{
    // SVG
    view("SVG");
    auto shape = SVG::Shape("Rectangle", RED, GREEN, 1.0,
                            Polygon().setup(Point(50, 50), 50, 45, 4));
    auto svg = SVG::svg(100, 100, SVG::polygon(shape), SVG::Metadata());
    save(svg, "svgOutput.svg");
    view(svg);

    std::cout << "SVG test finished!\n";
}
