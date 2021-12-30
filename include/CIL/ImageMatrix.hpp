#ifndef CIL_IMAGE_MATRIX
#define CIL_IMAGE_MATRIX
#include <CIL/Core/Types.hpp>
#include <CIL/Pixel.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace CIL {
    class ImageMatrix
    {
      private:
        uint32_t m_width, m_height;
        uint8_t m_num_components;
        uint8_t m_sample_depth;
        std::unique_ptr<uint8_t[]> m_data;
        bool m_has_alpha;

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
        using ValueType = uint8_t;
        ImageMatrix() {}
        ImageMatrix(uint32_t width, uint32_t height, uint8_t num_components,
                    uint8_t sample_depth,
                    std::unique_ptr<uint8_t[]> data = nullptr,
                    bool has_alpha = false);

        ImageMatrix(const ImageMatrix&);
        ImageMatrix& operator=(const ImageMatrix&);
        ImageMatrix(ImageMatrix&& img);
        ImageMatrix& operator=(ImageMatrix&& img);

        uint32_t width() const;
        uint32_t height() const;
        uint8_t numComponents() const;
        uint32_t sampleDepth() const;
        bool empty() const;
        uint32_t rowbytes() const;
        bool hasAlpha() const;
        iterator begin() { return (*this)(0, 0); }
        iterator end() { return (*this)(m_height, 0); }

        Pixel operator()(uint32_t row, uint32_t col);
        uint8_t& operator()(uint32_t row, uint32_t col, int comp);
        const uint8_t& operator()(uint32_t row, uint32_t col, int comp) const;

        void convolute(const Sequence<double>& v);
        void forEach(std::function<void(const typename ImageMatrix::ValueType&)>
                         fn) const;

        void
        forEach(std::function<void(const typename ImageMatrix::ValueType&,
                                   uint32_t row, uint32_t col, uint32_t comp)>
                    fn) const;
    };
    double computeErrorPercentage(const ImageMatrix& data1,
                                  const ImageMatrix& data2);

    ImageMatrix computeAbsDifference(const ImageMatrix& data1,
                                     const ImageMatrix& data2);
} // namespace CIL

#endif
