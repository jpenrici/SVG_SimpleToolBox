#include "svgToolBox.hpp"

#include <assert.h>
#include <iostream>
#include <vector>

using smalltoolbox::Angle;
using smalltoolbox::Cos;
using smalltoolbox::Distance;
using smalltoolbox::Equal;
using smalltoolbox::Join;
using smalltoolbox::LineIntersect;
using smalltoolbox::Load;
using smalltoolbox::Organize;
using smalltoolbox::Radians;
using smalltoolbox::Round;
using smalltoolbox::Save;
using smalltoolbox::Sin;
using smalltoolbox::Sort;
using smalltoolbox::Split;
using smalltoolbox::Sum;
using smalltoolbox::SumDistances;
using smalltoolbox::Total;
using smalltoolbox::TriangleArea;
using smalltoolbox::TriangleHeight;
using smalltoolbox::Trim;
using smalltoolbox::View;

using smalltoolbox::Origin;
using smalltoolbox::Zero;
using smalltoolbox::Point;
using smalltoolbox::Line;
using smalltoolbox::Triangle;
using smalltoolbox::Rectangle;
using smalltoolbox::Circle;
using smalltoolbox::Ellipse;
using smalltoolbox::Base;
using smalltoolbox::IrregularPolygon;
using smalltoolbox::RegularPolygon;

using smalltoolbox::SVG;

// Tests
void basic();
void point();
void line();
void triangle();
void rectangle();
void circle();
void regularPolygons();
void irregularPolygon();
void svg();

int main()
{
    basic();
    point();
    line();
    triangle();
    rectangle();
    circle();
    regularPolygons();
    irregularPolygon();
    svg();

    return 0;
}

