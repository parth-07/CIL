#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("rotate-1", "[transformation][rotate]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::rotate(img_ppm, 45);
    img_ppm.save("rotate-1.ppm");
    CHECK(CIL::equal(img_ppm, "results/rotate-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::rotate(img_png, 45);
    CHECK(CIL::equal(img_png, "results/rotate-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::rotate(img_jpeg, 45);
    CHECK(CIL::equal(img_jpeg, "results/rotate-1.jpeg") == true);
#endif
}