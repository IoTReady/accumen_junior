/*!
* \file    peak_afl.hpp
*
* \author  IDS Imaging Development Systems GmbH
* \date    2023-01-22
* \since   1.1
*
* Copyright (c) 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include "peak_afl/peak_afl.h"
#include "peak_ipl/types/peak_ipl_image.hpp"
#include "peak_ipl/algorithm/peak_ipl_gain.hpp"
#include "peak/node_map/peak_node_map.hpp"

#include <type_traits>
#include <cstdint>
#include <exception>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <memory>
#include <functional>

#if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
#    define PEAK_AFL_NO_DISCARD [[nodiscard]]
#    define PEAK_AFL_MAYBE_UNUSED [[maybe_unused]]
#else
#    ifdef _MSC_VER
#        define PEAK_AFL_NO_DISCARD _Check_return_
#        define PEAK_AFL_MAYBE_UNUSED
#    elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#        define PEAK_AFL_NO_DISCARD __attribute__((warn_unused_result))
#        define PEAK_AFL_MAYBE_UNUSED __attribute__((unused))
#    else
#        define PEAK_AFL_NO_DISCARD
#        define PEAK_AFL_MAYBE_UNUSED
#    endif
#endif

/*! \defgroup cpp C++ API
 *
 * C++ API Implementation
 */


/*! \defgroup cpp_library Library
 *  \ingroup cpp
 *
 * Library level functions and types.
 */

namespace peak
{
namespace afl
{
namespace library
{

/*!
 * \ingroup cpp_library
 * Version information for ids_peak_afl
 */
struct Version_t
{
    std::uint32_t major;    //! Major
    std::uint32_t minor;    //! Minor
    std::uint32_t subminor; //! Subminor
    std::uint32_t patch;    //! Patch

    /*!
     * The String representation for the ids_peak_afl version
     * \returns the string representation
     */
    PEAK_AFL_NO_DISCARD std::string ToString() const
    {
        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(subminor) + "."
            + std::to_string(patch);
    }

    /*!
     * \brief Returns the major part of the version which is the first part of the version scheme separated by dots.
     *
     * \return <b>a</b>.b.c.d
     */
    PEAK_AFL_NO_DISCARD uint32_t Major() const
    {
        return major;
    }

    /*!
     * \brief Returns the minor part of the version which is the second part of the version scheme separated by dots.
     *
     * \return a.<b>b</b>.c.d
     */
    PEAK_AFL_NO_DISCARD uint32_t Minor() const
    {
        return minor;
    }
    /*!
     * \brief Returns the subminor part of the version which is the third part of the version scheme separated by dots.
     *
     * \return a.b.<b>c</b>.d
     */
    PEAK_AFL_NO_DISCARD uint32_t Subminor() const
    {
        return subminor;
    }

    /*!
     * \brief Returns the patch part of the version which is the fourth part of the version scheme separated by dots.
     *
     * \return a.b.c.<b>d</b>
     */
    PEAK_AFL_NO_DISCARD uint32_t Patch() const
    {
        return patch;
    }
};

/*!
 * \ingroup cpp_library
 * \brief Init the peak_afl auto feature library
 *
 * Initializes the internal library status.
 *
 * This function must be called prior to any other function call.\n
 * The function may be called multiple times from a single client process.
 * For each call there must be a corresponding call to #peak::afl::library::Exit
 * to ensure proper deinitialization of the library status.
 *
 * \throws peak::afl::error::Exception if initialization fails
 */
inline void Init();

/*!
 * \ingroup cpp_library
 * \brief Exit the peak_afl auto feature library
 *
 * Deinitializes the internal library status.
 *
 * For each call to #peak::afl::library::Init there must be a corresponding call to this function
 * to ensure proper deinitialization of the library status. \n
 * After the library has been exited its functions (besides #peak::afl::library::Init) will not be operable
 * until #peak::afl::library::Init has been called again.
 *
 * \throws peak::afl::error::Exception if deinitialization fails
 */
inline void Exit();

/*!
 * \ingroup cpp_library
 * \brief Get the last error message
 *
 * \returns the last error as string or the error as string
 *
 */
PEAK_AFL_NO_DISCARD inline std::string GetLastError();

/*!
 * \ingroup cpp_library
 * \brief Query the library version
 *
 * Provides the version of the library divided in major, minor, subminor and patch, in that order of magnitude.
 *
 * \returns The version struct
 *
 * \throws peak::afl::error::Exception if an error occurs. Check code() and what() for an explanation of the error
 *
 * \note This function can be used even if the library is not initialized.
 */
PEAK_AFL_NO_DISCARD inline Version_t Version();
} // namespace library

namespace error
{


/*!
 * \ingroup cpp_library
 * General Error type
 */
class Exception : public std::exception
{
public:
    explicit Exception(peak_afl_status code, bool get_text=true)
        : m_code(code)
    {
        if (get_text)
        {
            m_text = library::GetLastError();
        }
    }

    /*!
     * \brief Get the return code for the failure
     *
     * \returns the return code
     */
    PEAK_AFL_NO_DISCARD peak_afl_status code() const
    {
        return m_code;
    }

    /*!
     * \brief Get the string representation for the exception
     *
     * \returns the string explaining the exception
     */
    PEAK_AFL_NO_DISCARD const char* what() const noexcept override
    {
        if(m_text.empty())
        {
            return "-";
        }
        else
        {
            return m_text.c_str();
        }
    }

