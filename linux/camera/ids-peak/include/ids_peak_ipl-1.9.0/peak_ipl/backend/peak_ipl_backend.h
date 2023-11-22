/*!
 * \file    peak_ipl_backend.h
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_dll_defines.h>

#if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
#    define PEAK_IPL_NO_DISCARD [[nodiscard]]
#    define PEAK_IPL_MAYBE_UNUSED [[maybe_unused]]
#else
#    ifdef _MSC_VER
#        define PEAK_IPL_NO_DISCARD _Check_return_
#        define PEAK_IPL_MAYBE_UNUSED
#    elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#        define PEAK_IPL_NO_DISCARD __attribute__((warn_unused_result))
#        define PEAK_IPL_MAYBE_UNUSED __attribute__((unused))
#    else
#        define PEAK_IPL_NO_DISCARD
#        define PEAK_IPL_MAYBE_UNUSED
#    endif
#endif

#ifdef __cplusplus
#    include <cstddef>
#    include <cstdint>

extern "C" {
#else
#    include <stddef.h>
#    include <stdint.h>
#endif

typedef int8_t PEAK_IPL_BOOL8;

/*! The enum holding the possible function return codes. */
enum PEAK_IPL_RETURN_CODE_t
{
    PEAK_IPL_RETURN_CODE_SUCCESS = 0,
    PEAK_IPL_RETURN_CODE_ERROR = 1,
    PEAK_IPL_RETURN_CODE_INVALID_HANDLE = 2,
    PEAK_IPL_RETURN_CODE_IO_ERROR = 3,
    PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL = 4,
    PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT = 5,
    PEAK_IPL_RETURN_CODE_OUT_OF_RANGE = 6,
    PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED = 7,
    PEAK_IPL_RETURN_CODE_FORMAT_INTERPRETATION_ERROR = 8,
    PEAK_IPL_RETURN_CODE_NOT_SUPPORTED = 9,
    PEAK_IPL_RETURN_CODE_NOT_PERMITTED = 10,
    PEAK_IPL_RETURN_CODE_BUSY = 11,
    PEAK_IPL_RETURN_CODE_TIMEOUT = 12

};
typedef int32_t PEAK_IPL_RETURN_CODE;

/*! The enum holding the possible conversion modes. */
enum PEAK_IPL_CONVERSION_MODE_t
{
    PEAK_IPL_CONVERSION_MODE_FAST = 1,
    PEAK_IPL_CONVERSION_MODE_HIGH_QUALITY = 2,
    PEAK_IPL_CONVERSION_MODE_CLASSIC = 3,
};
typedef int32_t PEAK_IPL_CONVERSION_MODE;

/*! The enum holding the possible pixel formats. */
enum PEAK_IPL_PIXEL_FORMAT_t
{
    PEAK_IPL_PIXEL_FORMAT_INVALID = 0,

    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_8 = 0x01080008,
    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_10 = 0x0110000C,
    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_12 = 0x01100010,

    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_8 = 0x01080009,
    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_10 = 0x0110000D,
    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_12 = 0x01100011,

    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_8 = 0x0108000A,
    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_10 = 0x0110000E,
    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_12 = 0x01100012,

    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_8 = 0x0108000B,
    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_10 = 0x0110000F,
    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_12 = 0x01100013,

    PEAK_IPL_PIXEL_FORMAT_MONO_8 = 0x01080001,
    PEAK_IPL_PIXEL_FORMAT_MONO_10 = 0x01100003,
    PEAK_IPL_PIXEL_FORMAT_MONO_12 = 0x01100005,

    PEAK_IPL_PIXEL_FORMAT_YUV420_8_YY_UV_SemiplanarIDS = 0x420C0001,
    PEAK_IPL_PIXEL_FORMAT_YUV420_8_YY_VU_SemiplanarIDS = 0x420C0002,

    PEAK_IPL_PIXEL_FORMAT_RGB_8 = 0x02180014,
    PEAK_IPL_PIXEL_FORMAT_RGB_10 = 0x02300018,
    PEAK_IPL_PIXEL_FORMAT_RGB_12 = 0x0230001A,

    PEAK_IPL_PIXEL_FORMAT_BGR_8 = 0x02180015,
    PEAK_IPL_PIXEL_FORMAT_BGR_10 = 0x02300019,
    PEAK_IPL_PIXEL_FORMAT_BGR_12 = 0x0230001B,

    PEAK_IPL_PIXEL_FORMAT_RGBA_8 = 0x02200016,
    PEAK_IPL_PIXEL_FORMAT_RGBA_10 = 0x0240005F,
    PEAK_IPL_PIXEL_FORMAT_RGBA_12 = 0x02400061,

    PEAK_IPL_PIXEL_FORMAT_BGRA_8 = 0x02200017,
    PEAK_IPL_PIXEL_FORMAT_BGRA_10 = 0x0240004C,
    PEAK_IPL_PIXEL_FORMAT_BGRA_12 = 0x0240004E,

    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_10_PACKED = 0x010A0052,
    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_12_PACKED = 0x010C0053,

    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_10_PACKED = 0x010A0054,
    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_12_PACKED = 0x010C0055,

    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_10_PACKED = 0x010A0056,
    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_12_PACKED = 0x010C0057,

    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_10_PACKED = 0x010A0058,
    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_12_PACKED = 0x010C0059,

    PEAK_IPL_PIXEL_FORMAT_MONO_10_PACKED = 0x010A0046,
    PEAK_IPL_PIXEL_FORMAT_MONO_12_PACKED = 0x010C0047,

    PEAK_IPL_PIXEL_FORMAT_RGB_10_PACKED_32 = 0x0220001D,

    PEAK_IPL_PIXEL_FORMAT_BGR_10_PACKED_32 = 0x0220001E,

    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_10_GROUPED_40_IDS = 0x40000001,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_10_GROUPED_40_IDS = 0x40000002,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_10_GROUPED_40_IDS = 0x40000003,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_10_GROUPED_40_IDS = 0x40000004,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_RG_12_GROUPED_24_IDS = 0x40000011,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_GB_12_GROUPED_24_IDS = 0x40000012,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_GR_12_GROUPED_24_IDS = 0x40000013,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_BAYER_BG_12_GROUPED_24_IDS = 0x40000014,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_MONO_10_GROUPED_40_IDS = 0x4000000f,
    /*!
     * \attention This pixel format is preliminary, and its name and value may change in a future product version.
     */
    PEAK_IPL_PIXEL_FORMAT_MONO_12_GROUPED_24_IDS = 0x4000001f,
};
typedef int32_t PEAK_IPL_PIXEL_FORMAT;

/*! The enum holding the possible bayer patterns. */
enum PEAK_IPL_BAYER_PATTERN_t
{
    PEAK_IPL_BAYER_PATTERN_UNKNOWN,
    PEAK_IPL_BAYER_PATTERN_NONE,
    PEAK_IPL_BAYER_PATTERN_BAYER_GR,
    PEAK_IPL_BAYER_PATTERN_BAYER_RG,
    PEAK_IPL_BAYER_PATTERN_BAYER_GB,
    PEAK_IPL_BAYER_PATTERN_BAYER_BG
};
typedef int32_t PEAK_IPL_BAYER_PATTERN;

/*! The enum holding the possible endianness types. */
enum PEAK_IPL_ENDIANNESS_t
{
    PEAK_IPL_ENDIANNESS_UNKNOWN,
    PEAK_IPL_ENDIANNESS_LITTLE_ENDIAN,
    PEAK_IPL_ENDIANNESS_BIG_ENDIAN
};
typedef int32_t PEAK_IPL_ENDIANNESS;

/*! The enum holding the possible orientations. */
enum PEAK_IPL_ORIENTATION_t
{
    PEAK_IPL_ORIENTATION_HORIZONTAL,
    PEAK_IPL_ORIENTATION_VERTICAL
};
typedef int32_t PEAK_IPL_ORIENTATION;

/*! Sensitivity parameter for the hotpixel correction algorithm */
enum PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_t
{
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_INVALID,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_LEVEL1,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_LEVEL2,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_LEVEL3,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_LEVEL4,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY_LEVEL5
};
typedef int32_t PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY;

/*! Rotation angle for the image transformer rotation algorithm*/
enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t
{
    LIMGTYPEIMAGE_TRANSFORMER_ROTATION_ANGLE_DEGREE_90_COUNTERCLOCKWISE = 90,
    LIMGTYPEIMAGE_TRANSFORMER_ROTATION_ANGLE_DEGREE_90_CLOCKWISE = 270,
    LIMGTYPEIMAGE_TRANSFORMER_ROTATION_ANGLE_DEGREE_180 = 180
};
typedef uint16_t PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE;

enum PEAK_IPL_BINNINGMODE_t
{
    Average = 0,
    Sum = 1
};
typedef uint16_t PEAK_IPL_BINNINGMODE;

enum PEAK_IPL_VIDEO_CONTAINER_t
{
    PEAK_IPL_VIDEO_CONTAINER_AVI = 0
};
typedef uint16_t PEAK_IPL_VIDEO_CONTAINER;

enum PEAK_IPL_VIDEO_ENCODER_t
{
    PEAK_IPL_VIDEO_ENCODER_RAW = 0,
    PEAK_IPL_VIDEO_ENCODER_MJPEG = 1
};
typedef uint16_t PEAK_IPL_VIDEO_ENCODER;

enum PEAK_IPL_VIDEO_ENCODER_OPTION_t
{
    PEAK_IPL_VIDEO_ENCODER_OPTION_QUALITY = 0
};
typedef uint16_t PEAK_IPL_VIDEO_ENCODER_OPTION;

enum PEAK_IPL_VIDEO_CONTAINER_OPTION_t
{
    PEAK_IPL_VIDEO_CONTAINER_OPTION_FRAMERATE = 0
};
typedef uint16_t PEAK_IPL_VIDEO_CONTAINER_OPTION;

/*! Position of a pixel in an image */
struct PEAK_IPL_POINT_2D
{
    size_t x;
    size_t y;
};

struct PEAK_IPL_SIZE_2D
{
    size_t width;
    size_t height;
};

struct PEAK_IPL_RECT_2D
{
    struct PEAK_IPL_POINT_2D position;
    struct PEAK_IPL_SIZE_2D size;
};

enum PEAK_IPL_SHARPNESS_ALGORITHM_t
{
    PEAK_IPL_SHARPNESS_ALGORITHM_TENENGRAD,
    PEAK_IPL_SHARPNESS_ALGORITHM_MEAN_SCORE,
    PEAK_IPL_SHARPNESS_ALGORITHM_HISTOGRAM_VARIANCE,
    PEAK_IPL_SHARPNESS_ALGORITHM_SOBEL
};
typedef uint16_t PEAK_IPL_SHARPNESS_ALGORITHM;

