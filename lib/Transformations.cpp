#include <CIL/Core/Coordinate.hpp>
#include <CIL/Core/Debug.hpp>
#include <CIL/Core/DetachedFPPixel.hpp>
#include <CIL/Core/Types.hpp>
#include <CIL/Core/Utils.hpp>
#include <CIL/ImageInfo.hpp>
#include <CIL/ThreadHandler.hpp>
#include <CIL/Transformations.hpp>
#include <cassert>
#include <cmath>
#include <limits>

namespace CIL {
    void invertColor(ImageInfo& img)
    {
        CIL::ThreadHandler th;
        th.fn = [](ImageInfo& old_img, Pixel px, Pixel new_px) {
            for (int i = 0;
                 i < px.numComponents() - old_img.hasAlphaComponent(); i++)
                new_px[i] = std::numeric_limits<uint8_t>::max() - px[i];
            /*
             *std::cerr << px.row() << " " << px.col() << "\n";
             */
        };

        for (auto px : img)
        {
            if (px.isValid())
                th.process_pixel(img, px, px);
        }
    }

    void changeContrast(ImageInfo& img, const double contrast)
    {
        double temp = 259 * (contrast + 255) / (255 * (259 - contrast));
        for (auto px : img)
        {
            for (auto i = 0; i < px.numComponents() - img.hasAlphaComponent();
                 i++)
            {
                double ans = temp * (px[i] - 128) + 128;
                if (ans > 255)
                    px[i] = 255;
                else if (ans < 0)
                    px[i] = 0;
                else
                    px[i] = round(ans);
            }
        }
    }
    void changeBrightness(ImageInfo& img, int16_t brightness)
    {
        for (auto px : img)
        {
            DetachedFPPixel dpx(px);
            dpx += brightness;
            dpx.capRange(0, 255);
            if (img.hasAlphaComponent())
            {
                dpx.back() = px.back();
            }
            px = dpx;
        }
    }

    void flipImage(ImageInfo& img, const Axis axis)
    {
        uint32_t outer, inner;
        if (axis == Axis::Y)
        {
            outer = img.height();
            inner = img.width() / 2;
        } else
        {
            outer = img.width();
            inner = img.height() / 2;
        }
        for (uint32_t i = 0; i < outer; i++)
        {
            for (uint32_t j = 0; j < inner; j++)
            {
                Pixel px1, px2;
                if (axis == Axis::Y)
                {
                    px1 = img(i, j);
                    px2 = img(i, img.width() - j - 1);
                } else
                {
                    px1 = img(j, i);
                    px2 = img(img.height() - j - 1, i);
                }
                Pixel::swap(px1, px2);
            }
        }
    }

    void cropImage(ImageInfo& img, const Dimensions& dims)
    {
        auto px1 = img(0, 0);
        px1.setBounds(dims);
        px1.init();

        auto new_width = img.width() - dims.left - dims.right;
        auto new_height = img.height() - dims.top - dims.bottom;

        ImageMatrix new_img(new_width, new_height, img.numComponents(),
                            img.sampleDepth());
        for (auto px2 : new_img)

        {
            px2.copyComponents(px1);
            px1++;
        }
        assert(!px1.isValid() && "px1 didn't reach end");
        img.setData(new_img);
    }

    void padImage(ImageInfo& img, const Dimensions& dims)
    {
        auto new_width = img.width() + dims.left + dims.right;
        auto new_height = img.height() + dims.top + dims.bottom;

        ImageMatrix new_img(new_width, new_height, img.numComponents(),
                            img.sampleDepth());

        Pixel px1 = new_img(dims.top, dims.left);
        px1.setBounds(dims);
        for (auto px2 : img)
        {
            px1.copyComponents(px2);
            ++px1;
        }
        img.setData(new_img);
    }

    static void createGaussianFilter(Sequence<double>& GKernel, int size)
    {
        // initialising standard deviation value to the size of kernel
        // to increase blur effect
        double sigma = size;
        double r, s = 2.0 * sigma * sigma;

        // sum is for normalization
        double sum = 0.0;

        GKernel.resize(size);
        const int temp = size / 2;
        // generating 5x5 kernel
        for (int x = -temp; x <= temp; x++)
        {
            GKernel[x + temp].resize(size);
            for (int y = -temp; y <= temp; y++)
            {
                r = sqrt(x * x + y * y);
                GKernel[x + temp][y + temp] = (exp(-(r * r) / s)) /
                                              (utils::pi * s);
                sum += GKernel[x + temp][y + temp];
            }
        }

        // normalising the Kernel
        for (auto i = 0U; i < GKernel.size(); ++i)
            for (auto j = 0U; j < GKernel[0].size(); ++j)
                GKernel[i][j] /= sum;
    }

    struct Kernel
    {
        const Sequence<double> matrix;
        const KernelType kernel_type;

