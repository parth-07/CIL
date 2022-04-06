#include <CIL/ImageInfo.hpp>
#ifdef CIL_JPEG_ENABLED
#include <CIL/JPEG/JPEGHandler.hpp>
#endif
#ifdef CIL_PNG_ENABLED
#include <CIL/PNG/PNGHandler.hpp>
#endif
#include <CIL/ImageHandler.hpp>
#include <CIL/PPM/PPMHandler.hpp>

#include <memory>
#include <vector>
namespace CIL {
    static std::vector<std::unique_ptr<ImageHandler>> registered_image_handlers;
    // static std::vector<ImageHandler*> registered_image_handlers = {new PPMHandler(), new PNG::PNGHandler(), };
    static int initializeImageHandlers()
    {
        registered_image_handlers.push_back(
            std::unique_ptr<ImageHandler>(new PPMHandler()));
#ifdef CIL_PNG_ENABLED
        registered_image_handlers.push_back(
            std::unique_ptr<ImageHandler>(new PNG::PNGHandler()));
#endif
#ifdef CIL_JPEG_ENABLED
        registered_image_handlers.push_back(
            std::unique_ptr<ImageHandler>(new JPEG::JPEGHandler()));
#endif
        return 1;
    }
    static int dummy = initializeImageHandlers();

    void AddImageHandler(std::unique_ptr<ImageHandler> handler)
    {
        registered_image_handlers.push_back(std::move(handler));
    }

    ImageInfo::ImageInfo(uint32_t width, uint32_t height,
                         uint32_t num_components, uint32_t sample_depth,
                         ColorModel color_model, const std::string& image_type,
                         std::unique_ptr<uint8_t[]> data, void* internal_info,
                         const std::string& internal_info_image_type)
        : m_color_model(color_model), m_image_type(image_type),
          m_data(width, height, num_components, sample_depth, std::move(data),
                 (color_model == ColorModel::COLOR_GRAY_ALPHA ||
                  color_model == ColorModel::COLOR_RGBA)),
          m_internal_info(internal_info),
          m_internal_info_image_type(internal_info_image_type)
    {}

    // TODO: We cannot just copy the internal_info pointer
    ImageInfo::ImageInfo(const ImageInfo& other)
    {
        *this = other;
    }

    ImageInfo& ImageInfo::operator=(const ImageInfo& other)
    {
        if (this == &other)
            return *this;
        m_color_model = other.m_color_model;
        m_image_type = other.m_image_type;
        m_data = other.m_data;
        destroyInternalInfo();
        m_internal_info = other.cloneInternalInfo();
        m_internal_info_image_type = other.m_internal_info_image_type;
        return *this;
    }

    // TODO: We cannot just copy the internal_info pointer
    ImageInfo::ImageInfo(ImageInfo&& other)
    {
        *this = std::move(other);
    }

    ImageInfo& ImageInfo::operator=(ImageInfo&& other)
    {
        if (this == &other)
            return *this;
        m_color_model = other.m_color_model;
        m_image_type = other.m_image_type;
        m_data = std::move(other.m_data);

        auto temp = other.m_internal_info;
        other.m_internal_info = m_internal_info;
        m_internal_info = temp;

        auto ii_image_type = m_internal_info;
        m_internal_info = other.m_internal_info;
        other.m_internal_info = ii_image_type;
        return *this;
    }

    void ImageInfo::printImageInfo()
    {
        std ::cerr << "Width:\t" << m_data.width() << "\nHeight:\t"
                   << m_data.height() << "\nNum Components:\t"
                   << m_data.numComponents() << "\nColor space:\t"
                   << static_cast<int>(m_color_model) << "\n";
    }
    ImageInfo::~ImageInfo() { destroyInternalInfo(); }

    void ImageInfo::resetInternalInfo()
    {
        m_internal_info = nullptr;
        m_internal_info_image_type = "";
    }

    void ImageInfo::destroyInternalInfo()
    {
        for (auto& handler : registered_image_handlers)
        {
            if (m_internal_info_image_type == handler->imageType())
            {
                handler->destroyInternalInfo(this);
                resetInternalInfo();
                return;
            }
        }
    }

    uint32_t ImageInfo::width() const { return m_data.width(); }
    uint32_t ImageInfo::height() const { return m_data.height(); }
    uint32_t ImageInfo::numComponents() const { return m_data.numComponents(); }
    uint32_t ImageInfo::sampleDepth() const { return m_data.sampleDepth(); }
    ColorModel ImageInfo::colorModel() const { return m_color_model; }
    uint64_t ImageInfo::size() const { return width() * height(); }

    void* ImageInfo::internalInfo() const { return m_internal_info; }
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

    PPMHandler ppm_handler;

    CIL::ImageInfo readImage(const char* filename)
    {
        // TODO: Check if file exists or not in the first place, 
        // this will allow us to give better errors.
        for (auto& handler : registered_image_handlers)
        {
            if (handler->isSupportedFile(filename))
            {
                return handler->read(filename);
            }
        }
        return ImageInfo();
    }

    CIL::ImageInfo readImage(const std::string& filename)
    {
        return readImage(filename.c_str());
    }

    bool ImageInfo::save(const std::string& filename) const
    {
        for (auto& handler : registered_image_handlers)
        {
            if (m_image_type == handler->imageType())
                return handler->write(this, filename.c_str());
        }
        return false;
    }
    void* ImageInfo::cloneInternalInfo() const
    {
        for (auto& handler : registered_image_handlers)
        {
            if (m_internal_info_image_type == handler->imageType())
            {
                return handler->cloneInternalInfo(this->m_internal_info);
            }
        }
        return nullptr;
    }

    bool ImageInfo::empty() const { return m_data.empty(); }

    bool ImageInfo::hasAlphaComponent() const
    {
        return m_color_model == ColorModel::COLOR_GRAY_ALPHA ||
               m_color_model == ColorModel::COLOR_RGBA;
    }

    std::string ImageInfo::imageType() const { return m_image_type; }

    bool equal(const ImageInfo& img1, const ImageInfo& img2)
    {
        if (img1.width() != img2.width() || img1.height() != img2.height() ||
            img1.numComponents() != img1.numComponents() ||
            img1.sampleDepth() != img2.sampleDepth() ||
            img1.colorModel() != img2.colorModel() ||
            img1.imageType() != img2.imageType())
        {
            return false;
        }
        double allowed_error_percentage = 0;
        if (img1.imageType() == "JPEG")
            allowed_error_percentage = 1;
        double err_percentage = computeErrorPercentage(img1.getData(),
                                                       img2.getData());
        show(err_percentage);
        return err_percentage <= allowed_error_percentage;
    }

    bool equal(const ImageInfo& img1, const std::string& image2_path)
    {
        auto img2 = readImage(image2_path);
        return equal(img1, img2);
    }
} // namespace CIL