void basic()
{
    // Math
    std::vector<unsigned> angles {
        0, 15, 30, 45, 60, 90, 120, 135, 150, 180, 225, 270, 315, 360
    };
    for (auto &a : angles) {
        assert(std::sin(a * std::numbers::pi / 180.0) == Sin(0, 1, a));
        assert(std::cos(a * std::numbers::pi / 180.0) == Cos(0, 1, a));
    }

    assert(Radians(360) == 2 * std::numbers::pi);
    assert(Angle(2 * std::numbers::pi) == 360);

    assert(Angle(0.0, 0.0,  0.0,  0.0) == 0);
    assert(Angle(0.0, 0.0,  1.0,  0.0) == 0);
    assert(Angle(0.0, 0.0,  0.0,  1.0) == 90);
    assert(Angle(0.0, 0.0, -1.0,  0.0) == 180);
    assert(Angle(0.0, 0.0,  0.0, -1.0) == 270);
    assert(Angle(0.0, 0.0,  1.0,  1.0) == 45);
    assert(Angle(0.0, 0.0, -1.0,  1.0) == 90 + 45);
    assert(Angle(0.0, 0.0, -1.0, -1.0) == 180 + 45);
    assert(Angle(0.0, 0.0,  1.0, -1.0) == 270 + 45);
    assert(Angle(1.0, 1.0,  0.0,  0.0) == 180 + 45);

    assert(Cos(5, 10,  0) == (5 + 10 * 1));
    assert(Sin(5, 10,  0) == (5 + 10 * 0));
    assert(Sin(5, 10, 90) == (5 + 10 * 1));

    assert(Distance(0.0, 0.0, 5.0, 0.0) == 5);
    assert(Distance(0.0, 0.0, 0.0, 5.0) == 5);
    assert(Distance(0.0, 0.0, 5.0, 5.0) == std::sqrt(50));

    assert(round(0.011) == 0.000);
    assert(round(0.011) != 0.011);
    assert(std::round(0.011) != 0.011);
    assert(Round(0.0110, 2) == 0.01000000);
    assert(Round(0.0190, 2) == 0.02000000);
    assert(Round(0.0101, 3) == 0.0100);
    assert(Round(0.0109, 3) == 0.0110);

    assert(TriangleArea(0, 0, 5, 0, 0, 10) == 25.0);    // Triangle
    assert(TriangleHeight(0, 0, 5, 0, 0, 10) == 10.0);  // Triangle

    double x, y;
    assert(!LineIntersect(1, 2, 5, 5, 2, 1, 6, 4, x, y));     // Parallel
    assert(!LineIntersect(0, 0, 5, 5, 1, 1, 4, 4, x, y));     // Coincident
    assert(LineIntersect(1, 1, -1, -1, 2, -2, -2, 2, x, y));  // Intersection at Origin
    assert(LineIntersect(2, 2, 2, 10, 0, 4, 10, 4, x, y));    // Intersection at (2,4)

    Numbers numbers {10, -1, 2};
    Numbers expected {-1, 2, 10};
    Numbers inOrder = Sort(numbers);
    assert(Equal(numbers, expected));         // Order is not important.
    assert(Equal(inOrder, expected, true));   // Order is important.

    // Strings
    assert(Join(std::vector<std::string> {"Hello", "World!"}, char(32)) == "Hello World!");
    assert(Equal(Split("Hello World!", char(32)), std::vector<std::string> {"Hello", "World!"}));
    assert(Equal(Trim(std::vector<std::string> {"100", "001"}, '0'), {"1", "1"}));
    assert(Trim("0.100", '0') == ".1");

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

    p1 = Zero;
    assert(p1 == Origin);

    p1 = Origin;
    p1 = p1.position(90, 10);
    assert(p1.round() == Point(0, 10));

    // Output
    //View(1);
    //View(0.5);
    //View(1.5f);

    //View(p1);
    //View(p2);
    //View(p1.XY());

    // Compare point vector.
    Point a(1, 1), b(2, 2), c(3, 3);
    assert(Equal(Points{a, b}, {b, a}));
    assert(Equal({a, b, c}, Points{c, b, a}));
    assert(Equal(Points{a + b, c}, {c, c}));
    assert(!Equal(Points{a, b}, Points{a, c}));

    // Sum
    assert(Equal(Sum({Point(1, 1), Point(2, 2)}, 5), {Point(6, 6), Point(7, 7)}));
    assert(Equal(Sum({Point(1, 1), Point(2, 2)}, Point(1, 2)), {Point(2, 3), Point(3, 4)}));

    // Sum of distances
    assert(SumDistances({Origin, Point(10, 0), Point(10, 20), Point(-10, 20)}) == 50.0);

    // Sort X-axis
    auto points = Sort({Origin, Point(-10, 0), Point(10, 0), Point(-5, -1)});
    Points expected = {Point(-10, 0), Point(-5, -1), Origin, Point(10, 0)};
    assert(Equal(points, expected, true));
    //View(points);
    //View(expected);

    // Sort Y-axis
    points = Sort({Origin, Point(-10, 0), Point(10, 0), Point(-5, -1)}, false);
    expected = {Point(-5, -1), Point(-10, 0), Origin, Point(10, 0)};
    assert(Equal(points, expected, true));
    //View(points);
    //View(expected);

    // Total
    assert(Total({Point(1, 1), Point(1, 1), Point(1, 1)}) == Point(3, 3));

    // Average
    Point result;
    assert(Average({Point(1, 3), Point(2, 1), Point(3, 2)}, result));
    assert(result.equal(Point(2, 2)));

    // Organize
    assert(Organize({}).empty());
    assert(Equal(Organize({Origin}), {Point(0, 0)}));
    assert(Equal(Organize({Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1)}),
                 {Point(1, 1), Point(-1, -1), Point(1, -1), Point(-1, 1)}));
    assert(Equal(Organize({Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1)}),
                 {Point(1, 1), Point(-1, 1), Point(-1, -1), Point(1, -1)}, true));

    // Angle between three points
    assert(Angle(Origin, Point(1, 1), Point(0, 1))       == 45);
    assert(Angle(Point(1, 1), Point(2, 2), Point(1, 2))  == 45);
    assert(Angle(Origin, Point(1, 1), Point(-1,  1))     == 90);
    assert(Angle(Origin, Point(1, 1), Point(-1, -1))     == 180);
    assert(Angle(Origin, Point(1, 1), Point(0, -1))      == 180 + 45);
    assert(Angle(Origin, Point(1, 1), Point(1, -1))      == 270);
    assert(Angle(Origin, Point(1, 0), Point(0, 1))       == 90);
    assert(Angle(Origin, Point(1, 0), Point(0, 1), true) == -90);
    assert(Angle(Origin, Origin, Origin)      == 0);
    assert(Angle(Origin, Origin, Point(1, 1)) == 45);

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
    //View(line.points());
    //View(line1.points());
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

    // Angle : line direction
    assert(Line(0, 0, 1, 1).angle() == 45);
    assert(Line(1, 1, 0, 0).angle() == 180 + 45);
    assert(Line(0, 0, 1, 1).angle() != Line(1, 1, 0, 0).angle());

    // Intersection
    assert(LineIntersect(Line(0, 0, 1, 1), Line(1, 0, 0, 1), p) == true);
    assert(p == Point(0.5, 0.5));
    assert(LineIntersect(Line(0, 0, 1, 1), Line(0, 1, 1, 2), p) == false);
    assert(LineIntersect(Line(0, 0, 1, 1), Line(1, 1, 0, 0), p) == false);

    // Perpendicular
    assert(Perpendicular(Line(-1, 0, 1, 0), Point(0.5, 2)).round() ==
           Line(0.5, 0, 0.5, 2)); // Between the two points.
    assert(Perpendicular(Line(-1, 0, 1, 0), Point(2.5, 2)).round() ==
           Line(2.5, 0, 2.5, 2)); // Out of range.
    assert(Perpendicular(Line(-1, 0, 1, 0), Point(-1.5, 2)).round() ==
           Line(-0.5, 0, -1.5, 2));// Out of range.

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
    //View(Triangle(a, b, c).points());
    //View(Triangle(a, b, c).lengthOfSides());

    // Area, height
    assert(TriangleArea(Origin, Point(20, 0), Point(0, 30)) == 300.0);
    assert(TriangleHeight(Origin, Point(20, 0), Point(0, 30)) == 30.0);

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
    //View(rectangle.points());
    //View(rectangle.lengthOfSides());

    Point a(1, 1), b(5, 1), c(5, 5), d(1, 5), e(5, 5);
    assert(Rectangle(a, b, c, d) == Rectangle(d, a, b, c));             // Equal
    assert(Rectangle(a, b, c, e) == Rectangle(a, b, c, a + a + c));     // Equal
    assert(Rectangle(a, b, c, e) == Rectangle(a, b, c, a * 4.0 + 1.0)); // Equal
    assert(Rectangle(a, b, c, d).round().area() == Rectangle(d, a, b, c).round().area());
    assert(Rectangle(a, b, c, d).round().perimeter() == Rectangle(d, a, b, c).round().perimeter());
    //View(Rectangle(a, b, c, d).points());
    //View(Rectangle(d, a, b, c).points());

    std::cout << "Rectangle test finished!\n";
}

