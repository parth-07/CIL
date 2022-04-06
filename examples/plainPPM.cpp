#include <CIL/ImageHandler.hpp>
#include <CIL/ImageInfo.hpp>
#include <CIL/ImageMatrix.hpp>
#include <fstream>
#include <string>

class PlainPPMHandler : public CIL::ImageHandler
{
    const std::string m_image_type = "PLAIN_PPM";

  public:
    CIL::ImageInfo read(const char* filename) override;
    bool write(const CIL::ImageInfo* CIL, const char* filename) override;
    bool isSupportedFile(const char* filename) override;
    std::string imageType();
    static const std::string magic_bytes;
};

bool PlainPPMHandler::isSupportedFile(const char* filename)
{
    std::ifstream f(filename, std::ios::in);
    std::string s;
    f >> s;
    return s == magic_bytes;
}

CIL::ImageInfo PlainPPMHandler::read(const char* filename)
{
    if (!isSupportedFile(filename))
        return CIL::ImageInfo();
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open())
        return CIL::ImageInfo();
    uint32_t height, width, max_value;
    std::string magic_bytes;
    f >> magic_bytes >> width >> height >> max_value;
    assert(max_value < 266);
    CIL::ImageMatrix data(width, height, 3, 8);
    for (std::size_t i = 0; i < height; ++i)
    {
        for (std::size_t j = 0; j < width; ++j)
        {
            for (std::size_t k = 0; k < 3; ++k)
                f >> data(i, j, k);
        }
    }
    f.close();
    return CIL::ImageInfo(CIL::ColorModel::COLOR_RGB, m_image_type,
                          std::move(data));
}

bool PlainPPMHandler::write(const CIL::ImageInfo* img, const char* filename)
{
    std::ofstream f(filename, std::ios::out);
    if (!f.is_open())
        return false;
    f << magic_bytes.c_str() << "\n";
    f << img->width() << ' ' << img->height() << "\n";
    f << 255 << "\n";
    for (std::size_t i = 0; i < img->height(); ++i)
    {
        for (std::size_t j = 0; j < img->width(); ++j)
        {
            for (std::size_t k = 0; k < 3; ++k)
            {
                f << " ";
                f << std::to_string((*img)(i, j, k));
            }
        }
        f << "\n";
    }
    f.close();
    return true;
}

std::string PlainPPMHandler::imageType() { return m_image_type; }
const std::string PlainPPMHandler::magic_bytes = "P3";

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.png> <out.png>";
        return 1;
    }
    std::unique_ptr<CIL::ImageHandler> plain_ppm_handler(new PlainPPMHandler());
    CIL::AddImageHandler(std::move(plain_ppm_handler));
    // CIL::AddImageHandler(new PlainPPMHandler());
    CIL::ImageInfo image_info = CIL::readImage(argv[1]);
    std::cout<<"Converting image to Plain PPM format\n";
    image_info.setImageType("PLAIN_PPM");
    image_info.save(argv[2]);
    return 0;
}
