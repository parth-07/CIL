#include <CIL/ImageInfo.hpp>
#ifdef CIL_JPEG_ENABLED
#include <CIL/JPEG/JPEGHandler.hpp>
#endif
#ifdef CIL_PNG_ENABLED
#include <CIL/PNG/PNGHandler.hpp>
#endif

namespace CIL {
    ImageInfo::ImageInfo(uint32_t width, uint32_t height,
                         uint32_t num_components, uint32_t sample_depth,
                         ColorModel color_model, ImageType image_type,
                         std::unique_ptr<uint8_t[]> data,
                         const void* internal_info)
        : m_color_model(color_model), m_image_type(image_type),
          m_data(width, height, num_components, sample_depth, std::move(data)),
          m_internal_info(internal_info)
    {}

    // TODO: We cannot just copy the internal_info pointer
    ImageInfo::ImageInfo(const ImageInfo& other)
        : m_color_model(other.m_color_model), m_image_type(other.m_image_type),
          m_data(other.m_data), m_internal_info(other.m_internal_info)
    {}

    ImageInfo& ImageInfo::operator=(const ImageInfo& other)
    {
        if (this == &other)
            return *this;
        m_color_model = other.m_color_model;
        m_image_type = other.m_image_type;
        m_data = other.m_data;
        return *this;
    }

    // TODO: We cannot just copy the internal_info pointer
    ImageInfo::ImageInfo(ImageInfo&& other)
        : m_color_model(other.m_color_model), m_image_type(other.m_image_type),
          m_data(std::move(other.m_data))
    {
        auto temp = other.m_internal_info;
        other.m_internal_info = m_internal_info;
        m_internal_info = temp;
    }

    ImageInfo& ImageInfo::operator=(ImageInfo&& other)
    {
        if (this == &other)
            return *this;
        m_color_model = other.m_color_model;
        m_image_type = other.m_image_type;
        m_data = std::move(other.m_data);
        return *this;
    }

    void ImageInfo::printImageInfo()
    {
        std ::cerr << "Width:\t" << m_data.width() << "\nHeight:\t"
                   << m_data.height() << "\nNum Components:\t"
                   << m_data.numComponents() << "\nColor space:\t"
                   << static_cast<int>(m_color_model) << "\n";
    }
    ImageInfo::~ImageInfo()
    {
        switch (m_image_type)
        {
#ifdef CIL_JPEG_ENABLED
            case ImageType::JPEG: JPEG::JPEGHandler::destroy(this); break;
#endif
#ifdef CIL_PNG_ENABLED
            case ImageType::PNG: PNG::PNGHandler::destroy(this); break;
#endif
            case ImageType::PPM: break;
        }
    }

    uint32_t ImageInfo::width() const { return m_data.width(); }
    uint32_t ImageInfo::height() const { return m_data.height(); }
    uint32_t ImageInfo::numComponents() const { return m_data.numComponents(); }
    uint32_t ImageInfo::sampleDepth() const { return m_data.sampleDepth(); }
    ColorModel ImageInfo::colorModel() const { return m_color_model; }
    uint64_t ImageInfo::size() const { return width() * height(); }

    const void* ImageInfo::internalInfo() const { return m_internal_info; }
    void ImageInfo::setColorModel(ColorModel color_model)
    {
        m_color_model = color_model;
    }

    Pixel ImageInfo::operator()(uint32_t row, uint32_t col)
    {
        return m_data(row, col);
    }
    uint8_t& ImageInfo::operator()(uint32_t row, uint32_t col, int comp)
    {
        return m_data(row, col, comp);
    }

    const uint8_t& ImageInfo::operator()(uint32_t row, uint32_t col,
                                         int comp) const
    {
        return m_data(row, col, comp);
    }

    CIL::ImageInfo readImage(const char* filename)
    {
#ifdef CIL_PNG_ENABLED
        if (PNG::PNGHandler::isPNGFile(filename))
        {
            return PNG::PNGHandler::readAs8RGB(filename);
        }
#endif
#ifdef CIL_JPEG_ENABLED
        if (JPEG::JPEGHandler::isJPEGFile(filename))
        {
            return JPEG::JPEGHandler::read(filename);
        }
#endif
        return ImageInfo();
    }

    CIL::ImageInfo readImage(const std::string& filename)
    {
        return readImage(filename.c_str());
    }

    bool ImageInfo::save(const std::string& filename) const
    {
        switch (m_image_type)
        {
#ifdef CIL_PNG_ENABLED
            case ImageType::PNG:
                return PNG::PNGHandler::write(this, filename.c_str());
#endif
#ifdef CIL_JPEG_ENABLED
            case ImageType::JPEG:
                return JPEG::JPEGHandler::write(this, filename.c_str());
#endif
            case ImageType::PPM: break;
        }
        return false;
    }

    bool ImageInfo::empty() const { return m_data.empty(); }
} // namespace CIL
