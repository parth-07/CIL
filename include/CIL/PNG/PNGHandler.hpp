#ifndef CIL_PNGHANDLER_HPP
#define CIL_PNGHANDLER_HPP
#include <iostream>
#include <setjmp.h>

namespace CIL {
    class ImageInfo;
    namespace PNG {
        class ImageInfo;
        /// Reads a PNG file and returns `CIL::ImageInfo` object for it.
        ///
        /// If the image uses palette color model then it is automatically
        /// converted to RGB color model.
        CIL::ImageInfo* readPNGFile(const char* filename);

        /// Reads a PNG file and internally converts the image to RGB (or RGBA)
        /// 8 sample depth format and returns a `CIL::ImageInfo` object for it.
        CIL::ImageInfo* readPNGFileAs8RGB(const char* filename);

        /// Writes the iamge described by `cil_img_info` iln the file
        /// `filename`.
        bool writePNGFile(const CIL::ImageInfo* cil_img_info,
                          const char* filename);

        /// Returns true if the provided filename is a PNG file; otherwise
        /// returns false.
        bool isPNGFile(const char* filename);

        /// Returns true if the provided file pointer points to the beginning of
        /// a PNG file.
        bool isPNGFile(FILE* fp);

        /// Frees the `internal_info` pointer of `CIL::ImageInfo` object.
        void destroyPNGImageInfo(const CIL::ImageInfo* cil_image_info);
    } // namespace PNG
} // namespace CIL
#endif