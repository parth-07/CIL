#include <CIL/Core/Utils.hpp>
#include <cmath>

namespace CIL {
    namespace utils {
        static const double pi = std::atan(1.0) * 4;

        double convertDegreesToRadians(double degrees)
        {
            return pi / 180 * degrees;
        }

        Coordinate computeCoordinateAfterRotation(Coordinate origin,
                                                  Coordinate P, int degrees)
        {
            origin.y = -origin.y;
            P.y = -P.y;
            Coordinate relative_P = P - origin;
            Coordinate relative_Q;
            auto rad = convertDegreesToRadians(degrees);
            relative_Q.x = relative_P.x * std::cos(rad) -
                           relative_P.y * std::sin(rad);
            relative_Q.y = relative_P.x * std::sin(rad) +
                           relative_P.y * std::cos(rad);
            Coordinate Q(relative_Q.x + origin.x, -(relative_Q.y + origin.y));
            return Q;
        }
    } // namespace utils
} // namespace CIL
