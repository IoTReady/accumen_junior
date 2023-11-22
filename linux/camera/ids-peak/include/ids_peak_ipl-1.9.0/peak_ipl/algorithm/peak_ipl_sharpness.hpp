/*!
 * \file    peak_ipl_sharpness.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-10-14
 * \since   1.6
 *
 * Copyright (c) 2022 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_rect.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>
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
enum class PixelFormatName;
/*!
 * \brief Configure and measure the sharpness of a given image.
 *
 * The class allows the measurement of the sharpness in a defined ROI of the given image. To get a sharpness value, the
 * edges in the image are evaluated. The sharpness can only be indicated as a relative value, as it depends on the edges
 * in the current image. An image with fewer edges won't reach the sharpness value of an image with a lot of edges.
 */
class Sharpness final
{
public:
    /*! \brief Constructor for a new Sharpness instance
     *
     * Creates a new Sharpness instance.
     *
     * \since 1.6
     */
    Sharpness();
    
    /*! \brief Destructor for the Sharpness instance
     *
     * Destroys a Sharpness instance.
     *
     * \since 1.6
     */
    ~Sharpness();

    /*! \brief Copy Constructor for a new Sharpness instance
     *
     * Creates a new Sharpness instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.6 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    Sharpness(const Sharpness& other);

    /*! \brief Copy assigment for Sharpness instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     * 
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.6 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    Sharpness& operator=(const Sharpness& other);

    /*! \brief Move constructor for Sharpness instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.6
     */
    Sharpness(Sharpness&& other) noexcept;

    /*! \brief Move assignment for Sharpness instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.6
     */
    Sharpness& operator=(Sharpness&& other) noexcept;

    //! Measure Algorithms
    enum class SharpnessAlgorithm : uint16_t
    {
        Tenengrad, //!< contrast-based sharpness algorithm (convolution)
        MeanScore, //!< contrast-based sharpness algorithm (mean value)
        HistogramVariance, //!< statistics-based sharpness algorithm
        Sobel //!< contrast-based sharpness algorithm (convolution)
    };

    /*! Weight for a sharpness roi. The weight influences the sum of the measure. Strong roi have more weight than
     *  medium and medium more weight than weak.
     */
    enum class SharpnessROIWeight : uint16_t
    {
        Weak = 0x0021, //!< Weak use of roi for the measurement
        Medium = 0x0042, //!< Normal use of roi for the measurement
        Strong = 0x0063 //!< Strong use of roi for the measurement
    };

    struct SharpnessROI
    {
#if !defined(__cpp_aggregate_nsdmi)
		SharpnessROI() = default;
		SharpnessROI(Rect2D rect, uint8_t xSamplingInterval, uint8_t ySamplingInterval, SharpnessROIWeight weight) :
			rect(std::move(rect)), xSamplingInterval(xSamplingInterval), ySamplingInterval(ySamplingInterval), weight(weight) {}
#endif
        Rect2D rect{ 0, 0, 0, 0 }; //!< Sharpness Rect
        uint8_t xSamplingInterval{ 1 }; //!< horizontal sampling interval
        uint8_t ySamplingInterval{ 1 }; //!< vertical sampling interval
        SharpnessROIWeight weight{ SharpnessROIWeight::Medium }; //!< weight for the roi
    };

    using SharpnessROIList = std::vector<SharpnessROI>;

    /*!
     * \brief Measures the sharpness of the given image. The higher the value, the better the sharpness.
     *
     * \param[in] image The image to measure the relative sharpness.
     *
     * \returns Relative sharpness value
     *
     * \throws InvalidPixelFormat pixel format isn't supported.
     *
     * \since 1.6
     */
    inline double Measure(const Image& image);

    /*!
     * \brief Checks if the given pixel format can be used for sharpness measurement.
     *
     * \returns Flag whether the given pixel format is supported.
     * \param[in] pixelFormatName The pixelFormat of the input image.
     *
     * \since 1.6
     */
    PEAK_IPL_NO_DISCARD inline bool IsPixelFormatSupported(PixelFormatName pixelFormatName) const;

    /*!
     * \brief Returns a list of supported pixel formats which can be used for sharpness measurement.
     * \returns List of supported pixel formats.
     *
     * \since 1.6
     */
    PEAK_IPL_NO_DISCARD inline std::vector<PixelFormatName> SupportedPixelFormats() const;

    /*!
     * \brief Sets the algorithm used for measurement calculation.
     *
     * \since 1.6
     */
    inline void SetAlgorithm(SharpnessAlgorithm algorithm);

    /*!
     * \brief Returns the used algorithm for the sharpness measurement.
     * \returns Current algorithm
     *
     * \since 1.6
     */
    PEAK_IPL_NO_DISCARD inline SharpnessAlgorithm Algorithm() const;

    /*!
     * \brief Gets the minimum size of a ROI, in which the sharpness is measured.
     * \returns Required minimum size of a roi
     *
     * \since 1.6
     */
    PEAK_IPL_NO_DISCARD inline Size2D ROIMinSize() const;

    /*!
     * \brief Sets the region of interests in which the sharpness is measured.
     * \param[in] rois SharpnessROIList to set.
     *
     * \since 1.6
     */
    inline void SetROIs(const SharpnessROIList& rois);

    /*!
     * \brief Set ROI in which the sharpness is measured.
     * \param[in] roi SharpnessROI to set.
     *
     * \since 1.6
     */
    inline void SetROI(const SharpnessROI& roi);

