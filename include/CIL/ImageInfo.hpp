#ifndef CIL_IMAGE_INFO_H
#define CIL_IMAGE_INFO_H
#include <iostream>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    enum class ColorModel
    {
        COLOR_RGB,
        COLOR_RGBA,
        COLOR_CMYK,
        COLOR_GRAY,
        COLOR_GRAY_ALPHA,
        COLOR_PALETTE
    };

    struct ImageInfo
    {
        uint32_t width;
        uint32_t height;
        int num_components;
        ColorModel color_model;
        uint8_t* data;
        uint8_t sample_depth;
        void print_image_info();
        const void* internal_info = nullptr;
    };

} // namespace CIL
#endif
