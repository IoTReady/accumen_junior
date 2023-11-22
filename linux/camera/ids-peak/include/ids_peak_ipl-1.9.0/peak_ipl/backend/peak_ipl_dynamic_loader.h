/*!
 * \file    peak_ipl_dynamic_loader.h
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */
#pragma once

#include "peak_ipl_backend.h"
#include <string>
#include <cstdint>

#ifdef __linux__
    #include <dlfcn.h>
#else
    #include <vector>
    #include <windows.h>
    #include <tchar.h>
#endif
 
#include <stdexcept>

#undef PEAK_IPL_PUBLIC
#define PEAK_IPL_PUBLIC
#undef PEAK_IPL_CALL_CONV
#define PEAK_IPL_CALL_CONV


namespace peak
{
namespace ipl
{
namespace dynamic
{

typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetVersionMajor)(uint32_t * libraryVersionMajor);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetVersionMinor)(uint32_t * libraryVersionMinor);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetVersionSubminor)(uint32_t * libraryVersionSubminor);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetMaxConcurrentThreads)(size_t * concurrentThreads);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_SetMaxConcurrentThreads)(size_t concurrentThreads);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetMultiThreadingEnabled)(PEAK_IPL_BOOL8 * enabled);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_SetMultiThreadingEnabled)(PEAK_IPL_BOOL8 enabled);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetSIMDEnabled)(PEAK_IPL_BOOL8 * enabled);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_SetSIMDEnabled)(PEAK_IPL_BOOL8 enabled);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Library_GetLastError)(PEAK_IPL_RETURN_CODE * lastErrorCode, char * lastErrorDescription, size_t * lastErrorDescriptionSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_Construct)(PEAK_IPL_IMAGE_CONVERTER_HANDLE * imageConverterHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_Acquire)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_PreAllocateConversion)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, size_t width, size_t height, size_t imageCount);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_GetConversionMode)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE * conversionMode);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_SetConversionMode)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE conversionMode);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT * outputPixelFormats, size_t * outputPixelFormatsSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_Convert)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_ConvertToBuffer)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t * outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageConverter_Destruct)(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Construct)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE * adaptiveHotpixelCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY sensitivityLevel);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY * sensitivityLevel);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t gainFactorPercent);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t * gainFactorPercent);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Detect)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, struct PEAK_IPL_POINT_2D * hotpixels, size_t * hotpixelsSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Correct)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, const struct PEAK_IPL_POINT_2D * hotpixels, size_t hotpixelsSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, const struct PEAK_IPL_POINT_2D * hotpixels, size_t hotpixelsSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct)(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_Construct)(PEAK_IPL_COLOR_CORRECTOR_HANDLE * colorCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_Acquire)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * colorCorrectorFactors);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * colorCorrectorFactors, size_t * colorCorrectorFactorsSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_Process)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_ProcessInPlace)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_Destruct)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_GetSaturation)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturation);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_GetSaturationMin)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturationMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_GetSaturationMax)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturationMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ColorCorrector_SetSaturation)(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float saturation);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_Construct)(PEAK_IPL_BINNING_HANDLE * binningHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_Acquire)(PEAK_IPL_BINNING_HANDLE binningHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_SetBinningX)(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningXValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_GetBinningX)(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningXValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_SetBinningY)(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningYValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_GetBinningY)(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningYValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_GetBinningMin)(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_GetBinningMax)(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_ExpectedOutputSize)(PEAK_IPL_BINNING_HANDLE binningHandle, size_t * outputWidth, size_t * outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_IsPixelformatSupported)(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_SetBinningMode)(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE * mode);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_GetBinningMode)(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE * mode);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_Process)(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Binning_Destruct)(PEAK_IPL_BINNING_HANDLE binningHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_Construct)(PEAK_IPL_DECIMATION_HANDLE * decimationHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_Acquire)(PEAK_IPL_DECIMATION_HANDLE decimationHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_SetDecimationX)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationXValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_GetDecimationX)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationXValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_SetDecimationY)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationYValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_GetDecimationY)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationYValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_GetDecimationMin)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_GetDecimationMax)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_ExpectedOutputSize)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, size_t * outputWidth, size_t * outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_IsPixelformatSupported)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_Process)(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Decimation_Destruct)(PEAK_IPL_DECIMATION_HANDLE decimationHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_Construct)(PEAK_IPL_GAIN_HANDLE * gainHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_Acquire)(PEAK_IPL_GAIN_HANDLE gainHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_SetMasterGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetMasterGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetMasterGainMax)(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetMasterGainMin)(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_SetRedGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetRedGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetRedGainMax)(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetRedGainMin)(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_SetGreenGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetGreenGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetGreenGainMax)(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetGreenGainMin)(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_SetBlueGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetBlueGainValue)(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetBlueGainMax)(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetBlueGainMin)(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_GetIsPixelFormatSupported)(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_Process)(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_ProcessInPlace)(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Gain_Destruct)(PEAK_IPL_GAIN_HANDLE gainHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_Construct)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE * gammaCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_Acquire)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaMax);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaMin);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_Process)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_ProcessInPlace)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_GammaCorrector_Destruct)(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_Construct)(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE * imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_Acquire)(PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_ConstructFromBuffer)(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint8_t * buffer, uint64_t bufferSize, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE * imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetWidth)(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * width);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetHeight)(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * height);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetPixelPointer)(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t xPos, size_t yPos, uint8_t * * pixelPointer);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetByteCount)(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * byteCount);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetPixelFormat)(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetData)(PEAK_IPL_IMAGE_HANDLE imageHandle, uint8_t * * data);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_Clone)(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_ConvertTo)(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_Scale)(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D * size, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_ScaleInto)(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D * size, uint8_t * buffer, size_t bufferSize, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_Crop)(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_POINT_2D * pos, const struct PEAK_IPL_SIZE_2D * size, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_ConvertToBuffer)(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t * outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_Destruct)(PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_Construct)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE * imageTransformerHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_Acquire)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_SetTimestamp)(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t timestamp_ns);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Image_GetTimestamp)(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t * timestamp_ns);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_MirrorUpDown)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_MirrorLeftRight)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_Rotate)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_RotateInPlace)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageTransformer_Destruct)(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_Construct)(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_HISTOGRAM_HANDLE * histogramHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_Acquire)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_GetPixelFormat)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_GetNumChannels)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t * numChannels);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_GetPixelSumForChannel)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * pixelSum);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_GetPixelCountForChannel)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * pixelCount);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_GetBinsForChannel)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * bins, size_t * binsSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_Histogram_Destruct)(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_Construct)(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_ORIENTATION orientation, size_t offset, PEAK_IPL_PIXEL_LINE_HANDLE * pixelLineHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_Acquire)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_GetPixelFormat)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_GetOrientation)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_ORIENTATION * orientation);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_GetOffset)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t * offset);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_GetNumChannels)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t * numChannels);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_GetValuesForChannel)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t channelIndex, uint32_t * values, size_t * valuesSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelLine_Destruct)(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetNumChannels)(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numChannels);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel)(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numSignificantBits);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel)(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numStorageBits);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel)(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint32_t * channelMaximumValue);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel)(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numSignificantBits);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel)(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numStorageBits);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetEndianness)(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_ENDIANNESS * endianness);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels)(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint64_t numPixels, uint64_t * size);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_IsPacked)(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPacked);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_IsBayered)(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isBayered);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_IsYUV)(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isYUV);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_PixelFormat_GetBayerPattern)(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BAYER_PATTERN * pattern);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageWriter_WriteAsRAW)(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageWriter_WriteAsBMP)(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageWriter_WriteAsPNG)(PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char * filePath, size_t filePathSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageWriter_WriteAsJPG)(PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char * filePath, size_t filePathSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageWriter_Write)(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageReader_Read)(const char * filePath, size_t filePathSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat)(const char * filePath, size_t filePathSize, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_Open)(PEAK_IPL_VIDEO_HANDLE * videoHandle, const char * filePath, PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER encoder);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_IsOpen)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_BOOL8 * isOpen);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_Close)(PEAK_IPL_VIDEO_HANDLE videoHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_Append)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Encoder_SetOption)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, const void * value, size_t count);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Encoder_GetOption)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * value, size_t count, size_t * outCount);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Encoder_OptionRange)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * min, void * max, void * inc);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer)(PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER * encoders, size_t * size);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Container_SetOption)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_CONTAINER_OPTION option, const void * value, size_t count);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Container_GetOption)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * value, size_t count, size_t * outCount);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Container_OptionRange)(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * min, void * max, void * inc);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder)(PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_VIDEO_CONTAINER * containers, size_t * size);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats)(PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_PIXEL_FORMAT * pixelFormatList, size_t * pixelFormatListSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_GetEncodedFrames)(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * numFrames);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_GetDroppedFrames)(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * numFrames);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_GetFileSize)(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * fileSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_ResetFrameCounters)(PEAK_IPL_VIDEO_HANDLE videoHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Queue_SetSize)(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t size);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Queue_GetSize)(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * size);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Queue_GetRange)(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * minSize, int32_t * maxSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Queue_GetQueued)(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * queued);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone)(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty)(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_Construct)(PEAK_IPL_SHARPNESS_HANDLE * handle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_Acquire)(PEAK_IPL_SHARPNESS_HANDLE handle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_Destruct)(PEAK_IPL_SHARPNESS_HANDLE handle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_Measure)(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, double * calculated_value);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported)(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList)(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT * pixelFormatList, size_t * pixelFormatListSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_SetAlgorithm)(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM algorithm);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_GetAlgorithm)(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM * algorithm);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_GetROIMinimumSize)(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SIZE_2D * min_size);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_SetROIs)(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI * roiList, size_t roiListSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_ImageSharpness_GetROIs)(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI * roiList, size_t * roiListSize);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_Construct)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE * handle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_Acquire)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_Destruct)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_ProcessInPlace)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_Process)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_GetFactorRange)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, struct PEAK_IPL_RANGE * range);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_SetFactor)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t factor);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_GetFactor)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t * factor);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_GetDefaultFactor)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t * factor);
typedef PEAK_IPL_C_API (*dyn_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported)(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);

                        
class DynamicLoader
{
private:
    DynamicLoader();
    
    static DynamicLoader& instance()
    {
        static DynamicLoader dynamicLoader{};
        return dynamicLoader;
    }
    bool loadLib(const char* file);
    void unload();
    bool setPointers(bool load);

public:
    ~DynamicLoader();
    
    static bool isLoaded();
    
