#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.png> <out.png>";
        return 1;
    }
    CIL::ImageInfo image_info = CIL::readImage(argv[1]);
    // gaussian blur removes noise for better edge detection
    CIL::applyKernel(image_info, CIL::KernelType::GAUSSIAN_BLUR, 5);
    CIL::applyKernel(image_info, CIL::KernelType::EDGE_DETECTION);
    image_info.save(argv[2]);
    return 0;
}