enum PEAK_IPL_SHARPNESS_ROI_WEIGHT_t
{
    PEAK_IPL_SHARPNESS_ROI_WEIGHT_WEAK = 0x0021,
    PEAK_IPL_SHARPNESS_ROI_WEIGHT_MEDIUM = 0x0042,
    PEAK_IPL_SHARPNESS_ROI_WEIGHT_STRONG = 0x0063
};
typedef uint16_t PEAK_IPL_SHARPNESS_ROI_WEIGHT;

enum PEAK_IPL_INTERPOLATION_ALGORITHM_t
{
    PEAK_IPL_INTERPOLATION_NEAREST,
    PEAK_IPL_INTERPOLATION_LINEAR,
    PEAK_IPL_INTERPOLATION_CUBIC,
    PEAK_IPL_INTERPOLATION_AREA,
    PEAK_IPL_INTERPOLATION_LANCZOS4,
    PEAK_IPL_INTERPOLATION_LINEAR_EXACT,
    PEAK_IPL_INTERPOLATION_NEAREST_EXACT
};
typedef int32_t PEAK_IPL_INTERPOLATION_ALGORITHM;

struct PEAK_IPL_SHARPNESS_ROI
{
    struct PEAK_IPL_RECT_2D rect;
    uint8_t xSamplingInterval;
    uint8_t ySamplingInterval;
    PEAK_IPL_SHARPNESS_ROI_WEIGHT weight;
};

struct PEAK_IPL_RANGE
{
    uint32_t min;
    uint32_t max;
    uint32_t inc;
};

struct PEAK_IPL_IMAGE_CONVERTER;
/*! The type of image converter handles. */
typedef struct PEAK_IPL_IMAGE_CONVERTER* PEAK_IPL_IMAGE_CONVERTER_HANDLE;

struct PEAK_IPL_IMAGE_transformer;
/*! The type of image transformer handles. */
typedef struct PEAK_IPL_IMAGE_transformer* PEAK_IPL_IMAGE_TRANSFORMER_HANDLE;

struct PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR;
/*! The type of adaptive hotpixel corrector handles. */
typedef struct PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR* PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE;

struct PEAK_IPL_BINNING;
/*! The type of binning handles. */
typedef struct PEAK_IPL_BINNING* PEAK_IPL_BINNING_HANDLE;

struct PEAK_IPL_DECIMATION;
/*! The type of decimation handles. */
typedef struct PEAK_IPL_DECIMATION* PEAK_IPL_DECIMATION_HANDLE;

struct PEAK_IPL_COLOR_CORRECTOR;
/*! The type of color corrector handles. */
typedef struct PEAK_IPL_COLOR_CORRECTOR* PEAK_IPL_COLOR_CORRECTOR_HANDLE;

struct PEAK_IPL_GAIN;
/*! The type of gain corrector handles. */
typedef struct PEAK_IPL_GAIN* PEAK_IPL_GAIN_HANDLE;

struct PEAK_IPL_GAMMA_CORRECTOR;
/*! The type of gamma corrector handles. */
typedef struct PEAK_IPL_GAMMA_CORRECTOR* PEAK_IPL_GAMMA_CORRECTOR_HANDLE;

struct PEAK_IPL_IMAGE;
/*! The type of image handles. */
typedef struct PEAK_IPL_IMAGE* PEAK_IPL_IMAGE_HANDLE;

struct PEAK_IPL_HISTOGRAM;
/*! The type of histogram handles. */
typedef struct PEAK_IPL_HISTOGRAM* PEAK_IPL_HISTOGRAM_HANDLE;

struct PEAK_IPL_PIXEL_LINE;
/*! The type of pixel line handles. */
typedef struct PEAK_IPL_PIXEL_LINE* PEAK_IPL_PIXEL_LINE_HANDLE;

struct PEAK_IPL_VIDEO;
/*! The type of video writer handles. */
typedef struct PEAK_IPL_VIDEO* PEAK_IPL_VIDEO_HANDLE;

struct PEAK_IPL_SHARPNESS;
/*! The type of sharpness handles. */
typedef struct PEAK_IPL_SHARPNESS* PEAK_IPL_SHARPNESS_HANDLE;

struct PEAK_IPL_EDGE_ENHANCEMENT;
/*! The type of edge_enhancement handles. */
typedef struct PEAK_IPL_EDGE_ENHANCEMENT* PEAK_IPL_EDGE_ENHANCEMENT_HANDLE;


#define PEAK_IPL_C_API PEAK_IPL_NO_DISCARD PEAK_IPL_PUBLIC PEAK_IPL_RETURN_CODE PEAK_IPL_CALL_CONV