    static PEAK_IPL_C_API PEAK_IPL_Library_GetVersionMajor(uint32_t * libraryVersionMajor);
    static PEAK_IPL_C_API PEAK_IPL_Library_GetVersionMinor(uint32_t * libraryVersionMinor);
    static PEAK_IPL_C_API PEAK_IPL_Library_GetVersionSubminor(uint32_t * libraryVersionSubminor);
    static PEAK_IPL_C_API PEAK_IPL_Library_GetMaxConcurrentThreads(size_t * concurrentThreads);
    static PEAK_IPL_C_API PEAK_IPL_Library_SetMaxConcurrentThreads(size_t concurrentThreads);
    static PEAK_IPL_C_API PEAK_IPL_Library_GetMultiThreadingEnabled(PEAK_IPL_BOOL8 * enabled);
    static PEAK_IPL_C_API PEAK_IPL_Library_SetMultiThreadingEnabled(PEAK_IPL_BOOL8 enabled);
    static PEAK_IPL_C_API PEAK_IPL_Library_GetSIMDEnabled(PEAK_IPL_BOOL8 * enabled);
    static PEAK_IPL_C_API PEAK_IPL_Library_SetSIMDEnabled(PEAK_IPL_BOOL8 enabled);
    static PEAK_IPL_C_API PEAK_IPL_Library_GetLastError(PEAK_IPL_RETURN_CODE * lastErrorCode, char * lastErrorDescription, size_t * lastErrorDescriptionSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_Construct(PEAK_IPL_IMAGE_CONVERTER_HANDLE * imageConverterHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_Acquire(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_PreAllocateConversion(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, size_t width, size_t height, size_t imageCount);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_GetConversionMode(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE * conversionMode);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_SetConversionMode(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE conversionMode);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT * outputPixelFormats, size_t * outputPixelFormatsSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_Convert(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_ConvertToBuffer(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t * outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageConverter_Destruct(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Construct(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE * adaptiveHotpixelCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Acquire(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY sensitivityLevel);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY * sensitivityLevel);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t gainFactorPercent);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t * gainFactorPercent);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Detect(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, struct PEAK_IPL_POINT_2D * hotpixels, size_t * hotpixelsSize);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Correct(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, const struct PEAK_IPL_POINT_2D * hotpixels, size_t hotpixelsSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, const struct PEAK_IPL_POINT_2D * hotpixels, size_t hotpixelsSize);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Construct(PEAK_IPL_COLOR_CORRECTOR_HANDLE * colorCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Acquire(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_SetColorCorrectionFactors(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * colorCorrectorFactors);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetColorCorrectionFactors(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * colorCorrectorFactors, size_t * colorCorrectorFactorsSize);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Process(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_ProcessInPlace(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_Destruct(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetSaturation(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturation);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetSaturationMin(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturationMin);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_GetSaturationMax(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturationMax);
    static PEAK_IPL_C_API PEAK_IPL_ColorCorrector_SetSaturation(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float saturation);
    static PEAK_IPL_C_API PEAK_IPL_Binning_Construct(PEAK_IPL_BINNING_HANDLE * binningHandle);
    static PEAK_IPL_C_API PEAK_IPL_Binning_Acquire(PEAK_IPL_BINNING_HANDLE binningHandle);
    static PEAK_IPL_C_API PEAK_IPL_Binning_SetBinningX(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningXValue);
    static PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningX(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningXValue);
    static PEAK_IPL_C_API PEAK_IPL_Binning_SetBinningY(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningYValue);
    static PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningY(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningYValue);
    static PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningMin(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningMin);
    static PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningMax(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningMax);
    static PEAK_IPL_C_API PEAK_IPL_Binning_ExpectedOutputSize(PEAK_IPL_BINNING_HANDLE binningHandle, size_t * outputWidth, size_t * outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat);
    static PEAK_IPL_C_API PEAK_IPL_Binning_IsPixelformatSupported(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
    static PEAK_IPL_C_API PEAK_IPL_Binning_SetBinningMode(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE * mode);
    static PEAK_IPL_C_API PEAK_IPL_Binning_GetBinningMode(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE * mode);
    static PEAK_IPL_C_API PEAK_IPL_Binning_Process(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Binning_Destruct(PEAK_IPL_BINNING_HANDLE binningHandle);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_Construct(PEAK_IPL_DECIMATION_HANDLE * decimationHandle);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_Acquire(PEAK_IPL_DECIMATION_HANDLE decimationHandle);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_SetDecimationX(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationXValue);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationX(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationXValue);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_SetDecimationY(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationYValue);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationY(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationYValue);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationMin(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationMin);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_GetDecimationMax(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationMax);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_ExpectedOutputSize(PEAK_IPL_DECIMATION_HANDLE decimationHandle, size_t * outputWidth, size_t * outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_IsPixelformatSupported(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_Process(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Decimation_Destruct(PEAK_IPL_DECIMATION_HANDLE decimationHandle);
    static PEAK_IPL_C_API PEAK_IPL_Gain_Construct(PEAK_IPL_GAIN_HANDLE * gainHandle);
    static PEAK_IPL_C_API PEAK_IPL_Gain_Acquire(PEAK_IPL_GAIN_HANDLE gainHandle);
    static PEAK_IPL_C_API PEAK_IPL_Gain_SetMasterGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetMasterGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetMasterGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainMax);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetMasterGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainMin);
    static PEAK_IPL_C_API PEAK_IPL_Gain_SetRedGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetRedGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetRedGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainMax);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetRedGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainMin);
    static PEAK_IPL_C_API PEAK_IPL_Gain_SetGreenGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetGreenGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetGreenGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainMax);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetGreenGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainMin);
    static PEAK_IPL_C_API PEAK_IPL_Gain_SetBlueGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetBlueGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainValue);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetBlueGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainMax);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetBlueGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainMin);
    static PEAK_IPL_C_API PEAK_IPL_Gain_GetIsPixelFormatSupported(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
    static PEAK_IPL_C_API PEAK_IPL_Gain_Process(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Gain_ProcessInPlace(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Gain_Destruct(PEAK_IPL_GAIN_HANDLE gainHandle);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Construct(PEAK_IPL_GAMMA_CORRECTOR_HANDLE * gammaCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Acquire(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_SetGammaCorrectionValue(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaValue);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetGammaCorrectionValue(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaValue);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetGammaCorrectionMax(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaMax);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetGammaCorrectionMin(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaMin);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Process(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_ProcessInPlace(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_GammaCorrector_Destruct(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_Construct(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE * imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_Acquire(PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_ConstructFromBuffer(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint8_t * buffer, uint64_t bufferSize, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE * imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetWidth(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * width);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetHeight(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * height);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetPixelPointer(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t xPos, size_t yPos, uint8_t * * pixelPointer);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetByteCount(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * byteCount);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetPixelFormat(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetData(PEAK_IPL_IMAGE_HANDLE imageHandle, uint8_t * * data);
    static PEAK_IPL_C_API PEAK_IPL_Image_Clone(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_ConvertTo(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_Scale(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D * size, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_ScaleInto(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D * size, uint8_t * buffer, size_t bufferSize, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm);
    static PEAK_IPL_C_API PEAK_IPL_Image_Crop(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_POINT_2D * pos, const struct PEAK_IPL_SIZE_2D * size, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_ConvertToBuffer(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t * outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_Destruct(PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Construct(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE * imageTransformerHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Acquire(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle);
    static PEAK_IPL_C_API PEAK_IPL_Image_SetTimestamp(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t timestamp_ns);
    static PEAK_IPL_C_API PEAK_IPL_Image_GetTimestamp(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t * timestamp_ns);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDown(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorLeftRight(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDownInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Rotate(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_RotateInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle);
    static PEAK_IPL_C_API PEAK_IPL_ImageTransformer_Destruct(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_Construct(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_HISTOGRAM_HANDLE * histogramHandle);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_Acquire(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_GetPixelFormat(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_GetNumChannels(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t * numChannels);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_GetPixelSumForChannel(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * pixelSum);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_GetPixelCountForChannel(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * pixelCount);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_GetBinsForChannel(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * bins, size_t * binsSize);
    static PEAK_IPL_C_API PEAK_IPL_Histogram_Destruct(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_Construct(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_ORIENTATION orientation, size_t offset, PEAK_IPL_PIXEL_LINE_HANDLE * pixelLineHandle);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_Acquire(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_GetPixelFormat(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_GetOrientation(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_ORIENTATION * orientation);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_GetOffset(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t * offset);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_GetNumChannels(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t * numChannels);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_GetValuesForChannel(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t channelIndex, uint32_t * values, size_t * valuesSize);
    static PEAK_IPL_C_API PEAK_IPL_PixelLine_Destruct(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumChannels(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numChannels);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numSignificantBits);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numStorageBits);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetMaximumValuePerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint32_t * channelMaximumValue);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numSignificantBits);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numStorageBits);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetEndianness(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_ENDIANNESS * endianness);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint64_t numPixels, uint64_t * size);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_IsPacked(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPacked);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_IsBayered(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isBayered);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_IsYUV(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isYUV);
    static PEAK_IPL_C_API PEAK_IPL_PixelFormat_GetBayerPattern(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BAYER_PATTERN * pattern);
    static PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsRAW(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsBMP(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsPNG(PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char * filePath, size_t filePathSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageWriter_WriteAsJPG(PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char * filePath, size_t filePathSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageWriter_Write(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageReader_Read(const char * filePath, size_t filePathSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_ImageReaderRead_ReadAsPixelFormat(const char * filePath, size_t filePathSize, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_Open(PEAK_IPL_VIDEO_HANDLE * videoHandle, const char * filePath, PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER encoder);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_IsOpen(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_BOOL8 * isOpen);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_Close(PEAK_IPL_VIDEO_HANDLE videoHandle);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_Append(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_SetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, const void * value, size_t count);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_GetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * value, size_t count, size_t * outCount);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_OptionRange(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * min, void * max, void * inc);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer(PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER * encoders, size_t * size);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Container_SetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_CONTAINER_OPTION option, const void * value, size_t count);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Container_GetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * value, size_t count, size_t * outCount);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Container_OptionRange(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * min, void * max, void * inc);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder(PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_VIDEO_CONTAINER * containers, size_t * size);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats(PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_PIXEL_FORMAT * pixelFormatList, size_t * pixelFormatListSize);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_GetEncodedFrames(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * numFrames);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_GetDroppedFrames(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * numFrames);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_GetFileSize(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * fileSize);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_ResetFrameCounters(PEAK_IPL_VIDEO_HANDLE videoHandle);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_SetSize(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t size);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_GetSize(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * size);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_GetRange(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * minSize, int32_t * maxSize);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Queue_GetQueued(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * queued);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms);
    static PEAK_IPL_C_API PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Construct(PEAK_IPL_SHARPNESS_HANDLE * handle);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Acquire(PEAK_IPL_SHARPNESS_HANDLE handle);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Destruct(PEAK_IPL_SHARPNESS_HANDLE handle);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_Measure(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, double * calculated_value);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT * pixelFormatList, size_t * pixelFormatListSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_SetAlgorithm(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM algorithm);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetAlgorithm(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM * algorithm);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetROIMinimumSize(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SIZE_2D * min_size);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_SetROIs(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI * roiList, size_t roiListSize);
    static PEAK_IPL_C_API PEAK_IPL_ImageSharpness_GetROIs(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI * roiList, size_t * roiListSize);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Construct(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE * handle);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Acquire(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Destruct(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_ProcessInPlace(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_Process(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_GetFactorRange(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, struct PEAK_IPL_RANGE * range);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_SetFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t factor);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_GetFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t * factor);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_GetDefaultFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t * factor);
    static PEAK_IPL_C_API PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported);
       
private:
    void* m_handle = nullptr;
    dyn_PEAK_IPL_Library_GetVersionMajor m_PEAK_IPL_Library_GetVersionMajor{};
    dyn_PEAK_IPL_Library_GetVersionMinor m_PEAK_IPL_Library_GetVersionMinor{};
    dyn_PEAK_IPL_Library_GetVersionSubminor m_PEAK_IPL_Library_GetVersionSubminor{};
    dyn_PEAK_IPL_Library_GetMaxConcurrentThreads m_PEAK_IPL_Library_GetMaxConcurrentThreads{};
    dyn_PEAK_IPL_Library_SetMaxConcurrentThreads m_PEAK_IPL_Library_SetMaxConcurrentThreads{};
    dyn_PEAK_IPL_Library_GetMultiThreadingEnabled m_PEAK_IPL_Library_GetMultiThreadingEnabled{};
    dyn_PEAK_IPL_Library_SetMultiThreadingEnabled m_PEAK_IPL_Library_SetMultiThreadingEnabled{};
    dyn_PEAK_IPL_Library_GetSIMDEnabled m_PEAK_IPL_Library_GetSIMDEnabled{};
    dyn_PEAK_IPL_Library_SetSIMDEnabled m_PEAK_IPL_Library_SetSIMDEnabled{};
    dyn_PEAK_IPL_Library_GetLastError m_PEAK_IPL_Library_GetLastError{};
    dyn_PEAK_IPL_ImageConverter_Construct m_PEAK_IPL_ImageConverter_Construct{};
    dyn_PEAK_IPL_ImageConverter_Acquire m_PEAK_IPL_ImageConverter_Acquire{};
    dyn_PEAK_IPL_ImageConverter_PreAllocateConversion m_PEAK_IPL_ImageConverter_PreAllocateConversion{};
    dyn_PEAK_IPL_ImageConverter_GetConversionMode m_PEAK_IPL_ImageConverter_GetConversionMode{};
    dyn_PEAK_IPL_ImageConverter_SetConversionMode m_PEAK_IPL_ImageConverter_SetConversionMode{};
    dyn_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats m_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats{};
    dyn_PEAK_IPL_ImageConverter_Convert m_PEAK_IPL_ImageConverter_Convert{};
    dyn_PEAK_IPL_ImageConverter_ConvertToBuffer m_PEAK_IPL_ImageConverter_ConvertToBuffer{};
    dyn_PEAK_IPL_ImageConverter_Destruct m_PEAK_IPL_ImageConverter_Destruct{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Construct m_PEAK_IPL_AdaptiveHotpixelCorrector_Construct{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire m_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity m_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity m_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent m_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent m_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Detect m_PEAK_IPL_AdaptiveHotpixelCorrector_Detect{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels m_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Correct m_PEAK_IPL_AdaptiveHotpixelCorrector_Correct{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace{};
    dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct m_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct{};
    dyn_PEAK_IPL_ColorCorrector_Construct m_PEAK_IPL_ColorCorrector_Construct{};
    dyn_PEAK_IPL_ColorCorrector_Acquire m_PEAK_IPL_ColorCorrector_Acquire{};
    dyn_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors m_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors{};
    dyn_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors m_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors{};
    dyn_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported m_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported{};
    dyn_PEAK_IPL_ColorCorrector_Process m_PEAK_IPL_ColorCorrector_Process{};
    dyn_PEAK_IPL_ColorCorrector_ProcessInPlace m_PEAK_IPL_ColorCorrector_ProcessInPlace{};
    dyn_PEAK_IPL_ColorCorrector_Destruct m_PEAK_IPL_ColorCorrector_Destruct{};
    dyn_PEAK_IPL_ColorCorrector_GetSaturation m_PEAK_IPL_ColorCorrector_GetSaturation{};
    dyn_PEAK_IPL_ColorCorrector_GetSaturationMin m_PEAK_IPL_ColorCorrector_GetSaturationMin{};
    dyn_PEAK_IPL_ColorCorrector_GetSaturationMax m_PEAK_IPL_ColorCorrector_GetSaturationMax{};
    dyn_PEAK_IPL_ColorCorrector_SetSaturation m_PEAK_IPL_ColorCorrector_SetSaturation{};
    dyn_PEAK_IPL_Binning_Construct m_PEAK_IPL_Binning_Construct{};
    dyn_PEAK_IPL_Binning_Acquire m_PEAK_IPL_Binning_Acquire{};
    dyn_PEAK_IPL_Binning_SetBinningX m_PEAK_IPL_Binning_SetBinningX{};
    dyn_PEAK_IPL_Binning_GetBinningX m_PEAK_IPL_Binning_GetBinningX{};
    dyn_PEAK_IPL_Binning_SetBinningY m_PEAK_IPL_Binning_SetBinningY{};
    dyn_PEAK_IPL_Binning_GetBinningY m_PEAK_IPL_Binning_GetBinningY{};
    dyn_PEAK_IPL_Binning_GetBinningMin m_PEAK_IPL_Binning_GetBinningMin{};
    dyn_PEAK_IPL_Binning_GetBinningMax m_PEAK_IPL_Binning_GetBinningMax{};
    dyn_PEAK_IPL_Binning_ExpectedOutputSize m_PEAK_IPL_Binning_ExpectedOutputSize{};
    dyn_PEAK_IPL_Binning_IsPixelformatSupported m_PEAK_IPL_Binning_IsPixelformatSupported{};
    dyn_PEAK_IPL_Binning_SetBinningMode m_PEAK_IPL_Binning_SetBinningMode{};
    dyn_PEAK_IPL_Binning_GetBinningMode m_PEAK_IPL_Binning_GetBinningMode{};
    dyn_PEAK_IPL_Binning_Process m_PEAK_IPL_Binning_Process{};
    dyn_PEAK_IPL_Binning_Destruct m_PEAK_IPL_Binning_Destruct{};
    dyn_PEAK_IPL_Decimation_Construct m_PEAK_IPL_Decimation_Construct{};
    dyn_PEAK_IPL_Decimation_Acquire m_PEAK_IPL_Decimation_Acquire{};
    dyn_PEAK_IPL_Decimation_SetDecimationX m_PEAK_IPL_Decimation_SetDecimationX{};
    dyn_PEAK_IPL_Decimation_GetDecimationX m_PEAK_IPL_Decimation_GetDecimationX{};
    dyn_PEAK_IPL_Decimation_SetDecimationY m_PEAK_IPL_Decimation_SetDecimationY{};
    dyn_PEAK_IPL_Decimation_GetDecimationY m_PEAK_IPL_Decimation_GetDecimationY{};
    dyn_PEAK_IPL_Decimation_GetDecimationMin m_PEAK_IPL_Decimation_GetDecimationMin{};
    dyn_PEAK_IPL_Decimation_GetDecimationMax m_PEAK_IPL_Decimation_GetDecimationMax{};
    dyn_PEAK_IPL_Decimation_ExpectedOutputSize m_PEAK_IPL_Decimation_ExpectedOutputSize{};
    dyn_PEAK_IPL_Decimation_IsPixelformatSupported m_PEAK_IPL_Decimation_IsPixelformatSupported{};
    dyn_PEAK_IPL_Decimation_Process m_PEAK_IPL_Decimation_Process{};
    dyn_PEAK_IPL_Decimation_Destruct m_PEAK_IPL_Decimation_Destruct{};
    dyn_PEAK_IPL_Gain_Construct m_PEAK_IPL_Gain_Construct{};
    dyn_PEAK_IPL_Gain_Acquire m_PEAK_IPL_Gain_Acquire{};
    dyn_PEAK_IPL_Gain_SetMasterGainValue m_PEAK_IPL_Gain_SetMasterGainValue{};
    dyn_PEAK_IPL_Gain_GetMasterGainValue m_PEAK_IPL_Gain_GetMasterGainValue{};
    dyn_PEAK_IPL_Gain_GetMasterGainMax m_PEAK_IPL_Gain_GetMasterGainMax{};
    dyn_PEAK_IPL_Gain_GetMasterGainMin m_PEAK_IPL_Gain_GetMasterGainMin{};
    dyn_PEAK_IPL_Gain_SetRedGainValue m_PEAK_IPL_Gain_SetRedGainValue{};
    dyn_PEAK_IPL_Gain_GetRedGainValue m_PEAK_IPL_Gain_GetRedGainValue{};
    dyn_PEAK_IPL_Gain_GetRedGainMax m_PEAK_IPL_Gain_GetRedGainMax{};
    dyn_PEAK_IPL_Gain_GetRedGainMin m_PEAK_IPL_Gain_GetRedGainMin{};
    dyn_PEAK_IPL_Gain_SetGreenGainValue m_PEAK_IPL_Gain_SetGreenGainValue{};
    dyn_PEAK_IPL_Gain_GetGreenGainValue m_PEAK_IPL_Gain_GetGreenGainValue{};
    dyn_PEAK_IPL_Gain_GetGreenGainMax m_PEAK_IPL_Gain_GetGreenGainMax{};
    dyn_PEAK_IPL_Gain_GetGreenGainMin m_PEAK_IPL_Gain_GetGreenGainMin{};
    dyn_PEAK_IPL_Gain_SetBlueGainValue m_PEAK_IPL_Gain_SetBlueGainValue{};
    dyn_PEAK_IPL_Gain_GetBlueGainValue m_PEAK_IPL_Gain_GetBlueGainValue{};
    dyn_PEAK_IPL_Gain_GetBlueGainMax m_PEAK_IPL_Gain_GetBlueGainMax{};
    dyn_PEAK_IPL_Gain_GetBlueGainMin m_PEAK_IPL_Gain_GetBlueGainMin{};
    dyn_PEAK_IPL_Gain_GetIsPixelFormatSupported m_PEAK_IPL_Gain_GetIsPixelFormatSupported{};
    dyn_PEAK_IPL_Gain_Process m_PEAK_IPL_Gain_Process{};
    dyn_PEAK_IPL_Gain_ProcessInPlace m_PEAK_IPL_Gain_ProcessInPlace{};
    dyn_PEAK_IPL_Gain_Destruct m_PEAK_IPL_Gain_Destruct{};
    dyn_PEAK_IPL_GammaCorrector_Construct m_PEAK_IPL_GammaCorrector_Construct{};
    dyn_PEAK_IPL_GammaCorrector_Acquire m_PEAK_IPL_GammaCorrector_Acquire{};
    dyn_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue m_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue{};
    dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue m_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue{};
    dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax{};
    dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin{};
    dyn_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported m_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported{};
    dyn_PEAK_IPL_GammaCorrector_Process m_PEAK_IPL_GammaCorrector_Process{};
    dyn_PEAK_IPL_GammaCorrector_ProcessInPlace m_PEAK_IPL_GammaCorrector_ProcessInPlace{};
    dyn_PEAK_IPL_GammaCorrector_Destruct m_PEAK_IPL_GammaCorrector_Destruct{};
    dyn_PEAK_IPL_Image_Construct m_PEAK_IPL_Image_Construct{};
    dyn_PEAK_IPL_Image_Acquire m_PEAK_IPL_Image_Acquire{};
    dyn_PEAK_IPL_Image_ConstructFromBuffer m_PEAK_IPL_Image_ConstructFromBuffer{};
    dyn_PEAK_IPL_Image_GetWidth m_PEAK_IPL_Image_GetWidth{};
    dyn_PEAK_IPL_Image_GetHeight m_PEAK_IPL_Image_GetHeight{};
    dyn_PEAK_IPL_Image_GetPixelPointer m_PEAK_IPL_Image_GetPixelPointer{};
    dyn_PEAK_IPL_Image_GetByteCount m_PEAK_IPL_Image_GetByteCount{};
    dyn_PEAK_IPL_Image_GetPixelFormat m_PEAK_IPL_Image_GetPixelFormat{};
    dyn_PEAK_IPL_Image_GetData m_PEAK_IPL_Image_GetData{};
    dyn_PEAK_IPL_Image_Clone m_PEAK_IPL_Image_Clone{};
    dyn_PEAK_IPL_Image_ConvertTo m_PEAK_IPL_Image_ConvertTo{};
    dyn_PEAK_IPL_Image_Scale m_PEAK_IPL_Image_Scale{};
    dyn_PEAK_IPL_Image_ScaleInto m_PEAK_IPL_Image_ScaleInto{};
    dyn_PEAK_IPL_Image_Crop m_PEAK_IPL_Image_Crop{};
    dyn_PEAK_IPL_Image_ConvertToBuffer m_PEAK_IPL_Image_ConvertToBuffer{};
    dyn_PEAK_IPL_Image_Destruct m_PEAK_IPL_Image_Destruct{};
    dyn_PEAK_IPL_ImageTransformer_Construct m_PEAK_IPL_ImageTransformer_Construct{};
    dyn_PEAK_IPL_ImageTransformer_Acquire m_PEAK_IPL_ImageTransformer_Acquire{};
    dyn_PEAK_IPL_Image_SetTimestamp m_PEAK_IPL_Image_SetTimestamp{};
    dyn_PEAK_IPL_Image_GetTimestamp m_PEAK_IPL_Image_GetTimestamp{};
    dyn_PEAK_IPL_ImageTransformer_MirrorUpDown m_PEAK_IPL_ImageTransformer_MirrorUpDown{};
    dyn_PEAK_IPL_ImageTransformer_MirrorLeftRight m_PEAK_IPL_ImageTransformer_MirrorLeftRight{};
    dyn_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight{};
    dyn_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace m_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace{};
    dyn_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace m_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace{};
    dyn_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace{};
    dyn_PEAK_IPL_ImageTransformer_Rotate m_PEAK_IPL_ImageTransformer_Rotate{};
    dyn_PEAK_IPL_ImageTransformer_RotateInPlace m_PEAK_IPL_ImageTransformer_RotateInPlace{};
    dyn_PEAK_IPL_ImageTransformer_Destruct m_PEAK_IPL_ImageTransformer_Destruct{};
    dyn_PEAK_IPL_Histogram_Construct m_PEAK_IPL_Histogram_Construct{};
    dyn_PEAK_IPL_Histogram_Acquire m_PEAK_IPL_Histogram_Acquire{};
    dyn_PEAK_IPL_Histogram_GetPixelFormat m_PEAK_IPL_Histogram_GetPixelFormat{};
    dyn_PEAK_IPL_Histogram_GetNumChannels m_PEAK_IPL_Histogram_GetNumChannels{};
    dyn_PEAK_IPL_Histogram_GetPixelSumForChannel m_PEAK_IPL_Histogram_GetPixelSumForChannel{};
    dyn_PEAK_IPL_Histogram_GetPixelCountForChannel m_PEAK_IPL_Histogram_GetPixelCountForChannel{};
    dyn_PEAK_IPL_Histogram_GetBinsForChannel m_PEAK_IPL_Histogram_GetBinsForChannel{};
    dyn_PEAK_IPL_Histogram_Destruct m_PEAK_IPL_Histogram_Destruct{};
    dyn_PEAK_IPL_PixelLine_Construct m_PEAK_IPL_PixelLine_Construct{};
    dyn_PEAK_IPL_PixelLine_Acquire m_PEAK_IPL_PixelLine_Acquire{};
    dyn_PEAK_IPL_PixelLine_GetPixelFormat m_PEAK_IPL_PixelLine_GetPixelFormat{};
    dyn_PEAK_IPL_PixelLine_GetOrientation m_PEAK_IPL_PixelLine_GetOrientation{};
    dyn_PEAK_IPL_PixelLine_GetOffset m_PEAK_IPL_PixelLine_GetOffset{};
    dyn_PEAK_IPL_PixelLine_GetNumChannels m_PEAK_IPL_PixelLine_GetNumChannels{};
    dyn_PEAK_IPL_PixelLine_GetValuesForChannel m_PEAK_IPL_PixelLine_GetValuesForChannel{};
    dyn_PEAK_IPL_PixelLine_Destruct m_PEAK_IPL_PixelLine_Destruct{};
    dyn_PEAK_IPL_PixelFormat_GetNumChannels m_PEAK_IPL_PixelFormat_GetNumChannels{};
    dyn_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel{};
    dyn_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel{};
    dyn_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel m_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel{};
    dyn_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel{};
    dyn_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel{};
    dyn_PEAK_IPL_PixelFormat_GetEndianness m_PEAK_IPL_PixelFormat_GetEndianness{};
    dyn_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels m_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels{};
    dyn_PEAK_IPL_PixelFormat_IsPacked m_PEAK_IPL_PixelFormat_IsPacked{};
    dyn_PEAK_IPL_PixelFormat_IsBayered m_PEAK_IPL_PixelFormat_IsBayered{};
    dyn_PEAK_IPL_PixelFormat_IsYUV m_PEAK_IPL_PixelFormat_IsYUV{};
    dyn_PEAK_IPL_PixelFormat_GetBayerPattern m_PEAK_IPL_PixelFormat_GetBayerPattern{};
    dyn_PEAK_IPL_ImageWriter_WriteAsRAW m_PEAK_IPL_ImageWriter_WriteAsRAW{};
    dyn_PEAK_IPL_ImageWriter_WriteAsBMP m_PEAK_IPL_ImageWriter_WriteAsBMP{};
    dyn_PEAK_IPL_ImageWriter_WriteAsPNG m_PEAK_IPL_ImageWriter_WriteAsPNG{};
    dyn_PEAK_IPL_ImageWriter_WriteAsJPG m_PEAK_IPL_ImageWriter_WriteAsJPG{};
    dyn_PEAK_IPL_ImageWriter_Write m_PEAK_IPL_ImageWriter_Write{};
    dyn_PEAK_IPL_ImageReader_Read m_PEAK_IPL_ImageReader_Read{};
    dyn_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat m_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat{};
    dyn_PEAK_IPL_VideoWriter_Video_Open m_PEAK_IPL_VideoWriter_Video_Open{};
    dyn_PEAK_IPL_VideoWriter_Video_IsOpen m_PEAK_IPL_VideoWriter_Video_IsOpen{};
    dyn_PEAK_IPL_VideoWriter_Video_Close m_PEAK_IPL_VideoWriter_Video_Close{};
    dyn_PEAK_IPL_VideoWriter_Video_Append m_PEAK_IPL_VideoWriter_Video_Append{};
    dyn_PEAK_IPL_VideoWriter_Encoder_SetOption m_PEAK_IPL_VideoWriter_Encoder_SetOption{};
    dyn_PEAK_IPL_VideoWriter_Encoder_GetOption m_PEAK_IPL_VideoWriter_Encoder_GetOption{};
    dyn_PEAK_IPL_VideoWriter_Encoder_OptionRange m_PEAK_IPL_VideoWriter_Encoder_OptionRange{};
    dyn_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer m_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer{};
    dyn_PEAK_IPL_VideoWriter_Container_SetOption m_PEAK_IPL_VideoWriter_Container_SetOption{};
    dyn_PEAK_IPL_VideoWriter_Container_GetOption m_PEAK_IPL_VideoWriter_Container_GetOption{};
    dyn_PEAK_IPL_VideoWriter_Container_OptionRange m_PEAK_IPL_VideoWriter_Container_OptionRange{};
    dyn_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder m_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder{};
    dyn_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats m_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats{};
    dyn_PEAK_IPL_VideoWriter_Video_GetEncodedFrames m_PEAK_IPL_VideoWriter_Video_GetEncodedFrames{};
    dyn_PEAK_IPL_VideoWriter_Video_GetDroppedFrames m_PEAK_IPL_VideoWriter_Video_GetDroppedFrames{};
    dyn_PEAK_IPL_VideoWriter_Video_GetFileSize m_PEAK_IPL_VideoWriter_Video_GetFileSize{};
    dyn_PEAK_IPL_VideoWriter_Video_ResetFrameCounters m_PEAK_IPL_VideoWriter_Video_ResetFrameCounters{};
    dyn_PEAK_IPL_VideoWriter_Queue_SetSize m_PEAK_IPL_VideoWriter_Queue_SetSize{};
    dyn_PEAK_IPL_VideoWriter_Queue_GetSize m_PEAK_IPL_VideoWriter_Queue_GetSize{};
    dyn_PEAK_IPL_VideoWriter_Queue_GetRange m_PEAK_IPL_VideoWriter_Queue_GetRange{};
    dyn_PEAK_IPL_VideoWriter_Queue_GetQueued m_PEAK_IPL_VideoWriter_Queue_GetQueued{};
    dyn_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone m_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone{};
    dyn_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty m_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty{};
    dyn_PEAK_IPL_ImageSharpness_Construct m_PEAK_IPL_ImageSharpness_Construct{};
    dyn_PEAK_IPL_ImageSharpness_Acquire m_PEAK_IPL_ImageSharpness_Acquire{};
    dyn_PEAK_IPL_ImageSharpness_Destruct m_PEAK_IPL_ImageSharpness_Destruct{};
    dyn_PEAK_IPL_ImageSharpness_Measure m_PEAK_IPL_ImageSharpness_Measure{};
    dyn_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported m_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported{};
    dyn_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList m_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList{};
    dyn_PEAK_IPL_ImageSharpness_SetAlgorithm m_PEAK_IPL_ImageSharpness_SetAlgorithm{};
    dyn_PEAK_IPL_ImageSharpness_GetAlgorithm m_PEAK_IPL_ImageSharpness_GetAlgorithm{};
    dyn_PEAK_IPL_ImageSharpness_GetROIMinimumSize m_PEAK_IPL_ImageSharpness_GetROIMinimumSize{};
    dyn_PEAK_IPL_ImageSharpness_SetROIs m_PEAK_IPL_ImageSharpness_SetROIs{};
    dyn_PEAK_IPL_ImageSharpness_GetROIs m_PEAK_IPL_ImageSharpness_GetROIs{};
    dyn_PEAK_IPL_EdgeEnhancement_Construct m_PEAK_IPL_EdgeEnhancement_Construct{};
    dyn_PEAK_IPL_EdgeEnhancement_Acquire m_PEAK_IPL_EdgeEnhancement_Acquire{};
    dyn_PEAK_IPL_EdgeEnhancement_Destruct m_PEAK_IPL_EdgeEnhancement_Destruct{};
    dyn_PEAK_IPL_EdgeEnhancement_ProcessInPlace m_PEAK_IPL_EdgeEnhancement_ProcessInPlace{};
    dyn_PEAK_IPL_EdgeEnhancement_Process m_PEAK_IPL_EdgeEnhancement_Process{};
    dyn_PEAK_IPL_EdgeEnhancement_GetFactorRange m_PEAK_IPL_EdgeEnhancement_GetFactorRange{};
    dyn_PEAK_IPL_EdgeEnhancement_SetFactor m_PEAK_IPL_EdgeEnhancement_SetFactor{};
    dyn_PEAK_IPL_EdgeEnhancement_GetFactor m_PEAK_IPL_EdgeEnhancement_GetFactor{};
    dyn_PEAK_IPL_EdgeEnhancement_GetDefaultFactor m_PEAK_IPL_EdgeEnhancement_GetDefaultFactor{};
    dyn_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported m_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported{};

};

inline void* import_function(void *module, const char* proc_name)
{
#ifdef __linux__
    return dlsym(module, proc_name);
#else
    return GetProcAddress(static_cast<HMODULE>(module), proc_name);
#endif
}
            
inline DynamicLoader::DynamicLoader()
{
#if defined _WIN32 || defined _WIN64
    size_t sz = 0;
    if (_wgetenv_s(&sz, NULL, 0, L"IDS_PEAK_GENERIC_SDK_PATH") == 0
        && sz > 0)
    {
        std::vector<wchar_t> env_ids_peak_ipl(sz);
        if (_wgetenv_s(&sz, env_ids_peak_ipl.data(), sz, L"IDS_PEAK_GENERIC_SDK_PATH") == 0)
        {
            if (_wgetenv_s(&sz, NULL, 0, L"PATH") == 0
                && sz > 0)
            {
                std::vector<wchar_t> env_path(sz);
                if (_wgetenv_s(&sz, env_path.data(), sz, L"PATH") == 0)
                {
                    std::wstring ids_peak_ipl_path(env_ids_peak_ipl.data());
#ifdef _WIN64
                    ids_peak_ipl_path.append(L"\\ipl\\lib\\x86_64");
#else
                    ids_peak_ipl_path.append(L"\\ipl\\lib\\x86_32");
#endif
                    std::wstring path_var(env_path.data());
                    path_var.append(L";").append(ids_peak_ipl_path);
                    _wputenv_s(L"PATH", path_var.c_str());
                }
            }
        }
    }
    
    loadLib("ids_peak_ipl.dll");
#else
    loadLib("libids_peak_ipl.so");
#endif
}

inline DynamicLoader::~DynamicLoader()
{
    if(m_handle != nullptr)
    {
        unload();
    }
}

inline bool DynamicLoader::isLoaded()
{
    auto&& inst = instance();
    return inst.m_handle != nullptr;
}

inline void DynamicLoader::unload()
{
    setPointers(false);
    
    if (m_handle != nullptr)
    {
#ifdef __linux__
        dlclose(m_handle);
#else
        FreeLibrary(static_cast<HMODULE>(m_handle));
#endif
    }
    m_handle = nullptr;
}


inline bool DynamicLoader::loadLib(const char* file)
{
    bool ret = false;
    
    if (file)
    {
#ifdef __linux__
        m_handle = dlopen(file, RTLD_NOW);
#else
        m_handle = LoadLibraryA(file);
#endif
        if (m_handle != nullptr)
        {
            try {
                setPointers(true);
                ret = true;
            } catch (const std::exception&) {
                unload();
                throw;
            }
        }
        else
        {
            throw std::runtime_error(std::string("Lib load failed: ") + file);
        }
    }
    else
    {
        throw std::runtime_error("Filename empty");
    }

    return ret;
}

inline bool DynamicLoader::setPointers(bool load)
{

    m_PEAK_IPL_Library_GetVersionMajor = (dyn_PEAK_IPL_Library_GetVersionMajor) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetVersionMajor") : nullptr);
    if(m_PEAK_IPL_Library_GetVersionMajor == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetVersionMajor");
    }        

    m_PEAK_IPL_Library_GetVersionMinor = (dyn_PEAK_IPL_Library_GetVersionMinor) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetVersionMinor") : nullptr);
    if(m_PEAK_IPL_Library_GetVersionMinor == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetVersionMinor");
    }        

    m_PEAK_IPL_Library_GetVersionSubminor = (dyn_PEAK_IPL_Library_GetVersionSubminor) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetVersionSubminor") : nullptr);
    if(m_PEAK_IPL_Library_GetVersionSubminor == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetVersionSubminor");
    }        

    m_PEAK_IPL_Library_GetMaxConcurrentThreads = (dyn_PEAK_IPL_Library_GetMaxConcurrentThreads) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetMaxConcurrentThreads") : nullptr);
    if(m_PEAK_IPL_Library_GetMaxConcurrentThreads == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetMaxConcurrentThreads");
    }        

    m_PEAK_IPL_Library_SetMaxConcurrentThreads = (dyn_PEAK_IPL_Library_SetMaxConcurrentThreads) (load ?  import_function(m_handle, "PEAK_IPL_Library_SetMaxConcurrentThreads") : nullptr);
    if(m_PEAK_IPL_Library_SetMaxConcurrentThreads == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_SetMaxConcurrentThreads");
    }        

    m_PEAK_IPL_Library_GetMultiThreadingEnabled = (dyn_PEAK_IPL_Library_GetMultiThreadingEnabled) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetMultiThreadingEnabled") : nullptr);
    if(m_PEAK_IPL_Library_GetMultiThreadingEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetMultiThreadingEnabled");
    }        

    m_PEAK_IPL_Library_SetMultiThreadingEnabled = (dyn_PEAK_IPL_Library_SetMultiThreadingEnabled) (load ?  import_function(m_handle, "PEAK_IPL_Library_SetMultiThreadingEnabled") : nullptr);
    if(m_PEAK_IPL_Library_SetMultiThreadingEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_SetMultiThreadingEnabled");
    }        

    m_PEAK_IPL_Library_GetSIMDEnabled = (dyn_PEAK_IPL_Library_GetSIMDEnabled) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetSIMDEnabled") : nullptr);
    if(m_PEAK_IPL_Library_GetSIMDEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetSIMDEnabled");
    }        

    m_PEAK_IPL_Library_SetSIMDEnabled = (dyn_PEAK_IPL_Library_SetSIMDEnabled) (load ?  import_function(m_handle, "PEAK_IPL_Library_SetSIMDEnabled") : nullptr);
    if(m_PEAK_IPL_Library_SetSIMDEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_SetSIMDEnabled");
    }        

    m_PEAK_IPL_Library_GetLastError = (dyn_PEAK_IPL_Library_GetLastError) (load ?  import_function(m_handle, "PEAK_IPL_Library_GetLastError") : nullptr);
    if(m_PEAK_IPL_Library_GetLastError == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Library_GetLastError");
    }        

    m_PEAK_IPL_ImageConverter_Construct = (dyn_PEAK_IPL_ImageConverter_Construct) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_Construct") : nullptr);
    if(m_PEAK_IPL_ImageConverter_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_Construct");
    }        

    m_PEAK_IPL_ImageConverter_Acquire = (dyn_PEAK_IPL_ImageConverter_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_Acquire") : nullptr);
    if(m_PEAK_IPL_ImageConverter_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_Acquire");
    }        

    m_PEAK_IPL_ImageConverter_PreAllocateConversion = (dyn_PEAK_IPL_ImageConverter_PreAllocateConversion) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_PreAllocateConversion") : nullptr);
    if(m_PEAK_IPL_ImageConverter_PreAllocateConversion == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_PreAllocateConversion");
    }        

    m_PEAK_IPL_ImageConverter_GetConversionMode = (dyn_PEAK_IPL_ImageConverter_GetConversionMode) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_GetConversionMode") : nullptr);
    if(m_PEAK_IPL_ImageConverter_GetConversionMode == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_GetConversionMode");
    }        

    m_PEAK_IPL_ImageConverter_SetConversionMode = (dyn_PEAK_IPL_ImageConverter_SetConversionMode) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_SetConversionMode") : nullptr);
    if(m_PEAK_IPL_ImageConverter_SetConversionMode == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_SetConversionMode");
    }        

    m_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats = (dyn_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats") : nullptr);
    if(m_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats");
    }        

    m_PEAK_IPL_ImageConverter_Convert = (dyn_PEAK_IPL_ImageConverter_Convert) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_Convert") : nullptr);
    if(m_PEAK_IPL_ImageConverter_Convert == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_Convert");
    }        

    m_PEAK_IPL_ImageConverter_ConvertToBuffer = (dyn_PEAK_IPL_ImageConverter_ConvertToBuffer) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_ConvertToBuffer") : nullptr);
    if(m_PEAK_IPL_ImageConverter_ConvertToBuffer == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_ConvertToBuffer");
    }        

    m_PEAK_IPL_ImageConverter_Destruct = (dyn_PEAK_IPL_ImageConverter_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_ImageConverter_Destruct") : nullptr);
    if(m_PEAK_IPL_ImageConverter_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageConverter_Destruct");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_Construct = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Construct) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_Construct") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_Construct");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_Acquire") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_Acquire");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_Detect = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Detect) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_Detect") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_Detect == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_Detect");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_Correct = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Correct) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_Correct") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_Correct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_Correct");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace");
    }        

    m_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct = (dyn_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_AdaptiveHotpixelCorrector_Destruct") : nullptr);
    if(m_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_AdaptiveHotpixelCorrector_Destruct");
    }        

    m_PEAK_IPL_ColorCorrector_Construct = (dyn_PEAK_IPL_ColorCorrector_Construct) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_Construct") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_Construct");
    }        

    m_PEAK_IPL_ColorCorrector_Acquire = (dyn_PEAK_IPL_ColorCorrector_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_Acquire") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_Acquire");
    }        

    m_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors = (dyn_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_SetColorCorrectionFactors") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_SetColorCorrectionFactors");
    }        

    m_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors = (dyn_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_GetColorCorrectionFactors") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_GetColorCorrectionFactors");
    }        

    m_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported = (dyn_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported");
    }        

    m_PEAK_IPL_ColorCorrector_Process = (dyn_PEAK_IPL_ColorCorrector_Process) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_Process") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_Process");
    }        

    m_PEAK_IPL_ColorCorrector_ProcessInPlace = (dyn_PEAK_IPL_ColorCorrector_ProcessInPlace) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_ProcessInPlace") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_ProcessInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_ProcessInPlace");
    }        

    m_PEAK_IPL_ColorCorrector_Destruct = (dyn_PEAK_IPL_ColorCorrector_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_Destruct") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_Destruct");
    }        

    m_PEAK_IPL_ColorCorrector_GetSaturation = (dyn_PEAK_IPL_ColorCorrector_GetSaturation) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_GetSaturation") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_GetSaturation == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_GetSaturation");
    }        

    m_PEAK_IPL_ColorCorrector_GetSaturationMin = (dyn_PEAK_IPL_ColorCorrector_GetSaturationMin) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_GetSaturationMin") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_GetSaturationMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_GetSaturationMin");
    }        

    m_PEAK_IPL_ColorCorrector_GetSaturationMax = (dyn_PEAK_IPL_ColorCorrector_GetSaturationMax) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_GetSaturationMax") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_GetSaturationMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_GetSaturationMax");
    }        

    m_PEAK_IPL_ColorCorrector_SetSaturation = (dyn_PEAK_IPL_ColorCorrector_SetSaturation) (load ?  import_function(m_handle, "PEAK_IPL_ColorCorrector_SetSaturation") : nullptr);
    if(m_PEAK_IPL_ColorCorrector_SetSaturation == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ColorCorrector_SetSaturation");
    }        

    m_PEAK_IPL_Binning_Construct = (dyn_PEAK_IPL_Binning_Construct) (load ?  import_function(m_handle, "PEAK_IPL_Binning_Construct") : nullptr);
    if(m_PEAK_IPL_Binning_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_Construct");
    }        

    m_PEAK_IPL_Binning_Acquire = (dyn_PEAK_IPL_Binning_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_Binning_Acquire") : nullptr);
    if(m_PEAK_IPL_Binning_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_Acquire");
    }        

    m_PEAK_IPL_Binning_SetBinningX = (dyn_PEAK_IPL_Binning_SetBinningX) (load ?  import_function(m_handle, "PEAK_IPL_Binning_SetBinningX") : nullptr);
    if(m_PEAK_IPL_Binning_SetBinningX == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_SetBinningX");
    }        

    m_PEAK_IPL_Binning_GetBinningX = (dyn_PEAK_IPL_Binning_GetBinningX) (load ?  import_function(m_handle, "PEAK_IPL_Binning_GetBinningX") : nullptr);
    if(m_PEAK_IPL_Binning_GetBinningX == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_GetBinningX");
    }        

    m_PEAK_IPL_Binning_SetBinningY = (dyn_PEAK_IPL_Binning_SetBinningY) (load ?  import_function(m_handle, "PEAK_IPL_Binning_SetBinningY") : nullptr);
    if(m_PEAK_IPL_Binning_SetBinningY == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_SetBinningY");
    }        

    m_PEAK_IPL_Binning_GetBinningY = (dyn_PEAK_IPL_Binning_GetBinningY) (load ?  import_function(m_handle, "PEAK_IPL_Binning_GetBinningY") : nullptr);
    if(m_PEAK_IPL_Binning_GetBinningY == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_GetBinningY");
    }        

    m_PEAK_IPL_Binning_GetBinningMin = (dyn_PEAK_IPL_Binning_GetBinningMin) (load ?  import_function(m_handle, "PEAK_IPL_Binning_GetBinningMin") : nullptr);
    if(m_PEAK_IPL_Binning_GetBinningMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_GetBinningMin");
    }        

    m_PEAK_IPL_Binning_GetBinningMax = (dyn_PEAK_IPL_Binning_GetBinningMax) (load ?  import_function(m_handle, "PEAK_IPL_Binning_GetBinningMax") : nullptr);
    if(m_PEAK_IPL_Binning_GetBinningMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_GetBinningMax");
    }        

    m_PEAK_IPL_Binning_ExpectedOutputSize = (dyn_PEAK_IPL_Binning_ExpectedOutputSize) (load ?  import_function(m_handle, "PEAK_IPL_Binning_ExpectedOutputSize") : nullptr);
    if(m_PEAK_IPL_Binning_ExpectedOutputSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_ExpectedOutputSize");
    }        

    m_PEAK_IPL_Binning_IsPixelformatSupported = (dyn_PEAK_IPL_Binning_IsPixelformatSupported) (load ?  import_function(m_handle, "PEAK_IPL_Binning_IsPixelformatSupported") : nullptr);
    if(m_PEAK_IPL_Binning_IsPixelformatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_IsPixelformatSupported");
    }        

    m_PEAK_IPL_Binning_SetBinningMode = (dyn_PEAK_IPL_Binning_SetBinningMode) (load ?  import_function(m_handle, "PEAK_IPL_Binning_SetBinningMode") : nullptr);
    if(m_PEAK_IPL_Binning_SetBinningMode == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_SetBinningMode");
    }        

    m_PEAK_IPL_Binning_GetBinningMode = (dyn_PEAK_IPL_Binning_GetBinningMode) (load ?  import_function(m_handle, "PEAK_IPL_Binning_GetBinningMode") : nullptr);
    if(m_PEAK_IPL_Binning_GetBinningMode == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_GetBinningMode");
    }        

    m_PEAK_IPL_Binning_Process = (dyn_PEAK_IPL_Binning_Process) (load ?  import_function(m_handle, "PEAK_IPL_Binning_Process") : nullptr);
    if(m_PEAK_IPL_Binning_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_Process");
    }        

    m_PEAK_IPL_Binning_Destruct = (dyn_PEAK_IPL_Binning_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_Binning_Destruct") : nullptr);
    if(m_PEAK_IPL_Binning_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Binning_Destruct");
    }        

    m_PEAK_IPL_Decimation_Construct = (dyn_PEAK_IPL_Decimation_Construct) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_Construct") : nullptr);
    if(m_PEAK_IPL_Decimation_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_Construct");
    }        

    m_PEAK_IPL_Decimation_Acquire = (dyn_PEAK_IPL_Decimation_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_Acquire") : nullptr);
    if(m_PEAK_IPL_Decimation_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_Acquire");
    }        

    m_PEAK_IPL_Decimation_SetDecimationX = (dyn_PEAK_IPL_Decimation_SetDecimationX) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_SetDecimationX") : nullptr);
    if(m_PEAK_IPL_Decimation_SetDecimationX == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_SetDecimationX");
    }        

    m_PEAK_IPL_Decimation_GetDecimationX = (dyn_PEAK_IPL_Decimation_GetDecimationX) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_GetDecimationX") : nullptr);
    if(m_PEAK_IPL_Decimation_GetDecimationX == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_GetDecimationX");
    }        

    m_PEAK_IPL_Decimation_SetDecimationY = (dyn_PEAK_IPL_Decimation_SetDecimationY) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_SetDecimationY") : nullptr);
    if(m_PEAK_IPL_Decimation_SetDecimationY == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_SetDecimationY");
    }        

    m_PEAK_IPL_Decimation_GetDecimationY = (dyn_PEAK_IPL_Decimation_GetDecimationY) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_GetDecimationY") : nullptr);
    if(m_PEAK_IPL_Decimation_GetDecimationY == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_GetDecimationY");
    }        

    m_PEAK_IPL_Decimation_GetDecimationMin = (dyn_PEAK_IPL_Decimation_GetDecimationMin) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_GetDecimationMin") : nullptr);
    if(m_PEAK_IPL_Decimation_GetDecimationMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_GetDecimationMin");
    }        

    m_PEAK_IPL_Decimation_GetDecimationMax = (dyn_PEAK_IPL_Decimation_GetDecimationMax) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_GetDecimationMax") : nullptr);
    if(m_PEAK_IPL_Decimation_GetDecimationMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_GetDecimationMax");
    }        

    m_PEAK_IPL_Decimation_ExpectedOutputSize = (dyn_PEAK_IPL_Decimation_ExpectedOutputSize) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_ExpectedOutputSize") : nullptr);
    if(m_PEAK_IPL_Decimation_ExpectedOutputSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_ExpectedOutputSize");
    }        

    m_PEAK_IPL_Decimation_IsPixelformatSupported = (dyn_PEAK_IPL_Decimation_IsPixelformatSupported) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_IsPixelformatSupported") : nullptr);
    if(m_PEAK_IPL_Decimation_IsPixelformatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_IsPixelformatSupported");
    }        

    m_PEAK_IPL_Decimation_Process = (dyn_PEAK_IPL_Decimation_Process) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_Process") : nullptr);
    if(m_PEAK_IPL_Decimation_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_Process");
    }        

    m_PEAK_IPL_Decimation_Destruct = (dyn_PEAK_IPL_Decimation_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_Decimation_Destruct") : nullptr);
    if(m_PEAK_IPL_Decimation_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Decimation_Destruct");
    }        

    m_PEAK_IPL_Gain_Construct = (dyn_PEAK_IPL_Gain_Construct) (load ?  import_function(m_handle, "PEAK_IPL_Gain_Construct") : nullptr);
    if(m_PEAK_IPL_Gain_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_Construct");
    }        

    m_PEAK_IPL_Gain_Acquire = (dyn_PEAK_IPL_Gain_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_Gain_Acquire") : nullptr);
    if(m_PEAK_IPL_Gain_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_Acquire");
    }        

    m_PEAK_IPL_Gain_SetMasterGainValue = (dyn_PEAK_IPL_Gain_SetMasterGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_SetMasterGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_SetMasterGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_SetMasterGainValue");
    }        

    m_PEAK_IPL_Gain_GetMasterGainValue = (dyn_PEAK_IPL_Gain_GetMasterGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetMasterGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_GetMasterGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetMasterGainValue");
    }        

    m_PEAK_IPL_Gain_GetMasterGainMax = (dyn_PEAK_IPL_Gain_GetMasterGainMax) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetMasterGainMax") : nullptr);
    if(m_PEAK_IPL_Gain_GetMasterGainMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetMasterGainMax");
    }        

    m_PEAK_IPL_Gain_GetMasterGainMin = (dyn_PEAK_IPL_Gain_GetMasterGainMin) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetMasterGainMin") : nullptr);
    if(m_PEAK_IPL_Gain_GetMasterGainMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetMasterGainMin");
    }        

    m_PEAK_IPL_Gain_SetRedGainValue = (dyn_PEAK_IPL_Gain_SetRedGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_SetRedGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_SetRedGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_SetRedGainValue");
    }        

    m_PEAK_IPL_Gain_GetRedGainValue = (dyn_PEAK_IPL_Gain_GetRedGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetRedGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_GetRedGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetRedGainValue");
    }        

    m_PEAK_IPL_Gain_GetRedGainMax = (dyn_PEAK_IPL_Gain_GetRedGainMax) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetRedGainMax") : nullptr);
    if(m_PEAK_IPL_Gain_GetRedGainMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetRedGainMax");
    }        

    m_PEAK_IPL_Gain_GetRedGainMin = (dyn_PEAK_IPL_Gain_GetRedGainMin) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetRedGainMin") : nullptr);
    if(m_PEAK_IPL_Gain_GetRedGainMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetRedGainMin");
    }        

    m_PEAK_IPL_Gain_SetGreenGainValue = (dyn_PEAK_IPL_Gain_SetGreenGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_SetGreenGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_SetGreenGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_SetGreenGainValue");
    }        

    m_PEAK_IPL_Gain_GetGreenGainValue = (dyn_PEAK_IPL_Gain_GetGreenGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetGreenGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_GetGreenGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetGreenGainValue");
    }        

    m_PEAK_IPL_Gain_GetGreenGainMax = (dyn_PEAK_IPL_Gain_GetGreenGainMax) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetGreenGainMax") : nullptr);
    if(m_PEAK_IPL_Gain_GetGreenGainMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetGreenGainMax");
    }        

    m_PEAK_IPL_Gain_GetGreenGainMin = (dyn_PEAK_IPL_Gain_GetGreenGainMin) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetGreenGainMin") : nullptr);
    if(m_PEAK_IPL_Gain_GetGreenGainMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetGreenGainMin");
    }        

    m_PEAK_IPL_Gain_SetBlueGainValue = (dyn_PEAK_IPL_Gain_SetBlueGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_SetBlueGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_SetBlueGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_SetBlueGainValue");
    }        

    m_PEAK_IPL_Gain_GetBlueGainValue = (dyn_PEAK_IPL_Gain_GetBlueGainValue) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetBlueGainValue") : nullptr);
    if(m_PEAK_IPL_Gain_GetBlueGainValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetBlueGainValue");
    }        

    m_PEAK_IPL_Gain_GetBlueGainMax = (dyn_PEAK_IPL_Gain_GetBlueGainMax) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetBlueGainMax") : nullptr);
    if(m_PEAK_IPL_Gain_GetBlueGainMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetBlueGainMax");
    }        

    m_PEAK_IPL_Gain_GetBlueGainMin = (dyn_PEAK_IPL_Gain_GetBlueGainMin) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetBlueGainMin") : nullptr);
    if(m_PEAK_IPL_Gain_GetBlueGainMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetBlueGainMin");
    }        

    m_PEAK_IPL_Gain_GetIsPixelFormatSupported = (dyn_PEAK_IPL_Gain_GetIsPixelFormatSupported) (load ?  import_function(m_handle, "PEAK_IPL_Gain_GetIsPixelFormatSupported") : nullptr);
    if(m_PEAK_IPL_Gain_GetIsPixelFormatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_GetIsPixelFormatSupported");
    }        

    m_PEAK_IPL_Gain_Process = (dyn_PEAK_IPL_Gain_Process) (load ?  import_function(m_handle, "PEAK_IPL_Gain_Process") : nullptr);
    if(m_PEAK_IPL_Gain_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_Process");
    }        

    m_PEAK_IPL_Gain_ProcessInPlace = (dyn_PEAK_IPL_Gain_ProcessInPlace) (load ?  import_function(m_handle, "PEAK_IPL_Gain_ProcessInPlace") : nullptr);
    if(m_PEAK_IPL_Gain_ProcessInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_ProcessInPlace");
    }        

    m_PEAK_IPL_Gain_Destruct = (dyn_PEAK_IPL_Gain_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_Gain_Destruct") : nullptr);
    if(m_PEAK_IPL_Gain_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Gain_Destruct");
    }        

    m_PEAK_IPL_GammaCorrector_Construct = (dyn_PEAK_IPL_GammaCorrector_Construct) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_Construct") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_Construct");
    }        

    m_PEAK_IPL_GammaCorrector_Acquire = (dyn_PEAK_IPL_GammaCorrector_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_Acquire") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_Acquire");
    }        

    m_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue = (dyn_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_SetGammaCorrectionValue") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_SetGammaCorrectionValue");
    }        

    m_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue = (dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_GetGammaCorrectionValue") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_GetGammaCorrectionValue");
    }        

    m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax = (dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_GetGammaCorrectionMax") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_GetGammaCorrectionMax");
    }        

    m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin = (dyn_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_GetGammaCorrectionMin") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_GetGammaCorrectionMin");
    }        

    m_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported = (dyn_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported");
    }        

    m_PEAK_IPL_GammaCorrector_Process = (dyn_PEAK_IPL_GammaCorrector_Process) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_Process") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_Process");
    }        

    m_PEAK_IPL_GammaCorrector_ProcessInPlace = (dyn_PEAK_IPL_GammaCorrector_ProcessInPlace) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_ProcessInPlace") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_ProcessInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_ProcessInPlace");
    }        

    m_PEAK_IPL_GammaCorrector_Destruct = (dyn_PEAK_IPL_GammaCorrector_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_GammaCorrector_Destruct") : nullptr);
    if(m_PEAK_IPL_GammaCorrector_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_GammaCorrector_Destruct");
    }        

    m_PEAK_IPL_Image_Construct = (dyn_PEAK_IPL_Image_Construct) (load ?  import_function(m_handle, "PEAK_IPL_Image_Construct") : nullptr);
    if(m_PEAK_IPL_Image_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_Construct");
    }        

    m_PEAK_IPL_Image_Acquire = (dyn_PEAK_IPL_Image_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_Image_Acquire") : nullptr);
    if(m_PEAK_IPL_Image_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_Acquire");
    }        

    m_PEAK_IPL_Image_ConstructFromBuffer = (dyn_PEAK_IPL_Image_ConstructFromBuffer) (load ?  import_function(m_handle, "PEAK_IPL_Image_ConstructFromBuffer") : nullptr);
    if(m_PEAK_IPL_Image_ConstructFromBuffer == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_ConstructFromBuffer");
    }        

    m_PEAK_IPL_Image_GetWidth = (dyn_PEAK_IPL_Image_GetWidth) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetWidth") : nullptr);
    if(m_PEAK_IPL_Image_GetWidth == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetWidth");
    }        

    m_PEAK_IPL_Image_GetHeight = (dyn_PEAK_IPL_Image_GetHeight) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetHeight") : nullptr);
    if(m_PEAK_IPL_Image_GetHeight == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetHeight");
    }        

    m_PEAK_IPL_Image_GetPixelPointer = (dyn_PEAK_IPL_Image_GetPixelPointer) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetPixelPointer") : nullptr);
    if(m_PEAK_IPL_Image_GetPixelPointer == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetPixelPointer");
    }        

    m_PEAK_IPL_Image_GetByteCount = (dyn_PEAK_IPL_Image_GetByteCount) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetByteCount") : nullptr);
    if(m_PEAK_IPL_Image_GetByteCount == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetByteCount");
    }        

    m_PEAK_IPL_Image_GetPixelFormat = (dyn_PEAK_IPL_Image_GetPixelFormat) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetPixelFormat") : nullptr);
    if(m_PEAK_IPL_Image_GetPixelFormat == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetPixelFormat");
    }        

    m_PEAK_IPL_Image_GetData = (dyn_PEAK_IPL_Image_GetData) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetData") : nullptr);
    if(m_PEAK_IPL_Image_GetData == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetData");
    }        

    m_PEAK_IPL_Image_Clone = (dyn_PEAK_IPL_Image_Clone) (load ?  import_function(m_handle, "PEAK_IPL_Image_Clone") : nullptr);
    if(m_PEAK_IPL_Image_Clone == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_Clone");
    }        

    m_PEAK_IPL_Image_ConvertTo = (dyn_PEAK_IPL_Image_ConvertTo) (load ?  import_function(m_handle, "PEAK_IPL_Image_ConvertTo") : nullptr);
    if(m_PEAK_IPL_Image_ConvertTo == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_ConvertTo");
    }        

    m_PEAK_IPL_Image_Scale = (dyn_PEAK_IPL_Image_Scale) (load ?  import_function(m_handle, "PEAK_IPL_Image_Scale") : nullptr);
    if(m_PEAK_IPL_Image_Scale == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_Scale");
    }        

    m_PEAK_IPL_Image_ScaleInto = (dyn_PEAK_IPL_Image_ScaleInto) (load ?  import_function(m_handle, "PEAK_IPL_Image_ScaleInto") : nullptr);
    if(m_PEAK_IPL_Image_ScaleInto == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_ScaleInto");
    }        

    m_PEAK_IPL_Image_Crop = (dyn_PEAK_IPL_Image_Crop) (load ?  import_function(m_handle, "PEAK_IPL_Image_Crop") : nullptr);
    if(m_PEAK_IPL_Image_Crop == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_Crop");
    }        

    m_PEAK_IPL_Image_ConvertToBuffer = (dyn_PEAK_IPL_Image_ConvertToBuffer) (load ?  import_function(m_handle, "PEAK_IPL_Image_ConvertToBuffer") : nullptr);
    if(m_PEAK_IPL_Image_ConvertToBuffer == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_ConvertToBuffer");
    }        

    m_PEAK_IPL_Image_Destruct = (dyn_PEAK_IPL_Image_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_Image_Destruct") : nullptr);
    if(m_PEAK_IPL_Image_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_Destruct");
    }        

    m_PEAK_IPL_ImageTransformer_Construct = (dyn_PEAK_IPL_ImageTransformer_Construct) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_Construct") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_Construct");
    }        

    m_PEAK_IPL_ImageTransformer_Acquire = (dyn_PEAK_IPL_ImageTransformer_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_Acquire") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_Acquire");
    }        

    m_PEAK_IPL_Image_SetTimestamp = (dyn_PEAK_IPL_Image_SetTimestamp) (load ?  import_function(m_handle, "PEAK_IPL_Image_SetTimestamp") : nullptr);
    if(m_PEAK_IPL_Image_SetTimestamp == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_SetTimestamp");
    }        

    m_PEAK_IPL_Image_GetTimestamp = (dyn_PEAK_IPL_Image_GetTimestamp) (load ?  import_function(m_handle, "PEAK_IPL_Image_GetTimestamp") : nullptr);
    if(m_PEAK_IPL_Image_GetTimestamp == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Image_GetTimestamp");
    }        

    m_PEAK_IPL_ImageTransformer_MirrorUpDown = (dyn_PEAK_IPL_ImageTransformer_MirrorUpDown) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_MirrorUpDown") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_MirrorUpDown == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_MirrorUpDown");
    }        

    m_PEAK_IPL_ImageTransformer_MirrorLeftRight = (dyn_PEAK_IPL_ImageTransformer_MirrorLeftRight) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_MirrorLeftRight") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_MirrorLeftRight == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_MirrorLeftRight");
    }        

    m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight = (dyn_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight");
    }        

    m_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace = (dyn_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_MirrorUpDownInPlace") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_MirrorUpDownInPlace");
    }        

    m_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace = (dyn_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace");
    }        

    m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace = (dyn_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace");
    }        

    m_PEAK_IPL_ImageTransformer_Rotate = (dyn_PEAK_IPL_ImageTransformer_Rotate) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_Rotate") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_Rotate == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_Rotate");
    }        

    m_PEAK_IPL_ImageTransformer_RotateInPlace = (dyn_PEAK_IPL_ImageTransformer_RotateInPlace) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_RotateInPlace") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_RotateInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_RotateInPlace");
    }        

    m_PEAK_IPL_ImageTransformer_Destruct = (dyn_PEAK_IPL_ImageTransformer_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_ImageTransformer_Destruct") : nullptr);
    if(m_PEAK_IPL_ImageTransformer_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageTransformer_Destruct");
    }        

    m_PEAK_IPL_Histogram_Construct = (dyn_PEAK_IPL_Histogram_Construct) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_Construct") : nullptr);
    if(m_PEAK_IPL_Histogram_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_Construct");
    }        

    m_PEAK_IPL_Histogram_Acquire = (dyn_PEAK_IPL_Histogram_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_Acquire") : nullptr);
    if(m_PEAK_IPL_Histogram_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_Acquire");
    }        

    m_PEAK_IPL_Histogram_GetPixelFormat = (dyn_PEAK_IPL_Histogram_GetPixelFormat) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_GetPixelFormat") : nullptr);
    if(m_PEAK_IPL_Histogram_GetPixelFormat == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_GetPixelFormat");
    }        

    m_PEAK_IPL_Histogram_GetNumChannels = (dyn_PEAK_IPL_Histogram_GetNumChannels) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_GetNumChannels") : nullptr);
    if(m_PEAK_IPL_Histogram_GetNumChannels == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_GetNumChannels");
    }        

    m_PEAK_IPL_Histogram_GetPixelSumForChannel = (dyn_PEAK_IPL_Histogram_GetPixelSumForChannel) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_GetPixelSumForChannel") : nullptr);
    if(m_PEAK_IPL_Histogram_GetPixelSumForChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_GetPixelSumForChannel");
    }        

    m_PEAK_IPL_Histogram_GetPixelCountForChannel = (dyn_PEAK_IPL_Histogram_GetPixelCountForChannel) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_GetPixelCountForChannel") : nullptr);
    if(m_PEAK_IPL_Histogram_GetPixelCountForChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_GetPixelCountForChannel");
    }        

    m_PEAK_IPL_Histogram_GetBinsForChannel = (dyn_PEAK_IPL_Histogram_GetBinsForChannel) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_GetBinsForChannel") : nullptr);
    if(m_PEAK_IPL_Histogram_GetBinsForChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_GetBinsForChannel");
    }        

    m_PEAK_IPL_Histogram_Destruct = (dyn_PEAK_IPL_Histogram_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_Histogram_Destruct") : nullptr);
    if(m_PEAK_IPL_Histogram_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_Histogram_Destruct");
    }        

    m_PEAK_IPL_PixelLine_Construct = (dyn_PEAK_IPL_PixelLine_Construct) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_Construct") : nullptr);
    if(m_PEAK_IPL_PixelLine_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_Construct");
    }        

    m_PEAK_IPL_PixelLine_Acquire = (dyn_PEAK_IPL_PixelLine_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_Acquire") : nullptr);
    if(m_PEAK_IPL_PixelLine_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_Acquire");
    }        

    m_PEAK_IPL_PixelLine_GetPixelFormat = (dyn_PEAK_IPL_PixelLine_GetPixelFormat) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_GetPixelFormat") : nullptr);
    if(m_PEAK_IPL_PixelLine_GetPixelFormat == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_GetPixelFormat");
    }        

    m_PEAK_IPL_PixelLine_GetOrientation = (dyn_PEAK_IPL_PixelLine_GetOrientation) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_GetOrientation") : nullptr);
    if(m_PEAK_IPL_PixelLine_GetOrientation == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_GetOrientation");
    }        

    m_PEAK_IPL_PixelLine_GetOffset = (dyn_PEAK_IPL_PixelLine_GetOffset) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_GetOffset") : nullptr);
    if(m_PEAK_IPL_PixelLine_GetOffset == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_GetOffset");
    }        

    m_PEAK_IPL_PixelLine_GetNumChannels = (dyn_PEAK_IPL_PixelLine_GetNumChannels) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_GetNumChannels") : nullptr);
    if(m_PEAK_IPL_PixelLine_GetNumChannels == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_GetNumChannels");
    }        

    m_PEAK_IPL_PixelLine_GetValuesForChannel = (dyn_PEAK_IPL_PixelLine_GetValuesForChannel) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_GetValuesForChannel") : nullptr);
    if(m_PEAK_IPL_PixelLine_GetValuesForChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_GetValuesForChannel");
    }        

    m_PEAK_IPL_PixelLine_Destruct = (dyn_PEAK_IPL_PixelLine_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_PixelLine_Destruct") : nullptr);
    if(m_PEAK_IPL_PixelLine_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelLine_Destruct");
    }        

    m_PEAK_IPL_PixelFormat_GetNumChannels = (dyn_PEAK_IPL_PixelFormat_GetNumChannels) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetNumChannels") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetNumChannels == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetNumChannels");
    }        

    m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel = (dyn_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel");
    }        

    m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel = (dyn_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel");
    }        

    m_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel = (dyn_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetMaximumValuePerChannel") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetMaximumValuePerChannel");
    }        

    m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel = (dyn_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel");
    }        

    m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel = (dyn_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel");
    }        

    m_PEAK_IPL_PixelFormat_GetEndianness = (dyn_PEAK_IPL_PixelFormat_GetEndianness) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetEndianness") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetEndianness == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetEndianness");
    }        

    m_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels = (dyn_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels") : nullptr);
    if(m_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels");
    }        

    m_PEAK_IPL_PixelFormat_IsPacked = (dyn_PEAK_IPL_PixelFormat_IsPacked) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_IsPacked") : nullptr);
    if(m_PEAK_IPL_PixelFormat_IsPacked == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_IsPacked");
    }        

    m_PEAK_IPL_PixelFormat_IsBayered = (dyn_PEAK_IPL_PixelFormat_IsBayered) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_IsBayered") : nullptr);
    if(m_PEAK_IPL_PixelFormat_IsBayered == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_IsBayered");
    }        

    m_PEAK_IPL_PixelFormat_IsYUV = (dyn_PEAK_IPL_PixelFormat_IsYUV) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_IsYUV") : nullptr);
    if(m_PEAK_IPL_PixelFormat_IsYUV == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_IsYUV");
    }        

    m_PEAK_IPL_PixelFormat_GetBayerPattern = (dyn_PEAK_IPL_PixelFormat_GetBayerPattern) (load ?  import_function(m_handle, "PEAK_IPL_PixelFormat_GetBayerPattern") : nullptr);
    if(m_PEAK_IPL_PixelFormat_GetBayerPattern == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_PixelFormat_GetBayerPattern");
    }        

    m_PEAK_IPL_ImageWriter_WriteAsRAW = (dyn_PEAK_IPL_ImageWriter_WriteAsRAW) (load ?  import_function(m_handle, "PEAK_IPL_ImageWriter_WriteAsRAW") : nullptr);
    if(m_PEAK_IPL_ImageWriter_WriteAsRAW == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageWriter_WriteAsRAW");
    }        

    m_PEAK_IPL_ImageWriter_WriteAsBMP = (dyn_PEAK_IPL_ImageWriter_WriteAsBMP) (load ?  import_function(m_handle, "PEAK_IPL_ImageWriter_WriteAsBMP") : nullptr);
    if(m_PEAK_IPL_ImageWriter_WriteAsBMP == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageWriter_WriteAsBMP");
    }        

    m_PEAK_IPL_ImageWriter_WriteAsPNG = (dyn_PEAK_IPL_ImageWriter_WriteAsPNG) (load ?  import_function(m_handle, "PEAK_IPL_ImageWriter_WriteAsPNG") : nullptr);
    if(m_PEAK_IPL_ImageWriter_WriteAsPNG == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageWriter_WriteAsPNG");
    }        

    m_PEAK_IPL_ImageWriter_WriteAsJPG = (dyn_PEAK_IPL_ImageWriter_WriteAsJPG) (load ?  import_function(m_handle, "PEAK_IPL_ImageWriter_WriteAsJPG") : nullptr);
    if(m_PEAK_IPL_ImageWriter_WriteAsJPG == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageWriter_WriteAsJPG");
    }        

    m_PEAK_IPL_ImageWriter_Write = (dyn_PEAK_IPL_ImageWriter_Write) (load ?  import_function(m_handle, "PEAK_IPL_ImageWriter_Write") : nullptr);
    if(m_PEAK_IPL_ImageWriter_Write == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageWriter_Write");
    }        

    m_PEAK_IPL_ImageReader_Read = (dyn_PEAK_IPL_ImageReader_Read) (load ?  import_function(m_handle, "PEAK_IPL_ImageReader_Read") : nullptr);
    if(m_PEAK_IPL_ImageReader_Read == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageReader_Read");
    }        

    m_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat = (dyn_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat) (load ?  import_function(m_handle, "PEAK_IPL_ImageReaderRead_ReadAsPixelFormat") : nullptr);
    if(m_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageReaderRead_ReadAsPixelFormat");
    }        

    m_PEAK_IPL_VideoWriter_Video_Open = (dyn_PEAK_IPL_VideoWriter_Video_Open) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_Open") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_Open == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_Open");
    }        

    m_PEAK_IPL_VideoWriter_Video_IsOpen = (dyn_PEAK_IPL_VideoWriter_Video_IsOpen) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_IsOpen") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_IsOpen == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_IsOpen");
    }        