    /*!
     *
     * \brief Get a string representation for an error code
     *
     * \param status the status code of an exception
     * \returns the string representation for that code
     */
    PEAK_AFL_NO_DISCARD static std::string translateCode(peak_afl_status status)
    {
        switch(status)
        {
        case PEAK_AFL_STATUS_SUCCESS:
            return "PEAK_AFL_STATUS_SUCCESS";

        case PEAK_AFL_STATUS_ERROR:
            return "PEAK_AFL_STATUS_ERROR";

        case PEAK_AFL_STATUS_NOT_INITIALIZED:
            return "PEAK_AFL_STATUS_NOT_INITIALIZED";

        case PEAK_AFL_STATUS_INVALID_PARAMETER:
            return "PEAK_AFL_STATUS_INVALID_PARAMETER";

        case PEAK_AFL_STATUS_ACCESS_DENIED:
            return "PEAK_AFL_STATUS_ACCESS_DENIED";

        case PEAK_AFL_STATUS_BUSY:
            return "PEAK_AFL_STATUS_BUSY";

        case PEAK_AFL_STATUS_BUFFER_TOO_SMALL:
            return "PEAK_AFL_STATUS_BUFFER_TOO_SMALL";

        case PEAK_AFL_STATUS_INVALID_IMAGE_FORMAT:
            return "PEAK_AFL_STATUS_INVALID_IMAGE_FORMAT";

        case PEAK_AFL_STATUS_NOT_SUPPORTED:
            return "PEAK_AFL_STATUS_NOT_SUPPORTED";

        default:
            return "Unknown Status code";
        }
    }

private:
    peak_afl_status m_code{};
    std::string m_text{};
};
} // namespace error

class Controller;

/*!
 * \ingroup cpp
 * The autofeature manager
 */
class Manager
{
public:
    /*!
     * \brief Create an automanager instance
     *
     * Creates an automanager instance.
     *
     * \param[in]  node_map the shared pointer to the device nodemap
     *
     * \throws peak::afl::error::Exception if an error occurs. Check code() and what() for an explanation of the error
     */
    inline explicit Manager(const std::shared_ptr<peak::core::NodeMap>& node_map);

    /*!
     * \brief Destructor of an automanager instance
     */
    inline ~Manager();

    Manager(const Manager&) = delete;
    inline Manager(Manager&&) noexcept;

    Manager& operator=(const Manager&) noexcept = delete;
    inline Manager& operator=(Manager&&) noexcept;

    /*!
     * \brief Add a controller to an automanager
     *
     * An autocontroller instance can only be added to one autofeature manager at the same time.
     * The same type can only be added one time.
     *
     * \param[in] controller shared_ptr to a controller
     *
     * \throws peak::afl::error::Exception if an error occurs. Check code() and what() for an explanation of the error
     */
    inline void AddController(const std::shared_ptr<Controller>& controller);

    /*!
     * \brief Remove a controller from an automanager
     *
     * Remove a controller from a manager.
     * For a function which destroys the object in one call, see #peak_afl_AutoFeatureManager_DestroyController.
     *
     * \param[in] controller shared_ptr to a controller
     *
     * \throws peak::afl::error::Exception if an error occurs. Check code() and what() for an explanation of the error
     */
    inline void RemoveController(const std::shared_ptr<Controller>& controller);

    /*!
     * \brief Process an image
     *
     * Processes an image.
     *
     * \param[in] image an peak::ipl::Image image
     *
     * \throws peak::afl::error::Exception if an error occurs. Check code() and what() for an explanation of the error
     */
    inline void Process(const peak::ipl::Image& image) const;

    /*!
     * \brief Create a controller and append it to Manager
     *
     * Convenience function to create an autofeature controller and add it to the manager.
     * The same type can only be added one time.
     *
     * \param[in]  type controller type, see #peak_afl_controllerType
     *
     * \returns the shared ptr to the created controller
     *
     * \throws peak::afl::error::Exception if deinitialization fails
     */
    PEAK_AFL_NO_DISCARD inline std::shared_ptr<Controller> CreateController(peak_afl_controllerType type);

    /*!
     * \brief Destroy all controllers for a manager.
     *
     * After this operation all controllers associated with the given manager will be invalid.
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void DestroyAllController();

    /*!
     * \brief Destroy a controller.
     *
     * After this operation the controller will be invalid.
     *
     * \param[in] controller shared ptr to a controller
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void DestroyController(const std::shared_ptr<Controller>& controller);

    /*!
     * \brief Sets the ipl gain.
     *
     * \param[in] gain an peak::ipl::Gain gain
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetGainIPL(const peak::ipl::Gain& gain);

    /*!
     * \brief Get the status of a manager.
     *
     * The returned value will contain whether the manager currently processes an image (true) or is idle (false).
     *
     * \returns manager status
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool Status() const;

    /*!
     * \brief Get the list of the controller associated with the manager
     *
     * \returns controller list
     */
    PEAK_AFL_NO_DISCARD inline std::vector<std::shared_ptr<Controller>> ControllerList() const;

    /*!
     * \brief Get the list count of the controller associated with the manager
     *
     * \returns number of controllers in list
     */
    PEAK_AFL_NO_DISCARD inline size_t ControllerCount() const;

    /*!
     * \brief Get the controller for a type
     *
     * \param[in] type controller type.
     *
     * \returns number of controllers in list
     *
     * \throws peak::afl::error::Exception if no controller is found
     */
    PEAK_AFL_NO_DISCARD inline std::shared_ptr<Controller> GetController(peak_afl_controllerType type) const;

private:
    std::vector<std::shared_ptr<Controller>> m_controllerList;
    mutable std::mutex m_mutex{};
    peak_afl_manager_handle m_handle{};
};

/*!
 * \ingroup cpp
 * \brief Convenience function to create a manager with the supplied controllers
 *
 * \param[in] node_map shared ptr to the node map of the camera
 * \param[in] types    variable argument list of controllers to create
 *
 * \returns two pair tuple with the manager and a vector of the created controllers
 */
template<typename... ControllerTypes>
std::tuple<Manager, std::vector<std::shared_ptr<Controller>>>
CreateManager(const std::shared_ptr<peak::core::NodeMap>& node_map, ControllerTypes... types)
{
    constexpr int size = sizeof...(types);
    constexpr std::array<peak_afl_controllerType, size> controller_types{types...};
    std::vector<std::shared_ptr<Controller>> controller{};
    Manager manager{node_map};
    controller.reserve(size);

    std::for_each(controller_types.cbegin(), controller_types.cend(), [&manager, &controller](peak_afl_controllerType type) {
        controller.emplace_back(std::move(manager.CreateController(type)));
    });

    return { std::move(manager), std::move(controller) };
}


/*!
 * /ingroup cpp
 * Simple range type
 */
template <typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
struct Range
{
    T min;
    T max;
    T inc;
};

namespace callback
{
using FinishedCallback = std::function<void(void)>;
using DataProcessingCallback = std::function<void(int, int)>;
using ComponentCallback = std::function<void(void)>;

using RegisterFunction = std::function<PEAK_AFL_API_STATUS(void*, void*)>;
using UnRegisterFunction = std::function<void()>;
}

/*!
 * \ingroup cpp
 * The Controller class
 */
class Controller : public std::enable_shared_from_this<Controller>
{
    friend class Manager;

