/*!
 * \file    peak_ipl_simple_types.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>

#include <string>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 *\brief Enum holding the possible conversion modes.
 */
enum class ConversionMode
{
    Fast = PEAK_IPL_CONVERSION_MODE_FAST, //!< Fast conversion mode
    HighQuality = PEAK_IPL_CONVERSION_MODE_HIGH_QUALITY, //!< High-Quality conversion mode
    Classic = PEAK_IPL_CONVERSION_MODE_CLASSIC, //!< Classic conversion mode
};

/*!
 *\brief Enum holding the possible pixel format names.
 */
enum class PixelFormatName
{
    Invalid = PEAK_IPL_PIXEL_FORMAT_INVALID, //!< Invalid Pixel Format

    BayerGR8 = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_8, //!< BayerGR 8-Bit pixel format
    BayerGR10 = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_10, //!< BayerGR 10-Bit pixel format
    BayerGR12 = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_12, //!< BayerGR 12-Bit pixel format

    BayerRG8 = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_8, //!< BayerRG 8-Bit pixel format
    BayerRG10 = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_10, //!< BayerRG 10-Bit pixel format
    BayerRG12 = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_12, //!< BayerRG 12-Bit pixel format

    BayerGB8 = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_8, //!< BayerGB 8-Bit pixel format
    BayerGB10 = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_10, //!< BayerGB 10-Bit pixel format
    BayerGB12 = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_12, //!< BayerGB 12-Bit pixel format

    BayerBG8 = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_8, //!< BayerBG 8-Bit pixel format
    BayerBG10 = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_10, //!< BayerBG 10-Bit pixel format
    BayerBG12 = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_12, //!< BayerBG 12-Bit pixel format

    Mono8 = PEAK_IPL_PIXEL_FORMAT_MONO_8, //!< Mono 8-Bit pixel format
    Mono10 = PEAK_IPL_PIXEL_FORMAT_MONO_10, //!< Mono 10-Bit pixel format
    Mono12 = PEAK_IPL_PIXEL_FORMAT_MONO_12, //!< Mono 12-Bit pixel format

    YUV420_8_YY_UV_SemiplanarIDS = PEAK_IPL_PIXEL_FORMAT_YUV420_8_YY_UV_SemiplanarIDS, //!< YUV 4:2:0 8-bit YY/UV Semiplanar
    YUV420_8_YY_VU_SemiplanarIDS = PEAK_IPL_PIXEL_FORMAT_YUV420_8_YY_VU_SemiplanarIDS, //!< YUV 4:2:0 8-bit YY/VU Semiplanar

    RGB8 = PEAK_IPL_PIXEL_FORMAT_RGB_8, //!< RGB 8-Bit pixel format
    RGB10 = PEAK_IPL_PIXEL_FORMAT_RGB_10, //!< RGB 10-Bit pixel format
    RGB12 = PEAK_IPL_PIXEL_FORMAT_RGB_12, //!< RGB 12-Bit pixel format

    BGR8 = PEAK_IPL_PIXEL_FORMAT_BGR_8, //!< BGR 8-Bit pixel format
    BGR10 = PEAK_IPL_PIXEL_FORMAT_BGR_10, //!< BGR 10-Bit pixel format
    BGR12 = PEAK_IPL_PIXEL_FORMAT_BGR_12, //!< BGR 12-Bit pixel format

    RGBa8 = PEAK_IPL_PIXEL_FORMAT_RGBA_8, //!< RGBa 8-Bit pixel format
    RGBa10 = PEAK_IPL_PIXEL_FORMAT_RGBA_10, //!< RGBa 10-Bit pixel format
    RGBa12 = PEAK_IPL_PIXEL_FORMAT_RGBA_12, //!< RGBa 12-Bit pixel format

    BGRa8 = PEAK_IPL_PIXEL_FORMAT_BGRA_8, //!< BGRa 8-Bit pixel format
    BGRa10 = PEAK_IPL_PIXEL_FORMAT_BGRA_10, //!< BGRa 10-Bit pixel format
    BGRa12 = PEAK_IPL_PIXEL_FORMAT_BGRA_12, //!< BGRa 12-Bit pixel format

