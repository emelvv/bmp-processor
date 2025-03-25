#pragma once

#include <cstdint>
#include <string>
#include <vector>

class BMP
{
  public:
#pragma pack(push, 1)
    struct BMPFileHeader
    {
        uint16_t bfType;  // Signature: (0x4D42)
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };

    struct BMPInfoHeader
    {
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter;
        int32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };
#pragma pack(pop)

  public:
    const size_t MAX_COLOR_VALUE = 255;
    const uint16_t BMP_SIGNATURE = 0x4D42;
    const uint16_t BIT_COUNT = 24;

    explicit BMP(const std::string& filename);

    void Load(const std::string& filename);
    void Save(const std::string& path);

    int32_t GetWidth() const;
    int32_t GetHeight() const;
    void SetWidth(int32_t new_width);
    void SetHeight(int32_t new_width);
    BMPFileHeader GetFileHeader() const;
    BMPInfoHeader GetInfoHeader() const;

    std::vector<uint8_t>& GetRowPixels()
    {
        return row_pixels_;
    }
    const std::vector<uint8_t>& GetRowPixels() const
    {
        return row_pixels_;
    }

  private:
    std::string filename_;

    BMPFileHeader file_header_;
    BMPInfoHeader info_header_;

    std::vector<uint8_t> row_pixels_;

    size_t Index(int32_t x, int32_t y) const;
};