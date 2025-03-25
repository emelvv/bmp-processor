#pragma once
#include "filter.h"
#include "filters/grayscale.h"
#include "utility/image.h"

class GaussianBlur : public Filter
{
  public:
    explicit GaussianBlur(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        if (parameters.size() != 1)
        {
            throw std::invalid_argument("Invalid number of parameters (blur)");
        }
        try
        {
            sigma_ = std::stof(parameters[0]);
        }
        catch (...)
        {
            throw std::invalid_argument("Invalid sigma value (blur)");
        }
        if (sigma_ <= 0)
        {
            throw std::invalid_argument("Sigma must be positive (blur)");
        }
        CreateKernel();
    }

    void Apply(Image& img) override
    {
        const Matrix<Pixel> original = img.GetPixelsMatrix();

        Convolve1D(img, kernel_h_, true, original);

        const Matrix<Pixel> intermediate = img.GetPixelsMatrix();

        Convolve1D(img, kernel_v_, false, intermediate);
    }

    std::string GetFilterName() const override
    {
        return filter_name_;
    }

  private:
    const std::string filter_name_ = "blur";
    std::vector<float> kernel_h_;
    std::vector<float> kernel_v_;
    float sigma_;

    void CreateKernel()
    {
        const int radius = static_cast<int>(std::ceil(3 * sigma_));
        const int size = 2 * radius + 1;

        std::vector<float> kernel_1_d(size);
        float sum = 0.0f;
        for (int i = -radius; i <= radius; ++i)
        {
            float i_float = static_cast<float>(i);
            float val = static_cast<float>(exp(-i_float * i_float / (2 * sigma_ * sigma_)));
            kernel_1_d[i + radius] = val;
            sum += val;
        }

        for (auto& val : kernel_1_d)
        {
            val /= sum;
        }

        kernel_h_ = kernel_1_d;
        kernel_v_ = kernel_1_d;
    }
};