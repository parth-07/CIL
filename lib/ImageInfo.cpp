#include <CIL/ImageInfo.hpp>
#include <CIL/JPEG/JPEGHandler.hpp>
#include <CIL/PNG/PNGHandler.hpp>

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
            case ImageType::JPEG:
                // JPEG::JpegHandler::destroy(img_info);
                break;
            case ImageType::PNG: PNG::destroyPNGImageInfo(img_info); break;
            case ImageType::PPM: break;
        }
        delete[] img_info->data;
        delete img_info;
    }
} // namespace CIL
