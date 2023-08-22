#include "svgToolBox.hpp"

#include <cassert>
#include <chrono>

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
void interpreter();
void loadTxt();

int main()
{
    std::cout << "Start tests ...\n";
    const auto start = std::chrono::steady_clock::now();

    // Sequence
    basic();
    point();
    line();
    triangle();
    rectangle();
    circle();
    regularPolygons();
    irregularPolygon();
    svg();
    //interpreter();
    loadTxt();

    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Finished tests.\nElapsed: " << elapsed_seconds << '\n';

    return 0;
}

void basic()
{
    using namespace smalltoolbox;

    // Math
    std::vector<unsigned> angles {
        0, 15, 30, 45, 60, 90, 120, 135, 150, 180, 225, 270, 315, 360
    };
    for (auto &a : angles) {
        assert(std::sin(a * std::numbers::pi / 180.0) == Math::sin(0, 1, a));
        assert(std::cos(a * std::numbers::pi / 180.0) == Math::cos(0, 1, a));
    }

    assert(Math::radians(360) == 2 * std::numbers::pi);
    assert(Math::angle(2 * std::numbers::pi) == 360);

    assert(Math::angle(0.0, 0.0,  0.0,  0.0) == 0);
    assert(Math::angle(0.0, 0.0,  1.0,  0.0) == 0);
    assert(Math::angle(0.0, 0.0,  0.0,  1.0) == 90);
    assert(Math::angle(0.0, 0.0, -1.0,  0.0) == 180);
    assert(Math::angle(0.0, 0.0,  0.0, -1.0) == 270);
    assert(Math::angle(0.0, 0.0,  1.0,  1.0) == 45);
    assert(Math::angle(0.0, 0.0, -1.0,  1.0) == 90 + 45);
    assert(Math::angle(0.0, 0.0, -1.0, -1.0) == 180 + 45);
    assert(Math::angle(0.0, 0.0,  1.0, -1.0) == 270 + 45);
    assert(Math::angle(1.0, 1.0,  0.0,  0.0) == 180 + 45);

    assert(Math::cos(5, 10,  0) == (5 + 10 * 1));
    assert(Math::sin(5, 10,  0) == (5 + 10 * 0));
    assert(Math::sin(5, 10, 90) == (5 + 10 * 1));

    assert(Math::distance(0.0, 0.0, 5.0, 0.0) == 5);
    assert(Math::distance(0.0, 0.0, 0.0, 5.0) == 5);
    assert(Math::distance(0.0, 0.0, 5.0, 5.0) == std::sqrt(50));

    assert(round(0.011) == 0.000);
    assert(round(0.011) != 0.011);
    assert(std::round(0.011) != 0.011);
    assert(Math::round(0.0110, 2) == 0.01000000);
    assert(Math::round(0.0190, 2) == 0.02000000);
    assert(Math::round(0.0101, 3) == 0.0100);
    assert(Math::round(0.0109, 3) == 0.0110);

    assert(Math::triangleArea(0, 0, 5, 0, 0, 10) == 25.0);    // Triangle
    assert(Math::triangleHeight(0, 0, 5, 0, 0, 10) == 10.0);  // Triangle

    Numbers numbers {10, -1, 2};
    Numbers expected {-1, 2, 10};
    Numbers inOrder = Math::sort(numbers);
    assert(Check::equal(numbers, expected));         // Order is not important.
    assert(Check::equal(inOrder, expected, true));   // Order is important.

    // Strings
    assert(Text::join(std::vector<std::string> {"Hello", "World!"}, char(32)) == "Hello World!");
    assert(Check::equal(Text::split("Hello World!", char(32)), std::vector<std::string> {"Hello", "World!"}));
    assert(Check::equal(Text::trim(std::vector<std::string> {"100", "001"}, '0'), {"1", "1"}));
    assert(Text::replace("010110", '1', "--") == "0--0----0");
    assert(Text::trim("0.100", '0') == ".1");
    assert(Text::ltrim("0001", '0') == "1");
    assert(Text::rtrim("1000", '0') == "1");

    assert(Text::strUpper("help") == "HELP");
    assert(Text::strLower("HelP") == "help");

    std::cout << "Basic test finished!\n";
}

