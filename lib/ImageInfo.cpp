#include <CIL/ImageInfo.hpp>
#ifdef CIL_JPEG_ENABLED
#include <CIL/JPEG/JPEGHandler.hpp>
#endif
#ifdef CIL_PNG_ENABLED
#include <CIL/PNG/PNGHandler.hpp>
#endif

namespace CIL {
    void ImageInfo::print_image_info()
    {
        std ::cerr << "Width:\t" << width << "\nHeight:\t" << height
                   << "\nNum Components:\t" << num_components
                   << "\nColor space:\t" << static_cast<int>(color_model)
                   << "\n";
    }
    void ImageInfo::destroy(ImageInfo* img_info)
    {
        switch (img_info->image_type)
        {
#ifdef CIL_JPEG_ENABLED
            case ImageType::JPEG: JPEG::JpegHandler::destroy(img_info); break;
#endif
#ifdef CIL_PNG_ENABLED
            case ImageType::PNG: PNG::destroyPNGImageInfo(img_info); break;
#endif
            case ImageType::PPM: break;
        }
        delete[] img_info->data;
        delete img_info;
    }
} // namespace CIL
