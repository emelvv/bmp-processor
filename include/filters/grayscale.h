#pragma once

#include "filter.h"

class Grayscale : public Filter
{
  public:
    explicit Grayscale(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        if (!parameters.empty())
        {
            throw std::invalid_argument("Invalid number of parameters (gs)");
        }
    }

    void Apply(Image& image) override
    {
        Matrix<Pixel>& pixels = image.GetPixelsMatrix();

        for (size_t row = 0; row < static_cast<size_t>(image.GetHeight()); ++row)
        {
            for (size_t col = 0; col < static_cast<size_t>(image.GetWidth()); ++col)
            {
                Pixel& pixel = pixels(row, col);
                float color = pixel.color.r * red_coef_ + pixel.color.g * green_coef_
                              + pixel.color.b * blue_coef_;
                pixel.SetColor(color, color, color);
            }
        }
    };

    std::string GetFilterName() const override
    {
        return filter_name_;
    }

  private:
    const float red_coef_ = 0.299f;
    const float green_coef_ = 0.587f;
    const float blue_coef_ = 0.114f;
    std::string filter_name_ = "gs";
};