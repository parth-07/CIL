#ifndef HANDLER_HPP
#define HANDLER_HPP
#include <errno.h>
#include <iostream>
#include <stddef.h>
#include <type_traits>

namespace methods {
    enum
    {
        invert = 10,
        brightness,
        contrast,
        crop,
        pad,
        resize,
        rotate,
        flip,
        grayscale,
        kernel,
    };
}

namespace args {
    enum
    {
        infile = 30,
        outfile,
        percentage,
        X_origin,
        Y_origin,
        length,
        breadth,
        degrees,
        axis,
        preserve_colortype
    };
}

namespace function_parameters {

#define IS_INVALID(...)                                                        \
    function_parameters::is_invalid_fn(#__VA_ARGS__, __VA_ARGS__)

    template <typename Arg> int is_invalid_fn(std::string name, Arg arg)
    {
        if (arg == Arg())
        {
            std::cerr << name << " not defined !! \n";
            return EINVAL;
        }
        return 0;
    }

    template <typename T, typename... Args>
    int is_invalid_fn(std::string var_names, T value, Args... args)
    {
        int ret = 0;
        std::string::size_type pos = var_names.find(',');
        std::string name = var_names.substr(0, pos);
        var_names = var_names.substr(pos + 1);
        ret = is_invalid_fn(name, value);
        if (ret)
            return ret;
        return is_invalid_fn(var_names, args...);
    }

    typedef struct
    {
        bool preserve_colortype;
    } grayscale;
    typedef struct
    {
        int percentage;
    } percentage;
    typedef struct
    {
        int X_origin;
        int Y_origin;
        int length;
        int breadth;
    } dimensions;
    typedef struct
    {
        int degrees;
    } rotate;
    typedef struct
    {
        int axis;
    } flip;
} // namespace function_parameters

struct function_handler
{
    int id;
    void* data;
    std::string infile, outfile;
};

int initialize_handler(struct function_handler* handler, int method)
{
    switch (method)
    {
        case methods::invert: goto out;
        case methods::brightness:
        case methods::contrast:
            handler->data = new function_parameters::percentage();
            break;
        case methods::crop:
        case methods::pad:
        case methods::resize:
            handler->data = new function_parameters::dimensions();
            break;
        case methods::rotate:
            handler->data = new function_parameters::rotate();
            break;
        case methods::flip:
            handler->data = new function_parameters::flip();
            break;
        case methods::grayscale:
            handler->data = new function_parameters::grayscale();
            break;
        default: return ENOSYS;
    }

    if (!handler->data)
        return ENOMEM;

out:
    handler->id = method;

    return 0;
}

int free_handler(struct function_handler* handler)
{
    switch (handler->id)
    {
        case methods::invert: goto out;
        case methods::brightness:
        case methods::contrast:
            delete (function_parameters::percentage*) handler->data;
            break;
        case methods::crop:
        case methods::pad:
        case methods::resize:
            delete (function_parameters::dimensions*) handler->data;
            break;
        case methods::rotate:
            delete (function_parameters::rotate*) handler->data;
            break;
        case methods::flip:
            delete (function_parameters::flip*) handler->data;
            break;
        case methods::grayscale:
            delete (function_parameters::grayscale*) handler->data;
            break;
        default: return ENOSYS;
    }

out:
    handler->data = NULL;
    handler->id = 0;

    return 0;
}

#endif
