#ifndef CIL_PNGHANDLER_HPP
#define CIL_PNGHANDLER_HPP
#include <CIL/ImageHandler.hpp>
#include <iostream>
#include <setjmp.h>
namespace CIL {
    class ImageInfo;
    namespace PNG {
        class PNGHandler : public ImageHandler
        {
          public:
            /// Reads a PNG file and returns `CIL::ImageInfo` object for it.
            ///
            /// If the image uses palette color model then it is automatically
            /// converted to RGB color model.
            CIL::ImageInfo readExact(const char* filename);

            /// Reads a PNG file and internally converts the image to RGB (or
            /// RGBA) 8 sample depth format and returns a `CIL::ImageInfo`
            /// object for it.
            CIL::ImageInfo read(const char* filename) override;

            /// Writes the image described by `cil_img_info` in the file
            /// `filename`.
            bool write(const CIL::ImageInfo* cil_img_info,
                              const char* filename) override;

            /// Returns true if the provided filename is a PNG file; otherwise
            /// returns false.
            bool isSupportedFile(const char* filename) override;

            std::string imageType() override;

            /// Frees the `internal_info` pointer of `CIL::ImageInfo` object.
            void destroyInternalInfo(const CIL::ImageInfo* cil_image_info) override;

            void* cloneInternalInfo(void* internal_img_info) override;

            /// Returns true if the provided file pointer points to the
            /// beginning of a PNG file.
            static bool isPNGFile(FILE* fp);

        };

    } // namespace PNG
} // namespace CIL
#endif