#ifndef JPEG_HPP
#define JPEG_HPP

#include <CIL/ImageInfo.hpp>
#include <CIL/ImageHandler.hpp>
#include <iostream>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>


namespace CIL {
    namespace JPEG {
        class JPEGHandler : public ImageHandler
        {
          public:
            CIL::ImageInfo read(const char* file_name) override;
            bool write(const CIL::ImageInfo* image_info,
                              const char* file_name) override;
            bool isSupportedFile(const char* file_name) override;
            std::string imageType() override;
            void destroyInternalInfo(const CIL::ImageInfo* image_info) override;
            void* cloneInternalInfo(void* internal_img_info) override;
            static bool isJPEGFile(FILE* fp);

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
