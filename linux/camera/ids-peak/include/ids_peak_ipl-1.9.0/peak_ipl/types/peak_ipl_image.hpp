/*!
 * \file    peak_ipl_image.hpp
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
#include <peak_ipl/types/peak_ipl_pixel_format.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>
#include <type_traits>

namespace
{
class ImageBackendAccessor;
}

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 * \brief Stores the pixel format, width and height of an image and the pointer to the image buffer.
 */
class Image
{
public:
    /*!
     * \brief Creates an empty image. In other words, size is 0x0 and an invalid pixel format.
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.1
     */
    Image();

    /*!
     * \brief Creates an image with the given pixel format and size.
     *
     * \param[in] pixelFormat       The pixel format.
     * \param[in] width             The width.
     * \param[in] height            The height.
     *
     * \throws OutOfRangeException  pixelFormat is valid but width or height is 0.
     * \throws Exception            An internal error has occurred.
     *
     * \since 1.0
     */
    Image(const peak::ipl::PixelFormat& pixelFormat, size_t width, size_t height);

    /*!
     * \brief Creates an image with the given pixel format and size from the given buffer.
     *
     * \param[in] pixelFormat       The pixel format.
     * \param[in] buffer            The buffer.
     * \param[in] bufferSize        The size of the buffer.
     * \param[in] width             The width.
     * \param[in] height            The height.
     *
     * \note The given buffer doesn't get copied. This is why the buffer mustn't be freed before
     *       the image gets freed.
     *
     * \throws InvalidArgumentException bufferSize is too small for the specified pixel format and image dimension.
     * \throws OutOfRangeException      pixelFormat is valid but width or height is 0.
     * \throws Exception                An internal error has occurred.
     *
     * \since 1.0
     */
    Image(
        const peak::ipl::PixelFormat& pixelFormat, uint8_t* buffer, size_t bufferSize, size_t width, size_t height);

    /*!
     * \brief Creates an image with the given pixel format, size and timestamp.
     *
     * \param[in] pixelFormat       The pixel format.
     * \param[in] width             The width.
     * \param[in] height            The height.
     * \param[in] timestamp_ns      The timestamp in ns.
     *
     * \throws OutOfRangeException  pixelFormat is valid but width or height is 0.
     * \throws Exception            An internal error has occurred.
     *
     * \since 1.5
     */
    Image(const peak::ipl::PixelFormat& pixelFormat, size_t width, size_t height, uint64_t timestamp_ns);

    /*!
     * \brief Creates an image with the given pixel format, size and timestamp from the given buffer.
     *
     * \param[in] pixelFormat       The pixel format.
     * \param[in] buffer            The buffer.
     * \param[in] bufferSize        The size of the buffer.
     * \param[in] width             The width.
     * \param[in] height            The height.
     * \param[in] timestamp_ns      The timestamp in ns.

     *
     * \note The given buffer doesn't get copied. This is why the buffer mustn't be freed before
     *       the image gets freed.
     *
     * \throws InvalidArgumentException bufferSize is too small for the specified pixel format and image dimension.
     * \throws OutOfRangeException      pixelFormat is valid but width or height is 0.
     * \throws Exception                An internal error has occurred.
     *
     * \since 1.5
     */
    Image(const peak::ipl::PixelFormat& pixelFormat, uint8_t* buffer, size_t bufferSize, size_t width, size_t height, uint64_t timestamp_ns);

    /*! \brief Destructor for the Image instance
     *
     * Destroys an Image instance.
     *
     * \since 1.0
     */
    ~Image();

    /*! \brief Copy Constructor for a new Image instance
     *
     * Creates a new Image instance.
     *
     * \note This only creates a shallow copy. To create a deepcopy, see #Image::Clone. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.0 Deleted
     * \since 1.8 Implemented as shallow copy
     */
    Image(const Image& other);
    
    /*! \brief Copy assigment for Image instance
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
    Image& operator=(const Image& other);

    /*! \brief Move constructor for Image instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    Image(Image&& other) noexcept;

    /*! \brief Move assignment for Image instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     *
     * \param[in] other The other instance to move.
     *
     * \since 1.0
     */
    Image& operator=(Image&& other) noexcept;

