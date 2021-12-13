#include <CIL/Core/Debug.hpp>
#include <CIL/Core/Types.hpp>
#include <CIL/Core/Utils.hpp>
#include <CIL/ImageInfo.hpp>
#include <CIL/Pixel.hpp>
#include <cmath>
#include <utility>

namespace CIL {
    namespace utils {
        const double pi = std::atan(1.0) * 4;

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

        void multiplyConstantToSequence(Sequence<double>& matrix,
                                        double constant)
        {
            for (auto i = 0U; i < matrix.size(); i++)
                for (auto j = 0U; j < matrix[i].size(); j++)
                    matrix[i][j] *= constant;
        }

        static double computeOverlappingArea(Pixel px, Coordinate S)
        {
            double left = 0, right = 0, up = 0, down = 0;
            left = std::max(static_cast<double>(px.col()), S.x - 0.5);
            right = std::min(static_cast<double>(px.col() + 1), S.x + 0.5);
            up = std::max(static_cast<double>(px.row()), S.y - 0.5);
            down = std::min(static_cast<double>(px.row() + 1), S.y + 0.5);
            if (left > right || up > down)
                return 0;
            double res = (right - left) * (down - up);
            return res;
        }

        DetachedFPPixel bilinearInterpolation(ImageInfo& img_info,
                                              Coordinate source)
        {
            std::pair<int, int> x_range;
            std::pair<int, int> y_range;
            x_range = {source.x - 0.5, source.x + 0.5};
            y_range = {source.y - 0.5, source.y + 0.5};
            DetachedFPPixel dpx(img_info.numComponents());

            for (auto i = y_range.first; i <= y_range.second; ++i)
            {
                for (auto j = x_range.first; j <= x_range.second; ++j)
                {
                    auto px = img_info(i, j);
                    if (!px.isValid())
                        continue;
                    auto wt = computeOverlappingArea(px, source);
                    dpx += wt*px;
                }
            }
            return dpx;
        }
    } // namespace utils
} // namespace CIL
