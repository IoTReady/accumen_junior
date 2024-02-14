/*!
 * \file    peak_ipl_video_writer.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-07-25
 * \since   1.5
 *
 * Copyright (c) 2022 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <peak_ipl/backend/peak_ipl_backend.h>
#include <peak_ipl/exception/peak_ipl_exception.hpp>
#include <peak_ipl/types/peak_ipl_pixel_format.hpp>
#include <peak_ipl/types/peak_ipl_range.hpp>
#include <peak_ipl/types/peak_ipl_simple_types.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace peak
{
namespace ipl
{

class VideoWriter;

namespace encoder
{

/*!
 * \brief Encoder Interface Class.
 */
class IEncoder
{
    friend class peak::ipl::VideoWriter;

protected:
    PEAK_IPL_VIDEO_HANDLE m_handle{nullptr};

    void SetHandle(PEAK_IPL_VIDEO_HANDLE handle)
    {
        this->m_handle = handle;
    }

public:
    IEncoder() = default;
    virtual ~IEncoder() = default;
    virtual PEAK_IPL_VIDEO_ENCODER Id() = 0;

    /*!
     * \brief Sets an option on the encoder.
     * \param[in] option The encoder option to set.
     * \param[in] value  The value for the encoder option.
     * \param[in] count  The size of the value.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void SetOption(PEAK_IPL_VIDEO_ENCODER_OPTION option, const void* value, size_t count)
    {
        ExecuteAndMapReturnCodes(
            [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Encoder_SetOption(m_handle, option, value, count); });
    }

    /*!
     * \brief Gets an option of the encoder.
     * \param[in]  option   The encoder option to get.
     * \param[out] value    The value for the encoder option.
     * \param[in]  count    The size of the value.
     * \param[out] outCount The resulting size of the value.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void GetOption(PEAK_IPL_VIDEO_ENCODER_OPTION option, void* value, size_t count, size_t* outCount)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Encoder_GetOption(m_handle, option, value, count, outCount);
        });
    }

    /*!
     * \brief Receive the range for an option of the encoder.
     * \param[in]  option The encoder option to receive the range from.
     * \param[out] min    The minimum value for the encoder option.
     * \param[out] max    The maximum value for the encoder option.
     * \param[out] inc    The increment value for the encoder option.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void OptionRange(PEAK_IPL_VIDEO_ENCODER_OPTION option, void* min, void* max, void* inc)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Encoder_OptionRange(m_handle, option, min, max, inc);
        });
    }
};

static std::unique_ptr<IEncoder> Create(PEAK_IPL_VIDEO_ENCODER encoder);

/*!
 * \brief JPEG encoder specialization of \ref IEncoder.
 * Create the encoder with the according Create function.
 */
class MJPEGEncoder : public IEncoder
{
    friend std::unique_ptr<IEncoder> Create(PEAK_IPL_VIDEO_ENCODER encoder);

    MJPEGEncoder() = default;

public:
    MJPEGEncoder(const MJPEGEncoder&) = delete;
    MJPEGEncoder(MJPEGEncoder&&) = default;

    MJPEGEncoder& operator=(const MJPEGEncoder&) = delete;
    MJPEGEncoder& operator=(MJPEGEncoder&&) = default;

    /*!
     * \brief Get the current type of the encoder.
     * \return Encoder type
     */
    PEAK_IPL_VIDEO_ENCODER Id() override
    {
        return PEAK_IPL_VIDEO_ENCODER_MJPEG;
    }

    /*!
     * \brief Sets the quality for the jpeg encoder.
     * \param[in] quality Quality to set.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void SetQuality(uint32_t quality)
    {
        SetOption(PEAK_IPL_VIDEO_ENCODER_OPTION_QUALITY, &quality, sizeof(quality));
    }

    /*!
     * \brief Gets the quality for the jpeg encoder.
     *
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    uint32_t GetQuality()
    {
        uint32_t quality = 0;
        GetOption(PEAK_IPL_VIDEO_ENCODER_OPTION_QUALITY, &quality, sizeof(quality), nullptr);
        return quality;
    }

    /*!
     * \brief Gets the quality range for the jpeg encoder.
     *
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    Range<double> QualityRange()
    {
        double min = 0, max = 0, inc = 0;
        OptionRange(PEAK_IPL_VIDEO_ENCODER_OPTION_QUALITY, &min, &max, &inc);
        return { min, max, inc };
    }
};

/*!
 * \brief Get the supported encoders
 * \return List of supported encoders
 *
 * \throws Exception                        An internal error has occurred
 *
 * \since 1.5
 */
static inline std::vector<PEAK_IPL_VIDEO_ENCODER> GetSupportedEncodersForAContainer(PEAK_IPL_VIDEO_CONTAINER container)
{
    size_t length{};
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer(container, nullptr, &length);
    });
    std::vector<PEAK_IPL_VIDEO_ENCODER> encoders(length);
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_GetSupportedEncodersForAContainer(
            container, encoders.data(), &length);
    });
    return encoders;
}

