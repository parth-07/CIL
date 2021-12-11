#ifndef CIL_CORE_TYPES_HPP
#define CIL_CORE_TYPES_HPP
#include <cstdint>
#include <vector>

namespace CIL {
    template <typename T> using Sequence = std::vector<std::vector<T>>;
}

#endif
