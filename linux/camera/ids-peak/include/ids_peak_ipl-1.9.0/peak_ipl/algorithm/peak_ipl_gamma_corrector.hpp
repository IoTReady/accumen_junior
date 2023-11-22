/*!
 * \file    peak_ipl_gamma_corrector.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2021-03-10
 * \since   1.2.2
 *
 * Copyright (c) 2021 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
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

class Image;

/*!
 * \brief Applies gamma correction to the data.
 */
class GammaCorrector final
{
public:
    /*! \brief Constructor for a new GammaCorrector instance
     *
     * Creates a new GammaCorrector instance.
     *
     * \since 1.2.2
     */
    GammaCorrector();
    
    /*! \brief Destructor for the GammaCorrector instance
     *
     * Destroys a GammaCorrector instance.
     *
     * \since 1.2.2
     */
    ~GammaCorrector();

    /*! \brief Copy Constructor for a new GammaCorrector instance
     *
     * Creates a new GammaCorrector instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.2.2 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    GammaCorrector(const GammaCorrector& other);

    /*! \brief Copy assigment for GammaCorrector instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     * 
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.2.2 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    GammaCorrector& operator=(const GammaCorrector& other);

    /*! \brief Move constructor for GammaCorrector instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.2.2
     */
    GammaCorrector(GammaCorrector&& other) noexcept;

    /*! \brief Move assignment for GammaCorrector instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.2.2
     */
    GammaCorrector& operator=(GammaCorrector&& other) noexcept;

    /*!
     * \brief Sets the values of the gamma correction.
     *
     * \param[in] gammaValue The value of the gamma correction.
     *
     * \throws InvalidArgument gammaValue isn't supported.
     *
     * \since 1.2.2
     */
    void SetGammaCorrectionValue(float gammaValue);

    /*!
     * \brief Returns the value of the gamma correction.
     *
     * \returns gammaValue The value of the gamma correction.
     *
     * \since 1.2.2
     */
    PEAK_IPL_NO_DISCARD float GammaCorrectionValue() const;

    /*!
     * \brief Returns the minimum value of the gamma correction.
     *
     * \returns gammaMin The minimum value of the gamma correction.
     *
     * \since 1.2.2
     */
    PEAK_IPL_NO_DISCARD float GammaCorrectionMin() const;

    /*!
     * \brief Returns the maximum value of the gamma correction.
     *
     * \returns gammaMax The maximum value of the gamma correction.
     *
     * \since 1.2.2
     */
    PEAK_IPL_NO_DISCARD float GammaCorrectionMax() const;

    /*!
     * \brief Returns whether the gamma corrector supports the given pixel format.
     *
     * \returns Flag whether the given pixel format is supported.
     *
     * \param[in] pixelFormatName The pixel format of interest.
     *
     * \since 1.2.2
     */
    PEAK_IPL_NO_DISCARD bool IsPixelFormatSupported(PixelFormatName pixelFormatName) const;

    /*!
     * \brief Corrects the gammas of the given image in place that is, it will change the input image.
     *
     * \param[in] image Image to process.
     *
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     *
     * \since 1.2.2
     */
    void ProcessInPlace(Image& image) const;

    /*! \brief Corrects the gammas of the given image.
     *
     * \param[in] inputImage Image to process.
     *
     * \returns A new created image containing the gamma-corrected pixels.
     *
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     *
     * \since 1.2.2
     */
    PEAK_IPL_NO_DISCARD Image Process(const Image& inputImage) const;

private:
    PEAK_IPL_GAMMA_CORRECTOR_HANDLE m_backendHandle{};
};

inline GammaCorrector::GammaCorrector()
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Construct(&m_backendHandle); });
}

inline GammaCorrector::~GammaCorrector()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Destruct(m_backendHandle);
    }
}

inline GammaCorrector::GammaCorrector(GammaCorrector&& other) noexcept
{
    *this = std::move(other);
}

inline GammaCorrector::GammaCorrector(const GammaCorrector& other)
{


    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline GammaCorrector& GammaCorrector::operator=(const GammaCorrector& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline GammaCorrector& GammaCorrector::operator=(GammaCorrector&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Destruct(m_backendHandle);
        }
        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline void GammaCorrector::SetGammaCorrectionValue(float gammaValue)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_SetGammaCorrectionValue(
            m_backendHandle, reinterpret_cast<float*>(&gammaValue));
    });
}

inline float GammaCorrector::GammaCorrectionValue() const
{
    float gammaValue;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_GetGammaCorrectionValue(
            m_backendHandle, reinterpret_cast<float*>(&gammaValue));
    });

    return gammaValue;
}

inline float GammaCorrector::GammaCorrectionMin() const
{
    float gammaMin;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_GetGammaCorrectionMin(
            m_backendHandle, reinterpret_cast<float*>(&gammaMin));
    });

    return gammaMin;
}

inline float GammaCorrector::GammaCorrectionMax() const
{
    float gammaMax;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_GetGammaCorrectionMax(
            m_backendHandle, reinterpret_cast<float*>(&gammaMax));
    });

    return gammaMax;
}

inline bool GammaCorrector::IsPixelFormatSupported(PixelFormatName pixelFormatName) const
{
    PEAK_IPL_BOOL8 isPixelFormatSupported = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormatName),
            reinterpret_cast<PEAK_IPL_BOOL8*>(&isPixelFormatSupported));
    });

    return isPixelFormatSupported > 0;
}

inline void GammaCorrector::ProcessInPlace(Image& image) const
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_ProcessInPlace(
            m_backendHandle, ImageBackendAccessor::BackendHandle(image));
    });
}

inline Image GammaCorrector::Process(const Image& inputImage) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_GammaCorrector_Process(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

} /* namespace ipl */
} /* namespace peak */
