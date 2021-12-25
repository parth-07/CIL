#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/Pixel.hpp>
#include <algorithm>
#include <numeric>

namespace CIL {
    DetachedFPPixel::DetachedFPPixel(const Pixel& px)
        : m_components(px.numComponents(), 0)
    {
        for (auto i = 0U; i < m_components.size(); ++i)
        {
            m_components[i] = px[i];
        }
    }

    DetachedFPPixel operator*(double scale, const DetachedFPPixel& dpx)
    {
        DetachedFPPixel scaled_dpx(dpx.numComponents());
        for (auto i = 0U; i < dpx.numComponents(); ++i)
        {
            scaled_dpx[i] = scale * dpx[i];
        }
        return scaled_dpx;
    }

    DetachedFPPixel& operator+=(DetachedFPPixel& lhs,
                                const DetachedFPPixel& rhs)
    {
        assert(lhs.numComponents() == rhs.numComponents());
        for (auto i = 0U; i < lhs.numComponents(); ++i)
        {
            lhs[i] += rhs[i];
        }
        return lhs;
    }

    void DetachedFPPixel::scale(const std::vector<ValueType> multipliers)
    {
        for (auto i = 0U; i < std::min(multipliers.size(), m_components.size());
             ++i)
        {
            m_components[i] *= multipliers[i];
        }
    }

    DetachedFPPixel::ValueType DetachedFPPixel::sum() const
    {
        return std::accumulate(m_components.begin(), m_components.end(),
                               ValueType());
    }

    void DetachedFPPixel::capRange(double l, double r)
    {
        for (auto i = 0U; i < m_components.size(); ++i)
        {
            if (m_components[i] < l)
                m_components[i] = l;
            else if (m_components[i] > r)
                m_components[i] = r;
        }
    }

    DetachedFPPixel& operator+=(DetachedFPPixel& lhs,
                                typename DetachedFPPixel::ValueType val)
    {
        for (auto i = 0U; i < lhs.numComponents(); ++i)
        {
            lhs[i] += val;
        }
        return lhs;
    }
} // namespace CIL
