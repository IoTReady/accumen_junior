/*!
 * \file    peak_ipl_image_reader.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-16
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_pixel_format.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>
#include <peak_ipl/types/peak_ipl_image.hpp>

#include <string>

namespace peak
{
namespace ipl
{

class Image;

/*!
 * \brief Reads an image from a file.
 *
 * Supported Formats are currently:
 * JPEG, PNG and BMP
 */
class ImageReader final
{
public:
    ImageReader() = delete;

    /*!
     * \brief Returns a new created image which is read from the file with the given file path.
     *
     * The format is specified by the file ending.
     *
     * \param[in] filePath The path to the file to read as an UTF-8 encoded string.
     *
     * \throws ImageFormatNotSupportedException An image format of this file isn't supported
     * \throws IOException                      Errors during file access e.g. no permissions on this file
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.0
     * \since 1.6 Defined character encoding UTF-8
     */
    PEAK_IPL_NO_DISCARD static Image Read(const std::string& filePath);

    /*!
     * \brief Returns a new created image which is read from the file with the given file path.
     *
     * The Pixel format to use is specified manually here. The function tries to interpret the file with the given
     * format. If this isn't possible, an exception is thrown. Explicit conversion of image formats must be done
     * manually.
     *
     * \param[in] filePath    The path to the file to read as an UTF-8 encoded string.
     * \param[in] pixelFormat The Pixel format the retrieved image should have afterwards.
     *
     * \throws ImageFormatInterpretationException Can't interpret this file with the given pixel format
     * \throws ImageFormatNotSupportedException   An image format of this file isn't supported
     * \throws IOException                        Errors during file access e.g. no permissions on this file
     * \throws InvalidArgumentException           Arguments passed are invalid
     * \throws Exception                          An internal error has occurred
     *
     * \since 1.0
     * \since 1.2 Will throw ImageFormatNotSupportedException if pixelFormat is a packed pixel format
     * \since 1.6 Defined character encoding UTF-8
     */
    PEAK_IPL_NO_DISCARD static Image Read(const std::string& filePath, const peak::ipl::PixelFormat& pixelFormat);
};

inline Image ImageReader::Read(const std::string& filePath)
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageReader_Read(filePath.c_str(), filePath.size(), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

inline peak::ipl::Image ImageReader::Read(const std::string& filePath, const PixelFormat& pixelFormat)
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_ImageReaderRead_ReadAsPixelFormat(filePath.c_str(), filePath.size(),
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormat.PixelFormatName()), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

} /* namespace ipl */
} /* namespace peak */
