#include <CIL/ImageInfo.hpp>
#include <CIL/Pixel.hpp>
#include <functional>
#include <thread>
#include <vector>

namespace CIL {
    class ThreadHandler
    {
        const int processor_count = std::thread::hardware_concurrency();
        std::vector<std::thread> v;
        static int index;

      public:
        std::function<void(ImageInfo&, Pixel&, Pixel&)> fn;
        ThreadHandler() { v.resize(processor_count); }
        void process_pixel(ImageInfo& prev_img, Pixel& prev_px, Pixel& new_px)
        {
            if (v[index].joinable())
                v[index].join();
            v[index++] = std::thread(fn, prev_img, prev_px, new_px);
            index = index % processor_count;
        }
        void wait()
        {
            for (int i = 0; i < processor_count; i++)
                if (v[i].joinable())
                    v[i].join();
        }
        ~ThreadHandler() { wait(); }
    };
} // namespace CIL

