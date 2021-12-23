#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <iostream>
int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <image1> <image2>";
        return 1;
    }
    CIL::ImageInfo image1 = CIL::readImage(argv[1]);
    CIL::ImageInfo image2 = CIL::readImage(argv[2]);
    if (CIL::equal(image1, image2)) {
      std::cout<<"Images are equal\n";
    } else {
      std::cout<<"Images are not equal\n";
    }
    return 0;
}