    template<class T>
    class Callback
    {
#ifndef SWIG
    public:
        template<typename R, typename ...Args>
        Callback(const std::shared_ptr<peak::afl::Controller>& controller, callback::RegisterFunction register_function, 
            callback::UnRegisterFunction unregister_function,  std::function<R(Args...)> callback) :
            m_callback(std::move(callback)),
            m_unregister_function(std::move(unregister_function))
        {
            const auto ret = register_function(reinterpret_cast<void*>(&Callback::Func<Args...>), this);
            if (ret != PEAK_AFL_STATUS_SUCCESS)
            {
                throw error::Exception(ret);
            }
            m_controller = controller;
        }

        virtual ~Callback()
        {
            if (auto controller = m_controller.lock())
            {
                m_unregister_function();
            }
        }

    private:
        template<typename ...Args>
        static void Func(Args... args, void* ptr)
        {
            static_cast<Callback *>(ptr)->m_callback(args...);
        };
#endif

        std::weak_ptr<peak::afl::Controller> m_controller{};
        callback::UnRegisterFunction m_unregister_function{};
        T m_callback{};
    };

    
    /*!
     * \brief Creates a controller from a handle
     *
     * This is a internal function. See #Controller::Create
     *
     * \param handle the handle to a controller
     */
    inline explicit Controller(peak_afl_controller_handle handle)
        : m_handle(handle) {};

public:
    /*!
     * Destructor for the controller
     */
    inline ~Controller();

