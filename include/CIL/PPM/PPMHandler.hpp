#ifndef CIL_PPM_HANDLER_HPP
#define CIL_PPM_HANDLER_HPP

#include <CIL/ImageInfo.hpp>
#include <CIL/ImageHandler.hpp>
#include <string>

namespace CIL {

    class PPMHandler : public ImageHandler
    {
      const std::string m_image_type = "PPM";
      public:
        ImageInfo read(const char* filename) override;
        bool write(const CIL::ImageInfo* CIL, const char* filename) override;
        bool isSupportedFile(const char* filename) override;
        std::string imageType();
        static const std::string magic_bytes;
    };

} // namespace CIL
#endif