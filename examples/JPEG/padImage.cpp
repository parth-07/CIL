#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.jpg> <out.jpg>";
        return 1;
    }
    auto img_info = CIL::readImage(argv[1]);
    CIL::Dimensions dims(10);
    img_info.printImageInfo();
    CIL::padImage(img_info, dims);
    img_info.save(argv[2]);
    return 0;
}
