/*!
 * \file    peak_afl.h
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-11-15
 * \since   1.0
 *
 * Copyright (c) 2022 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak/backend/peak_backend.h>

/* Function declaration modifiers */
#if defined(_WIN32)
#    ifndef PEAK_AFL_NO_DECLSPEC_STATEMENTS
#        ifdef PEAK_AFL_EXPORTING
#            define PEAK_AFL_EXPORT __declspec(dllexport)
#        else
#            define PEAK_AFL_EXPORT __declspec(dllimport)
#        endif
#    else
#        define PEAK_AFL_EXPORT
#    endif
#    if defined(_M_IX86) || defined(__i386__)
#        define PEAK_AFL_CALLCONV __cdecl
#    else
#        define PEAK_AFL_CALLCONV
#    endif
#else
#    define PEAK_AFL_EXPORT
#    define PEAK_AFL_CALLCONV
#endif

#ifdef __cplusplus
#    include <cstddef>
#    include <cstdint>

extern "C" {
#else
#    include <stddef.h>
#    include <stdint.h>
#endif


/*! \brief opaque controller handle type */
typedef void* peak_afl_controller_handle;
/*! \brief opaque manager handle type */
typedef void* peak_afl_manager_handle;

#define PEAK_AFL_VERSION_CODE(major, minor, subminor, patch) (((major) << 24) + ((minor) << 16) + ((subminor) << 8) + (patch))
#define PEAK_AFL_API_STATUS PEAK_AFL_EXPORT peak_afl_status PEAK_AFL_CALLCONV

/*! \defgroup library Library
 *
 * Library level functions and types.
 */

/*! \defgroup status Status types and values
 *
 * Definitions on status types and values.
 */

/*! \defgroup automanager Automanager
 *
 * Definitions on Automanager and values.
 */

/*! \defgroup autocontroller AutoController
 *
 * Definitions on AutoController and values.
 */

/*! \defgroup numeric Numerical
 *
 * Definitions numerical types
 */

/*!
 * \ingroup status
 * \brief peak_afl Status codes
 *
 * The majority of the peak_afl functions return a status code.
 */
typedef enum peak_afl_status
{
    /*! \brief Success */
    PEAK_AFL_STATUS_SUCCESS = 0,

    /*! \brief Error */
    PEAK_AFL_STATUS_ERROR = 1,

    /*! \brief Library not Initialized */
    PEAK_AFL_STATUS_NOT_INITIALIZED = 2,

    /*! \brief Invalid Parameter */
    PEAK_AFL_STATUS_INVALID_PARAMETER = 3,

    /*! \brief Access Denied */
    PEAK_AFL_STATUS_ACCESS_DENIED = 4,

    /*! \brief Busy */
    PEAK_AFL_STATUS_BUSY = 5,

    /*! \brief Buffer too small */
    PEAK_AFL_STATUS_BUFFER_TOO_SMALL = 6,

    /*! \brief Invalid image format */
    PEAK_AFL_STATUS_INVALID_IMAGE_FORMAT = 7,

    /*! \brief Not supported */
    PEAK_AFL_STATUS_NOT_SUPPORTED = 8
} peak_afl_status;

/*!
 * \ingroup autocontroller
 * \brief controller types
 */
typedef enum peak_afl_controllerType
{
    /*! \brief Invalid controller */
    PEAK_AFL_CONTROLLER_TYPE_INVALID = 0,

    /*! \brief Exposure controller */
    PEAK_AFL_CONTROLLER_TYPE_BRIGHTNESS = 1,

    /*! \brief White balance controller */
    PEAK_AFL_CONTROLLER_TYPE_WHITE_BALANCE = 2,

    /*! \brief Autofocus Controller */
    PEAK_AFL_CONTROLLER_TYPE_AUTOFOCUS = 5
} peak_afl_controllerType;

/*!
 * \ingroup autocontroller
 * \brief controller automode
 */
typedef enum peak_afl_controller_automode
{
    /*! \brief Automode off */
    PEAK_AFL_CONTROLLER_AUTOMODE_OFF,

    /*! \brief Automode continuous */
    PEAK_AFL_CONTROLLER_AUTOMODE_CONTINUOUS,

    /*! \brief Automode once */
    PEAK_AFL_CONTROLLER_AUTOMODE_ONCE
} peak_afl_controller_automode;

/*!
 * \ingroup autocontroller
 * \brief controller status
 */
typedef enum peak_afl_controller_status
{
    /*! \brief Controller status undefined */
    PEAK_AFL_CONTROLLER_STATUS_UNDEFINED,

    /*! \brief Controller status off */
    PEAK_AFL_CONTROLLER_STATUS_OFF,

    /*! \brief Controller status in progress */
    PEAK_AFL_CONTROLLER_STATUS_IN_PROGRESS,

    /*! \brief Controller status finished */
    PEAK_AFL_CONTROLLER_STATUS_FINISHED,

    /*! \brief Controller status busy */
    PEAK_AFL_CONTROLLER_STATUS_BUSY,

    /*! \brief Controller status canceled */
    PEAK_AFL_CONTROLLER_STATUS_CANCELED,

    /*! \brief Controller status error */
    PEAK_AFL_CONTROLLER_STATUS_ERROR
} peak_afl_controller_status;

/*!
 * \ingroup autocontroller
 * \brief controller algorithm
 */
typedef enum peak_afl_controller_algorithm
{
    /*! \brief Controller algorithm auto */
    PEAK_AFL_CONTROLLER_ALGORITHM_AUTO,

    /*! \brief Controller algorithm golden ratio search */
    PEAK_AFL_CONTROLLER_ALGORITHM_GOLDEN_RATIO_SEARCH,

    /*! \brief Controller algorithm hill climbing search */
    PEAK_AFL_CONTROLLER_ALGORITHM_HILL_CLIMBING_SEARCH,

    /*! \brief Controller algorithm global search */
    PEAK_AFL_CONTROLLER_ALGORITHM_GLOBAL_SEARCH,

    /*! \brief Controller algorithm full scan */
    PEAK_AFL_CONTROLLER_ALGORITHM_FULL_SCAN
} peak_afl_controller_algorithm;

/*!
 * \ingroup autocontroller
 * \brief controller sharpness algorithm (focus)
 */
typedef enum peak_afl_controller_sharpness_algorithm
{
    /*! \brief Controller sharpness algorithm auto */
    PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_AUTO,

    /*! \brief Controller sharpness algorithm tenengrad */
    PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_TENENGRAD,

    /*! \brief Controller sharpness algorithm sobel */
    PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_SOBEL,

    /*! \brief Controller sharpness algorithm mean score */
    PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_MEAN_SCORE,

    /*! \brief Controller sharpness algorithm histogram variance */
    PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_HISTOGRAM_VARIANCE
} peak_afl_controller_sharpness_algorithm;

/*!
 * \ingroup autocontroller
 * \brief controller brightness component
 */
typedef enum peak_afl_controller_brightness_component
{
    /*! \brief exposure */
    PEAK_AFL_CONTROLLER_BRIGHTNESS_COMPONENT_EXPOSURE = 0x01,

    /*! \brief gain */
    PEAK_AFL_CONTROLLER_BRIGHTNESS_COMPONENT_GAIN = 0x02,

} peak_afl_controller_brightness_component;

/*!
 * \ingroup autocontroller
 * \brief controller roi weight (focus)
 */
typedef enum peak_afl_roi_weight
{
    /*! \brief Controller roi weight weak */
    PEAK_AFL_CONTROLLER_ROI_WEIGHT_WEAK = 0x0021,

    /*! \brief Controller roi weight middle */
    PEAK_AFL_CONTROLLER_ROI_WEIGHT_MEDIUM = 0x0042,

    /*! \brief Controller roi weight strong */
    PEAK_AFL_CONTROLLER_ROI_WEIGHT_STRONG = 0x0063
} peak_afl_roi_weight;

/*!
 * \ingroup autocontroller
 * \brief controller callback type
 */
typedef enum peak_afl_callback_type
{
    /*! \brief Controller callback type finished */
    PEAK_AFL_CONTROLLER_CALLBACK_FINISHED,

    /*! \brief Controller callback type processing data */
    PEAK_AFL_CONTROLLER_CALLBACK_PROCESSING_DATA
} peak_afl_callback_type;

/*! \brief Callback function type definition for #PEAK_AFL_CONTROLLER_CALLBACK_FINISHED
 * \param context User context data
 */
typedef void(PEAK_AFL_CALLCONV* PEAK_AFL_CALLBACK_FINISHED_FUNC)(void* context);

/*! \brief Callback function type definition for #PEAK_AFL_CONTROLLER_CALLBACK_PROCESSING_DATA
 * \param sharpnessValue Current sharpness value
 * \param focusValue Current focus value
 * \param context User context data
 */
typedef void(PEAK_AFL_CALLCONV* PEAK_AFL_CALLBACK_PROCESSING_DATA_FUNC)(int focusValue, int sharpnessValue, void* context);

/*!
 * \ingroup autocontroller
 * \brief controller roi preset
 */
typedef enum peak_afl_roi_preset
{
    /*! \brief controller roi preset center */
    PEAK_AFL_CONTROLLER_ROI_PRESET_CENTER
} peak_afl_roi_preset;

/*!
 * \ingroup numeric
 * \brief peak_afl Size (2D)
 *
 * Defines a size in a 2-dimensional coordinate space.
 *
 * This type is used in several library functions and types.
 */
typedef struct
{
    /*! \brief Width */
    uint32_t width;

    /*! \brief Height */
    uint32_t height;
} peak_afl_size;

/*!
 * \ingroup numeric
 * \brief peak_afl Position (2D)
 *
 * Defines a position in a 2-dimensional coordinate space.
 *
 * This type is used in several library functions and types.
 */
typedef struct
{
    /*! \brief X-Position */
    uint32_t x;

    /*! \brief Y-Position */
    uint32_t y;
} peak_afl_position;

/*!
 * \ingroup numeric
 * \brief peak_afl Rectangle (2D)
 *
 * Defines a position in a 2-dimensional coordinate space.
 *
 * This type is used in several library functions and types.
 */
typedef struct
{
    /*! \brief X-Position */
    uint32_t x;

    /*! \brief Y-Position */
    uint32_t y;

    /*! \brief Width */
    uint32_t width;

    /*! \brief Height */
    uint32_t height;
} peak_afl_rectangle;

/*!
 * \ingroup numeric
 * \brief peak_afl Focus Rectangle
 *
 * Defines a focus roi.
 *
 */
typedef struct
{
    /*! \brief Roi */
    peak_afl_rectangle roi;

    /*! \brief Weight */
    peak_afl_roi_weight weight;
} peak_afl_weighted_rectangle;

/*!
 * \ingroup numeric
 * \brief peak_afl Focus Limit
 *
 * Defines a focus limit.
 *
 */
typedef struct
{
    /*! \brief Minimum */
    int min;

    /*! \brief Maximum */
    int max;
} peak_afl_controller_limit;

/*! \brief custom boolean type */
typedef uint8_t peak_afl_BOOL8;

/*! \brief Boolean True */
#define PEAK_AFL_TRUE 1

/*! \brief Boolean False */
#define PEAK_AFL_FALSE 0

/*!
 * \ingroup library
 * \brief Init the peak_afl auto feature library
 *
 * Initializes the internal library status.
 *
 * This function must be called prior to any other function call.\n
 * The function may be called multiple times from a single client process.
 * For each call there must be a corresponding call to #peak_afl_Exit
 * to ensure proper deinitialization of the library status.
 *
 * \return #PEAK_AFL_STATUS_SUCCESS Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_ERROR   An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_Init(void);

/*!
 * \ingroup library
 * \brief Exit the peak_afl auto feature library
 *
 * Deinitializes the internal library status.
 *
 * For each call to #peak_afl_Init there must be a corresponding call to this function
 * to ensure proper deinitialization of the library status. \n
 * After the library has been exited its functions (besides #peak_afl_Init) will not be operable
 * until #peak_afl_Init has been called again.
 *
 * \return #PEAK_AFL_STATUS_SUCCESS         Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED The library is not initialized.
 * \return #PEAK_AFL_STATUS_ERROR           An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_Exit(void);

/*!
 * \ingroup library
 * \brief Get the last error message
 *
 * This function implements the \ref principle_two_stage_query principle.
 *
 * \param[out] lastErrorCode            The error code of the last error. NULL if not required.
 * \param[out] lastErrorMessage         Pointer to a user allocated C string buffer to receive the last error text.
 *                                      If this parameter is NULL, \p lastErrorMessageSize will contain the needed size
 *                                      of \p lastErrorMessage in bytes. The size includes the terminating 0.
 * \param[in,out] lastErrorMessageSize  \li \p lastErrorMessage equal NULL: \n
 *                                          out: minimal size of \p lastErrorMessage in bytes to hold all information \n
 *                                      \li \p lastErrorMessage unequal NULL: \n
 *                                          in: size of the provided \p lastErrorMessage in bytes \n
 *                                          out: number of bytes filled by the function
 *
 * \return #PEAK_AFL_STATUS_SUCCESS          Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED  The library is not initialized.
 * \return #PEAK_AFL_STATUS_BUFFER_TOO_SMALL The supplied buffer is too small.
 * \return #PEAK_AFL_STATUS_ERROR            An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_GetLastError(peak_afl_status* lastErrorCode, char* lastErrorMessage, size_t* lastErrorMessageSize);

/*!
 * \ingroup library
 * \brief Query the library version
 *
 * Provides the version of the library divided in major, minor, subminor and patch, in that order of magnitude.
 *
 * You are allowed to pass NULL for parts you are not interested in.
 *
 * \param[out] majorVersion      Major Version. NULL if not required.
 * \param[out] minorVersion      Minor Version. NULL if not required.
 * \param[out] subminorVersion   Subminor Version. NULL if not required.
 * \param[out] patchVersion      Patch Version. NULL if not required.
 *
 * \return #PEAK_AFL_STATUS_SUCCESS  Operation was successful; no error occurred.
 *
 * \note This function can be used even if the library is not initialized.
 */
PEAK_AFL_API_STATUS peak_afl_GetVersion(
    uint32_t* majorVersion, uint32_t* minorVersion, uint32_t* subminorVersion, uint32_t* patchVersion);

/*!
 * \ingroup automanager
 * \brief Create an automanager instance
 *
 * Creates an automanager instance.
 * The PEAK_NODE_MAP_HANDLE can be acquired by calling Handle() from std::shared_ptr<peak::core::NodeMap>
 *
 * \param[out] handle        pointer to an autofeature handle
 * \param[in]  nodeMapHandle a handle to the device nodemap
 *
 * \return #PEAK_AFL_STATUS_SUCCESS         Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED The library is not initialized.
 * \return #PEAK_AFL_STATUS_ERROR           An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Create(peak_afl_manager_handle* handle, PEAK_NODE_MAP_HANDLE nodeMapHandle);

/*!
 * \ingroup automanager
 * \brief Destroy an automanager instance
 *
 * If the functions succeeds the handle is invalid.
 *
 * \param[in] handle autofeature handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The library is not initialized.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Destroy(peak_afl_manager_handle handle);

/*!
 * \ingroup automanager
 * \brief Add a controller to an automanager instance
 *
 * An autocontroller instance can only be added to one autofeature manager at the same time.
 * The same type can only be added one time.
 * To create an instance call #peak_afl_AutoController_Create or call the convenience function
 * #peak_afl_AutoFeatureManager_CreateController.
 *
 * \param[in] handle     autofeature handle
 * \param[in] controller controller handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_AddController(peak_afl_manager_handle handle, peak_afl_manager_handle controller);

/*!
 * \ingroup automanager
 * \brief Remove a controller to an automanager instance
 *
 * Remove a controller from a manager.
 * To destroy the controller object, call #peak_afl_AutoController_Destroy afterwards.
 * Or #peak_afl_AutoFeatureManager_AddController to add it to another manager.
 * For a function which destroys the object in one call, see #peak_afl_AutoFeatureManager_DestroyController.
 *
 * \param[in] handle     autofeature handle
 * \param[in] controller controller handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_RemoveController(
    peak_afl_manager_handle handle, peak_afl_manager_handle controller);

/*!
 * \ingroup automanager
 * \brief Process an image
 *
 * Processes an image. To get the handle for the image, call ImageBackendAccessor::BackendHandle(img)
 * with img being a peak::ipl::Image
 *
 * \param[in] handle      autofeature handle
 * \param[in] imageHandle image handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS              Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED      The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER    The argument is invalid.
 * \return #PEAK_AFL_STATUS_INVALID_IMAGE_FORMAT The image format is not supported
 * \return #PEAK_AFL_STATUS_ERROR                An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Process(peak_afl_manager_handle handle, PEAK_IPL_IMAGE_HANDLE imageHandle);

/*!
 * \ingroup automanager
 * \brief Create a controller and append it to Manager
 *
 * Convenience function to create an autofeature controller and add it to the manager.
 * The same type can only be added one time.
 *
 * \param[in]  handle         manager handle
 * \param[out] controller     controller handle
 * \param[in]  controllerType controller type, see #peak_afl_controllerType
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_CreateController(
    peak_afl_manager_handle handle, peak_afl_controller_handle* controller, peak_afl_controllerType controllerType);

/*!
 * \ingroup automanager
 * \brief Destroy all controllers for a manager.
 *
 * After this operation all controller handles associated with the given manager will be invalid.
 *
 * \param[in] handle controller handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_DestroyAllController(peak_afl_manager_handle handle);

/*!
 * \ingroup automanager
 * \brief Destroy a controller.
 *
 * After this operation the controller handle will be invalid.
 *
 * \param[in] handle     manager handle
 * \param[in] controller controller handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_DestroyController(
    peak_afl_manager_handle handle, peak_afl_controller_handle controller);

/*!
 * \ingroup automanager
 * \brief Set the IPL Gain
 *
 * To get the handle for the gain, call GainBackendAccessor::BackendHandle(gain) with gain being a peak::ipl::Gain
 *
 * \param[in] handle        pointer to an autofeature handle
 * \param[in] gainHandle    a handle to the ipl gain
 *
 * \return #PEAK_AFL_STATUS_SUCCESS         Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED The library is not initialized.
 * \return #PEAK_AFL_STATUS_ERROR           An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_SetGainIPL(peak_afl_manager_handle handle, PEAK_IPL_GAIN_HANDLE gainHandle);

/*!
 * \ingroup automanager
 * \brief Get the status of a manager.
 *
 * The parameter /p running will contain whether the manager currently processes an image (#PEAK_AFL_TRUE) or is idle
 * (#PEAK_AFL_FALSE).
 *
 * \param[in]  handle  manager handle
 * \param[out] running manager status
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoFeatureManager_Status(peak_afl_manager_handle handle, peak_afl_BOOL8* running);

/*!
 * \ingroup autocontroller
 * \brief Create a new controller
 *
 * \param[out] controller     controller handle
 * \param[in]  controllerType controller type
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Create(
    peak_afl_controller_handle* controller, peak_afl_controllerType controllerType);

/*!
 * \ingroup autocontroller
 * \brief Destroy a controller. Will fail if it is queued inside a manager
 *
 * \param[in] controller controller handle
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ACCESS_DENIED     The operation is not allowed. Remove the instance from a manager first.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Destroy(peak_afl_controller_handle controller);

/*!
 * \ingroup autocontroller
 * \brief Check if skip frames is supported for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports skipping frames
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set number of frames skipped for a controller.
 *
 * Sets the skipped frames for the controller. Only every N-th image will be processed.
 *
 * \param[in] controller controller handle
 * \param[in] count      number of frames skipped
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_Set(peak_afl_controller_handle controller, uint32_t count);

/*!
 * \ingroup autocontroller
 * \brief Get number of frames skipped for a controller.
 *
 * Gets the skipped frames for the controller. Only every N-th image will be processed.
 *
 * \param[in]  controller controller handle
 * \param[out] count      number of frames skipped
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_Get(peak_afl_controller_handle controller, uint32_t* count);

/*!
 * \ingroup autocontroller
 * \brief Get range for frames skipped for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] min        minimum number of frames skipped
 * \param[out] max        maximum number of frames skipped
 * \param[out] inc        increment number of frames skipped

 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SkipFrames_GetRange(
    peak_afl_controller_handle controller, uint32_t* min, uint32_t* max, uint32_t* inc);

/*!
 * \ingroup autocontroller
 * \brief Check if setting a region of interest is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE region of interest is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports setting a region of interest
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature region of interest for a controller.
 *
 * Set the region of interest for a controller. The processed image will be cropped to the region of interest set.
 *
 * \param[in] controller controller handle
 * \param[in] roi        region of interest
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Set(peak_afl_controller_handle controller, peak_afl_rectangle roi);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature region of interest for a controller.
 *
 * Get the region of interest for a controller. The processed image will be cropped to the region of interest set.
 *
 * \param[in] controller controller handle
 * \param[out] roi        region of interest
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Get(peak_afl_controller_handle controller, peak_afl_rectangle* roi);

/*!
 * \ingroup autocontroller
 * \brief Check if ROI preset is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE ROI preset is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports ROI preset
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Preset_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature region of interest preset for a controller.
 *
 * Will set the supplied preset as region of interest. See also #peak_afl_AutoController_ROI_Set.
 *
 * \param[in] controller controller handle
 * \param[in] roiPreset  region of interest preset
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_ROI_Preset_Set(peak_afl_controller_handle controller, peak_afl_roi_preset roiPreset);

/*!
 * \ingroup autocontroller
 * \brief Check if auto mode is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE auto mode is supported, otherwise it is unsupported.

 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports auto mode
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Mode_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature mode for a controller.
 *
 * Will set the controller mode to mode. See #peak_afl_controller_automode for a list of valid values.
 *
 * \param[in] controller controller handle
 * \param[in] mode       autofeature mode
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Mode_Set(peak_afl_controller_handle controller, peak_afl_controller_automode mode);

/*!
 * \ingroup autocontroller
 * \brief Get the current autofeature mode for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] mode       autofeature mode
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Mode_Get(peak_afl_controller_handle controller, peak_afl_controller_automode* mode);

/*!
 * \ingroup autocontroller
 * \brief Check if brightness component is supported for a controller component.
 *
 * If \p supported is #PEAK_AFL_TRUE auto mode is supported, otherwise it is unsupported.

 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller component supports auto mode
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Mode_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature mode for a brightness component.
 *
 * Will set the controller component to component. See #peak_afl_controller_brightness_component for a list of valid values.
 *
 * \param[in] controller controller handle
 * \param[in] component  brightness component
 * \param[in] mode       autofeature mode
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Mode_Set(
    peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode mode);

/*!
 * \ingroup autocontroller
 * \brief Get the current brightness component mode for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] component  brightness component
 * \param[out] mode       autofeature mode
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Mode_Get(
    peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_automode* mode);

/*!
 * \ingroup autocontroller
 * \brief Get the status for a controller component.
 *
 * See #peak_afl_controller_status for a list of values.
 *
 * \param[in]  controller controller handle
 * \param[out] component  brightness component
 * \param[out] status     controller status
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Status(
    peak_afl_controller_handle controller, peak_afl_controller_brightness_component component, peak_afl_controller_status* status);

/*!
 * \ingroup autocontroller
 * \brief Set a callback for a controller component.
 *
 * Set \p funcPtr to NULL to disable the callback.
 * Set \p funcPtr to !NULL to enable the callback.
 *
 * The callback set by \p funcPtr will be called with \p context as its last parameter.
 *
 * For a list of valid types see #peak_afl_callback_type.
 *
 * \param[in]  controller controller handle
 * \param[out] component  brightness component
 * \param[in]  type       callback type
 * \param[in]  funcPtr    function pointer
 * \param[in]  context    pointer
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_BrightnessComponent_Callback_Set(peak_afl_controller_handle controller,
    peak_afl_controller_brightness_component component, peak_afl_callback_type type, void* funcPtr, void* context);

/*!
 * \ingroup autocontroller
 * \brief Get the status for a controller.
 *
 * See #peak_afl_controller_status for a list of values.
 *
 * \param[in]  controller controller handle
 * \param[out] status     controller status
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Status(peak_afl_controller_handle controller, peak_afl_controller_status* status);

/*!
 * \ingroup autocontroller
 * \brief Get the last auto average for a controller.
 *
 * Used by Controllers processing a mono image.
 *
 * \param[in]  controller controller handle
 * \param[out] average    autofeature average
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_GetLastAutoAverage(peak_afl_controller_handle controller, uint8_t* average);

/*!
 * \ingroup autocontroller
 * \brief Get the last auto average for a controller.
 *
 * Used by Controllers processing a color image.
 *
 * \param[in]  controller   controller handle
 * \param[out] averageRed   autofeature average red channel
 * \param[out] averageGreen autofeature average green channel
 * \param[out] averageBlue  autofeature average blue channel
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_GetLastAutoAverages(
    peak_afl_controller_handle controller, uint8_t* averageRed, uint8_t* averageGreen, uint8_t* averageBlue);

/*!
 * \ingroup autocontroller
 * \brief check if auto target is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE auto target is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports auto target
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the auto target for a controller.
 *
 * Set an auto target. Call #peak_afl_AutoController_AutoTarget_GetRange to get the valid range.
 * End value which will be targeted by the controller.
 *
 * \param[in] controller controller handle
 * \param[in] target     auto target value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_Set(peak_afl_controller_handle controller, uint32_t target);

/*!
 * \ingroup autocontroller
 * \brief Get the currently set auto target for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] target     auto target value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_Get(peak_afl_controller_handle controller, uint32_t* target);

/*!
 * \ingroup autocontroller
 * \brief Get the auto target range for a controller.
 *
 * Call this function to get the range of valid values which can be set by a call to
 * #peak_afl_AutoController_AutoTarget_Set.
 *
 * \param[in]  controller controller handle
 * \param[out] min        auto target minimum value
 * \param[out] max        auto target maximum value
 * \param[out] inc        auto target increment value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTarget_GetRange(
    peak_afl_controller_handle controller, uint32_t* min, uint32_t* max, uint32_t* inc);

/*!
 * \ingroup autocontroller
 * \brief check if auto tolerance is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE auto tolerance is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports auto tolerance
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the auto tolerance for a controller.
 *
 * Sets the +/- tolerance for the auto target value.
 *
 * \param[in] controller controller handle
 * \param[in] tolerance  auto target value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_Set(peak_afl_controller_handle controller, uint32_t tolerance);

/*!
 * \ingroup autocontroller
 * \brief Get the current auto tolerance for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] tolerance  auto tolerance value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_Get(peak_afl_controller_handle controller, uint32_t* tolerance);

/*!
 * \ingroup autocontroller
 * \brief Get the auto tolerance range for a controller.
 *
 * Call this function to get the range of valid values which can be set by a call to
 * #peak_afl_AutoController_AutoTolerance_Set.
 *
 * \param[in]  controller controller handle
 * \param[out] min        auto tolerance minimum value
 * \param[out] max        auto tolerance maximum value
 * \param[out] inc        auto tolerance increment value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoTolerance_GetRange(
    peak_afl_controller_handle controller, uint32_t* min, uint32_t* max, uint32_t* inc);

/*!
 * \ingroup autocontroller
 * \brief check if auto percentile is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE auto percentile is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports auto percentile
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the auto percentile for a controller.
 *
 * This is the used percentile value for a controller.
 * To get the valid range, call #peak_afl_AutoController_AutoPercentile_GetRange.
 *
 * \param[in] controller controller handle
 * \param[in] percentile auto percentile value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_Set(peak_afl_controller_handle controller, double percentile);

/*!
 * \ingroup autocontroller
 * \brief Get the auto percentile for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] percentile auto percentile value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_Get(peak_afl_controller_handle controller, double* percentile);

/*!
 * \ingroup autocontroller
 * \brief Get the auto percentile range for a controller.
 *
 * Call this function to get the range of valid values which can be set by a call to
 * #peak_afl_AutoController_AutoPercentile_Set.
 *
 * \param[in]  controller controller handle
 * \param[out] min        auto percentile minimum value
 * \param[out] max        auto percentile maximum value
 * \param[out] inc        auto percentile increment value
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_AutoPercentile_GetRange(
    peak_afl_controller_handle controller, double* min, double* max, double* inc);

/*!
 * \ingroup autocontroller
 * \brief Get the controller type.
 *
 * See #peak_afl_controllerType for a list of values.
 *
 * \param[in]  controller controller handle
 * \param[out] type       auto controller type
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Type_Get(peak_afl_controller_handle controller, peak_afl_controllerType* type);

/*!
 * \ingroup autocontroller
 * \brief check if setting an algorithm is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE algorithm is supported, otherwise it is unsupported.
 *
 * Call #peak_afl_AutoController_Algorithm_GetList to get a list of supported algorithms.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports setting an algorithm
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the used algorithm for a controller.
 *
 * To get a list of supported algorithms see #peak_afl_AutoController_Algorithm_GetList.
 *
 * \param[in]  controller controller handle
 * \param[in]  algorithm  auto controller algorithm
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_Set(
    peak_afl_controller_handle controller, peak_afl_controller_algorithm algorithm);

/*!
 * \ingroup autocontroller
 * \brief Get the used algorithm for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] algorithm  auto controller algorithm
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_Get(
    peak_afl_controller_handle controller, peak_afl_controller_algorithm* algorithm);

/*!
 * \ingroup autocontroller
 * \brief Get the list of supported algorithms for a controller.
 *
 * Uses the \ref principle_two_stage_query principle.
 * To set a value, see #peak_afl_AutoController_Algorithm_Set.
 *
 * \param[in]     controller  controller handle
 * \param[out]    typeList    Pointer to a user allocated list of #peak_afl_controller_algorithm.
 *                            If this parameter is NULL, \p listSize will contain the needed count of \p typeList.
 * \param[in,out] listSize    \li \p typeList equal NULL: \n
 *                                out: minimal size of \p listSize in count of #peak_afl_controller_algorithm to hold the list \n
 *                            \li \p typeList unequal NULL: \n
 *                                in: size of the provided \p lastErrorMessage in counts of #peak_afl_controller_algorithm \n
 *                                out: number of #peak_afl_controller_algorithm used by the function
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_BUFFER_TOO_SMALL  The supplied buffer is too small.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Algorithm_GetList(
    peak_afl_controller_handle controller, peak_afl_controller_algorithm* typeList, uint32_t* listSize);

/*!
 * \ingroup autocontroller
 * \brief check setting a sharpness algorithm is supported by a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE sharpness algorithm is supported, otherwise it is unsupported.
 *
 * Call #peak_afl_AutoController_SharpnessAlgorithm_GetList to get a list of supported sharpness algorithms.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports setting a sharpness algorithm
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Set the used sharpness algorithm for a controller.
 *
 * To get a list of supported algorithms see #peak_afl_AutoController_SharpnessAlgorithm_GetList.
 *
 * \param[in]  controller controller handle
 * \param[in]  algorithm  auto controller sharpness algorithm
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_Set(
    peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm algorithm);

/*!
 * \ingroup autocontroller
 * \brief Get the used sharpness algorithm for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] algorithm  auto controller sharpness algorithm
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_Get(
    peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm* algorithm);

/*!
 * \ingroup autocontroller
 * \brief Get the list of supported sharpness algorithms for a controller.
 *
 * Uses the \ref principle_two_stage_query principle.
 * To set a value, see #peak_afl_AutoController_SharpnessAlgorithm_Set.
 *
 * \param[in]     controller  controller handle
 * \param[out]    typeList    Pointer to a user allocated list of #peak_afl_controller_sharpness_algorithm.
 *                            If this parameter is NULL, \p listSize will contain the needed count of \p typeList.
 * \param[in,out] listSize    \li \p typeList equal NULL: \n
 *                                out: minimal size of \p listSize in count of #peak_afl_controller_sharpness_algorithm to hold the list
 * \n \li \p typeList unequal NULL: \n in: size of the provided \p lastErrorMessage in counts of #peak_afl_controller_sharpness_algorithm
 * \n out: number of #peak_afl_controller_sharpness_algorithm used by the function
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_BUFFER_TOO_SMALL  The supplied buffer is too small.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_SharpnessAlgorithm_GetList(
    peak_afl_controller_handle controller, peak_afl_controller_sharpness_algorithm* typeList, uint32_t* listSize);

/*!
 * \ingroup autocontroller
 * \brief Set a callback for a controller.
 *
 * Set \p funcPtr to NULL to disable the callback.
 * Set \p funcPtr to !NULL to enable the callback.
 *
 * The callback set by \p funcPtr will be called with \p context as its last parameter.
 *
 * For a list of valid types see #peak_afl_callback_type.
 *
 * \param[in] controller controller handle
 * \param[in] type       callback type
 * \param[in] funcPtr    function pointer
 * \param[in] context    pointer
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Callback_Set(
    peak_afl_controller_handle controller, peak_afl_callback_type type, void* funcPtr, void* context);

/*!
 * \ingroup autocontroller
 * \brief Check if weighted region of interest is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE weighted region of interest is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports weighted roi
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature minimum size of weighted region of interest for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] size       the minimum size
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_Min_Size(peak_afl_controller_handle controller, peak_afl_size* size);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature weighted region of interest for a controller.
 *
 * Already set weighted regions of interest will be overwritten
 *
 * \param[in] controller      controller handle
 * \param[in] weightedRoiList list of weighted region of interest
 * \param[in] listSize        count of weighted region of interest in /p weightedRoiList
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_Set(
    peak_afl_controller_handle controller, const peak_afl_weighted_rectangle* weightedRoiList, uint32_t listSize);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature weighted region of interest for a controller.
 *
 * Uses the \ref principle_two_stage_query principle.
 *
 * \param[in]     controller      controller handle
 * \param[out]    weightedRoiList Pointer to a user allocated list of #peak_afl_weighted_rectangle.
 *                                If this parameter is NULL, \p listSize will contain the needed count of \p typeList.
 * \param[in,out] listSize        \li \p weightedRoiList equal NULL: \n
 *                                    out: minimal size of \p listSize in count of
 *                                         #peak_afl_weighted_rectangle to hold the list \n
 *                                \li \p typeList unequal NULL: \n
 *                                    in: size of the provided \p lastErrorMessage in counts of
 *                                        #peak_afl_weighted_rectangle \n
 *                                    out: number of #peak_afl_weighted_rectangle used by the function
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_BUFFER_TOO_SMALL  The supplied buffer is too small.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Weighted_ROI_Get(
    peak_afl_controller_handle controller, peak_afl_weighted_rectangle* weightedRoiList, uint32_t* listSize);

/*!
 * \ingroup autocontroller
 * \brief Check if limit is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE limit is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports limit
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_IsSupported(peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature default limit.
 *
 * \param[in]  controller controller handle
 * \param[out] limit      the default limit
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_Default(
    peak_afl_controller_handle controller, peak_afl_controller_limit* limit);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature limit for a controller.
 *
 * Sets the minimum and maximum limit for the algorithm set by #peak_afl_AutoController_Algorithm_Set
 *
 * \param[in] controller controller handle
 * \param[in] limit      the limit to set
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_Set(peak_afl_controller_handle controller, peak_afl_controller_limit limit);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature limit for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] limit      the limit to get
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Limit_Get(peak_afl_controller_handle controller, peak_afl_controller_limit* limit);

/*!
 * \ingroup autocontroller
 * \brief Check if hysteresis is supported for a controller.
 *
 * If \p supported is #PEAK_AFL_TRUE hysteresis is supported, otherwise it is unsupported.
 *
 * \param[in]  controller controller handle
 * \param[out] supported  boolean if controller supports hysteresis
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_IsSupported(
    peak_afl_controller_handle controller, peak_afl_BOOL8* supported);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature hysteresis default.
 *
 * \param[in]  controller controller handle
 * \param[out] hysteresis the default hysteresis
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_Default(peak_afl_controller_handle controller, uint8_t* hysteresis);

/*!
 * \ingroup autocontroller
 * \brief Set the autofeature hysteresis for a controller.
 *
 * Set the hysteresis for the algorithm set by #peak_afl_AutoController_Algorithm_Set
 *
 * \param[in] controller controller handle
 * \param[in] hysteresis the hysteresis to set
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_Set(peak_afl_controller_handle controller, uint8_t hysteresis);

/*!
 * \ingroup autocontroller
 * \brief Get the autofeature hysteresis for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] hysteresis the hysteresis
 *
 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_Get(peak_afl_controller_handle controller, uint8_t* hysteresis);

/*!
 * \ingroup autocontroller
 * \brief Get autofeature hysteresis range for a controller.
 *
 * \param[in]  controller controller handle
 * \param[out] min        minimum number of hysteresis
 * \param[out] max        maximum number of hysteresis
 * \param[out] inc        increment number of hysteresis

 * \return #PEAK_AFL_STATUS_SUCCESS           Operation was successful; no error occurred.
 * \return #PEAK_AFL_STATUS_NOT_INITIALIZED   The library is not initialized.
 * \return #PEAK_AFL_STATUS_INVALID_PARAMETER The argument is invalid.
 * \return #PEAK_AFL_STATUS_NOT_SUPPORTED     The function is not supported.
 * \return #PEAK_AFL_STATUS_ERROR             An unexpected internal error occurred.
 */
PEAK_AFL_API_STATUS peak_afl_AutoController_Hysteresis_GetRange(
    peak_afl_controller_handle controller, uint8_t* min, uint8_t* max, uint8_t* inc);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
