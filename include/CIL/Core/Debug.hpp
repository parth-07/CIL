#ifndef CIL_CORE_DEBUG_HPP
#define CIL_CORE_DEBUG_HPP
#include <iostream>
#include <map>
#include <string>
#include <vector>
#define show(...) CIL::debug::describe(#__VA_ARGS__, __VA_ARGS__);

namespace CIL {
    namespace debug {
        template <typename T> void describe(std::string var_name, T value)
        {
            std::clog << var_name << " = " << value << "\n";
        }

        template <typename T, typename... Args>
        void describe(std::string var_names, T value, Args... args)
        {
            std::string::size_type pos = var_names.find(',');
            std::string name = var_names.substr(0, pos);
            var_names = var_names.substr(pos + 1);
            std::clog << name << " = " << value << " | ";
            describe(var_names, args...);
        }

        template <typename T, typename S, typename G, typename... R,
                  template <typename, typename, typename, typename...> class M>
        std::ostream& operator<<(std::ostream& os, const M<T, S, G, R...>& v)
        {
            os << "{";
            for (auto i : v)
            {
                os << i << ',';
            }
            os << "\b}";
            return os;
        }

        template <class T, class V>
        std::ostream& operator<<(std::ostream& os, const std::pair<T, V>& p)
        {
            os << p.first << ":" << p.second;
            return os;
        }

        template <typename S, typename T,
                  template <typename, typename...> class V>
        std::ostream& operator<<(std::ostream& os, const V<S, T>& m)
        {
            os << "[";
            // show((func(S,T...)));
            for (auto i : m)
            {
                os << i << ",";
            }
            os << "\b]";
            return os;
        }
    } // namespace debug
} // namespace CIL
#endif