    Controller(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller& operator=(Controller&&) = delete;

    /*!
     * \brief Create a new controller
     *
     * \param[in]  type controller type
     *
     * \returns the shared ptr to the created controller
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline static std::shared_ptr<Controller> Create(peak_afl_controllerType type);

    /*!
     * \brief Check if skip frames is supported for a controller.
     *
     * \returns  boolean if controller supports skipping frames
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsSkipFramesSupported() const;

    /*!
     * \brief Set number of frames skipped for a controller.
     *
     * Sets the skipped frames for the controller. Only every N-th image will be processed.
     *
     * \param[in] count      number of frames skipped
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetSkipFrames(std::uint32_t count);

    /*!
     * \brief Get number of frames skipped for a controller.
     *
     * Gets the skipped frames for the controller. Only every N-th image will be processed.
     *
     * \returns  number of frames skipped
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::uint32_t GetSkipFrames() const;

    /*!
     * \brief Get range for frames skipped for a controller.
     *
     * \returns valid peak::afl::Range for frames skipped
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline Range<std::uint32_t> GetSkipFramesRange() const;

    /*!
     * \brief Check if setting a region of interest is supported for a controller.
     *
     * If true region of interest is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports setting a region of interest
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsROISupported() const;

    /*!
     * \brief Set the autofeature region of interest for a controller.
     *
     * Set the region of interest for a controller. The processed image will be cropped to the region of interest set.
     *
     * \param[in] rect        region of interest
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetROI(peak_afl_rectangle rect);

    /*!
     * \brief Get the autofeature region of interest for a controller.
     *
     * Get the region of interest for a controller. The processed image will be cropped to the region of interest set.
     *
     * \returns region of interest
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_rectangle GetROI() const;

    /*!
     * \brief Check if ROI preset is supported for a controller.
     *
     * true if ROI preset is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports ROI preset
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsROIPresetSupported() const;

    /*!
     * \brief Set the autofeature region of interest preset for a controller.
     *
     * Will set the supplied preset as region of interest. See also #SetROI.
     *
     * \param[in] preset  region of interest preset
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetROIPreset(peak_afl_roi_preset preset);

    /*!
     * \brief Check if auto mode is supported for a controller.
     *
     * true if auto mode is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports auto mode
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsModeSupported() const;

    /*!
     * \brief Set the autofeature mode for a controller.
     *
     * Will set the controller mode to mode. See #peak_afl_controller_automode for a list of valid values.
     *
     * \param[in] mode       autofeature mode
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetMode(peak_afl_controller_automode mode);

    /*!
     * \brief Get the current autofeature mode for a controller.
     *
     * \returns autofeature mode
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_automode GetMode() const;

    /*!
     * \brief Get the status for a controller.
     *
     * See #peak_afl_controller_status for a list of values.
     *
     * \returns controller status
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_status Status() const;

    /*!
     * \brief Get the last auto average for a controller.
     *
     * Used by Controllers processing a mono image.
     *
     * \returns autofeature average
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::uint8_t GetLastAutoAverage() const;

    /*!
     * \brief Get the last auto average for a controller.
     *
     * Used by Controllers processing a color image.
     *
     * \returns std::tuple<red, green, blue> in that order
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> GetLastAutoAverages() const;

    /*!
     * \brief check if auto target is supported for a controller.
     *
     * True if auto target is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports auto target
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsAutoTargetSupported() const;

    /*!
     * \brief Set the auto target for a controller.
     *
     * Set an auto target. Call #peak_afl_AutoController_AutoTarget_GetRange to get the valid range.
     * End value which will be targeted by the controller.
     *
     * \param[in] target     auto target value
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetAutoTarget(uint32_t target);

    /*!
     * \brief Get the currently set auto target for a controller.
     *
     * \returns auto target value
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::uint32_t GetAutoTarget() const;

    /*!
     * \brief Get the auto target range for a controller.
     *
     * Call this function to get the range of valid values which can be set by a call to
     * #SetAutoTarget.
     *
     * \returns peak::afl::Range with valid values
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline Range<std::uint32_t> GetAutoTargetRange() const;

    /*!
     * \brief check if auto tolerance is supported for a controller.
     *
     * True if auto tolerance is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports auto tolerance
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsAutoToleranceSupported() const;

    /*!
     * \brief Set the auto tolerance for a controller.
     *
     * Sets the +/- tolerance for the auto target value.
     *
     * \param[in] tolerance  auto target value
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetAutoTolerance(std::uint32_t tolerance);

    /*!
     * \brief Get the current auto tolerance for a controller.
     *
     * \returns auto tolerance value
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::uint32_t GetAutoTolerance() const;

    /*!
     * \brief Get the auto tolerance range for a controller.
     *
     * Call this function to get the range of valid values which can be set by a call to
     * #SetAutoTolerance.
     *
     * \returns peak::afl::Range with valid values
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline Range<std::uint32_t> GetAutoToleranceRange() const;

    /*!
     * \brief check if auto percentile is supported for a controller.
     *
     * True if auto percentile is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports auto percentile
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsAutoPercentileSupported() const;

    /*!
     * \brief Set the auto percentile for a controller.
     *
     * This is the used percentile value for a controller.
     * To get the valid range, call #peak_afl_AutoController_AutoPercentile_GetRange.
     *
     * \param[in] percentile auto percentile value
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetAutoPercentile(double percentile);

    /*!
     * \brief Get the auto percentile for a controller.
     *
     * \returns auto percentile value
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline double GetAutoPercentile() const;

    /*!
     * \brief Get the auto percentile range for a controller.
     *
     * Call this function to get the range of valid values which can be set by a call to
     * #SetAutoPercentile.
     *
     * \returns peak::afl::Range with valid values
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline Range<double> GetAutoPercentileRange() const;

    /*!
     * \brief Get the controller type.
     *
     * See #peak_afl_controllerType for a list of values.
     *
     * \returns auto controller type
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controllerType Type() const;

    /*!
     * \brief check if setting an algorithm is supported for a controller.
     *
     * True if algorithm is supported, otherwise it is unsupported.
     *
     * Call #GetAlgorithmList to get a list of supported algorithms.
     *
     * \returns boolean if controller supports setting an algorithm
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsAlgorithmSupported() const;

    /*!
     * \brief Set the used algorithm for a controller.
     *
     * To get a list of supported algorithms see #peak_afl_AutoController_Algorithm_GetList.
     *
     * \param[in] algorithm auto controller algorithm
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetAlgorithm(peak_afl_controller_algorithm algorithm);

    /*!
     * \brief Get the used algorithm for a controller.
     *
     * \returns auto controller algorithm
     *
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_algorithm GetAlgorithm() const;

    /*!
     * \brief Get the list of supported algorithms for a controller.
     *
     * To set a value, see #SetAlgorithm.
     *
     * \returns vector of supported algorithms
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::vector<peak_afl_controller_algorithm> GetAlgorithmList() const;

    /*!
     * \brief check setting a sharpness algorithm is supported by a controller.
     *
     * True if sharpness algorithm is supported, otherwise it is unsupported.
     *
     * Call #GetSharpnessAlgorithmList to get a list of supported sharpness algorithms.
     *
     * \returns boolean if controller supports setting a sharpness algorithm
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsSharpnessAlgorithmSupported() const;

    /*!
     * \brief Set the used sharpness algorithm for a controller.
     *
     * To get a list of supported algorithms see #peak_afl_AutoController_SharpnessAlgorithm_GetList.
     *
     * \param[in] algorithm  auto controller sharpness algorithm
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetSharpnessAlgorithm(peak_afl_controller_sharpness_algorithm algorithm);

    /*!
     * \brief Get the used sharpness algorithm for a controller.
     *
     * \returns auto controller sharpness algorithm
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_sharpness_algorithm GetSharpnessAlgorithm() const;

    /*!
     * \brief Get the list of supported sharpness algorithms for a controller.
     *
     * To set a value, see #SetSharpnessAlgorithm.
     *
     * \returns vector of supported algorithms
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::vector<peak_afl_controller_sharpness_algorithm> GetSharpnessAlgorithmList() const;

    /*!
     * \brief Set finished callback for a controller.
     */
    inline void RegisterFinishedCallback(const callback::FinishedCallback& callback);

    /*!
     * \brief Reset the finished callback for a controller.
     */
    inline void UnRegisterFinishedCallback();

    /*!
     * \brief Set component callback for a controller.
     */
    inline void RegisterComponentCallback(peak_afl_controller_brightness_component component, const callback::FinishedCallback& callback);

    /*!
     * \brief Reset the component callback for a controller.
     */
    inline void UnRegisterComponentCallback(peak_afl_controller_brightness_component component);

    /*!
     * \brief Set the data callback for a controller.
     *
     */
    inline void RegisterDataProcessingCallback(const callback::DataProcessingCallback& callback);

    /*!
     * \brief Reset the finished callback for a controller.
     */
    inline void UnRegisterDataProcessingCallback();

    /*!
     * \brief Check if weighted region of interest is supported for a controller.
     *
     * True if weighted region of interest is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports weighted roi
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsWeightedROISupported() const;

    /*!
     * \brief Set the autofeature weighted region of interest for a controller.
     *
     * Already set weighted regions of interest will be overwritten
     *
     * \param[in] list vector of weighted region of interest
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetWeightedROIs(const std::vector<peak_afl_weighted_rectangle>& list);

    /*!
     * \brief Set single autofeature weighted region of interest for a controller.
     *
     * Already set weighted regions of interest will be overwritten
     *
     * \param[in] rect weighted region of interest
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetWeightedROI(const peak_afl_weighted_rectangle& rect);

    /*!
     * \brief Get the autofeature minimum size of weighted region of interest for a controller.
     *
     * \returns the minimum size
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_size GetWeightedROIMinSize() const;

    /*!
     * \brief Get the autofeature weighted region of interest for a controller.
     *
     * \returns vector with set #peak_afl_weighted_rectangle
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::vector<peak_afl_weighted_rectangle> GetWeightedROIs() const;

    /*!
     * \brief Check if limit is supported for a controller.
     *
     * True if limit is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports limit
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsLimitSupported() const;

    /*!
     * \brief Set the autofeature limit for a controller.
     *
     * Sets the minimum and maximum limit for the algorithm set by #peak_afl_AutoController_Algorithm_Set
     *
     * \param[in] limit      the limit to set
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetLimit(peak_afl_controller_limit limit);

    /*!
     * \brief Get the autofeature limit for a controller.
     *
     * \returns the limit set
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_limit GetLimit() const;

    /*!
     * \brief Get the autofeature default limit.
     *
     * \returns the default limit set
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_limit GetDefaultLimit() const;

    /*!
     * \brief Check if hysteresis is supported for a controller.
     *
     * True if hysteresis is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports hysteresis
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsHysteresisSupported() const;

    /*!
     * \brief Set the autofeature hysteresis for a controller.
     *
     * Set the hysteresis for the algorithm set by #SetAlgorithm
     *
     * \param[in] hysteresis the hysteresis to set
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void SetHysteresis(std::uint8_t hysteresis);

    /*!
     * \brief Get the autofeature hysteresis for a controller.
     *
     * \returns the hysteresis set
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::uint8_t GetHysteresis() const;

    /*!
     * \brief Get the autofeature hysteresis default.
     *
     * \returns the default hysteresis
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline std::uint8_t GetDefaultHysteresis() const;

    /*!
     * \brief Get autofeature hysteresis range for a controller.
     *
     * \returns the peak::afl::Range of the hysteresis
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline Range<std::uint8_t> GetHysteresisRange() const;

    /*!
     * \brief Check if auto mode is supported for a brightness controller component.
     *
     * true if auto mode is supported, otherwise it is unsupported.
     *
     * \returns boolean if controller supports auto mode
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline bool IsBrightnessComponentModeSupported() const;

    /*!
     * \brief Set the autofeature mode for a brightness controller component.
     *
     * Will set the controller mode to mode. See #peak_afl_controller_automode for a list of valid values.
     *
     * \param[in] component  autofeature brightness component
     * \param[in] mode       autofeature mode
     *
     * \throws peak::afl::error::Exception if function fails
     */
    inline void BrightnessComponentSetMode(peak_afl_controller_brightness_component component, peak_afl_controller_automode mode);

    /*!
     * \brief Get the current autofeature mode for a brightness controller component.
     *
     * \param[in] component autofeature brightness component
     * \returns autofeature mode
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_automode BrightnessComponentGetMode(peak_afl_controller_brightness_component component) const;

    /*!
     * \brief Get the status for a brightness controller component.
     *
     * See #peak_afl_controller_status for a list of values.
     *
     * \param[in] component autofeature brightness component
     * \returns controller status
     *
     * \throws peak::afl::error::Exception if function fails
     */
    PEAK_AFL_NO_DISCARD inline peak_afl_controller_status BrightnessComponentStatus(peak_afl_controller_brightness_component component) const;

private:
    using FinishedCallbackType = Callback<callback::FinishedCallback>;
    using DataProcessingCallbackType = Callback<callback::DataProcessingCallback>;
    using ComponentCallbackType = Callback<callback::ComponentCallback>;

    peak_afl_controller_handle m_handle{};
    std::unique_ptr<FinishedCallbackType> m_finishedCallback{};
    std::unique_ptr<DataProcessingCallbackType> m_dataProcessingCallback{};
    std::unique_ptr<ComponentCallbackType> m_finishedExposureCallback{};
    std::unique_ptr<ComponentCallbackType> m_finishedGainCallback{};
};

namespace library
{
inline void Init()
{
    const auto ret = peak_afl_Init();
    if (ret != PEAK_AFL_STATUS_SUCCESS)

    {
        throw error::Exception(ret, false);
    }
}

inline void Exit()
{
    const auto ret = peak_afl_Exit();
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret, false);
    }
}

inline Version_t Version()
{
    Version_t version{};

    const auto ret = peak_afl_GetVersion(&version.major, &version.minor, &version.subminor, &version.patch);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret, false);
    }

