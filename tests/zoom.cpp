#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("zoom-1", "[transformation][zoom]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::zoom(img_ppm, CIL::Dimensions(50));
    CHECK(CIL::equal(img_ppm, "results/zoom-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::zoom(img_png, CIL::Dimensions(50));
    CHECK(CIL::equal(img_png, "results/zoom-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::zoom(img_jpeg, CIL::Dimensions(50));
    CHECK(CIL::equal(img_jpeg, "results/zoom-1.jpeg") == true);
#endif
}
