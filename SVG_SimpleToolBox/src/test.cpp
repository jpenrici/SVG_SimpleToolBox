#include "svgToolBox.h"

#include <assert.h>

using namespace smalltoolbox;

void test();

int main()
{
    test();

    return 0;
}

void test()
{
    // Generic
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

    // Point
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

    // Output
    view(1);
    view(0.5);
    view(1.5f);

    view(p1);
    view(p2);
    view(p1.XY());

    // Line
    view(Line(Point(1, 1), Point(2, 2)).points());

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

    // SVG
    view("SVG");
    auto shape = SVG::Shape("Rectangle", RED, GREEN, 1.0,
                            Polygon().setup(Point(50, 50), 50, 45, 4));
    auto svg = SVG::svg(100, 100, SVG::polygon(shape), SVG::Metadata());
    save(svg, "svgOutput.svg");
    view(svg);
}
