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

    enum class Axis
    {
        X,
        Y
    };

    enum class RotationKind
    {
        rotation_by_sampling,
        rotation_by_area_mapping
    };

    enum class ResizeAlgorithm
    {
        nearest_neighbour_interpolation,
        bilinear_interpolation
    };

    void invertColor(ImageInfo&);
    void flipImage(ImageInfo& img, const Axis axis);
    void cropImage(ImageInfo& img, const Dimensions& dims);
    void padImage(ImageInfo& img, const Dimensions& dims);
    void applyKernel(ImageInfo& img, KernelType kernel_type, int size = 3);
    void
    rotate(ImageInfo& img, int degrees,
           RotationKind rotation_kind = RotationKind::rotation_by_area_mapping);
    void convertToGrayscale(ImageInfo& img);
    void resize(ImageInfo& img, uint32_t new_width, uint32_t new_height,
                ResizeAlgorithm resize_algorithm =
                    ResizeAlgorithm::bilinear_interpolation);
} // namespace CIL
#endif