    return version;
}

inline std::string GetLastError()
{
    std::size_t lastErrorMessageSize;
    peak_afl_status code;

    auto ret = peak_afl_GetLastError(&code, nullptr, &lastErrorMessageSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        return "Cannot get last error!";
    }

    std::vector<char> text;
    text.resize(lastErrorMessageSize);

    ret = peak_afl_GetLastError(&code, text.data(), &lastErrorMessageSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        return "Cannot get last error!";
    }

    return { text.begin(), text.end() };
}
} // namespace library

inline Manager::Manager(const std::shared_ptr<peak::core::NodeMap>& node_map)
{
    if(!node_map)
    {
        throw std::runtime_error("Invalid node map handle!");
    }

    const auto ret = peak_afl_AutoFeatureManager_Create(&m_handle, node_map->Handle());
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline Manager::~Manager()
{
    if (m_handle != nullptr)
    {
        peak_afl_AutoFeatureManager_DestroyAllController(m_handle);

        {
            std::lock_guard<std::mutex> lck(m_mutex);
            for (auto& c : m_controllerList)
            {
                c->m_handle = nullptr;
            }
        }

        peak_afl_AutoFeatureManager_Destroy(m_handle);
        m_handle = nullptr;
    }
}

inline Manager::Manager(Manager&& other) noexcept {
    m_handle = other.m_handle;
    other.m_handle = nullptr;

    std::lock_guard<std::mutex> lck(other.m_mutex);
    m_controllerList = std::move(other.m_controllerList);
}

inline Manager& Manager::operator=(Manager&& other) noexcept
{
    m_handle = other.m_handle;
    other.m_handle = nullptr;

    std::lock_guard<std::mutex> lck(other.m_mutex);
    m_controllerList = std::move(other.m_controllerList);

    return *this;
}

inline void Manager::AddController(const std::shared_ptr<Controller>& controller)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    auto cnt = std::any_of(m_controllerList.begin(), m_controllerList.end(), [controller](const std::shared_ptr<Controller>& c) {
        return c == controller;
    });

    if(cnt)
    {
        return;
    }

    const auto ret = peak_afl_AutoFeatureManager_AddController(m_handle, controller->m_handle);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    m_controllerList.emplace_back(controller);
}

