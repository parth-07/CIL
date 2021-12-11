#include <CIL/Core/Debug.hpp>
#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/ImageMatrix.hpp>
#include <CIL/Pixel.hpp>
#include <cassert>
#include <cstring>
#include <iostream>

namespace CIL {
    ImageMatrix::ImageMatrix(uint32_t width, uint32_t height,
                             int num_componenets, uint8_t sample_depth,
                             std::unique_ptr<uint8_t[]> data)
        : m_width(width), m_height(height), m_num_components(num_componenets),
          m_sample_depth(sample_depth)
    {
        if (data != nullptr)
            m_data = std::move(data);
        else
            m_data.reset(new uint8_t[m_height * rowbytes()]());
    }

    ImageMatrix::ImageMatrix(const ImageMatrix& other) { *this = other; }

    ImageMatrix& ImageMatrix::operator=(const ImageMatrix& other)
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_num_components = other.m_num_components;
        m_sample_depth = other.m_sample_depth;
        m_data.reset(new uint8_t[other.m_height * other.rowbytes()]);
        std::memcpy(m_data.get(), other.m_data.get(),
                    other.m_height * other.rowbytes());
        return *this;
    }

    ImageMatrix::ImageMatrix(ImageMatrix&& other) { *this = other; }

    ImageMatrix& ImageMatrix::operator=(ImageMatrix&& rvalue)
    {
        if (this == &rvalue)
            return *this;
        m_width = rvalue.m_width;
        m_height = rvalue.m_height;
        m_num_components = rvalue.m_num_components;
        m_sample_depth = rvalue.m_sample_depth;
        m_data.reset(rvalue.m_data.release());
        return *this;
    }

    uint32_t ImageMatrix::width() const { return m_width; }
    uint32_t ImageMatrix::height() const { return m_height; }
    int ImageMatrix::numComponents() const { return m_num_components; }
    uint32_t ImageMatrix::sampleDepth() const { return m_sample_depth; }
    bool ImageMatrix::empty() const { return m_width == 0 || m_height == 0; }

    uint32_t ImageMatrix::rowbytes() const
    {
        uint32_t rowbytes = 0;
        rowbytes = m_width * m_num_components;
        if (m_sample_depth == 16)
            rowbytes *= 2;
        else
            rowbytes /= (8 / m_sample_depth);
        return rowbytes;
    }

    Pixel ImageMatrix::operator()(uint32_t row, uint32_t col)
    {
        return Pixel(*this, row, col);
    }

    // Currently, we only support reading and writing 8 bit depth images in this
    // way.
    uint8_t& ImageMatrix::operator()(uint32_t row, uint32_t col, int comp)
    {
        assert(comp < numComponents() &&
               "Component number is greater than number of components in "
               "image");
        return m_data[row * rowbytes() + col * m_num_components + comp];
    }

    const uint8_t& ImageMatrix::operator()(uint32_t row, uint32_t col,
                                           int comp) const
    {
        assert(comp < numComponents() &&
               "Component number is greater than number of components in "
               "image");
        return m_data[row * rowbytes() + col * m_num_components + comp];
    }

    void ImageMatrix::convolute(const Sequence<double>& v)
    {
        auto new_img = ImageMatrix(width() - v[0].size() + 1,
                                   height() - v.size() + 1, numComponents(),
                                   sampleDepth());
        for (auto px : *this)
        {
            px.setBounds(
                Dimensions(this, {0, 0}, new_img.width(), new_img.height()));
            if (!px.isValid())
                continue;

            auto boundedPx = px;
            boundedPx.setBounds(
                Dimensions(this, {px.row(), px.col()}, v[0].size(), v.size()));

            DetachedFPPixel dePx(new_img(px.row(), px.col()));

            for (auto i = 0U; i < v.size(); i++)
                for (auto j = 0U; j < v[i].size(); j++)
                {
                    for (int comp = 0; comp < px.numComponents(); comp++)
                    {
                        dePx[comp] += static_cast<double>(boundedPx[comp]) *
                                      v[i][j];
                    }
                    boundedPx++;
                }

            new_img(px.row(), px.col()) = dePx;
        }
        *this = new_img;
    }
} // namespace CIL
