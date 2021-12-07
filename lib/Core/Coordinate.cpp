#include <CIL/Core/Coordinate.hpp>

namespace CIL {

    Coordinate operator-(const Coordinate& a, const Coordinate& b)
    {
        Coordinate c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        return c;
    }
} // namespace CIL