/*!
 * \file    ids_gentl_custom_extensions.h
 * \author  IDS Imaging Development Systems GmbH
 *
 * \brief   IDS GenTL Producer Interface Extension
 */

#ifndef IDS_GENTLEX_H
#define IDS_GENTLEX_H

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
/*! \brief IDS GenTL namespace */
namespace IDS_GenTL
{
#endif

/*! \brief Custom %Error code enumeration.
 *
 * This enumeration extends the standard enumeration of error codes.
 */
enum GC_ERROR_LIST_EX
{
    /*! \brief Internal error. */
    GC_ERR_INTERNAL = -10000 - 0
};

/*! \brief Data type for statistics data. */
typedef struct Statistics
{
    /*! \brief The number of values that were considered for the statistics. */
    uint64_t value_count;

    /*! \brief The mean value. */
    uint64_t mean_value;

    /*! \brief The minimum value. */
    uint64_t min_value;

    /*! \brief The maximum value. */
    uint64_t max_value;

} Statistics;

/*! \brief Custom %Buffer Info command enumeration.
 *
 * This enumeration extends the standard enumeration of commands to retrieve
 * information with the GenICam::TL::Client::DSGetBufferInfo function on a data stream / buffer handle pair.
 */
enum  BUFFER_INFO_CMD_LIST_EX
{
    /*! \brief Gets the status value that the device reported in the leader packet.
     *
     *  Range of values: UINT16.
     *
     * \note DSGetBufferInfo() will return GC_ERR_NOT_AVAILABLE if
     *       there was no leader received for the frame of interest.
     */
    BUFFER_INFO_LEADER_STATUS  = 1000 + 0,

    /*! \brief Gets the status value that the device reported in the trailer packet.
     *
     *  Range of values: UINT16.
     * 
     * \note DSGetBufferInfo() will return GC_ERR_NOT_AVAILABLE if
     *       there was no trailer received for the frame of interest.
     */
    BUFFER_INFO_TRAILER_STATUS = 1000 + 1,
};

/*! \brief Custom %Stream Info command enumeration.
 *
 * This enumeration extends the standard enumeration of commands to retrieve
 * information with the GenICam::TL::Client::DSGetStreamInfo function on a data stream handle.
 */
enum  STREAM_INFO_CMD_LIST_EX
{
    /*! \brief Gets the buffer fill level statistics.
     *
     *  The fill level is the percentage of received bytes in respect to expected bytes for the buffer.
     *  The fill level statistics is reset with acquisition start.
     *
     *  Range of values: IDS_GenTL::Statistics with values out of range 0..100.
     *
     * \note Supported for interface Socket GEV only.
     */
    STREAM_INFO_BUFFER_FILL_LEVEL_STATISTICS    = 7000 + 0,

    /*! \brief Gets the buffer packet resend statistics.
     *
     *  The packet resend value is the ratio of requested packet resends to the total packets for the buffer.
     *  The value is given in permille.
     *
     *  Range of values: IDS_GenTL::Statistics with values out of range 0..1000.
     *
     * \note Supported for interface Socket GEV only.
     */
    STREAM_INFO_BUFFER_PACKET_RESEND_STATISTICS = 7000 + 1
};

/*! \brief Custom %Event Type enumeration.
 *
 * This enumeration extends the standard enumeration of event types
 * that can be registered on certain modules with the GenICam::TL::Client::GCRegisterEvent function.
 */
enum EVENT_TYPE_LIST_EX
{
    /*! \brief Notification if the connection status of an opened device changed.
     *
     *  This notification indicates that the remote device was detached or re-attached.
     *
     *  This event can be registered on %Device modules.
     *  The event related data is of type EVENT_REMOTE_DEVICE_CONNECTION_STATUS_CHANGE_DATA.
     *
     * \note Supported for interface GEV only.
     */
    EVENT_REMOTE_DEVICE_CONNECTION_STATUS_CHANGE = 1000 + 0
};

/* Structure of the data returned from a signaled "Remote Device Connection Status Change" event. */
#pragma pack (push, 1)
typedef struct S_EVENT_REMOTE_DEVICE_CONNECTION_STATUS_CHANGE
{
    /*! \brief The Handle of the Local %Device that this event refers to. */
    GenTL::DEV_HANDLE DeviceHandle;

    /*! \brief The new Remote Device Connection Status.
     *
     * 0 for Remote Device Detached
     * 1 for Remote Device Attached
     */
    uint16_t NewRemoteDeviceConnectionStatus;
} EVENT_REMOTE_DEVICE_CONNECTION_STATUS_CHANGE_DATA;
#pragma pack (pop)

#ifdef __cplusplus
} /* end of namespace IDS_GenTL */
} /* end of extern "C" */
#endif


#endif /* IDS_GENTLEX_H */