/*!
 * \brief Get the supported pixel formats for an encoder.
 * \return List of supported pixel formats
 *
 * \throws Exception                        An internal error has occurred
 *
 * \since 1.5
 */
static inline std::vector<PixelFormatName> GetSupportedPixelFormats(PEAK_IPL_VIDEO_ENCODER encoder)
{
    size_t sizeOfData{};
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats(encoder, nullptr, &sizeOfData);
    });
    std::vector<PixelFormatName> formats(sizeOfData);
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Encoder_GetSupportedPixelFormats(
            encoder, reinterpret_cast<PEAK_IPL_PIXEL_FORMAT*>(formats.data()), &sizeOfData);
    });
    return formats;
}

/*!
 * \brief Factory to create an encoder.
 * \param[in] encoder Create an encoder of this type.
 * \return Unique instance of the created encoder.
 *
 * \throws InvalidArgumentException         Arguments passed are invalid
 *
 * \since 1.5
 */
static inline std::unique_ptr<IEncoder> Create(PEAK_IPL_VIDEO_ENCODER encoder)
{
    switch (encoder)
    {
    case PEAK_IPL_VIDEO_ENCODER_MJPEG:
        return std::unique_ptr<MJPEGEncoder>(new MJPEGEncoder());
    default:
        throw InvalidArgumentException("The supplied encoder is not a valid value!");
    }

}

} // namespace encoder

namespace container
{

/*!
 * \brief Container Interface Class.
 */
class IContainer
{
    friend class peak::ipl::VideoWriter;

protected:
    PEAK_IPL_VIDEO_HANDLE m_handle{ nullptr };
    void SetHandle(PEAK_IPL_VIDEO_HANDLE handle)
    {
        this->m_handle = handle;
    }

public:
    IContainer() = default;
    virtual ~IContainer() = default;
    virtual PEAK_IPL_VIDEO_CONTAINER Id() = 0;

    /*!
     * \brief Sets an option on the container.
     * \param[in] option The container option to set.
     * \param[in] value  The value for the container option.
     * \param[in] count  The size of the value.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void SetOption(PEAK_IPL_VIDEO_CONTAINER_OPTION option, const void* value, size_t count)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Container_SetOption(m_handle, option, value, count);
        });
    }

    /*!
     * \brief Gets an option of the container.
     * \param[in]  option   The container option to get.
     * \param[out] value    The value for the container option.
     * \param[in]  count    The size of the value.
     * \param[out] outCount The resulting size of the value.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void GetOption(PEAK_IPL_VIDEO_CONTAINER_OPTION option, void* value, size_t count, size_t* outCount)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Container_GetOption(
                m_handle, option, value, count, outCount);
        });
    }

    /*!
     * \brief Receive the range for an option of the container.
     * \param[in]  option The container option to receive the range from.
     * \param[out] min    The minimum value for the container option.
     * \param[out] max    The maximum value for the container option.
     * \param[out] inc    The increment value for the container option.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void OptionRange(PEAK_IPL_VIDEO_CONTAINER_OPTION option, void* min, void* max, void* inc)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Container_OptionRange(m_handle, option, min, max, inc);
        });
    }
};

static std::unique_ptr<IContainer> Create(PEAK_IPL_VIDEO_CONTAINER container);

/*!
 * \brief AVI container specialization of \ref IContainer.
 * Create the container with the according Create function.
 */
class AVIContainer : public IContainer
{
    friend std::unique_ptr<IContainer> Create(PEAK_IPL_VIDEO_CONTAINER container);
    AVIContainer() = default;

public:
    AVIContainer(const AVIContainer&) = delete;
    AVIContainer(AVIContainer&&) = default;

    AVIContainer& operator=(const AVIContainer&) = delete;
    AVIContainer& operator=(AVIContainer&&) = default;

    /*!
     * \brief Get the current type of the container.
     * \return Container type
     */
    PEAK_IPL_VIDEO_CONTAINER Id() override
    {
        return PEAK_IPL_VIDEO_CONTAINER_AVI;
    }

