#include <CIL/ImageInfo.hpp>
#include <iostream>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    namespace JPEG {
        class ImageInfo
        {
            uint32_t width;
            uint32_t height;
            int num_components;
            J_COLOR_SPACE color_model;
            uint8_t* data;
            const JPEG::ImageInfo* old_img_info;

            friend class JpegHandler;

          public:
            CIL::ImageInfo* toCILImage();
            void fromCILImage(const CIL::ImageInfo* cil_imageinfo);
        };
    } // namespace JPEG
} // namespace CIL
