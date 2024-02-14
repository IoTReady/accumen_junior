/*!
 * \file    camera_dialog.h
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

#ifndef CAMERA_DIALOG_H
#define CAMERA_DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableWidget>

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

#include "camera_model.h"

class CameraListDialog : public QDialog
{
    Q_OBJECT

public:
    using CameraSelection = QList<QPair<QString, peak_camera_id>>;

    explicit CameraListDialog(QWidget* parent = nullptr);

    CameraSelection getSelectedCameras() const
    {
        return m_selectedCamera;
    }

    void accept() override;
    size_t cameraCount() const
    {
        return m_model->cameraCount();
    }

private slots:
    void onSelectionChanged();

private:
    CameraSelection m_selectedCamera{};
    QTableView* m_table{};
    QDialogButtonBox* m_buttonBox{};
    CameraListModel* m_model{};
};


#endif // CAMERA_DIALOG_H
