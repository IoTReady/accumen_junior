/*!
 * \file    peak_ipl_color_corrector.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.2.2
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_image.hpp>
#include <peak_ipl/types/peak_ipl_pixel_format.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>

namespace
{
class GainBackendAccessor;
}

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 * \brief Applies gain to the data.
 */
class Gain final
{
public:
    /*! \brief Constructor for a new Gain instance
     *
     * Creates a new Gain instance.
     *
     * \since 1.2.2
     */
    Gain();
    
    /*! \brief Destructor for the Gain instance
     *
     * Destroys a Gain instance.
     *
     * \since 1.2.2
     */
    ~Gain();

    /*! \brief Copy Constructor for a new Gain instance
     *
     * Creates a new Gain instance.
     *
     * \note This only creates a shallow copy. See \ref shallowCopy for a detailed explanation.
     *
     * \param[in] other The other instance to acquire a reference for.
     * 
     * \since 1.2.2 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    Gain(const Gain& other);

    /*! \brief Copy assigment for Gain instance
     *
     * Acquires a reference from \p other to this instance. If this instance already has a reference, it will be destroyed.
     *
     * \note See \ref shallowCopy for a detailed explanation.
     * 
     * \param[in] other The other instance to acquire a reference for.
     *
     * \since 1.2.2 Deleted
     * \since 1.8   Implemented as shallow copy
     */
    Gain& operator=(const Gain& other);

    /*! \brief Move constructor for Gain instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.2.2
     */
    Gain(Gain&& other) noexcept;

    /*! \brief Move assignment for Gain instance
     *
     * Moves the instance from \p other to this. Afterwards \p other is invalid.
     * 
     * \param[in] other The other instance to move.
     *
     * \since 1.2.2
     */
    Gain& operator=(Gain&& other) noexcept;

    /*!
     * \brief Sets the values of the master Gain.
     *
     * \param[in] masterGainValue The value of the master gain.
     *
     * \throws InvalidArgument masterGainValue isn't supported.
     *
     * \since 1.3.2
     */
    void SetMasterGainValue(float masterGainValue);

    /*!
     * \brief Returns the value of the master gain.
     *
     * \returns masterGainValue The value of the master gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float MasterGainValue() const;

    /*!
     * \brief Returns the minimum value of the master gain.
     *
     * \returns masterGainMin The minimum value of the master gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float MasterGainMin() const;

    /*!
     * \brief Returns the maximum value of the master gain.
     *
     * \returns masterGainMax The maximum value of the master gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float MasterGainMax() const;

    /*!
     * \brief Sets the values of the red Gain.
     *
     * \param[in] redGainValue The value of the red gain.
     *
     * \throws InvalidArgument redGainValue isn't supported.
     *
     * \since 1.3.2
     */
    void SetRedGainValue(float redGainValue);

    /*!
     * \brief Returns the value of the red gain.
     *
     * \returns redGainValue The value of the red gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float RedGainValue() const;

    /*!
     * \brief Returns the minimum value of the red gain.
     *
     * \returns redGainMin The minimum value of the red gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float RedGainMin() const;

    /*!
     * \brief Returns the maximum value of the red gain.
     *
     * \returns redGainMax The maximum value of the red gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float RedGainMax() const;

    /*!
     * \brief Sets the values of the green Gain.
     *
     * \param[in] greenGainValue The value of the green gain.
     *
     * \throws InvalidArgument greenGainValue isn't supported.
     *
     * \since 1.3.2
     */
    void SetGreenGainValue(float greenGainValue);

    /*!
     * \brief Returns the value of the green gain.
     *
     * \returns greenGainValue The value of the green gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float GreenGainValue() const;

    /*!
     * \brief Returns the minimum value of the green gain.
     *
     * \returns greenGainMin The minimum value of the green gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float GreenGainMin() const;

    /*!
     * \brief Returns the maximum value of the green gain.
     *
     * \returns greenGainMax The maximum value of the green gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float GreenGainMax() const;

    /*!
     * \brief Sets the values of the blue Gain.
     *
     * \param[in] blueGainValue The value of the blue gain.
     *
     * \throws InvalidArgument blueGainValue isn't supported.
     *
     * \since 1.3.2
     */
    void SetBlueGainValue(float blueGainValue);

