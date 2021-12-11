#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/ImageMatrix.hpp>
#include <CIL/Pixel.hpp>
#include <cassert>
#include <iostream>
#include <limits>

namespace CIL {
    Dimensions::Dimensions(ImageMatrix* img_info,
                           std::pair<uint32_t, uint32_t> start, uint32_t width,
                           uint32_t height)
    {
        top = start.first;
        left = start.second;
        right = ((left + width) > img_info->width())
                    ? img_info->width()
                    : img_info->width() - (left + width);
        bottom = ((top + height) > img_info->height())
                     ? img_info->height()
                     : img_info->height() - (top + height);
    }

    uint8_t& Pixel::operator[](int comp)
    {
        assert(isValid() && "pixel is empty");
        return (*m_image_matrix)(m_row, m_col, comp);
    }

    const uint8_t& Pixel::operator[](int comp) const
    {
        assert(isValid() && "pixel is empty");
        return (*m_image_matrix)(m_row, m_col, comp);
    }

    int Pixel::numComponents() const { return m_image_matrix->numComponents(); }

    void Pixel::swap(Pixel& p1, Pixel& p2)
    {
        assert((p1.numComponents() == p2.numComponents()) &&
               "Number of components in pixels aren't equal");
        for (auto i = 0; i < p1.numComponents(); i++)
            std::swap(p1[i], p2[i]);
        std::swap(p1.m_col, p2.m_col);
        std::swap(p1.m_row, p2.m_row);
    }

    bool Pixel::operator==(Pixel& p2)
    {
        return (m_row == p2.m_row && m_col == p2.m_col &&
                m_image_matrix == p2.m_image_matrix);
    }

    bool Pixel::operator!=(Pixel& p2) { return !(*this == p2); }

    // post increment operator
    Pixel Pixel::operator++(int)
    {
        Pixel temp = *this;
        ++(*this);
        return temp;
    }

    // pre increment operator
    Pixel& Pixel::operator++()
    {
        if (m_col + 1 >= m_image_matrix->width() - m_bounds.right)
        {
            m_row++;
            m_col = m_bounds.left;
        } else
            m_col++;
        return *this;
    }

    Pixel Pixel::operator--(int)
    {
        Pixel temp = *this;
        --(*this);
        return temp;
    }

    Pixel& Pixel::operator--()
    {
        // remember: unsigned comparision
        if (m_col - 1 < m_bounds.left ||
            m_col - 1 >= m_image_matrix->width() - m_bounds.right)
        {
            m_row--;
            m_col = m_image_matrix->width() - m_bounds.right;
        } else
            m_col--;
        return *this;
    }

    bool Pixel::isValid() const
    {
        if (m_row + m_bounds.bottom >= m_image_matrix->height() ||
            m_col + m_bounds.right >= m_image_matrix->width() ||
            m_row < m_bounds.top || m_col < m_bounds.left)
            return false;
        return true;
    }

    Pixel& Pixel::operator=(const DetachedFPPixel& dpx)
    {
        assert(numComponents() == dpx.numComponents());
        for (int i = 0; i < numComponents(); ++i)
        {
            (*this)[i] = dpx[i];
        }
        return *this;
    }
} // namespace CIL
