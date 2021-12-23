#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <catch2/catch.hpp>

TEST_CASE("flip-1", "[transformation][flip]")
{
    auto img_ppm = CIL::readImage("in.ppm");
    CIL::flipImage(img_ppm, CIL::Axis::X);
    // img_ppm.save("flip-1.ppm");
    CHECK(CIL::equal(img_ppm, "results/flip-1.ppm") == true);

#ifdef CIL_PNG_ENABLED
    auto img_png = CIL::readImage("in.png");
    CIL::flipImage(img_png, CIL::Axis::X);
    // img_png.save("flip-1.png");
    CHECK(CIL::equal(img_png, "results/flip-1.png") == true);
#endif

#ifdef CIL_JPEG_ENABLED
    auto img_jpeg = CIL::readImage("in.jpeg");
    CIL::flipImage(img_jpeg, CIL::Axis::X);
    // img_jpeg.save("flip-1.jpeg");
    CHECK(CIL::equal(img_jpeg, "results/flip-1.jpeg") == true);
#endif
}
