/*!
* \file    camera_model.cpp
* \author  IDS Imaging Development Systems GmbH
* \date    2023-04-24
* \since   2.5.0
*
* \version 1.0.0
*
* Copyright (C) 2023, IDS Imaging Development Systems GmbH.
*
* The information in this document is subject to change without notice
* and should not be construed as a commitment by IDS Imaging Development Systems GmbH.
* IDS Imaging Development Systems GmbH does not assume any responsibility for any errors
* that may appear in this document.
*
* This document, or source code, is provided solely as an example of how to utilize
* IDS Imaging Development Systems GmbH software libraries in a sample application.
* IDS Imaging Development Systems GmbH does not assume any responsibility
* for the use or reliability of any portion of this document.
*
* General permission to copy or modify is hereby granted.
*/

#include "camera_model.h"
#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

CameraListModel::CameraListModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    m_header << tr("ID") << tr("Type") << tr("Access") << tr("Serial") << tr("Model") << tr("UserDefinedName");

    // update is called by CameraListUpdater

    auto status = peak_CameraList_Get(nullptr, &m_cameraCount);
    if (status != PEAK_STATUS_SUCCESS)
    {
        return;
    }

    m_descriptorList.resize(m_cameraCount);
    status = peak_CameraList_Get(m_descriptorList.data(), &m_cameraCount);
    if (status == PEAK_STATUS_BUFFER_TOO_SMALL)
    {
        m_descriptorList.resize(m_cameraCount);
        status = peak_CameraList_Get(m_descriptorList.data(), &m_cameraCount);
    }

    if (status != PEAK_STATUS_SUCCESS)
    {
        return;
    }

    for(const auto& cam : m_descriptorList)
    {
        m_accessStatus.emplace_back(peak_Camera_GetAccessStatus(cam.cameraID));
    }
}

int CameraListModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(m_cameraCount);
}

int CameraListModel::columnCount(const QModelIndex& parent) const
{
    return 6;
}

QVariant CameraListModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole || index.row() >= m_cameraCount || index.column() > 5)
    {
        return {};
    }

    auto& cam = m_descriptorList.at(index.row());


    switch (index.column())
    {
    case 0:
        return static_cast<int>(cam.cameraID);

    case 1:
        return mapType(cam.cameraType);

    case 2: {
        if (m_accessStatus.size() > index.row())
        {
            auto access_status = m_accessStatus.at(index.row());
            return mapStatus(access_status);
        }
        else
        {
            return "Invalid";
        }
    }

    case 3:
        return cam.modelName;

    case 4:
        return cam.serialNumber;

    case 5:
        return cam.userDefinedName;

    default:
        break;
    }

    return {};
}

Qt::ItemFlags CameraListModel::flags(const QModelIndex& index) const
{
    if (index.row() >= m_accessStatus.size() || index.column() > 5)
    {
        return {};
    }

    auto access_status = m_accessStatus.at(index.row());

    Qt::ItemFlags f = Qt::ItemIsSelectable;

    if (access_status == PEAK_ACCESS_READWRITE)
    {
        f |= Qt::ItemIsEnabled;
    }

    return f;
}

QVariant CameraListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole || section > m_header.size())
    {
        return {};
    }

    return m_header.at(section);
}

QString CameraListModel::mapType(peak_camera_type type)
{
    switch (type)
    {
    case PEAK_CAMERA_TYPE_UEYE_PLUS_U3V:
        return "U3V";
    case PEAK_CAMERA_TYPE_UEYE_PLUS_GEV:
        return "GEV";
    case PEAK_CAMERA_TYPE_UEYE_USB:
        return "UI USB";
    case PEAK_CAMERA_TYPE_UEYE_ETH:
        return "UI ETH";
    case PEAK_CAMERA_TYPE_INVALID:
        break;
    }

    return "Invalid";
}

QString CameraListModel::mapStatus(peak_access_status status)
{
    switch (status)
    {
    case PEAK_ACCESS_NOT_SUPPORTED:
        return "-";
    case PEAK_ACCESS_NONE:
        return "NONE";
    case PEAK_ACCESS_GFA_LOCK:
        return "LOCK";
    case PEAK_ACCESS_READONLY:
        return "RO";
    case PEAK_ACCESS_WRITEONLY:
        return "WO";
    case PEAK_ACCESS_READWRITE:
        return "R/W";
    case PEAK_ACCESS_INVALID:
        break;
    }

    return "Invalid";
}
