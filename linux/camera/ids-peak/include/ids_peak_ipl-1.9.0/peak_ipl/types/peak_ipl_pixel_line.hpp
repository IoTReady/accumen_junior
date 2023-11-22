/*!
 * \file    peak_ipl_pixel_line.hpp
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
#include <peak_ipl/types/peak_ipl_image.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 * \brief Represents the values of a horizontal or vertical line of pixels in an image.
 */
class PixelLine
{
public:
    /*!
     * \brief Represents the values of the pixel line.
     */
    struct Channel
    {
        std::vector<uint32_t> Values;
    };

public:
    PixelLine() = delete;
    
    /*! \brief Constructor.
     *
     * \param[in] image The image to process.
     * \param[in] orientation The orientation of the pixel line.
     * \param[in] offset Y offset if orientation = peak::ipl::Orientation::Horizontal; X offset if orientation = peak::ipl::Orientation::Vertical
     *
     * \throws ImageFormatNotSupportedException image has a packed pixel format
     *
     * \since 1.0
     * \since 1.2 Will throw ImageFormatNotSupportedException if image has a packed pixel format
     */
    explicit PixelLine(const Image& image, peak::ipl::Orientation orientation, size_t offset);
    
    /*! \brief Destructor for the PixelLine instance
     *
     * Destroys a PixelLine instance.
     *
     * \since 1.0
     */
    virtual ~PixelLine();
    
    /*! \brief Copy Constructor for a new PixelLine instance
     *
     * Creates a new PixelLine instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.0 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    PixelLine(const PixelLine& other);

    /*! \brief Copy assigment for PixelLine instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.0 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    PixelLine& operator=(const PixelLine& other);

    /*! \brief Move constructor for PixelLine instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    PixelLine(PixelLine&& other) noexcept;

    /*! \brief Move assignment for PixelLine instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    PixelLine& operator=(PixelLine&& other) noexcept;

    /*! \brief Returns the pixel format of the given pixel line.
     *
     * \returns PixelFormatName
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD peak::ipl::PixelFormatName PixelFormatName() const;

    /*! \brief Returns the orientation of the given pixel line.
     *
     * \returns Orientation
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD peak::ipl::Orientation Orientation() const;

    /*! \brief Returns the offset of the given pixel line.
     * (Vertical: Left - Horizontal: Top).
     *
     * \returns Offset
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD size_t Offset() const;

    /*! \brief Returns the number of the given pixel line's channels.
     *
     * \returns Channels
     *
     * \since 1.0
     *
     * \throws Exception An internal error has occurred.
     */
    PEAK_IPL_NO_DISCARD std::vector<Channel> Channels() const;

private:
    PEAK_IPL_PIXEL_LINE_HANDLE m_backendHandle{};
};

/*!
 * \brief Represents the values for a horizontal line of pixels in an image.
 */
class PixelRow : public PixelLine
{
public:
    PixelRow() = delete;
    explicit PixelRow(const Image& image, size_t row);
    ~PixelRow() override = default;
    PixelRow(const PixelRow& other) = default;
    PixelRow& operator=(const PixelRow& other) = default;
    PixelRow(PixelRow&& other) = default;
    PixelRow& operator=(PixelRow&& other) = default;
};

/*!
 * \brief Represents the values for a vertical line of pixels in an image.
 */
class PixelColumn : public PixelLine
{
public:
    PixelColumn() = delete;
    explicit PixelColumn(const Image& image, size_t column);
    ~PixelColumn() override = default;
    PixelColumn(const PixelColumn& other) = default;
    PixelColumn& operator=(const PixelColumn& other) = default;
    PixelColumn(PixelColumn&& other) = default;
    PixelColumn& operator=(PixelColumn&& other) = default;
};

inline PixelRow::PixelRow(const Image& image, size_t row)
    : PixelLine(image, peak::ipl::Orientation::Horizontal, row)
{}

inline PixelColumn::PixelColumn(const Image& image, size_t column)
    : PixelLine(image, peak::ipl::Orientation::Vertical, column)
{}


inline PixelLine::PixelLine(const Image& image, peak::ipl::Orientation orientation, size_t offset)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_Construct(ImageBackendAccessor::BackendHandle(image),
            static_cast<PEAK_IPL_ORIENTATION>(orientation), offset, &m_backendHandle);
    });
}

inline PixelLine::~PixelLine()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_Destruct(m_backendHandle);
    }
}

inline PixelLine::PixelLine(const PixelLine& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline PixelLine& PixelLine::operator=(const PixelLine& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline PixelLine::PixelLine(PixelLine&& other) noexcept
{
    *this = std::move(other);
}

inline PixelLine& PixelLine::operator=(PixelLine&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline peak::ipl::PixelFormatName PixelLine::PixelFormatName() const
{
    peak::ipl::PixelFormatName pixelFormatName = peak::ipl::PixelFormatName::Invalid;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_GetPixelFormat(
            m_backendHandle, reinterpret_cast<PEAK_IPL_PIXEL_FORMAT*>(&pixelFormatName));
    });

    return pixelFormatName;
}

inline peak::ipl::Orientation PixelLine::Orientation() const
{
    peak::ipl::Orientation orientation = peak::ipl::Orientation::Horizontal;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_GetOrientation(
            m_backendHandle, reinterpret_cast<PEAK_IPL_ORIENTATION*>(&orientation));
    });

    return orientation;
}

inline size_t PixelLine::Offset() const
{
    size_t offset = 0;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_GetOffset(m_backendHandle, &offset); });

    return offset;
}

inline std::vector<PixelLine::Channel> PixelLine::Channels() const
{
    std::vector<Channel> channels;

    size_t numChannels = 0;
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_GetNumChannels(m_backendHandle, &numChannels); });

    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        size_t valueListSize = 0;
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_GetValuesForChannel(
                m_backendHandle, ch, nullptr, &valueListSize);
        });
        std::vector<uint32_t> valueList(valueListSize);
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_PixelLine_GetValuesForChannel(
                m_backendHandle, ch, valueList.data(), &valueListSize);
        });

        channels.emplace_back(Channel{ std::move(valueList) });
    }

    return channels;
}

} /* namespace ipl */
} /* namespace peak */
