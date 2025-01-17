#ifndef JPEG_HPP
#define JPEG_HPP

#include <CIL/ImageInfo.hpp>
#include <iostream>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    namespace JPEG {
        class JPEGHandler
        {
          public:
            static CIL::ImageInfo read(const char* file_name);
            static bool write(const CIL::ImageInfo* image_info,
                              const char* file_name);
            static void destroy(CIL::ImageInfo* image_info);
            static bool isJPEGFile(FILE* fp);
            static bool isJPEGFile(const char* file_name);
            static void* clone(void* internal_img_info);

          private:
            struct ErrorManager
            {
                jpeg_error_mgr default_error_manager;
                jmp_buf jump_buffer;
            };

            static void errorExit(j_common_ptr cinfo);
            static void outputMessage(j_common_ptr cinfo);
        };
    } // namespace JPEG
} // namespace CIL

#endif
