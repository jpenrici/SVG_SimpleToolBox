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

smalltoolbox::Point smalltoolbox::Point::operator*(double value)
{
    return Point(X.value * value, Y.value * value);
}

void smalltoolbox::Point::operator*=(double value)
{
    multiply(value, value);
}

bool smalltoolbox::Point::operator==(const Point point)
{
    return equal(point);
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
    return X.value == point.X.value && Y.value == point.Y.value;
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
std::vector<smalltoolbox::Point>
smalltoolbox::Polygon::setup(Point center, double radius,
                             double angle, unsigned sides)
{
    // Check
    sides = sides > 360 ? 360 : sides;
    if (radius < 1 || sides < 3) {
        return {};
    }

    // Update
    this->center = center;
    this->angle = angle;
    this->radius = radius;
    this->sides = sides;

    vertices.clear();
    for (unsigned a = angle; a < 360 + angle; a += (360 / sides)) {
        vertices.push_back(Point(smalltoolbox::cos(center.X.value, radius, a),
                                 smalltoolbox::sin(center.Y.value, radius, a)));
    }

    return vertices;
}

std::vector<smalltoolbox::Point> smalltoolbox::Polygon::points()
{
    return vertices;
}

double smalltoolbox::Polygon::sideLength()
{
    if (vertices.size() < 2) {
        return 0;
    }

    return vertices[0].distance(vertices[1]);
}

double smalltoolbox::Polygon::diagonalLength()
{
    return 2 * radius;
}

// Line

double smalltoolbox::Line::length()
{
    return first.distance(second);
}

std::vector<smalltoolbox::Point> smalltoolbox::Line::points()
{
    return {first, second};
}

// SVG
const std::string smalltoolbox::SVG::INT2HEX(unsigned int value)
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

const std::string smalltoolbox::SVG::RGB2HEX(unsigned R, unsigned G, unsigned B)
{
    return "#" + INT2HEX(R) + INT2HEX(G) + INT2HEX(B);
}

const std::string smalltoolbox::SVG::RGBA2HEX(unsigned R, unsigned G, unsigned B, unsigned A)
{
    return RGB2HEX(R, G, B) + INT2HEX(A);
}

const std::string smalltoolbox::SVG::group(std::string id, std::string elements)
{
    id = id.empty() ? "<g>\n" : "<g id=\"" + id + "\" >\n";
    return elements.empty() ? "" : id + elements + "</g>\n";
}

const std::string smalltoolbox::SVG::polyline(Shape shape)
{

    if (shape.points.empty()) {
        return "<!-- Empty -->\n";
    }

    std::string values = "";
    for (unsigned i = 0; i < shape.points.size(); i++) {
        values += shape.points[i].toStr() + " ";
    }
    shape.name = shape.name.empty() ? "polyline" : shape.name;
    shape.stroke = shape.stroke.empty() ? "#000000" : shape.stroke;
    shape.fillOpacity = shape.fillOpacity < 0 ? 0 : std::min(shape.fillOpacity / 255, 1.0);
    shape.strokeOpacity = shape.strokeOpacity < 0 ? 0 : std::min(shape.strokeOpacity / 255, 1.0);
    std::string opacity = std::to_string(shape.fillOpacity);
    std::string strokeOpacity = std::to_string(shape.strokeOpacity);
    std::string strokeWidth = std::to_string(shape.strokeWidth);

    return {
        "     <polyline\n"
        "        id=\"" + shape.name + "\"\n"
        "        style=\"opacity:" + opacity + ";fill:" + shape.fill + ";stroke:" + shape.stroke + ";stroke-width:" + strokeWidth + ";stroke-opacity:" + strokeOpacity + ";stroke-linejoin:round;stroke-linecap:round\"\n"
        "        points=\"" + values + "\" />\n"
    };
}

const std::string smalltoolbox::SVG::polygon(Shape shape)
{

    if (shape.points.empty()) {
        return "<!-- Empty -->\n";
    }

    std::string values = "";
    for (unsigned i = 0; i < shape.points.size() - 1; i++) {
        values += shape.points[i].toStr() + " L ";
    }
    values += shape.points[shape.points.size() - 1].toStr();
    shape.name = shape.name.empty() ? "polygon" : shape.name;
    shape.fill = shape.fill.empty() ? "#FFFFFF" : shape.fill;
    shape.stroke = shape.stroke.empty() ? "#000000" : shape.stroke;
    shape.fillOpacity = shape.fillOpacity < 0 ? 0 : std::min(shape.fillOpacity / 255, 1.0);
    shape.strokeOpacity = shape.strokeOpacity < 0 ? 0 : std::min(shape.strokeOpacity / 255, 1.0);
    std::string opacity = std::to_string(shape.fillOpacity);
    std::string strokeOpacity = std::to_string(shape.strokeOpacity);
    std::string strokeWidth = std::to_string(shape.strokeWidth);
    return {
        "     <path\n"
        "        id=\"" + shape.name + "\"\n"
        "        style=\"opacity:" + opacity + ";fill:" + shape.fill + ";stroke:" + shape.stroke + ";stroke-width:" + strokeWidth + ";stroke-opacity:" + strokeOpacity + ";stroke-linejoin:round;stroke-linecap:round\"\n"
        "        d=\"M " + values + " Z\" />\n"
    };
}

const std::string smalltoolbox::SVG::svg(int width, int height,
                                         const std::string &xml, Metadata metadata)
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
        "   width=\"" + std::to_string(width) + "\"\n"
        "   height=\"" + std::to_string(height) + "\"\n"
        "   viewBox= \"0 0 " + std::to_string(width) + " " + std::to_string(height) + "\"\n"
        "   version=\"1.1\"\n"
        "   id=\"svg8\">\n"
        "  <title\n"
        "     id=\"title1\">" + metadata.title + "</title>\n"
        "  <defs\n"
        "     id=\"defs1\" />\n"
        "  <metadata\n"
        "     id=\"metadata1\">\n"
        "    <rdf:RDF>\n"
        "      <cc:Work\n"
        "         rdf:about=\"\">\n"
        "        <dc:format>image/svg+xml</dc:format>\n"
        "        <dc:type\n"
        "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
        "        <dc:title>" + metadata.title + "</dc:title>\n"
        "        <dc:date>" + metadata.date + "</dc:date>\n"
        "        <dc:publisher>\n"
        "          <cc:Agent>\n"
        "            <dc:title>" + metadata.publisherAgentTitle + "</dc:title>\n"
        "          </cc:Agent>\n"
        "        </dc:publisher>\n"
        "        <dc:subject>\n"
        "          <rdf:Bag>\n"
        "            <rdf:li></rdf:li>\n"
        "            <rdf:li></rdf:li>\n"
        "            <rdf:li></rdf:li>\n"
        "            <rdf:li></rdf:li>\n"
        "          </rdf:Bag>\n"
        "        </dc:subject>\n"
        "        <dc:creator>\n"
        "          <cc:Agent>\n"
        "            <dc:title>" + metadata.creator + "</dc:title>\n"
        "          </cc:Agent>\n"
        "        </dc:creator>\n"
        "        <cc:license\n"
        "           rdf:resource=\"http://creativecommons.org/publicdomain/zero/1.0/\" />\n"
        "        <dc:description>SVG created automatically by algorithm in C++.</dc:description>\n"
        "      </cc:Work>\n"
        "      <cc:License\n"
        "         rdf:about=\"http://creativecommons.org/publicdomain/zero/1.0/\">\n"
        "        <cc:permits\n"
        "           rdf:resource=\"http://creativecommons.org/ns#Reproduction\" />\n"
        "        <cc:permits\n"
        "           rdf:resource=\"http://creativecommons.org/ns#Distribution\" />\n"
        "        <cc:permits\n"
        "           rdf:resource=\"http://creativecommons.org/ns#DerivativeWorks\" />\n"
        "      </cc:License>\n"
        "    </rdf:RDF>\n"
        "  </metadata>\n"
        "  <!--      Created in C++ algorithm       -->\n"
        "  <!-- Attention: do not modify this code. -->\n"
        "\n"
        "" + xml + ""
        "\n"
        "  <!-- Attention: do not modify this code. -->\n"
        "</svg>"
    };
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

double smalltoolbox::angle(double x0, double y0, double x1, double y1)
{
    double result = angle(atan((y1 - y0) / (x1 - x0)));

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

double smalltoolbox::cos(double value, double radius, double angle)
{
    return value + radius * std::cos(radians(angle));
}

double smalltoolbox::sin(double value, double radius, double angle)
{
    return value + radius * std::sin(radians(angle));
}

double smalltoolbox::distance(double x0, double y0, double x1, double y1)
{
    return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}

void smalltoolbox::view(double value)
{
    std::cout << std::to_string(value) << '\n';
}

void smalltoolbox::view(Point point)
{
    std::cout << "(" << point.toStr() << ")" << '\n';
}

void smalltoolbox::view(std::array<double, 2> arr)
{
    std::cout << "x: " << std::to_string(arr[0]) << '\n'
              << "y: " << std::to_string(arr[1]) << '\n';
}

void smalltoolbox::view(std::vector<Point> points)
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

void smalltoolbox::view(std::string str)
{
    std::cout << str << '\n';
}

bool smalltoolbox::save(const std::string &text, std::string path)
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
