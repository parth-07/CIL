#ifndef CIL_PIXEL_HPP
#define CIL_PIXEL_HPP
#include <stdint.h>

namespace CIL {
    class ImageMatrix;
    class DetachedFPPixel;
    class Pixel
    {
        uint32_t m_row, m_col;
        ImageMatrix& m_image_matrix;

      public:
        int numComponents() const;
        const uint8_t& operator[](int comp) const;
        uint8_t& operator[](int comp);
        bool empty() const;
        uint32_t row() const { return m_row; }
        uint32_t col() const { return m_col; }
        Pixel(ImageMatrix* image_matrix, uint32_t row, uint32_t col)
            : m_row(row), m_col(col), m_image_matrix(*image_matrix)
        {}
        Pixel(ImageMatrix& image_matrix, uint32_t row, uint32_t col)
            : m_row(row), m_col(col), m_image_matrix(image_matrix)
        {}
        bool operator==(Pixel& p2);
        bool operator!=(Pixel& p2);
        Pixel operator++(int);
        Pixel& operator++();
        Pixel operator--(int);
        Pixel& operator--();
        void copyComponents(const Pixel& p2)
        {
            for (auto i = 0; i < numComponents(); i++)
                (*this)[i] = p2[i];
        }
        void static swap(CIL::Pixel& p1, CIL::Pixel& p2);
        Pixel& operator=(const DetachedFPPixel& dpx);
    };
} // namespace CIL

#endif
