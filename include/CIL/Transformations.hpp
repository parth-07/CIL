#ifndef CIL_TRANFORMATIONS_HPP
#define CIL_TRANFORMATIONS_HPP
#include <cstdint>

namespace CIL {
    class ImageInfo;
    struct Dimensions
    {
        uint32_t top = 0, bottom = 0, left = 0, right = 0;
        Dimensions(uint32_t pad_all) { top = bottom = left = right = pad_all; }
    };

    void invertColor(ImageInfo&);
    void padImage(ImageInfo& img, const Dimensions& dims);
} // namespace CIL
#endif
