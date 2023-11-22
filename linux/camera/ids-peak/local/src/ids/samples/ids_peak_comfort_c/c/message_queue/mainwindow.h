/*!
 * \file    mainwindow.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.0.0
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "control_widget.h"
#include "queue_worker.h"
#include "cameralist_updater.h"


#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QSlider>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

#include <QComboBox>
#include <QTableView>
#include <cstdint>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    bool hasError() const;

public slots:
    void handleLibraryError(QString message, int status) const;
    void handleErrorAndQuit(QString message) const;

    void onAboutQtLinkActivated(const QString& link);

    void onFlushButtonClicked();
    void newMessageInserted();
    void onOpenCameraButtonClicked();

signals:
    void libraryError(QString message, int status);

private:
    void composeMainWindow();
    void createControls();

    static void emitErrorSignal(const char* message, int status, void* errorCallbackContext);

    QWidget* m_centralWidget{};
    QLabel* m_labelCameraInfo{};
    QLabel* m_labelInfo{};
    QVBoxLayout* m_layout{};

    QTableView* m_message_view{};
    QueueModel* m_message_model{};

    QDockWidget* m_dockedControlsWidget{};
    ControlWidget* m_controlsWidget{};

    Queue* m_queue{};
    CameraListUpdater* m_updater{};

    QPushButton* m_flushViewButton{};
    QPushButton* m_openCameraButton{};
    QCheckBox* m_checkScrollBottom{};

    bool m_hasError{};
    bool m_blockErrorMessages{};
};

#endif // MAINWINDOW_H
