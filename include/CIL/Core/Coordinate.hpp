#ifndef CIL_CORE_COORDINATE_HPP
#define CIL_CORE_COORDINATE_HPP

namespace CIL {

    struct Coordinate
    {
        double x, y;
        Coordinate(double p_x = 0, double p_y = 0) : x(p_x), y(p_y) {}
    };

    Coordinate operator-(const Coordinate& a, const Coordinate& b);
} // namespace CIL

#endif