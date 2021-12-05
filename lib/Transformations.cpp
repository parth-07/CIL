#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <limits>

namespace CIL {
    void invertColor(ImageInfo& img)
    {
        for (auto px : img)
        {
            for (int i = 0; i < px.numComponents(); i++)
                px[i] = std::numeric_limits<uint8_t>::max() - px[i];
        }
    }
} // namespace CIL