        Kernel(const KernelType kernel_type, const Sequence<double>& matrix)
            : matrix(matrix), kernel_type(kernel_type)
        {}
        static Kernel get(const KernelType& kernel_type, int size = 3)
        {
            if (size == 0)
                assert("size can't be zero");

            switch (kernel_type)
            {
                case KernelType::BOX_BLUR:
                {
                    Sequence<double> matrix;
                    matrix.resize(size);
                    for (int i = 0; i < size; i++)
                    {
                        matrix[i].resize(size, 1.00);
                    }
                    utils::multiplyConstantToSequence(matrix,
                                                      1.00 / size * size);
                    return Kernel(kernel_type, matrix);
                }
                case KernelType::GAUSSIAN_BLUR:
                {
                    Sequence<double> matrix;
                    createGaussianFilter(matrix, size);
                    return Kernel(kernel_type, matrix);
                }
                case KernelType::EDGE_DETECTION:
                {
                    // TODO: need to find algorithm to expand it's size to
                    // different ones
                    Sequence<double> matrix{{-1, -1, -1},
                                            {-1, 8, -1},
                                            {-1, -1, -1}};
                    return Kernel(kernel_type, matrix);
                }
                default:
                {
                    assert(false);
                }
            }
        }
    };

    void applyKernel(ImageInfo& img, KernelType kernel_type, int size)
    {
        assert((size & 1) && "only odd sized kernels allowed");

        Kernel k = Kernel::get(kernel_type, size);

        // pad the image to get the output image of same size as input image
        padImage(img, Dimensions((k.matrix.size() - 1) / 2));

        ImageMatrix matrix = img.getData();
        matrix.convolute(k.matrix);
        img.setData(matrix);
    }

    // TODO: Add `preserve_image` parameter.
    void rotate(ImageInfo& img, int degrees, RotationKind rotation_kind)
    {
        ImageMatrix tf_img_data(img.width(), img.height(), img.numComponents(),
                                img.sampleDepth());

        Coordinate origin((img.width() / 2) + 0.5, (img.height() / 2) + 0.5);

        for (auto px : tf_img_data)
        {
            Coordinate P(px.col() + 0.5, px.row() + 0.5);
            auto source = utils::computeCoordinateAfterRotation(origin, P,
                                                                -degrees);
            if (rotation_kind == RotationKind::rotation_by_area_mapping)
            {
                px = utils::bilinearInterpolation(img, source);
            } else if (rotation_kind == RotationKind::rotation_by_sampling)
            {
                auto source_pixel = img(std::lround(source.y),
                                        std::lround(source.x));
                if (!source_pixel.isValid())
                    continue;

                for (auto i = 0; i < px.numComponents(); ++i)
                {
                    px[i] = source_pixel[i];
                }
            }
        }
        img.setData(tf_img_data);
    }

    void convertToGrayscale(ImageInfo& img, bool preserve_colortype)
    {
        if (img.colorModel() == ColorModel::COLOR_GRAY ||
            img.colorModel() == ColorModel::COLOR_GRAY_ALPHA)
            return;
        auto num_components = (preserve_colortype
                                   ? img.numComponents()
                                   : 1 + img.hasAlphaComponent());
        ImageMatrix new_img_data(img.width(), img.height(), num_components,
                                 img.sampleDepth());

        for (auto px : new_img_data)
        {
            DetachedFPPixel dpx = img(px.row(), px.col());
            dpx.scale({0.299, 0.587, 0.114});
            auto gray_value = std::lround(dpx.sum(/*exclude_alpha=*/true));
            dpx.forEach([gray_value](typename DetachedFPPixel::ValueType&
                                         val) { val = gray_value; },
                        /*exclude_alpha=*/true);
            px = dpx;
        }
        if (!preserve_colortype)
        {
            if (img.hasAlphaComponent())
                img.setColorModel(ColorModel::COLOR_GRAY_ALPHA);
            else
                img.setColorModel(ColorModel::COLOR_GRAY);
        }
        img.setData(new_img_data);
    }

    void resize(ImageInfo& img, uint32_t new_width, uint32_t new_height,
                ResizeAlgorithm resize_algorithm)
    {
        double scale_x = (new_width * 1.00) / img.width();
        double scale_y = (new_height * 1.00) / img.height();

        ImageMatrix new_image_data(new_width, new_height, img.numComponents(),
                                   img.sampleDepth());

        for (auto px : new_image_data)
        {
            Coordinate source = {px.col() / scale_x, px.row() / scale_y};
            switch (resize_algorithm)
            {
                case ResizeAlgorithm::nearest_neighbour_interpolation:
                {
                    auto old_px = img(source.getNearestIntegralY(),
                                      source.getNearestIntegralX());
                    if (old_px.isValid())
                        px.copyComponents(old_px);
                    break;
                }
                case ResizeAlgorithm::bilinear_interpolation:
                    px = utils::bilinearInterpolation(img, source);
                    break;
            }
        }
        img.setData(new_image_data);
    }

    void zoom(ImageInfo& img, const Dimensions& dims)
    {
        auto width = img.width();
        auto height = img.height();
        cropImage(img, dims);
        resize(img, width, height);
    }
} // namespace CIL
