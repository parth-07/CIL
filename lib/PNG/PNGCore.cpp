#include <CIL/ImageInfo.hpp>
#include <CIL/PNG/PNGCore.hpp>
#include <CIL/PNG/PNGHandler.hpp>
#include <cstring>
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
            if (!PNGHandler::isPNGFile(fp))
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

        ImageInfo::ImageInfo(LibpngReadData& lrd, png_bytepp& scanlines)
            : m_scanlines(scanlines)
        {
            m_width = png_get_image_width(lrd.png_ptr, lrd.info_ptr);
            m_height = png_get_image_height(lrd.png_ptr, lrd.info_ptr);
            m_sample_depth = png_get_bit_depth(lrd.png_ptr, lrd.info_ptr);
            m_color_type = png_get_color_type(lrd.png_ptr, lrd.info_ptr);
            m_num_channels = png_get_channels(lrd.png_ptr, lrd.info_ptr);
            m_interlace_type = png_get_interlace_type(lrd.png_ptr,
                                                      lrd.info_ptr);
            m_compression_type = png_get_compression_type(lrd.png_ptr,
                                                          lrd.info_ptr);
            m_filter_type = png_get_filter_type(lrd.png_ptr, lrd.info_ptr);
            m_rowbytes = png_get_rowbytes(lrd.png_ptr, lrd.info_ptr);
        }

        PNG::ImageInfo::ImageInfo(const PNG::ImageInfo& other)
            : m_width(other.m_width), m_height(other.m_height),
              m_sample_depth(other.m_sample_depth),
              m_color_type(other.m_color_type),
              m_num_channels(other.m_num_channels),
              m_interlace_type(other.m_interlace_type),
              m_compression_type(other.m_compression_type),
              m_filter_type(other.m_filter_type), m_rowbytes(other.m_rowbytes)
        {
            m_scanlines = nullptr;
            if (other.m_scanlines)
            {
                m_scanlines = new png_bytep[m_height];
                auto data = new png_byte[m_height * m_rowbytes];
                std::memcpy(data, other.m_scanlines[0],
                            sizeof(png_byte) * m_height * m_rowbytes);
            }
        }

        void ImageInfo::dumpImageHeaders() const
        {
            std::clog << "Dimensions: " << m_width << "x" << m_height << "\n";
            std::clog << "sample depth: "
                      << static_cast<unsigned>(m_sample_depth) << "\n";
            std::clog << "number of channels: "
                      << static_cast<unsigned>(m_num_channels) << "\n";
            std::clog << "Color type: " << static_cast<unsigned>(m_color_type)
                      << "\n";
            std::clog << "Interlace type: "
                      << static_cast<unsigned>(m_interlace_type) << "\n";
            std::clog << "Compression type: "
                      << static_cast<unsigned>(m_compression_type) << "\n";
            std::clog << "Filter type: " << static_cast<unsigned>(m_filter_type)
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

            png_set_IHDR(lwd.png_ptr, lwd.info_ptr, img_info->width(),
                         img_info->height(), img_info->sampleDepth(),
                         img_info->colorType(), img_info->interlaceType(),
                         img_info->compressionType(), img_info->filterType());

            png_write_info(lwd.png_ptr, lwd.info_ptr);
            if (!img_info->scanlines())
                return false;
            png_write_image(lwd.png_ptr, img_info->scanlines());
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

        CIL::ImageInfo PNG::ImageInfo::toCILImageInfo()
        {
            auto color_model = getCorrespondingCILColorModel(m_color_type);
            void* internal_info = this;
            auto image_type = ImageType::PNG;

            std::unique_ptr<uint8_t[]> data(
                static_cast<uint8_t*>(m_scanlines[0]));

            delete[] m_scanlines;
            m_scanlines = static_cast<png_bytepp>(nullptr);
            CIL::ImageInfo cil_img_info(m_width, m_height, m_num_channels,
                                        m_sample_depth, color_model, image_type,
                                        std::move(data), internal_info);
            return cil_img_info;
        }

        bool PNG::ImageInfo::init(const CIL::ImageInfo* cil_img_info)
        {
            auto internal_info = static_cast<const PNG::ImageInfo*>(
                cil_img_info->internalInfo());
            m_width = cil_img_info->width();
            m_height = cil_img_info->height();
            m_color_type = getCorrespondingLibpngColorModel(
                cil_img_info->colorModel());
            m_num_channels = static_cast<png_byte>(
                cil_img_info->numComponents());
            m_sample_depth = static_cast<png_byte>(cil_img_info->sampleDepth());
            m_rowbytes = internal_info->m_rowbytes;

            m_scanlines = new png_bytep[m_height];
            auto data = new png_byte[m_height * m_rowbytes];

            std::memcpy(data, &(*cil_img_info)(0, 0, 0), m_height * m_rowbytes);

            for (auto i = 0U; i < m_height; ++i)
            {
                m_scanlines[i] = data + i * m_rowbytes;
            }

            m_interlace_type = internal_info->m_interlace_type;
            m_compression_type = internal_info->m_compression_type;
            m_filter_type = internal_info->m_filter_type;
            return true;
        }

        PNG::ImageInfo::~ImageInfo()
        {
            if (m_scanlines)
            {
                if (m_scanlines[0])
                    delete[] m_scanlines[0];
                delete[] m_scanlines;
            }
        }
        void PNG::ImageInfo::destroy(PNG::ImageInfo** img_info)
        {
            if (img_info == nullptr)
                return;
            delete *img_info;
            *img_info = nullptr;
        }
    } // namespace PNG
} // namespace CIL
