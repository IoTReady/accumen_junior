/*!
 * \file    display.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   1.2.0
 *
 * \brief   The Display class implements an easy way to display images from a
 *          camera in a QT widgets window. It can be used for other QT widget
 *          applications as well.
 *
 * \version 1.2.0
 *
 * Copyright (C) 2021 - 2023, IDS Imaging Development Systems GmbH.
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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QImage>
#include <QMutex>
#include <QRect>
#include <QWidget>



class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CustomGraphicsScene(QWidget* pParent);
    void setImage(QImage image);

private:
    void drawBackground(QPainter* painter, const QRectF& rect) override;

    QMutex m_mutex{};
    QImage m_image;
};

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget* parent);

private:
    CustomGraphicsScene* m_scene;
    QGraphicsView* m_view;

public slots:
    void onImageReceived(QImage image);
};

#endif // DISPLAY_H
