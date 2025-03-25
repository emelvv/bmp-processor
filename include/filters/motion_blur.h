#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

#include "filter.h"

class MotionBlur : public Filter
{
  public:
    explicit MotionBlur(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        if (parameters.size() != 2)
        {
            throw std::invalid_argument(
                "MotionBlur requires 2 parameters: <size> <angle> (motion)");
        }

        try
        {
            size_ = std::stoi(parameters[0]);
            angle_ = std::stof(parameters[1]);
        }
        catch (...)
        {
            throw std::invalid_argument("Invalid motion blur parameters (motion)");
        }

        const float max_angle = 360;
        if (size_ < 1 || angle_ < 0 || angle_ >= max_angle)
        {
            throw std::invalid_argument("Size must be >=1, angle in [0, 360) (motion)");
        }

        CreateKernels();
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
        return "motion";
    }

  private:
    int size_;
    float angle_;
    std::vector<float> kernel_h_;
    std::vector<float> kernel_v_;

    void CreateKernels()
    {
        const float radians = static_cast<float>(angle_ * M_PI / 180.0f);
        const float cos_val = std::cos(radians);
        const float sin_val = std::sin(radians);

        kernel_h_ = Create1DKernel(size_, cos_val);
        kernel_v_ = Create1DKernel(size_, sin_val);
    }

    std::vector<float> Create1DKernel(int size, float proportion)
    {
        std::vector<float> kernel(size, 0.0f);
        const int center = size / 2;
        const int length = static_cast<int>(std::abs(proportion) * static_cast<float>(size - 1));

        int start = std::max(0, center - length / 2);
        int end = std::min(size, center + length / 2 + 1);
        std::fill(kernel.begin() + start, kernel.begin() + end, 1.0f);

        float sum = std::accumulate(kernel.begin(), kernel.end(), 0.0f);
        if (sum > 0)
        {
            for (auto& val : kernel)
            {
                val /= sum;
            }
        }

        return kernel;
    }
};