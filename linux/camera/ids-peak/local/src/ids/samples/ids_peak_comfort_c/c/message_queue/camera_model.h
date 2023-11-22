/*!
* \file    camera_model.h
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

#ifndef CAMERA_MODEL_H
#define CAMERA_MODEL_H

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <QAbstractTableModel>

class CameraListModel : public QAbstractTableModel
{
public:
    explicit CameraListModel(QObject* parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role= Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    size_t cameraCount() const
    {
        return m_cameraCount;
    }

private:
    static QString mapType(peak_camera_type type) ;
    static QString mapStatus(peak_access_status status) ;

    size_t m_cameraCount{};
    std::vector<peak_camera_descriptor> m_descriptorList{};
    std::vector<peak_access_status> m_accessStatus;
    QStringList m_header{};
};


#endif // CAMERA_MODEL_H
