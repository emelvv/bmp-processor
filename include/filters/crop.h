#pragma once

#include "filter.h"

class Crop : public Filter
{
  public:
    explicit Crop(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        if (parameters.size() != 2)
        {
            throw std::invalid_argument("Invalid number of parameters (crop)");
        }
        for (const auto& param : parameters)
        {
            if (param.empty() || !std::all_of(param.begin(), param.end(), ::isdigit))
            {
                throw std::invalid_argument("Parameters must be numeric values (crop)");
            }
        }
    }

    void Apply(Image& image) override
    {
        std::vector<std::string> parameters = Parameters();

        size_t width = std::stoul(parameters[0]);
        size_t height = std::stoul(parameters[1]);

        image.SetWidth(static_cast<int32_t>(width));
        image.SetHeight(static_cast<int32_t>(height));
    };

    std::string GetFilterName() const override
    {
        return filter_name_;
    }

  private:
    const std::string filter_name_ = "crop";
};