    /*!
     * \brief Returns the width.
     *
     * \returns Width
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD size_t Width() const;

    /*!
     * \brief Returns the height.
     *
     * \returns Height
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD size_t Height() const;

    /*!
     * \brief Returns the Size.
     *
     * \returns Size
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD peak::ipl::Size2D Size() const;

    /*!
     * \brief Returns the pixel pointer to the given pixel position in relation to the size of the pixel format.
     *
     * \note Accessing the raw data pointer isn't thread safe. Ensure data is read/written only by one thread concurrently.
     *       Also note that the buffer may be freed in the meantime.
     *
     * \param[in] xPos The x position.
     * \param[in] yPos The y position.
     *
     * \returns PixelPointer
     *
     * \throws OutOfRangeException The pixel position is outside the image.
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD uint8_t* PixelPointer(size_t xPos, size_t yPos) const;

    /*!
     * \brief Returns the size of the given image in number of bytes.
     *
     * \returns ByteCount
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD size_t ByteCount() const;

    /*!
     * \brief Returns the pixel format.
     *
     * \return Pixel format
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD peak::ipl::PixelFormat PixelFormat() const;

    /*!
     * \brief Returns the pointer to the first pixel position in dependency on the size of the pixel format.
     *
     * \note Accessing the raw data pointer isn't thread safe. Ensure data is read/written only by one thread concurrently.
     *       Also note that the buffer may be freed in the meantime.
     *
     * \returns Buffer data.
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD uint8_t* Data() const;

    /*!
     * \brief Returns a new created image containing the data of the current image converted to the given pixel format.
     *
     * \param[in]  outputPixelFormat The output pixel format.
     * \param[in]  conversionMode    The conversion mode.
     *
     * \returns Converted image
     *
     * \note This function allocates memory on every use. For faster processing, consider using ImageConverter::Convert instead.
     *
     * \throws Exception An internal error has occurred. 
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD Image ConvertTo(const peak::ipl::PixelFormat& outputPixelFormat,
        peak::ipl::ConversionMode conversionMode = peak::ipl::ConversionMode::Fast) const;

    /*!
     * \brief Saves the data of the current image converted to the given pixel format into a destination buffer
     *        and creates an image from that buffer.
     *
     * \param[in]  outputPixelFormat     The output pixel format.
     * \param[out] outputImageBuffer     Pointer to destination buffer.
     * \param[in]  outputImageBufferSize Size of destination buffer.
     * \param[in]  conversionMode        The conversion mode.
     *
     * \returns Converted image
     * 
     * \note This function allocates memory on every use. For faster processing, consider using ImageConverter::Convert instead.
     *
     * \throws BufferTooSmallException  The provided buffer is too small for the output image.
     * \throws Exception                An internal error has occurred.
     *
     * \since 1.1
     */
    Image ConvertTo(const peak::ipl::PixelFormat& outputPixelFormat, uint8_t* outputImageBuffer,
        size_t outputImageBufferSize,
        peak::ipl::ConversionMode conversionMode = peak::ipl::ConversionMode::Fast) const;

    /*!
     * \brief Scale the image to the supplied width and height using the supplied \p algorithm
     *
     * \param[in] width         output image width
     * \param[in] height        output image height
     * \param[in] algorithm     the used interpolation algorithm
     *
     * \returns The scaled image
     *
     * \throws Exception        An internal error has occurred.
     *
     * \since 1.7
     */
    template <typename Integer1, typename Integer2,
        typename std::enable_if_t<std::is_integral<Integer1>::value, int> = 0,
        typename std::enable_if_t<std::is_integral<Integer2>::value, int> = 0>
    PEAK_IPL_NO_DISCARD inline Image Scale(Integer1 width, Integer2 height, peak::ipl::InterpolationAlgorithm algorithm=InterpolationAlgorithm::linear) const
    {
        return Scale(peak::ipl::Size2D{static_cast<std::size_t>(width), static_cast<std::size_t>(height)}, algorithm);
    }

