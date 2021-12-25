#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("brightness-1", "[transformation][brightness]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::changeBrightness(img_ppm, 100);
    CHECK(CIL::equal(img_ppm, "results/brightness-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::changeBrightness(img_png, 100);
    CHECK(CIL::equal(img_png, "results/brightness-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::changeBrightness(img_jpeg, 100);
    CHECK(CIL::equal(img_jpeg, "results/brightness-1.jpeg") == true);
#endif
}