void circle()
{
    Ellipse ellipse(Origin, 8, 2);
    Circle circle(Origin, 4);
    //View(circle.sides);
    //View(ellipse.sides);
    assert(circle.area() == ellipse.area());
    assert(circle.horizontalRadius != ellipse.horizontalRadius);
    assert(circle.verticalRadius != ellipse.verticalRadius);

}

void regularPolygons()
{
    // Polygon
    RegularPolygon polygon;
    polygon.setup(Point(0, 0), -1,  0,  0); // Return empty
    assert(polygon.points().empty());
    polygon.setup(Point(0, 0),  0,  0, -1); // Return empty
    assert(polygon.points().empty());
    polygon.setup(Point(1, 1),  0,  0,  0); // Return empty
    assert(polygon.points().empty());
    polygon.setup(Point(0, 0),  1,  0,  1); // Return empty
    assert(polygon.points().empty());

    //View(polygon.setup(Point(0, 0),  1,    0,  3)); // Triangle
    //View(polygon.setup(Point(0, 0),  1,   45,  3)); // Triangle
    //View(polygon.setup(Point(0, 0),  1,   90,  3)); // Triangle
    //View(polygon.setup(Point(0, 0),  1,  180,  3)); // Triangle
    //View(polygon.setup(Point(0, 0),  1,  270,  3)); // Triangle
    //View(polygon.setup(Point(0, 0),  1,    0,  4)); // Rectangle
    //View(polygon.setup(Point(0, 0),  2,   45,  4)); // Rectangle
    //View(polygon.setup(Point(0, 0),  1,    0,  5)); // Pentagon
    //View(polygon.setup(Point(0, 0),  1,   45,  5)); // Pentagon

    // Rectangle (1,0)(0,1)(-1,0)(0,-1)
    polygon.setup(Point(0, 0),  1,  0,  4);
    assert(round(polygon.sideLength()) == round(Point(1, 0).distance(Point(0, 1))));

    RegularPolygon p1, p2;
    p1.setup(Origin, std::sqrt(2), 0, 4);
    p2.setup(Origin, std::sqrt(2), 90, 4);
    //View(p1.points());
    //View(p1.round().points());
    //View(p2.points());
    //View(p2.round().points());
    assert(p1.round() == p2.round());       // Equal
    assert(p1.round().equal(p2.round()));   // Equal
    assert(!p1.equal(p2));                  // Same but different precision.

    // Rectangle 2 x 2 : (1,1)(-1,1)(-1,-1)(1,-1)
    polygon = RegularPolygon(Origin, std::sqrt(2), 45, 4);
    //View(polygon.points());
    assert(polygon.isConvex());
    assert(round(polygon.area()) == 4.0);
    assert(round(polygon.perimeter()) == 8.0);
    assert(round(polygon.sideLength()) == 2.0);
    polygon.sides = 4;
    assert(polygon.state() == false);   // Unmodified data!
    polygon.sides = 5;
    assert(polygon.state() == true);    // Modified data!

    // Polygon
    polygon = RegularPolygon(Origin, 1, 0, 5);
    assert(polygon.isConvex());
    //View(polygon.averageLength());
    //View(polygon.sideLength());
    //View(polygon.lengthOfSides());

    polygon = RegularPolygon(Origin, 1, 0, 6);
    assert(polygon.isConvex());

    polygon = RegularPolygon(Origin, 1, 0, 360);
    assert(polygon.isConvex());

    std::cout << "Polygon test finished!\n";
}

