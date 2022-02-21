#include <CIL/Drawing.hpp>
#include <CIL/ImageInfo.hpp>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: ./bin <in.png> <out.png>";
        return 1;
    }
    CIL::ImageInfo image_info = CIL::readImage(argv[1]);
    CIL::drawPoint(image_info(5, 5), CIL::Color::RED);
    CIL::drawPoint(image_info(5, 6), CIL::Color::GREEN);
    CIL::drawPoint(image_info(6, 5), CIL::Color::BLUE);
    CIL::drawPoint(image_info(6, 6), CIL::Color::WHITE);
    CIL::drawPoint(image_info(7, 7), {155, 155, 155});
    image_info.save(argv[2]);
    return 0;
}