inline void Manager::RemoveController(const std::shared_ptr<Controller>& controller)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    const auto ret = peak_afl_AutoFeatureManager_RemoveController(m_handle, controller->m_handle);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    auto it = std::remove_if(m_controllerList.begin(), m_controllerList.end(), [controller](const std::shared_ptr<Controller>& c) {
        return c == controller;
    });

    m_controllerList.erase(it, m_controllerList.end());
}

inline void Manager::Process(const peak::ipl::Image& image) const
{
    const auto ret = peak_afl_AutoFeatureManager_Process(m_handle, ImageBackendAccessor::BackendHandle(image));
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline std::shared_ptr<Controller> Manager::CreateController(peak_afl_controllerType type)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    peak_afl_controller_handle handle{};
    const auto ret = peak_afl_AutoFeatureManager_CreateController(m_handle, &handle, type);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    auto shared = std::shared_ptr<Controller>(new Controller(handle));
    m_controllerList.emplace_back(shared);

    return shared;
}

inline void Manager::DestroyAllController()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    const auto ret = peak_afl_AutoFeatureManager_DestroyAllController(m_handle);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    for(auto& c : m_controllerList)
    {
        c->m_handle = nullptr;
    }

    m_controllerList.clear();
}

inline void Manager::DestroyController(const std::shared_ptr<Controller>& controller)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    const auto ret = peak_afl_AutoFeatureManager_DestroyController(m_handle, controller->m_handle);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    auto it = std::remove_if(m_controllerList.begin(), m_controllerList.end(), [controller](const std::shared_ptr<Controller>& c) {
        return c == controller;
    });

    m_controllerList.erase(it, m_controllerList.end());

    controller->m_handle = nullptr;
}

