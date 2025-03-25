#include "io/bmp.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

BMP::BMP(const std::string& filename)
{
    Load(filename);
}

int32_t BMP::GetWidth() const
{
    return info_header_.biWidth;
}

int32_t BMP::GetHeight() const
{
    return info_header_.biHeight;
}

void BMP::Load(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    filename_ = filename;

    in.read(reinterpret_cast<char*>(&file_header_), sizeof(file_header_));
    in.read(reinterpret_cast<char*>(&info_header_), sizeof(info_header_));

    if (file_header_.bfType != BMP_SIGNATURE)
    {
        throw std::runtime_error("Not a valid BMP file.");
    }

    if (info_header_.biBitCount != BIT_COUNT || info_header_.biCompression != 0)
    {
        throw std::runtime_error("Unsupported BMP format (only 24-bit uncompressed supported).");
    }

    if (info_header_.biClrUsed > 0 || info_header_.biBitCount < BIT_COUNT)
    {
        throw std::runtime_error("Indexed BMP with color table is not supported.");
    }

    int32_t width = info_header_.biWidth;
    int32_t height = info_header_.biHeight;

    int row_padded = (width * 3 + 3) & (~3);
    int padding = row_padded - width * 3;

    row_pixels_.resize(width * height * 3);

    for (int y = height - 1; y >= 0; --y)
    {
        in.read(reinterpret_cast<char*>(&row_pixels_[y * width * 3]), width * 3);
        in.ignore(padding);
    }

    in.close();
}

void BMP::Save(const std::string& path)
{
    std::ofstream out(path, std::ios::binary);
    if (!out)
    {
        throw std::runtime_error("Cannot open file for writing: " + path);
    }

    int32_t width = info_header_.biWidth;
    int32_t height = info_header_.biHeight;

    int row_padded = (width * 3 + 3) & (~3);
    int padding = row_padded - width * 3;

    out.write(reinterpret_cast<const char*>(&file_header_), sizeof(file_header_));
    out.write(reinterpret_cast<const char*>(&info_header_), sizeof(info_header_));

    std::vector<uint8_t> padding_data(padding, 0);
    for (int y = height - 1; y >= 0; --y)
    {
        out.write(reinterpret_cast<const char*>(&row_pixels_[y * width * 3]), width * 3);
        if (padding > 0)
        {
            out.write(reinterpret_cast<const char*>(padding_data.data()), padding);
        }
    }

    out.close();
}

void BMP::SetWidth(int32_t new_width)
{
    int32_t current_width = info_header_.biWidth;
    int32_t height = info_header_.biHeight;
    if (new_width <= 0)
    {
        throw std::invalid_argument("Width must be positive");
    }

    if (new_width > current_width)
    {
        new_width = current_width;
    }

    std::vector<uint8_t> new_pixels(new_width * height * 3);
    for (int32_t y = 0; y < height; ++y)
    {
        std::copy(row_pixels_.begin() + y * current_width * 3,
                  row_pixels_.begin() + y * current_width * 3 + new_width * 3,
                  new_pixels.begin() + y * new_width * 3);
    }
    row_pixels_ = std::move(new_pixels);

    info_header_.biWidth = new_width;
    int32_t new_row_padded = (new_width * 3 + 3) & (~3);
    info_header_.biSizeImage = new_row_padded * height;
    file_header_.bfSize = new_row_padded * height + sizeof(file_header_) + sizeof(info_header_);
}

void BMP::SetHeight(int32_t new_height)
{
    int32_t current_height = info_header_.biHeight;
    int32_t width = info_header_.biWidth;
    if (new_height <= 0)
    {
        throw std::invalid_argument("Height must be positive");
    }

    if (new_height > current_height)
    {
        new_height = current_height;
    }

    std::vector<uint8_t> new_pixels(width * new_height * 3);
    for (int32_t y = 0; y < new_height; ++y)
    {
        std::copy(row_pixels_.begin() + y * width * 3,
                  row_pixels_.begin() + y * width * 3 + width * 3,
                  new_pixels.begin() + y * width * 3);
    }
    row_pixels_ = std::move(new_pixels);

    info_header_.biHeight = new_height;
    int32_t new_row_padded = (width * 3 + 3) & (~3);
    info_header_.biSizeImage = new_row_padded * new_height;
    file_header_.bfSize = new_row_padded * new_height + sizeof(file_header_) + sizeof(info_header_);
}

BMP::BMPFileHeader BMP::GetFileHeader() const
{
    return file_header_;
}

BMP::BMPInfoHeader BMP::GetInfoHeader() const
{
    return info_header_;
}

size_t BMP::Index(int32_t x, int32_t y) const
{
    return ((info_header_.biHeight - 1 - y) * info_header_.biWidth + x) * 3;
}