#ifndef PEAK_IPL_DYNAMIC_LOADING
/*!
 * \brief Queries the library major version.
 *
 * \param[out] libraryVersionMajor The major version of the library.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT libraryVersionMajor is a null pointer
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetVersionMajor(uint32_t* libraryVersionMajor);

/*!
 * \brief Queries the library minor version.
 *
 * \param[out] libraryVersionMinor The minor version of the library.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT libraryVersionMinor is a null pointer
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetVersionMinor(uint32_t* libraryVersionMinor);

/*!
 * \brief Queries the library subminor version.
 *
 * \param[out] libraryVersionSubminor The subminor version of the library.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT libraryVersionSubminor is a null pointer
 *
 * \since 1.1
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetVersionSubminor(uint32_t* libraryVersionSubminor);

/*!
 * \brief Queries the maximum number of concurrent threads used by the library.
 *
 * \param[out] concurrentThreads The mumber of concurrent threads used by the library.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT concurrentThreads is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetMaxConcurrentThreads(size_t* concurrentThreads);

/*!
 * \brief Sets the maximum number of concurrent threads used by the library.
 *
 * Set the number of threads to 0, to let the library decide the amount. Select any other number to set the amount
 * manually.
 *
 * \param[in] concurrentThreads The number of concurrent threads used by the library.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_Library_SetMaxConcurrentThreads(size_t concurrentThreads);

/*!
 * \brief Disables/Enables multithreading in the library
 *
 * \param[out] enabled true if multithreading is enabled, false if not
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT concurrentThreads is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetMultiThreadingEnabled(PEAK_IPL_BOOL8* enabled);

/*!
 * \brief Disables/Enables multithreading in the library
 *
 * \param[in] enabled set to true to enable multithreading, false to disable it
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_Library_SetMultiThreadingEnabled(PEAK_IPL_BOOL8 enabled);

/*!
 * \brief Disables/Enables SIMD in the library
 *
 * \param[out] enabled SIMD optimized versions are used if this is true, otherwise they're not used
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT concurrentThreads is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetSIMDEnabled(PEAK_IPL_BOOL8* enabled);

/*!
 * \brief Disables/Enables SIMD in the library
 *
 * \param[in] enabled set to true to use the SIMD optimized functions, false to disable them
 *
 * \note This is normally true
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_Library_SetSIMDEnabled(PEAK_IPL_BOOL8 enabled);

/*!
 * \brief Queries the last error.
 *
 * This function is normally used by applying a two-step procedure. First of all, call the function with all
 * arguments except of lastErrorDescription.
 * \code
 *   // Error handling is omitted
 *   PEAK_IPL_RETURN_CODE lastErrorCode = PEAK_IPL_RETURN_CODE_SUCCESS;
 *   size_t size = 0;
 *   PEAK_IPL_GetLastError(&lastErrorCode, NULL, &size);
 * \endcode
 * The function then gives you the last error code and the size of the error description. You could stop now if you only
 * want to query the last error code. If you want to query the error description as well, you have to go on.
 * \code
 *   // Error handling is omitted
 *   char errorDescription[size];
 *   PEAK_IPL_GetLastError(&returnCode, errorDescription, &size);
 * \endcode
 *
 * This two-step procedure may not be necessary if you just pass a buffer big enough for holding the description at the
 * first function call.
 *
 * \param[out]    lastErrorCode            The last function error code.
 * \param[out]    lastErrorDescription     The description for the last error.
 * \param[in,out] lastErrorDescriptionSize IN: Size of the given buffer - OUT: Size of the queried data
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT At least one of the arguments is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL lastErrorDescription is valid but lastErrorDescriptionSize is too small
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Library_GetLastError(
    PEAK_IPL_RETURN_CODE* lastErrorCode, char* lastErrorDescription, size_t* lastErrorDescriptionSize);

/*!
 * \brief Creates an image converter.
 *
 * \note To speed up processing image converters maintain internal memory pools to reuse
 * memory instead of allocating new memory for each conversion. The memory is freed when all image converter objects
 * are destroyed using PEAK_IPL_ImageConverter_Destruct().
 *
 * \param[out] imageConverterHandle The handle to the created image converter.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_Construct(PEAK_IPL_IMAGE_CONVERTER_HANDLE* imageConverterHandle);

/*!
 * \brief Increase the reference count of an image converter.
 *
 * \note To speed up processing image converters maintain internal memory pools to reuse
 * memory instead of allocating new memory for each conversion. The memory is freed when all image converter objects
 * are destroyed using PEAK_IPL_ImageConverter_Destruct().
 *
 * \param[in] imageConverterHandle The handle to an image converter.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_Acquire(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle);

/*! \brief Pre-allocates internal buffer pools for the specified conversion.
 *
 * \note You can use the image converter without pre-allocating the internal buffers, but keep in mind that the
 *       first conversion of a new image size or format will need to allocate the buffers if not pre-allocated.
 *       This results in the first conversion taking more time than normal.
 *
 * \param[in] imageConverterHandle The handle of the image converter to be used.
 * \param[in] inputPixelFormat     The pixelformat of the source image
 * \param[in] outputPixelFormat    The pixelformat of the converted image
 * \param[in] width                Width of the source image
 * \param[in] height               Height of the source image
 * \param[in] imageCount           Count of converted images held at one time
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_PreAllocateConversion(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle,
    PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, size_t width, size_t height,
    size_t imageCount);

/*!
 * \brief Queries the given image converter's current conversion mode.
 *
 * \param[in]  imageConverterHandle The handle to the image converter of interest.
 * \param[out] conversionMode       The current conversion mode.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageConverterHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT conversionMode is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_GetConversionMode(
    PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE* conversionMode);

/*!
 * \brief Sets the given image converter's conversion mode.
 *
 * \param[in] imageConverterHandle The handle to the image converter of interest.
 * \param[in] conversionMode       The conversion mode to set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE imageConverterHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_SetConversionMode(
    PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE conversionMode);

/*!
 * \brief Queries the supported output pixel formats for a given input pixel format.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     imageConverterHandle   The handle to the image converter of interest.
 * \param[in]     inputPixelFormat       The input pixel format.
 * \param[out]    outputPixelFormats     The list of supported output pixel formats.
 * \param[in,out] outputPixelFormatsSize IN: Size of the given buffer - OUT: Size of the queried data
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageConverterHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT size is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL outputPixelFormats is valid but size is too small<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle,
    PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT* outputPixelFormats, size_t* outputPixelFormatsSize);

/*!
 * \brief Creates a new image containing the data of the input image converted to the given pixel format.
 *
 * \param[in]  imageConverterHandle The handle to the image converter of interest.
 * \param[in]  inputImageHandle     The handle to the created image.
 * \param[in]  outputPixelFormat    The output pixel format.
 * \param[out] outputImageHandle    The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   At least one of imageConverterHandle and inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_Convert(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle,
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat,
    PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Creates a new image containing the data of the input image converted to the given pixel format.
 *
 * \param[in]  imageConverterHandle      The handle to the image converter of interest.
 * \param[in]  inputImageHandle          The handle to the input image.
 * \param[in]  outputPixelFormat         The output pixel format.
 * \param[out] outputImageBuffer         Pointer to the destination buffer.
 * \param[in]  outputImageBufferSize     Size of the destination buffer.
 * \param[out] outputImageHandle         The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   At least one of imageConverterHandle and inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT At least one of outputImageBuffer and outputImageHandle is a null
 *                                              pointer<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL The output buffer size is too small regarding width, height and
 *                                              pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.1
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_ConvertToBuffer(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle,
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t* outputImageBuffer,
    size_t outputImageBufferSize, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Destroys the image converter associated with the given handle.
 *
 * \param[in] imageConverterHandle The handle to the image converter of interest.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE imageConverterHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageConverter_Destruct(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle);

/*!
 * \brief Creates an adaptive hotpixel corrector algorithm.
 *
 * \param[out] adaptiveHotpixelCorrectorHandle The handle to the created adaptive hotpixel corrector algorithm.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Construct(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE* adaptiveHotpixelCorrectorHandle);

/*!
 * \brief Increase the reference count of an adaptive hotpixel corrector.
 *
 * \param[in] adaptiveHotpixelCorrectorHandle The handle to an adaptive hotpixel corrector object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Acquire(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle);

/*!
 * \brief Sets the given adaptive hotpixel corrector algorithm's sensitivity.
 *
 * \param[in] adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in] sensitivityLevel                The sensitivity level to set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE adaptiveHotpixelCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY sensitivityLevel);
/*!
 * \brief Queries the given adaptive hotpixel corrector algorithm's current sensitivity.
 *
 * \param[in]  adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[out] sensitivityLevel                The current sensitivity level factor.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   adaptiveHotpixelCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT sensitivityLevel is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle,
    PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY* sensitivityLevel);

/*!
 * \brief Sets the given adaptive hotpixel corrector algorithm's gain factor in percent.
 *
 * \param[in] adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in] gainFactorPercent               The gain factor in percent to set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE adaptiveHotpixelCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t gainFactorPercent);

/*!
 * \brief Queries the given adaptive hotpixel corrector algorithm's current gain factor in percent.
 *
 * \param[in]  adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[out] gainFactorPercent               The current gain factor in percent.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   adaptiveHotpixelCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT gainFactorPercent is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t* gainFactorPercent);

/*!
 * \brief Detects hotpixels in the given image.
 *
 * \param[in] adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in] inputImageHandle                The handle to the input image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             At least one of adaptiveHotpixelCorrectorHandle and
 *                                                        inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with inputImageHandle has a packed pixel
 *                                                        format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Detect(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle);

/*!
 * \brief Queries previously detected hotpixels.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[out]    hotpixels                       The list of hotpixels.
 * \param[in,out] hotpixelsSize                   IN: Size of the given buffer - OUT: Size of the queried data
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   adaptiveHotpixelCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT hotpixelsSize is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL hotpixels are valid but the size is too small<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, struct PEAK_IPL_POINT_2D* hotpixels,
    size_t* hotpixelsSize);

/*!
 * \brief Corrects the given hotpixels in the given image.
 *
 * \param[in]  adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in]  inputImageHandle                The handle to the input image.
 * \param[in]  hotpixels                       The list of hotpixels to be corrected.
 * \param[in]  hotpixelsSize                   Size of the given hotpixels list.
 * \param[out] outputImageHandle               The handle associated with the image containing the corrected data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             At least one of adaptiveHotpixelCorrectorHandle and
 *                                                        inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with inputImageHandle has packed pixel
 *                                                        format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Correct(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle,
    const struct PEAK_IPL_POINT_2D* hotpixels, size_t hotpixelsSize, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Corrects the given hotpixels in place of the given image.
 *
 * \param[in]  adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in]  inputImageHandle                The handle to the input image.
 * \param[in]  hotpixels                       The list of hotpixels to be corrected.
 * \param[in]  hotpixelsSize                   Size of the given hotpixels list.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             At least one of adaptiveHotpixelCorrectorHandle and
 *                                                        inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with inputImageHandle has packed pixel
 *                                                        format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle,
    const struct PEAK_IPL_POINT_2D* hotpixels, size_t hotpixelsSize);

/*!
 * \brief Detects and corrects hotpixels in the given image.
 *
 * \param[in]  adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in]  inputImageHandle                The handle to the input image.
 * \param[out] outputImageHandle               The handle associated with the image containing the corrected data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             At least one of adaptiveHotpixelCorrectorHandle and
 *                                                        inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with inputImageHandle has packed pixel
 *                                                        format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle,
    PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Detects and corrects hotpixels in place within the given image.
 *
 * \param[in]  adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 * \param[in]  inputImageHandle                The handle to the input image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             At least one of adaptiveHotpixelCorrectorHandle and
 *                                                        inputImageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with inputImageHandle has packed pixel
 *                                                        format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle);

/*!
 * \brief Destroys the adaptive hotpixel corrector algorithm associated with the given handle.
 *
 * \param[in] adaptiveHotpixelCorrectorHandle The handle to the adaptive hotpixel corrector algorithm of interest.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE adaptiveHotpixelCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(
    PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle);

/*!
 * \brief Creates a color corrector.
 *
 * \param[out] colorCorrectorHandle The handle to the created color corrector.
 *
 * \return IMG_RETURN_CODE_SUCCESS Success<br>
 *         IMG_RETURN_CODE_ERROR   Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Construct(PEAK_IPL_COLOR_CORRECTOR_HANDLE* colorCorrectorHandle);

/*!
 * \brief Increase the reference count of a color corrector.
 *
 * \param[in] colorCorrectorHandle The handle to a color corrector object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Acquire(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle);

/*!
 * \brief Sets the factors of the color correction matrix.
 *
 * \param[in] colorCorrectorHandle      The handle to the color corrector.
 * \param[in] colorCorrectorFactors     The color corrector factors that are set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE colorCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_SetColorCorrectionFactors(
    PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float* colorCorrectorFactors);

/*!
 * \brief Returns the color corrector 3x3 matrix factors and its size
 *
 * \param[in]  colorCorrectorHandle             The handle to the color corrector
 * \param[out] colorCorrectorFactors            The current color corrector factors.
 * \param[out] colorCorrectorFactorsSize        The current color corrector factors size, default 3x3 = 9.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   colorCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT colorCorrectorFactors and/or colorCorrectorFactorsSize is/are a null
 *                                              pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetColorCorrectionFactors(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle,
    float* colorCorrectorFactors, size_t* colorCorrectorFactorsSize);

/*!
 * \brief Asks the given color corrector whether it supports the given pixel format.
 *
 * \param[in]  colorCorrectorHandle   The handle to the color corrector to use.
 * \param[in]  pixelFormat            The pixel format of interest.
 * \param[out] isPixelFormatSupported The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE colorCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle,
    PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

/*!
 * \brief Corrects the colors of the given image.
 *
 * \param[in]  colorCorrectorHandle The handle to the color corrector to use.
 * \param[in]  imageHandle          The handle to the image on which the color correction matrix is applied.
 * \param[out] outputImageHandle    The handle to the image on which the color correction matrix was applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             colorCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has an unsupported pixel
 * format (e.g. packed pixel format)<br> PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Process(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Corrects the colors of the given image.
 *
 * \param[in] colorCorrectorHandle The handle to the color corrector to use.
 * \param[in] imageHandle          The handle to the image on which the color correction matrix is applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             colorCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has an unsupported pixel
 * format (e.g. packed pixel format)<br> PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_ProcessInPlace(
    PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Destroys the color corrector associated with the given handle.
 *
 * \param[in] colorCorrectorHandle The handle to the color corrector to destroy.
 *
 * \return IMG_RETURN_CODE_SUCCESS        Success<br>
 *         IMG_RETURN_CODE_INVALID_HANDLE colorCorrectorHandle is invalid<br>
 *         IMG_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Destruct(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle);

/*!
 * \brief Returns the color saturation value.
 *
 * \param[in]  colorCorrectorHandle             The handle to the color corrector.
 * \param[out] saturation                       The current color saturation value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   colorCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT saturation is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetSaturation(
    PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float* saturation);

/*!
 * \brief Returns the minimum color saturation value.
 *
 * \param[in]  colorCorrectorHandle             The handle to the color corrector.
 * \param[out] saturationMin                    The minimum color saturation value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   colorCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT saturationMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetSaturationMin(
    PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float* saturationMin);

/*!
 * \brief Returns the maximum color saturation value.
 *
 * \param[in]  colorCorrectorHandle             The handle to the color corrector.
 * \param[out] saturationMax                    The maximum color saturation value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   colorCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT saturationMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetSaturationMax(
    PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float* saturationMax);

/*!
 * \brief Sets the color saturation value.
 *
 * \param[in] colorCorrectorHandle      The handle to the color corrector.
 * \param[in] saturation                The color saturation value to be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE colorCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ColorCorrector_SetSaturation(
    PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float saturation);

/*!
 * \brief Creates a binning.
 *
 * \param[out]                     binningHandle The handle to the created binning.
 *
 * \return IMG_RETURN_CODE_SUCCESS Success<br>
 *         IMG_RETURN_CODE_ERROR   Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_Construct(PEAK_IPL_BINNING_HANDLE* binningHandle);

/*!
 * \brief Increase the reference count of a binning object.
 *
 * \param[in] binningHandle The handle to a binning object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_Binning_Acquire(PEAK_IPL_BINNING_HANDLE binningHandle);

/*!
 * \brief Sets the value of binning in horizontal direction.
 *
 * \param[in] binningHandle                     The handle to the binning.
 * \param[in] binningXValue                     The horizontal binning value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningXValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_SetBinningX(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t* binningXValue);

/*!
 * \brief Returns the value of binning in horizontal direction.
 *
 * \param[in]  binningHandle                    The handle to the binning
 * \param[out] binningXValue                    The value of binning in horizontal direction.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningXValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningX(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t* binningXValue);

/*!
 * \brief Sets the value of binning in vertical direction.
 *
 * \param[in] binningHandle                     The handle to the binning.
 * \param[in] binningYValue                     The vertical binning value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningYValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_SetBinningY(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t* binningYValue);

/*!
 * \brief Returns the value of binning in vertical direction.
 *
 * \param[in]  binningHandle                    The handle to the binning
 * \param[out] binningYValue                    The value of binning in vertical direction.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningYValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningY(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t* binningYValue);

/*!
 * \brief Returns the minimum binning value which can be set.
 *
 * \param[in]  binningHandle                    The handle to the binning
 * \param[out] binningMin                       The minimum binning value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningMin(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t* binningMin);

/*!
 * \brief Returns the maximum binning value which can be set.
 *
 * \param[in]  binningHandle                    The handle to the binning
 * \param[out] binningMax                       The maximum binning value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningMax(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t* binningMax);

/*!
 * \brief Returns the size of a binned image given the input size and pixelformat.
 *
 * \param[in]  binningHandle                    The handle to the binning
 * \param[out] outputWidth                      The expected width of the binned image.
 * \param[out] outputHeight                     The expected height of the binned image.
 * \param[in]  inputWidth                       The width of the input image.
 * \param[in]  inputHeight                      The height of the input image.
 * \param[in]  pixelFormat                      The pixelFormat of the input image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputWidth or outputHeight is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.4
 */
