#ifndef CIL_IMAGE_MATRIX
#define CIL_IMAGE_MATRIX
#include <CIL/Pixel.hpp>
#include <memory>

namespace CIL {
    class ImageMatrix
    {
      public:
        class iterator
            : public std::iterator<
                  std::bidirectional_iterator_tag, // iterator_category
                  Pixel,                           // value_type
                  long,                            // difference_type
                  Pixel*,                          // pointer
                  Pixel                            // reference
                  >
        {
            Pixel m_px;

          public:
            iterator(Pixel px) : m_px(px) {}
            iterator& operator--()
            {
                --m_px;
                return *this;
            }
            iterator operator--(int)
            {
                iterator retval = *this;
                --(*this);
                return retval;
            }
            iterator& operator++()
            {
                ++m_px;
                return *this;
            }
            iterator operator++(int)
            {
                iterator retval = *this;
                ++(*this);
                return retval;
            }
            bool operator==(iterator other) { return m_px == other.m_px; }
            bool operator!=(iterator other) { return !(*this == other); }
            reference operator*() { return m_px; }
            pointer operator->() { return &m_px; }
        };

        ImageMatrix() {}
        ImageMatrix(uint32_t width, uint32_t height, int num_componenets,
                    uint8_t sample_depth,
                    std::unique_ptr<uint8_t[]> data = nullptr);
        ImageMatrix(const ImageMatrix&);
        ImageMatrix& operator=(const ImageMatrix&);
        ImageMatrix(ImageMatrix&& img);
        ImageMatrix& operator=(ImageMatrix&& img);
        iterator begin() { return iterator((*this)(0, 0)); }
        iterator end() { return iterator((*this)(m_height, 0)); }
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
