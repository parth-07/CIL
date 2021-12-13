#ifndef CIL_CORE_COORDINATE_HPP
#define CIL_CORE_COORDINATE_HPP

namespace CIL {

    struct Coordinate
    {
        using ValueType = double;
        ValueType x, y;
        Coordinate(ValueType p_x = 0, ValueType p_y = 0) : x(p_x), y(p_y) {}
        int getNearestIntegralX() const;
        int getNearestIntegralY() const;
    };

    Coordinate operator-(const Coordinate& a, const Coordinate& b);
} // namespace CIL

#endif