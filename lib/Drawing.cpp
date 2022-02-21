#include <CIL/Drawing.hpp>
#include <CIL/ImageInfo.hpp>

namespace CIL {
    void drawPoint(Pixel px, const ColorMap color) { px.assign(color); }
} // namespace CIL
