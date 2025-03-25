#pragma once
#include "filter.h"
#include "utility/image.h"

class Negative : public Filter
{
  public:
    explicit Negative(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        if (!parameters.empty())
        {
            throw std::invalid_argument("Invalid number of parameters (neg)");
        }
    }

    void Apply(Image& image) override
    {
        Matrix<Pixel>& pixels = image.GetPixelsMatrix();

        std::vector<std::string> parameters = Parameters();
        for (size_t row = 0; row < static_cast<size_t>(image.GetHeight()); ++row)
        {
            for (size_t col = 0; col < static_cast<size_t>(image.GetWidth()); ++col)
            {
                Pixel& pixel = pixels(row, col);
                pixel.SetColor(1 - pixel.color.r, 1 - pixel.color.g, 1 - pixel.color.b);
            }
        }
    };

    std::string GetFilterName() const override
    {
        return filter_name_;
    }

  private:
    const std::string filter_name_ = "neg";
};