PEAK_IPL_C_API PEAK_IPL_Binning_ExpectedOutputSize(PEAK_IPL_BINNING_HANDLE binningHandle, size_t* outputWidth,
    size_t* outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat);

/*!
 * \brief Asks the given binning whether it supports the given pixel format.
 *
 * \param[in]  binningHandle                    The handle to the binning to use.
 * \param[in]  pixelFormat                      The pixel format of interest.
 * \param[out] isPixelFormatSupported           The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT isPixelFormatSupported is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_IsPixelformatSupported(
    PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

/*!
 * \brief Sets the value of binning mode.
 *
 * \param[in] binningHandle                     The handle to the binning.
 * \param[in] mode                              The binning mode that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningYValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_SetBinningMode(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE* mode);

/*!
 * \brief Returns the value of binning mode.
 *
 * \param[in]  binningHandle                    The handle to the binning
 * \param[out] mode                             The value of binning mode.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   binningHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT binningYValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningMode(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE* mode);

/*!
 * \brief Apply the binning on the given image.
 *
 * \param[in]  binningHandle                              The handle to the binning to use.
 * \param[in]  imageHandle                                The handle to the image on which the binning is applied.
 * \param[out] outputImageHandle                          The handle to the image on which the binning was applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             binningHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a too small image<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel
 *                                                        format.<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_Process(
    PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Destroys the binning associated with the given handle.
 *
 * \param[in] binningHandle               The handle to the binning to destroy.
 *
 * \return IMG_RETURN_CODE_SUCCESS        Success<br>
 *         IMG_RETURN_CODE_INVALID_HANDLE binningHandle is invalid<br>
 *         IMG_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.3.3
 */
PEAK_IPL_C_API PEAK_IPL_Binning_Destruct(PEAK_IPL_BINNING_HANDLE binningHandle);

/*!
 * \brief Creates a decimation.
 *
 * \param[out] decimationHandle    The handle to the created decimation.
 *
 * \return IMG_RETURN_CODE_SUCCESS Success<br>
 *         IMG_RETURN_CODE_ERROR   Something went wrong
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_Construct(PEAK_IPL_DECIMATION_HANDLE* decimationHandle);

/*!
 * \brief Increase the reference count of a decimation object.
 *
 * \param[in] decimationHandle The handle to a decimation object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_Acquire(PEAK_IPL_DECIMATION_HANDLE decimationHandle);

/*!
 * \brief Sets the value of decimation in horizontal direction.
 *
 * \param[in] decimationHandle                  The handle to the decimation.
 * \param[in] decimationXValue                  The horizontal decimation value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT decimationXValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_SetDecimationX(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t* decimationXValue);

/*!
 * \brief Returns the value of decimation in horizontal direction.
 *
 * \param[in]  decimationHandle                 The handle to the decimation
 * \param[out] decimationXValue                 The value of decimation in horizontal direction.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT decimationXValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationX(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t* decimationXValue);

/*!
 * \brief Sets the value of decimation in vertical direction.
 *
 * \param[in] decimationHandle                  The handle to the decimation.
 * \param[in] decimationYValue                  The vertical decimation value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT decimationYValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_SetDecimationY(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t* decimationYValue);

/*!
 * \brief Returns the value of decimation in vertical direction.
 *
 * \param[in]  decimationHandle                 The handle to the decimation
 * \param[out] decimationYValue                 The value of decimation in vertical direction.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT decimationYValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationY(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t* decimationYValue);

/*!
 * \brief Returns the minimum decimation value which can be set.
 *
 * \param[in]  decimationHandle                 The handle to the decimation
 * \param[out] decimationMin                    The minimum decimation value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT decimationMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationMin(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t* decimationMin);

/*!
 * \brief Returns the maximum decimation value which can be set.
 *
 * \param[in]  decimationHandle                 The handle to the decimation
 * \param[out] decimationMax                    The maximum decimation value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT decimationMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationMax(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t* decimationMax);

/*!
 * \brief Returns the size of a binned image given the input size and pixelformat.
 *
 * \param[in]  decimationHandle                 The handle to the decimation
 * \param[out] outputWidth                      The expected width of the binned image.
 * \param[out] outputHeight                     The expected height of the binned image.
 * \param[in] inputWidth                        The width of the input image.
 * \param[in] inputHeight                       The height of the input image.
 * \param[in] pixelFormat                       The pixelFormat of the input image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputWidth or outputHeight is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_ExpectedOutputSize(PEAK_IPL_DECIMATION_HANDLE decimationHandle, size_t* outputWidth,
    size_t* outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat);

/*!
 * \brief Asks the given decimation whether it supports the given pixel format.
 *
 * \param[in] decimationHandle                  The handle to the decimation to use.
 * \param[in] pixelFormat                       The pixel format of interest.
 * \param[out] isPixelFormatSupported           The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   decimationHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT isPixelFormatSupported is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_IsPixelformatSupported(PEAK_IPL_DECIMATION_HANDLE decimationHandle,
    PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

/*!
 * \brief Apply the decimation on the given image.
 *
 * \param[in]  decimationHandle                           The handle to the decimation to use.
 * \param[in]  imageHandle                                The handle to the image on which the decimation is applied.
 * \param[out] outputImageHandle                          The handle to the image on which the decimation was applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             decimationHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a too small image<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel
 *                                                        format.<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_Process(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_IMAGE_HANDLE imageHandle,
    PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Destroys the decimation associated with the given handle.
 *
 * \param[in] decimationHandle            The handle to the decimation to destroy.
 *
 * \return IMG_RETURN_CODE_SUCCESS        Success<br>
 *         IMG_RETURN_CODE_INVALID_HANDLE decimationHandle is invalid<br>
 *         IMG_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.5.0
 */
PEAK_IPL_C_API PEAK_IPL_Decimation_Destruct(PEAK_IPL_DECIMATION_HANDLE decimationHandle);

/*!
 * \brief Creates a gain.
 *
 * \param[out] gainHandle The handle to the created gain.
 *
 * \return IMG_RETURN_CODE_SUCCESS Success<br>
 *         IMG_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.2.2
 */

PEAK_IPL_C_API PEAK_IPL_Gain_Construct(PEAK_IPL_GAIN_HANDLE* gainHandle);

/*!
 * \brief Increase the reference count of a gain object.
 *
 * \param[in] gainHandle The handle to a gain object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_Gain_Acquire(PEAK_IPL_GAIN_HANDLE gainHandle);

/*!
 * \brief Sets the value of the master gain.
 *
 * \param[in] gainHandle                        The handle to the gain.
 * \param[in] masterGainValue                   The master gain value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_SetMasterGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* masterGainValue);

/*!
 * \brief Returns the master gain value.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] masterGainValue                  The current master gain value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT masterGainValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetMasterGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* masterGainValue);

/*!
 * \brief Returns the maximum master gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] masterGainMax                    The maximum master gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT masterGainMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetMasterGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float* masterGainMax);

/*!
 * \brief Returns the minimum master gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the master gain
 * \param[out] masterGainMin                    The minimum master gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT masterGainMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetMasterGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float* masterGainMin);

/*!
 * \brief Sets the value of the red gain.
 *
 * \param[in] gainHandle                        The handle to the gain.
 * \param[in] redGainValue                      The red gain value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_SetRedGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* redGainValue);

/*!
 * \brief Returns the red gain value.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] redGainValue                     The current red gain value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT redGainValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetRedGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* redGainValue);

/*!
 * \brief Returns the maximum red gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] redGainMax                       The maximum red gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT redGainMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetRedGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float* redGainMax);

/*!
 * \brief Returns the minimum red gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the master gain
 * \param[out] redGainMin                       The minimum red gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT redGainMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetRedGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float* redGainMin);

/*!
 * \brief Sets the value of the green gain.
 *
 * \param[in] gainHandle                        The handle to the gain.
 * \param[in] greenGainValue                    The green gain value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_SetGreenGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* greenGainValue);

/*!
 * \brief Returns the green gain value.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] greenGainValue                   The current green gain value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT greenGainValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetGreenGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* greenGainValue);

/*!
 * \brief Returns the maximum green gain value which can be set.
 *
 * \param[in]  gainHandle                      The handle to the gain
 * \param[out] greenGainMax                    The maximum green gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT greenGainMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetGreenGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float* greenGainMax);

/*!
 * \brief Returns the minimum green gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the master gain
 * \param[out] greenGainMin                     The minimum green gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT greenGainMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetGreenGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float* greenGainMin);

/*!
 * \brief Sets the value of the blue gain.
 *
 * \param[in] gainHandle                        The handle to the gain.
 * \param[in] blueGainValue                     The blue gain value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_SetBlueGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* blueGainValue);

/*!
 * \brief Returns the blue gain value.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] blueGainValue                    The current blue gain value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT blueGainValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetBlueGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float* blueGainValue);

/*!
 * \brief Returns the maximum blue gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the gain
 * \param[out] blueGainMax                      The maximum blue gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT blueGainMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetBlueGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float* blueGainMax);

/*!
 * \brief Returns the minimum blue gain value which can be set.
 *
 * \param[in]  gainHandle                       The handle to the master gain
 * \param[out] blueGainMin                      The minimum blue gain value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gainHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT blueGainMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetBlueGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float* blueGainMin);

/*!
 * \brief Asks the given gamma corrector whether it supports the given pixel format.
 *
 * \param[in]  gainHandle             The handle to the gamma corrector to use.
 * \param[in]  pixelFormat            The pixel format of interest.
 * \param[out] isPixelFormatSupported The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE gammaCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_GetIsPixelFormatSupported(
    PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

/*!
 * \brief Apply the gain on the given image.
 *
 * \param[in]  gainHandle           The handle to the gamma corrector to use.
 * \param[in]  imageHandle          The handle to the image on which the gamma correction matrix is applied.
 * \param[out] outputImageHandle    The handle to the image on which the gamma correction matrix was applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             gainHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel
 *                                                        format.<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_Process(
    PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Apply the gain on the given image.
 *
 * \param[in] gainHandle           The handle to the gamma corrector to use.
 * \param[in] imageHandle          The handle to the image on which the gamma correction matrix is applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             gainHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel
 *                                                        format.<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_ProcessInPlace(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Destroys the gain associated with the given handle.
 *
 * \param[in] gainHandle The handle to the gamma corrector to destroy.
 *
 * \return IMG_RETURN_CODE_SUCCESS        Success<br>
 *         IMG_RETURN_CODE_INVALID_HANDLE gainHandle is invalid<br>
 *         IMG_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.3.2
 */
