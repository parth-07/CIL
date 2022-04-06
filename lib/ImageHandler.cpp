#include <CIL/ImageHandler.hpp>

#include <CIL/ImageInfo.hpp>

#include <cassert>

namespace CIL {
    void ImageHandler::destroyInternalInfo(const CIL::ImageInfo* cil_img_info)
    {
        assert(cil_img_info->internalInfo() == nullptr);
        return;
    }
    // TODO: Should we use `const void*` here?
    void* ImageHandler::cloneInternalInfo(void* internal_img_info)
    {
        assert(internal_img_info == nullptr);
        return nullptr;
    }
} // namespace CIL