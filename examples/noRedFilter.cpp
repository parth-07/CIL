#include <CIL/ImageInfo.hpp>
#include <CIL/PNG/PNGHandler.hpp>

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        auto img_info = CIL::readImage(argv[1]);
        for (auto i = 0U; i < img_info.height(); ++i)
        {
            for (auto j = 0U; j < img_info.width(); ++j)
            {
                auto px = img_info(i, j);
                px[1] = 0;
            }
        }
        if (img_info.save(argv[2]))
        {
            std::cout << "Image written successfully\n";
        } else
        {
            std::cout << "Failed to write the image\n";
        }
    }
}
