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
    auto image_info = CIL::readImage(argv[1]);
    for (auto px = image_info(0, 0); !px.empty(); ++px)
    {
        px[0] = 0; // set R component zero
    }
    image_info.save(argv[2]);
    return 0;
}
