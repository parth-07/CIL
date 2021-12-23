#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("crop-1", "[transformation][crop]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::cropImage(img_ppm, CIL::Dimensions(10));
    img_ppm.save("crop-1.ppm");
    CHECK(CIL::equal(img_ppm, "results/crop-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::cropImage(img_png, CIL::Dimensions(10));
    CHECK(CIL::equal(img_png, "results/crop-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::cropImage(img_jpeg, CIL::Dimensions(10));
    CHECK(CIL::equal(img_jpeg, "results/crop-1.jpeg") == true);
#endif
}