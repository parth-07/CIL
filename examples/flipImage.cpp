#include <CIL/ImageInfo.hpp>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.png> <out.png>";
        return 1;
    }
    CIL::ImageInfo image_info = CIL::readImage(argv[1]);
    for (uint32_t rows = 0; rows < image_info.height(); rows++)
    {
        for (uint32_t cols = 0; cols < image_info.width() / 2; cols++)
        {
            auto px1 = image_info(rows, cols);
            auto px2 = image_info(rows, image_info.width() - cols);
            CIL::Pixel::swap(px1, px2);
        }
    }
    image_info.save(argv[2]);
    return 0;
}