    BayerBG10p = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_10_PACKED, //!< BayerBG 10-Bit packed pixel format
    BayerBG12p = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_12_PACKED, //!< BayerBG 12-Bit packed pixel format

    BayerGB10p = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_10_PACKED, //!< BayerGB 10-Bit packed pixel format
    BayerGB12p = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_12_PACKED, //!< BayerGB 12-Bit packed pixel format

    BayerGR10p = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_10_PACKED, //!< BayerGR 10-Bit packed pixel format
    BayerGR12p = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_12_PACKED, //!< BayerGR 12-Bit packed pixel format

    BayerRG10p = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_10_PACKED, //!< BayerRG 10-Bit packed pixel format
    BayerRG12p = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_12_PACKED, //!< BayerRG 12-Bit packed pixel format

    Mono10p = PEAK_IPL_PIXEL_FORMAT_MONO_10_PACKED, //!< Mono 10-Bit packed pixel format
    Mono12p = PEAK_IPL_PIXEL_FORMAT_MONO_12_PACKED, //!< Mono 10-Bit packed pixel format

    RGB10p32 = PEAK_IPL_PIXEL_FORMAT_RGB_10_PACKED_32, //!< BayerBG 10-Bit packed pixel format

    BGR10p32 = PEAK_IPL_PIXEL_FORMAT_BGR_10_PACKED_32, //!< BayerBG 10-Bit packed pixel format

