#include "helpers/handler.hpp"
#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <errno.h>
#include <getopt.h>
#include <string.h>

#define ERR(errcode, errstring, label)                                         \
    do                                                                         \
    {                                                                          \
        ret = errno = errcode;                                                 \
        errmsg = errstring;                                                    \
        goto label;                                                            \
    } while (0);

int handle_percentage(struct function_handler* handler, char arg, char* optarg)
{
    function_parameters::percentage* fun = (function_parameters::percentage*)
                                               handler->data;
    switch (arg)
    {
        case args::percentage: fun->percentage = atoi(optarg); break;
        default: printf("Argument not found for method\n"); return EINVAL;
    }
    return 0;
}

int handle_dimensions(struct function_handler* handler, char arg, char* optarg)
{
    function_parameters::dimensions* fun = (function_parameters::dimensions*)
                                               handler->data;
    switch (arg)
    {
        case args::X_origin: fun->X_origin = atoi(optarg); break;
        case args::Y_origin: fun->Y_origin = atoi(optarg); break;
        case args::length: fun->length = atoi(optarg); break;
        case args::breadth: fun->breadth = atoi(optarg); break;
        default: printf("Argument not found for method\n"); return EINVAL;
    }
    return 0;
}

int handle_rotate(struct function_handler* handler, char arg, char* optarg)
{
    function_parameters::rotate* fun = (function_parameters::rotate*)
                                           handler->data;
    switch (arg)
    {
        case args::degrees: fun->degrees = atoi(optarg); break;
        default: printf("Argument not found for method\n"); return EINVAL;
    }
    return 0;
}

int handle_flip(struct function_handler* handler, char arg, char* optarg)
{
    function_parameters::flip* fun = (function_parameters::flip*) handler->data;
    switch (arg)
    {
        case args::axis: fun->axis = atoi(optarg); break;
        default: printf("Argument not found for method\n"); return EINVAL;
    }
    return 0;
}

int handle_grayscale(struct function_handler* handler, char arg, char* optarg)
{
    function_parameters::grayscale* fun = (function_parameters::grayscale*)
                                              handler->data;
    switch (arg)
    {
        case args::preserve_colortype:
            fun->preserve_colortype = atoi(optarg);
            break;
        default: printf("Argument not found for method\n"); return EINVAL;
    }
    return 0;
}

void print_usage()
{
    printf(
        "Usage: "
        "./main <method> <args> -infile <input file> -outfile <output file>\n"
        "\n"
        "Methods\n"
        "\n"
        "-invert\n"
        "\t no arguments required\n"
        "-contrast or -brightness\n"
        "\t--percentage             <int>\n"
        "-crop or -pad or -resize\n"
        "\t--X_origin               <int> (default:0)\n"
        "\t--Y_origin               <int> (default:0)\n"
        "\t--length                 <int>\n"
        "\t--breadth                <int>\n"
        "-rotate\n"
        "\t--degrees                <int>\n"
        "-flip\n"
        "\t--axis                   <int>\n"
        "\t     1           X_axis\n"
        "\t     2           Y_axis\n"
        "-grayscale\n"
        "\t--preserve_colortype     <bool> (default:0)\n");
}

int handler_call_function(struct function_handler* handler, CIL::ImageInfo img)
{
    int ret = 0;
    switch (handler->id)
    {
        case methods::invert:
        {
            CIL::invertColor(img);
            break;
        }
        case methods::brightness:
        {
            function_parameters::percentage*
                args = (function_parameters::percentage*) handler->data;

            ret = IS_INVALID(args->percentage);
            if (ret)
                goto err;

            CIL::changeBrightness(img, args->percentage);
            break;
        }
        case methods::contrast:
        {
            function_parameters::percentage*
                args = (function_parameters::percentage*) handler->data;

            ret = IS_INVALID(args->percentage);
            if (ret)
                goto err;

            CIL::changeContrast(img, args->percentage);
            break;
        }
        case methods::crop:
        {
            function_parameters::dimensions*
                args = (function_parameters::dimensions*) handler->data;

            ret = IS_INVALID(args->length, args->breadth);
            if (ret)
                goto err;

            CIL::cropImage(img,
                           img.getDimensions({args->X_origin, args->Y_origin},
                                             args->length, args->breadth));
            break;
        }
        case methods::pad:
        {
            function_parameters::dimensions*
                args = (function_parameters::dimensions*) handler->data;

            ret = IS_INVALID(args->length, args->breadth);
            if (ret)
                goto err;

            CIL::padImage(img,
                          img.getDimensions({args->X_origin, args->Y_origin},
                                            args->length, args->breadth));
            break;
        }
        case methods::resize:
        {
            function_parameters::dimensions*
                args = (function_parameters::dimensions*) handler->data;

            ret = IS_INVALID(args->length, args->breadth);
            if (ret)
                goto err;

            CIL::resize(img, args->length, args->breadth);
            break;
        }
        case methods::rotate:
        {
            function_parameters::rotate* args = (function_parameters::rotate*)
                                                    handler->data;

            ret = IS_INVALID(args->degrees);
            if (ret)
                goto err;

            CIL::rotate(img, args->degrees);
            break;
        }
        case methods::flip:
        {
            function_parameters::flip* args = (function_parameters::flip*)
                                                  handler->data;

            ret = IS_INVALID(args->axis);
            if (ret)
                goto err;

            if (args->axis > 2 || args->axis < 0)
            {
                fprintf(stderr, "axis value undefined");
                goto err;
            }

            CIL::flipImage(img, static_cast<CIL::Axis>(args->axis - 1));
            break;
        }
        case methods::grayscale:
        {
            function_parameters::grayscale*
                args = (function_parameters::grayscale*) handler->data;

            CIL::convertToGrayscale(img, args->preserve_colortype);
            break;
        }
        default: return ENOSYS;
    }

    img.save(handler->outfile);
err:
    return ret;
}

