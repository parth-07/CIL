#ifndef CIL_CORE_MATHS_HPP
#define CIL_CORE_MATHS_HPP

#include <CIL/Core/Coordinate.hpp>

namespace CIL {
    namespace utils {
        double convertDegreesToRadians(double degrees);
        Coordinate computeCoordinateAfterRotation(Coordinate origin,
                                                  Coordinate P, int degrees);
    } // namespace utils
} // namespace CIL

#endif