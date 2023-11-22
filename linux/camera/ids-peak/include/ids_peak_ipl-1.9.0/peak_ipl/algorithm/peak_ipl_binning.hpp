/*!
 * \file    peak_ipl_binning.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-03-17
 * \since   1.3.3
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

class Binning final
{
public:
    /*!
     * \brief Mode parameter for the binning algorithm.
     *
     * The enum holding the possible modi.
     */
    enum class BinningMode : uint16_t
    {
        //! Average (mean) of pixels
        Average = 0,

        //! Sum of pixels
        Sum = 1
    };

    /*! \brief Constructor for a new Binning instance
     *
     * Creates a new Binning instance.
     *
     * \since 1.3.3
     */
    Binning();

    /*! \brief Destructor for the Binning instance
     *
     * Destroys a Binning instance.
     *
     * \since 1.3.3
     */
    ~Binning();

    /*! \brief Copy Constructor for a new Binning instance
     *
     * Creates a new Binning instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.3.3 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    Binning(const Binning& other);

    /*! \brief Copy assigment for Binning instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.3.3 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    Binning& operator=(const Binning& other);

    /*! \brief Move constructor for Binning instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move.
     *
     * \since 1.3.3
     */
    Binning(Binning&& other) noexcept;

    /*! \brief Move assignment for Binning instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move.
     *
     * \since 1.3.3
     */
    Binning& operator=(Binning&& other) noexcept;

    /*!
     * \brief Sets the values of the binning in horizontal direction.
     *
     * \param[in] value The value of the binning in horizontal direction.
     *
     * \throws InvalidArgument value isn't supported.
     *
     * \since 1.3.3
     */
    void SetBinningX(uint8_t value);

    /*!
     * \brief Returns the value of the binning in horizontal direction.
     *
     * \returns The value of the binning in horizontal direction.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD uint8_t BinningX() const;

    /*!
     * \brief Sets the values of the binning in vertical direction.
     *
     * \param[in] value The value of the binning in vertical direction.
     *
     * \throws InvalidArgument value isn't supported.
     *
     * \since 1.3.3
     */
    void SetBinningY(uint8_t value);

    /*!
     * \brief Returns the value of the binning in vertical direction.
     *
     * \returns The value of the binning in vertical direction.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD uint8_t BinningY() const;

    /*!
     * \brief Returns the minimum value for the binning.
     *
     * \returns The minimum value for the binning.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD uint8_t BinningMin() const;

    /*!
     * \brief Returns the maximum value for the binning.
     *
     * \returns The maximum value for the binning.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD uint8_t BinningMax() const;

    /*!
     * \brief Returns the size of a binned image given the input size and pixelformat.
     *
     * \param[in] inputSize                         The size of the input image.
     * \param[in] pixelFormat                       The pixelFormat of the input image.
     *
     * \returns The size of a binned image
     *
     * \throws InvalidArgument pixelFormat isn't supported.
     *
     * \since 1.3.4
     */
    PEAK_IPL_NO_DISCARD Size2D ExpectedOutputSize(Size2D inputSize, PixelFormatName pixelFormat) const;

    /*!
     * \brief Sets the values of the binning mode.
     *
     * \param[in] mode The value of the binning mode.
     *
     * \throws InvalidArgument value isn't supported.
     *
     * \since 1.3.3
     */
    void SetMode(BinningMode mode);

    /*!
     * \brief Returns the value of the binning mode.
     *
     * \returns The value of the binning mode.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD BinningMode Mode() const;

    /*!
     * \brief Returns whether the binning supports the given pixel format.
     *
     * \returns Flag whether the given pixel format is supported.
     *
     * \param[in] pixelFormatName The pixel format of interest.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD bool IsPixelFormatSupported(PixelFormatName pixelFormatName) const;

    /*! \brief Applies the binning on the given image.
     *
     * \param[in] inputImage Image to process.
     *
     * \returns A new created image containing the gamma-corrected pixels.
     *
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     * \throws InvalidArgument the image is too small.
     *
     * \since 1.3.3
     */
    PEAK_IPL_NO_DISCARD Image Process(const Image& inputImage) const;

private:
    PEAK_IPL_BINNING_HANDLE m_backendHandle{};
};

inline Binning::Binning()
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Construct(&m_backendHandle); });
}

inline Binning::~Binning()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Destruct(m_backendHandle);
    }
}

inline Binning::Binning(const Binning& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline Binning& Binning::operator=(const Binning& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline Binning::Binning(Binning&& other) noexcept
{
    *this = std::move(other);
}

inline Binning& Binning::operator=(Binning&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline void Binning::SetBinningX(uint8_t value)
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_SetBinningX(m_backendHandle, &value); });
}

inline void Binning::SetBinningY(uint8_t value)
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_SetBinningY(m_backendHandle, &value); });
}

inline uint8_t Binning::BinningX() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_GetBinningX(m_backendHandle, &value); });

    return value;
}

inline uint8_t Binning::BinningY() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_GetBinningY(m_backendHandle, &value); });

    return value;
}

inline uint8_t Binning::BinningMin() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_GetBinningMin(m_backendHandle, &value); });

    return value;
}

inline uint8_t Binning::BinningMax() const
{
    uint8_t value;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_GetBinningMax(m_backendHandle, &value); });

    return value;
}

inline Size2D Binning::ExpectedOutputSize(Size2D inputSize, PixelFormatName pixelFormat) const
{
    Size2D size{};
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_ExpectedOutputSize(m_backendHandle, &size.width, &size.height,
            inputSize.width, inputSize.height, static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormat));
    });

    return size;
}

inline bool Binning::IsPixelFormatSupported(PixelFormatName pixelFormatName) const
{
    PEAK_IPL_BOOL8 isPixelFormatSupported = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_IsPixelformatSupported(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormatName),
            reinterpret_cast<PEAK_IPL_BOOL8*>(&isPixelFormatSupported));
    });

    return isPixelFormatSupported > 0;
}

inline Image Binning::Process(const Image& inputImage) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_Process(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline void Binning::SetMode(Binning::BinningMode mode)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_SetBinningMode(
            m_backendHandle, reinterpret_cast<PEAK_IPL_BINNINGMODE*>(&mode));
    });
}

inline Binning::BinningMode Binning::Mode() const
{
    BinningMode value;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Binning_GetBinningMode(
            m_backendHandle, reinterpret_cast<PEAK_IPL_BINNINGMODE*>(&value));
    });

    return value;
}


} /* namespace ipl */
} /* namespace peak */
