/*!
 * \file    mainwindow.cpp
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

#include "mainwindow.h"

#include "backend.h"

#include "camera_dialog.h"
#include "control_widget.h"
#include "queue_worker.h"
#include "queue_model.h"

#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

#include <QDockWidget>
#include <QHeaderView>
#include <QTableView>

#define VERSION "1.0.0"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centralWidget);
    setCentralWidget(m_centralWidget);

    // Set minimum window size
    setMinimumSize(800, 600);

    // connect the error callback from backend to MainWindow
    connect(this, &MainWindow::libraryError, this, &MainWindow::handleLibraryError);
    backend_errorCallback_connect(this, &MainWindow::emitErrorSignal);

    // Start function of backend: open library
    int status = backend_start();
    if (status != 0)
    {
        m_hasError = true;
        m_blockErrorMessages = true;
        return;
    }

    // Create all user interface controls
    composeMainWindow();
    createControls();

    // Start Queue Worker Thread
    m_queue = new Queue(this);

    // Start CameraList updater. This is needed for DeviceLost / DeviceFound to be triggered.
    m_updater = new CameraListUpdater(this);
    m_updater->start();

    // Register custom Type (needed for signal to work)
    qRegisterMetaType<MessageData>();

    // connect signals
    connect(m_queue, &Queue::newMessage, m_message_model, qOverload<const MessageData&>(&QueueModel::addMessage));
    connect(m_controlsWidget, &ControlWidget::enableMessage, m_queue, &Queue::enableMessage);
    connect(m_controlsWidget, &ControlWidget::disableMessage, m_queue, &Queue::disableMessage);
    connect(m_controlsWidget, &ControlWidget::isMessageSupported, m_queue, &Queue::isMessageSupported);
    connect(m_message_model, &QueueModel::newMessageInserted, this, &MainWindow::newMessageInserted);
}

MainWindow::~MainWindow()
{
    m_queue->stop();
    m_updater->stop();
    backend_exit();
}


bool MainWindow::hasError() const
{
    return m_hasError;
}

void MainWindow::composeMainWindow()
{
    auto buttonLayout = new QHBoxLayout;
    m_flushViewButton = new QPushButton(tr("Flush Messages"), this);
    m_openCameraButton = new QPushButton(tr("Open Camera"), this);
    m_checkScrollBottom = new QCheckBox(tr("AutoScroll"), this);
    m_checkScrollBottom->setChecked(true);

    connect(m_flushViewButton, &QPushButton::clicked, this, &MainWindow::onFlushButtonClicked);
    connect(m_openCameraButton, &QPushButton::clicked, this, &MainWindow::onOpenCameraButtonClicked);

    buttonLayout->addWidget(m_openCameraButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_checkScrollBottom);
    buttonLayout->addWidget(m_flushViewButton);
    m_layout->addLayout(buttonLayout);

    m_message_view = new QTableView(m_centralWidget);
    m_layout->addWidget(m_message_view);

    m_message_model = new QueueModel(this);

    m_message_view->setAlternatingRowColors(true);
    m_message_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_message_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_message_view->setShowGrid(true);
    m_message_view->setModel(m_message_model);
    m_message_view->verticalHeader()->setDefaultSectionSize(m_message_view->verticalHeader()->minimumSectionSize());
    m_message_view->verticalHeader()->hide();
    m_message_view->horizontalHeader()->setStretchLastSection(true);
    m_message_view->horizontalHeader()->setHighlightSections(false);
    m_message_view->horizontalHeader()->setSortIndicatorShown(false);

    auto hLayoutInfo = new QHBoxLayout();
    m_layout->addLayout(hLayoutInfo);

    m_labelInfo = new QLabel(this);
    hLayoutInfo->addWidget(m_labelInfo);
    hLayoutInfo->addStretch();

    auto m_labelVersion = new QLabel(("message_queue_c v" VERSION), this);
    hLayoutInfo->addWidget(m_labelVersion);

    auto m_labelAboutQt = new QLabel(tr(R"(<a href="#aboutQt">About Qt</a>)"), this);
    connect(m_labelAboutQt, &QLabel::linkActivated, this, &MainWindow::onAboutQtLinkActivated);
    hLayoutInfo->addWidget(m_labelAboutQt);

    show();
}


void MainWindow::createControls()
{
    m_dockedControlsWidget = new QDockWidget(tr("Messages"), this);
    m_dockedControlsWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    m_dockedControlsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_controlsWidget = new ControlWidget(m_dockedControlsWidget);

    m_dockedControlsWidget->setWidget(m_controlsWidget);

    addDockWidget(Qt::LeftDockWidgetArea, m_dockedControlsWidget);

    auto width = m_controlsWidget->minimumSize().width();

    if (width == 0)
    {
        width = 200;
    }

    resizeDocks({ m_dockedControlsWidget }, { width }, Qt::Horizontal);
}

void MainWindow::onAboutQtLinkActivated(const QString& link)
{
    if (link == "#aboutQt")
    {
        QMessageBox::aboutQt(this, tr("About Qt"));
    }
}

void MainWindow::emitErrorSignal(const char* message, int status, void* errorCallbackContext)
{
    emit static_cast<MainWindow*>(errorCallbackContext)->libraryError(QString::fromStdString(message), status);
};

void MainWindow::handleLibraryError(QString message, int) const
{
    // block all error messages due to a critical message
    if (m_blockErrorMessages)
    {
        return;
    }

    message.prepend("Error: ");

    QMessageBox::warning(nullptr, "Error", message, QMessageBox::Ok);
}


void MainWindow::handleErrorAndQuit(QString message) const
{
    // block all error messages due to a critical message
    if (m_blockErrorMessages)
    {
        return;
    }

    message += "\n(Error!)";
    message += "\nExiting application";

    QMessageBox::critical(nullptr, "Error", message, QMessageBox::Ok);
    qApp->quit();
}

void MainWindow::onOpenCameraButtonClicked()
{
    CameraListDialog dialog(this);

    if (dialog.cameraCount() > 0)
    {
        dialog.exec();

        auto cameras = dialog.getSelectedCameras();

        for (const auto& cam : cameras)
        {
            m_controlsWidget->addCamera(cam.first, cam.second);
        }
    }
    else
    {
        QMessageBox::critical(this, tr("No Camera!"), tr("No camera is connected!"), QMessageBox::Ok);
    }

    m_controlsWidget->updateControl();
}

void MainWindow::onFlushButtonClicked()
{
    m_message_model->flush();
}

void MainWindow::newMessageInserted()
{
    if (m_checkScrollBottom->isChecked())
    {
        m_message_view->resizeColumnsToContents();
        m_message_view->scrollToBottom();
    }
}
