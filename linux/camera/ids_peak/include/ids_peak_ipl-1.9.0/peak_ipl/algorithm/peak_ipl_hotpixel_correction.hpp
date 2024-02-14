/*!
 * \file    peak_ipl_hotpixel_correction.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>

#include <cstdint>
#include <vector>

namespace peak
{
namespace ipl
{

class Image;
struct Point2D;



/*!
 * \brief Algorithm for (adaptive) hotpixel detection and correction.
 *
 * This algorithm corrects hotpixels either manually by correcting a predefined list of pixels or adaptively by
 * automatically updating the list of pixels to correct. In manual mode, pass a list of hotpixels to Correct(). The
 * hotpixels can be obtained using Detect(), or from a different source, e.g. retrieved directly from the camera from an
 * initial hotpixel calibration. In adaptive mode, just keep passing new images to CorrectAdaptive(). Hotpixels are then
 * detected and corrected automatically.
 */
class HotpixelCorrection final
{
public:
    /*!
     * \brief SensitivityLevel parameter for the HotpixelCorrection algorithm.
     *
     * Higher sensitivity levels mean more hotpixels will be detected and corrected, but can also lead to more
     * false-positives.
     */
    enum class SensitivityLevel
    {
        Invalid,
        SensitivityLevel1,
        SensitivityLevel2,
        SensitivityLevel3, //!< default
        SensitivityLevel4,
        SensitivityLevel5,
    };

    /*! \brief Constructor for a new HotpixelCorrection instance
     *
     * Creates a new HotpixelCorrection instance.
     *
     * \since 1.0
     */
    HotpixelCorrection();
    
    /*! \brief Destructor for the HotpixelCorrection instance
     *
     * Destroys a HotpixelCorrection instance.
     *
     * \since 1.0
     */
    ~HotpixelCorrection();

    /*! \brief Copy Constructor for a new HotpixelCorrection instance
     *
     * Creates a new HotpixelCorrection instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.0 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    HotpixelCorrection(const HotpixelCorrection& other);

    /*! \brief Copy assigment for HotpixelCorrection instance
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
    HotpixelCorrection& operator=(const HotpixelCorrection& other);

    /*! \brief Move constructor for HotpixelCorrection instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    HotpixelCorrection(HotpixelCorrection&& other) noexcept;

    /*! \brief Move assignment for HotpixelCorrection instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    HotpixelCorrection& operator=(HotpixelCorrection&& other) noexcept;

    /*!
     * \brief Sets the sensitivity of the hotpixel detection.
     *
     * \param[in] sensitivityLevel The sensitivity level to set.
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    void SetSensitivity(SensitivityLevel sensitivityLevel = SensitivityLevel::SensitivityLevel3);

    /*!
     * \brief Returns the current sensitivity.
     *
     * \returns SensitivityLevel
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD SensitivityLevel Sensitivity() const;

    /*!
     * \brief Sets the gain factor in percent.
     *
     * \param[in] gainFactorPercent The gain factor in percent to set.
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    void SetGainFactorPercent(uint32_t gainFactorPercent = 100);

    /*!
     * \brief Returns the current gain factor in percent.
     *
     * \returns the Gain factor in percent
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD uint32_t GainFactorPercent() const;

    /*!
     * \brief Detects hotpixels in the given image.
     *
     * \param[in] inputImage The input image.
     *
     * \returns List of detected hotpixels
     *
     * \throws ImageFormatNotSupportedException inputImage has a packed pixel format
     * \throws OutOfRangeException inputImage is too small to process correctly
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     * \since 1.2 Will throw ImageFormatNotSupportedException if inputImage has a packed pixel format
     * \since 1.6 Will throw OutOfRangeException if inputImage is too small to be processed correctly
     */
    PEAK_IPL_NO_DISCARD std::vector<Point2D> Detect(const Image& inputImage) const;

    /*!
     * \brief Corrects the given hotpixels in the given image.
     *
     * Pass a list of hotpixels, either returned by Detect(), or from a different source (e.g. retrieved directly from
     * the camera from an initial hotpixel calibration).
     *
     * \param[in] inputImage The input image.
     * \param[in] hotpixels  The list of hotpixels to be corrected.
     *
     * \returns Corrected image
     *
     * \throws ImageFormatNotSupportedException inputImage has a packed pixel format
     * \throws OutOfRangeException inputImage is too small to process correctly
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     * \since 1.2 Will throw ImageFormatNotSupportedException if inputImage has a packed pixel format
     * \since 1.6 Will throw OutOfRangeException if inputImage is too small to be processed correctly
     */
    PEAK_IPL_NO_DISCARD Image Correct(const Image& inputImage, const std::vector<Point2D>& hotpixels) const;