void point()
{
    using namespace smalltoolbox;

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
    //Console::view(1);
    //Console::view(0.5);
    //Console::view(1.5f);

    //Console::view(p1);
    //Console::view(p2);
    //Console::view(p1.XY());

    // Compare point vector.
    Point a(1, 1), b(2, 2), c(3, 3);
    assert(Check::equal(Points{a, b}, {b, a}));
    assert(Check::equal({a, b, c}, Points{c, b, a}));
    assert(Check::equal(Points{a + b, c}, {c, c}));
    assert(!Check::equal(Points{a, b}, Points{a, c}));

    // Sum
    assert(Check::equal(Point::sum({Point(1, 1), Point(2, 2)}, 5), {Point(6, 6), Point(7, 7)}));
    assert(Check::equal(Point::sum({Point(1, 1), Point(2, 2)}, Point(1, 2)), {Point(2, 3), Point(3, 4)}));

    // Sum of distances
    assert(Point::sumDistances({Origin, Point(10, 0), Point(10, 20), Point(-10, 20)}) == 50.0);

    // Sort X-axis
    auto points = Point::sort({Origin, Point(-10, 0), Point(10, 0), Point(-5, -1)});
    Points expected = {Point(-10, 0), Point(-5, -1), Origin, Point(10, 0)};
    assert(Check::equal(points, expected, true));
    //Console::view(points);
    //Console::view(expected);

    // Sort Y-axis
    points = Point::sort({Origin, Point(-10, 0), Point(10, 0), Point(-5, -1)}, false);
    expected = {Point(-5, -1), Point(-10, 0), Origin, Point(10, 0)};
    assert(Check::equal(points, expected, true));
    //Console::view(points);
    //Console::view(expected);

    // Total
    assert(Point::total({Point(1, 1), Point(1, 1), Point(1, 1)}) == Point(3, 3));

    // Average
    Point result;
    assert(Point::average({Point(1, 3), Point(2, 1), Point(3, 2)}, result));
    assert(result.equal(Point(2, 2)));

    // Organize
    assert(Point::organize({}).empty());
    assert(Check::equal(Point::organize({Origin}), {Point(0, 0)}));
    assert(Check::equal(Point::organize({Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1)}),
    {Point(1, 1), Point(-1, -1), Point(1, -1), Point(-1, 1)}));
    assert(Check::equal(Point::organize({Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1)}),
    {Point(1, 1), Point(-1, 1), Point(-1, -1), Point(1, -1)}, true));

    // Angle between three points
    assert(Point::angle(Origin, Point(1, 1), Point(0, 1))       == 45);
    assert(Point::angle(Point(1, 1), Point(2, 2), Point(1, 2))  == 45);
    assert(Point::angle(Origin, Point(1, 1), Point(-1,  1))     == 90);
    assert(Point::angle(Origin, Point(1, 1), Point(-1, -1))     == 180);
    assert(Point::angle(Origin, Point(1, 1), Point(0, -1))      == 180 + 45);
    assert(Point::angle(Origin, Point(1, 1), Point(1, -1))      == 270);
    assert(Point::angle(Origin, Point(1, 0), Point(0, 1))       == 90);
    assert(Point::angle(Origin, Point(1, 0), Point(0, 1), true) == -90);
    assert(Point::angle(Origin, Origin, Origin)                 == 0);
    assert(Point::angle(Origin, Origin, Point(1, 1))            == 45);

    // String
    p1.X.value = 1.101;
    p1.Y.value = 1.202;
    assert(p1.toStr(true) == "1.101,1.202");
    //Console::view(p1.toStr());

    std::cout << "Point test finished!\n";
}

void line()
{
    using namespace smalltoolbox;

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
    //Console::view(line.points());
    //Console::view(line1.points());
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
    assert(!Point::lineIntersect(1, 2, 5, 5, 2, 1, 6, 4, p));     // Parallel
    assert(!Point::lineIntersect(0, 0, 5, 5, 1, 1, 4, 4, p));     // Coincident
    assert(Point::lineIntersect(1, 1, -1, -1, 2, -2, -2, 2, p));  // Intersection at Origin
    assert(Point::lineIntersect(2, 2, 2, 10, 0, 4, 10, 4, p));    // Intersection at (2,4)

    assert(Line::lineIntersect(Line(0, 0, 1, 1), Line(1, 0, 0, 1), p) == true);
    assert(p == Point(0.5, 0.5));
    assert(Line::lineIntersect(Line(0, 0, 1, 1), Line(0, 1, 1, 2), p) == false);
    assert(Line::lineIntersect(Line(0, 0, 1, 1), Line(1, 1, 0, 0), p) == false);

    // Perpendicular
    assert(Line::perpendicular(Line(-1, 0, 1, 0), Point(0.5, 2)).round() ==
           Line(0.5, 0, 0.5, 2)); // Between the two points.
    assert(Line::perpendicular(Line(-1, 0, 1, 0), Point(2.5, 2)).round() ==
           Line(2.5, 0, 2.5, 2)); // Out of range.
    assert(Line::perpendicular(Line(-1, 0, 1, 0), Point(-1.5, 2)).round() ==
           Line(-0.5, 0, -1.5, 2));// Out of range.

    std::cout << "Line test finished!\n";
}

