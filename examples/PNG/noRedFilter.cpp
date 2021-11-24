#include <CIL/ImageInfo.hpp>
#include <CIL/PNG/PNGHandler.hpp>

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        auto img_info = CIL::PNG::readPNGFile(argv[1]);
        for (int i = 0U; i < img_info->height; ++i)
        {
            auto row = img_info->data +
                       i * img_info->width * img_info->num_components;
            for (int j = 0; j < img_info->width; ++j)
            {
                auto px = row + j * img_info->num_components;
                px[0] = 0;
            }
        }
        if (CIL::PNG::writePNGFile(img_info, argv[2]))
        {
            std::cout << "Image written successfully\n";
        } else
        {
            std::cout << "Failed to write the image\n";
        }
        CIL::ImageInfo::destroy(img_info);
    }
}