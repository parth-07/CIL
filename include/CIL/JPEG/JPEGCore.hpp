#include <CIL/ImageInfo.hpp>
#include <iostream>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    namespace JPEG {
        typedef J_COLOR_SPACE JColorType;

        class ImageInfo
        {
            uint32_t width;
            uint32_t height;
            int num_components;
            JColorType color_model;
            uint8_t* data;
            const JPEG::ImageInfo* old_img_info;

            friend class JPEGHandler;

          public:
            CIL::ImageInfo toCILImage();
            static const JPEG::ImageInfo*
            fromCILImage(const CIL::ImageInfo* cil_imageinfo);
        };
    } // namespace JPEG
} // namespace CIL
