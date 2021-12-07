#include <CIL/Core/Coordinate.hpp>
#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/Core/Utils.hpp>
#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <cmath>
#include <limits>

namespace CIL {
    void invertColor(ImageInfo& img)
    {
        for (auto px : img)
        {
            for (int i = 0; i < px.numComponents(); i++)
                px[i] = std::numeric_limits<uint8_t>::max() - px[i];
        }
    }

    void padImage(ImageInfo& img, const Dimensions& dims)
    {
        auto new_width = img.width() + dims.left + dims.right;
        auto new_height = img.height() + dims.top + dims.bottom;

        ImageMatrix new_img(new_width, new_height, img.numComponents(),
                            img.sampleDepth());

        ImageMatrix::iterator it = img.begin();
        const auto endpx = new_img(new_img.height() - dims.bottom,
                                   new_img.width() - dims.right);

        for (ImageMatrix::iterator px = new_img(dims.top, dims.left);
             px != endpx && it != img.end(); px++, it++)
        {
            px->copyComponents(*it);
        }
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

    // TODO: Add `preserve_image` parameter.
    void rotate(ImageInfo& img, int degrees, RotationKind rotation_kind)
    {
        ImageMatrix tf_img_data(img.width(), img.height(), img.numComponents(),
                                img.sampleDepth());

        Coordinate origin((img.width() / 2) + 0.5, (img.height() / 2) + 0.5);

        for (auto px : tf_img_data)
        {
            Coordinate P(px.col() + 0.5, px.row() + 0.5);
            auto source = utils::computeCoordinateAfterRotation(origin, P,
                                                                -degrees);
            if (rotation_kind == RotationKind::rotation_by_area_mapping)
            {
                DetachedFPPixel dpx(px.numComponents());
                uint32_t i = std::floor(source.y - 0.5);
                uint32_t j = std::floor(source.x - 0.5);
                auto neighbours = {img(i, j), img(i, j + 1), img(i + 1, j + 1),
                                   img(i + 1, j)};
                for (auto neighbour : neighbours)
                {
                    if (neighbour.empty())
                        continue;

                    auto wt = computeOverlappingArea(neighbour, source);
                    dpx += wt * DetachedFPPixel(neighbour);
                }
                px = dpx;
            } else if (rotation_kind == RotationKind::rotation_by_sampling)
            {
                auto source_pixel = img(std::lround(source.y),
                                        std::lround(source.x));
                if (source_pixel.empty())
                    continue;
                for (auto i = 0; i < px.numComponents(); ++i)
                {
                    px[i] = source_pixel[i];
                }
            }
        }
        img.setData(tf_img_data);
    }
} // namespace CIL