    /*!
     * \brief BayerRG 10-Bit packed 40bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerRG10g40IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_10_GROUPED_40_IDS,
    /*!
     * \brief BayerGB 10-Bit packed 40bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerGB10g40IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_10_GROUPED_40_IDS,
    /*!
     * \brief BayerGR 10-Bit packed 40bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerGR10g40IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_10_GROUPED_40_IDS,
    /*!
     * \brief BayerBG 10-Bit packed 40bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerBG10g40IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_10_GROUPED_40_IDS,

    /*!
     * \brief BayerRG 12-Bit packed 24bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerRG12g24IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_RG_12_GROUPED_24_IDS,
    /*!
     * \brief BayerGB 12-Bit packed 24bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerGB12g24IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_GB_12_GROUPED_24_IDS,
    /*!
     * \brief BayerGR 12-Bit packed 24bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerGR12g24IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_GR_12_GROUPED_24_IDS,
    /*!
     * \brief BayerBG 12-Bit packed 24bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    BayerBG12g24IDS = PEAK_IPL_PIXEL_FORMAT_BAYER_BG_12_GROUPED_24_IDS,

    /*!
     * \brief Mono 10-Bit packed 40bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    Mono10g40IDS = PEAK_IPL_PIXEL_FORMAT_MONO_10_GROUPED_40_IDS,
    /*!
     * \brief Mono 12-Bit packed 24bit grouped pixel format
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    Mono12g24IDS = PEAK_IPL_PIXEL_FORMAT_MONO_12_GROUPED_24_IDS,
};

/*!
 *\brief Enum holding the possible bayer patterns.
 */
enum class BayerPattern
{
    Unknown = PEAK_IPL_BAYER_PATTERN_UNKNOWN, //!< Unknown pattern
    None = PEAK_IPL_BAYER_PATTERN_NONE, //!< No pattern
    Bayer_GR = PEAK_IPL_BAYER_PATTERN_BAYER_GR, //!< BayerGR pattern
    Bayer_RG = PEAK_IPL_BAYER_PATTERN_BAYER_RG, //!< BayerRG pattern
    Bayer_GB = PEAK_IPL_BAYER_PATTERN_BAYER_GB, //!< BayerGB pattern
    Bayer_BG = PEAK_IPL_BAYER_PATTERN_BAYER_BG //!< BayerBG pattern
};

/*!
 *\brief Enum holding the possible endianness (the byte order).
 */
enum class Endianness
{
    Unknown = PEAK_IPL_ENDIANNESS_UNKNOWN, //!< Unknown endianness
    LittleEndian = PEAK_IPL_ENDIANNESS_LITTLE_ENDIAN, //!< Little endian (lower bytes first)
    BigEndian = PEAK_IPL_ENDIANNESS_BIG_ENDIAN //!< Big endian (higher bytes first)
};

/*!
 *\brief Enum holding the possible orientation.
 */
enum class Orientation
{
    Horizontal = PEAK_IPL_ORIENTATION_HORIZONTAL, //!< Horizontal orientation
    Vertical = PEAK_IPL_ORIENTATION_VERTICAL //!< Vertical orientation
};

/*!
 *\brief Enum holding the possible interpolation algorithm
 */
enum class InterpolationAlgorithm
{
    nearest = PEAK_IPL_INTERPOLATION_NEAREST, //!< nearest neighbor interpolation
    linear = PEAK_IPL_INTERPOLATION_LINEAR, //!< bilinear interpolation
    cubic = PEAK_IPL_INTERPOLATION_CUBIC, //!< bicubic interpolation
    area = PEAK_IPL_INTERPOLATION_AREA, //!< resampling using pixel area relation
    lanczos4 = PEAK_IPL_INTERPOLATION_LANCZOS4, //!< Lanczos interpolation over 8x8 neighborhood
    linear_exact = PEAK_IPL_INTERPOLATION_LINEAR_EXACT, //!< Bit-exact bilinear interpolation
    nearest_exact = PEAK_IPL_INTERPOLATION_NEAREST_EXACT //!< Bit-exact bicubic interpolation
};

/*!
 * \brief 2D size of an image.
 */
struct Size2D
{
    size_t width; //!< Width of size
    size_t height; //!< Height of size
};

/*!
 * \brief Compare two Size2D for equality
 */
PEAK_IPL_NO_DISCARD inline bool operator==(const Size2D& lhs, const Size2D& rhs)
{
    return lhs.height == rhs.height && lhs.width == rhs.width;
}

/*!
 * \brief Compare two Size2D for inequality
 */
PEAK_IPL_NO_DISCARD inline bool operator!=(const Size2D& lhs, const Size2D& rhs)
{
    return !(lhs == rhs);
}
/*!
 * \brief 2D position in an image.
 */
struct Point2D
{
    size_t x; //!< X position
    size_t y; //!< Y position
};

/*!
 * \brief Compare two Point2D for equality
 */
PEAK_IPL_NO_DISCARD inline bool operator==(const Point2D& lhs, const Point2D& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

/*!
 * \brief Compare two Point2D for inequality
 */
PEAK_IPL_NO_DISCARD inline bool operator!=(const Point2D& lhs, const Point2D& rhs)
{
    return !(lhs == rhs);
}

/*!
 * \brief Scale Factors of an image
 */
struct ScaleFactor
{
    double x; //!< X factor
    double y; //!< Y factor
};

/*!
 * \brief Convert a ConversionMode to a string
 * \param entry ConversionMode to convert.
 * \return String representation of \p entry.
 */
PEAK_IPL_NO_DISCARD inline std::string ToString(ConversionMode entry)
{
    switch (entry)
    {
    case ConversionMode::Fast:
        return "Fast";
    case ConversionMode::HighQuality:
        return "HighQuality";
    case ConversionMode::Classic:
        return "Classic";
    }

    return "INVALID CONVERSION MODE VALUE";
}

/*!
 * \brief Convert a PixelFormatName to a string
 * \param entry PixelFormatName to convert.
 * \return String representation of \p entry.
 */
PEAK_IPL_NO_DISCARD inline std::string ToString(PixelFormatName entry)
{
    switch (entry)
    {
    case PixelFormatName::Invalid:
        return "Invalid";
    case PixelFormatName::BayerGR8:
        return "BayerGR8";
    case PixelFormatName::BayerGR10:
        return "BayerGR10";
    case PixelFormatName::BayerGR12:
        return "BayerGR12";
    case PixelFormatName::BayerRG8:
        return "BayerRG8";
    case PixelFormatName::BayerRG10:
        return "BayerRG10";
    case PixelFormatName::BayerRG12:
        return "BayerRG12";
    case PixelFormatName::BayerGB8:
        return "BayerGB8";
    case PixelFormatName::BayerGB10:
        return "BayerGB10";
    case PixelFormatName::BayerGB12:
        return "BayerGB12";
    case PixelFormatName::BayerBG8:
        return "BayerBG8";
    case PixelFormatName::BayerBG10:
        return "BayerBG10";
    case PixelFormatName::BayerBG12:
        return "BayerBG12";
    case PixelFormatName::Mono8:
        return "Mono8";
    case PixelFormatName::Mono10:
        return "Mono10";
    case PixelFormatName::Mono12:
        return "Mono12";
    case PixelFormatName::YUV420_8_YY_UV_SemiplanarIDS:
        return "YUV420_8_YY_UV_SemiplanarIDS";
    case PixelFormatName::YUV420_8_YY_VU_SemiplanarIDS:
        return "YUV420_8_YY_VU_SemiplanarIDS";
    case PixelFormatName::RGB8:
        return "RGB8";
    case PixelFormatName::RGB10:
        return "RGB10";
    case PixelFormatName::RGB12:
        return "RGB12";
    case PixelFormatName::BGR8:
        return "BGR8";
    case PixelFormatName::BGR10:
        return "BGR10";
    case PixelFormatName::BGR12:
        return "BGR12";
    case PixelFormatName::RGBa8:
        return "RGBa8";
    case PixelFormatName::RGBa10:
        return "RGBa10";
    case PixelFormatName::RGBa12:
        return "RGBa12";
    case PixelFormatName::BGRa8:
        return "BGRa8";
    case PixelFormatName::BGRa10:
        return "BGRa10";
    case PixelFormatName::BGRa12:
        return "BGRa12";
    case PixelFormatName::BayerBG10p:
        return "BayerBG10p";
    case PixelFormatName::BayerBG12p:
        return "BayerBG12p";
    case PixelFormatName::BayerGB10p:
        return "BayerGB10p";
    case PixelFormatName::BayerGB12p:
        return "BayerGB12p";
    case PixelFormatName::BayerGR10p:
        return "BayerGR10p";
    case PixelFormatName::BayerGR12p:
        return "BayerGR12p";
    case PixelFormatName::BayerRG10p:
        return "BayerRG10p";
    case PixelFormatName::BayerRG12p:
        return "BayerRG12p";
    case PixelFormatName::Mono10p:
        return "Mono10p";
    case PixelFormatName::Mono12p:
        return "Mono12p";
    case PixelFormatName::RGB10p32:
        return "RGB10p32";
    case PixelFormatName::BGR10p32:
        return "BGR10p32";
    case PixelFormatName::BayerRG10g40IDS:
        return "BayerRG10g40IDS";
    case PixelFormatName::BayerGB10g40IDS:
        return "BayerGB10g40IDS";
    case PixelFormatName::BayerGR10g40IDS:
        return "BayerGR10g40IDS";
    case PixelFormatName::BayerBG10g40IDS:
        return "BayerBG10g40IDS";
    case PixelFormatName::BayerRG12g24IDS:
        return "BayerRG12g24IDS";
    case PixelFormatName::BayerGB12g24IDS:
        return "BayerGB12g24IDS";
    case PixelFormatName::BayerGR12g24IDS:
        return "BayerGR12g24IDS";
    case PixelFormatName::BayerBG12g24IDS:
        return "BayerBG12g24IDS";
    case PixelFormatName::Mono10g40IDS:
        return "Mono10g40IDS";
    case PixelFormatName::Mono12g24IDS:
        return "Mono12g24IDS";
    }

    return "INVALID PIXEL FORMAT VALUE";
}

/*!
 * \brief Convert an Endianness to a string
 * \param entry Endianness to convert.
 * \return String representation of \p entry.
 */
PEAK_IPL_NO_DISCARD inline std::string ToString(Endianness entry)
{
    switch (entry)
    {
    case Endianness::Unknown:
        return "Unknown";
    case Endianness::LittleEndian:
        return "LittleEndian";
    case Endianness::BigEndian:
        return "BigEndian";
    }

    return "INVALID ENDIANNESS VALUE";
}

/*!
 * \brief Convert an Orientation to a string
 * \param entry Orientation to convert.
 * \return String representation of \p entry.
 */
PEAK_IPL_NO_DISCARD inline std::string ToString(Orientation entry)
{
    switch (entry)
    {
    case Orientation::Horizontal:
        return "Horizontal";
    case Orientation::Vertical:
        return "Vertical";
    }

    return "INVALID ORIENTATION VALUE";
}

} /* namespace ipl */
} /* namespace peak */
