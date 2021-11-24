#include <CIL/ImageInfo.hpp>
#include <CIL/PNG/PNGCore.hpp>
#include <CIL/PNG/PNGHandler.hpp>
#include <iostream>
#include <setjmp.h>

namespace CIL {
    namespace PNG {
        /// Attaches a very basic error handler routine with the provided
        /// `png_ptr` and `info_ptr`.
        // TODO: Use more sophisticated error handler routine.
        static void set_libpng_error_handler(png_structp png_ptr,
                                             png_infop info_ptr)
        {
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            }
        }

        bool LibpngReadData::init(FILE* fp)
        {
            // `png_ptr` is already initialized, therefore should not be
            // initialized again.
            if (png_ptr)
                return false;
            if (!fp)
                return false;
            if (!isPNGFile(fp))
                return false;
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                             /*error_ptr*/ NULL,
                                             /*error_fn*/ NULL,
                                             /*warn_fn*/ NULL);
            if (!png_ptr)
                return false;
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                destroy();
                return false;
            }
            set_libpng_error_handler(png_ptr, info_ptr);
            png_init_io(png_ptr, fp);
            return true;
        }

        bool LibpngWriteData::init(FILE* fp)
        {
            // `png_ptr` is already initialized, therefore should not be
            // initialized again.
            if (png_ptr)
                return false;
            if (!fp)
            {
                return false;
            }

            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                              /*error_ptr*/ NULL,
                                              /*error_fn*/ NULL,
                                              /*warn_fn*/ NULL);
            if (!png_ptr)
                return false;

            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                destroy();
                return false;
            }

            set_libpng_error_handler(png_ptr, info_ptr);
            png_init_io(png_ptr, fp);
            return true;
        }

        ImageInfo::ImageInfo(LibpngReadData& lrd, png_bytepp& p_scanlines)
            : scanlines(p_scanlines)
        {
            width = png_get_image_width(lrd.png_ptr, lrd.info_ptr);
            height = png_get_image_height(lrd.png_ptr, lrd.info_ptr);
            sample_depth = png_get_bit_depth(lrd.png_ptr, lrd.info_ptr);
            color_type = png_get_color_type(lrd.png_ptr, lrd.info_ptr);
            num_channels = png_get_channels(lrd.png_ptr, lrd.info_ptr);
            interlace_type = png_get_interlace_type(lrd.png_ptr, lrd.info_ptr);
            compression_type = png_get_compression_type(lrd.png_ptr,
                                                        lrd.info_ptr);
            filter_type = png_get_filter_type(lrd.png_ptr, lrd.info_ptr);
        }

        void ImageInfo::dumpImageHeaders() const
        {
            std::clog << "Dimensions: " << width << "x" << height << "\n";
            std::clog << "sample depth: " << static_cast<unsigned>(sample_depth)
                      << "\n";
            std::clog << "number of channels: "
                      << static_cast<unsigned>(num_channels) << "\n";
            std::clog << "Color type: " << static_cast<unsigned>(color_type)
                      << "\n";
            std::clog << "Interlace type: "
                      << static_cast<unsigned>(interlace_type) << "\n";
            std::clog << "Compression type: "
                      << static_cast<unsigned>(compression_type) << "\n";
            std::clog << "Filter type: " << static_cast<unsigned>(filter_type)
                      << "\n";
        }

        bool writePNGFile(const PNG::ImageInfo* img_info, const char* filename)
        {
            FILE* fp = fopen(filename, "wb");
            LibpngWriteData lwd;
            if (!lwd.init(fp))
            {
                fclose(fp);
                return false;
            }

            png_set_IHDR(lwd.png_ptr, lwd.info_ptr, img_info->width,
                         img_info->height, img_info->sample_depth,
                         img_info->color_type, img_info->interlace_type,
                         img_info->compression_type, img_info->filter_type);

            png_write_info(lwd.png_ptr, lwd.info_ptr);
            if (!img_info->scanlines)
                return false;
            png_write_image(lwd.png_ptr, img_info->scanlines);
            png_write_end(lwd.png_ptr, /*end_info_ptr*/ NULL);

            lwd.destroy();
            fclose(fp);
            return true;
        }

        static ColorModel
        getCorrespondingCILColorModel(png_byte libpng_color_model)
        {
            switch (libpng_color_model)
            {
                case PNG_COLOR_TYPE_GRAY: return ColorModel::COLOR_GRAY;
                case PNG_COLOR_TYPE_GRAY_ALPHA:
                    return ColorModel::COLOR_GRAY_ALPHA;
                case PNG_COLOR_TYPE_PALETTE: return ColorModel::COLOR_PALETTE;
                case PNG_COLOR_TYPE_RGB: return ColorModel::COLOR_RGB;
                case PNG_COLOR_TYPE_RGB_ALPHA: return ColorModel::COLOR_RGBA;
                default: throw("Unrecognized color model");
            }
        }

        static png_byte getCorrespondingLibpngColorModel(ColorModel color_model)
        {
            switch (color_model)
            {
                case ColorModel::COLOR_GRAY: return PNG_COLOR_TYPE_GRAY;
                case ColorModel::COLOR_GRAY_ALPHA:
                    return PNG_COLOR_TYPE_GRAY_ALPHA;
                case ColorModel::COLOR_PALETTE: return PNG_COLOR_TYPE_PALETTE;
                case ColorModel::COLOR_RGB: return PNG_COLOR_TYPE_RGB;
                case ColorModel::COLOR_RGBA: return PNG_COLOR_TYPE_RGB_ALPHA;
                default: throw("Unrecognized color model");
            }
        }

        CIL::ImageInfo* PNG::ImageInfo::toCILImageInfo() const
        {
            auto cil_img_info = new CIL::ImageInfo();
            cil_img_info->width = width;
            cil_img_info->height = height;
            cil_img_info->color_model = getCorrespondingCILColorModel(
                color_type);
            cil_img_info->num_components = static_cast<int>(num_channels);
            cil_img_info->sample_depth = sample_depth;
            cil_img_info->data = static_cast<uint8_t*>(scanlines[0]);
            cil_img_info->internal_info = this;
            cil_img_info->image_type = ImageType::PNG;

            return cil_img_info;
        }

        bool PNG::ImageInfo::init(const CIL::ImageInfo* cil_img_info)
        {
            width = cil_img_info->width;
            height = cil_img_info->height;
            color_type = getCorrespondingLibpngColorModel(
                cil_img_info->color_model);
            num_channels = static_cast<png_byte>(cil_img_info->num_components);
            sample_depth = static_cast<png_byte>(cil_img_info->sample_depth);
            // TODO: Add support for different bit depth images.
            auto rowbytes = (width * num_channels);
            if (sample_depth == 16)
                rowbytes *= 2;
            else
                rowbytes /= (8 / sample_depth);

            scanlines = new png_bytep[height];
            for (auto i = 0U; i < height; ++i)
            {
                scanlines[i] = cil_img_info->data + i * rowbytes;
            }

            auto internal_info = static_cast<const PNG::ImageInfo*>(
                cil_img_info->internal_info);
            interlace_type = internal_info->interlace_type;
            compression_type = internal_info->compression_type;
            filter_type = internal_info->filter_type;
            return true;
        }

        void PNG::ImageInfo::destroy(const PNG::ImageInfo** img_info)
        {
            delete[](*img_info)->scanlines;
            delete *img_info;
            *img_info = nullptr;
        }

        void PNG::ImageInfo::destroy(PNG::ImageInfo** img_info)
        {
            auto ptr = const_cast<const PNG::ImageInfo**>(img_info);
            destroy(ptr);
        }
    } // namespace PNG
} // namespace CIL