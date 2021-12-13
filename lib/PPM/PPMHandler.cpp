#include "CIL/PPM/PPMHandler.hpp"
#include "CIL/Core/Debug.hpp"
#include "CIL/ImageMatrix.hpp"
#include <fstream>

namespace CIL {
    bool PPMHandler::isValidFile(const std::string& filename)
    {
        std::ifstream f(filename, std::ios::in | std::ios::binary);
        std::string s;
        f >> s;
        return s == magic_bytes;
    }

    ImageInfo PPMHandler::read(const std::string& filename)
    {
        if (!isValidFile(filename))
            return ImageInfo();
        std::ifstream f(filename, std::ios::in | std::ios::binary);
        if (!f.is_open())
            return ImageInfo();
        uint32_t height, width, max_value;
        std::string magic_bytes;
        f >> magic_bytes >> width >> height >> max_value;
        assert(max_value < 266);
        ImageMatrix data(width, height, 3, 8);
        f.seekg(1, std::ios::cur);
        f.read(reinterpret_cast<char*>(&data(0, 0, 0)),
               sizeof(uint8_t) * width * height * 3);
        f.close();
        return CIL::ImageInfo(ColorModel::COLOR_RGB, ImageType::PPM,
                              std::move(data));
    }

    bool PPMHandler::write(const ImageInfo& img, const std::string& filename)
    {
        std::ofstream f(filename, std::ios::out | std::ios::binary);
        if (!f.is_open())
            return false;
        f << magic_bytes.c_str() << "\n";
        f << img.width() << "\n";
        f << img.height() << "\n";
        f << 255 << "\n";
        f.write(reinterpret_cast<const char*>(&img(0, 0, 0)),
                sizeof(uint8_t) * img.width() * img.height() *
                    img.numComponents());
        f.close();
        return true;
    }

    const std::string PPMHandler::magic_bytes = "P6";
} // namespace CIL