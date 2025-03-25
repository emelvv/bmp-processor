#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "../io/bmp.h"
#include "matrix.h"

struct Color
{
    float r;
    float g;
    float b;
};

struct Position
{
    size_t x;
    size_t y;
};

struct Pixel
{
    Color color;
    Position pos;

    void SetColor(float red, float green, float blue)
    {
        color.r = red;
        color.g = green;
        color.b = blue;
    }
};

class Image : public BMP
{
  public:
    explicit Image(const std::string& filename) : BMP(filename)
    {
        ProcessPixels(GetRowPixels());
    }

    void AddBlueSquare(size_t side_size);
    void AddGreenSquare(size_t side_size);
    void AddRedSquare(size_t side_size);

    const Pixel& GetClamped(int x, int y) const;

    Pixel& GetClamped(int x, int y);

    void ApplyChanges();

    const Matrix<Pixel>& GetPixelsMatrix() const
    {
        return pixels_;
    }
    Matrix<Pixel>& GetPixelsMatrix()
    {
        return pixels_;
    }

  private:
    void ProcessPixels(const std::vector<uint8_t>& pixel_data);

    Matrix<Pixel> pixels_;
};
