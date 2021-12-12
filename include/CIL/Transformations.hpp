#ifndef CIL_TRANFORMATIONS_HPP
#define CIL_TRANFORMATIONS_HPP
#include <cstdint>
#include <map>
namespace CIL {
    class ImageInfo;
    class ImageMatrix;
    struct Dimensions;
    enum class KernelType
    {
        BOX_BLUR,
        GAUSSIAN_BLUR,
        EDGE_DETECTION
    };

    enum class RotationKind
    {
        rotation_by_sampling,
        rotation_by_area_mapping
    };

    void invertColor(ImageInfo&);
    void padImage(ImageInfo& img, const Dimensions& dims);
    void applyKernel(ImageInfo& img, KernelType kernel_type, int size = 3);
    void
    rotate(ImageInfo& img, int degrees,
           RotationKind rotation_kind = RotationKind::rotation_by_area_mapping);
    void convertToGrayscale(ImageInfo& img);           
} // namespace CIL
#endif
