#include <CIL/JPEG/JPEGHandler.hpp>

using CIL::JPEG::JpegHandler;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.jpg> <out.jpg>";
        return 1;
    }

    CIL::ImageInfo* image_info = JpegHandler::read(argv[1]);
    image_info->color_model = CIL::ColorModel::COLOR_GRAY;
    JpegHandler::write(image_info, argv[2]);
    CIL::ImageInfo::destroy(image_info);

    return 0;
}
