/*!
 * \file    peak_ipl_exception.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 * \brief The base class for all exceptions thrown by the library.
 */
class Exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/*!
 * \brief The exception thrown for trying to access a value being out of range.
 */
class OutOfRangeException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when a given buffer is too small for the data.
 */
class BufferTooSmallException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when passing an invalid parameter to a function.
 */
class InvalidArgumentException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when an image format isn't supported by a function
 */
class ImageFormatNotSupportedException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when a given image format can't be used on this data, e.g. during reading from file
 */
class ImageFormatInterpretationException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when a given image format can't be used on this data, e.g. during reading from file
 */
class IOException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when the resource busy
 */
class BusyException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when the operation not permitted.
 */
class NotPermittedException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when the operation times out.
 */
class TimeoutException : public Exception
{
    using Exception::Exception;
};

/*!
 * \brief The exception thrown when passing an invalid handle to a function.
 */
class InvalidHandleException : public Exception
{
    using Exception::Exception;
};

inline std::string StringFromPEAK_IPL_RETURN_CODE(PEAK_IPL_RETURN_CODE_t entry)
{
    std::string entryString;

    switch(entry)
    {
    case PEAK_IPL_RETURN_CODE_SUCCESS:
        entryString = "PEAK_IPL_RETURN_CODE_SUCCESS";
        break;

    case PEAK_IPL_RETURN_CODE_ERROR:
        entryString = "PEAK_IPL_RETURN_CODE_ERROR";
        break;

    case PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL:
        entryString = "PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL";
        break;

    case PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT:
        entryString = "PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT";
        break;

    case PEAK_IPL_RETURN_CODE_INVALID_HANDLE:
        entryString = "PEAK_IPL_RETURN_CODE_INVALID_HANDLE";
        break;

    case PEAK_IPL_RETURN_CODE_OUT_OF_RANGE:
        entryString = "PEAK_IPL_RETURN_CODE_OUT_OF_RANGE";
        break;

    case PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED:
        entryString = "PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED";
        break;

    case PEAK_IPL_RETURN_CODE_IO_ERROR:
        entryString = "PEAK_IPL_RETURN_CODE_IO_ERROR";
        break;

    case PEAK_IPL_RETURN_CODE_BUSY:
        entryString = "limgTypeRETURN_CODE_BUSY";
        break;

    case PEAK_IPL_RETURN_CODE_TIMEOUT:
        entryString = "limgTypeRETURN_CODE_TIMEOUT";
        break;

    case PEAK_IPL_RETURN_CODE_FORMAT_INTERPRETATION_ERROR:
        entryString = "PEAK_IPL_RETURN_CODE_FORMAT_INTERPRETATION_ERROR";
        break;

    case PEAK_IPL_RETURN_CODE_NOT_SUPPORTED:
        entryString = "limgTypeRETURN_CODE_NOT_SUPPORTED";
        break;

    case PEAK_IPL_RETURN_CODE_NOT_PERMITTED:
        entryString = "limgTypeRETURN_CODE_NOT_PERMITTED";
        break;
    }

    return entryString;
}

template <class CallableType>
void ExecuteAndMapReturnCodes(const CallableType& callableObject)
{
    if (callableObject() != PEAK_IPL_RETURN_CODE_SUCCESS)
    {
        PEAK_IPL_RETURN_CODE lastErrorCode = PEAK_IPL_RETURN_CODE_SUCCESS;
        size_t lastErrorDescriptionSize = 0;
        if (PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Library_GetLastError(&lastErrorCode, nullptr, &lastErrorDescriptionSize)
            != PEAK_IPL_RETURN_CODE_SUCCESS)
        {
            throw Exception("Could not query the last error!");
        }
        std::vector<char> lastErrorDescription(lastErrorDescriptionSize);
        if (PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Library_GetLastError(
                &lastErrorCode, lastErrorDescription.data(), &lastErrorDescriptionSize)
            != PEAK_IPL_RETURN_CODE_SUCCESS)
        {
            throw Exception("Could not query the last error!");
        }

        std::stringstream errorText;
        errorText << "[Error-Code: " << lastErrorCode << " ("
                  << StringFromPEAK_IPL_RETURN_CODE(static_cast<PEAK_IPL_RETURN_CODE_t>(lastErrorCode))
                  << ") | Error-Description: " << lastErrorDescription.data() << "]";

        switch (lastErrorCode)
        {
        case PEAK_IPL_RETURN_CODE_OUT_OF_RANGE:
            throw OutOfRangeException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT:
            throw InvalidArgumentException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED:
            throw ImageFormatNotSupportedException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_FORMAT_INTERPRETATION_ERROR:
            throw ImageFormatInterpretationException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_IO_ERROR:
            throw IOException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL:
            throw BufferTooSmallException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_NOT_PERMITTED:
            throw NotPermittedException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_BUSY:
            throw BusyException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_INVALID_HANDLE:
            throw InvalidHandleException(errorText.str().c_str());
        case PEAK_IPL_RETURN_CODE_TIMEOUT:
            throw TimeoutException(errorText.str().c_str());
        default:
            throw Exception(errorText.str().c_str());
        }
    }
}

} /* namespace ipl */
} /* namespace peak */
