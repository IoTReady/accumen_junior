/*!
 * \file    main.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.0.0
 *
 * \brief   This application demonstrates how to use IDS peak comfortC
 *          combined with a QT widgets GUI to trigger and display images
 *          from Genicam compatible device.
 *
 * \version 1.1.0
 *
 * Copyright (C) 2022 - 2023, IDS Imaging Development Systems GmbH.
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

#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (w.hasError())
    {
        return -1;
    }

    w.show();
    return QApplication::exec();
}