    /*!
     * \brief Set ROI in which the sharpness is measured.
     * \param[in] roi Rect2D to set.
     *
     * \since 1.6
     */
    inline void SetROI(const Rect2D& roi);

    /*!
     * \brief Gets ROIs in which the sharpness is measured.
     * \returns Active measurement rois
     *
     * \since 1.6
     */
    PEAK_IPL_NO_DISCARD inline SharpnessROIList ROIs() const;

private:
    PEAK_IPL_SHARPNESS_HANDLE m_backendHandle{};
};

inline Sharpness::Sharpness()
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Construct(&m_backendHandle); });
}

inline Sharpness::~Sharpness()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Destruct(m_backendHandle);
    }
}

inline Sharpness::Sharpness(const Sharpness& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline Sharpness& Sharpness::operator=(const Sharpness& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes(
                [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline Sharpness::Sharpness(Sharpness&& other) noexcept
{
    *this = std::move(other);
}

inline Sharpness& Sharpness::operator=(Sharpness&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline double Sharpness::Measure(const Image& image)
{
    double calculated_value;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_Measure(
            m_backendHandle, ImageBackendAccessor::BackendHandle(image), &calculated_value);
    });

    return calculated_value;
}

inline bool Sharpness::IsPixelFormatSupported(PixelFormatName pixelFormatName) const
{
    PEAK_IPL_BOOL8 isPixelFormatSupported = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormatName),
            reinterpret_cast<PEAK_IPL_BOOL8*>(&isPixelFormatSupported));
    });

    return isPixelFormatSupported > 0;
}

inline std::vector<PixelFormatName> Sharpness::SupportedPixelFormats() const
{
    size_t sizeOfData{};
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList(
            m_backendHandle, nullptr, &sizeOfData);
    });

    std::vector<PixelFormatName> formats(sizeOfData);
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList(
            m_backendHandle, reinterpret_cast<PEAK_IPL_PIXEL_FORMAT*>(formats.data()), &sizeOfData);
    });

    return formats;
}

inline void Sharpness::SetAlgorithm(SharpnessAlgorithm algorithm)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_SetAlgorithm(
            m_backendHandle, static_cast<PEAK_IPL_SHARPNESS_ALGORITHM>(algorithm));
    });
}

inline Sharpness::SharpnessAlgorithm Sharpness::Algorithm() const
{
    SharpnessAlgorithm algorithm;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetAlgorithm(
            m_backendHandle, reinterpret_cast<PEAK_IPL_SHARPNESS_ALGORITHM*>(&algorithm));
    });

    return algorithm;
}

inline Size2D Sharpness::ROIMinSize() const
{
    PEAK_IPL_SIZE_2D size{};

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetROIMinimumSize(m_backendHandle, &size); });

    return Size2D{ size.width, size.height };
}

inline void Sharpness::SetROIs(const SharpnessROIList& rois)
{
    ExecuteAndMapReturnCodes([&] {
        std::vector<PEAK_IPL_SHARPNESS_ROI> sharpness_rois{};

        std::transform(std::cbegin(rois), std::cend(rois), std::back_inserter(sharpness_rois), [](const auto roi) {
            return PEAK_IPL_SHARPNESS_ROI{ PEAK_IPL_RECT_2D{ { roi.rect.topLeft().x, roi.rect.topLeft().y },
                                              { roi.rect.size().width, roi.rect.size().height } },
                roi.xSamplingInterval, roi.ySamplingInterval,
                static_cast<std::underlying_type_t<SharpnessROIWeight>>(roi.weight) };
        });

        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_SetROIs(
            m_backendHandle, sharpness_rois.data(), sharpness_rois.size());
    });
}

inline void Sharpness::SetROI(const SharpnessROI& roi)
{
    SharpnessROIList rois{ roi };
    SetROIs(rois);
}

inline void Sharpness::SetROI(const Rect2D& roi)
{
    SharpnessROI sharpnessRoi{ roi, 1, 1, SharpnessROIWeight::Medium };
    SetROI(sharpnessRoi);
}

inline Sharpness::SharpnessROIList Sharpness::ROIs() const
{
    size_t roiListSize{};
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetROIs(m_backendHandle, nullptr, &roiListSize); });

    std::vector<PEAK_IPL_SHARPNESS_ROI> sharpness_rois(roiListSize);
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageSharpness_GetROIs(
            m_backendHandle, sharpness_rois.data(), &roiListSize);
    });

    SharpnessROIList rois{};

    std::transform(
        std::cbegin(sharpness_rois), std::cend(sharpness_rois), std::back_inserter(rois), [](const auto roi) {
            return SharpnessROI{ Rect2D{ roi.rect.position.x, roi.rect.position.y, roi.rect.size.width,
                                     roi.rect.size.height },
                roi.xSamplingInterval, roi.ySamplingInterval, static_cast<SharpnessROIWeight>(roi.weight) };
        });

    return rois;
}

inline bool operator==(const Sharpness::SharpnessROI& lhs, const Sharpness::SharpnessROI& rhs)
{
    return lhs.rect == rhs.rect && lhs.weight == rhs.weight && lhs.xSamplingInterval == rhs.xSamplingInterval
        && lhs.ySamplingInterval == rhs.ySamplingInterval;
}

inline bool operator!=(const Sharpness::SharpnessROI& lhs, const Sharpness::SharpnessROI& rhs)
{
    return !(lhs == rhs);
}

} /* namespace ipl */
} /* namespace peak */
