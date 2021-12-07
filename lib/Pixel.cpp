#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/ImageMatrix.hpp>
#include <CIL/Pixel.hpp>
#include <cassert>

namespace CIL {
    uint8_t& Pixel::operator[](int comp)
    {
        return m_image_matrix(m_row, m_col, comp);
    }

    const uint8_t& Pixel::operator[](int comp) const
    {
        return m_image_matrix(m_row, m_col, comp);
    }

    int Pixel::numComponents() const { return m_image_matrix.numComponents(); }

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
                &m_image_matrix == &p2.m_image_matrix);
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
        if (m_col + 1 >= m_image_matrix.width())
        {
            m_row++;
            m_col = 0;
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
        if (m_col - 1 >= m_image_matrix.width())
        {
            m_row--;
            m_col = m_image_matrix.width();
        } else
            m_col--;
        return *this;
    }

    bool Pixel::empty() const
    {
        if (m_row >= m_image_matrix.height() || m_col >= m_image_matrix.width())
            return true;
        return false;
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