void triangle()
{
    using namespace smalltoolbox;

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
    //Console::view(Triangle(a, b, c).points());
    //Console::view(Triangle(a, b, c).lengthOfSides());

    // Area, height
    assert(Point::triangleArea(Origin, Point(20, 0), Point(0, 30)) == 300.0);
    assert(Point::triangleHeight(Origin, Point(20, 0), Point(0, 30)) == 30.0);

    std::cout << "Triangle test finished!\n";
}

void rectangle()
{
    using namespace smalltoolbox;

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
    //Console::view(rectangle.points());
    //Console::view(rectangle.lengthOfSides());

    Point a(1, 1), b(5, 1), c(5, 5), d(1, 5), e(5, 5);
    assert(Rectangle(a, b, c, d) == Rectangle(d, a, b, c));             // Equal
    assert(Rectangle(a, b, c, e) == Rectangle(a, b, c, a + a + c));     // Equal
    assert(Rectangle(a, b, c, e) == Rectangle(a, b, c, a * 4.0 + 1.0)); // Equal
    assert(Rectangle(a, b, c, d).round().area() == Rectangle(d, a, b, c).round().area());
    assert(Rectangle(a, b, c, d).round().perimeter() == Rectangle(d, a, b, c).round().perimeter());
    //Console::view(Rectangle(a, b, c, d).points());
    //Console::view(Rectangle(d, a, b, c).points());

    std::cout << "Rectangle test finished!\n";
}

void circle()
{
    using namespace smalltoolbox;

    Ellipse ellipse(Origin, 8, 2);
    Circle circle(Origin, 4);
    //Console::view(circle.sides);
    //Console::view(ellipse.sides);
    assert(circle.area() == ellipse.area());
    assert(circle.horizontalRadius != ellipse.horizontalRadius);
    assert(circle.verticalRadius != ellipse.verticalRadius);

}

void regularPolygons()
{
    using namespace smalltoolbox;

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

    //Console::view(polygon.setup(Point(0, 0),  1,    0,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,   45,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,   90,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,  180,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,  270,  3)); // Triangle
    //Console::view(polygon.setup(Point(0, 0),  1,    0,  4)); // Rectangle
    //Console::view(polygon.setup(Point(0, 0),  2,   45,  4)); // Rectangle
    //Console::view(polygon.setup(Point(0, 0),  1,    0,  5)); // Pentagon
    //Console::view(polygon.setup(Point(0, 0),  1,   45,  5)); // Pentagon

    // Rectangle (1,0)(0,1)(-1,0)(0,-1)
    polygon.setup(Point(0, 0),  1,  0,  4);
    assert(round(polygon.sideLength()) == round(Point(1, 0).distance(Point(0, 1))));

    RegularPolygon p1, p2;
    p1.setup(Origin, std::sqrt(2), 0, 4);
    p2.setup(Origin, std::sqrt(2), 90, 4);
    //Console::view(p1.points());
    //Console::view(p1.round().points());
    //Console::view(p2.points());
    //Console::view(p2.round().points());
    assert(p1.round() == p2.round());       // Equal
    assert(p1.round().equal(p2.round()));   // Equal
    assert(!p1.equal(p2));                  // Same but different precision.

    // Rectangle 2 x 2 : (1,1)(-1,1)(-1,-1)(1,-1)
    polygon = RegularPolygon(Origin, std::sqrt(2), 45, 4);
    //Console::view(polygon.points());
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
    //Console::view(polygon.averageLength());
    //Console::view(polygon.sideLength());
    //Console::view(polygon.lengthOfSides());

    polygon = RegularPolygon(Origin, 1, 0, 6);
    assert(polygon.isConvex());

    polygon = RegularPolygon(Origin, 1, 0, 360);
    assert(polygon.isConvex());

    std::cout << "Polygon test finished!\n";
}

