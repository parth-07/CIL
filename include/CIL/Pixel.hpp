#ifndef CIL_PIXEL_HPP
#define CIL_PIXEL_HPP
#include <stdint.h>

namespace CIL {
    class ImageMatrix;
    class Pixel
    {
        uint32_t m_row, m_col;
        ImageMatrix& m_image_matrix;

      public:
        int numComponents() const;
        const uint8_t& operator[](int comp) const;
        uint8_t& operator[](int comp);
        bool empty() const;
        Pixel(ImageMatrix* image_matrix, uint32_t row, uint32_t col)
            : m_row(row), m_col(col), m_image_matrix(*image_matrix)
        {}
        Pixel(ImageMatrix& image_matrix, uint32_t row, uint32_t col)
            : m_row(row), m_col(col), m_image_matrix(image_matrix)
        {}
        Pixel operator++(int);
        Pixel& operator++();
        void static swap(CIL::Pixel& p1, CIL::Pixel& p2);
    };
} // namespace CIL

#endif
