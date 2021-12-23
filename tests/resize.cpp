#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("resize-1", "[transformation][resize]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::resize(img_ppm, 500, 500);
    CHECK(CIL::equal(img_ppm, "results/resize-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::resize(img_png, 500, 500);
    CHECK(CIL::equal(img_png, "results/resize-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::resize(img_jpeg, 500, 500);
    CHECK(CIL::equal(img_jpeg, "results/resize-1.jpeg") == true);
#endif
}
