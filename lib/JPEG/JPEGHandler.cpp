#include <CIL/JPEG/JPEGCore.hpp>
#include <CIL/JPEG/JPEGHandler.hpp>
#include <iostream>

namespace CIL {
    namespace JPEG {
        CIL::ImageInfo JPEGHandler::read(const char* file_name)
        {
            JPEG::ImageInfo* image_info;

            jpeg_decompress_struct cinfo;

            ErrorManager error_manager;
            error_manager.default_error_manager.error_exit = errorExit;
            error_manager.default_error_manager.output_message = outputMessage;

            FILE* file = fopen(file_name, "rb");
            if (!file || !isJPEGFile(file))
                return CIL::ImageInfo();

            if (setjmp(error_manager.jump_buffer))
            {
                // We jump here on error
                jpeg_destroy_decompress(&cinfo);
                fclose(file);
                return CIL::ImageInfo();
            }

            // set our custom error handler
            cinfo.err = jpeg_std_error(&error_manager.default_error_manager);

            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, file);
            jpeg_read_header(&cinfo, TRUE);
            jpeg_start_decompress(&cinfo);

            // allocate memory in heap only
            image_info = new JPEG::ImageInfo();

            // output prefix in cinfo objects is counterintuitive since we are
            // reading the image but they tell the "output" image format that
            // will be accessible after decompression
            image_info->width = cinfo.output_width;
            image_info->height = cinfo.output_height;
            image_info->num_components = cinfo.out_color_components;
            image_info->color_model = cinfo.out_color_space;
            image_info
                ->data = new uint8_t[image_info->width * image_info->height *
                                     image_info->num_components];

            while (cinfo.output_scanline < cinfo.image_height)
            {
                uint8_t* p = image_info->data + cinfo.output_scanline *
                                                    cinfo.image_width *
                                                    cinfo.num_components;
                jpeg_read_scanlines(&cinfo, &p, 1);
            }

            jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            fclose(file);

            std::cerr << "Load success\n";

            return image_info->toCILImage();
        }

        bool JPEGHandler::write(const CIL::ImageInfo* cil_image_info,
                                const char* file_name)
        {
            if (!cil_image_info)
                std::cerr << "CIL::ImageInfo* is null";

            struct jpeg_compress_struct cinfo;
            ErrorManager errorManager;

            FILE* file = fopen(file_name, "wb");
            if (!file)
                return false;

            const JPEG::ImageInfo* image_info = JPEG::ImageInfo::fromCILImage(
                cil_image_info);

            // set our custom error handler
            cinfo.err = jpeg_std_error(&errorManager.default_error_manager);
            errorManager.default_error_manager.error_exit = errorExit;
            errorManager.default_error_manager.output_message = outputMessage;

            if (setjmp(errorManager.jump_buffer))
            {
                // We jump here on error
                jpeg_destroy_compress(&cinfo);
                fclose(file);
                return false;
            }

            jpeg_create_compress(&cinfo);
            jpeg_stdio_dest(&cinfo, file);

            cinfo.image_width = image_info->width;
            cinfo.image_height = image_info->height;
            cinfo.input_components = image_info->num_components;
            cinfo.in_color_space = image_info->old_img_info->color_model;

            jpeg_set_defaults(&cinfo);
            jpeg_set_colorspace(&cinfo, image_info->color_model);
            jpeg_start_compress(&cinfo, TRUE);
            while (cinfo.next_scanline < image_info->height)
            {
                uint8_t* p = image_info->data + cinfo.next_scanline *
                                                    image_info->width *
                                                    image_info->num_components;
                jpeg_write_scanlines(&cinfo, &p, 1);
            }

            jpeg_finish_compress(&cinfo);
            jpeg_destroy_compress(&cinfo);
            fclose(file);
            delete image_info;

            std::cerr << "Write success\n";
            return true;
        }

        bool JPEGHandler::isJPEGFile(FILE* fp)
        {
            if (!fp)
                return false;

            uint8_t signature_chunk[2];
            int elements_read = fread(signature_chunk, 1, 2, fp);
            if (elements_read != 2)
            {
                std::cerr << "Error reading file\n";
                fseek(fp, -elements_read, SEEK_CUR);
                return false;
            }

            // 0xff 0xd8 are the magic bytes for jpeg image as used for internal
            // checks during libjpeg

            bool is_jpeg = (signature_chunk[0] == 0xff &&
                            signature_chunk[1] == 0xd8);
            fseek(fp, -elements_read, SEEK_CUR);
            return is_jpeg;
        }

        bool JPEGHandler::isJPEGFile(const char* file_name)
        {
            FILE* fp = fopen(file_name, "rb");
            bool is_jpeg = isJPEGFile(fp);
            fclose(fp);
            return is_jpeg;
        }

        void JPEGHandler::errorExit(j_common_ptr cinfo)
        {
            // cinfo->err is actually a pointer to
            // my_error_mgr.default_error_manager, since pub is the first
            // element of my_error_mgr we can do a sneaky cast
            ErrorManager* error_manager = (ErrorManager*) cinfo->err;
            (*cinfo->err->output_message)(cinfo); // print error message
            longjmp(error_manager->jump_buffer, 1);
        }

        void JPEGHandler::outputMessage(j_common_ptr cinfo)
        {
            char buffer[JMSG_LENGTH_MAX];
            (*cinfo->err->format_message)(cinfo, buffer);
            fprintf(stderr, "%s\n", buffer);
        }

        void JPEGHandler::destroy(CIL::ImageInfo* cil_image_info)
        {
            auto jpeg_img = static_cast<const JPEG::ImageInfo*>(
                cil_image_info->internalInfo());
            delete jpeg_img;
        }
    } // namespace JPEG
} // namespace CIL
