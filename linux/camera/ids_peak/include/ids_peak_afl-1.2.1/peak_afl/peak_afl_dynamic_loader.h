/*!
 * \file    peak_afl_dynamic_loader.h
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */
#pragma once

#include "peak_afl.h"
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

#undef PEAK_AFL_EXPORT
#define PEAK_AFL_EXPORT
#undef PEAK_AFL_CALLCONV
#define PEAK_AFL_CALLCONV


namespace peak
{
namespace afl
{
namespace dynamic
{

typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_Init)(void );
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_Exit)(void );
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_GetLastError)(peak_afl_status * lastErrorCode, char * lastErrorMessage, size_t * lastErrorMessageSize);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_GetVersion)(uint32_t * majorVersion, uint32_t * minorVersion, uint32_t * subminorVersion, uint32_t * patchVersion);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_Create)(peak_afl_manager_handle * handle, PEAK_NODE_MAP_HANDLE nodeMapHandle);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_Destroy)(peak_afl_manager_handle handle);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_AddController)(peak_afl_manager_handle handle, peak_afl_manager_handle controller);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_RemoveController)(peak_afl_manager_handle handle, peak_afl_manager_handle controller);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_Process)(peak_afl_manager_handle handle, PEAK_IPL_IMAGE_HANDLE imageHandle);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_CreateController)(peak_afl_manager_handle handle, peak_afl_controller_handle * controller, peak_afl_controllerType controllerType);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_DestroyAllController)(peak_afl_manager_handle handle);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_DestroyController)(peak_afl_manager_handle handle, peak_afl_controller_handle controller);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_SetGainIPL)(peak_afl_manager_handle handle, PEAK_IPL_GAIN_HANDLE gainHandle);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoFeatureManager_Status)(peak_afl_manager_handle handle, peak_afl_BOOL8 * running);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Create)(peak_afl_controller_handle * controller, peak_afl_controllerType controllerType);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Destroy)(peak_afl_controller_handle controller);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SkipFrames_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SkipFrames_Set)(peak_afl_controller_handle controller, uint32_t count);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SkipFrames_Get)(peak_afl_controller_handle controller, uint32_t * count);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SkipFrames_GetRange)(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_ROI_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_ROI_Set)(peak_afl_controller_handle controller, peak_afl_rectangle roi);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_ROI_Get)(peak_afl_controller_handle controller, peak_afl_rectangle * roi);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_ROI_Preset_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_ROI_Preset_Set)(peak_afl_controller_handle controller, peak_afl_roi_preset roiPreset);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Mode_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Mode_Set)(peak_afl_controller_handle controller, peak_afl_controller_automode mode);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Mode_Get)(peak_afl_controller_handle controller, peak_afl_controller_automode * mode);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_BrightnessComponent_Mode_Set)(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode mode);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_BrightnessComponent_Mode_Get)(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode * mode);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_BrightnessComponent_Status)(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_status * status);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_BrightnessComponent_Callback_Set)(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_callback_type type, void * funcPtr, void * context);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Status)(peak_afl_controller_handle controller, peak_afl_controller_status * status);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_GetLastAutoAverage)(peak_afl_controller_handle controller, uint8_t * average);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_GetLastAutoAverages)(peak_afl_controller_handle controller, uint8_t * averageRed, uint8_t * averageGreen, uint8_t * averageBlue);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTarget_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTarget_Set)(peak_afl_controller_handle controller, uint32_t target);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTarget_Get)(peak_afl_controller_handle controller, uint32_t * target);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTarget_GetRange)(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTolerance_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTolerance_Set)(peak_afl_controller_handle controller, uint32_t tolerance);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTolerance_Get)(peak_afl_controller_handle controller, uint32_t * tolerance);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoTolerance_GetRange)(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoPercentile_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoPercentile_Set)(peak_afl_controller_handle controller, double percentile);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoPercentile_Get)(peak_afl_controller_handle controller, double * percentile);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_AutoPercentile_GetRange)(peak_afl_controller_handle controller, double * min, double * max, double * inc);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Type_Get)(peak_afl_controller_handle controller, peak_afl_controllerType * type);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Algorithm_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Algorithm_Set)(peak_afl_controller_handle controller, peak_afl_controller_algorithm algorithm);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Algorithm_Get)(peak_afl_controller_handle controller, peak_afl_controller_algorithm * algorithm);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Algorithm_GetList)(peak_afl_controller_handle controller, peak_afl_controller_algorithm * typeList, uint32_t * listSize);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SharpnessAlgorithm_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SharpnessAlgorithm_Set)(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm algorithm);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SharpnessAlgorithm_Get)(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm * algorithm);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_SharpnessAlgorithm_GetList)(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm * typeList, uint32_t * listSize);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Callback_Set)(peak_afl_controller_handle controller, peak_afl_callback_type type, void * funcPtr, void * context);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Weighted_ROI_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Weighted_ROI_Min_Size)(peak_afl_controller_handle controller, peak_afl_size * size);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Weighted_ROI_Set)(peak_afl_controller_handle controller, const peak_afl_weighted_rectangle * weightedRoiList, uint32_t listSize);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Weighted_ROI_Get)(peak_afl_controller_handle controller, peak_afl_weighted_rectangle * weightedRoiList, uint32_t * listSize);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Limit_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Limit_Default)(peak_afl_controller_handle controller, peak_afl_controller_limit * limit);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Limit_Set)(peak_afl_controller_handle controller, peak_afl_controller_limit limit);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Limit_Get)(peak_afl_controller_handle controller, peak_afl_controller_limit * limit);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Hysteresis_IsSupported)(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Hysteresis_Default)(peak_afl_controller_handle controller, uint8_t * hysteresis);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Hysteresis_Set)(peak_afl_controller_handle controller, uint8_t hysteresis);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Hysteresis_Get)(peak_afl_controller_handle controller, uint8_t * hysteresis);
typedef PEAK_AFL_API_STATUS (*dyn_peak_afl_AutoController_Hysteresis_GetRange)(peak_afl_controller_handle controller, uint8_t * min, uint8_t * max, uint8_t * inc);

                        
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
    
    static PEAK_AFL_API_STATUS peak_afl_Init(void );
    static PEAK_AFL_API_STATUS peak_afl_Exit(void );
    static PEAK_AFL_API_STATUS peak_afl_GetLastError(peak_afl_status * lastErrorCode, char * lastErrorMessage, size_t * lastErrorMessageSize);
    static PEAK_AFL_API_STATUS peak_afl_GetVersion(uint32_t * majorVersion, uint32_t * minorVersion, uint32_t * subminorVersion, uint32_t * patchVersion);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Create(peak_afl_manager_handle * handle, PEAK_NODE_MAP_HANDLE nodeMapHandle);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Destroy(peak_afl_manager_handle handle);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_AddController(peak_afl_manager_handle handle, peak_afl_manager_handle controller);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_RemoveController(peak_afl_manager_handle handle, peak_afl_manager_handle controller);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Process(peak_afl_manager_handle handle, PEAK_IPL_IMAGE_HANDLE imageHandle);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_CreateController(peak_afl_manager_handle handle, peak_afl_controller_handle * controller, peak_afl_controllerType controllerType);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_DestroyAllController(peak_afl_manager_handle handle);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_DestroyController(peak_afl_manager_handle handle, peak_afl_controller_handle controller);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_SetGainIPL(peak_afl_manager_handle handle, PEAK_IPL_GAIN_HANDLE gainHandle);
    static PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Status(peak_afl_manager_handle handle, peak_afl_BOOL8 * running);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Create(peak_afl_controller_handle * controller, peak_afl_controllerType controllerType);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Destroy(peak_afl_controller_handle controller);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_Set(peak_afl_controller_handle controller, uint32_t count);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_Get(peak_afl_controller_handle controller, uint32_t * count);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_GetRange(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Set(peak_afl_controller_handle controller, peak_afl_rectangle roi);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Get(peak_afl_controller_handle controller, peak_afl_rectangle * roi);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Preset_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Preset_Set(peak_afl_controller_handle controller, peak_afl_roi_preset roiPreset);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Mode_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Mode_Set(peak_afl_controller_handle controller, peak_afl_controller_automode mode);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Mode_Get(peak_afl_controller_handle controller, peak_afl_controller_automode * mode);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Mode_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Mode_Set(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode mode);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Mode_Get(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode * mode);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Status(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_status * status);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Callback_Set(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_callback_type type, void * funcPtr, void * context);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Status(peak_afl_controller_handle controller, peak_afl_controller_status * status);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_GetLastAutoAverage(peak_afl_controller_handle controller, uint8_t * average);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_GetLastAutoAverages(peak_afl_controller_handle controller, uint8_t * averageRed, uint8_t * averageGreen, uint8_t * averageBlue);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_Set(peak_afl_controller_handle controller, uint32_t target);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_Get(peak_afl_controller_handle controller, uint32_t * target);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_GetRange(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_Set(peak_afl_controller_handle controller, uint32_t tolerance);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_Get(peak_afl_controller_handle controller, uint32_t * tolerance);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_GetRange(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_Set(peak_afl_controller_handle controller, double percentile);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_Get(peak_afl_controller_handle controller, double * percentile);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_GetRange(peak_afl_controller_handle controller, double * min, double * max, double * inc);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Type_Get(peak_afl_controller_handle controller, peak_afl_controllerType * type);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_Set(peak_afl_controller_handle controller, peak_afl_controller_algorithm algorithm);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_Get(peak_afl_controller_handle controller, peak_afl_controller_algorithm * algorithm);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_GetList(peak_afl_controller_handle controller, peak_afl_controller_algorithm * typeList, uint32_t * listSize);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_Set(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm algorithm);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_Get(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm * algorithm);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_GetList(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm * typeList, uint32_t * listSize);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Callback_Set(peak_afl_controller_handle controller, peak_afl_callback_type type, void * funcPtr, void * context);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_Min_Size(peak_afl_controller_handle controller, peak_afl_size * size);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_Set(peak_afl_controller_handle controller, const peak_afl_weighted_rectangle * weightedRoiList, uint32_t listSize);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_Get(peak_afl_controller_handle controller, peak_afl_weighted_rectangle * weightedRoiList, uint32_t * listSize);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_Default(peak_afl_controller_handle controller, peak_afl_controller_limit * limit);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_Set(peak_afl_controller_handle controller, peak_afl_controller_limit limit);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_Get(peak_afl_controller_handle controller, peak_afl_controller_limit * limit);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_Default(peak_afl_controller_handle controller, uint8_t * hysteresis);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_Set(peak_afl_controller_handle controller, uint8_t hysteresis);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_Get(peak_afl_controller_handle controller, uint8_t * hysteresis);
    static PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_GetRange(peak_afl_controller_handle controller, uint8_t * min, uint8_t * max, uint8_t * inc);
       
private:
    void* m_handle = nullptr;
    dyn_peak_afl_Init m_peak_afl_Init{};
    dyn_peak_afl_Exit m_peak_afl_Exit{};
    dyn_peak_afl_GetLastError m_peak_afl_GetLastError{};
    dyn_peak_afl_GetVersion m_peak_afl_GetVersion{};
    dyn_peak_afl_AutoFeatureManager_Create m_peak_afl_AutoFeatureManager_Create{};
    dyn_peak_afl_AutoFeatureManager_Destroy m_peak_afl_AutoFeatureManager_Destroy{};
    dyn_peak_afl_AutoFeatureManager_AddController m_peak_afl_AutoFeatureManager_AddController{};
    dyn_peak_afl_AutoFeatureManager_RemoveController m_peak_afl_AutoFeatureManager_RemoveController{};
    dyn_peak_afl_AutoFeatureManager_Process m_peak_afl_AutoFeatureManager_Process{};
    dyn_peak_afl_AutoFeatureManager_CreateController m_peak_afl_AutoFeatureManager_CreateController{};
    dyn_peak_afl_AutoFeatureManager_DestroyAllController m_peak_afl_AutoFeatureManager_DestroyAllController{};
    dyn_peak_afl_AutoFeatureManager_DestroyController m_peak_afl_AutoFeatureManager_DestroyController{};
    dyn_peak_afl_AutoFeatureManager_SetGainIPL m_peak_afl_AutoFeatureManager_SetGainIPL{};
    dyn_peak_afl_AutoFeatureManager_Status m_peak_afl_AutoFeatureManager_Status{};
    dyn_peak_afl_AutoController_Create m_peak_afl_AutoController_Create{};
    dyn_peak_afl_AutoController_Destroy m_peak_afl_AutoController_Destroy{};
    dyn_peak_afl_AutoController_SkipFrames_IsSupported m_peak_afl_AutoController_SkipFrames_IsSupported{};
    dyn_peak_afl_AutoController_SkipFrames_Set m_peak_afl_AutoController_SkipFrames_Set{};
    dyn_peak_afl_AutoController_SkipFrames_Get m_peak_afl_AutoController_SkipFrames_Get{};
    dyn_peak_afl_AutoController_SkipFrames_GetRange m_peak_afl_AutoController_SkipFrames_GetRange{};
    dyn_peak_afl_AutoController_ROI_IsSupported m_peak_afl_AutoController_ROI_IsSupported{};
    dyn_peak_afl_AutoController_ROI_Set m_peak_afl_AutoController_ROI_Set{};
    dyn_peak_afl_AutoController_ROI_Get m_peak_afl_AutoController_ROI_Get{};
    dyn_peak_afl_AutoController_ROI_Preset_IsSupported m_peak_afl_AutoController_ROI_Preset_IsSupported{};
    dyn_peak_afl_AutoController_ROI_Preset_Set m_peak_afl_AutoController_ROI_Preset_Set{};
    dyn_peak_afl_AutoController_Mode_IsSupported m_peak_afl_AutoController_Mode_IsSupported{};
    dyn_peak_afl_AutoController_Mode_Set m_peak_afl_AutoController_Mode_Set{};
    dyn_peak_afl_AutoController_Mode_Get m_peak_afl_AutoController_Mode_Get{};
    dyn_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported m_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported{};
    dyn_peak_afl_AutoController_BrightnessComponent_Mode_Set m_peak_afl_AutoController_BrightnessComponent_Mode_Set{};
    dyn_peak_afl_AutoController_BrightnessComponent_Mode_Get m_peak_afl_AutoController_BrightnessComponent_Mode_Get{};
    dyn_peak_afl_AutoController_BrightnessComponent_Status m_peak_afl_AutoController_BrightnessComponent_Status{};
    dyn_peak_afl_AutoController_BrightnessComponent_Callback_Set m_peak_afl_AutoController_BrightnessComponent_Callback_Set{};
    dyn_peak_afl_AutoController_Status m_peak_afl_AutoController_Status{};
    dyn_peak_afl_AutoController_GetLastAutoAverage m_peak_afl_AutoController_GetLastAutoAverage{};
    dyn_peak_afl_AutoController_GetLastAutoAverages m_peak_afl_AutoController_GetLastAutoAverages{};
    dyn_peak_afl_AutoController_AutoTarget_IsSupported m_peak_afl_AutoController_AutoTarget_IsSupported{};
    dyn_peak_afl_AutoController_AutoTarget_Set m_peak_afl_AutoController_AutoTarget_Set{};
    dyn_peak_afl_AutoController_AutoTarget_Get m_peak_afl_AutoController_AutoTarget_Get{};
    dyn_peak_afl_AutoController_AutoTarget_GetRange m_peak_afl_AutoController_AutoTarget_GetRange{};
    dyn_peak_afl_AutoController_AutoTolerance_IsSupported m_peak_afl_AutoController_AutoTolerance_IsSupported{};
    dyn_peak_afl_AutoController_AutoTolerance_Set m_peak_afl_AutoController_AutoTolerance_Set{};
    dyn_peak_afl_AutoController_AutoTolerance_Get m_peak_afl_AutoController_AutoTolerance_Get{};
    dyn_peak_afl_AutoController_AutoTolerance_GetRange m_peak_afl_AutoController_AutoTolerance_GetRange{};
    dyn_peak_afl_AutoController_AutoPercentile_IsSupported m_peak_afl_AutoController_AutoPercentile_IsSupported{};
    dyn_peak_afl_AutoController_AutoPercentile_Set m_peak_afl_AutoController_AutoPercentile_Set{};
    dyn_peak_afl_AutoController_AutoPercentile_Get m_peak_afl_AutoController_AutoPercentile_Get{};
    dyn_peak_afl_AutoController_AutoPercentile_GetRange m_peak_afl_AutoController_AutoPercentile_GetRange{};
    dyn_peak_afl_AutoController_Type_Get m_peak_afl_AutoController_Type_Get{};
    dyn_peak_afl_AutoController_Algorithm_IsSupported m_peak_afl_AutoController_Algorithm_IsSupported{};
    dyn_peak_afl_AutoController_Algorithm_Set m_peak_afl_AutoController_Algorithm_Set{};
    dyn_peak_afl_AutoController_Algorithm_Get m_peak_afl_AutoController_Algorithm_Get{};
    dyn_peak_afl_AutoController_Algorithm_GetList m_peak_afl_AutoController_Algorithm_GetList{};
    dyn_peak_afl_AutoController_SharpnessAlgorithm_IsSupported m_peak_afl_AutoController_SharpnessAlgorithm_IsSupported{};
    dyn_peak_afl_AutoController_SharpnessAlgorithm_Set m_peak_afl_AutoController_SharpnessAlgorithm_Set{};
    dyn_peak_afl_AutoController_SharpnessAlgorithm_Get m_peak_afl_AutoController_SharpnessAlgorithm_Get{};
    dyn_peak_afl_AutoController_SharpnessAlgorithm_GetList m_peak_afl_AutoController_SharpnessAlgorithm_GetList{};
    dyn_peak_afl_AutoController_Callback_Set m_peak_afl_AutoController_Callback_Set{};
    dyn_peak_afl_AutoController_Weighted_ROI_IsSupported m_peak_afl_AutoController_Weighted_ROI_IsSupported{};
    dyn_peak_afl_AutoController_Weighted_ROI_Min_Size m_peak_afl_AutoController_Weighted_ROI_Min_Size{};
    dyn_peak_afl_AutoController_Weighted_ROI_Set m_peak_afl_AutoController_Weighted_ROI_Set{};
    dyn_peak_afl_AutoController_Weighted_ROI_Get m_peak_afl_AutoController_Weighted_ROI_Get{};
    dyn_peak_afl_AutoController_Limit_IsSupported m_peak_afl_AutoController_Limit_IsSupported{};
    dyn_peak_afl_AutoController_Limit_Default m_peak_afl_AutoController_Limit_Default{};
    dyn_peak_afl_AutoController_Limit_Set m_peak_afl_AutoController_Limit_Set{};
    dyn_peak_afl_AutoController_Limit_Get m_peak_afl_AutoController_Limit_Get{};
    dyn_peak_afl_AutoController_Hysteresis_IsSupported m_peak_afl_AutoController_Hysteresis_IsSupported{};
    dyn_peak_afl_AutoController_Hysteresis_Default m_peak_afl_AutoController_Hysteresis_Default{};
    dyn_peak_afl_AutoController_Hysteresis_Set m_peak_afl_AutoController_Hysteresis_Set{};
    dyn_peak_afl_AutoController_Hysteresis_Get m_peak_afl_AutoController_Hysteresis_Get{};
    dyn_peak_afl_AutoController_Hysteresis_GetRange m_peak_afl_AutoController_Hysteresis_GetRange{};

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
    if (_wgetenv_s(&sz, NULL, 0, L"aapienvvarrootpath") == 0
        && sz > 0)
    {
        std::vector<wchar_t> env_peak_afl(sz);
        if (_wgetenv_s(&sz, env_peak_afl.data(), sz, L"aapienvvarrootpath") == 0)
        {
            if (_wgetenv_s(&sz, NULL, 0, L"PATH") == 0
                && sz > 0)
            {
                std::vector<wchar_t> env_path(sz);
                if (_wgetenv_s(&sz, env_path.data(), sz, L"PATH") == 0)
                {
                    std::wstring peak_afl_path(env_peak_afl.data());
#ifdef _WIN64
                    peak_afl_path.append(L"\\aapi\\lib\\x86_64");
#else
                    peak_afl_path.append(L"\\aapi\\lib\\x86_32");
#endif
                    std::wstring path_var(env_path.data());
                    path_var.append(L";").append(peak_afl_path);
                    _wputenv_s(L"PATH", path_var.c_str());
                }
            }
        }
    }
    
    loadLib("peak_afl.dll");
#else
    loadLib("libpeak_afl.so");
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

    m_peak_afl_Init = (dyn_peak_afl_Init) (load ?  import_function(m_handle, "peak_afl_Init") : nullptr);
    if(m_peak_afl_Init == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_Init");
    }        

    m_peak_afl_Exit = (dyn_peak_afl_Exit) (load ?  import_function(m_handle, "peak_afl_Exit") : nullptr);
    if(m_peak_afl_Exit == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_Exit");
    }        

    m_peak_afl_GetLastError = (dyn_peak_afl_GetLastError) (load ?  import_function(m_handle, "peak_afl_GetLastError") : nullptr);
    if(m_peak_afl_GetLastError == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_GetLastError");
    }        

    m_peak_afl_GetVersion = (dyn_peak_afl_GetVersion) (load ?  import_function(m_handle, "peak_afl_GetVersion") : nullptr);
    if(m_peak_afl_GetVersion == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_GetVersion");
    }        

    m_peak_afl_AutoFeatureManager_Create = (dyn_peak_afl_AutoFeatureManager_Create) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_Create") : nullptr);
    if(m_peak_afl_AutoFeatureManager_Create == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_Create");
    }        

    m_peak_afl_AutoFeatureManager_Destroy = (dyn_peak_afl_AutoFeatureManager_Destroy) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_Destroy") : nullptr);
    if(m_peak_afl_AutoFeatureManager_Destroy == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_Destroy");
    }        

    m_peak_afl_AutoFeatureManager_AddController = (dyn_peak_afl_AutoFeatureManager_AddController) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_AddController") : nullptr);
    if(m_peak_afl_AutoFeatureManager_AddController == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_AddController");
    }        

    m_peak_afl_AutoFeatureManager_RemoveController = (dyn_peak_afl_AutoFeatureManager_RemoveController) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_RemoveController") : nullptr);
    if(m_peak_afl_AutoFeatureManager_RemoveController == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_RemoveController");
    }        

    m_peak_afl_AutoFeatureManager_Process = (dyn_peak_afl_AutoFeatureManager_Process) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_Process") : nullptr);
    if(m_peak_afl_AutoFeatureManager_Process == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_Process");
    }        

    m_peak_afl_AutoFeatureManager_CreateController = (dyn_peak_afl_AutoFeatureManager_CreateController) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_CreateController") : nullptr);
    if(m_peak_afl_AutoFeatureManager_CreateController == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_CreateController");
    }        

    m_peak_afl_AutoFeatureManager_DestroyAllController = (dyn_peak_afl_AutoFeatureManager_DestroyAllController) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_DestroyAllController") : nullptr);
    if(m_peak_afl_AutoFeatureManager_DestroyAllController == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_DestroyAllController");
    }        

    m_peak_afl_AutoFeatureManager_DestroyController = (dyn_peak_afl_AutoFeatureManager_DestroyController) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_DestroyController") : nullptr);
    if(m_peak_afl_AutoFeatureManager_DestroyController == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_DestroyController");
    }        

    m_peak_afl_AutoFeatureManager_SetGainIPL = (dyn_peak_afl_AutoFeatureManager_SetGainIPL) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_SetGainIPL") : nullptr);
    if(m_peak_afl_AutoFeatureManager_SetGainIPL == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_SetGainIPL");
    }        

    m_peak_afl_AutoFeatureManager_Status = (dyn_peak_afl_AutoFeatureManager_Status) (load ?  import_function(m_handle, "peak_afl_AutoFeatureManager_Status") : nullptr);
    if(m_peak_afl_AutoFeatureManager_Status == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoFeatureManager_Status");
    }        

    m_peak_afl_AutoController_Create = (dyn_peak_afl_AutoController_Create) (load ?  import_function(m_handle, "peak_afl_AutoController_Create") : nullptr);
    if(m_peak_afl_AutoController_Create == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Create");
    }        

    m_peak_afl_AutoController_Destroy = (dyn_peak_afl_AutoController_Destroy) (load ?  import_function(m_handle, "peak_afl_AutoController_Destroy") : nullptr);
    if(m_peak_afl_AutoController_Destroy == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Destroy");
    }        

    m_peak_afl_AutoController_SkipFrames_IsSupported = (dyn_peak_afl_AutoController_SkipFrames_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_SkipFrames_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_SkipFrames_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SkipFrames_IsSupported");
    }        

    m_peak_afl_AutoController_SkipFrames_Set = (dyn_peak_afl_AutoController_SkipFrames_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_SkipFrames_Set") : nullptr);
    if(m_peak_afl_AutoController_SkipFrames_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SkipFrames_Set");
    }        

    m_peak_afl_AutoController_SkipFrames_Get = (dyn_peak_afl_AutoController_SkipFrames_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_SkipFrames_Get") : nullptr);
    if(m_peak_afl_AutoController_SkipFrames_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SkipFrames_Get");
    }        

    m_peak_afl_AutoController_SkipFrames_GetRange = (dyn_peak_afl_AutoController_SkipFrames_GetRange) (load ?  import_function(m_handle, "peak_afl_AutoController_SkipFrames_GetRange") : nullptr);
    if(m_peak_afl_AutoController_SkipFrames_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SkipFrames_GetRange");
    }        

    m_peak_afl_AutoController_ROI_IsSupported = (dyn_peak_afl_AutoController_ROI_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_ROI_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_ROI_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_ROI_IsSupported");
    }        

    m_peak_afl_AutoController_ROI_Set = (dyn_peak_afl_AutoController_ROI_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_ROI_Set") : nullptr);
    if(m_peak_afl_AutoController_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_ROI_Set");
    }        

    m_peak_afl_AutoController_ROI_Get = (dyn_peak_afl_AutoController_ROI_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_ROI_Get") : nullptr);
    if(m_peak_afl_AutoController_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_ROI_Get");
    }        

    m_peak_afl_AutoController_ROI_Preset_IsSupported = (dyn_peak_afl_AutoController_ROI_Preset_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_ROI_Preset_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_ROI_Preset_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_ROI_Preset_IsSupported");
    }        

    m_peak_afl_AutoController_ROI_Preset_Set = (dyn_peak_afl_AutoController_ROI_Preset_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_ROI_Preset_Set") : nullptr);
    if(m_peak_afl_AutoController_ROI_Preset_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_ROI_Preset_Set");
    }        

    m_peak_afl_AutoController_Mode_IsSupported = (dyn_peak_afl_AutoController_Mode_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_Mode_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_Mode_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Mode_IsSupported");
    }        

    m_peak_afl_AutoController_Mode_Set = (dyn_peak_afl_AutoController_Mode_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_Mode_Set") : nullptr);
    if(m_peak_afl_AutoController_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Mode_Set");
    }        

    m_peak_afl_AutoController_Mode_Get = (dyn_peak_afl_AutoController_Mode_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_Mode_Get") : nullptr);
    if(m_peak_afl_AutoController_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Mode_Get");
    }        

    m_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported = (dyn_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_BrightnessComponent_Mode_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_BrightnessComponent_Mode_IsSupported");
    }        

    m_peak_afl_AutoController_BrightnessComponent_Mode_Set = (dyn_peak_afl_AutoController_BrightnessComponent_Mode_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_BrightnessComponent_Mode_Set") : nullptr);
    if(m_peak_afl_AutoController_BrightnessComponent_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_BrightnessComponent_Mode_Set");
    }        

    m_peak_afl_AutoController_BrightnessComponent_Mode_Get = (dyn_peak_afl_AutoController_BrightnessComponent_Mode_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_BrightnessComponent_Mode_Get") : nullptr);
    if(m_peak_afl_AutoController_BrightnessComponent_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_BrightnessComponent_Mode_Get");
    }        

    m_peak_afl_AutoController_BrightnessComponent_Status = (dyn_peak_afl_AutoController_BrightnessComponent_Status) (load ?  import_function(m_handle, "peak_afl_AutoController_BrightnessComponent_Status") : nullptr);
    if(m_peak_afl_AutoController_BrightnessComponent_Status == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_BrightnessComponent_Status");
    }        

    m_peak_afl_AutoController_BrightnessComponent_Callback_Set = (dyn_peak_afl_AutoController_BrightnessComponent_Callback_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_BrightnessComponent_Callback_Set") : nullptr);
    if(m_peak_afl_AutoController_BrightnessComponent_Callback_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_BrightnessComponent_Callback_Set");
    }        

    m_peak_afl_AutoController_Status = (dyn_peak_afl_AutoController_Status) (load ?  import_function(m_handle, "peak_afl_AutoController_Status") : nullptr);
    if(m_peak_afl_AutoController_Status == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Status");
    }        

    m_peak_afl_AutoController_GetLastAutoAverage = (dyn_peak_afl_AutoController_GetLastAutoAverage) (load ?  import_function(m_handle, "peak_afl_AutoController_GetLastAutoAverage") : nullptr);
    if(m_peak_afl_AutoController_GetLastAutoAverage == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_GetLastAutoAverage");
    }        

    m_peak_afl_AutoController_GetLastAutoAverages = (dyn_peak_afl_AutoController_GetLastAutoAverages) (load ?  import_function(m_handle, "peak_afl_AutoController_GetLastAutoAverages") : nullptr);
    if(m_peak_afl_AutoController_GetLastAutoAverages == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_GetLastAutoAverages");
    }        

    m_peak_afl_AutoController_AutoTarget_IsSupported = (dyn_peak_afl_AutoController_AutoTarget_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTarget_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_AutoTarget_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTarget_IsSupported");
    }        

    m_peak_afl_AutoController_AutoTarget_Set = (dyn_peak_afl_AutoController_AutoTarget_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTarget_Set") : nullptr);
    if(m_peak_afl_AutoController_AutoTarget_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTarget_Set");
    }        

    m_peak_afl_AutoController_AutoTarget_Get = (dyn_peak_afl_AutoController_AutoTarget_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTarget_Get") : nullptr);
    if(m_peak_afl_AutoController_AutoTarget_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTarget_Get");
    }        

    m_peak_afl_AutoController_AutoTarget_GetRange = (dyn_peak_afl_AutoController_AutoTarget_GetRange) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTarget_GetRange") : nullptr);
    if(m_peak_afl_AutoController_AutoTarget_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTarget_GetRange");
    }        

    m_peak_afl_AutoController_AutoTolerance_IsSupported = (dyn_peak_afl_AutoController_AutoTolerance_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTolerance_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_AutoTolerance_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTolerance_IsSupported");
    }        

    m_peak_afl_AutoController_AutoTolerance_Set = (dyn_peak_afl_AutoController_AutoTolerance_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTolerance_Set") : nullptr);
    if(m_peak_afl_AutoController_AutoTolerance_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTolerance_Set");
    }        

    m_peak_afl_AutoController_AutoTolerance_Get = (dyn_peak_afl_AutoController_AutoTolerance_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTolerance_Get") : nullptr);
    if(m_peak_afl_AutoController_AutoTolerance_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTolerance_Get");
    }        

    m_peak_afl_AutoController_AutoTolerance_GetRange = (dyn_peak_afl_AutoController_AutoTolerance_GetRange) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoTolerance_GetRange") : nullptr);
    if(m_peak_afl_AutoController_AutoTolerance_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoTolerance_GetRange");
    }        

    m_peak_afl_AutoController_AutoPercentile_IsSupported = (dyn_peak_afl_AutoController_AutoPercentile_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoPercentile_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_AutoPercentile_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoPercentile_IsSupported");
    }        

    m_peak_afl_AutoController_AutoPercentile_Set = (dyn_peak_afl_AutoController_AutoPercentile_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoPercentile_Set") : nullptr);
    if(m_peak_afl_AutoController_AutoPercentile_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoPercentile_Set");
    }        

    m_peak_afl_AutoController_AutoPercentile_Get = (dyn_peak_afl_AutoController_AutoPercentile_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoPercentile_Get") : nullptr);
    if(m_peak_afl_AutoController_AutoPercentile_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoPercentile_Get");
    }        

    m_peak_afl_AutoController_AutoPercentile_GetRange = (dyn_peak_afl_AutoController_AutoPercentile_GetRange) (load ?  import_function(m_handle, "peak_afl_AutoController_AutoPercentile_GetRange") : nullptr);
    if(m_peak_afl_AutoController_AutoPercentile_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_AutoPercentile_GetRange");
    }        

    m_peak_afl_AutoController_Type_Get = (dyn_peak_afl_AutoController_Type_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_Type_Get") : nullptr);
    if(m_peak_afl_AutoController_Type_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Type_Get");
    }        

    m_peak_afl_AutoController_Algorithm_IsSupported = (dyn_peak_afl_AutoController_Algorithm_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_Algorithm_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_Algorithm_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Algorithm_IsSupported");
    }        

    m_peak_afl_AutoController_Algorithm_Set = (dyn_peak_afl_AutoController_Algorithm_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_Algorithm_Set") : nullptr);
    if(m_peak_afl_AutoController_Algorithm_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Algorithm_Set");
    }        

    m_peak_afl_AutoController_Algorithm_Get = (dyn_peak_afl_AutoController_Algorithm_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_Algorithm_Get") : nullptr);
    if(m_peak_afl_AutoController_Algorithm_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Algorithm_Get");
    }        

    m_peak_afl_AutoController_Algorithm_GetList = (dyn_peak_afl_AutoController_Algorithm_GetList) (load ?  import_function(m_handle, "peak_afl_AutoController_Algorithm_GetList") : nullptr);
    if(m_peak_afl_AutoController_Algorithm_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Algorithm_GetList");
    }        

    m_peak_afl_AutoController_SharpnessAlgorithm_IsSupported = (dyn_peak_afl_AutoController_SharpnessAlgorithm_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_SharpnessAlgorithm_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_SharpnessAlgorithm_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SharpnessAlgorithm_IsSupported");
    }        

    m_peak_afl_AutoController_SharpnessAlgorithm_Set = (dyn_peak_afl_AutoController_SharpnessAlgorithm_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_SharpnessAlgorithm_Set") : nullptr);
    if(m_peak_afl_AutoController_SharpnessAlgorithm_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SharpnessAlgorithm_Set");
    }        

    m_peak_afl_AutoController_SharpnessAlgorithm_Get = (dyn_peak_afl_AutoController_SharpnessAlgorithm_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_SharpnessAlgorithm_Get") : nullptr);
    if(m_peak_afl_AutoController_SharpnessAlgorithm_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SharpnessAlgorithm_Get");
    }        

    m_peak_afl_AutoController_SharpnessAlgorithm_GetList = (dyn_peak_afl_AutoController_SharpnessAlgorithm_GetList) (load ?  import_function(m_handle, "peak_afl_AutoController_SharpnessAlgorithm_GetList") : nullptr);
    if(m_peak_afl_AutoController_SharpnessAlgorithm_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_SharpnessAlgorithm_GetList");
    }        

    m_peak_afl_AutoController_Callback_Set = (dyn_peak_afl_AutoController_Callback_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_Callback_Set") : nullptr);
    if(m_peak_afl_AutoController_Callback_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Callback_Set");
    }        

    m_peak_afl_AutoController_Weighted_ROI_IsSupported = (dyn_peak_afl_AutoController_Weighted_ROI_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_Weighted_ROI_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_Weighted_ROI_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Weighted_ROI_IsSupported");
    }        

    m_peak_afl_AutoController_Weighted_ROI_Min_Size = (dyn_peak_afl_AutoController_Weighted_ROI_Min_Size) (load ?  import_function(m_handle, "peak_afl_AutoController_Weighted_ROI_Min_Size") : nullptr);
    if(m_peak_afl_AutoController_Weighted_ROI_Min_Size == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Weighted_ROI_Min_Size");
    }        

    m_peak_afl_AutoController_Weighted_ROI_Set = (dyn_peak_afl_AutoController_Weighted_ROI_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_Weighted_ROI_Set") : nullptr);
    if(m_peak_afl_AutoController_Weighted_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Weighted_ROI_Set");
    }        

    m_peak_afl_AutoController_Weighted_ROI_Get = (dyn_peak_afl_AutoController_Weighted_ROI_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_Weighted_ROI_Get") : nullptr);
    if(m_peak_afl_AutoController_Weighted_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Weighted_ROI_Get");
    }        

    m_peak_afl_AutoController_Limit_IsSupported = (dyn_peak_afl_AutoController_Limit_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_Limit_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_Limit_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Limit_IsSupported");
    }        

    m_peak_afl_AutoController_Limit_Default = (dyn_peak_afl_AutoController_Limit_Default) (load ?  import_function(m_handle, "peak_afl_AutoController_Limit_Default") : nullptr);
    if(m_peak_afl_AutoController_Limit_Default == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Limit_Default");
    }        

    m_peak_afl_AutoController_Limit_Set = (dyn_peak_afl_AutoController_Limit_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_Limit_Set") : nullptr);
    if(m_peak_afl_AutoController_Limit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Limit_Set");
    }        

    m_peak_afl_AutoController_Limit_Get = (dyn_peak_afl_AutoController_Limit_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_Limit_Get") : nullptr);
    if(m_peak_afl_AutoController_Limit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Limit_Get");
    }        

    m_peak_afl_AutoController_Hysteresis_IsSupported = (dyn_peak_afl_AutoController_Hysteresis_IsSupported) (load ?  import_function(m_handle, "peak_afl_AutoController_Hysteresis_IsSupported") : nullptr);
    if(m_peak_afl_AutoController_Hysteresis_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Hysteresis_IsSupported");
    }        

    m_peak_afl_AutoController_Hysteresis_Default = (dyn_peak_afl_AutoController_Hysteresis_Default) (load ?  import_function(m_handle, "peak_afl_AutoController_Hysteresis_Default") : nullptr);
    if(m_peak_afl_AutoController_Hysteresis_Default == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Hysteresis_Default");
    }        

    m_peak_afl_AutoController_Hysteresis_Set = (dyn_peak_afl_AutoController_Hysteresis_Set) (load ?  import_function(m_handle, "peak_afl_AutoController_Hysteresis_Set") : nullptr);
    if(m_peak_afl_AutoController_Hysteresis_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Hysteresis_Set");
    }        

    m_peak_afl_AutoController_Hysteresis_Get = (dyn_peak_afl_AutoController_Hysteresis_Get) (load ?  import_function(m_handle, "peak_afl_AutoController_Hysteresis_Get") : nullptr);
    if(m_peak_afl_AutoController_Hysteresis_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Hysteresis_Get");
    }        

    m_peak_afl_AutoController_Hysteresis_GetRange = (dyn_peak_afl_AutoController_Hysteresis_GetRange) (load ?  import_function(m_handle, "peak_afl_AutoController_Hysteresis_GetRange") : nullptr);
    if(m_peak_afl_AutoController_Hysteresis_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_afl_AutoController_Hysteresis_GetRange");
    }        

            
            return true;
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_Init(void )
{
    auto& inst = instance();
    if(inst.m_peak_afl_Init)
    {
        return inst.m_peak_afl_Init();
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_Exit(void )
{
    auto& inst = instance();
    if(inst.m_peak_afl_Exit)
    {
        return inst.m_peak_afl_Exit();
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_GetLastError(peak_afl_status * lastErrorCode, char * lastErrorMessage, size_t * lastErrorMessageSize)
{
    auto& inst = instance();
    if(inst.m_peak_afl_GetLastError)
    {
        return inst.m_peak_afl_GetLastError(lastErrorCode, lastErrorMessage, lastErrorMessageSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_GetVersion(uint32_t * majorVersion, uint32_t * minorVersion, uint32_t * subminorVersion, uint32_t * patchVersion)
{
    auto& inst = instance();
    if(inst.m_peak_afl_GetVersion)
    {
        return inst.m_peak_afl_GetVersion(majorVersion, minorVersion, subminorVersion, patchVersion);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_Create(peak_afl_manager_handle * handle, PEAK_NODE_MAP_HANDLE nodeMapHandle)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_Create)
    {
        return inst.m_peak_afl_AutoFeatureManager_Create(handle, nodeMapHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_Destroy(peak_afl_manager_handle handle)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_Destroy)
    {
        return inst.m_peak_afl_AutoFeatureManager_Destroy(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_AddController(peak_afl_manager_handle handle, peak_afl_manager_handle controller)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_AddController)
    {
        return inst.m_peak_afl_AutoFeatureManager_AddController(handle, controller);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_RemoveController(peak_afl_manager_handle handle, peak_afl_manager_handle controller)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_RemoveController)
    {
        return inst.m_peak_afl_AutoFeatureManager_RemoveController(handle, controller);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_Process(peak_afl_manager_handle handle, PEAK_IPL_IMAGE_HANDLE imageHandle)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_Process)
    {
        return inst.m_peak_afl_AutoFeatureManager_Process(handle, imageHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_CreateController(peak_afl_manager_handle handle, peak_afl_controller_handle * controller, peak_afl_controllerType controllerType)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_CreateController)
    {
        return inst.m_peak_afl_AutoFeatureManager_CreateController(handle, controller, controllerType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_DestroyAllController(peak_afl_manager_handle handle)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_DestroyAllController)
    {
        return inst.m_peak_afl_AutoFeatureManager_DestroyAllController(handle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_DestroyController(peak_afl_manager_handle handle, peak_afl_controller_handle controller)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_DestroyController)
    {
        return inst.m_peak_afl_AutoFeatureManager_DestroyController(handle, controller);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_SetGainIPL(peak_afl_manager_handle handle, PEAK_IPL_GAIN_HANDLE gainHandle)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_SetGainIPL)
    {
        return inst.m_peak_afl_AutoFeatureManager_SetGainIPL(handle, gainHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoFeatureManager_Status(peak_afl_manager_handle handle, peak_afl_BOOL8 * running)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoFeatureManager_Status)
    {
        return inst.m_peak_afl_AutoFeatureManager_Status(handle, running);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Create(peak_afl_controller_handle * controller, peak_afl_controllerType controllerType)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Create)
    {
        return inst.m_peak_afl_AutoController_Create(controller, controllerType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Destroy(peak_afl_controller_handle controller)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Destroy)
    {
        return inst.m_peak_afl_AutoController_Destroy(controller);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SkipFrames_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SkipFrames_IsSupported)
    {
        return inst.m_peak_afl_AutoController_SkipFrames_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SkipFrames_Set(peak_afl_controller_handle controller, uint32_t count)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SkipFrames_Set)
    {
        return inst.m_peak_afl_AutoController_SkipFrames_Set(controller, count);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SkipFrames_Get(peak_afl_controller_handle controller, uint32_t * count)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SkipFrames_Get)
    {
        return inst.m_peak_afl_AutoController_SkipFrames_Get(controller, count);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SkipFrames_GetRange(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SkipFrames_GetRange)
    {
        return inst.m_peak_afl_AutoController_SkipFrames_GetRange(controller, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_ROI_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_ROI_IsSupported)
    {
        return inst.m_peak_afl_AutoController_ROI_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_ROI_Set(peak_afl_controller_handle controller, peak_afl_rectangle roi)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_ROI_Set)
    {
        return inst.m_peak_afl_AutoController_ROI_Set(controller, roi);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_ROI_Get(peak_afl_controller_handle controller, peak_afl_rectangle * roi)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_ROI_Get)
    {
        return inst.m_peak_afl_AutoController_ROI_Get(controller, roi);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_ROI_Preset_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_ROI_Preset_IsSupported)
    {
        return inst.m_peak_afl_AutoController_ROI_Preset_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_ROI_Preset_Set(peak_afl_controller_handle controller, peak_afl_roi_preset roiPreset)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_ROI_Preset_Set)
    {
        return inst.m_peak_afl_AutoController_ROI_Preset_Set(controller, roiPreset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Mode_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Mode_IsSupported)
    {
        return inst.m_peak_afl_AutoController_Mode_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Mode_Set(peak_afl_controller_handle controller, peak_afl_controller_automode mode)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Mode_Set)
    {
        return inst.m_peak_afl_AutoController_Mode_Set(controller, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Mode_Get(peak_afl_controller_handle controller, peak_afl_controller_automode * mode)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Mode_Get)
    {
        return inst.m_peak_afl_AutoController_Mode_Get(controller, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_BrightnessComponent_Mode_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported)
    {
        return inst.m_peak_afl_AutoController_BrightnessComponent_Mode_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_BrightnessComponent_Mode_Set(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode mode)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_BrightnessComponent_Mode_Set)
    {
        return inst.m_peak_afl_AutoController_BrightnessComponent_Mode_Set(controller, component, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_BrightnessComponent_Mode_Get(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode * mode)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_BrightnessComponent_Mode_Get)
    {
        return inst.m_peak_afl_AutoController_BrightnessComponent_Mode_Get(controller, component, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_BrightnessComponent_Status(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_status * status)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_BrightnessComponent_Status)
    {
        return inst.m_peak_afl_AutoController_BrightnessComponent_Status(controller, component, status);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_BrightnessComponent_Callback_Set(peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_callback_type type, void * funcPtr, void * context)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_BrightnessComponent_Callback_Set)
    {
        return inst.m_peak_afl_AutoController_BrightnessComponent_Callback_Set(controller, component, type, funcPtr, context);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Status(peak_afl_controller_handle controller, peak_afl_controller_status * status)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Status)
    {
        return inst.m_peak_afl_AutoController_Status(controller, status);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_GetLastAutoAverage(peak_afl_controller_handle controller, uint8_t * average)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_GetLastAutoAverage)
    {
        return inst.m_peak_afl_AutoController_GetLastAutoAverage(controller, average);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_GetLastAutoAverages(peak_afl_controller_handle controller, uint8_t * averageRed, uint8_t * averageGreen, uint8_t * averageBlue)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_GetLastAutoAverages)
    {
        return inst.m_peak_afl_AutoController_GetLastAutoAverages(controller, averageRed, averageGreen, averageBlue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTarget_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTarget_IsSupported)
    {
        return inst.m_peak_afl_AutoController_AutoTarget_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTarget_Set(peak_afl_controller_handle controller, uint32_t target)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTarget_Set)
    {
        return inst.m_peak_afl_AutoController_AutoTarget_Set(controller, target);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTarget_Get(peak_afl_controller_handle controller, uint32_t * target)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTarget_Get)
    {
        return inst.m_peak_afl_AutoController_AutoTarget_Get(controller, target);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTarget_GetRange(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTarget_GetRange)
    {
        return inst.m_peak_afl_AutoController_AutoTarget_GetRange(controller, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTolerance_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTolerance_IsSupported)
    {
        return inst.m_peak_afl_AutoController_AutoTolerance_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTolerance_Set(peak_afl_controller_handle controller, uint32_t tolerance)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTolerance_Set)
    {
        return inst.m_peak_afl_AutoController_AutoTolerance_Set(controller, tolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTolerance_Get(peak_afl_controller_handle controller, uint32_t * tolerance)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTolerance_Get)
    {
        return inst.m_peak_afl_AutoController_AutoTolerance_Get(controller, tolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoTolerance_GetRange(peak_afl_controller_handle controller, uint32_t * min, uint32_t * max, uint32_t * inc)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoTolerance_GetRange)
    {
        return inst.m_peak_afl_AutoController_AutoTolerance_GetRange(controller, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoPercentile_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoPercentile_IsSupported)
    {
        return inst.m_peak_afl_AutoController_AutoPercentile_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoPercentile_Set(peak_afl_controller_handle controller, double percentile)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoPercentile_Set)
    {
        return inst.m_peak_afl_AutoController_AutoPercentile_Set(controller, percentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoPercentile_Get(peak_afl_controller_handle controller, double * percentile)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoPercentile_Get)
    {
        return inst.m_peak_afl_AutoController_AutoPercentile_Get(controller, percentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_AutoPercentile_GetRange(peak_afl_controller_handle controller, double * min, double * max, double * inc)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_AutoPercentile_GetRange)
    {
        return inst.m_peak_afl_AutoController_AutoPercentile_GetRange(controller, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Type_Get(peak_afl_controller_handle controller, peak_afl_controllerType * type)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Type_Get)
    {
        return inst.m_peak_afl_AutoController_Type_Get(controller, type);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Algorithm_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Algorithm_IsSupported)
    {
        return inst.m_peak_afl_AutoController_Algorithm_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Algorithm_Set(peak_afl_controller_handle controller, peak_afl_controller_algorithm algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Algorithm_Set)
    {
        return inst.m_peak_afl_AutoController_Algorithm_Set(controller, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Algorithm_Get(peak_afl_controller_handle controller, peak_afl_controller_algorithm * algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Algorithm_Get)
    {
        return inst.m_peak_afl_AutoController_Algorithm_Get(controller, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Algorithm_GetList(peak_afl_controller_handle controller, peak_afl_controller_algorithm * typeList, uint32_t * listSize)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Algorithm_GetList)
    {
        return inst.m_peak_afl_AutoController_Algorithm_GetList(controller, typeList, listSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SharpnessAlgorithm_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SharpnessAlgorithm_IsSupported)
    {
        return inst.m_peak_afl_AutoController_SharpnessAlgorithm_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SharpnessAlgorithm_Set(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SharpnessAlgorithm_Set)
    {
        return inst.m_peak_afl_AutoController_SharpnessAlgorithm_Set(controller, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SharpnessAlgorithm_Get(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm * algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SharpnessAlgorithm_Get)
    {
        return inst.m_peak_afl_AutoController_SharpnessAlgorithm_Get(controller, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_SharpnessAlgorithm_GetList(peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm * typeList, uint32_t * listSize)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_SharpnessAlgorithm_GetList)
    {
        return inst.m_peak_afl_AutoController_SharpnessAlgorithm_GetList(controller, typeList, listSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Callback_Set(peak_afl_controller_handle controller, peak_afl_callback_type type, void * funcPtr, void * context)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Callback_Set)
    {
        return inst.m_peak_afl_AutoController_Callback_Set(controller, type, funcPtr, context);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Weighted_ROI_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Weighted_ROI_IsSupported)
    {
        return inst.m_peak_afl_AutoController_Weighted_ROI_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Weighted_ROI_Min_Size(peak_afl_controller_handle controller, peak_afl_size * size)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Weighted_ROI_Min_Size)
    {
        return inst.m_peak_afl_AutoController_Weighted_ROI_Min_Size(controller, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Weighted_ROI_Set(peak_afl_controller_handle controller, const peak_afl_weighted_rectangle * weightedRoiList, uint32_t listSize)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Weighted_ROI_Set)
    {
        return inst.m_peak_afl_AutoController_Weighted_ROI_Set(controller, weightedRoiList, listSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Weighted_ROI_Get(peak_afl_controller_handle controller, peak_afl_weighted_rectangle * weightedRoiList, uint32_t * listSize)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Weighted_ROI_Get)
    {
        return inst.m_peak_afl_AutoController_Weighted_ROI_Get(controller, weightedRoiList, listSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Limit_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Limit_IsSupported)
    {
        return inst.m_peak_afl_AutoController_Limit_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Limit_Default(peak_afl_controller_handle controller, peak_afl_controller_limit * limit)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Limit_Default)
    {
        return inst.m_peak_afl_AutoController_Limit_Default(controller, limit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Limit_Set(peak_afl_controller_handle controller, peak_afl_controller_limit limit)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Limit_Set)
    {
        return inst.m_peak_afl_AutoController_Limit_Set(controller, limit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Limit_Get(peak_afl_controller_handle controller, peak_afl_controller_limit * limit)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Limit_Get)
    {
        return inst.m_peak_afl_AutoController_Limit_Get(controller, limit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Hysteresis_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8 * supported)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Hysteresis_IsSupported)
    {
        return inst.m_peak_afl_AutoController_Hysteresis_IsSupported(controller, supported);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Hysteresis_Default(peak_afl_controller_handle controller, uint8_t * hysteresis)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Hysteresis_Default)
    {
        return inst.m_peak_afl_AutoController_Hysteresis_Default(controller, hysteresis);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Hysteresis_Set(peak_afl_controller_handle controller, uint8_t hysteresis)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Hysteresis_Set)
    {
        return inst.m_peak_afl_AutoController_Hysteresis_Set(controller, hysteresis);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Hysteresis_Get(peak_afl_controller_handle controller, uint8_t * hysteresis)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Hysteresis_Get)
    {
        return inst.m_peak_afl_AutoController_Hysteresis_Get(controller, hysteresis);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_AFL_API_STATUS DynamicLoader::peak_afl_AutoController_Hysteresis_GetRange(peak_afl_controller_handle controller, uint8_t * min, uint8_t * max, uint8_t * inc)
{
    auto& inst = instance();
    if(inst.m_peak_afl_AutoController_Hysteresis_GetRange)
    {
        return inst.m_peak_afl_AutoController_Hysteresis_GetRange(controller, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

} /* namespace dynamic */
} /* namespace afl */
} /* namespace peak */