inline void Manager::SetGainIPL(const peak::ipl::Gain& gain)
{
    const auto ret = peak_afl_AutoFeatureManager_SetGainIPL(m_handle, GainBackendAccessor::BackendHandle(gain));
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline bool Manager::Status() const
{
    std::uint8_t status;
    const auto ret = peak_afl_AutoFeatureManager_Status(m_handle, &status);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return status;
}

inline std::vector<std::shared_ptr<Controller>> Manager::ControllerList() const
{
    std::lock_guard<std::mutex> lck(m_mutex);

    std::vector<std::shared_ptr<Controller>> controller{};

    std::copy(m_controllerList.begin(), m_controllerList.end(), std::back_inserter(controller));

    return controller;
}

inline size_t Manager::ControllerCount() const
{
    return m_controllerList.size();
}

inline std::shared_ptr<Controller> Manager::GetController(peak_afl_controllerType type) const
{
    std::lock_guard<std::mutex> lck(m_mutex);

    auto it = std::find_if(m_controllerList.cbegin(), m_controllerList.cend(), [=](const std::shared_ptr<Controller>& controller) {
        return type == controller->Type();
    });

    if(it != m_controllerList.cend())
    {
        return *it;
    }
    else
    {
        throw error::Exception(PEAK_AFL_STATUS_ERROR, false);
    }
}

inline std::shared_ptr<Controller> Controller::Create(peak_afl_controllerType type)
{
    peak_afl_controller_handle handle{};
    const auto ret = peak_afl_AutoController_Create(&handle, type);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return std::shared_ptr<Controller>(new Controller(handle));
}

inline Controller::~Controller()
{
    if (m_handle != nullptr)
    {
        peak_afl_AutoController_Destroy(m_handle);
        m_handle = nullptr;
    }
}

inline bool Controller::IsSkipFramesSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_SkipFrames_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetSkipFrames(std::uint32_t count)
{
    const auto ret = peak_afl_AutoController_SkipFrames_Set(m_handle, count);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline std::uint32_t Controller::GetSkipFrames() const
{
    std::uint32_t count{};
    const auto ret = peak_afl_AutoController_SkipFrames_Get(m_handle, &count);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return count;
}

inline Range<std::uint32_t> Controller::GetSkipFramesRange() const
{
    Range<std::uint32_t> range{};
    const auto ret = peak_afl_AutoController_SkipFrames_GetRange(m_handle, &range.min, &range.max, &range.inc);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return range;
}

inline bool Controller::IsROISupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_ROI_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetROI(peak_afl_rectangle rect)
{
    const auto ret = peak_afl_AutoController_ROI_Set(m_handle, rect);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline peak_afl_rectangle Controller::GetROI() const
{
    peak_afl_rectangle rect{};
    const auto ret = peak_afl_AutoController_ROI_Get(m_handle, &rect);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return rect;
}

inline bool Controller::IsROIPresetSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_ROI_Preset_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetROIPreset(peak_afl_roi_preset preset)
{
    const auto ret = peak_afl_AutoController_ROI_Preset_Set(m_handle, preset);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline bool Controller::IsModeSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_Mode_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetMode(peak_afl_controller_automode mode)
{
    const auto ret = peak_afl_AutoController_Mode_Set(m_handle, mode);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline peak_afl_controller_automode Controller::GetMode() const
{
    peak_afl_controller_automode mode{};
    const auto ret = peak_afl_AutoController_Mode_Get(m_handle, &mode);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return mode;
}

inline peak_afl_controller_status Controller::Status() const
{
    peak_afl_controller_status status{};
    const auto ret = peak_afl_AutoController_Status(m_handle, &status);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return status;
}


inline std::uint8_t Controller::GetLastAutoAverage() const
{
    std::uint8_t average{};
    const auto ret = peak_afl_AutoController_GetLastAutoAverage(m_handle, &average);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return average;
}

inline std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> Controller::GetLastAutoAverages() const
{
    std::uint8_t averageRed{}, averageGreen{}, averageBlue{};
    const auto ret = peak_afl_AutoController_GetLastAutoAverages(m_handle, &averageRed, &averageGreen, &averageBlue);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return { averageRed, averageGreen, averageBlue };
}

inline bool Controller::IsAutoTargetSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_AutoTarget_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetAutoTarget(std::uint32_t target)
{
    const auto ret = peak_afl_AutoController_AutoTarget_Set(m_handle, target);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline std::uint32_t Controller::GetAutoTarget() const
{
    std::uint32_t target{};
    const auto ret = peak_afl_AutoController_AutoTarget_Get(m_handle, &target);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return target;
}

inline Range<std::uint32_t> Controller::GetAutoTargetRange() const
{
    Range<std::uint32_t> range{};
    const auto ret = peak_afl_AutoController_AutoTarget_GetRange(m_handle, &range.min, &range.max, &range.inc);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return range;
}

inline bool Controller::IsAutoToleranceSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_AutoTolerance_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetAutoTolerance(uint32_t tolerance)
{
    const auto ret = peak_afl_AutoController_AutoTolerance_Set(m_handle, tolerance);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline uint32_t Controller::GetAutoTolerance() const
{
    std::uint32_t tolerance;
    const auto ret = peak_afl_AutoController_AutoTolerance_Get(m_handle, &tolerance);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return tolerance;
}

inline Range<uint32_t> Controller::GetAutoToleranceRange() const
{
    Range<uint32_t> range{};
    const auto ret = peak_afl_AutoController_AutoTolerance_GetRange(m_handle, &range.min, &range.max, &range.inc);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return range;
}

inline bool Controller::IsAutoPercentileSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_AutoPercentile_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetAutoPercentile(double percentile)
{
    const auto ret = peak_afl_AutoController_AutoPercentile_Set(m_handle, percentile);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline double Controller::GetAutoPercentile() const
{
    double percentile;
    const auto ret = peak_afl_AutoController_AutoPercentile_Get(m_handle, &percentile);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return percentile;
}

inline Range<double> Controller::GetAutoPercentileRange() const
{
    Range<double> range{};
    const auto ret = peak_afl_AutoController_AutoPercentile_GetRange(m_handle, &range.min, &range.max, &range.inc);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return range;
}

inline peak_afl_controllerType Controller::Type() const
{
    peak_afl_controllerType type{};
    const auto ret = peak_afl_AutoController_Type_Get(m_handle, &type);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return type;
}

inline bool Controller::IsAlgorithmSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_Algorithm_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetAlgorithm(peak_afl_controller_algorithm alg)
{
    const auto ret = peak_afl_AutoController_Algorithm_Set(m_handle, alg);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline peak_afl_controller_algorithm Controller::GetAlgorithm() const
{
    peak_afl_controller_algorithm alg{};
    const auto ret = peak_afl_AutoController_Algorithm_Get(m_handle, &alg);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return alg;
}

inline std::vector<peak_afl_controller_algorithm> Controller::GetAlgorithmList() const
{
    std::vector<peak_afl_controller_algorithm> algList{};
    std::uint32_t listSize{};

    auto ret = peak_afl_AutoController_Algorithm_GetList(m_handle, nullptr, &listSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    algList.resize(listSize);

    ret = peak_afl_AutoController_Algorithm_GetList(m_handle, algList.data(), &listSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return algList;
}

inline bool Controller::IsSharpnessAlgorithmSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_SharpnessAlgorithm_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetSharpnessAlgorithm(peak_afl_controller_sharpness_algorithm alg)
{
    const auto ret = peak_afl_AutoController_SharpnessAlgorithm_Set(m_handle, alg);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline peak_afl_controller_sharpness_algorithm Controller::GetSharpnessAlgorithm() const
{
    peak_afl_controller_sharpness_algorithm alg{};
    const auto ret = peak_afl_AutoController_SharpnessAlgorithm_Get(m_handle, &alg);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return alg;
}

inline std::vector<peak_afl_controller_sharpness_algorithm> Controller::GetSharpnessAlgorithmList() const
{
    std::vector<peak_afl_controller_sharpness_algorithm> algList{};
    std::uint32_t listSize{};

    auto ret = peak_afl_AutoController_SharpnessAlgorithm_GetList(m_handle, nullptr, &listSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    algList.resize(listSize);

    ret = peak_afl_AutoController_SharpnessAlgorithm_GetList(m_handle, algList.data(), &listSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return algList;
}

inline void Controller::RegisterFinishedCallback(const callback::FinishedCallback& callback)
{
    m_finishedCallback = std::make_unique<FinishedCallbackType>(shared_from_this(), 
        [handle = m_handle](void* ptr, void* ctx) {
            return peak_afl_AutoController_Callback_Set(handle, PEAK_AFL_CONTROLLER_CALLBACK_FINISHED, ptr, ctx); },
        [handle = m_handle] { 
            peak_afl_AutoController_Callback_Set(handle, PEAK_AFL_CONTROLLER_CALLBACK_FINISHED, nullptr, nullptr); },
         callback);
}

inline void Controller::UnRegisterFinishedCallback()
{
    m_finishedCallback.reset();
}

inline void Controller::RegisterComponentCallback(peak_afl_controller_brightness_component component, const callback::FinishedCallback& callback)
{
    if (!IsBrightnessComponentModeSupported())
    {
        throw error::Exception(PEAK_AFL_STATUS_NOT_SUPPORTED);
    }

    if (PEAK_AFL_CONTROLLER_BRIGHTNESS_COMPONENT_EXPOSURE == component)
    {
        m_finishedExposureCallback = std::make_unique<ComponentCallbackType>(shared_from_this(),
            [handle = m_handle, component](void* ptr, void* ctx) {
                return peak_afl_AutoController_BrightnessComponent_Callback_Set(handle, component, PEAK_AFL_CONTROLLER_CALLBACK_FINISHED,
                    ptr, ctx); },
            [handle = m_handle, component] {
                peak_afl_AutoController_BrightnessComponent_Callback_Set(handle, component, PEAK_AFL_CONTROLLER_CALLBACK_FINISHED,
                    nullptr, nullptr); },
            callback);
    }
    else if (PEAK_AFL_CONTROLLER_BRIGHTNESS_COMPONENT_GAIN == component)
    {
        m_finishedGainCallback = std::make_unique<ComponentCallbackType>(shared_from_this(),
            [handle = m_handle, component](void* ptr, void* ctx) {
                return peak_afl_AutoController_BrightnessComponent_Callback_Set(handle, component, PEAK_AFL_CONTROLLER_CALLBACK_FINISHED,
                    ptr, ctx); },
            [handle = m_handle, component] {
                peak_afl_AutoController_BrightnessComponent_Callback_Set(handle, component, PEAK_AFL_CONTROLLER_CALLBACK_FINISHED,
                    nullptr, nullptr); },
            callback);
    }
    else
    {
        throw error::Exception(PEAK_AFL_STATUS_INVALID_PARAMETER);
    }
}

inline void Controller::UnRegisterComponentCallback(peak_afl_controller_brightness_component component)
{
    if (PEAK_AFL_CONTROLLER_BRIGHTNESS_COMPONENT_EXPOSURE == component)
    {
        m_finishedExposureCallback.reset();
    }
    else if (PEAK_AFL_CONTROLLER_BRIGHTNESS_COMPONENT_GAIN == component)
    {
        m_finishedGainCallback.reset();
    }
    else
    {
        throw error::Exception(PEAK_AFL_STATUS_INVALID_PARAMETER);
    }
}

inline void Controller::RegisterDataProcessingCallback(const callback::DataProcessingCallback& callback)
{
    m_dataProcessingCallback = std::make_unique<DataProcessingCallbackType>(shared_from_this(), 
        [handle = m_handle](void* ptr, void* ctx) {
            return peak_afl_AutoController_Callback_Set(handle, PEAK_AFL_CONTROLLER_CALLBACK_PROCESSING_DATA, ptr, ctx); },
        [handle = m_handle] { 
            peak_afl_AutoController_Callback_Set(handle, PEAK_AFL_CONTROLLER_CALLBACK_PROCESSING_DATA, nullptr, nullptr);},
         callback);
}

inline void Controller::UnRegisterDataProcessingCallback()
{
    m_dataProcessingCallback.reset();
}

inline bool Controller::IsWeightedROISupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_Weighted_ROI_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetWeightedROIs(const std::vector<peak_afl_weighted_rectangle>& list)
{
    const auto ret = peak_afl_AutoController_Weighted_ROI_Set(m_handle, list.data(), static_cast<uint32_t>(list.size()));
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline void Controller::SetWeightedROI(const peak_afl_weighted_rectangle& rect)
{
    const auto ret = peak_afl_AutoController_Weighted_ROI_Set(m_handle, &rect, 1);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline peak_afl_size Controller::GetWeightedROIMinSize() const
{
    peak_afl_size minSize{};
    const auto ret = peak_afl_AutoController_Weighted_ROI_Min_Size(m_handle, &minSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return minSize;
}

inline std::vector<peak_afl_weighted_rectangle> Controller::GetWeightedROIs() const
{
    std::vector<peak_afl_weighted_rectangle> list;
    std::uint32_t listSize{};

    auto ret = peak_afl_AutoController_Weighted_ROI_Get(m_handle, nullptr, &listSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    list.resize(listSize);

    ret = peak_afl_AutoController_Weighted_ROI_Get(m_handle, list.data(), &listSize);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return list;
}

inline bool Controller::IsLimitSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_Limit_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetLimit(peak_afl_controller_limit limit)
{
    const auto ret = peak_afl_AutoController_Limit_Set(m_handle, limit);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline peak_afl_controller_limit Controller::GetLimit() const
{
    peak_afl_controller_limit limit{};
    const auto ret = peak_afl_AutoController_Limit_Get(m_handle, &limit);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return limit;
}

inline peak_afl_controller_limit Controller::GetDefaultLimit() const
{
    peak_afl_controller_limit limit{};
    const auto ret = peak_afl_AutoController_Limit_Default(m_handle, &limit);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return limit;
}

inline bool Controller::IsHysteresisSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_Hysteresis_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::SetHysteresis(std::uint8_t hysteresis)
{
    const auto ret = peak_afl_AutoController_Hysteresis_Set(m_handle, hysteresis);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

inline std::uint8_t Controller::GetHysteresis() const
{
    std::uint8_t hysteresis{};
    const auto ret = peak_afl_AutoController_Hysteresis_Get(m_handle, &hysteresis);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return hysteresis;
}

inline std::uint8_t Controller::GetDefaultHysteresis() const
{
    std::uint8_t hysteresis{};
    const auto ret = peak_afl_AutoController_Hysteresis_Default(m_handle, &hysteresis);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return hysteresis;
}

inline Range<std::uint8_t> Controller::GetHysteresisRange() const
{
    Range<std::uint8_t> hysteresis{};
    const auto ret = peak_afl_AutoController_Hysteresis_GetRange(
        m_handle, &hysteresis.min, &hysteresis.max, &hysteresis.inc);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return hysteresis;
}

PEAK_AFL_NO_DISCARD inline bool Controller::IsBrightnessComponentModeSupported() const
{
    peak_afl_BOOL8 supported{};
    const auto ret = peak_afl_AutoController_BrightnessComponent_Mode_IsSupported(m_handle, &supported);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }

    return supported;
}

inline void Controller::BrightnessComponentSetMode(
    peak_afl_controller_brightness_component component, peak_afl_controller_automode mode)
{
    const auto ret = peak_afl_AutoController_BrightnessComponent_Mode_Set(m_handle, component, mode);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
}

PEAK_AFL_NO_DISCARD inline peak_afl_controller_automode Controller::BrightnessComponentGetMode(
    peak_afl_controller_brightness_component component) const
{
    peak_afl_controller_automode mode{};
    const auto ret = peak_afl_AutoController_BrightnessComponent_Mode_Get(m_handle, component, &mode);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return mode;
}

PEAK_AFL_NO_DISCARD inline peak_afl_controller_status Controller::BrightnessComponentStatus(
    peak_afl_controller_brightness_component component) const
{
    peak_afl_controller_status status{};
    const auto ret = peak_afl_AutoController_BrightnessComponent_Status(m_handle, component, &status);
    if (ret != PEAK_AFL_STATUS_SUCCESS)
    {
        throw error::Exception(ret);
    }
    return status;
}
} /* namespace afl */
} /* namespace peak */
