#include <CIL/Core/Coordinate.hpp>
#include <cmath>
namespace CIL {

    Coordinate operator-(const Coordinate& a, const Coordinate& b)
    {
        Coordinate c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        return c;
    }

    int Coordinate::getNearestIntegralX() const {
        return static_cast<int>(std::lround(x));
    }
    int Coordinate::getNearestIntegralY() const {
        return static_cast<int>(std::lround(y));
    }
} // namespace CIL