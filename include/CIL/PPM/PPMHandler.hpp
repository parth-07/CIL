#ifndef CIL_PPM_HANDLER_HPP
#define CIL_PPM_HANDLER_HPP

#include <CIL/ImageInfo.hpp>
#include <string>

namespace CIL {

    class PPMHandler
    {
      public:
        ImageInfo read(const std::string& filename);
        bool write(const CIL::ImageInfo& CIL, const std::string& filename);
        bool isValidFile(const std::string& filename);
        static const std::string magic_bytes;
    };

} // namespace CIL
#endif