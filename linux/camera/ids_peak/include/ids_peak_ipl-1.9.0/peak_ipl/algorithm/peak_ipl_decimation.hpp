/*!
 * \file    peak_ipl_decimation.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-08-18
 * \since   1.6
 *
 * Copyright (c) 2022 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_image.hpp>
#include <peak_ipl/types/peak_ipl_pixel_format.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

class Decimation final
{
public:
    /*! \brief Constructor for a new Decimation instance
     *
     * Creates a new Decimation instance.
     *
     * \since 1.6.0
     */
    Decimation();
    
    /*! \brief Destructor for the Decimation instance
     *
     * Destroys a Decimation instance.
     *
     * \since 1.6.0
     */
    ~Decimation();

    /*! \brief Copy Constructor for a new Decimation instance
     *
     * Creates a new Decimation instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.6.0 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    Decimation(const Decimation& other);

    /*! \brief Copy assigment for Decimation instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     * 
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.6.0 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    Decimation& operator=(const Decimation& other);

    /*! \brief Move constructor for Decimation instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.6.0
     */
    Decimation(Decimation&& other) noexcept;

    /*! \brief Move assignment for Decimation instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.6.0
     */
    Decimation& operator=(Decimation&& other) noexcept;

    /*!
     * \brief Sets the values of the decimation in horizontal direction.
     *
     * \param[in] value The value of the decimation in horizontal direction.
     *
     * \throws InvalidArgument value isn't supported.
     *
     * \since 1.6.0
     */
    void SetDecimationX(uint8_t value);

    /*!
     * \brief Returns the value of the decimation in horizontal direction.
     *
     * \returns The value of the decimation in horizontal direction.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD uint8_t DecimationX() const;

    /*!
     * \brief Sets the values of the decimation in vertical direction.
     *
     * \param[in] value The value of the decimation in vertical direction.
     *
     * \throws InvalidArgument value isn't supported.
     *
     * \since 1.6.0
     */
    void SetDecimationY(uint8_t value);

    /*!
     * \brief Returns the value of the decimation in vertical direction.
     *
     * \returns The value of the decimation in vertical direction.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD uint8_t DecimationY() const;

    /*!
     * \brief Returns the minimum value for the decimation.
     *
     * \returns The minimum value for the decimation.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD uint8_t DecimationMin() const;
    /*!
     * \brief Returns the maximum value for the decimation.
     *
     * \returns The maximum value for the decimation.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD uint8_t DecimationMax() const;

    /*!
     * \brief Returns the size of a decimated image given the input size and pixelformat.
     *
     * \param[in] inputSize                         The size of the input image.
     * \param[in] pixelFormat                       The pixelFormat of the input image.
     *
     * \returns The size of a decimated image
     *
     * \throws InvalidArgument pixelFormat isn't supported.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD Size2D ExpectedOutputSize(Size2D inputSize, PixelFormatName pixelFormat) const;

    /*!
     * \brief Returns whether the decimation supports the given pixel format.
     *
     * \returns Flag whether the given pixel format is supported.
     *
     * \param[in] pixelFormatName The pixel format of interest.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD bool IsPixelFormatSupported(PixelFormatName pixelFormatName) const;

    /*! \brief Applies the decimation on the given image.
     *
     * \param[in] inputImage Image to process.
     *
     * \returns A new created image containing the decimated pixels.
     *
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     * \throws InvalidArgument the image is too small.
     *
     * \since 1.6.0
     */
    PEAK_IPL_NO_DISCARD Image Process(const Image& inputImage) const;

private:
    PEAK_IPL_DECIMATION_HANDLE m_backendHandle{};
};

inline Decimation::Decimation()
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Construct(&m_backendHandle); });
}

inline Decimation::~Decimation()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Destruct(m_backendHandle);
    }
}

inline Decimation::Decimation(const Decimation& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline Decimation& Decimation::operator=(const Decimation& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline Decimation::Decimation(Decimation&& other) noexcept
{
    *this = std::move(other);
}

inline Decimation& Decimation::operator=(Decimation&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline void Decimation::SetDecimationX(uint8_t value)
{
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_SetDecimationX(m_backendHandle, &value); });
}

inline void Decimation::SetDecimationY(uint8_t value)
{
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_SetDecimationY(m_backendHandle, &value); });
}

inline uint8_t Decimation::DecimationX() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_GetDecimationX(m_backendHandle, &value); });

    return value;
}

inline uint8_t Decimation::DecimationY() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_GetDecimationY(m_backendHandle, &value); });

    return value;
}

inline uint8_t Decimation::DecimationMin() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_GetDecimationMin(m_backendHandle, &value); });

    return value;
}

inline uint8_t Decimation::DecimationMax() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_GetDecimationMax(m_backendHandle, &value); });

    return value;
}

inline Size2D Decimation::ExpectedOutputSize(Size2D inputSize, PixelFormatName pixelFormat) const
{
    Size2D size{};
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_ExpectedOutputSize(m_backendHandle, &size.width, &size.height,
            inputSize.width, inputSize.height, static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormat));
    });

    return size;
}

inline bool Decimation::IsPixelFormatSupported(PixelFormatName pixelFormatName) const
{
    PEAK_IPL_BOOL8 isPixelFormatSupported = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_IsPixelformatSupported(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormatName),
            reinterpret_cast<PEAK_IPL_BOOL8*>(&isPixelFormatSupported));
    });

    return isPixelFormatSupported > 0;
}

inline Image Decimation::Process(const Image& inputImage) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Decimation_Process(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

} /* namespace ipl */
} /* namespace peak */
