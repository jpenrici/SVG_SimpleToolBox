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
    rectangle();
    polygons();
    //    svg();

    return 0;
}

void basic()
{
    // Math
    std::vector<unsigned> angles {
        0, 15, 30, 45, 60, 90, 120, 135, 150, 180, 225, 270, 315, 360
    };
    for (auto &a : angles) {
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

    assert(smalltoolbox::triangleArea(0, 0, 5, 0, 0, 10) == 25.0);    // Triangle
    assert(smalltoolbox::triangleHeight(0, 0, 5, 0, 0, 10) == 10.0);  // Triangle

    std::array<double, 2> xy;
    assert(!lineIntersect(1, 2, 5, 5, 2, 1, 6, 4, xy));     // Parallel
    assert(!lineIntersect(0, 0, 5, 5, 1, 1, 4, 4, xy));     // Coincident
    assert(lineIntersect(1, 1, -1, -1, 2, -2, -2, 2, xy));  // Intersection at Origin
    assert(lineIntersect(2, 2, 2, 10, 0, 4, 10, 4, xy));    // Intersection at (2,4)

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

    p1 = Origin;
    p1 = p1.position(90, 10);
    assert(p1.round() == Point(0, 10));

    // Output
    // view(1);
    // view(0.5);
    // view(1.5f);

    // view(p1);
    // view(p2);
    // view(p1.XY());

    std::cout << "Point test finished!\n";
}

void line()
{
    Line line;

    assert(line.first == Origin);
    assert(line.second.equal(Origin));

    line.setup(Point(1, 1), Point(2, 2));
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

    line.setup(Origin, 90, 10);
    assert(line.second.round() == Point(0, 10));

    Point p;
    Line line1;
    line.setup(Point(2, 2), Point(2, 10));  // Line (2,2)(2,10)
    line1.setup(0, 4, 10, 4);               // Line (0,4)(10,4)
    // view(line.points());
    // view(line1.points());
    assert(line.intersection(line1, p));    // Intersect
    assert(p == Point(2, 4));               // at (2,4)

    Point a(1, 1), b(2, 2), c(3, 3);
    assert(Line(a, b) == Line(a, b));           // Equal
    assert(Line(b, a).equal(Line(a, b)));       // Equal
    assert(Line(a, b).equal(Line(b, a)));       // Equal
    assert(Line(a, a).equal(Line(a, a)));       // Equal
    assert(Line(a + b, a).equal(Line(c, a)));   // Equal
    assert(!Line(a, b).equal(Line(c, b)));      // Not equal

    assert(Line(0, 0, 0, 10).length() == 10.0);

    assert(Line(Point(-1, 0), Point(10, 0)).perpendicular(Point(0, 10)).round() ==
           Line(Point(0, 0), Point(0, 10)).round());
    assert(Line(Point(0, 1), Point(0, 10)).perpendicular(Point(5, 5)).round() ==
           Line(Point(0, 5), Point(5, 5)).round());

    std::cout << "Line test finished!\n";
}

void triangle()
{
    Triangle triangle;

    assert(triangle.first == Origin);
    assert(triangle.second == Origin);
    assert(triangle.third == Origin);

    triangle.setup(Point(-1, 0), Point(1, 0), 10);
    assert(triangle.third.round() == Point(0, 10));

    Line side;
    side.setup(Point(-1, 0), 0, 2);
    triangle.setup(side, 10);
    assert(triangle.third.round() == Point(0, 10));

    // Base 2, height 10 : Area (2 * 10 / 2)
    triangle.setup(Point(-2, 0), Point(0, 0), Point(0, 10));
    assert(triangle.area() == (2 * 10 / 2.0));
    assert(triangle.height() == 10.0);

    triangle.setup(Origin, Point(10, 0), Point(0, 10));
    assert(triangle.perimeter() == 10 + 10 + std::sqrt(200));

    Point a(1, 1), b(2, 2), c(3, 3), d(4, 4);
    assert(Triangle(a, b, c) == Triangle(b, a, c));         // Equal
    assert(Triangle(a, b, a + b) == Triangle(c, b, a));     // Equal
    assert(!Triangle(a, b, c).equal(Triangle(a, b, d)));    // Not Equal
    // view(Triangle(a, b, c).points());
    // view(Triangle(a, b, c).lengthOfSides());

    std::cout << "Triangle test finished!\n";
}

void rectangle()
{
    Rectangle rectangle;

    assert(rectangle.first == Origin);
    assert(rectangle.second == Origin);
    assert(rectangle.third == Origin);
    assert(rectangle.fourth == Origin);

    rectangle.setup(Point(0, 0), 10, 10);
    assert(rectangle.second.round() == Point(10, 0));
    assert(rectangle.third.round() == Point(10, 10));
    assert(rectangle.fourth.round() == Point(0, 10));
    assert(rectangle.area() == 100);
    assert(rectangle.perimeter() == 40);

    rectangle.setup(Point(0, 0), 20, 10);
    // view(rectangle.points());
    // view(rectangle.lengthOfSides());

    Point a(1, 1), b(5, 1), c(5, 5), d(1, 5), e(5, 5);
    assert(Rectangle(a, b, c, d) == Rectangle(d, a, b, c));             // Equal
    assert(Rectangle(a, b, c, e) == Rectangle(a, b, c, a + a + c));     // Equal
    assert(Rectangle(a, b, c, e) == Rectangle(a, b, c, a * 4.0 + 1.0)); // Equal
    assert(Rectangle(a, b, c, d).round().area() ==
           Rectangle(d, a, b, c).round().area());
    assert(Rectangle(a, b, c, d).round().perimeter() ==
           Rectangle(d, a, b, c).round().perimeter());
    // view(Rectangle(a, b, c, d).points());
    // view(Rectangle(d, a, b, c).points());

    std::cout << "Rectangle test finished!\n";
}

void polygons()
{
    // Polygon
    Polygon polygon;
    polygon.setup(Point(0, 0), -1,  0,  0); // Return empty
    assert(polygon.points().empty());
    polygon.setup(Point(0, 0),  0,  0, -1); // Return empty
    assert(polygon.points().empty());
    polygon.setup(Point(1, 1),  0,  0,  0); // Return empty
    assert(polygon.points().empty());
    polygon.setup(Point(0, 0),  1,  0,  1); // Return empty
    assert(polygon.points().empty());

    view(polygon.setup(Point(0, 0),  1,  0,  3)); // Triangle
    view(polygon.setup(Point(0, 0),  1,  0,  4)); // Rectangle
    view(polygon.setup(Point(0, 0),  1, 45,  4)); // Rectangle
    view(polygon.setup(Point(0, 0),  2, 90,  4)); // Rectangle
    view(polygon.setup(Point(0, 0),  1,  0,  5)); // Pentagon
    view(polygon.points()); // returns the last configuration points

    // Rectangle (1,0)(0,1)(-1,0)(0,-1)
    polygon.setup(Point(0, 0),  1,  0,  4);
    assert(smalltoolbox::round(polygon.sideLength()) ==
           smalltoolbox::round(Point(1, 0).distance(Point(0, 1))));
    //assert(smalltoolbox::round(polygon.diagonalLength()) == 2.0); // TO DO

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
