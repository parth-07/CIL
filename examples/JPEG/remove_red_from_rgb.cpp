#include <CIL/ImageInfo.hpp>
#include <CIL/JPEG/JPEGHandler.hpp>

using CIL::JPEG::JPEGHandler;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.jpg> <out.jpg>";
        return 1;
    }
    auto img_info = CIL::readImage(argv[1]);
    for (auto px : img_info)
    {
        px[0] = 0; // set R component zero
    }
    img_info.save(argv[2]);
    return 0;
}
