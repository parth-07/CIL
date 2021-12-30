#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("grayscale-1", "[transformation][grayscale]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::convertToGrayscale(img_ppm);
    CHECK(CIL::equal(img_ppm, "results/grayscale-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::convertToGrayscale(img_png);
    CHECK(CIL::equal(img_png, "results/grayscale-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::convertToGrayscale(img_jpeg);
    CHECK(CIL::equal(img_jpeg, "results/grayscale-1.jpeg") == true);
#endif
}