    /*!
     * \brief Set the frame rate for the opened video.
     * \param[in] fps The frame rate to set.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void SetFramerate(double fps)
    {
        SetOption(PEAK_IPL_VIDEO_CONTAINER_OPTION_FRAMERATE, &fps, sizeof(fps));
    }

    /*!
     * \brief Get the frame rate for the opened video.
     *
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    double GetFramerate()
    {
        double fps = 0;
        GetOption(PEAK_IPL_VIDEO_CONTAINER_OPTION_FRAMERATE, &fps, sizeof(fps), nullptr);
        return fps;
    }
};

/*!
 * \brief Get the supported containers.
 * \return List of the supported containers.
 */
static inline std::vector<PEAK_IPL_VIDEO_CONTAINER> GetSupportedContainersForAnEncoder(PEAK_IPL_VIDEO_ENCODER encoder)
{
    size_t length{};
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder(encoder, nullptr, &length);
    });
    std::vector<PEAK_IPL_VIDEO_CONTAINER> containers(length);
    ExecuteAndMapReturnCodes([&] {
        return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_GetSupportedContainersForAnEncoder(
            encoder, containers.data(), &length);
    });
    return containers;
}

/*!
 * \brief Factory to create a container.
 * \param[in] container Create a container of this type.
 * \return Unique instance of the created container.
 *
 * \throws InvalidArgumentException         Arguments passed are invalid
 *
 * \since 1.5
 */
static std::unique_ptr<IContainer> Create(PEAK_IPL_VIDEO_CONTAINER container)
{
    switch (container)
    {
    case PEAK_IPL_VIDEO_CONTAINER_AVI:
        return std::unique_ptr<AVIContainer>(new AVIContainer());
    default:
        throw InvalidArgumentException("The supplied container is not a valid value!");
    }
}

} // namespace container

class Image;

/*!
 * \brief Writes images to a file in the form of a video.
 */
class VideoWriter final
{
public:
    VideoWriter() = default;
    ~VideoWriter()
    {
        if(IsOpen())
        {
            Close();
        }
    }
    VideoWriter(const VideoWriter&) = delete;
    VideoWriter(VideoWriter&& other) noexcept
    {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
        m_encoder = std::move(other.m_encoder);
        m_container = std::move(other.m_container);
    }
    VideoWriter& operator=(const VideoWriter&) = delete;
    VideoWriter& operator=(VideoWriter&& other) noexcept
    {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
        m_encoder = std::move(other.m_encoder);
        m_container = std::move(other.m_container);

        return *this;
    }

