#pragma once
#include "filter.h"
#include "utility/image.h"

class Sharpening : public Filter
{
  public:
    explicit Sharpening(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        CreateKernel();
        if (!parameters.empty())
        {
            throw std::invalid_argument("Invalid number of parameters (sharp)");
        }
    }

    void Apply(Image& img) override
    {
        Matrix<Pixel> original_pixels = img.GetPixelsMatrix();

        Convolve2D(img, kernel_, original_pixels);
    };

    std::string GetFilterName() const override
    {
        return filter_name_;
    }

  private:
    const std::string filter_name_ = "sharp";
    Matrix<float> kernel_;

    Matrix<float> CreateKernel()
    {
        kernel_ = Matrix<float>(3, 3);
        const float middle_value = 5;
        kernel_(0, 1) = -1;
        kernel_(1, 0) = -1;
        kernel_(1, 1) = middle_value;
        kernel_(1, 2) = -1;
        kernel_(2, 1) = -1;
        return kernel_;
    }
};