int main(int argc, char** argv)
{
    int option_index = 0;
    int ret = 0, method = 0;
    std::string errmsg = "";
    function_handler handler = function_handler();
    CIL::ImageInfo img;
    errno = 0;
    char arg = 0;

    const option long_options[] =
        {{"infile", required_argument, NULL, args::infile},
         {"outfile", required_argument, NULL, args::outfile},
         {"invert", no_argument, &method, methods::invert},
         {"brightness", no_argument, &method, methods::brightness},
         {"contrast", no_argument, &method, methods::contrast},
         {"crop", no_argument, &method, methods::crop},
         {"pad", no_argument, &method, methods::pad},
         {"resize", no_argument, &method, methods::resize},
         {"rotate", no_argument, &method, methods::rotate},
         {"flip", no_argument, &method, methods::flip},
         {"grayscale", no_argument, &method, methods::grayscale},
         {"kernel", no_argument, &method, methods::kernel},
         {"percentage", required_argument, NULL, args::percentage},
         {"X_origin", required_argument, NULL, args::X_origin},
         {"Y_origin", required_argument, NULL, args::Y_origin},
         {"length", required_argument, NULL, args::length},
         {"breadth", required_argument, NULL, args::breadth},
         {"degrees", required_argument, NULL, args::degrees},
         {"axis", required_argument, NULL, args::axis},
         {"preserve_colortype", required_argument, NULL,
          args::preserve_colortype},
         {NULL, 0, NULL, 0}};

    while ((arg = getopt_long_only(argc, argv, "", long_options,
                                   &option_index)) != -1)
    {
        if (method)
        {
            if (handler.id)
                ERR(EINVAL, "Specifying multiple methods", err_free_handler);

            ret = initialize_handler(&handler, method);
            if (ret)
                goto out;

            // method is stored in handler->id now;
            method = 0;
            continue;
        }

        if (!handler.id)
            ERR(EINVAL, "Method not set", out);

        switch (arg)
        {
            case args::infile: handler.infile = optarg; continue;
            case args::outfile: handler.outfile = optarg; continue;
            case '?':
                printf("Unknown option %s\n", long_options[option_index].name);
                ret = EINVAL;
                break;
            case ':':
                printf("Missing option for %s\n",
                       long_options[option_index].name);
                ret = EINVAL;
                break;
        }

        if (ret)
            goto err_free_handler;

        switch (handler.id)
        {
            case methods::invert: break;
            case methods::brightness:
            case methods::contrast:
                ret = handle_percentage(&handler, arg, optarg);
                break;
            case methods::crop:
            case methods::pad:
            case methods::resize:
                ret = handle_dimensions(&handler, arg, optarg);
                break;
            case methods::rotate:
                ret = handle_rotate(&handler, arg, optarg);
                break;
            case methods::flip: ret = handle_flip(&handler, arg, optarg); break;
            case methods::grayscale:
                ret = handle_grayscale(&handler, arg, optarg);
                break;
        }

        if (ret)
            goto err_free_handler;
    }

    if (argc == 1)
        ERR(EINVAL, "Please add arguments", out);

    if (!handler.infile.length() || !handler.outfile.length())
        ERR(ENOENT, "-infile or -outfile not specified", err_free_handler);

    img = CIL::readImage(handler.infile);

    ret = handler_call_function(&handler, img);
    if (ret)
        ERR(EINVAL, "Missing arguments", err_free_handler);

err_free_handler:
    free_handler(&handler);
out:
    errno = ret;
    perror(errmsg.c_str());
    if (ret)
        print_usage();
    return ret;
}
