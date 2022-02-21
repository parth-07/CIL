#ifndef CIL_DRAWING_HPP
#define CIL_DRAWING_HPP
#include <cstdint>
#include <limits>
#include <vector>
namespace CIL {
    class Pixel;
    class ColorMap
    {
      public:
        using ValueType = uint8_t;

      private:
        std::vector<ValueType> m_components;

      public:
        ColorMap(const std::initializer_list<ValueType>& components)
            : m_components(components)
        {}
        ColorMap(std::initializer_list<ValueType>&& components) noexcept
            : m_components(std::move(components))
        {}
        ColorMap(const ColorMap& other) : m_components(other.m_components) {}
        ValueType& operator[](int idx) { return m_components[idx]; }
        const ValueType& operator[](int idx) const { return m_components[idx]; }
        std::size_t numComponents() const { return m_components.size(); }
    };

    namespace Color {
        const ColorMap::ValueType
            max = std::numeric_limits<ColorMap::ValueType>::max();
        const ColorMap RED = {max, 0, 0};
        const ColorMap GREEN = {0, max, 0};
        const ColorMap BLUE = {0, 0, max};
        const ColorMap BLACK = {0, 0, 0};
        const ColorMap WHITE = {max, max, max};
    } // namespace Color
    void drawPoint(Pixel px, ColorMap color);
} // namespace CIL
#endif