    /*!
     * \brief Scale the image to the supplied width and height using the supplied \p algorithm
     *
     * \param[in] size          output size of the image
     * \param[in] algorithm     the used interpolation algorithm
     *
     * \returns The scaled image
     *
     * \throws Exception        An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD Image Scale(const peak::ipl::Size2D& size, peak::ipl::InterpolationAlgorithm algorithm=InterpolationAlgorithm::linear) const;

    /*!
     * \brief Scale the image to the supplied scale factors \p factor using the supplied \p algorithm
     *
     * \param[in] factor        output image scale factors
     * \param[in] algorithm     the used interpolation algorithm
     *
     * \returns The scaled image
     *
     * \note the image size is { input_image.width * factor.X, input_image.height * factor.Y }
     *
     * \throws Exception        An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD Image Scale(const peak::ipl::ScaleFactor& factor, peak::ipl::InterpolationAlgorithm algorithm=InterpolationAlgorithm::linear) const;

    /*!
     * \brief Scale the image to the supplied scale factors \p scaleFactorX and \p scaleFactorY using the supplied \p algorithm
     *
     * \param[in] scaleFactorX  output image scale factor for width
     * \param[in] scaleFactorY  output image scale factor for height
     * \param[in] algorithm     the used interpolation algorithm
     *
     * \returns The scaled image
     *
     * \note the image size is { input_image.width * scaleFactorX, input_image.height * scaleFactorY }
     *
     * \throws Exception        An internal error has occurred.
     *
     * \since 1.7
     */
    template<typename FloatingPoint1, typename FloatingPoint2,
        typename std::enable_if_t<std::is_floating_point<FloatingPoint1>::value, int> = 0,
        typename std::enable_if_t<std::is_floating_point<FloatingPoint2>::value, int> = 0>
    PEAK_IPL_NO_DISCARD inline Image Scale(FloatingPoint1 scaleFactorX, FloatingPoint2 scaleFactorY, peak::ipl::InterpolationAlgorithm algorithm=InterpolationAlgorithm::linear) const
    {
        return Scale(peak::ipl::ScaleFactor{static_cast<double>(scaleFactorX), static_cast<double>(scaleFactorY)}, algorithm);
    }

    /*!
     * \brief Scale the image to the supplied width and height using the supplied \p algorithm and save it into the supplied buffer
     *
     * \param[in] size          output size of the image
     * \param[in] buffer        the used interpolation algorithm
     * \param[in] bufferSize    the used interpolation algorithm
     * \param[in] algorithm     the used interpolation algorithm
     *
     * \throws Exception        An internal error has occurred.
     *
     * \since 1.7
     */
    void ScaleInto(const peak::ipl::Size2D& size, uint8_t* buffer, std::size_t bufferSize, peak::ipl::InterpolationAlgorithm algorithm=InterpolationAlgorithm::linear) const;

    /*!
     * \brief Crop the image to the \p size starting at \p pos.
     *
     * \param[in] pos        the startting position
     * \param[in] size       the used size
     *
     * \returns The cropped image
     *     *
     * \throws Exception        An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD Image Crop(const peak::ipl::Point2D& pos, const peak::ipl::Size2D& size) const;

    /*!
     * \brief Crop the image to the supplied size. Position is set to (0, 0)
     *
     * \param[in] size     The output size.
     *
     * \returns The cropped image
     *     *
     * \throws Exception    An internal error has occurred.
     *
     * \since 1.7
     */
    PEAK_IPL_NO_DISCARD Image Crop(const peak::ipl::Size2D& size) const;

    /*!
     * \brief Returns a new created image containing the data of the current image as `deep copy`.
     *
     * \returns The copied image
     *
     * \throws Exception An internal error has occurred.
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD Image Clone() const;

    /*!
     * \brief Checks whether the image is empty.
     *
     * An image can be empty if its data have been moved.
     *
     * \returns True if the image is empty
     *
     * \since 1.0
     */
    PEAK_IPL_NO_DISCARD bool Empty() const;

    /*! \brief The timestamp in nanoseconds
     *
     * The point in camera time at which the image was exposed.
     *
     * \returns Timestamp in nanoseconds
     *
     * \since 1.6
     */
    PEAK_IPL_NO_DISCARD uint64_t Timestamp() const;

    /*!
     * \brief Checks whether the image is data-wise the same.
     * \returns True if image is data-wise the same, false if not.
     */
    PEAK_IPL_NO_DISCARD bool operator==(const Image& img) const;

