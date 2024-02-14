/*!
 * \file    peak_ipl_histogram.hpp
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
 * \brief Stores the histogram values.
 */
class Histogram final
{
public:
    /*!
     * \brief Stores the values of the histogram.
     */
    struct HistogramChannel
    {
        uint64_t PixelSum;
        uint64_t PixelCount;
        std::vector<uint64_t> Bins;
    };

public:
    Histogram() = delete;
    /*!
     * \brief Constructor.
     *
     * \param[in] image Image to process.
     *
     * \throws ImageFormatNotSupportedException image has a packed pixel format
     *
     * \since 1.0
     * \since 1.2 Will throw ImageFormatNotSupportedException if image has a packed pixel format
     */
    explicit Histogram(const Image& image);
    
    /*! \brief Destructor for the Histogram instance
     *
     * Destroys a Histogram instance.
     *
     * \since 1.0
     */
    ~Histogram();

    /*! \brief Copy Constructor for a new Histogram instance
     *
     * Creates a new Histogram instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.0 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    Histogram(const Histogram& other);

    /*! \brief Copy assigment for Histogram instance
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
    Histogram& operator=(const Histogram& other);

    /*! \brief Move constructor for Histogram instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    Histogram(Histogram&& other) noexcept;

    /*! \brief Move assignment for Histogram instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    Histogram& operator=(Histogram&& other) noexcept;
    
    /*!
     * \brief Returns the pixel format of the histogram.
     *
     * \returns PixelFormat
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD peak::ipl::PixelFormat PixelFormat() const;

    /*!
     * \brief Returns a vector containing the bin list of each channel.
     *
     * \returns Channels
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD std::vector<HistogramChannel> Channels() const;

private:
    PEAK_IPL_HISTOGRAM_HANDLE m_backendHandle{};
};

inline Histogram::Histogram(const Image& image)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_Construct(
            ImageBackendAccessor::BackendHandle(image), &m_backendHandle);
    });
}

inline Histogram::~Histogram()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_Destruct(m_backendHandle);
    }
}

inline Histogram::Histogram(const Histogram& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline Histogram& Histogram::operator=(const Histogram& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline Histogram::Histogram(Histogram&& other) noexcept
{
    *this = std::move(other);
}

inline Histogram& Histogram::operator=(Histogram&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline peak::ipl::PixelFormat Histogram::PixelFormat() const
{
    PixelFormatName pixelFormatName = PixelFormatName::Invalid;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_GetPixelFormat(
            m_backendHandle, reinterpret_cast<PEAK_IPL_PIXEL_FORMAT*>(&pixelFormatName));
    });

    return peak::ipl::PixelFormat{ pixelFormatName };
}

inline std::vector<Histogram::HistogramChannel> Histogram::Channels() const
{
    std::vector<HistogramChannel> channels;

    size_t numChannels = 0;
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_GetNumChannels(m_backendHandle, &numChannels); });

    for (size_t ch = 0; ch < numChannels; ++ch)
    {
        uint64_t pixelSum = 0;
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_GetPixelSumForChannel(m_backendHandle, ch, &pixelSum);
        });

        uint64_t pixelCount = 0;
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_GetPixelCountForChannel(m_backendHandle, ch, &pixelCount);
        });

        size_t binListSize = 0;
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_GetBinsForChannel(m_backendHandle, ch, nullptr, &binListSize);
        });
        std::vector<uint64_t> binList(binListSize);
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Histogram_GetBinsForChannel(
                m_backendHandle, ch, binList.data(), &binListSize);
        });

        channels.emplace_back(HistogramChannel{ pixelSum, pixelCount, std::move(binList) });
    }

    return channels;
}

} /* namespace ipl */
} /* namespace peak */
