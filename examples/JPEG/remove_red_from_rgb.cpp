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
    for (uint32_t rows = 0; rows < image_info->height; rows++)
    {
        for (uint32_t cols = 0;
             cols < image_info->width * image_info->num_components;
             cols += image_info->num_components)
        {
            *(image_info->data +
              (rows * image_info->width * image_info->num_components) +
              cols) = 0; // set R component zero
        }
    }
    JpegHandler::write(image_info, argv[2]);
    CIL::ImageInfo::destroy(image_info);
    return 0;
}