    /*!
     * \brief Returns the value of the blue gain.
     *
     * \returns blueGainValue The value of the blue gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float BlueGainValue() const;

    /*!
     * \brief Returns the minimum value of the blue gain.
     *
     * \returns blueGainMin The minimum value of the blue gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float BlueGainMin() const;

    /*!
     * \brief Returns the maximum value of the blue gain.
     *
     * \returns blueGainMax The maximum value of the blue gain.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD float BlueGainMax() const;

    /*!
     * \brief Returns whether the gain supports the given pixel format.
     *
     * \returns Flag whether the given pixel format is supported.
     *
     * \param[in] pixelFormatName The pixel format of interest.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD bool IsPixelFormatSupported(PixelFormatName pixelFormatName) const;

    /*!
     * \brief Applies the gain on the given image in place that is, it will change the input image.
     *
     * \param[in] image Image to process.
     *
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     *
     * \since 1.3.2
     */
    void ProcessInPlace(Image& image) const;

    /*! \brief Applies the gain on the given image.
     *
     * \param[in] inputImage Image to process.
     *
     * \returns A new created image containing the gamma-corrected pixels.
     *
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     *
     * \since 1.3.2
     */
    PEAK_IPL_NO_DISCARD Image Process(const Image& inputImage) const;

private:
    friend GainBackendAccessor;
    explicit Gain(PEAK_IPL_GAIN_HANDLE gainHandle) noexcept;
    PEAK_IPL_GAIN_HANDLE m_backendHandle{};
};

} /* namespace ipl */
} /* namespace peak */

namespace
{

//! helper class to access the C-backend of the Gain, which are hidden from the public interface
class GainBackendAccessor
{
public:
    /*!
     * \brief Get the handle for a Gain instance
     *
     * \param gain the Gain class instance.
     *
     * \return The backend handle for the Gain instance.
     */
    static PEAK_IPL_GAIN_HANDLE BackendHandle(const peak::ipl::Gain& gain)
    {
        return gain.m_backendHandle;
    }

    /*!
     * \brief Create a Gain instance from a handle
     *
     * \param gainHandle The Gain handle to create the instance from.
     *
     * \note Will acquire a reference to the Gain instance by handle. See \ref shallowCopy for a detailed explanation.
     *
     * \return The Gain instance.
     */
    static peak::ipl::Gain CreateGain(PEAK_IPL_GAIN_HANDLE gainHandle)
    {
        return peak::ipl::Gain(gainHandle);
    }
};

} /* namespace */

namespace peak
{
namespace ipl
{
inline Gain::Gain()
{    
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Construct(&m_backendHandle); });
}

inline Gain::Gain(PEAK_IPL_GAIN_HANDLE gainHandle) noexcept
    : m_backendHandle{ gainHandle }
{
    ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Acquire(m_backendHandle); });
}

inline Gain::~Gain()
{
    if (m_backendHandle)
    {
        (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Destruct(m_backendHandle);
    }
}

inline Gain::Gain(Gain&& other) noexcept
{
    *this = std::move(other);
}

inline Gain::Gain(const Gain& other)
{
    if (other.m_backendHandle)
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Acquire(other.m_backendHandle); });
    }

    m_backendHandle = other.m_backendHandle;
}

inline Gain& Gain::operator=(const Gain& other)
{
    if (this != &other)
    {
        if (m_backendHandle)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Destruct(m_backendHandle);
        }

        if (other.m_backendHandle)
        {
            ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Acquire(other.m_backendHandle); });
        }
    }

    m_backendHandle = other.m_backendHandle;

    return *this;
}

