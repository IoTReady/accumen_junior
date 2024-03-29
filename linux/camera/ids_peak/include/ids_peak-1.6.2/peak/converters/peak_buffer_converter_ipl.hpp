/*!
 * \file    peak_buffer_converter_ipl.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once


#include <peak_ipl/peak_ipl.hpp>
#include <peak/data_stream/peak_data_stream.hpp>
#include <peak/device/peak_device.hpp>
#include <peak/peak_buffer_converter.hpp>

#include <cassert>


namespace peak
{

/*!
 * \brief Converts a core::Buffer into a peak::ipl::Image.
 *
 * This creates a peak::ipl::Image as a shallow copy of the buffer (i.e. using the same memory). Remember that the
 * buffer's memory is only under your control until you re-queue the buffer.
 *
 * \note To use this method, this file needs to be included explicitly:
 * \code
 * #include <peak/converters/peak_buffer_converter_peak_ipl_.hpp>
 * \endcode
 */
template <>
inline peak::ipl::Image BufferTo<peak::ipl::Image>(const std::shared_ptr<peak::core::Buffer>& buffer)
{
    if (buffer == nullptr)
    {
        throw core::InvalidArgumentException("The given buffer is a nullptr!");
    }

    if (buffer->HasImage())
    {
        assert(buffer->PixelFormatNamespace() != core::PixelFormatNamespace::IIDC
            && buffer->PixelFormatNamespace() != core::PixelFormatNamespace::Custom);

        size_t usableHeight = buffer->Height();
        const size_t usableWidth = buffer->Width();
        size_t usableBytes = buffer->Size() - buffer->ImageOffset();
        uint8_t* firstPixelPtr = static_cast<uint8_t*>(buffer->BasePtr()) + buffer->ImageOffset();

        if (buffer->HasChunks())
        {
            auto chunks = buffer->Chunks();

            if (chunks.empty())
            {
                throw core::InternalErrorException("Buffer has no chunks.");
            }

            const auto imageSizeWithoutChunks = chunks[0]->Size();

            for (const auto& chunk : chunks)
            {
                if (0x73020405 == chunk->ID())
                {
                    uint16_t* chunkBase = static_cast<uint16_t*>(chunk->BasePtr());

                    usableHeight = static_cast<size_t>(chunkBase[1]);
                    const auto usableOffsetY = static_cast<size_t>(chunkBase[3]);

                    // Pitch per line
                    const auto pitch = imageSizeWithoutChunks / buffer->Height();
                    const auto cutOffTop = pitch * usableOffsetY;
                    const auto cutOff = (buffer->Height() - usableHeight) * pitch;

                    firstPixelPtr += cutOffTop;
                    usableBytes = imageSizeWithoutChunks - cutOff;

                    break;
                }
            }
        }

        return peak::ipl::Image(static_cast<peak::ipl::PixelFormatName>(buffer->PixelFormat()),
            firstPixelPtr, usableBytes, usableWidth, usableHeight, buffer->Timestamp_ns());
    }
    else if (buffer->PayloadType() == core::BufferPayloadType::Chunk)
    {
        auto remoteNodeMap = buffer->ParentDataStream()->ParentDevice()->RemoteDevice()->NodeMaps()[0];
        remoteNodeMap->UpdateChunkNodes(buffer);

        auto chunkWidth = remoteNodeMap->FindNode<core::nodes::IntegerNode>("ChunkWidth");
        if (chunkWidth->AccessStatus() == core::nodes::NodeAccessStatus::NotAvailable)
        {
            throw core::InvalidCastException("Buffer has no ChunkWidth.");
        }
        auto width = chunkWidth->Value();

        auto chunkHeight = remoteNodeMap->FindNode<core::nodes::IntegerNode>("ChunkHeight");
        if (chunkHeight->AccessStatus() == core::nodes::NodeAccessStatus::NotAvailable)
        {
            throw core::InvalidCastException("Buffer has no ChunkHeight.");
        }
        auto height = chunkHeight->Value();

        auto chunkPixelFormat = remoteNodeMap->FindNode<core::nodes::EnumerationNode>("ChunkPixelFormat");
        if (chunkPixelFormat->AccessStatus() == core::nodes::NodeAccessStatus::NotAvailable)
        {
            throw core::InvalidCastException("Buffer has no ChunkPixelFormat.");
        }
        auto pixelFormatValue = chunkPixelFormat->CurrentEntry()->NumericValue();
        auto pixelFormat = static_cast<peak::ipl::PixelFormatName>(pixelFormatValue);

        // assume first chunks is image data
        auto imageData = buffer->Chunks()[0];

        auto expectedDataSize = peak::ipl::PixelFormat(pixelFormat).CalculateStorageSizeOfPixels(static_cast<uint64_t>(width * height));
        if (imageData->Size() < expectedDataSize)
        {
            std::stringstream msg;
            msg << "The buffer's first chunk's size (" << imageData->Size()
                << ") is smaller than the expected data size (" << expectedDataSize << ").";
            throw core::InvalidCastException(msg.str());
        }

        return peak::ipl::Image(pixelFormat, static_cast<uint8_t*>(imageData->BasePtr()), imageData->Size(),
            static_cast<size_t>(width), static_cast<size_t>(height), buffer->Timestamp_ns());
    }
    else
    {
        throw core::InvalidCastException("Buffer has no image data and no chunks.");
    }
}

} /* namespace peak */