    /*!
     * \brief Checks whether the image is data-wise different.
     * \returns True if image is data-wise different, false if not.
     */
    PEAK_IPL_NO_DISCARD bool operator!=(const Image& img) const;

private:
    friend ImageBackendAccessor;

    explicit Image(PEAK_IPL_IMAGE_HANDLE imageHandle, bool acquireReference=false);

    PEAK_IPL_IMAGE_HANDLE m_backendHandle{};
};

} /* namespace ipl */
} /* namespace peak */

namespace
{

//! helper class to access the C-backend of the Image, which are hidden from the public interface
class ImageBackendAccessor
{
public:
    /*!
     * \brief Get the handle for an Image instance
     *
     * \param image the Image class instance.
     *
     * \return The backend handle for the Image instance.
     */
    PEAK_IPL_NO_DISCARD static PEAK_IPL_IMAGE_HANDLE BackendHandle(const peak::ipl::Image& image)
    {
        return image.m_backendHandle;
    }

    /*!
     * \brief Create an Image instance from a handle
     *
     * \param imageHandle The Image handle to create the instance from.
     *
     * \note Don't use this to create an Image instance if one already exists. Use #CreateImageReference instead.
     *
     * \return The Image instance.
     */
    PEAK_IPL_NO_DISCARD static peak::ipl::Image CreateImage(PEAK_IPL_IMAGE_HANDLE imageHandle)
    {
        return peak::ipl::Image(imageHandle, false);
    }

    /*!
     * \brief Create an Image instance from a handle
     *
     * \param imageHandle The Image handle to create the instance from.
     *
     * \note Will acquire a reference to the Gain instance by handle. See \ref shallowCopy for a detailed explanation.
     *
     * \return The Image instance.
     */
    PEAK_IPL_NO_DISCARD static peak::ipl::Image CreateImageReference(PEAK_IPL_IMAGE_HANDLE imageHandle)
    {
        return peak::ipl::Image(imageHandle, true);
    }
};

} /* namespace */

namespace peak
{
namespace ipl
{
inline Image::Image()
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Construct(PEAK_IPL_PIXEL_FORMAT_INVALID, 0, 0, &m_backendHandle);
    });
}

inline Image::Image(const peak::ipl::PixelFormat& pixelFormat, size_t width, size_t height)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Construct(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormat.PixelFormatName()), width, height, &m_backendHandle);
    });
}

inline Image::Image(
    const peak::ipl::PixelFormat& pixelFormat, uint8_t* buffer, size_t bufferSize, size_t width, size_t height)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_ConstructFromBuffer(
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormat.PixelFormatName()), buffer, bufferSize, width, height,
            &m_backendHandle);
    });
}

inline Image::Image(const peak::ipl::PixelFormat& pixelFormat, size_t width, size_t height, uint64_t timestamp_ns) :
    Image::Image(pixelFormat, width, height)
{
        ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_SetTimestamp(m_backendHandle, timestamp_ns);
    });
}

inline Image::Image(
    const peak::ipl::PixelFormat& pixelFormat, uint8_t* buffer, size_t bufferSize, size_t width, size_t height, uint64_t timestamp_ns) :
    Image::Image(pixelFormat, buffer, bufferSize, width, height)
{
 ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_SetTimestamp(m_backendHandle, timestamp_ns);
    });
}

inline Image::Image(PEAK_IPL_IMAGE_HANDLE imageHandle, bool acquireReference)
    : m_backendHandle(imageHandle)
{
    if (acquireReference && m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Acquire(m_backendHandle); });
    }
}

inline Image::~Image()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Destruct(m_backendHandle);
    }
}

inline Image::Image(const Image& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline Image& Image::operator=(const Image& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline Image::Image(Image&& other) noexcept
{
    *this = std::move(other);
}

inline Image& Image::operator=(Image&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Destruct(m_backendHandle);
        }
        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}

inline size_t Image::Width() const
{
    size_t width = 0;

    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetWidth(m_backendHandle, &width); });

    return width;
}

inline size_t Image::Height() const
{
    size_t height = 0;

    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetHeight(m_backendHandle, &height); });

    return height;
}

inline peak::ipl::Size2D Image::Size() const
{
    return {Width(), Height()};
}

inline uint8_t* Image::PixelPointer(size_t xPos, size_t yPos) const
{
    uint8_t* pixelPosition = nullptr;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetPixelPointer(m_backendHandle, xPos, yPos, &pixelPosition);
    });

    return pixelPosition;
}

