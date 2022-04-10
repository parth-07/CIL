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
        std::function<void(int, int)> fn;
        ThreadHandler() { v.resize(processor_count); }
        void process_row(int row, int width)
        {
            if (v[index].joinable())
                v[index].join();

            std::function<void()> fn2 = [this, row, width]() {
                for (auto i = 0; i < width; i++)
                    fn(row, i);
            };

            v[index++] = std::thread(fn2);
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

