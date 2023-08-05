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
    Point p1, p2; // (0, 0)

    assert(p1.equal(p2));
    assert(p1 == Point(0, 0));
    assert(p1 + Point(1, 1) == Point(1, 1));
    assert(p1 - Point(1, 1) == Point(-1, -1));
    assert(p1 * Point(2, 2) == Point(0, 0));

    view(1);
    view(0.5);
    view(1.5f);

    view(p1);
    view(p1.XY());
}

