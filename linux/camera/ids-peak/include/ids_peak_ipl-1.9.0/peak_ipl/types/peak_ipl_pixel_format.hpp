/*!
 * \file    peak_ipl_pixel_format.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>

#include <cstddef>
#include <cstdint>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 * \brief Represents a pixel format and its specific properties.
 */
class PixelFormat final
{
public:
    /*! \brief Constructor for a new PixelFormat instance
     *
     * Creates a new PixelFormat instance. The PixelFormatName is initialized to #PixelFormatName::Invalid.
     *
     * \since 1.0
     */
    PixelFormat()
        : m_name(peak::ipl::PixelFormatName::Invalid)
    {}

    /*! \brief Constructor for a new PixelFormat instance
     *
     * Creates a new PixelFormat instance. The PixelFormatName is initialized to \p name.
     *
     * \param[in] name Used Pixel Format Name.
     *
     * \since 1.0
     */
    PixelFormat(peak::ipl::PixelFormatName name); // NOLINT (note: implicitly used by Image...)


    /*! \brief Destructor for the PixelFormat instance
     *
     * Destroys a PixelFormat instance.
     *
     * \since 1.0
     */
    ~PixelFormat() = default;

    /*! \brief Copy Constructor for a new PixelFormat instance
     *
     * Creates a new PixelFormat instance. The PixelFormatName is copied from \p other
     *
     * \param[in] other The other instance to copy the PixelFormatName from.
     *
     * \since 1.0
     */
    PixelFormat(PixelFormat&& other) = default;

    /*! \brief Move constructor for PixelFormat instance
     *
     * Moves the PixelFormatName from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move the PixelFormatName from.
     *
     * \since 1.0
     */
    PixelFormat(const PixelFormat& other) = default;

    /*! \brief Copy assigment for PixelFormat instance
     *
     * Copies the PixelFormatName from \p other to this instance.
     *
     * \param[in] other The other instance to copy the PixelFormatName from.
     *
     * \since 1.0
     */
    PixelFormat& operator=(const PixelFormat& other) = default;

    /*! \brief Move assignment for PixelFormat instance
     *
     * Moves the PixelFormatName from \p other to this.
     *
     * \param[in] other The other instance to move the PixelFormatName from.
     *
     * \since 1.0
     */
    PixelFormat& operator=(PixelFormat&& other) = default;

