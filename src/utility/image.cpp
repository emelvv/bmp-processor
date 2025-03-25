#include "utility/image.h"

void Image::ProcessPixels(const std::vector<uint8_t>& pixel_data)
{
    pixels_ = Matrix<Pixel>(GetHeight(), GetWidth());

    size_t idx = 0;
    for (size_t row = 0; row < static_cast<size_t>(GetHeight()); ++row)
    {
        for (size_t col = 0; col < static_cast<size_t>(GetWidth()); ++col)
        {
            Pixel& pixel = pixels_(row, col);
            pixel.color.b =
                static_cast<float>(pixel_data[idx++]) / static_cast<float>(MAX_COLOR_VALUE);
            pixel.color.g =
                static_cast<float>(pixel_data[idx++]) / static_cast<float>(MAX_COLOR_VALUE);
            pixel.color.r =
                static_cast<float>(pixel_data[idx++]) / static_cast<float>(MAX_COLOR_VALUE);
            pixel.pos.x = row;
            pixel.pos.y = col;
        }
    }
}

void Image::ApplyChanges()
{
    std::vector<uint8_t> data;
    data.reserve(GetHeight() * GetWidth() * 3);
    for (size_t row = 0; row < static_cast<size_t>(GetHeight()); ++row)
    {
        for (size_t col = 0; col < static_cast<size_t>(GetWidth()); ++col)
        {
            const Pixel& pixel = pixels_(row, col);
            data.push_back(static_cast<uint8_t>(
                std::round(pixel.color.b * static_cast<float>(MAX_COLOR_VALUE))));
            data.push_back(static_cast<uint8_t>(
                std::round(pixel.color.g * static_cast<float>(MAX_COLOR_VALUE))));
            data.push_back(static_cast<uint8_t>(
                std::round(pixel.color.r * static_cast<float>(MAX_COLOR_VALUE))));
        }
    }
    GetRowPixels() = std::move(data);
}

Pixel& Image::GetClamped(int x, int y)
{
    x = std::clamp(x, 0, static_cast<int>(GetHeight()) - 1);
    y = std::clamp(y, 0, static_cast<int>(GetWidth()) - 1);
    return pixels_(x, y);
}

const Pixel& Image::GetClamped(int x, int y) const
{
    x = std::clamp(x, 0, static_cast<int>(GetHeight()) - 1);
    y = std::clamp(y, 0, static_cast<int>(GetWidth()) - 1);
    return pixels_(x, y);
}

void Image::AddRedSquare(size_t side_size)
{
    for (size_t row = GetHeight() - side_size; row < static_cast<size_t>(GetHeight()); ++row)
    {
        for (size_t col = 0; col < side_size; ++col)
        {
            Pixel& pixel = pixels_(row, col);
            pixel.color.r = 1;
            pixel.color.b = 0;
            pixel.color.g = 0;
        }
    }
    ApplyChanges();
}

void Image::AddGreenSquare(size_t side_size)
{
    for (size_t row = 0; row < side_size; ++row)
    {
        for (size_t col = GetWidth() - side_size; col < static_cast<size_t>(GetWidth()); ++col)
        {
            Pixel& pixel = pixels_(row, col);
            pixel.color.g = 1;
            pixel.color.b = 0;
            pixel.color.r = 0;
        }
    }
    ApplyChanges();
}

void Image::AddBlueSquare(size_t side_size)
{
    for (size_t row = 0; row < side_size; ++row)
    {
        for (size_t col = 0; col < side_size; ++col)
        {
            Pixel& pixel = pixels_(row, col);
            pixel.color.b = 1;
            pixel.color.r = 0;
            pixel.color.g = 0;
        }
    }
    ApplyChanges();
}