void irregularPolygon()
{
    using namespace smalltoolbox;

    IrregularPolygon polygon;

    polygon.setup({Point(0, 0), Point(0, 1), Point(1, 1)});             // Triangle
    assert(polygon.isConvex());
    assert(Math::round(polygon.area(), 2) == 0.5);
    assert(polygon.perimeter() == 2 + std::sqrt(2));

    polygon.setup(Triangle(Origin, Point(0, 1), Point(1, 1)).points()); // Triangle
    assert(polygon.isConvex());
    assert(Math::round(polygon.area(), 2) == 0.5);
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
    using namespace smalltoolbox;

    assert(!Color::RGBA(1, 0, 1, 0).equal(Color::RGBA(0, 1, 0, 1)));

    //Console::view(Color::RGBA(255, 255, 0, 0).toStr());
    //Console::view(Color::RGBA(255, 255, 0, 0).toStr(false));
    //Console::view(Color::RGBA(std::vector<int> {1}).toStr(true));
    //Console::view(Color::RGBA(std::vector<int> {1, 2}).toStr(true));
    //Console::view(Color::RGBA(std::vector<int> {1, 2, 3}).toStr(true));
    //Console::view(Color::RGBA(std::vector<int> {1, 2, 3, 4}).toStr(true));
    //Console::view(Color::RGBA(std::vector<int> {-1, -2, -3, -4}).toStr(true));

    assert(SVG::INT2HEX(0) == "00");
    assert(SVG::INT2HEX(10) == "0A");
    assert(SVG::INT2HEX(255) == "FF");

    assert(SVG::HEX2INT("#")   ==   0);
    assert(SVG::HEX2INT("##")  ==   0);
    assert(SVG::HEX2INT("#0#") ==   0);
    assert(SVG::HEX2INT("#0")  ==   0);
    assert(SVG::HEX2INT("0A")  ==  10);
    assert(SVG::HEX2INT("ff")  == 255);
    assert(SVG::HEX2INT("FF")  == 255);

    assert(SVG::HEX2RGB("#").empty());
    assert(SVG::HEX2RGB("#1234567").empty());
    assert(Check::equal(SVG::HEX2RGB("#0102"), {1, 2}));
    assert(Check::equal(SVG::HEX2RGB("#01020304"), {1, 2, 3, 4}));
    assert(Check::equal(SVG::HEX2RGB("#7F7F7F"), {127, 127, 127}));        // RGB
    assert(Check::equal(SVG::HEX2RGB("#7F7F7FFF"), {127, 127, 127, 255})); // RGBA
    //Console::view(SVG::HEX2RGB("#7F7F7FFF"));
    //Console::view(Color::RGBA(SVG::HEX2RGB("#7F7F7FFF")).toStr());

    std::vector<Base> shapes {
        Line(Point(10, 150), Point(500, 150)),
        Triangle(Point(10, 120), Point(150, 120), -100),
        Rectangle(Point(300, 160), 150, 300),
        Circle(Point(250, 80), 50),
        Ellipse(Point(400, 80), 80, 50),
        RegularPolygon(Point(200, 300), 100, 90, 6),
        IrregularPolygon(Point::sum(Polygon(Point(200, 300), 100, 90, 6).points(),
                                    Point(-100, 200)))
    };

    auto svg = Sketch::svg(600, 600, Sketch::Join(shapes), Sketch::Metadata());
    IO::save(svg, "svgOutput.svg");
    //Console::view(svg);

    std::cout << "SVG test finished!\n";
}

void interpreter()
{
    using namespace smalltoolbox;

    Interpreter interpreter;

    std::vector<std::string> commands {
        "",                     // Empty
        "Text",                 // Ignore : Invalid command!
        "Point",                // Ignore : Invalid command!
        "Points:",              // Ignore : Incomplete!
        "PoInTs:",              // Ignore : Incomplete!
        "Circle:",              // Ignore : Incomplete!
        "Points: {",            // Error  : expected '{ something }'
        "Points: }",            // Error  : expected '{ something }'
        "Points: {}",           // Ignore : expected '{ something }'
        "Points: } {",          // Ignore : expected '{ something }'
        "Points: {} {}",        // Error  : only a '{}' is expected
        "Points: { ( ) }",      // Ignore : expected '{ (x,y) }'
        "Points: { ( }",        // Error  : expected '{ (x,y) }'
        "Points:{(1,1)}",       // Ignore : one point
        "Points:{(1,1)(1,a)}",  // Error  : invalid argument
        "Points:{(1,1)(1,2)}",
        "Points:{(1,1) (1,2) (1,3) }",
        "Points:{(1,1) (1,2)} angle=45 label=Test",
        "Line:{(0,0) (5,5)}",
        "Line: {(0,0)} angle=90 length=20",
        "Line: angle=90 {(0,0)} length=20 label=Line1",
        "Rectangle:{(1,1)} width=100 height=100"
    };

    std::string error;
    for (auto command : commands) {
        auto result = interpreter.svg(command, error);
        if (!result.empty()) {
            Console::view(command);
            Console::view(result);
        }
        else {
            Console::view(error);
        }
    }

    std::cout << "Interpreter test finished!\n";
}

void loadTxt()
{
    using namespace smalltoolbox;

    Interpreter interpreter;

    std::string errors;
    auto svg = interpreter.load("Resources/test_commands.txt", errors);

    svg = Sketch::svg(100, 100, svg, Sketch::Metadata());
    IO::save(svg, "Resources/svgOutput.svg");

    //Console::view(svg);
    //Console::view(errors);

    std::cout << "Load Text File test finished!\n";
}