inline Gain& Gain::operator=(Gain&& other) noexcept
{
    if (this != &other)
    {
        if (m_backendHandle != nullptr)
        {
            (void)PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Destruct(m_backendHandle);
        }

        m_backendHandle = other.m_backendHandle;
        other.m_backendHandle = nullptr;
    }

    return *this;
}
inline void Gain::SetMasterGainValue(float masterGainValue)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_SetMasterGainValue(
            m_backendHandle, reinterpret_cast<float*>(&masterGainValue));
    });
}
inline float Gain::MasterGainValue() const
{
    float masterGainValue;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetMasterGainValue(
            m_backendHandle, reinterpret_cast<float*>(&masterGainValue));
    });

    return masterGainValue;
}
inline float Gain::MasterGainMin() const
{
    float masterGainMin;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetMasterGainMin(
            m_backendHandle, reinterpret_cast<float*>(&masterGainMin));
    });

    return masterGainMin;
}
inline float Gain::MasterGainMax() const
{
    float masterGainMax;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetMasterGainMax(
            m_backendHandle, reinterpret_cast<float*>(&masterGainMax));
    });

    return masterGainMax;
}
inline void Gain::SetRedGainValue(float redGainValue)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_SetRedGainValue(
            m_backendHandle, reinterpret_cast<float*>(&redGainValue));
    });
}
inline float Gain::RedGainValue() const
{
    float redGainValue;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetRedGainValue(
            m_backendHandle, reinterpret_cast<float*>(&redGainValue));
    });

    return redGainValue;
}
inline float Gain::RedGainMin() const
{
    float redGainMin;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetRedGainMin(m_backendHandle, reinterpret_cast<float*>(&redGainMin));
    });

    return redGainMin;
}
inline float Gain::RedGainMax() const
{
    float redGainMax;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetRedGainMax(m_backendHandle, reinterpret_cast<float*>(&redGainMax));
    });

    return redGainMax;
}
inline void Gain::SetGreenGainValue(float greenGainValue)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_SetGreenGainValue(
            m_backendHandle, reinterpret_cast<float*>(&greenGainValue));
    });
}
inline float Gain::GreenGainValue() const
{
    float greenGainValue;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetGreenGainValue(
            m_backendHandle, reinterpret_cast<float*>(&greenGainValue));
    });

    return greenGainValue;
}
inline float Gain::GreenGainMin() const
{
    float greenGainMin;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetGreenGainMin(
            m_backendHandle, reinterpret_cast<float*>(&greenGainMin));
    });

    return greenGainMin;
}
inline float Gain::GreenGainMax() const
{
    float greenGainMax;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetGreenGainMax(
            m_backendHandle, reinterpret_cast<float*>(&greenGainMax));
    });

    return greenGainMax;
}
inline void Gain::SetBlueGainValue(float blueGainValue)
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_SetBlueGainValue(
            m_backendHandle, reinterpret_cast<float*>(&blueGainValue));
    });
}
inline float Gain::BlueGainValue() const
{
    float blueGainValue;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetBlueGainValue(
            m_backendHandle, reinterpret_cast<float*>(&blueGainValue));
    });

    return blueGainValue;
}
inline float Gain::BlueGainMin() const
{
    float blueGainMin;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetBlueGainMin(
            m_backendHandle, reinterpret_cast<float*>(&blueGainMin));
    });

    return blueGainMin;
}
inline float Gain::BlueGainMax() const
{
    float blueGainMax;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetBlueGainMax(
            m_backendHandle, reinterpret_cast<float*>(&blueGainMax));
    });

    return blueGainMax;
}
inline bool Gain::IsPixelFormatSupported(PixelFormatName pixelFormatName) const
{
    PEAK_IPL_BOOL8 isPixelFormatSupported = 0;

    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_GetIsPixelFormatSupported(m_backendHandle,
            static_cast<PEAK_IPL_PIXEL_FORMAT>(pixelFormatName),
            reinterpret_cast<PEAK_IPL_BOOL8*>(&isPixelFormatSupported));
    });

    return isPixelFormatSupported > 0;
}
inline void Gain::ProcessInPlace(Image& image) const
{
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_ProcessInPlace(
            m_backendHandle, ImageBackendAccessor::BackendHandle(image));
    });
}
inline Image Gain::Process(const Image& inputImage) const
{
    PEAK_IPL_IMAGE_HANDLE outputImageHandle = nullptr;
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_Gain_Process(
            m_backendHandle, ImageBackendAccessor::BackendHandle(inputImage), &outputImageHandle);
    });

    return ImageBackendAccessor::CreateImage(outputImageHandle);
}

} /* namespace ipl */
} /* namespace peak */
