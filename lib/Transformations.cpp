#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
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
} // namespace CIL
