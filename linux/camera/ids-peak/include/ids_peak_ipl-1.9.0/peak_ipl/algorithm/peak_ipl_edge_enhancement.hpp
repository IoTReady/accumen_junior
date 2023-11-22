/*!
 * \file    peak_ipl_edge_enhancement.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-01-30
 * \since   1.7
 *
 * Copyright (c) 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>
#include <peak_ipl/types/peak_ipl_range.hpp>
#include <peak_ipl/types/peak_ipl_image.hpp>

#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <vector>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{
class Image;
enum class PixelFormatName;
/*!
 * \brief Edge Enhancement for a given image.
 *
 * This class uses a software filter that emphasizes the edges in the image. The enhancement factor is configurable.
 */
class EdgeEnhancement final
{
public:
    /*! \brief Constructor for a new EdgeEnhancement instance
     *
     * Creates a new EdgeEnhancement instance.
     *
     * \since 1.7
     */
    EdgeEnhancement();
    
    /*! \brief Destructor for the EdgeEnhancement instance
     *
     * Destroys an EdgeEnhancement instance.
     *
     * \since 1.7
     */
    ~EdgeEnhancement();

    /*! \brief Copy Constructor for a new EdgeEnhancement instance
     *
     * Creates a new EdgeEnhancement instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.7 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    EdgeEnhancement(const EdgeEnhancement& other);

    /*! \brief Copy assigment for EdgeEnhancement instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     * 
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.7 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    EdgeEnhancement& operator=(const EdgeEnhancement& other);

    /*! \brief Move constructor for EdgeEnhancement instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.7
     */
    EdgeEnhancement(EdgeEnhancement&& other) noexcept;

    /*! \brief Move assignment for EdgeEnhancement instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.7
     */
    EdgeEnhancement& operator=(EdgeEnhancement&& other) noexcept;

    /*!
     * \brief Process an image
     *
     * To check if it is supported, call #EdgeEnhancement::IsPixelFormatSupported first
     * This function causes a higher CPU load.
     *
     * \param[in] image Image to process.
     *
     * \throws InvalidPixelFormat pixel format isn't supported.
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    inline void ProcessInPlace(Image& image) const;

    /*!
     * \brief Process an image
     *
     * To check if it is supported, call #EdgeEnhancement::IsPixelFormatSupported first
     * This function causes a higher CPU load.
     *
     * \param[in] image Image to process.
     *
     * \returns the sharpened image
     *
     * \throws InvalidPixelFormat pixel format isn't supported.
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD inline Image Process(const Image& image) const;

    /*!
     * \brief Check if the pixel format is supported
     *
     * \note Is currently only implemented for not packed and not raw pixel formats
     *
     * \param[in] pixelFormatName The PixelFormatName to check support for.
     *
     * \returns true if pixelformat is supported, else false
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD inline bool IsPixelFormatSupported(PixelFormatName pixelFormatName) const;

    /*!
     * \brief Set the edge enhancement factor
     *
     * \param[in] factor The factor to set.
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    inline void SetFactor(std::uint32_t factor);

    /*!
     * \brief Get the edge enhancement factor
     *
     * \returns the current edge enhancement factor
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD inline std::uint32_t Factor() const;

    /*!
     * \brief Get the default edge enhancement factor
     *
     * \returns the default edge enhancement factor
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD inline std::uint32_t DefaultFactor() const;

    /*!
     * \brief Get the range for the edge enhancement factor
     *
     * \returns the range for the edge enhancement factor
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD inline peak::ipl::Range<std::uint32_t> Range() const;

private:
    PEAK_IPL_EDGE_ENHANCEMENT_HANDLE m_backendHandle{};
};

inline EdgeEnhancement::EdgeEnhancement()
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Construct(&m_backendHandle); });
}

inline EdgeEnhancement::~EdgeEnhancement()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Destruct(m_backendHandle);
    }
}

inline EdgeEnhancement::EdgeEnhancement(EdgeEnhancement&& other) noexcept
{
    *this = std::move(other);
}

inline EdgeEnhancement::EdgeEnhancement(const EdgeEnhancement& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline EdgeEnhancement& EdgeEnhancement::operator=(const EdgeEnhancement& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline EdgeEnhancement& EdgeEnhancement::operator=(EdgeEnhancement&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline void EdgeEnhancement::ProcessInPlace(Image& image) const
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_ProcessInPlace(
            m_backendHandle, ImageBackendAccessor::BackendHandle(image));
    });
}

inline Image EdgeEnhancement::Process(const Image& image) const
{
    PEAK_IPL_IMAGE_HANDLE handle{};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_Process(
            m_backendHandle, ImageBackendAccessor::BackendHandle(image), &handle);
    });

    return ImageBackendAccessor::CreateImage(handle);
}

inline bool EdgeEnhancement::IsPixelFormatSupported(PixelFormatName pixelFormatName) const
{
    PEAK_IPL_BOOL8 isPixelFormatSupported = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormatName), &isPixelFormatSupported);
    });

    return isPixelFormatSupported != 0;
}

inline void EdgeEnhancement::SetFactor(std::uint32_t factor)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_SetFactor(
            m_backendHandle, factor);
    });
}

inline uint32_t EdgeEnhancement::Factor() const
{
    uint32_t factor{};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_GetFactor(
            m_backendHandle, &factor);
    });

    return factor;
}

inline uint32_t EdgeEnhancement::DefaultFactor() const
{
    uint32_t factor{};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_GetDefaultFactor(
            m_backendHandle, &factor);
    });

    return factor;
}

inline Range<std::uint32_t> EdgeEnhancement::Range() const
{
    PEAK_IPL_RANGE range{};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_EdgeEnhancement_GetFactorRange(
            m_backendHandle, &range);
    });

    return { range.min, range.max, range.inc };
}

} /* namespace ipl */
} /* namespace peak */
