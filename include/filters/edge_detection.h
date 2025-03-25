#pragma once
#include "filter.h"
#include "filters/grayscale.h"
#include "utility/image.h"

class EdgeDetection : public Filter
{
  public:
    explicit EdgeDetection(const std::vector<std::string>& parameters) : Filter(parameters)
    {
        if (parameters.size() != 1)
        {
            throw std::invalid_argument("Invalid number of parameters (edge)");
        }
        try
        {
            threshold_ = std::stof(parameters[0]);
        }
        catch (...)
        {
            throw std::invalid_argument("edge: Invalid threshold value (edge)");
        }
        if (threshold_ < 0.0f || threshold_ > 1.0f)
        {
            throw std::invalid_argument("Threshold must be between 0 and 1 (edge)");
        }
        CreateKernel();
    }

    void Apply(Image& img) override
    {
        Matrix<Pixel> original_pixels = img.GetPixelsMatrix();

        Grayscale gs({});
        gs.Apply(img);

        original_pixels = img.GetPixelsMatrix();

        Convolve2D(img, kernel_, original_pixels);

        for (size_t x = 0; x < original_pixels.Rows(); ++x)
        {
            for (size_t y = 0; y < original_pixels.Cols(); ++y)
            {
                Pixel& pixel = img.GetPixelsMatrix()(x, y);

                if (fabs(pixel.color.r) > threshold_)
                {
                    pixel.SetColor(1, 1, 1);
                }
                else
                {
                    pixel.SetColor(0, 0, 0);
                }
            }
        }
    };

    std::string GetFilterName() const override
    {
        return filter_name_;
    }

  private:
    const std::string filter_name_ = "edge";
    Matrix<float> kernel_;
    float threshold_;

    Matrix<float> CreateKernel()
    {
        kernel_ = Matrix<float>(3, 3);
        const float middle_value = 4;
        kernel_(0, 1) = -1;
        kernel_(1, 0) = -1;
        kernel_(1, 1) = middle_value;
        kernel_(1, 2) = -1;
        kernel_(2, 1) = -1;
        return kernel_;
    }
};