void irregularPolygon()
{
    IrregularPolygon polygon;

    polygon.setup({Point(0, 0), Point(0, 1), Point(1, 1)});             // Triangle
    assert(polygon.isConvex());
    assert(Round(polygon.area(), 2) == 0.5);
    assert(polygon.perimeter() == 2 + std::sqrt(2));

    polygon.setup(Triangle(Origin, Point(0, 1), Point(1, 1)).points()); // Triangle
    assert(polygon.isConvex());
    assert(Round(polygon.area(), 2) == 0.5);
    assert(polygon.perimeter() == 2 + std::sqrt(2));

    polygon.setup({Origin, Point(1, 0), Point(1, 1), Point(0, 1)});     // Rectangle
    assert(polygon.isConvex());
    assert(round(polygon.area()) == 1.0);
    assert(round(polygon.perimeter()) == 4.0);

    polygon.setup(Rectangle(Origin, 50, 20).points());                  // Rectangle
    assert(polygon.isConvex());
    assert(round(polygon.area()) == 50 * 20);
    assert(round(polygon.perimeter()) == 2 * 50 + 2 * 20);

    // Point type behavior. Returning empty.
    assert(IrregularPolygon({Point(1, 1)}).points().empty());

    // Line type behavior.
    assert(!IrregularPolygon(Line(Origin, Point(1, 1)).points()).points().empty());
    assert(IrregularPolygon(Line(Origin, Point(1, 1)).points()).points().size() == 2);

    // Convex polygon
    polygon = IrregularPolygon({Point(0, 2), Point(-2, 1), Point(-1, -1),
                                Point(1, -1), Point(2, 1)});
    assert(polygon.isConvex());

    // Concave polygon
    polygon = IrregularPolygon({Point(0, 2), Point(-2, 1), Point(1, 0),
                                Point(-1, -1), Point(1, -1), Point(2, 1)});
    assert(!polygon.isConvex());

    std::cout << "Irregular Polygon test finished!\n";
}

void svg()
{
    Line line(Point(10, 150), Point(500, 150));
    Triangle triangle(Point(10, 120), Point(150, 120), -100);
    Rectangle rectangle(Point(300, 160), 150, 300);
    Circle circle(Point(250, 80), 50);
    Ellipse ellipse(Point(400, 80), 80, 50);
    RegularPolygon rpolygon(Point(200, 300), 100, 90, 6);
    IrregularPolygon ipolygon(Sum(rpolygon.points(), Point(-100, 200)));

    std::vector<Base> shapes {
        line, triangle, rectangle, circle, ellipse, rpolygon, ipolygon
    };

    std::string strShape{};
    for (auto item : shapes) {
        strShape += SVG::polygon(SVG::Shape("Poly", SVG::RED, SVG::GREEN, 1.0, item.points()));
    }

    auto svg = SVG::svg(600, 600, strShape, SVG::Metadata());
    Save(svg, "svgOutput.svg");
    //View(svg);

    std::cout << "SVG test finished!\n";
}
