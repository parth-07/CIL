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
    enum class RotationKind
    {
        rotation_by_sampling,
        rotation_by_area_mapping
    };

    void invertColor(ImageInfo&);
    void padImage(ImageInfo& img, const Dimensions& dims);
    void
    rotate(ImageInfo& img, int degrees,
           RotationKind rotation_kind = RotationKind::rotation_by_area_mapping);
} // namespace CIL
#endif
