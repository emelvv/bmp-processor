#pragma once
#include <numeric>
#include <unordered_map>
#include <vector>

#include "utility/image.h"

class Filter
{
  public:
    virtual ~Filter() = default;

    explicit Filter(const std::vector<std::string>& parameters) : parameters_(parameters)
    {
    }

    virtual void Apply(Image& img) = 0;

    virtual std::string GetFilterName() const = 0;

  protected:
    const std::vector<std::string>& Parameters() const
    {
        return parameters_;
    }

    template <typename T>
    static void Convolve2D(Image& img, const Matrix<T>& kernel, const Matrix<Pixel>& original)
    {
        const int k_rows = kernel.Rows();
        const int k_cols = kernel.Cols();
        const int k_center_x = k_rows / 2;
        const int k_center_y = k_cols / 2;

        for (size_t x = 0; x < original.Rows(); ++x)
        {
            for (size_t y = 0; y < original.Cols(); ++y)
            {
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;

                for (int i = 0; i < k_rows; ++i)
                {
                    for (int j = 0; j < k_cols; ++j)
                    {
                        const int dx = i - k_center_x;
                        const int dy = j - k_center_y;

                        const int nx = static_cast<int>(x) + dx;
                        const int ny = static_cast<int>(y) + dy;

                        const Pixel& pix = original.GetClamped(nx, ny);
                        const T weight = kernel(i, j);

                        r += pix.color.r * weight;
                        g += pix.color.g * weight;
                        b += pix.color.b * weight;
                    }
                }

                Pixel& target = img.GetClamped(static_cast<int>(x), static_cast<int>(y));
                target.color.r = std::clamp(r, 0.0f, 1.0f);
                target.color.g = std::clamp(g, 0.0f, 1.0f);
                target.color.b = std::clamp(b, 0.0f, 1.0f);
            }
        }
    }

    template <typename T>
    static void Convolve1D(Image& img,
                           const std::vector<T>& kernel,
                           bool horizontal,
                           const Matrix<Pixel>& original)
    {
        const int k_center = kernel.size() / 2;

        for (size_t x = 0; x < original.Rows(); ++x)
        {
            for (size_t y = 0; y < original.Cols(); ++y)
            {
                float r = 0;
                float g = 0;
                float b = 0;

                for (size_t k = 0; k < kernel.size(); ++k)
                {
                    const size_t offset = k - k_center;

                    size_t nx = x;
                    size_t ny = y;
                    if (horizontal)
                    {
                        ny += offset;
                    }
                    else
                    {
                        nx += offset;
                    }

                    const auto& pix =
                        original.GetClamped(static_cast<int>(nx), static_cast<int>(ny));
                    const T weight = kernel[k];

                    r += pix.color.r * weight;
                    g += pix.color.g * weight;
                    b += pix.color.b * weight;
                }

                img.GetClamped(static_cast<int>(x), static_cast<int>(y)).SetColor(r, g, b);
            }
        }
    }

  private:
    std::vector<std::string> parameters_;
};