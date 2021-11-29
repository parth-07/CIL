#include <CIL/JPEG/JPEGCore.hpp>

namespace CIL {
    namespace JPEG {
        static ColorModel handleColorModel(const J_COLOR_SPACE jpeg_color_space)
        {
            switch (jpeg_color_space)
            {
                case JCS_RGB: return ColorModel::COLOR_RGB;
                case JCS_CMYK: return ColorModel::COLOR_CMYK;
                case JCS_GRAYSCALE: return ColorModel::COLOR_GRAY;
                default: throw("Unrecognised color model");
            }
        }
        static J_COLOR_SPACE handleColorModel(const ColorModel cil_color_space)
        {
            switch (cil_color_space)
            {
                case ColorModel::COLOR_RGB: return JCS_RGB;
                case ColorModel::COLOR_CMYK: return JCS_CMYK;
                case ColorModel::COLOR_GRAY: return JCS_GRAYSCALE;
                default: throw("Unrecognised color model");
            }
        }

        CIL::ImageInfo JPEG::ImageInfo::toCILImage()
        {
            std::unique_ptr<uint8_t[]> data(this->data);
            const uint8_t sample_depth = 8;
            const void* internal_info = this;
            CIL::ImageInfo cil_img_info(width, height, num_components,
                                        sample_depth,
                                        handleColorModel(color_model),
                                        CIL::ImageType::JPEG, std::move(data),
                                        internal_info);
            data = nullptr;
            return cil_img_info;
        }

        const JPEG::ImageInfo*
        JPEG::ImageInfo::fromCILImage(const CIL::ImageInfo* cil_img_info)
        {
            JPEG::ImageInfo* jpeg_img_info = new JPEG::ImageInfo();
            jpeg_img_info->height = cil_img_info->height();
            jpeg_img_info->width = cil_img_info->width();
            jpeg_img_info->num_components = cil_img_info->numComponents();
            jpeg_img_info->color_model = handleColorModel(
                cil_img_info->colorModel());
            jpeg_img_info->data = const_cast<uint8_t*>(
                &(*cil_img_info)(0, 0, 0));
            jpeg_img_info->old_img_info = static_cast<const JPEG::ImageInfo*>(
                cil_img_info->internalInfo());
            return jpeg_img_info;
        }
    } // namespace JPEG
} // namespace CIL
