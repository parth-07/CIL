#include <CIL/JPEG/JPEGCore.hpp>
#include <cstring>

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
        JPEG::ImageInfo::ImageInfo(uint32_t width, uint32_t height,
                                   int num_components, JColorType color_type,
                                   uint8_t* data)
            : m_width(width), m_height(height),
              m_num_components(num_components), m_color_type(color_type),
              m_data(data)
        {}

        JPEG::ImageInfo::ImageInfo(const JPEG::ImageInfo& other)
        {
            *this = other;
        }

        JPEG::ImageInfo& JPEG::ImageInfo::operator=(const ImageInfo& other)
        {
            m_width = other.m_width;
            m_height = other.m_height;
            m_num_components = other.m_num_components;
            m_color_type = other.m_color_type;
            if (other.m_data != nullptr)
            {
                size_t size = m_width * m_height * m_num_components;
                m_data = new uint8_t[size];
                std::memcpy(m_data, other.m_data, size);
            }
            return *this;
        }

        JPEG::ImageInfo::ImageInfo(JPEG::ImageInfo&& rvalue) { *this = rvalue; }

        JPEG::ImageInfo& JPEG::ImageInfo::operator=(JPEG::ImageInfo&& rvalue)
        {
            if (this == &rvalue)
                return *this;
            m_width = rvalue.m_width;
            m_height = rvalue.m_height;
            m_num_components = rvalue.m_num_components;
            m_color_type = rvalue.m_color_type;
            m_data = rvalue.m_data;
            rvalue.m_data = nullptr;
            return *this;
        }

        const JPEG::ImageInfo*
        JPEG::ImageInfo::fromCILImage(const CIL::ImageInfo* cil_img_info)
        {
            JPEG::ImageInfo* jpeg_img_info = new JPEG::ImageInfo();
            jpeg_img_info->m_height = cil_img_info->height();
            jpeg_img_info->m_width = cil_img_info->width();
            jpeg_img_info->m_num_components = cil_img_info->numComponents();
            jpeg_img_info->m_color_type = handleColorModel(
                cil_img_info->colorModel());
            jpeg_img_info->m_data = const_cast<uint8_t*>(
                &(*cil_img_info)(0, 0, 0));
            return jpeg_img_info;
        }

        CIL::ImageInfo JPEG::ImageInfo::toCILImage()
        {
            std::unique_ptr<uint8_t[]> data(this->m_data);
            const uint8_t sample_depth = 8;
            void* internal_info = this;
            CIL::ImageInfo cil_img_info(m_width, m_height, m_num_components,
                                        sample_depth,
                                        handleColorModel(m_color_type),
                                        CIL::ImageType::JPEG, std::move(data),
                                        internal_info);
            m_data = nullptr;
            return cil_img_info;
        }

    } // namespace JPEG
} // namespace CIL