PEAK_IPL_C_API PEAK_IPL_Gain_Destruct(PEAK_IPL_GAIN_HANDLE gainHandle);

/*!
 * \brief Creates a gamma corrector.
 *
 * \param[out] gammaCorrectorHandle The handle to the created gamma corrector.
 *
 * \return IMG_RETURN_CODE_SUCCESS Success<br>
 *         IMG_RETURN_CODE_ERROR   Something went wrong
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Construct(PEAK_IPL_GAMMA_CORRECTOR_HANDLE* gammaCorrectorHandle);

/*!
 * \brief Increase the reference count of a gamma corrector object.
 *
 * \param[in] gammaCorrectorHandle The handle to a gamma corrector object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Acquire(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle);

/*!
 * \brief Sets the value of the gamma correction.
 *
 * \param[in] gammaCorrectorHandle      The handle to the gamma corrector.
 * \param[in] gammaValue                The gamma corrector value that is set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE gammaCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_SetGammaCorrectionValue(
    PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float* gammaValue);

/*!
 * \brief Returns the gamma corrector value.
 *
 * \param[in]  gammaCorrectorHandle             The handle to the gamma corrector
 * \param[out] gammaValue                       The current gamma corrector value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gammaCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT gammaValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetGammaCorrectionValue(
    PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float* gammaValue);

/*!
 * \brief Returns the maximum gamma corrector value which can be set.
 *
 * \param[in]  gammaCorrectorHandle             The handle to the gamma corrector
 * \param[out] gammaMax                         The maximum gamma corrector value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gammaCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT gammaMax is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetGammaCorrectionMax(
    PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float* gammaMax);

/*!
 * \brief Returns the minimum gamma corrector value which can be set.
 *
 * \param[in]  gammaCorrectorHandle             The handle to the gamma corrector
 * \param[out] gammaMin                         The minimum gamma corrector value which can be set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   gammaCorrectorHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT gammaMin is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetGammaCorrectionMin(
    PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float* gammaMin);

/*!
 * \brief Asks the given gamma corrector whether it supports the given pixel format.
 *
 * \param[in] gammaCorrectorHandle    The handle to the gamma corrector to use.
 * \param[in] pixelFormat             The pixel format of interest.
 * \param[out] isPixelFormatSupported The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE gammaCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle,
    PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

/*!
 * \brief Corrects the gammas of the given image.
 *
 * \param[in]  gammaCorrectorHandle The handle to the gamma corrector to use.
 * \param[in]  imageHandle          The handle to the image on which the gamma correction matrix is applied.
 * \param[out] outputImageHandle    The handle to the image on which the gamma correction matrix was applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             gammaCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel
 *                                                        format.<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Process(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Corrects the gammas of the given image.
 *
 * \param[in] gammaCorrectorHandle The handle to the gamma corrector to use.
 * \param[in] imageHandle          The handle to the image on which the gamma correction matrix is applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             gammaCorrectorHandle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel
 *                                                        format.<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_ProcessInPlace(
    PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Destroys the gamma corrector associated with the given handle.
 *
 * \param[in] gammaCorrectorHandle The handle to the gamma corrector to destroy.
 *
 * \return IMG_RETURN_CODE_SUCCESS        Success<br>
 *         IMG_RETURN_CODE_INVALID_HANDLE gammaCorrectorHandle is invalid<br>
 *         IMG_RETURN_CODE_ERROR          Something went wrong
 *
 * \since 1.2.2
 */
PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Destruct(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle);

/*!
 * \brief Creates an image.
 *
 * \param[in]  pixelFormat The desired pixel format of the image.
 * \param[in]  width       The desired width of the image.
 * \param[in]  height      The desired height of the image.
 * \param[out] imageHandle The handle to the created image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_Construct(
    PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE* imageHandle);

/*!
 * \brief Increase the reference count of an image object.
 *
 * \param[in] imageHandle The handle to an image object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_Image_Acquire(PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Creates an image from a given buffer.
 *
 * \param[in]  pixelFormat The desired pixel format of the image.
 * \param[in]  buffer      The buffer to use for the image.
 * \param[in]  bufferSize  The size of *buffer.
 * \param[in]  width       The desired width of the image in pixels.
 * \param[in]  height      The desired height of the image in lines.
 * \param[out] imageHandle The handle to the created image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT At least one of buffer and imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_ConstructFromBuffer(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint8_t* buffer, uint64_t bufferSize,
    size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE* imageHandle);

/*!
 * \brief Queries the width of the given image.
 *
 * \param[in]  imageHandle The handle to the image of interest.
 * \param[out] width       The width of the image in pixels.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT width is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something  went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetWidth(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t* width);

/*!
 * \brief Queries the height of the given image.
 *
 * \param[in]  imageHandle The handle to the image of interest.
 * \param[out] height      The height of the image in lines.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT height is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            Something  went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetHeight(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t* height);

/*!
 * \brief Queries the pointer to the given pixel position.
 *
 * \param[in]  imageHandle  The handle to the image of interest.
 * \param[in]  xPos         The x position of the pixel of interest.
 * \param[in]  yPos         The y position of the pixel of interest.
 * \param[out] pixelPointer The pointer to the given pixel position.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pixelPosition is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_OUT_OF_RANGE     At least one of xPos and yPos is out of range<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetPixelPointer(
    PEAK_IPL_IMAGE_HANDLE imageHandle, size_t xPos, size_t yPos, uint8_t** pixelPointer);

/*!
 * \brief Queries the size of the given image in number of bytes.
 *
 * \param[in]  imageHandle The handle to the image of interest.
 * \param[out] byteCount   The size of the given image in number of bytes.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT byteCount is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetByteCount(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t* byteCount);

/*!
 * \brief Queries the pixel format of the given image.
 *
 * \param[in]  imageHandle The handle to the image of interest.
 * \param[out] pixelFormat The pixel format of the given image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pixelFormat is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetPixelFormat(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT* pixelFormat);

/*!
 * \brief Queries the data pointer of the given image.
 *
 * \param[in]  imageHandle The handle to the image of interest.
 * \param[out] data        The data pointer of the given image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT data is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetData(PEAK_IPL_IMAGE_HANDLE imageHandle, uint8_t** data);

/*!
 * \brief Returns a new created image containing the data of the current image as deep copy.
 *
 * \param[in]  imageHandle       The handle to the image of interest.
 * \param[out] outputImageHandle The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_Clone(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Creates a new image containing the data of the current image converted to the given pixel format.
 *
 * \param[in]  imageHandle       The handle to the image of interest.
 * \param[in]  outputPixelFormat The output pixel format.
 * \param[in]  conversionMode    The conversion mode.
 * \param[out] outputImageHandle The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputImageHandle is a null pointer or the mirror direction is
 *                                              invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_ConvertTo(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat,
    PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Creates a new image scaled to \p size using \p algorithm
 *
 * \param[in]  imageHandle          The handle to the image of interest.
 * \param[in]  size                 The size of the output image.
 * \param[in]  algorithm            The used interpolation algorithm.
 * \param[out] outputImageHandle    The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputImageHandle is a null pointer or the mirror direction is
 *                                              invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_Image_Scale(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D* size,
    PEAK_IPL_INTERPOLATION_ALGORITHM algorithm, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Creates a new image scaled to \p size using \p algorithm into the supplied buffer
 *
 * \param[in]  imageHandle          The handle to the image of interest.
 * \param[in]  size                 The size of the output image.
 * \param[in]  buffer               The output buffer for the output image.
 * \param[in]  bufferSize           The size of the output buffer.
 * \param[in]  algorithm            The used interpolation algorithm.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputImageHandle is a null pointer or the mirror direction is
 *                                              invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_Image_ScaleInto(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D* size,
    uint8_t* buffer, size_t bufferSize, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm);


/*!
 * \brief Creates a new image cropped to \p size starting at \p position
 *
 * \param[in]  imageHandle          The handle to the image of interest.
 * \param[in]  pos                  The starting position of the output image.
 * \param[in]  size                 The size of the output image.
 * \param[out] outputImageHandle    The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT outputImageHandle is a null pointer or the mirror direction is
 *                                              invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_Image_Crop(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_POINT_2D* pos,
    const struct PEAK_IPL_SIZE_2D* size, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Stores the data of the current image converted to the given pixel format into a destination buffer
 *        and creates a new image.
 *
 * \param[in]  imageHandle           The handle to the image of interest.
 * \param[in]  outputPixelFormat     The output pixel format.
 * \param[out] outputImageBuffer     Pointer to the destination buffer.
 * \param[in]  outputImageBufferSize Size of the destination buffer.
 * \param[in]  conversionMode        The conversion mode.
 * \param[out] outputImageHandle     The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT At least one of outputImageBuffer and outputImageHandle is a null
 *                                              pointer<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL The output buffer size is too small regarding width, height and
 *                                              pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.1
 */