    /*!
     * \brief Corrects the given hotpixels inplace in the given image.
     *
     * Pass a list of hotpixels, either returned by Detect(), or from a different source (e.g. retrieved directly from
     * the camera from an initial hotpixel calibration). The image won't be copied and the hotpixel will be
     * corrected within the given image.
     *
     * \param[in] inputImage The input image.
     * \param[in] hotpixels  The list of hotpixels to be corrected.
     *
     * \returns Corrected image
     *
     * \throws ImageFormatNotSupportedException inputImage has a packed pixel format
     * \throws OutOfRangeException inputImage is too small to process correctly
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    Image& CorrectInPlace(Image& inputImage, const std::vector<Point2D>& hotpixels) const;

    /*!
     * \brief Corrects the given image adaptively.
     *
     * For each new passed to this method, first the list of hotpixels is adapted, then all pixels in the adapted
     * hotpixel list are corrected.
     *
     * \param[in] inputImage The input image.
     *
     * \returns Corrected image
     *
     * \throws ImageFormatNotSupportedException inputImage has a packed pixel format
     * \throws OutOfRangeException inputImage is too small to process correctly
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     * \since 1.2 Will throw ImageFormatNotSupportedException if inputImage has a packed pixel format
     * \since 1.6 Will throw OutOfRangeException if inputImage is too small to be processed correctly
     */
    PEAK_IPL_NO_DISCARD Image CorrectAdaptive(const Image& inputImage);

    /*!
     * \brief Corrects the given image inplace adaptively.
     *
     * For each new passed to this method, first the list of hotpixels is adapted, then all pixels in the adapted
     * hotpixel list are corrected. The image won't be copied.
     *
     * \param[in] inputImage The input image.
     *
     * \returns Corrected image
     *
     * \throws ImageFormatNotSupportedException inputImage has a packed pixel format
     * \throws OutOfRangeException inputImage is too small to process correctly
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    Image& CorrectAdaptiveInPlace(Image& inputImage);

private:
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE m_backendHandle{};
};

} /* namespace ipl */
} /* namespace peak */

#include <peak_ipl/types/peak_ipl_image.hpp>

namespace peak
{
namespace ipl
{

inline HotpixelCorrection::HotpixelCorrection()
{
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Construct(&m_backendHandle); });

    // make sure default params are set
    SetSensitivity();
    SetGainFactorPercent();
}

inline HotpixelCorrection::~HotpixelCorrection()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(m_backendHandle);
    }
}

inline HotpixelCorrection::HotpixelCorrection(const HotpixelCorrection& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline HotpixelCorrection& HotpixelCorrection::operator=(const HotpixelCorrection& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline HotpixelCorrection::HotpixelCorrection(HotpixelCorrection&& other) noexcept
{
    *this = std::move(other);
}

inline HotpixelCorrection& HotpixelCorrection::operator=(HotpixelCorrection&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline void HotpixelCorrection::SetSensitivity(SensitivityLevel sensitivityLevel)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity(
            m_backendHandle, static_cast<PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY>(sensitivityLevel));
    });
}

inline HotpixelCorrection::SensitivityLevel HotpixelCorrection::Sensitivity() const
{
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY sensitivityLevel;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity(
            m_backendHandle, &sensitivityLevel);
    });

    return static_cast<SensitivityLevel>(sensitivityLevel);
}

inline void HotpixelCorrection::SetGainFactorPercent(uint32_t gainFactorPercent)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent(
            m_backendHandle, gainFactorPercent);
    });
}

inline uint32_t HotpixelCorrection::GainFactorPercent() const
{
    uint32_t gainFactorPercent;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent(
            m_backendHandle, &gainFactorPercent);
    });

    return gainFactorPercent;
}

inline std::vector<Point2D> HotpixelCorrection::Detect(const Image& inputImage) const
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Detect(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage));
    });

    size_t size = 0;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels(m_backendHandle, nullptr, &size);
    });
    std::vector<Point2D> hotpixels(size);
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels(
            m_backendHandle, reinterpret_cast<PEAK_IPL_POINT_2D*>(hotpixels.data()), &size);
    });

    return hotpixels;
}

inline Image HotpixelCorrection::Correct(const Image& inputImage, const std::vector<Point2D>& hotpixels) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_Correct(m_backendHandle,
            ImageBackendAccessor::BackendHandle(inputImage),
            reinterpret_cast<const PEAK_IPL_POINT_2D*>(hotpixels.data()), hotpixels.size(), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline Image& HotpixelCorrection::CorrectInPlace(Image& inputImage, const std::vector<Point2D>& hotpixels) const
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace(m_backendHandle,
            ImageBackendAccessor::BackendHandle(inputImage),
            reinterpret_cast<const PEAK_IPL_POINT_2D*>(hotpixels.data()), hotpixels.size());
    });

    return inputImage;
}

inline Image HotpixelCorrection::CorrectAdaptive(const Image& inputImage)
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline Image& HotpixelCorrection::CorrectAdaptiveInPlace(Image& inputImage)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage));
    });

    return inputImage;
}


} /* namespace ipl */
} /* namespace peak */
