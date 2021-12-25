#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in> <out>";
        return 1;
    }
    CIL::ImageInfo image_info = CIL::readImage(argv[1]);
    changeBrightness(image_info, 100);
    image_info.save(argv[2]);
    return 0;
}
