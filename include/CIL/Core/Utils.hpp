#ifndef CIL_CORE_MATHS_HPP
#define CIL_CORE_MATHS_HPP

#include <CIL/Core/Coordinate.hpp>
#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/Core/Types.hpp>

namespace CIL {
    class ImageInfo;
    namespace utils {
        extern const double pi;
        double convertDegreesToRadians(double degrees);
        Coordinate computeCoordinateAfterRotation(Coordinate origin,
                                                  Coordinate P, int degrees);
        void multiplyConstantToSequence(Sequence<double>& matrix,
                                        double constant);
        DetachedFPPixel bilinearInterpolation(ImageInfo& img_info,
                                              Coordinate source);
    } // namespace utils
} // namespace CIL

#endif
