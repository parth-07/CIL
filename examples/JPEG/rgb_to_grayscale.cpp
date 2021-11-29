#include <CIL/JPEG/JPEGHandler.hpp>

using CIL::JPEG::JPEGHandler;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.jpg> <out.jpg>";
        return 1;
    }

    auto image_info = CIL::readImage(argv[1]);
    image_info.setColorModel(CIL::ColorModel::COLOR_GRAY);
    image_info.save(argv[2]);
    return 0;
}
