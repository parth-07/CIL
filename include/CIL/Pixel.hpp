#ifndef CIL_PIXEL_HPP
#define CIL_PIXEL_HPP
#include <CIL/Core/Debug.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <stdint.h>

namespace CIL {
    class ImageMatrix;
    class DetachedFPPixel;
    class Pixel;
    class ColorMap;

    struct Dimensions
    {
        uint32_t top, bottom, left, right;
        Dimensions(uint32_t pad_all = 0)
        {
            top = bottom = left = right = pad_all;
        }
        Dimensions(const Dimensions& other) { *this = other; }
        Dimensions& operator=(const Dimensions& other)
        {
            top = other.top;
            bottom = other.bottom;
            right = other.right;
            left = other.left;
            return *this;
        }
        Dimensions(ImageMatrix* img_info, std::pair<uint32_t, uint32_t> start,
                   uint32_t width, uint32_t height);
        void showBounds()
        {
            fprintf(stderr, "top: %u, bottom: %u, right: %u, left: %u\n", top,
                    bottom, right, left);
        }
    };

    class Pixel
    {
        uint32_t m_row, m_col;
        ImageMatrix* m_image_matrix;
        Dimensions m_bounds;

      public:
        using ValueType = uint8_t;

        Pixel() : m_row(0), m_col(0), m_image_matrix(nullptr), m_bounds(0) {}
        Pixel(ImageMatrix* image_matrix, uint32_t row, uint32_t col,
              Dimensions dims = 0)
            : m_row(row), m_col(col), m_image_matrix(image_matrix),
              m_bounds(dims)
        {}
        Pixel(ImageMatrix& image_matrix, uint32_t row, uint32_t col,
              Dimensions dims = 0)
            : m_row(row), m_col(col), m_image_matrix(&image_matrix),
              m_bounds(dims)
        {}

        uint32_t row() const { return m_row; }
        uint32_t col() const { return m_col; }
        uint8_t numComponents() const;
        bool isValid() const;
        bool hasAlpha() const;

        Pixel& operator=(const DetachedFPPixel& dpx);
        void assign(const ColorMap& color);

        // TODO: improve design
        Dimensions getBounds() const { return m_bounds; }
        void setBounds(Dimensions dims) { m_bounds = dims; }
        void setBounds(std::pair<uint32_t, uint32_t> origin, uint32_t length,
                       uint32_t breadth)
        {
            m_bounds = Dimensions(m_image_matrix, origin, length, breadth);
        }
        void init();

        void copyComponents(const Pixel& p2)
        {
            assert(numComponents() == p2.numComponents());
            for (auto i = 0; i < numComponents(); i++)
                (*this)[i] = p2[i];
        }

        const uint8_t& operator[](int comp) const;
        uint8_t& operator[](int comp);

        bool operator==(Pixel& p2);
        bool operator!=(Pixel& p2);
        Pixel operator++(int);
        Pixel& operator++();
        Pixel operator--(int);
        Pixel& operator--();
        ValueType& back();
        void static swap(CIL::Pixel& p1, CIL::Pixel& p2);

        // debug
        void showPixel()
        {
            std::cerr << "row: " << row() << " col: " << col() << "\n";
            for (auto i = 0; i < numComponents(); i++)
                show(i, static_cast<int>((*this)[i]));
        }
    };
} // namespace CIL

#endif
