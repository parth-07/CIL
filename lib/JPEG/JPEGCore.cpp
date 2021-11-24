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

        CIL::ImageInfo* JPEG::ImageInfo::toCILImage()
        {
            CIL::ImageInfo* cil_imageinfo = new CIL::ImageInfo();
            cil_imageinfo->height = this->height;
            cil_imageinfo->width = this->width;
            cil_imageinfo->num_components = this->num_components;
            cil_imageinfo->color_model = handleColorModel(this->color_model);
            cil_imageinfo->data = this->data;
            cil_imageinfo->internal_info = this;
            cil_imageinfo->image_type = ImageType::JPEG;
            return cil_imageinfo;
        }

        void JPEG::ImageInfo::fromCILImage(const CIL::ImageInfo* cil_imageinfo)
        {
            this->height = cil_imageinfo->height;
            this->width = cil_imageinfo->width;
            this->num_components = cil_imageinfo->num_components;
            this->color_model = handleColorModel(cil_imageinfo->color_model);
            this->data = cil_imageinfo->data;
            this->old_img_info = static_cast<const JPEG::ImageInfo*>(
                cil_imageinfo->internal_info);
        }
    } // namespace JPEG
} // namespace CIL
