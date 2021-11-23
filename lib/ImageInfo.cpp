#include <CIL/ImageInfo.hpp>

namespace CIL {
    void ImageInfo::print_image_info()
    {
        std ::cerr << "Width:\t" << width << "\nHeight:\t" << height
                   << "\nNum Components:\t" << num_components
                   << "\nColor space:\t" << static_cast<int>(color_model)
                   << "\n";
    }
} // namespace CIL