    /*!
     * \brief Open the VideoWriter and sets up a container and an encoder. The file is created upon receiving the first
     * image with \ref Append.
     *
     * \param[in] fileName The filename of the video as an utf-8 encoded string (u8string)
     * \param[in] container The container of the video.
     * \param[in] encoder The encoder of the video.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void Open(const std::string& fileName, std::shared_ptr<container::IContainer> container,
        std::shared_ptr<encoder::IEncoder> encoder)
    {
        if (!container || !encoder)
        {
            throw InvalidArgumentException("The given pointer is a null pointer!");
        }

        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_Open(
                &m_handle, fileName.c_str(), container->Id(), encoder->Id());
        });
        m_encoder = encoder;
        m_container = container;

        m_encoder->SetHandle(m_handle);
        m_container->SetHandle(m_handle);
    }

    /*!
     * \brief Open the VideoWriter with AVI container and mjpeg encoder.
     * The file is created upon receiving the first image with \ref Append.
     * \param[in] fileName The filename of the video as an utf-8 encoded string (u8string)
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void Open(const std::string& fileName)
    {
        auto container = container::Create(PEAK_IPL_VIDEO_CONTAINER_AVI);
        auto encoder = encoder::Create(PEAK_IPL_VIDEO_ENCODER_MJPEG);
        Open(fileName, std::move(container), std::move(encoder));
    }

    /*!
     * \brief Check if the VideoWriter is already open.
     * \return Boolean if the VideoWriter is already open.

     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    PEAK_IPL_NO_DISCARD bool IsOpen() const
    {
        PEAK_IPL_BOOL8 isOpen = 0;
        ExecuteAndMapReturnCodes(
            [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_IsOpen(m_handle, &isOpen); });
        return isOpen != 0;
    }

    /*!
     * \brief Close the opened VideoWriter.
     *
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void Close()
    {
        ExecuteAndMapReturnCodes([&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_Close(m_handle); });

        m_handle = nullptr;
    }

    /*!
     * \brief Append an image to the opened video.
     * \remark Note that the image dimensions must be divisible by 8 (May be lifted by a later version)
     * \param[in] image The image to append.
     *
     * \throws InvalidArgumentException         Arguments passed are invalid
     * \throws ImageFormatNotSupportedException image has unsupported pixel format.
     * \throws BusyException                    The internal queue is full. The image won't be added to the video
     *                                          stream.
     * \throws Exception                        An internal error has occurred
     *
     * \since 1.5
     */
    void Append(const Image& image)
    {
        if (m_encoder == nullptr)
        {
            throw InvalidArgumentException("The open method must be called before append!");
        }
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_Append(
                m_handle, ImageBackendAccessor::BackendHandle(image));
        });
    }

    /*! @copydoc VideoWriter::Append() */
    VideoWriter& operator<<(const Image& image)
    {
        Append(image);
        return *this;
    }

    /*!
     * \brief Get the used encoder.
     * \return Instance of the encoder.
     */
    PEAK_IPL_NO_DISCARD std::shared_ptr<encoder::IEncoder> Encoder() const
    {
        return m_encoder;
    }

    /*!
     * \brief Get the used container.
     * \return Instance of the container.
     */
    PEAK_IPL_NO_DISCARD std::shared_ptr<container::IContainer> Container() const
    {
        return m_container;
    }

    /*!
     * \brief Get the encoded frames in the video stream.
     * \return Number of encoded frames in the video stream.
     */
    PEAK_IPL_NO_DISCARD uint64_t NumFramesEncoded() const
    {
        uint64_t numFramesEncoded{};
        ExecuteAndMapReturnCodes(
            [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_GetEncodedFrames(m_handle, &numFramesEncoded); });
        return numFramesEncoded;
    }

    /*!
     * \brief Get the dropped frames in the video stream.
     * \return Number of dropped frames in the video stream.
     */
    PEAK_IPL_NO_DISCARD uint64_t NumFramesDropped() const
    {
        uint64_t numFramesDropped{};
        ExecuteAndMapReturnCodes(
            [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_GetDroppedFrames(m_handle, &numFramesDropped); });
        return numFramesDropped;
    }

    /*!
     * \brief Get the file size of the video in bytes.
     * \return File size of the video.
     */
    PEAK_IPL_NO_DISCARD uint64_t FileSize() const
    {
        uint64_t s{};
        ExecuteAndMapReturnCodes(
            [&] { return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_GetFileSize(m_handle, &s); });
        return s;
    }

    /*!
     * \brief Reset compressed and dropped frame counters.
     */
    void ResetFrameCounters()
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_ResetFrameCounters(m_handle);
        });
    }

    /*!
     * \brief Set the internal queue size of the video writer
     * \param queueSize internal queue size to set. See \ref QueueSizeRange() for a range of valid values
     */
    void SetQueueSize(int32_t queueSize)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Queue_SetSize(m_handle, queueSize);
        });
    }

    /*!
     * \brief Get the internal queue size of the video writer
     * \returns the internal queue size
     */
    PEAK_IPL_NO_DISCARD int32_t QueueSize()
    {
        int32_t queueSize{};
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Queue_GetSize(m_handle, &queueSize);
        });

        return queueSize;
    }

    /*!
     * \brief Get the range of the queue size
     * \returns Range<int32_t> with valid values
     */
    PEAK_IPL_NO_DISCARD Range<int32_t> QueueSizeRange()
    {
        int32_t minQueueSize{}, maxQueueSize{};
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Queue_GetRange(m_handle, &minQueueSize, &maxQueueSize);
        });

        return {minQueueSize, maxQueueSize, 1};
    }

    /*!
     * \brief Get the count of the currently queued buffers, which are waiting to be processed
     * \returns the count of the queued buffers
     */
    PEAK_IPL_NO_DISCARD int32_t NumBuffersQueued()
    {
        int32_t queued{};
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Queue_GetQueued(m_handle, &queued);
        });

        return queued;
    }

    /*!
     * \brief Wait until frame processing is done.
     * \param timeout_ms The timeout in milliseconds.
     */
    void WaitUntilFrameDone(int32_t timeout_ms)
    {
        ExecuteAndMapReturnCodes([&] {
          return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_WaitUntilFrameDone(m_handle, timeout_ms);
        });
    }

    /*!
     * \brief Wait until the queue is empty.
     * \param timeout_ms The timeout in milliseconds.
     */
    void WaitUntilQueueEmpty(int32_t timeout_ms)
    {
        ExecuteAndMapReturnCodes([&] {
            return PEAK_IPL_C_ABI_PREFIX PEAK_IPL_VideoWriter_Video_WaitUntilQueueEmpty(m_handle, timeout_ms);
        });
    }

private:
    PEAK_IPL_VIDEO_HANDLE m_handle{nullptr};
    std::shared_ptr<encoder::IEncoder> m_encoder{};
    std::shared_ptr<container::IContainer> m_container{};
};

} /* namespace ipl */
} /* namespace peak */
