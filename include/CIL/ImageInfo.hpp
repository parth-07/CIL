#ifndef CIL_IMAGE_INFO_H
#define CIL_IMAGE_INFO_H
#include <CIL/ImageMatrix.hpp>
#include <CIL/Pixel.hpp>

#include <iostream>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h> // when build by autotools, this will be included
#endif

namespace CIL {
    class ImageHandler;
    enum class ColorModel
    {
        COLOR_RGB,
        COLOR_RGBA,
        COLOR_CMYK,
        COLOR_GRAY,
        COLOR_GRAY_ALPHA,
        COLOR_PALETTE
    };

    // TODO: Make the design more extendible
    class ImageInfo
    {
        ColorModel m_color_model;
        std::string m_image_type;
        ImageMatrix m_data;
        void* m_internal_info = nullptr;
        std::string m_internal_info_image_type = "";

      public:
        ImageInfo() {}
        ImageInfo(uint32_t width, uint32_t height, uint32_t num_components,
                  uint32_t sample_depth, ColorModel color_model,
                  const std::string& image_type,
                  std::unique_ptr<uint8_t[]> data,
                  void* internal_info = nullptr,
                  const std::string& internal_info_image_type = "");

        ImageInfo(ColorModel color_model, const std::string& image_type,
                  const ImageMatrix& data)
            : m_color_model(color_model), m_image_type(image_type), m_data(data)
        {}

        ImageInfo(ColorModel color_model, const std::string& image_type,
                  ImageMatrix&& data)
            : m_color_model(color_model), m_image_type(image_type),
              m_data(std::move(data))
        {}
        ~ImageInfo();

        ImageInfo(const ImageInfo&);
        ImageInfo& operator=(const ImageInfo&);
        ImageInfo(ImageInfo&&);
        ImageInfo& operator=(ImageInfo&&);

        uint32_t width() const;
        uint32_t height() const;
        uint32_t numComponents() const;
        uint32_t sampleDepth() const;
        uint64_t size() const;
        uint32_t rowbytes() const { return m_data.rowbytes(); }
        ImageMatrix getData() const { return m_data; }
        void* internalInfo() const;
        std::string internalInfoImageType() const
        {
            return m_internal_info_image_type;
        }
        ColorModel colorModel() const;
        std::string imageType() const;
        bool empty() const;

        void setColorModel(ColorModel color_model);
        void setImageType(std::string image_type) { m_image_type = image_type; }
        void setData(const ImageMatrix& img_matrix) { m_data = img_matrix; }
        void setData(ImageMatrix&& img_matrix)
        {
            m_data = std::move(img_matrix);
        }

        ImageMatrix::iterator begin() { return m_data.begin(); }
        ImageMatrix::iterator end() { return m_data.end(); }

        Pixel operator()(uint32_t row, uint32_t col);
        uint8_t& operator()(uint32_t row, uint32_t col, int comp);
        const uint8_t& operator()(uint32_t row, uint32_t col, int comp) const;

        void printImageInfo();
        bool save(const std::string& filename) const;
        bool hasAlphaComponent() const;

      private:
        void* cloneInternalInfo() const;
        void destroyInternalInfo();
        void resetInternalInfo();
    };

    CIL::ImageInfo readImage(const char* filename);
    CIL::ImageInfo readImage(const std::string& filename);

    bool equal(const ImageInfo& img1, const ImageInfo& img2);
    bool equal(const ImageInfo& img1, const std::string& image2_path);

    void AddImageHandler(std::unique_ptr<ImageHandler> handler);
} // namespace CIL
#endif
