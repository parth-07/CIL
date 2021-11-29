#include <CIL/ImageInfo.hpp>
#include <CIL/PNG/PNGCore.hpp>
#include <CIL/PNG/PNGHandler.hpp>
#include <png.h>

namespace CIL {
    namespace PNG {
        /// Allocates memory to store PNG image data.
        ///
        /// Returns a `rows` pointer such that, `rows[i]` points to the ith row
        /// of the image data.
        static png_bytepp allocate_image_data_memory(LibpngReadData& lrd)
        {
            auto height = png_get_image_height(lrd.png_ptr, lrd.info_ptr);
            auto rowbytes = png_get_rowbytes(lrd.png_ptr, lrd.info_ptr);
            png_bytepp rows = new png_bytep[height];
            png_bytep data = new png_byte[height * rowbytes];
            for (auto i = 0U; i < height; ++i)
            {
                rows[i] = data + i * rowbytes;
            }
            return rows;
        }

        CIL::ImageInfo PNGHandler::read(const char* filename)
        {
            LibpngReadData lrd;
            FILE* fp = fopen(filename, "rb");

            if (!lrd.init(fp))
            {
                fclose(fp);
                return CIL::ImageInfo();
            }
            png_read_info(lrd.png_ptr, lrd.info_ptr);

            auto color_type = png_get_color_type(lrd.png_ptr, lrd.info_ptr);
            if (color_type == PNG_COLOR_TYPE_PALETTE)
                png_set_palette_to_rgb(lrd.png_ptr);
            png_read_update_info(lrd.png_ptr, lrd.info_ptr);
            auto scanlines = allocate_image_data_memory(lrd);
            png_read_image(lrd.png_ptr, scanlines);
            // TODO: Store and utilize end info as well.
            png_read_end(lrd.png_ptr, /*end_info_ptr*/ NULL);

            auto img_info = new PNG::ImageInfo(lrd, scanlines);
            fclose(fp);
            lrd.destroy();
            return img_info->toCILImageInfo();
        }

        CIL::ImageInfo PNGHandler::readAs8RGB(const char* filename)
        {
            LibpngReadData lrd;
            FILE* fp = fopen(filename, "rb");

            if (!lrd.init(fp))
            {
                fclose(fp);
                return CIL::ImageInfo();
            }

            png_read_info(lrd.png_ptr, lrd.info_ptr);

            auto color_type = png_get_color_type(lrd.png_ptr, lrd.info_ptr);
            auto bit_depth = png_get_bit_depth(lrd.png_ptr, lrd.info_ptr);

            if (bit_depth == 16)
                png_set_strip_16(lrd.png_ptr);

            if (color_type == PNG_COLOR_TYPE_PALETTE)
                png_set_palette_to_rgb(lrd.png_ptr);

            if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                png_set_expand_gray_1_2_4_to_8(lrd.png_ptr);

            if (png_get_valid(lrd.png_ptr, lrd.info_ptr, PNG_INFO_tRNS))
                png_set_tRNS_to_alpha(lrd.png_ptr);

            if (color_type == PNG_COLOR_TYPE_GRAY ||
                color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
                png_set_gray_to_rgb(lrd.png_ptr);

            png_read_update_info(lrd.png_ptr, lrd.info_ptr);

            auto scanlines = allocate_image_data_memory(lrd);
            png_read_image(lrd.png_ptr, scanlines);
            // TODO: Store and utilize end info as well.
            png_read_end(lrd.png_ptr, /*end_info_ptr*/ NULL);

            auto img_info = new PNG::ImageInfo(lrd, scanlines);
            fclose(fp);
            lrd.destroy();
            return img_info->toCILImageInfo();
        }

        bool PNGHandler::write(const CIL::ImageInfo* cil_img_info,
                               const char* filename)
        {
            PNG::ImageInfo* img_info = new PNG::ImageInfo(cil_img_info);
            bool res = writePNGFile(img_info, filename);
            PNG::ImageInfo::destroy(&img_info);
            return res;
        }

        bool PNGHandler::isPNGFile(FILE* fp)
        {
            if (!fp)
                return false;
            png_byte signature_chunk[chunk_size];
            int elements_read = fread(signature_chunk, 1, chunk_size, fp);
            if (elements_read != chunk_size)
            {
                fseek(fp, -elements_read, SEEK_CUR);
                return false;
            }
            bool is_png = !png_sig_cmp(signature_chunk, 0, chunk_size);
            fseek(fp, -elements_read, SEEK_CUR);
            return is_png;
        }

        bool PNGHandler::isPNGFile(const char* filename)
        {
            FILE* fp = fopen(filename, "rb");
            bool is_png = isPNGFile(fp);
            fclose(fp);
            return is_png;
        }

        void PNGHandler::destroy(const CIL::ImageInfo* cil_image_info)
        {
            auto ptr = const_cast<PNG::ImageInfo*>(
                static_cast<const PNG::ImageInfo*>(
                    cil_image_info->internalInfo()));
            PNG::ImageInfo::destroy(&ptr);
        }
    } // namespace PNG
} // namespace CIL