inline size_t Image::ByteCount() const
{
    size_t byteCount = 0;

    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetByteCount(m_backendHandle, &byteCount); });

    return byteCount;
}

inline peak::ipl::PixelFormat Image::PixelFormat() const
{
    peak::ipl::PixelFormatName pixelFormatName = PixelFormatName::Invalid;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetPixelFormat(
            m_backendHandle, reinterpret_cast<PEAK_IPL_PIXEL_FORMAT*>(&pixelFormatName));
    });

    return peak::ipl::PixelFormat{ pixelFormatName };
}

inline uint8_t* Image::Data() const
{
    uint8_t* data = nullptr;

    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetData(m_backendHandle, &data); });

    return data;
}

inline Image Image::ConvertTo(
    const peak::ipl::PixelFormat& outputPixelFormat, peak::ipl::ConversionMode conversionMode) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_ConvertTo(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(outputPixelFormat.PixelFormatName()),
            static_cast<PEAK_IPL_CONVERSION_MODE>(conversionMode), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline Image Image::ConvertTo(const peak::ipl::PixelFormat& outputPixelFormat, uint8_t* outputImageBuffer,
    size_t outputImageBufferSize, peak::ipl::ConversionMode conversionMode) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_ConvertToBuffer(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(outputPixelFormat.PixelFormatName()), outputImageBuffer,
            outputImageBufferSize, static_cast<PEAK_IPL_CONVERSION_MODE>(conversionMode), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline Image Image::Scale(const Size2D& size, peak::ipl::InterpolationAlgorithm algorithm) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    const PEAK_IPL_SIZE_2D size_in{size.width, size.height};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Scale(m_backendHandle, &size_in, static_cast<PEAK_IPL_INTERPOLATION_ALGORITHM>(algorithm), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline Image Image::Scale(const ScaleFactor& factor, peak::ipl::InterpolationAlgorithm algorithm) const
{
    return Scale(Size2D{static_cast<size_t>(static_cast<double>(Width()) * factor.x), static_cast<size_t>(static_cast<double>(Height()) * factor.y)}, algorithm);
}

inline void Image::ScaleInto(const Size2D& size, uint8_t* buffer, std::size_t bufferSize, peak::ipl::InterpolationAlgorithm algorithm) const
{
    const PEAK_IPL_SIZE_2D size_in{size.width, size.height};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_ScaleInto(m_backendHandle, &size_in, buffer, bufferSize, static_cast<PEAK_IPL_INTERPOLATION_ALGORITHM>(algorithm));
    });
}


inline Image Image::Crop(const Point2D& pos, const Size2D& size) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    const PEAK_IPL_POINT_2D pos_in{pos.x, pos.y};
    const PEAK_IPL_SIZE_2D size_in{size.width, size.height};

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Crop(m_backendHandle, &pos_in, &size_in, &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline Image Image::Crop(const Size2D& size) const
{
    return Crop({0,0}, size);
}

inline Image Image::Clone() const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes(
        [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_Clone(m_backendHandle, &outputImageHandle); });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline bool Image::Empty() const
{
    return (m_backendHandle == nullptr) || (PixelFormat().PixelFormatName() == PixelFormatName::Invalid);
}

inline uint64_t Image::Timestamp() const
{
    uint64_t timestamp_ns;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Image_GetTimestamp(m_backendHandle, &timestamp_ns);
    });

    return timestamp_ns;
}

inline bool Image::operator==(const peak::ipl::Image& img) const
{
    if (img.ByteCount() != this->ByteCount())
    {
        return false;
    }
    if (img.Width() != this->Width())
    {
        return false;
    }
    if (img.Height() != this->Height())
    {
        return false;
    }
    if (img.PixelFormat().PixelFormatName() != this->PixelFormat().PixelFormatName())
    {
        return false;
    }
    if (img.Timestamp() != this->Timestamp())
    {
        return false;
    }

    return memcmp(img.Data(), this->Data(), img.ByteCount()) == 0;
}

inline bool Image::operator!=(const peak::ipl::Image& img) const
{
    return !(*this == img);
}

} /* namespace ipl */
} /* namespace peak */