    /*!
     * \brief Returns the name of the pixel format as String.
     *
     * \returns Name
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD std::string Name() const
    {
        return peak::ipl::ToString(m_name);
    }

    /*!
     * \brief Returns the name of the pixel format as enum value.
     *
     * \returns PixelFormatName
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD peak::ipl::PixelFormatName PixelFormatName() const
    {
        return m_name;
    }

    /*! \brief Compare PixelFormat
     *
     * Compares the PixelFormatName from \p other to this instance
     *
     * \param[in] other The other instance to compare.
     *
     * \returns True if equal, false otherwise.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD bool operator==(const PixelFormat& other) const
    {
        return m_name == other.m_name;
    }

    /*! \brief Returns the number of significant bits per pixel per channel of the given pixel format.
     *
     * \returns Number of significant bits per pixel per channel.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD size_t NumSignificantBitsPerChannel() const;

    /*! \brief Returns the number of storage bits per pixel per channel of the given pixel format.
     *
     * \returns Number of storage bits per pixel per channel.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD size_t NumStorageBitsPerChannel() const;

    /*! \brief Returns the number of channels the given pixel format has.
     *
     * \returns Number of channels for pixel format.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD size_t NumChannels() const;

    /*!
     * \brief Returns the maximum value of one pixel channel the given pixel format has.
     *
     * \returns Maximum value of one pixel channel.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD uint32_t MaximumValuePerChannel() const;

    /*!
     * \brief Returns the number of significant bits per pixel the given pixel format has.
     *
     * \returns Number of significant bits used.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD size_t NumSignificantBitsPerPixel() const;

    /*!
     * \brief Returns the number of storage bits per pixel the given pixel format has.
     *
     * For unpacked formats, this is normally a multiple of 8.
     * For packed formats, this is normally the same as #NumSignificantBitsPerPixel.
     *
     * \returns Number of storage bits used.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD size_t NumStorageBitsPerPixel() const;


    /*! \brief Returns the endianness of the given pixel format.
     *
     * \returns The endianness of the pixel format.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD peak::ipl::Endianness Endianness() const;

    /*! \brief Returns the storage size the given number pixels of the given pixel format in bytes.
     *
     * For example, the calculation of the required buffer size, supply width multiplied by height here.
     *
     * \param[in]  numPixels   The number of pixels.
     *
     * \returns The calculated storage size for the number of pixels.
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD uint64_t CalculateStorageSizeOfPixels(uint64_t numPixels) const;

    /*! \brief Returns whether the pixel format is packed.
     *
     * \returns Whether the pixel format is packed. True for packed, false for unpacked.
     *
     * \since 1.7
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD bool IsPacked() const;

    /*! \brief Returns whether the pixel format is a bayer format.
     *
     * \returns Whether the pixel format is a bayer format. True for Bayer, false for all else.
     *
     * \since 1.7
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD bool IsBayered() const;

    /*! \brief Returns whether the pixel format is a yuv format.
     *
     * \returns Whether the pixel format is a yuv format. True for yuv, false for all else.
     *
     * \since 1.9
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD bool IsYUV() const;

    /*! \brief Queries the bayer pattern of the given pixel format.
     *
     * \returns The bayer pattern if the format is a bayer format. Or #BayerPattern::None if the format has no Bayer pattern.
     *
     * \since 1.8
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD peak::ipl::BayerPattern BayerPattern() const;

private:
    peak::ipl::PixelFormatName m_name{};
};

inline PixelFormat::PixelFormat(peak::ipl::PixelFormatName name)
    : m_name(name)
{}

inline size_t PixelFormat::NumSignificantBitsPerChannel() const
{
    size_t numSignificantBitsPerChannel = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), &numSignificantBitsPerChannel);
    });

    return numSignificantBitsPerChannel;
}

inline size_t PixelFormat::NumStorageBitsPerChannel() const
{
    size_t numStorageBitsPerChannel = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), &numStorageBitsPerChannel);
    });

    return numStorageBitsPerChannel;
}

inline size_t PixelFormat::NumChannels() const
{
    size_t numChannels = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetNumChannels(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), &numChannels);
    });

    return numChannels;
}

inline uint32_t PixelFormat::MaximumValuePerChannel() const
{
    uint32_t maximumValuePerChannel = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetMaximumValuePerChannel(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), &maximumValuePerChannel);
    });

    return maximumValuePerChannel;
}

inline size_t PixelFormat::NumSignificantBitsPerPixel() const
{
    size_t numSignificantBitsPerPixel = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), &numSignificantBitsPerPixel);
    });

    return numSignificantBitsPerPixel;
}

inline size_t PixelFormat::NumStorageBitsPerPixel() const
{
    size_t numStorageBitsPerPixel = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), &numStorageBitsPerPixel);
    });

    return numStorageBitsPerPixel;
}

inline peak::ipl::Endianness PixelFormat::Endianness() const
{
    peak::ipl::Endianness endianness = peak::ipl::Endianness::LittleEndian;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetEndianness(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), reinterpret_cast<PEAK_IPL_ENDIANNESS*>(&endianness));
    });

    return endianness;
}

inline uint64_t PixelFormat::CalculateStorageSizeOfPixels(uint64_t numPixels) const
{
    uint64_t sizeOfPixels = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), numPixels, &sizeOfPixels);
    });

    return sizeOfPixels;
}

inline bool PixelFormat::IsPacked() const
{
    bool isPacked = false;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_IsPacked(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), reinterpret_cast<PEAK_IPL_BOOL8*>(&isPacked));
    });

    return isPacked;
}

inline bool PixelFormat::IsBayered() const
{
    bool isPacked = false;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_IsBayered(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), reinterpret_cast<PEAK_IPL_BOOL8*>(&isPacked));
    });

    return isPacked;
}

inline bool PixelFormat::IsYUV() const
{
    auto result{ false };

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_IsYUV(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), reinterpret_cast<PEAK_IPL_BOOL8*>(&result));
    });

    return result;
}

inline peak::ipl::BayerPattern PixelFormat::BayerPattern() const
{
    peak::ipl::BayerPattern pattern = peak::ipl::BayerPattern::Unknown;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelFormat_GetBayerPattern(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(m_name), reinterpret_cast<PEAK_IPL_BAYER_PATTERN*>(&pattern));
    });

    return pattern;
}

} /* namespace ipl */
} /* namespace peak */
