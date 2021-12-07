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

    enum class ColorModel
    {
        COLOR_RGB,
        COLOR_RGBA,
        COLOR_CMYK,
        COLOR_GRAY,
        COLOR_GRAY_ALPHA,
        COLOR_PALETTE
    };

    enum class ImageType
    {
        JPEG,
        PNG,
        PPM
    };

    // TODO: Make the design more extendible
    class ImageInfo
    {
        ColorModel m_color_model;
        ImageType m_image_type;
        ImageMatrix m_data;
        void* m_internal_info = nullptr;

      public:
        ImageInfo() {}
        ImageInfo(uint32_t width, uint32_t height, uint32_t num_components,
                  uint32_t sample_depth, ColorModel color_model,
                  ImageType image_type, std::unique_ptr<uint8_t[]> data,
                  void* internal_info = nullptr);
        ImageInfo(const ImageInfo&);
        ImageInfo& operator=(const ImageInfo&);
        ImageInfo(ImageInfo&&);
        ImageInfo& operator=(ImageInfo&&);
        ~ImageInfo();
        uint32_t width() const;
        uint32_t height() const;
        uint32_t numComponents() const;
        uint32_t sampleDepth() const;
        uint64_t size() const;
        void* internalInfo() const;
        ColorModel colorModel() const;
        void setColorModel(ColorModel color_model);
        bool save(const std::string& filename) const;
        ImageMatrix::iterator begin() { return m_data.begin(); }
        ImageMatrix::iterator end() { return m_data.end(); }
        Pixel operator()(uint32_t row, uint32_t col);
        uint8_t& operator()(uint32_t row, uint32_t col, int comp);
        const uint8_t& operator()(uint32_t row, uint32_t col, int comp) const;
        void printImageInfo();
        bool empty() const;
        void setData(const ImageMatrix& img_matrix) {
          m_data = img_matrix;
        }
        void setData(ImageMatrix&& img_matrix) {
          m_data = std::move(img_matrix);
        }
      private:
        void* cloneInternalInfo() const;
        void destroyInternalInfo();
    };

    CIL::ImageInfo readImage(const char* filename);
    CIL::ImageInfo readImage(const std::string& filename);
} // namespace CIL
#endif
