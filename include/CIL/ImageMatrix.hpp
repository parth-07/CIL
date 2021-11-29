#ifndef CIL_IMAGE_MATRIX
#define CIL_IMAGE_MATRIX
#include <memory>

namespace CIL {
    class Pixel;
    class ImageMatrix
    {
      public:
        ImageMatrix() {}
        ImageMatrix(uint32_t width, uint32_t height, int num_componenets,
                    uint8_t sample_depth, std::unique_ptr<uint8_t[]> data);
        ImageMatrix(const ImageMatrix&);
        ImageMatrix& operator=(const ImageMatrix&);
        ImageMatrix(ImageMatrix&& img);
        ImageMatrix& operator=(ImageMatrix&& img);

        Pixel operator()(uint32_t row, uint32_t col);
        uint8_t& operator()(uint32_t row, uint32_t col, int comp);
        const uint8_t& operator()(uint32_t row, uint32_t col, int comp) const;
        bool empty() const;

      private:
        uint32_t m_width, m_height;
        int m_num_components;
        uint8_t m_sample_depth;
        std::unique_ptr<uint8_t[]> m_data;

      public:
        uint32_t rowbytes() const;
        uint32_t width() const;
        uint32_t height() const;
        int numComponents() const;
        uint32_t sampleDepth() const;
    };
} // namespace CIL

#endif
