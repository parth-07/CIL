#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <iostream>
int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.png> <out.png>";
        return 1;
    }
    CIL::ImageInfo image_info = CIL::readImage(argv[1]);
    image_info.printImageInfo();
    uint32_t new_height=0, new_width=0;
    std::cout<<"Enter new width: ";
    std::cin>>new_width;
    std::cout<<"Enter new height: ";
    std::cin>>new_height;
    CIL::resize(image_info, new_width, new_height);
    image_info.save(argv[2]);
    return 0;
}
