#ifndef CIL_PNGCORE_HPP
#define CIL_PNGCORE_HPP
#include <png.h>

#include <setjmp.h>

namespace CIL {
    class ImageInfo;
    namespace PNG {
        const int chunk_size = 8;

        /// Convenient data structure to store libpng read data structs and make
        /// them easier to use.
        ///
        /// `destroy()` member function should be called when the underlying
        /// libpng read structs are no longer needed.
        struct LibpngReadData
        {
            png_structp png_ptr;
            png_infop info_ptr;

            LibpngReadData(png_structp p_png_ptr = nullptr,
                           png_infop p_info_ptr = nullptr)
                : png_ptr(p_png_ptr), info_ptr(p_info_ptr)
            {}
            void destroy()
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            }
            bool empty() const { return png_ptr != NULL; }
            /// Returns true if `png_ptr` and `info_ptr` are initialized in this
            /// function; otherwise returns false.
            bool init(FILE* fp);
        };

        /// Convenient data structure to store libpng write data structs and
        /// make them easier to use.
        ///
        /// `destroy()` member function should be called when the underlying
        /// write data structs are no longer needed.
        struct LibpngWriteData
        {
            png_structp png_ptr;
            png_infop info_ptr;

            LibpngWriteData(png_structp p_png_ptr = nullptr,
                            png_infop p_info_ptr = nullptr)
                : png_ptr(p_png_ptr), info_ptr(p_info_ptr)
            {}
            void destroy() { png_destroy_write_struct(&png_ptr, &info_ptr); }
            bool empty() const { return png_ptr != NULL; }
            bool init(FILE* fp);
        };

        /// It is used to store all the required information of a image to
        /// construct the image again using libpng write routines
        // TODO: Also preserve ancillary chunk information such tRNS, gAMA,
        // sRGB, bKGD etc
        // TODO: Add a function to free image data.
        class ImageInfo
        {
            // Image headers
            png_uint_32 m_width = 0, m_height = 0;
            png_byte m_sample_depth = 0, m_color_type = 0, m_num_channels = 0;
            png_byte m_interlace_type = 0, m_compression_type = 0,
                     m_filter_type = 0;
            size_t m_rowbytes = 0;
            // Image data
            png_bytepp m_scanlines = nullptr;

            public:
            ImageInfo(LibpngReadData& lrd, png_bytepp& scanlines);
            ImageInfo(const CIL::ImageInfo* cil_img_info)
            {
                init(cil_img_info);
            }
            ImageInfo(const PNG::ImageInfo&);
            /// Initialises the `PNG::ImageInfo` object using a `CIL::ImageInfo`
            /// object.
            bool init(const CIL::ImageInfo* cil_img_info);
            /// Builds a `CIL::ImageInfo` object that describes the same image
            /// as desribed by the `this` object.
            CIL::ImageInfo toCILImageInfo();
            void dumpImageHeaders() const;
            ~ImageInfo();
            static void destroy(PNG::ImageInfo** img_info);
            uint32_t width() const { return m_width; }
            uint32_t height() const { return m_height; }
            png_byte sampleDepth() const { return m_sample_depth; }
            png_byte colorType() const { return m_color_type; };
            png_byte numChannels() const { return m_num_channels; }
            png_byte interlaceType() const { return m_interlace_type; }
            png_byte compressionType() const { return m_compression_type; }
            png_byte filterType() const { return m_filter_type; }
            size_t rowbytes() const { return m_rowbytes; }
            png_bytepp scanlines() const {return m_scanlines;}
        };

        /// Writes the image described by `img_info` in the file `filename`.
        bool writePNGFile(const PNG::ImageInfo* img_info, const char* filename);
    } // namespace PNG
} // namespace CIL
#endif