PEAK_IPL_C_API PEAK_IPL_Image_ConvertToBuffer(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat,
    uint8_t* outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_CONVERSION_MODE conversionMode,
    PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Destroys the image associated with the given handle.
 *
 * \param[in] imageHandle The handle to the image to destroy.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Image_Destruct(PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Creates an image transformer.
 *
 * \note To speed up processing, image transformers maintain internal memory pools to reuse
 * memory instead of allocating new memory for each transformation. The memory is freed when the image transformer is
 * destroyed using PEAK_IPL_ImageTransformer_Destruct().
 *
 * \param[out] imageTransformerHandle The handle to the created image transformer.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Construct(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE* imageTransformerHandle);

/*!
 * \brief Increase the reference count of an image transformer object.
 *
 * \param[in] imageTransformerHandle The handle to an image transformer object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Acquire(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle);

/*!
 * \brief Set the timestamp for an image in nanoseconds
 *
 * \param[in] imageHandle  The handle to the created image.<br>
 * \param[in] timestamp_ns Timestamp in nanoseconds.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_Image_SetTimestamp(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t timestamp_ns);

/*!
 * \brief Get the timestamp for an image in nanoseconds
 *
 * \param[in]  imageHandle  The handle to the created image.<br>
 * \param[out] timestamp_ns Timestamp in nanoseconds.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_Image_GetTimestamp(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t* timestamp_ns);

/*!
 * \brief Creates a new image containing the data of the current image mirrored in up-down direction.
 *
 * If the transformed image is a bayer-format image and the number of rows is even,
 * the format will change. (e.g. BayerBG8 -> BayerGR8)
 *
 * \param[in]  imageTransformerHandle The handle to the image transformer to use.
 * \param[in]  imageHandle            The handle to the created image.
 * \param[out] outputImageHandle      The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDown(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Creates a new image containing the data of the current image mirrored in left-right direction.
 *
 * If the transformed image is a bayer-format image and the number of columns is even,
 * the format will change. (e.g. BayerBG8 -> BayerGB8)
 *
 * \param[in]  imageTransformerHandle The handle to the image transformer to use.
 * \param[in]  imageHandle            The handle to the created image.
 * \param[out] outputImageHandle      The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorLeftRight(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Creates a new image containing the data of the current image mirrored in up-down and left-right direction.
 *
 * If the transformed image is a bayer-format image and the number of rows or columns are even,
 * the format will change. (e.g. BayerBG8 -> BayerRG8)
 *
 * \param[in]  imageTransformerHandle The handle to the image transformer to use.
 * \param[in]  imageHandle            The handle to the created image.
 * \param[out] outputImageHandle      The handle associated with the image containing the converted data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Mirrors the image in up-down direction in place i.e. it will change the input image itself.
 *
 * If the transformed image is a bayer-format image and the number of rows is even,
 * the format will change. (e.g. BayerBG8 -> BayerGR8)
 *
 * \param[in]     imageTransformerHandle The handle to the image transformer to use.
 * \param[in,out] imageHandle            The handle to the image to be mirrored.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageTransformerHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has a packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDownInPlace(
    PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Mirrors the image left-right direction in place i.e. it will change the input image itself.
 *
 * If the transformed image is a bayer-format image and the number of columns is even,
 * the format will change. (e.g. BayerBG8 -> BayerGB8)
 *
 * \param[in]     imageTransformerHandle The handle to the image transformer to use.
 * \param[in,out] imageHandle            The handle to the image to be mirrored.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageTransformerHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has a packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace(
    PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Mirrors the image in up-down and left-right direction in place i.e. it will change the input image itself.
 *
 * If the transformed image is a bayer-format image and the number of rows or columns are even,
 * the format will change. (e.g. BayerBG8 -> BayerRG8)
 *
 * \param[in]     imageTransformerHandle The handle to the image transformer to use.
 * \param[in,out] imageHandle            The handle to the image to be mirrored.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageTransformerHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has a packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace(
    PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Creates a new image containing the data of the current image rotated in the supplied direction and
 * value of the rotation angle.
 *
 * If the transformed image is a bayer-format image and the number of rows or columns are even,
 * the format will change. (e.g. 90_COUNTERCLOCKWISE & an image with even width: BayerGB8 -> BayerBG8)
 *
 * \param[in]  imageTransformerHandle The handle to the image transformer to use.
 * \param[in]  imageHandle            The handle to the created image.
 * \param[in]  rotationAngle          The rotation angle for the image transformer rotation algorithm
 * \param[out] outputImageHandle      The handle associated with the image containing the rotated data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           outputImageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Rotate(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle,
    enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle);

/*!
 * \brief Rotates the image in the direction and value of the rotation angle in place
 * i.e. it will change the input image itself.
 *
 * If the transformed image is a bayer-format image and the number of rows or columns are even,
 * the format will change. (e.g. 90_COUNTERCLOCKWISE & an image with even width: BayerGB8 -> BayerBG8)
 *
 * \param[in]     imageTransformerHandle The handle to the image transformer to use.
 * \param[in]     rotationAngle          The rotation angle for the image transformer rotation algorithm
 * \param[in,out] imageHandle            The handle to the image to be mirrored.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageTransformerHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has a packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_RotateInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle,
    PEAK_IPL_IMAGE_HANDLE imageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle);

/*!
 * \brief Destroys the image transformer associated with the given handle.
 *
 * \param[in] imageTransformerHandle The handle to the image converter of interest.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE imageTransformerHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Destruct(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle);

/*!
 * \brief Creates a histogram from image data.
 *
 * \param[in]  imageHandle     The handle to the image of interest.
 * \param[out] histogramHandle The handle to the created histogram.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           histogramHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with imageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_Construct(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_HISTOGRAM_HANDLE* histogramHandle);

/*!
 * \brief Increase the reference count of a histogram object.
 *
 * \param[in] histogramHandle The handle to a histogram object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_Acquire(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle);

/*!
 * \brief Queries the pixel format of the given histogram.
 *
 * \param[in]  histogramHandle The handle to the histogram of interest.
 * \param[out] pixelFormat     The pixel format.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   histogramHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pixelFormat is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_GetPixelFormat(
    PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, PEAK_IPL_PIXEL_FORMAT* pixelFormat);

/*!
 * \brief Queries the number of the given histogram's channels.
 *
 * \param[in]  histogramHandle The handle to the histogram of interest.
 * \param[out] numChannels     The number of channels.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   histogramHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numChannels is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_GetNumChannels(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t* numChannels);

/*!
 * \brief Queries the pixel sum for the channel with the given index.
 *
 * \param[in]  histogramHandle The handle to the histogram of interest.
 * \param[in]  channelIndex    The index of the channel of interest.
 * \param[out] pixelSum        The pixel sum of the given channel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   histogramHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pixelSum is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_OUT_OF_RANGE     channelIndex is out of range<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_GetPixelSumForChannel(
    PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t* pixelSum);

/*!
 * \brief Queries the pixel count for the channel with the given index.
 *
 * \param[in]  histogramHandle The handle to the histogram of interest.
 * \param[in]  channelIndex    The index of the channel of interest.
 * \param[out] pixelCount      The pixel count of the given channel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   histogramHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pixelCount is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_OUT_OF_RANGE     channelIndex is out of range<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_GetPixelCountForChannel(
    PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t* pixelCount);

/*!
 * \brief Queries the bin list of the channel with the given index.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     histogramHandle The handle to the histogram of interest.
 * \param[in]     channelIndex    The index of the channel of interest.
 * \param[out]    bins            The bin list of the given channel.
 * \param[in,out] binsSize        IN: Size of the given buffer - OUT: Size of the queried data
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   histogramHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT Either binList or binsSize is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_OUT_OF_RANGE     channelIndex is out of range<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL binList is valid but binsSize is too small<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_GetBinsForChannel(
    PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t* bins, size_t* binsSize);

/*!
 * \brief Destroys the histogram associated with the given handle.
 *
 * \param[in] histogramHandle The handle to the histogram to destroy.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE histogramHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_Histogram_Destruct(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle);

/*!
 * \brief Creates a pixel line depending on the given orientation.
 *
 * A pixel line is a whole line or a whole column of the image's pixels.
 *
 * \param[in] imageHandle      The image to create the pixel line from.
 * \param[in] orientation      The orientation of the line.
 * \param[in] offset           The offset from the reference border.
 *                             The reference border depends on the given orientation
                               (vertical: left - horizontal: top).
 * \param[out] pixelLineHandle The handle to the created pixel line.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             imageHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           pixelLineHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has a packed pixel format<br>
 *         PEAK_IPL_RETURN_CODE_OUT_OF_RANGE               offset is out of range<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has
 *            a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_Construct(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_ORIENTATION orientation,
    size_t offset, PEAK_IPL_PIXEL_LINE_HANDLE* pixelLineHandle);

/*!
 * \brief Increase the reference count of a pixel line object.
 *
 * \param[in] pixelLineHandle The handle to a pixel line object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_Acquire(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle);

/*!
 * \brief Queries the pixel format of the given pixel line.
 *
 * \param[in]  pixelLineHandle The handle to the pixel line of interest.
 * \param[out] pixelFormat     The pixel format.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   pixelLineHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pixelFormat is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_GetPixelFormat(
    PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_PIXEL_FORMAT* pixelFormat);

/*!
 * \brief Queries the orientation of the given pixel line.
 *
 * \param[in]  pixelLineHandle The handle to the pixel line of interest.
 * \param[out] orientation     The orientation.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   pixelLineHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT orientation is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_GetOrientation(
    PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_ORIENTATION* orientation);

/*!
 * \brief Queries the offset of the given pixel line.
 *
 * \param[in]  pixelLineHandle The handle to the pixel line of interest.
 * \param[out] offset          The offset.
 *                             The reference border depends on the orientation of the collection line
                               (vertical: left - horizontal: top).
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   pixelLineHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT offset is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_GetOffset(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t* offset);

/*!
 * \brief Queries the number of the given pixel line's channels.
 *
 * \param[in]  pixelLineHandle The handle to the pixel line of interest.
 * \param[out] numChannels     The number of channels.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   pixelLineHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numChannels is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_GetNumChannels(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t* numChannels);

/*!
 * \brief Queries the value list for the channel with the given index.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     pixelLineHandle The handle to the pixel line of interest.
 * \param[in]     channelIndex    The index of the channel of interest.
 * \param[out]    values          The value list of the given channel.
 * \param[in,out] valuesSize      IN: Size of the given buffer - OUT: Size of the queried data
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   pixelLineHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT At least one of values and valuesSize is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_OUT_OF_RANGE     channelIndex is out of range<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL values are valid but valuesSize is too small<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_GetValuesForChannel(
    PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t channelIndex, uint32_t* values, size_t* valuesSize);

/*!
 * \brief Destroys the pixel line associated with the given handle.
 *
 * \param[in] pixelLineHandle The handle to the pixel line to destroy.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE pixelLineHandle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelLine_Destruct(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle);

/*!
 * \brief Queries the number of channels of the given pixel format.
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[out] numChannels The number of channels.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numChannels is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumChannels(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t* numChannels);

/*!
 * \brief Queries the number of significant bits per pixel per channel of the given pixel format.
 *
 * \param[in]  pixelFormat        The pixel format of interest.
 * \param[out] numSignificantBits The number of significant bits per pixel per channel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numSignificantBits is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel(
    PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t* numSignificantBits);

/*!
 * \brief Queries the number of storage bits per pixel per channel of the given pixel format.
 *
 * \param[in]  pixelFormat    The pixel format of interest.
 * \param[out] numStorageBits The number of storage bits per pixel per channel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numStorageBits is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t* numStorageBits);

/*!
 * \brief Queries the maximum value of one pixel channel of the given pixel format.
 *
 * \param[in]  pixelFormat         The pixel format of interest.
 * \param[out] channelMaximumValue The maximum value of one pixel channel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT channelMaximumValue is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetMaximumValuePerChannel(
    PEAK_IPL_PIXEL_FORMAT pixelFormat, uint32_t* channelMaximumValue);

/*!
 * \brief Queries the number of significant bits per pixel of the given pixel format.
 *
 * \param[in]  pixelFormat        The pixel format of interest.
 * \param[out] numSignificantBits The number of significant bits per pixel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numSignificantBits is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel(
    PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t* numSignificantBits);

/*!
 * \brief Queries the number of storage bits per pixel of the given pixel format.
 *
 * \param[in]  pixelFormat    The pixel format of interest.
 * \param[out] numStorageBits The number of significant bits per pixel.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT numStorageBits is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t* numStorageBits);

/*!
 * \brief Queries the endianness of the given pixel format.
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[out] endianness  The endianness.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT endianness is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetEndianness(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_ENDIANNESS* endianness);

/*!
 * \brief Calculates the storage size of the given number of pixels of the given pixel format in bytes.
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[in]  numPixels   The number of pixels.
 * \param[out] size        The size of the pixels in bytes.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT size is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels(
    PEAK_IPL_PIXEL_FORMAT pixelFormat, uint64_t numPixels, uint64_t* size);

/*!
 * \brief Check if the supplied format is packed
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[out] isPacked    true if the format is a packed format else false.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT isPacked is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_IsPacked(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPacked);

/*!
 * \brief Check if the supplied format is a bayer format
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[out] isBayered   true if the format is a bayer format else false.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT isBayered is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_IsBayered(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isBayered);

/*!
 * \brief Check if the supplied format is a yuv format
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[out] isYUV   true if the format is a yuv format else false.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT isYUV is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.9
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_IsYUV(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isYUV);

/*!
 * \brief Queries the bayer pattern of the given pixel format.
 *
 * \param[in]  pixelFormat The pixel format of interest.
 * \param[out] pattern     The bayer pattern if the format is a bayer format else PEAK_IPL_BAYER_PATTERN_NONE.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT pattern is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetBayerPattern(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BAYER_PATTERN* pattern);

/*!
 * \brief Writes an image as raw data binary file.
 *
 * This is supported by all formats.
 *
 * \param[in] inputImageHandle The handle to the image to save.
 * \param[in] filePath         The file to write the given image to.
 * \param[in] filePathSize     The size of the given string.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           filePath is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                   Errors during file access e.g. no permissions on this file<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED A file type is not supported for this image pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something else went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsRAW(
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char* filePath, size_t filePathSize);

/*!
 * \brief Writes an image as BMP image file.
 *
 * Supported for the following formats:
 * MONO_8, MONO_10, MONO_12, RGB_8, RGB_10, BGR_8, BGR_10, RGBA_8, BGRA_8, RGB_10_PACKED_32, BGR_10_PACKED_32
 * Written as MONO:
 * BAYER_GR_8, BAYER_RG_8, BAYER_GB_8, BAYER_BG_8, BAYER_GR_10, BAYER_RG_10, BAYER_GB_10, BAYER_BG_10
 *
 * \param[in] inputImageHandle The handle to the image to save.
 * \param[in] filePath         The file to write the given image to.
 * \param[in] filePathSize     The size of the given string.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           filePath is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                   Errors during file access e.g. no permissions on this file<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED A file type is not supported for this image pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something else went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsBMP(
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char* filePath, size_t filePathSize);

/*!
 * \brief Writes an image as PNG image file.
 *
 * Supported for following formats:
 * MONO_8, MONO_10, MONO_12, RGB_8, RGB_10, RGB_12, RGBA_8, RGBA_10, RGBA_12
 * Written as MONO:
 * BAYER_GR_8, BAYER_RG_8, BAYER_GB_8, BAYER_BG_8, BAYER_GR_10, BAYER_RG_10, BAYER_GB_10, BAYER_BG_10, BAYER_GR_12,
 * BAYER_RG_12, BAYER_GB_12, BAYER_BG_12
 * Written as RGB:
 * BGR_8, BGR_10, BGR_12, BGRA_8, BGRA_10, BGRA_12
 *
 * \param[in] inputImageHandle The handle to the image to save.
 * \param[in] quality          Specifies the output image quality from 0 to 100.
 * \param[in] filePath         The file to write the given image to.
 * \param[in] filePathSize     The size of the given string.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           filePath is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                   Errors during file access e.g. no permissions on this file<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED A file type is not supported for this image pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something else went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsPNG(
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char* filePath, size_t filePathSize);

/*!
 * \brief Writes an image as JPEG image file.
 *
 * Supported for the following formats:
 * MONO_8, RGB_8, BGR_8, RGBA_8, BGRA_8
 * Written as MONO:
 * BAYER_GR_8, BAYER_RG_8, BAYER_GB_8, BAYER_BG_8
 *
 * \param[in] inputImageHandle The handle to the image to save.
 * \param[in] quality          Specifies the output image quality from 0 to 100.
 * \param[in] filePath         The file to write the given image to.
 * \param[in] filePathSize     The size of the given string.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           filePath is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                   Errors during file access e.g. no permissions on this file<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED A file type is not supported for this image pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something else went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsJPG(
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char* filePath, size_t filePathSize);

/*!
 * \brief Writes an image to file and selects the format by file ending. In case the format has parameters, the default
 *        parameters are chosen.
 *
 * \param[in] inputImageHandle The handle to the image to save.
 * \param[in] filePath         The file to write the given image to.
 * \param[in] filePathSize     The size of the given string.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           filePath is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                   Errors during file access e.g. no permissions on this file<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED A file type is not supported for this image pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something else went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if image associated with inputImageHandle has a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageWriter_Write(
    PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char* filePath, size_t filePathSize);

/*!
 * \brief Reads an image from a file. Image type is determined by the file ending.
 *
 * \param[in]  filePath          The file to read from.
 * \param[in]  filePathSize      The size of the given string.
 * \param[out] outputImageHandle The handle to the read image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           filePath and/or outputImageHandle are/is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                   Errors during file access e.g. no permissions on this file<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED An image format of this file is not supported<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      Something went wrong
 *
 * \since 1.0
 */
PEAK_IPL_C_API PEAK_IPL_ImageReader_Read(
    const char* filePath, size_t filePathSize, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Reads an image from a file. Image type is determined by the file ending.
 *
 * \param[in]  filePath          The file to read from.
 * \param[in]  filePathSize      The size of the given string.
 * \param[in]  pixelFormat       The pixel format of the data to read.
 * \param[out] outputImageHandle The handle to read image.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            filePath and/or outputImageHandle are/is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IO_ERROR                    Errors during file access e.g. no permissions on this
 * file<br> PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED  An image format of this file is not supported<br>
 *         PEAK_IPL_RETURN_CODE_FORMAT_INTERPRETATION_ERROR Can not interpret this file with the given pixel format<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.0
 * \since 1.2 Will return PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED if pixelFormat is a packed pixel format
 */
PEAK_IPL_C_API PEAK_IPL_ImageReaderRead_ReadAsPixelFormat(const char* filePath, size_t filePathSize,
    PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief Open a video stream.
 *
 * \param[out] videoHandle The video handle to operate on.
 * \param[in]  filePath    The filename of the video stream as an utf-8 encoded string (u8string)
 * \param[in]  container   The container type of the video stream.
 * \param[in]  encoder     The encoder type of the video stream.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            filePath is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_Open(PEAK_IPL_VIDEO_HANDLE* videoHandle, const char* filePath,
    PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER encoder);

/*!
 * \brief Check if the video stream is open.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] isOpen      Boolean if the video stream is open.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            isOpen is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_IsOpen(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_BOOL8* isOpen);

/*!
 * \brief Close the video stream.
 *
 * \param[in] videoHandle The handle of the video stream to close.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_Close(PEAK_IPL_VIDEO_HANDLE videoHandle);

/*!
 * \brief Append an image to the video stream.
 *
 * \param[in] videoHandle The handle of the video stream.
 * \param[in] imageHandle The handle of the image to append.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_Append(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \brief Sets an option on the encoder.
 *
 * \param[in] videoHandle The handle of the video stream.
 * \param[in] option      The encoder option to set.
 * \param[in] value       The value for the encoder option.
 * \param[in] count       The size of the value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            value is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_SetOption(
    PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, const void* value, size_t count);

/*!
 * \brief Sets an option of the encoder.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[in]  option      The encoder option to set.
 * \param[out] value       The value for the encoder option.
 * \param[in]  count       The size of the value.
 * \param[out] outCount    The resulting size of the value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            value is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_GetOption(
    PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void* value, size_t count, size_t* outCount);

/*!
 * \brief Receive the range for an option of the encoder.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[in]  option      The encoder option to receive the range from.
 * \param[out] min         The minimum value for the encoder option.
 * \param[out] max         The maximum value for the encoder option.
 * \param[out] inc         The increment value for the encoder option.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            At least one of min, max and inc is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_OptionRange(
    PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void* min, void* max, void* inc);

/*!
 * \brief Get the supported encoders for a container.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     container The container type to query.
 * \param[out]    encoders  The list of supported encoders.
 * \param[in,out] size      IN: Size of the given buffer - OUT: Size of the queried data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            At least one of encoders and size is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer(
    PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER* encoders, size_t* size);

/*!
 * \brief Sets an option on the container.
 *
 * \param[in] videoHandle The handle of the video stream.
 * \param[in] option      The container option to set.
 * \param[in] value       The value for the container option.
 * \param[in] count       The size of the value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            value is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Container_SetOption(
    PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_CONTAINER_OPTION option, const void* value, size_t count);

/*!
 * \brief Gets an option of the container.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[in]  option      The container option to set.
 * \param[out] value       The value for the container option.
 * \param[in]  count       The size of the value.
 * \param[out] outCount    The resulting size of the value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            value is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Container_GetOption(
    PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void* value, size_t count, size_t* outCount);

/*!
 * \brief Receive the range for an option of the container.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[in]  option      The container option to receive the range from.
 * \param[out] min         The minimum value for the encoder option.
 * \param[out] max         The maximum value for the encoder option.
 * \param[out] inc         The increment value for the encoder option.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            At least one of min, max and inc is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Container_OptionRange(
    PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void* min, void* max, void* inc);

/*!
 * \brief Get the supported containers for an encoder.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     encoder    The encoder, which shall be used to retrieve the supported containers.
 * \param[out]    containers The list of supported containers.
 * \param[in,out] size       IN: Size of the given buffer - OUT: Size of the queried data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            At least one of containers and size is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder(
    PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_VIDEO_CONTAINER* containers, size_t* size);

/*!
 * \brief Get the supported pixelformats for an encoder.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     encoder             The container type to query.
 * \param[out]    pixelFormatList     The list of supported pixel formats.
 * \param[in,out] pixelFormatListSize IN: Size of the given buffer - OUT: Size of the queried data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            At least one of pixelFormatList and pixelFormatListSize<br>
 *                                                         is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats(
    PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_PIXEL_FORMAT* pixelFormatList, size_t* pixelFormatListSize);

/*!
 * \brief Gets the number of encoded frames in the video stream.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] numFrames   The number of encoded frames in the video stream.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            nrOfFrames is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_GetEncodedFrames(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t* numFrames);

/*!
 * \brief Gets the number of dropped frames in the video stream.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] numFrames   The number of dropped frames in the video stream.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            nrOfFrames is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_GetDroppedFrames(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t* numFrames);

/*!
 * \brief Gets the file size of the video.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] fileSize    The file size of the video.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT            fileSize is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_GetFileSize(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t* fileSize);

/*!
 * \brief Reset compressed and dropped frame counters.
 *
 * \param[in] videoHandle The handle of the video stream.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_ResetFrameCounters(PEAK_IPL_VIDEO_HANDLE videoHandle);

/*!
 * \brief Set the internal queue size of the video writer.
 *
 * \param[in] videoHandle The handle of the video stream.
 * \param[in] size        The queue size to set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_SetSize(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t size);

/*!
 * \brief Get the internal queue size of the video writer
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] size        The current queue size.

 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_GetSize(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t* size);

/*!
* \brief Get the possible range of the internal queue size
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] minSize     The minimum possible queue size.
 * \param[out] maxSize     The maximum possible queue size.

 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_GetRange(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t* minSize, int32_t* maxSize);

/*!
 * \brief Get the count of the currently queued buffers.
 *
 * \param[in]  videoHandle The handle of the video stream.
 * \param[out] queued      The currently queued buffer count.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_GetQueued(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t* queued);

/*!
 * \brief Wait until frame processing is done.
 *
 * \param[in] videoHandle The handle of the video stream.
 * \param[in] timeout_ms  The timeout in ms.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong<br>
 *         PEAK_IPL_RETURN_CODE_TIMEOUT                     Wait timed out<br>
 *         limgTypeRETURN_CODE_NOT_SUPPORTED               This function is not supported
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms);

/*!
 * \brief Wait until the queue is empty.
 *
 * \param[in] videoHandle The handle of the video stream.
 * \param[in] timeout_ms  The timeout in ms.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                     Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                       Something went wrong<br>
 *         PEAK_IPL_RETURN_CODE_TIMEOUT                     Wait timed out<br>
 *         limgTypeRETURN_CODE_NOT_SUPPORTED               This function is not supported
 *
 * \since 1.5
 */
PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms);

/*!
 * \brief Creates a sharpness measurement.
 *
 * \param[out] handle The handle to the created sharpness.
 *
 * \return IMG_RETURN_CODE_SUCCESS Success<br>
 *         IMG_RETURN_CODE_ERROR   Something went wrong
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Construct(PEAK_IPL_SHARPNESS_HANDLE* handle);

/*!
 * \brief Increase the reference count of a sharpness measurement object.
 *
 * \param[in] handle The handle to a sharpness measurement object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Acquire(PEAK_IPL_SHARPNESS_HANDLE handle);

/*!
 * \brief Destroys the sharpness measurement associated with the given handle.
 *
 * \param[in] handle The handle to the sharpness measurement.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Destruct(PEAK_IPL_SHARPNESS_HANDLE handle);

/*!
 * \brief Measures the relative sharpness of the given image.
 *
 * \param[in]  handle           The handle to the sharpness to use.
 * \param[in]  imageHandle      The handle to the image on which the sharpness is measured.
 * \param[out] calculated_value The relative measured sharpness value.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel format
 *                                                        (e.g. packed pixel format)<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Measure(
    PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, double* calculated_value);

/*!
 * \brief Asks the given sharpness measurement whether it supports the given pixel format.
 *
 * \param[in]  handle                 The handle to the sharpness measurement to use.
 * \param[in]  pixelFormat            The pixel format of interest.
 * \param[out] isPixelFormatSupported The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported(
    PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

/*!
 * \brief Queries the supported pixel formats.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     handle              The handle to the image converter of interest.
 * \param[out]    pixelFormatList     The list of supported output pixel formats.
 * \param[in,out] pixelFormatListSize IN: Size of the given buffer - OUT: Size of the queried data
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT size is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_BUFFER_TOO_SMALL pixelFormatList is valid but size is too small<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList(
    PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT* pixelFormatList, size_t* pixelFormatListSize);

/*!
 * \brief Sets the algorithm used for measurement calculation
 *
 * \param[in] handle    The handle to the sharpness measurement.
 * \param[in] algorithm Algorithm to use. \see PEAK_IPL_SHARPNESS_ALGORITHM_t
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_SetAlgorithm(
    PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM algorithm);

/*!
 * \brief Returns the algorithm to use for measurement calculation
 *
 * \param[in]  handle    The handle to the sharpness measurement.
 * \param[out] algorithm Current used algorithm. \see PEAK_IPL_SHARPNESS_ALGORITHM_t
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT algorithm is an invalid pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetAlgorithm(
    PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM* algorithm);

/*!
 * \brief Returns the minimum size of a ROI to use for measurement calculation
 *
 * \param[in]  handle   The handle to the sharpness measurement.
 * \param[out] min_size Minimum size of ROI. \see PEAK_IPL_SIZE_2D
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT min_size is an invalid pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetROIMinimumSize(
    PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SIZE_2D* min_size);

/*!
 * \brief Sets ROIs in which the sharpness is measured.
 *
 * \param[in] handle      The handle to the sharpness measurement.
 * \param[in] roiList     The list of ROIs to set.
 * \param[in] roiListSize Number of ROIs to set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_SetROIs(
    PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI* roiList, size_t roiListSize);

/*!
 * \brief Returns the ROIs in which the sharpness is measured.
 *
 * For more details on how to apply the two-step procedure this function requires, see also PEAK_IPL_GetLastError().
 *
 * \param[in]     handle      The handle to the sharpness measurement.
 * \param[in]     roiList     The list of ROIs to get.
 * \param[in,out] roiListSize IN: Size of the given buffer - OUT: Size of the queried data.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS          Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE   handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT roi is an invalid pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR            An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetROIs(
    PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI* roiList, size_t* roiListSize);

/*!
 * \brief Create the edge enhancement associated with the given handle.
 *
 * \param[in] handle The handle to the edge enhancement measurement.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Construct(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE* handle);

/*!
 * \brief Increase the reference count of an edge enhancement object.
 *
 * \param[in] handle The handle to a sharpness measurement object.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS Success<br>
 *         PEAK_IPL_RETURN_CODE_ERROR   An internal error has occurred.
 *
 * \since 1.8
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Acquire(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle);

/*!
 * \brief Destruct the edge enhancement associated with the given handle.
 *
 * \param[in] handle The handle to the edge enhancement.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Destruct(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle);

/*!
 * \brief Process the given image with edge enhancement.
 *
 * \param[in] handle        The handle to the edge enhancement to use.
 * \param[in] imageHandle   The handle to the image on which the edge enhancement is applied.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle and/or imageHandle are/is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel format
 *                                                        (e.g. packed pixel format)<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_ProcessInPlace(
    PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
* \brief Process the given image with edge enhancement.
*
* \param[in] handle             The handle to the edge enhancement to use.
* \param[in] imageHandle        The handle to the image on which the edge enhancement is applied.
* \param[in] outputImageHandle  The handle to the image on which the edge enhancement is applied.

* \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
*         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle and/or imageHandle are/is invalid<br>
*         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           imageHandle is a null pointer<br>
*         PEAK_IPL_RETURN_CODE_IMAGE_FORMAT_NOT_SUPPORTED Image associated with imageHandle has unsupported pixel format
*                                                        (e.g. packed pixel format)<br>
*         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
*
* \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Process(
    PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE* outputImageHandle);

/*!
 * \brief  Get the range of valid factors for edge enhancement.
 *
 * \param[in]  handle   The handle to the edge enhancement to use.
 * \param[out] range    A pointer to a range struct of type #PEAK_IPL_RANGE
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           range is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_GetFactorRange(
    PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, struct PEAK_IPL_RANGE* range);

/*!
 * \brief  Set the factor for edge enhancement.
 *
 * \param[in] handle    The handle to the edge enhancement to use.
 * \param[in] factor    The factor to set.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           range is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_SetFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t factor);

/*!
 * \brief  Get the factor for edge enhancement.
 *
 * \param[in]  handle   The handle to the edge enhancement to use.
 * \param[out] factor   A pointer to the variable which will receive the current factor.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           range is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_GetFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t* factor);

/*!
 * \brief  Get the default factor for edge enhancement.
 *
 * \param[in]  handle    The handle to the edge enhancement to use.
 * \param[out] factor    A pointer to the variable which will receive the default factor.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS                    Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE             handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_ARGUMENT           range is a null pointer<br>
 *         PEAK_IPL_RETURN_CODE_ERROR                      An internal error has occurred.
 *
 * \since 1.7
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_GetDefaultFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t* factor);

/*!
 * \brief Asks the given edge enhancement whether it supports the given pixel format.
 *
 * \param[in]  handle                 The handle to the sharpness measurement to use.
 * \param[in]  pixelFormat            The pixel format of interest.
 * \param[out] isPixelFormatSupported The flag telling whether the given pixel format is supported.
 *
 * \return PEAK_IPL_RETURN_CODE_SUCCESS        Success<br>
 *         PEAK_IPL_RETURN_CODE_INVALID_HANDLE handle is invalid<br>
 *         PEAK_IPL_RETURN_CODE_ERROR          An internal error has occurred.
 *
 * \since 1.6
 */
PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported(
    PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8* isPixelFormatSupported);

#    ifdef __cplusplus
} // extern "C"
#    endif

#else
#    ifdef __cplusplus
} // extern "C"
#    endif

#    include <peak_ipl/backend/peak_ipl_dynamic_loader.h>
#endif // !PEAK_IPL_DYNAMIC_LOADING