    m_PEAK_IPL_VideoWriter_Video_Close = (dyn_PEAK_IPL_VideoWriter_Video_Close) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_Close") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_Close == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_Close");
    }        

    m_PEAK_IPL_VideoWriter_Video_Append = (dyn_PEAK_IPL_VideoWriter_Video_Append) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_Append") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_Append == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_Append");
    }        

    m_PEAK_IPL_VideoWriter_Encoder_SetOption = (dyn_PEAK_IPL_VideoWriter_Encoder_SetOption) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Encoder_SetOption") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Encoder_SetOption == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Encoder_SetOption");
    }        

    m_PEAK_IPL_VideoWriter_Encoder_GetOption = (dyn_PEAK_IPL_VideoWriter_Encoder_GetOption) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Encoder_GetOption") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Encoder_GetOption == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Encoder_GetOption");
    }        

    m_PEAK_IPL_VideoWriter_Encoder_OptionRange = (dyn_PEAK_IPL_VideoWriter_Encoder_OptionRange) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Encoder_OptionRange") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Encoder_OptionRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Encoder_OptionRange");
    }        

    m_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer = (dyn_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer") : nullptr);
    if(m_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer");
    }        

    m_PEAK_IPL_VideoWriter_Container_SetOption = (dyn_PEAK_IPL_VideoWriter_Container_SetOption) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Container_SetOption") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Container_SetOption == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Container_SetOption");
    }        

    m_PEAK_IPL_VideoWriter_Container_GetOption = (dyn_PEAK_IPL_VideoWriter_Container_GetOption) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Container_GetOption") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Container_GetOption == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Container_GetOption");
    }        

    m_PEAK_IPL_VideoWriter_Container_OptionRange = (dyn_PEAK_IPL_VideoWriter_Container_OptionRange) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Container_OptionRange") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Container_OptionRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Container_OptionRange");
    }        

    m_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder = (dyn_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder") : nullptr);
    if(m_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder");
    }        

    m_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats = (dyn_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats");
    }        

    m_PEAK_IPL_VideoWriter_Video_GetEncodedFrames = (dyn_PEAK_IPL_VideoWriter_Video_GetEncodedFrames) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_GetEncodedFrames") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_GetEncodedFrames == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_GetEncodedFrames");
    }        

    m_PEAK_IPL_VideoWriter_Video_GetDroppedFrames = (dyn_PEAK_IPL_VideoWriter_Video_GetDroppedFrames) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_GetDroppedFrames") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_GetDroppedFrames == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_GetDroppedFrames");
    }        

    m_PEAK_IPL_VideoWriter_Video_GetFileSize = (dyn_PEAK_IPL_VideoWriter_Video_GetFileSize) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_GetFileSize") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_GetFileSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_GetFileSize");
    }        

    m_PEAK_IPL_VideoWriter_Video_ResetFrameCounters = (dyn_PEAK_IPL_VideoWriter_Video_ResetFrameCounters) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_ResetFrameCounters") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_ResetFrameCounters == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_ResetFrameCounters");
    }        

    m_PEAK_IPL_VideoWriter_Queue_SetSize = (dyn_PEAK_IPL_VideoWriter_Queue_SetSize) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Queue_SetSize") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Queue_SetSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Queue_SetSize");
    }        

    m_PEAK_IPL_VideoWriter_Queue_GetSize = (dyn_PEAK_IPL_VideoWriter_Queue_GetSize) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Queue_GetSize") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Queue_GetSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Queue_GetSize");
    }        

    m_PEAK_IPL_VideoWriter_Queue_GetRange = (dyn_PEAK_IPL_VideoWriter_Queue_GetRange) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Queue_GetRange") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Queue_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Queue_GetRange");
    }        

    m_PEAK_IPL_VideoWriter_Queue_GetQueued = (dyn_PEAK_IPL_VideoWriter_Queue_GetQueued) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Queue_GetQueued") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Queue_GetQueued == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Queue_GetQueued");
    }        

    m_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone = (dyn_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone");
    }        

    m_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty = (dyn_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty) (load ?  import_function(m_handle, "PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty") : nullptr);
    if(m_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty");
    }        

    m_PEAK_IPL_ImageSharpness_Construct = (dyn_PEAK_IPL_ImageSharpness_Construct) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_Construct") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_Construct");
    }        

    m_PEAK_IPL_ImageSharpness_Acquire = (dyn_PEAK_IPL_ImageSharpness_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_Acquire") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_Acquire");
    }        

    m_PEAK_IPL_ImageSharpness_Destruct = (dyn_PEAK_IPL_ImageSharpness_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_Destruct") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_Destruct");
    }        

    m_PEAK_IPL_ImageSharpness_Measure = (dyn_PEAK_IPL_ImageSharpness_Measure) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_Measure") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_Measure == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_Measure");
    }        

    m_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported = (dyn_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported");
    }        

    m_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList = (dyn_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList");
    }        

    m_PEAK_IPL_ImageSharpness_SetAlgorithm = (dyn_PEAK_IPL_ImageSharpness_SetAlgorithm) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_SetAlgorithm") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_SetAlgorithm == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_SetAlgorithm");
    }        

    m_PEAK_IPL_ImageSharpness_GetAlgorithm = (dyn_PEAK_IPL_ImageSharpness_GetAlgorithm) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_GetAlgorithm") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_GetAlgorithm == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_GetAlgorithm");
    }        

    m_PEAK_IPL_ImageSharpness_GetROIMinimumSize = (dyn_PEAK_IPL_ImageSharpness_GetROIMinimumSize) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_GetROIMinimumSize") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_GetROIMinimumSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_GetROIMinimumSize");
    }        

    m_PEAK_IPL_ImageSharpness_SetROIs = (dyn_PEAK_IPL_ImageSharpness_SetROIs) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_SetROIs") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_SetROIs == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_SetROIs");
    }        

    m_PEAK_IPL_ImageSharpness_GetROIs = (dyn_PEAK_IPL_ImageSharpness_GetROIs) (load ?  import_function(m_handle, "PEAK_IPL_ImageSharpness_GetROIs") : nullptr);
    if(m_PEAK_IPL_ImageSharpness_GetROIs == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_ImageSharpness_GetROIs");
    }        

    m_PEAK_IPL_EdgeEnhancement_Construct = (dyn_PEAK_IPL_EdgeEnhancement_Construct) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_Construct") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_Construct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_Construct");
    }        

    m_PEAK_IPL_EdgeEnhancement_Acquire = (dyn_PEAK_IPL_EdgeEnhancement_Acquire) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_Acquire") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_Acquire == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_Acquire");
    }        

    m_PEAK_IPL_EdgeEnhancement_Destruct = (dyn_PEAK_IPL_EdgeEnhancement_Destruct) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_Destruct") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_Destruct == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_Destruct");
    }        

    m_PEAK_IPL_EdgeEnhancement_ProcessInPlace = (dyn_PEAK_IPL_EdgeEnhancement_ProcessInPlace) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_ProcessInPlace") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_ProcessInPlace == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_ProcessInPlace");
    }        

    m_PEAK_IPL_EdgeEnhancement_Process = (dyn_PEAK_IPL_EdgeEnhancement_Process) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_Process") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_Process");
    }        

    m_PEAK_IPL_EdgeEnhancement_GetFactorRange = (dyn_PEAK_IPL_EdgeEnhancement_GetFactorRange) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_GetFactorRange") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_GetFactorRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_GetFactorRange");
    }        

    m_PEAK_IPL_EdgeEnhancement_SetFactor = (dyn_PEAK_IPL_EdgeEnhancement_SetFactor) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_SetFactor") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_SetFactor == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_SetFactor");
    }        

    m_PEAK_IPL_EdgeEnhancement_GetFactor = (dyn_PEAK_IPL_EdgeEnhancement_GetFactor) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_GetFactor") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_GetFactor == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_GetFactor");
    }        

    m_PEAK_IPL_EdgeEnhancement_GetDefaultFactor = (dyn_PEAK_IPL_EdgeEnhancement_GetDefaultFactor) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_GetDefaultFactor") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_GetDefaultFactor == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_GetDefaultFactor");
    }        

    m_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported = (dyn_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported) (load ?  import_function(m_handle, "PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported") : nullptr);
    if(m_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported");
    }        

            
            return true;
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetVersionMajor(uint32_t * libraryVersionMajor)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetVersionMajor)
    {
        return inst.m_PEAK_IPL_Library_GetVersionMajor(libraryVersionMajor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetVersionMinor(uint32_t * libraryVersionMinor)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetVersionMinor)
    {
        return inst.m_PEAK_IPL_Library_GetVersionMinor(libraryVersionMinor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetVersionSubminor(uint32_t * libraryVersionSubminor)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetVersionSubminor)
    {
        return inst.m_PEAK_IPL_Library_GetVersionSubminor(libraryVersionSubminor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetMaxConcurrentThreads(size_t * concurrentThreads)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetMaxConcurrentThreads)
    {
        return inst.m_PEAK_IPL_Library_GetMaxConcurrentThreads(concurrentThreads);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_SetMaxConcurrentThreads(size_t concurrentThreads)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_SetMaxConcurrentThreads)
    {
        return inst.m_PEAK_IPL_Library_SetMaxConcurrentThreads(concurrentThreads);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetMultiThreadingEnabled(PEAK_IPL_BOOL8 * enabled)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetMultiThreadingEnabled)
    {
        return inst.m_PEAK_IPL_Library_GetMultiThreadingEnabled(enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_SetMultiThreadingEnabled(PEAK_IPL_BOOL8 enabled)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_SetMultiThreadingEnabled)
    {
        return inst.m_PEAK_IPL_Library_SetMultiThreadingEnabled(enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetSIMDEnabled(PEAK_IPL_BOOL8 * enabled)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetSIMDEnabled)
    {
        return inst.m_PEAK_IPL_Library_GetSIMDEnabled(enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_SetSIMDEnabled(PEAK_IPL_BOOL8 enabled)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_SetSIMDEnabled)
    {
        return inst.m_PEAK_IPL_Library_SetSIMDEnabled(enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Library_GetLastError(PEAK_IPL_RETURN_CODE * lastErrorCode, char * lastErrorDescription, size_t * lastErrorDescriptionSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Library_GetLastError)
    {
        return inst.m_PEAK_IPL_Library_GetLastError(lastErrorCode, lastErrorDescription, lastErrorDescriptionSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_Construct(PEAK_IPL_IMAGE_CONVERTER_HANDLE * imageConverterHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_Construct)
    {
        return inst.m_PEAK_IPL_ImageConverter_Construct(imageConverterHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_Acquire(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_Acquire)
    {
        return inst.m_PEAK_IPL_ImageConverter_Acquire(imageConverterHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_PreAllocateConversion(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, size_t width, size_t height, size_t imageCount)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_PreAllocateConversion)
    {
        return inst.m_PEAK_IPL_ImageConverter_PreAllocateConversion(imageConverterHandle, inputPixelFormat, outputPixelFormat, width, height, imageCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_GetConversionMode(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE * conversionMode)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_GetConversionMode)
    {
        return inst.m_PEAK_IPL_ImageConverter_GetConversionMode(imageConverterHandle, conversionMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_SetConversionMode(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_CONVERSION_MODE conversionMode)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_SetConversionMode)
    {
        return inst.m_PEAK_IPL_ImageConverter_SetConversionMode(imageConverterHandle, conversionMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_PIXEL_FORMAT inputPixelFormat, PEAK_IPL_PIXEL_FORMAT * outputPixelFormats, size_t * outputPixelFormatsSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats)
    {
        return inst.m_PEAK_IPL_ImageConverter_GetSupportedOutputPixelFormats(imageConverterHandle, inputPixelFormat, outputPixelFormats, outputPixelFormatsSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_Convert(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_Convert)
    {
        return inst.m_PEAK_IPL_ImageConverter_Convert(imageConverterHandle, inputImageHandle, outputPixelFormat, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_ConvertToBuffer(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t * outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_ConvertToBuffer)
    {
        return inst.m_PEAK_IPL_ImageConverter_ConvertToBuffer(imageConverterHandle, inputImageHandle, outputPixelFormat, outputImageBuffer, outputImageBufferSize, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageConverter_Destruct(PEAK_IPL_IMAGE_CONVERTER_HANDLE imageConverterHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageConverter_Destruct)
    {
        return inst.m_PEAK_IPL_ImageConverter_Destruct(imageConverterHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_Construct(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE * adaptiveHotpixelCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Construct)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Construct(adaptiveHotpixelCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_Acquire(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Acquire(adaptiveHotpixelCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY sensitivityLevel)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_SetSensitivity(adaptiveHotpixelCorrectorHandle, sensitivityLevel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_HOTPIXELCORRECTION_SENSITIVITY * sensitivityLevel)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_GetSensitivity(adaptiveHotpixelCorrectorHandle, sensitivityLevel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t gainFactorPercent)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_SetGainFactorPercent(adaptiveHotpixelCorrectorHandle, gainFactorPercent);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, uint32_t * gainFactorPercent)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_GetGainFactorPercent(adaptiveHotpixelCorrectorHandle, gainFactorPercent);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_Detect(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Detect)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Detect(adaptiveHotpixelCorrectorHandle, inputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, struct PEAK_IPL_POINT_2D * hotpixels, size_t * hotpixelsSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_GetHotpixels(adaptiveHotpixelCorrectorHandle, hotpixels, hotpixelsSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_Correct(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, const struct PEAK_IPL_POINT_2D * hotpixels, size_t hotpixelsSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Correct)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Correct(adaptiveHotpixelCorrectorHandle, inputImageHandle, hotpixels, hotpixelsSize, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, const struct PEAK_IPL_POINT_2D * hotpixels, size_t hotpixelsSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectInPlace(adaptiveHotpixelCorrectorHandle, inputImageHandle, hotpixels, hotpixelsSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptive(adaptiveHotpixelCorrectorHandle, inputImageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle, PEAK_IPL_IMAGE_HANDLE inputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_CorrectAdaptiveInPlace(adaptiveHotpixelCorrectorHandle, inputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(PEAK_IPL_ADAPTIVE_HOTPIXEL_CORRECTOR_HANDLE adaptiveHotpixelCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct)
    {
        return inst.m_PEAK_IPL_AdaptiveHotpixelCorrector_Destruct(adaptiveHotpixelCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_Construct(PEAK_IPL_COLOR_CORRECTOR_HANDLE * colorCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_Construct)
    {
        return inst.m_PEAK_IPL_ColorCorrector_Construct(colorCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_Acquire(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_Acquire)
    {
        return inst.m_PEAK_IPL_ColorCorrector_Acquire(colorCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_SetColorCorrectionFactors(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * colorCorrectorFactors)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors)
    {
        return inst.m_PEAK_IPL_ColorCorrector_SetColorCorrectionFactors(colorCorrectorHandle, colorCorrectorFactors);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_GetColorCorrectionFactors(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * colorCorrectorFactors, size_t * colorCorrectorFactorsSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors)
    {
        return inst.m_PEAK_IPL_ColorCorrector_GetColorCorrectionFactors(colorCorrectorHandle, colorCorrectorFactors, colorCorrectorFactorsSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported)
    {
        return inst.m_PEAK_IPL_ColorCorrector_GetIsPixelFormatSupported(colorCorrectorHandle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_Process(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_Process)
    {
        return inst.m_PEAK_IPL_ColorCorrector_Process(colorCorrectorHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_ProcessInPlace(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_ProcessInPlace)
    {
        return inst.m_PEAK_IPL_ColorCorrector_ProcessInPlace(colorCorrectorHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_Destruct(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_Destruct)
    {
        return inst.m_PEAK_IPL_ColorCorrector_Destruct(colorCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_GetSaturation(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturation)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_GetSaturation)
    {
        return inst.m_PEAK_IPL_ColorCorrector_GetSaturation(colorCorrectorHandle, saturation);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_GetSaturationMin(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturationMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_GetSaturationMin)
    {
        return inst.m_PEAK_IPL_ColorCorrector_GetSaturationMin(colorCorrectorHandle, saturationMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_GetSaturationMax(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float * saturationMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_GetSaturationMax)
    {
        return inst.m_PEAK_IPL_ColorCorrector_GetSaturationMax(colorCorrectorHandle, saturationMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ColorCorrector_SetSaturation(PEAK_IPL_COLOR_CORRECTOR_HANDLE colorCorrectorHandle, float saturation)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ColorCorrector_SetSaturation)
    {
        return inst.m_PEAK_IPL_ColorCorrector_SetSaturation(colorCorrectorHandle, saturation);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_Construct(PEAK_IPL_BINNING_HANDLE * binningHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_Construct)
    {
        return inst.m_PEAK_IPL_Binning_Construct(binningHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_Acquire(PEAK_IPL_BINNING_HANDLE binningHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_Acquire)
    {
        return inst.m_PEAK_IPL_Binning_Acquire(binningHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_SetBinningX(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningXValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_SetBinningX)
    {
        return inst.m_PEAK_IPL_Binning_SetBinningX(binningHandle, binningXValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_GetBinningX(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningXValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_GetBinningX)
    {
        return inst.m_PEAK_IPL_Binning_GetBinningX(binningHandle, binningXValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_SetBinningY(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningYValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_SetBinningY)
    {
        return inst.m_PEAK_IPL_Binning_SetBinningY(binningHandle, binningYValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_GetBinningY(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningYValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_GetBinningY)
    {
        return inst.m_PEAK_IPL_Binning_GetBinningY(binningHandle, binningYValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_GetBinningMin(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_GetBinningMin)
    {
        return inst.m_PEAK_IPL_Binning_GetBinningMin(binningHandle, binningMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_GetBinningMax(PEAK_IPL_BINNING_HANDLE binningHandle, uint8_t * binningMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_GetBinningMax)
    {
        return inst.m_PEAK_IPL_Binning_GetBinningMax(binningHandle, binningMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_ExpectedOutputSize(PEAK_IPL_BINNING_HANDLE binningHandle, size_t * outputWidth, size_t * outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_ExpectedOutputSize)
    {
        return inst.m_PEAK_IPL_Binning_ExpectedOutputSize(binningHandle, outputWidth, outputHeight, inputWidth, inputHeight, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_IsPixelformatSupported(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_IsPixelformatSupported)
    {
        return inst.m_PEAK_IPL_Binning_IsPixelformatSupported(binningHandle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_SetBinningMode(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE * mode)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_SetBinningMode)
    {
        return inst.m_PEAK_IPL_Binning_SetBinningMode(binningHandle, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_GetBinningMode(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_BINNINGMODE * mode)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_GetBinningMode)
    {
        return inst.m_PEAK_IPL_Binning_GetBinningMode(binningHandle, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_Process(PEAK_IPL_BINNING_HANDLE binningHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_Process)
    {
        return inst.m_PEAK_IPL_Binning_Process(binningHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Binning_Destruct(PEAK_IPL_BINNING_HANDLE binningHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Binning_Destruct)
    {
        return inst.m_PEAK_IPL_Binning_Destruct(binningHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_Construct(PEAK_IPL_DECIMATION_HANDLE * decimationHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_Construct)
    {
        return inst.m_PEAK_IPL_Decimation_Construct(decimationHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_Acquire(PEAK_IPL_DECIMATION_HANDLE decimationHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_Acquire)
    {
        return inst.m_PEAK_IPL_Decimation_Acquire(decimationHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_SetDecimationX(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationXValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_SetDecimationX)
    {
        return inst.m_PEAK_IPL_Decimation_SetDecimationX(decimationHandle, decimationXValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_GetDecimationX(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationXValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_GetDecimationX)
    {
        return inst.m_PEAK_IPL_Decimation_GetDecimationX(decimationHandle, decimationXValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_SetDecimationY(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationYValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_SetDecimationY)
    {
        return inst.m_PEAK_IPL_Decimation_SetDecimationY(decimationHandle, decimationYValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_GetDecimationY(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationYValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_GetDecimationY)
    {
        return inst.m_PEAK_IPL_Decimation_GetDecimationY(decimationHandle, decimationYValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_GetDecimationMin(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_GetDecimationMin)
    {
        return inst.m_PEAK_IPL_Decimation_GetDecimationMin(decimationHandle, decimationMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_GetDecimationMax(PEAK_IPL_DECIMATION_HANDLE decimationHandle, uint8_t * decimationMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_GetDecimationMax)
    {
        return inst.m_PEAK_IPL_Decimation_GetDecimationMax(decimationHandle, decimationMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_ExpectedOutputSize(PEAK_IPL_DECIMATION_HANDLE decimationHandle, size_t * outputWidth, size_t * outputHeight, size_t inputWidth, size_t inputHeight, PEAK_IPL_PIXEL_FORMAT pixelFormat)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_ExpectedOutputSize)
    {
        return inst.m_PEAK_IPL_Decimation_ExpectedOutputSize(decimationHandle, outputWidth, outputHeight, inputWidth, inputHeight, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_IsPixelformatSupported(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_IsPixelformatSupported)
    {
        return inst.m_PEAK_IPL_Decimation_IsPixelformatSupported(decimationHandle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_Process(PEAK_IPL_DECIMATION_HANDLE decimationHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_Process)
    {
        return inst.m_PEAK_IPL_Decimation_Process(decimationHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Decimation_Destruct(PEAK_IPL_DECIMATION_HANDLE decimationHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Decimation_Destruct)
    {
        return inst.m_PEAK_IPL_Decimation_Destruct(decimationHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_Construct(PEAK_IPL_GAIN_HANDLE * gainHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_Construct)
    {
        return inst.m_PEAK_IPL_Gain_Construct(gainHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_Acquire(PEAK_IPL_GAIN_HANDLE gainHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_Acquire)
    {
        return inst.m_PEAK_IPL_Gain_Acquire(gainHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_SetMasterGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_SetMasterGainValue)
    {
        return inst.m_PEAK_IPL_Gain_SetMasterGainValue(gainHandle, masterGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetMasterGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetMasterGainValue)
    {
        return inst.m_PEAK_IPL_Gain_GetMasterGainValue(gainHandle, masterGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetMasterGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetMasterGainMax)
    {
        return inst.m_PEAK_IPL_Gain_GetMasterGainMax(gainHandle, masterGainMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetMasterGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * masterGainMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetMasterGainMin)
    {
        return inst.m_PEAK_IPL_Gain_GetMasterGainMin(gainHandle, masterGainMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_SetRedGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_SetRedGainValue)
    {
        return inst.m_PEAK_IPL_Gain_SetRedGainValue(gainHandle, redGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetRedGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetRedGainValue)
    {
        return inst.m_PEAK_IPL_Gain_GetRedGainValue(gainHandle, redGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetRedGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetRedGainMax)
    {
        return inst.m_PEAK_IPL_Gain_GetRedGainMax(gainHandle, redGainMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetRedGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * redGainMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetRedGainMin)
    {
        return inst.m_PEAK_IPL_Gain_GetRedGainMin(gainHandle, redGainMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_SetGreenGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_SetGreenGainValue)
    {
        return inst.m_PEAK_IPL_Gain_SetGreenGainValue(gainHandle, greenGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetGreenGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetGreenGainValue)
    {
        return inst.m_PEAK_IPL_Gain_GetGreenGainValue(gainHandle, greenGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetGreenGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetGreenGainMax)
    {
        return inst.m_PEAK_IPL_Gain_GetGreenGainMax(gainHandle, greenGainMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetGreenGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * greenGainMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetGreenGainMin)
    {
        return inst.m_PEAK_IPL_Gain_GetGreenGainMin(gainHandle, greenGainMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_SetBlueGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_SetBlueGainValue)
    {
        return inst.m_PEAK_IPL_Gain_SetBlueGainValue(gainHandle, blueGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetBlueGainValue(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetBlueGainValue)
    {
        return inst.m_PEAK_IPL_Gain_GetBlueGainValue(gainHandle, blueGainValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetBlueGainMax(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetBlueGainMax)
    {
        return inst.m_PEAK_IPL_Gain_GetBlueGainMax(gainHandle, blueGainMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetBlueGainMin(PEAK_IPL_GAIN_HANDLE gainHandle, float * blueGainMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetBlueGainMin)
    {
        return inst.m_PEAK_IPL_Gain_GetBlueGainMin(gainHandle, blueGainMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_GetIsPixelFormatSupported(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_GetIsPixelFormatSupported)
    {
        return inst.m_PEAK_IPL_Gain_GetIsPixelFormatSupported(gainHandle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_Process(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_Process)
    {
        return inst.m_PEAK_IPL_Gain_Process(gainHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_ProcessInPlace(PEAK_IPL_GAIN_HANDLE gainHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_ProcessInPlace)
    {
        return inst.m_PEAK_IPL_Gain_ProcessInPlace(gainHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Gain_Destruct(PEAK_IPL_GAIN_HANDLE gainHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Gain_Destruct)
    {
        return inst.m_PEAK_IPL_Gain_Destruct(gainHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_Construct(PEAK_IPL_GAMMA_CORRECTOR_HANDLE * gammaCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_Construct)
    {
        return inst.m_PEAK_IPL_GammaCorrector_Construct(gammaCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_Acquire(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_Acquire)
    {
        return inst.m_PEAK_IPL_GammaCorrector_Acquire(gammaCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_SetGammaCorrectionValue(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue)
    {
        return inst.m_PEAK_IPL_GammaCorrector_SetGammaCorrectionValue(gammaCorrectorHandle, gammaValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_GetGammaCorrectionValue(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue)
    {
        return inst.m_PEAK_IPL_GammaCorrector_GetGammaCorrectionValue(gammaCorrectorHandle, gammaValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_GetGammaCorrectionMax(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaMax)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax)
    {
        return inst.m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMax(gammaCorrectorHandle, gammaMax);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_GetGammaCorrectionMin(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, float * gammaMin)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin)
    {
        return inst.m_PEAK_IPL_GammaCorrector_GetGammaCorrectionMin(gammaCorrectorHandle, gammaMin);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported)
    {
        return inst.m_PEAK_IPL_GammaCorrector_GetIsPixelFormatSupported(gammaCorrectorHandle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_Process(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_Process)
    {
        return inst.m_PEAK_IPL_GammaCorrector_Process(gammaCorrectorHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_ProcessInPlace(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_ProcessInPlace)
    {
        return inst.m_PEAK_IPL_GammaCorrector_ProcessInPlace(gammaCorrectorHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_GammaCorrector_Destruct(PEAK_IPL_GAMMA_CORRECTOR_HANDLE gammaCorrectorHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_GammaCorrector_Destruct)
    {
        return inst.m_PEAK_IPL_GammaCorrector_Destruct(gammaCorrectorHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_Construct(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE * imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_Construct)
    {
        return inst.m_PEAK_IPL_Image_Construct(pixelFormat, width, height, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_Acquire(PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_Acquire)
    {
        return inst.m_PEAK_IPL_Image_Acquire(imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_ConstructFromBuffer(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint8_t * buffer, uint64_t bufferSize, size_t width, size_t height, PEAK_IPL_IMAGE_HANDLE * imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_ConstructFromBuffer)
    {
        return inst.m_PEAK_IPL_Image_ConstructFromBuffer(pixelFormat, buffer, bufferSize, width, height, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetWidth(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * width)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetWidth)
    {
        return inst.m_PEAK_IPL_Image_GetWidth(imageHandle, width);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetHeight(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * height)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetHeight)
    {
        return inst.m_PEAK_IPL_Image_GetHeight(imageHandle, height);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetPixelPointer(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t xPos, size_t yPos, uint8_t * * pixelPointer)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetPixelPointer)
    {
        return inst.m_PEAK_IPL_Image_GetPixelPointer(imageHandle, xPos, yPos, pixelPointer);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetByteCount(PEAK_IPL_IMAGE_HANDLE imageHandle, size_t * byteCount)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetByteCount)
    {
        return inst.m_PEAK_IPL_Image_GetByteCount(imageHandle, byteCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetPixelFormat(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetPixelFormat)
    {
        return inst.m_PEAK_IPL_Image_GetPixelFormat(imageHandle, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetData(PEAK_IPL_IMAGE_HANDLE imageHandle, uint8_t * * data)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetData)
    {
        return inst.m_PEAK_IPL_Image_GetData(imageHandle, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_Clone(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_Clone)
    {
        return inst.m_PEAK_IPL_Image_Clone(imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_ConvertTo(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_ConvertTo)
    {
        return inst.m_PEAK_IPL_Image_ConvertTo(imageHandle, outputPixelFormat, conversionMode, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_Scale(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D * size, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_Scale)
    {
        return inst.m_PEAK_IPL_Image_Scale(imageHandle, size, algorithm, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_ScaleInto(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_SIZE_2D * size, uint8_t * buffer, size_t bufferSize, PEAK_IPL_INTERPOLATION_ALGORITHM algorithm)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_ScaleInto)
    {
        return inst.m_PEAK_IPL_Image_ScaleInto(imageHandle, size, buffer, bufferSize, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_Crop(PEAK_IPL_IMAGE_HANDLE imageHandle, const struct PEAK_IPL_POINT_2D * pos, const struct PEAK_IPL_SIZE_2D * size, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_Crop)
    {
        return inst.m_PEAK_IPL_Image_Crop(imageHandle, pos, size, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_ConvertToBuffer(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_PIXEL_FORMAT outputPixelFormat, uint8_t * outputImageBuffer, size_t outputImageBufferSize, PEAK_IPL_CONVERSION_MODE conversionMode, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_ConvertToBuffer)
    {
        return inst.m_PEAK_IPL_Image_ConvertToBuffer(imageHandle, outputPixelFormat, outputImageBuffer, outputImageBufferSize, conversionMode, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_Destruct(PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_Destruct)
    {
        return inst.m_PEAK_IPL_Image_Destruct(imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_Construct(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE * imageTransformerHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_Construct)
    {
        return inst.m_PEAK_IPL_ImageTransformer_Construct(imageTransformerHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_Acquire(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_Acquire)
    {
        return inst.m_PEAK_IPL_ImageTransformer_Acquire(imageTransformerHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_SetTimestamp(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t timestamp_ns)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_SetTimestamp)
    {
        return inst.m_PEAK_IPL_Image_SetTimestamp(imageHandle, timestamp_ns);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Image_GetTimestamp(PEAK_IPL_IMAGE_HANDLE imageHandle, uint64_t * timestamp_ns)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Image_GetTimestamp)
    {
        return inst.m_PEAK_IPL_Image_GetTimestamp(imageHandle, timestamp_ns);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_MirrorUpDown(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_MirrorUpDown)
    {
        return inst.m_PEAK_IPL_ImageTransformer_MirrorUpDown(imageTransformerHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_MirrorLeftRight(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_MirrorLeftRight)
    {
        return inst.m_PEAK_IPL_ImageTransformer_MirrorLeftRight(imageTransformerHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight)
    {
        return inst.m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRight(imageTransformerHandle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_MirrorUpDownInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace)
    {
        return inst.m_PEAK_IPL_ImageTransformer_MirrorUpDownInPlace(imageTransformerHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace)
    {
        return inst.m_PEAK_IPL_ImageTransformer_MirrorLeftRightInPlace(imageTransformerHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace)
    {
        return inst.m_PEAK_IPL_ImageTransformer_MirrorUpDownLeftRightInPlace(imageTransformerHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_Rotate(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_Rotate)
    {
        return inst.m_PEAK_IPL_ImageTransformer_Rotate(imageTransformerHandle, imageHandle, outputImageHandle, rotationAngle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_RotateInPlace(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle, PEAK_IPL_IMAGE_HANDLE imageHandle, enum PEAK_IPL_IMAGE_TRANSFORMER_ROTATION_ANGLE_t rotationAngle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_RotateInPlace)
    {
        return inst.m_PEAK_IPL_ImageTransformer_RotateInPlace(imageTransformerHandle, imageHandle, rotationAngle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageTransformer_Destruct(PEAK_IPL_IMAGE_TRANSFORMER_HANDLE imageTransformerHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageTransformer_Destruct)
    {
        return inst.m_PEAK_IPL_ImageTransformer_Destruct(imageTransformerHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_Construct(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_HISTOGRAM_HANDLE * histogramHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_Construct)
    {
        return inst.m_PEAK_IPL_Histogram_Construct(imageHandle, histogramHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_Acquire(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_Acquire)
    {
        return inst.m_PEAK_IPL_Histogram_Acquire(histogramHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_GetPixelFormat(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_GetPixelFormat)
    {
        return inst.m_PEAK_IPL_Histogram_GetPixelFormat(histogramHandle, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_GetNumChannels(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t * numChannels)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_GetNumChannels)
    {
        return inst.m_PEAK_IPL_Histogram_GetNumChannels(histogramHandle, numChannels);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_GetPixelSumForChannel(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * pixelSum)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_GetPixelSumForChannel)
    {
        return inst.m_PEAK_IPL_Histogram_GetPixelSumForChannel(histogramHandle, channelIndex, pixelSum);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_GetPixelCountForChannel(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * pixelCount)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_GetPixelCountForChannel)
    {
        return inst.m_PEAK_IPL_Histogram_GetPixelCountForChannel(histogramHandle, channelIndex, pixelCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_GetBinsForChannel(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle, size_t channelIndex, uint64_t * bins, size_t * binsSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_GetBinsForChannel)
    {
        return inst.m_PEAK_IPL_Histogram_GetBinsForChannel(histogramHandle, channelIndex, bins, binsSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_Histogram_Destruct(PEAK_IPL_HISTOGRAM_HANDLE histogramHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_Histogram_Destruct)
    {
        return inst.m_PEAK_IPL_Histogram_Destruct(histogramHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_Construct(PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_ORIENTATION orientation, size_t offset, PEAK_IPL_PIXEL_LINE_HANDLE * pixelLineHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_Construct)
    {
        return inst.m_PEAK_IPL_PixelLine_Construct(imageHandle, orientation, offset, pixelLineHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_Acquire(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_Acquire)
    {
        return inst.m_PEAK_IPL_PixelLine_Acquire(pixelLineHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_GetPixelFormat(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_PIXEL_FORMAT * pixelFormat)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_GetPixelFormat)
    {
        return inst.m_PEAK_IPL_PixelLine_GetPixelFormat(pixelLineHandle, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_GetOrientation(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, PEAK_IPL_ORIENTATION * orientation)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_GetOrientation)
    {
        return inst.m_PEAK_IPL_PixelLine_GetOrientation(pixelLineHandle, orientation);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_GetOffset(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t * offset)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_GetOffset)
    {
        return inst.m_PEAK_IPL_PixelLine_GetOffset(pixelLineHandle, offset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_GetNumChannels(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t * numChannels)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_GetNumChannels)
    {
        return inst.m_PEAK_IPL_PixelLine_GetNumChannels(pixelLineHandle, numChannels);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_GetValuesForChannel(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle, size_t channelIndex, uint32_t * values, size_t * valuesSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_GetValuesForChannel)
    {
        return inst.m_PEAK_IPL_PixelLine_GetValuesForChannel(pixelLineHandle, channelIndex, values, valuesSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelLine_Destruct(PEAK_IPL_PIXEL_LINE_HANDLE pixelLineHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelLine_Destruct)
    {
        return inst.m_PEAK_IPL_PixelLine_Destruct(pixelLineHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetNumChannels(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numChannels)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetNumChannels)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetNumChannels(pixelFormat, numChannels);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numSignificantBits)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerChannel(pixelFormat, numSignificantBits);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numStorageBits)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerChannel(pixelFormat, numStorageBits);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetMaximumValuePerChannel(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint32_t * channelMaximumValue)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetMaximumValuePerChannel(pixelFormat, channelMaximumValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numSignificantBits)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetNumSignificantBitsPerPixel(pixelFormat, numSignificantBits);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel(PEAK_IPL_PIXEL_FORMAT pixelFormat, size_t * numStorageBits)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetNumStorageBitsPerPixel(pixelFormat, numStorageBits);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetEndianness(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_ENDIANNESS * endianness)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetEndianness)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetEndianness(pixelFormat, endianness);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels(PEAK_IPL_PIXEL_FORMAT pixelFormat, uint64_t numPixels, uint64_t * size)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels)
    {
        return inst.m_PEAK_IPL_PixelFormat_CalculateStorageSizeOfPixels(pixelFormat, numPixels, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_IsPacked(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPacked)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_IsPacked)
    {
        return inst.m_PEAK_IPL_PixelFormat_IsPacked(pixelFormat, isPacked);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_IsBayered(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isBayered)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_IsBayered)
    {
        return inst.m_PEAK_IPL_PixelFormat_IsBayered(pixelFormat, isBayered);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_IsYUV(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isYUV)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_IsYUV)
    {
        return inst.m_PEAK_IPL_PixelFormat_IsYUV(pixelFormat, isYUV);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_PixelFormat_GetBayerPattern(PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BAYER_PATTERN * pattern)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_PixelFormat_GetBayerPattern)
    {
        return inst.m_PEAK_IPL_PixelFormat_GetBayerPattern(pixelFormat, pattern);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageWriter_WriteAsRAW(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageWriter_WriteAsRAW)
    {
        return inst.m_PEAK_IPL_ImageWriter_WriteAsRAW(inputImageHandle, filePath, filePathSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageWriter_WriteAsBMP(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageWriter_WriteAsBMP)
    {
        return inst.m_PEAK_IPL_ImageWriter_WriteAsBMP(inputImageHandle, filePath, filePathSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageWriter_WriteAsPNG(PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char * filePath, size_t filePathSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageWriter_WriteAsPNG)
    {
        return inst.m_PEAK_IPL_ImageWriter_WriteAsPNG(inputImageHandle, quality, filePath, filePathSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageWriter_WriteAsJPG(PEAK_IPL_IMAGE_HANDLE inputImageHandle, uint32_t quality, const char * filePath, size_t filePathSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageWriter_WriteAsJPG)
    {
        return inst.m_PEAK_IPL_ImageWriter_WriteAsJPG(inputImageHandle, quality, filePath, filePathSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageWriter_Write(PEAK_IPL_IMAGE_HANDLE inputImageHandle, const char * filePath, size_t filePathSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageWriter_Write)
    {
        return inst.m_PEAK_IPL_ImageWriter_Write(inputImageHandle, filePath, filePathSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageReader_Read(const char * filePath, size_t filePathSize, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageReader_Read)
    {
        return inst.m_PEAK_IPL_ImageReader_Read(filePath, filePathSize, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageReaderRead_ReadAsPixelFormat(const char * filePath, size_t filePathSize, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat)
    {
        return inst.m_PEAK_IPL_ImageReaderRead_ReadAsPixelFormat(filePath, filePathSize, pixelFormat, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_Open(PEAK_IPL_VIDEO_HANDLE * videoHandle, const char * filePath, PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER encoder)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_Open)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_Open(videoHandle, filePath, container, encoder);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_IsOpen(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_BOOL8 * isOpen)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_IsOpen)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_IsOpen(videoHandle, isOpen);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_Close(PEAK_IPL_VIDEO_HANDLE videoHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_Close)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_Close(videoHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_Append(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_Append)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_Append(videoHandle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Encoder_SetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, const void * value, size_t count)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Encoder_SetOption)
    {
        return inst.m_PEAK_IPL_VideoWriter_Encoder_SetOption(videoHandle, option, value, count);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Encoder_GetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * value, size_t count, size_t * outCount)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Encoder_GetOption)
    {
        return inst.m_PEAK_IPL_VideoWriter_Encoder_GetOption(videoHandle, option, value, count, outCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Encoder_OptionRange(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * min, void * max, void * inc)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Encoder_OptionRange)
    {
        return inst.m_PEAK_IPL_VideoWriter_Encoder_OptionRange(videoHandle, option, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer(PEAK_IPL_VIDEO_CONTAINER container, PEAK_IPL_VIDEO_ENCODER * encoders, size_t * size)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer)
    {
        return inst.m_PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer(container, encoders, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Container_SetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_CONTAINER_OPTION option, const void * value, size_t count)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Container_SetOption)
    {
        return inst.m_PEAK_IPL_VideoWriter_Container_SetOption(videoHandle, option, value, count);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Container_GetOption(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * value, size_t count, size_t * outCount)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Container_GetOption)
    {
        return inst.m_PEAK_IPL_VideoWriter_Container_GetOption(videoHandle, option, value, count, outCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Container_OptionRange(PEAK_IPL_VIDEO_HANDLE videoHandle, PEAK_IPL_VIDEO_ENCODER_OPTION option, void * min, void * max, void * inc)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Container_OptionRange)
    {
        return inst.m_PEAK_IPL_VideoWriter_Container_OptionRange(videoHandle, option, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder(PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_VIDEO_CONTAINER * containers, size_t * size)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder)
    {
        return inst.m_PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder(encoder, containers, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats(PEAK_IPL_VIDEO_ENCODER encoder, PEAK_IPL_PIXEL_FORMAT * pixelFormatList, size_t * pixelFormatListSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats)
    {
        return inst.m_PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats(encoder, pixelFormatList, pixelFormatListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_GetEncodedFrames(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * numFrames)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_GetEncodedFrames)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_GetEncodedFrames(videoHandle, numFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_GetDroppedFrames(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * numFrames)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_GetDroppedFrames)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_GetDroppedFrames(videoHandle, numFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_GetFileSize(PEAK_IPL_VIDEO_HANDLE videoHandle, uint64_t * fileSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_GetFileSize)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_GetFileSize(videoHandle, fileSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_ResetFrameCounters(PEAK_IPL_VIDEO_HANDLE videoHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_ResetFrameCounters)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_ResetFrameCounters(videoHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Queue_SetSize(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t size)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Queue_SetSize)
    {
        return inst.m_PEAK_IPL_VideoWriter_Queue_SetSize(videoHandle, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Queue_GetSize(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * size)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Queue_GetSize)
    {
        return inst.m_PEAK_IPL_VideoWriter_Queue_GetSize(videoHandle, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Queue_GetRange(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * minSize, int32_t * maxSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Queue_GetRange)
    {
        return inst.m_PEAK_IPL_VideoWriter_Queue_GetRange(videoHandle, minSize, maxSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Queue_GetQueued(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t * queued)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Queue_GetQueued)
    {
        return inst.m_PEAK_IPL_VideoWriter_Queue_GetQueued(videoHandle, queued);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone(videoHandle, timeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty(PEAK_IPL_VIDEO_HANDLE videoHandle, int32_t timeout_ms)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty)
    {
        return inst.m_PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty(videoHandle, timeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_Construct(PEAK_IPL_SHARPNESS_HANDLE * handle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_Construct)
    {
        return inst.m_PEAK_IPL_ImageSharpness_Construct(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_Acquire(PEAK_IPL_SHARPNESS_HANDLE handle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_Acquire)
    {
        return inst.m_PEAK_IPL_ImageSharpness_Acquire(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_Destruct(PEAK_IPL_SHARPNESS_HANDLE handle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_Destruct)
    {
        return inst.m_PEAK_IPL_ImageSharpness_Destruct(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_Measure(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, double * calculated_value)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_Measure)
    {
        return inst.m_PEAK_IPL_ImageSharpness_Measure(handle, imageHandle, calculated_value);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported)
    {
        return inst.m_PEAK_IPL_ImageSharpness_GetIsPixelFormatSupported(handle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_PIXEL_FORMAT * pixelFormatList, size_t * pixelFormatListSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList)
    {
        return inst.m_PEAK_IPL_ImageSharpness_GetSupportedPixelFormatList(handle, pixelFormatList, pixelFormatListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_SetAlgorithm(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM algorithm)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_SetAlgorithm)
    {
        return inst.m_PEAK_IPL_ImageSharpness_SetAlgorithm(handle, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_GetAlgorithm(PEAK_IPL_SHARPNESS_HANDLE handle, PEAK_IPL_SHARPNESS_ALGORITHM * algorithm)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_GetAlgorithm)
    {
        return inst.m_PEAK_IPL_ImageSharpness_GetAlgorithm(handle, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_GetROIMinimumSize(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SIZE_2D * min_size)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_GetROIMinimumSize)
    {
        return inst.m_PEAK_IPL_ImageSharpness_GetROIMinimumSize(handle, min_size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_SetROIs(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI * roiList, size_t roiListSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_SetROIs)
    {
        return inst.m_PEAK_IPL_ImageSharpness_SetROIs(handle, roiList, roiListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_ImageSharpness_GetROIs(PEAK_IPL_SHARPNESS_HANDLE handle, struct PEAK_IPL_SHARPNESS_ROI * roiList, size_t * roiListSize)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_ImageSharpness_GetROIs)
    {
        return inst.m_PEAK_IPL_ImageSharpness_GetROIs(handle, roiList, roiListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_Construct(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE * handle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_Construct)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_Construct(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_Acquire(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_Acquire)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_Acquire(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_Destruct(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_Destruct)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_Destruct(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_ProcessInPlace(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_ProcessInPlace)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_ProcessInPlace(handle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_Process(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_IMAGE_HANDLE imageHandle, PEAK_IPL_IMAGE_HANDLE * outputImageHandle)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_Process)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_Process(handle, imageHandle, outputImageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_GetFactorRange(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, struct PEAK_IPL_RANGE * range)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_GetFactorRange)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_GetFactorRange(handle, range);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_SetFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t factor)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_SetFactor)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_SetFactor(handle, factor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_GetFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t * factor)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_GetFactor)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_GetFactor(handle, factor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_GetDefaultFactor(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, uint32_t * factor)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_GetDefaultFactor)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_GetDefaultFactor(handle, factor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_IPL_C_API DynamicLoader::PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported(PEAK_IPL_EDGE_ENHANCEMENT_HANDLE handle, PEAK_IPL_PIXEL_FORMAT pixelFormat, PEAK_IPL_BOOL8 * isPixelFormatSupported)
{
    auto& inst = instance();
    if(inst.m_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported)
    {
        return inst.m_PEAK_IPL_EdgeEnhancement_IsPixelFormatSupported(handle, pixelFormat, isPixelFormatSupported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

} /* namespace dynamic */
} /* namespace ipl */
} /* namespace peak */

