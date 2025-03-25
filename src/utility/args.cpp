#include "utility/args.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

void PrintHelp()
{
    std::cout << "Usage: bmp_editor [input_file] [output_file] [filters...]\n";
    std::cout << "Filters:\n";
    std::cout << "  -crop [width] [height]  - crop image to specified size\n";
    std::cout << "    Example: -crop 100 100\n";
    std::cout << "  -gs  - convert image to grayscale\n";
    std::cout << "  -neg  - invert image colors\n";
    std::cout << "  -sharp  - sharpen image\n";
    std::cout << "  -edge  - detect edges in image\n";
    std::cout << "  -blur [sigma] - blur image with specified size and angle\n";
    std::cout << "    Example: -blur 10\n";
    std::cout << "  -motion [size] [angle]  - blur image with motion blur filter "
                 "with specified size and angle\n";
    std::cout << "    Example: -motion 20 0\n";
}

Args::FilterArg::FilterArg(std::string_view name, std::vector<std::string>&& parameters)
    : name_(name), parameters_(std::move(parameters))
{
}

const std::string& Args::FilterArg::GetName() const
{
    return name_;
}

const std::vector<std::string>& Args::FilterArg::GetParameters() const
{
    return parameters_;
}

void Args::FilterArg::AddParameter(const std::string& param)
{
    parameters_.push_back(param);
}

Args::Args() : input_file_(), output_file_(), filters_()
{
}

Args::Args(int argc, const char* argv[])
{
    if (argc == 1)
    {
        PrintHelp();
        exit(0);
    }
    else if (argc < 3)
    {
        throw std::invalid_argument("Not enough arguments.");
    }

    input_file_ = argv[1];
    output_file_ = argv[2];

    for (int i = 3; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (!arg.empty() && arg[0] == '-')
        {
            std::string filter_name = arg.substr(1);
            if (!IsValidFilterName(filter_name))
            {
                throw std::invalid_argument("Invalid filter name: " + filter_name);
            }
            filters_.emplace_back(filter_name, std::vector<std::string>{});
        }
        else
        {
            if (filters_.empty())
            {
                throw std::invalid_argument("Parameter '" + arg
                                            + "' is not associated with any filter.");
            }
            filters_.back().AddParameter(arg);
        }
    }
}

const std::string& Args::GetInputFile() const
{
    return input_file_;
}

const std::string& Args::GetOutputFile() const
{
    return output_file_;
}

const std::vector<Args::FilterArg>& Args::GetFilters() const
{
    return filters_;
}

const std::vector<std::string> POSSIBLE_FILTERS =
    {"crop", "gs", "neg", "sharp", "edge", "blur", "motion"};

bool Args::IsValidFilterName(std::string_view name)
{
    return !name.empty()
           && std::find(POSSIBLE_FILTERS.begin(), POSSIBLE_FILTERS.end(), name)
                  != POSSIBLE_FILTERS.end();
}
