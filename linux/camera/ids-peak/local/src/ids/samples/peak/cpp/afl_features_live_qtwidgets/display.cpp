/*!
 * \file    display.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-03-07
 * \since   1.0.0
 *
 * \brief   The Display class implements an easy way to display images from a
 *          camera in a QT widgets window. It can be used for other QT widget
 *          applications as well.
 *
 * \version 1.1.0
 *
 * Copyright (C) 2019 - 2023, IDS Imaging Development Systems GmbH.
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

#include "display.h"

#include <QDebug>
#include <QGraphicsView>
#include <QImage>
#include <QWidget>

#include <cmath>


Display::Display(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new CustomGraphicsScene(this))
{
    this->setScene(m_scene);
}


void Display::onImageReceived(QImage image)
{
    m_scene->setImage(std::move(image));
    // make sure we fit the (background) image into view
    // NOTE: this is done since the scene view normally uses the items it displays
    // for determining what the bounding rect of the scene is,
    // but we want to always use the images as the "bounding rect" thus making sure it gets
    // shown correctly
    auto imageRect = getImageRect();
    m_scene->setSceneRect(imageRect);
    fitInView(imageRect, Qt::KeepAspectRatio);
}

QRectF Display::getImageRect()
{
    return m_scene->getImageRect();
}

void Display::mouseMoveEvent(QMouseEvent* event)
{
    if (m_drawROI)
    {
        event->accept();
        // relative to this widget
        auto newPos = event->localPos();
        if (m_dragRect == nullptr)
        {
            // map viewport pos newPos to scene
            auto scenePos = mapToScene(static_cast<int>(newPos.x()), static_cast<int>(newPos.y()));
            QRectF newRect(scenePos.x(), scenePos.y(), 0, 0);
            const int FULL_CHANNEL_8BIT = 255;
            m_dragRect = m_scene->addRect(newRect, QPen(QColor(FULL_CHANNEL_8BIT, 0, FULL_CHANNEL_8BIT)),
                QBrush(QColor::fromRgb(FULL_CHANNEL_8BIT, 0, FULL_CHANNEL_8BIT, 125)));
        }
        else
        {
            // newRect is in scene coordinates, so we have to map it first
            auto startPosScene = m_dragRect->rect().topLeft();
            QPoint viewPos = mapFromScene(startPosScene.x(), startPosScene.y());

            // Calculate new size
            // NOTE: width/height can be negative
            int width = static_cast<int>(newPos.x() - viewPos.x());
            int height = static_cast<int>(newPos.y() - viewPos.y());

            m_dragRect->setRect(QRectF(startPosScene.x(), startPosScene.y(), width, height));
        }
    }
}

void Display::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_drawROI)
    {
        event->accept();
        if (m_dragRect != nullptr)
        {
            emit newROI(m_dragRect->rect());
            m_scene->removeItem(m_dragRect);
        }
        m_dragRect = nullptr;
    }
}

void Display::toggleDrawROI()
{
    m_drawROI = !m_drawROI;
}

CustomGraphicsScene::CustomGraphicsScene(Display* parent)
    : QGraphicsScene(parent)
    , m_parent(parent)
{}


void CustomGraphicsScene::setImage(QImage image)
{
    m_image = std::move(image);
    update();
}


QRectF CustomGraphicsScene::getImageRect()
{
    // Display size
    auto displayWidth = static_cast<double>(m_parent->width());
    auto displayHeight = static_cast<double>(m_parent->height());

    // Image size
    auto imageWidth = static_cast<double>(m_image.width());
    auto imageHeight = static_cast<double>(m_image.height());

    // Calculate aspect ratio of the display
    double ratio1 = displayWidth / displayHeight;

    // Calculate aspect ratio of the image
    double ratio2 = imageWidth / imageHeight;

    if (ratio1 > ratio2)
    {
        // the height with must fit to the display height. So h remains and w must be scaled down
        imageWidth = displayHeight * ratio2;
        imageHeight = displayHeight;
    }
    else
    {
        // the image width must fit to the display width. So w remains and h must be scaled down
        imageWidth = displayWidth;
        imageHeight = displayWidth / ratio2;
    }

    double imagePosX = -1.0 * (imageWidth / 2.0);
    double imagePosY = -1.0 * (imageHeight / 2.0);

    // Remove digits afer point
    imagePosX = trunc(imagePosX);
    imagePosY = trunc(imagePosY);

    QRectF rect(imagePosX, imagePosY, imageWidth, imageHeight);
    return rect;
}

void CustomGraphicsScene::drawBackground(QPainter* painter, const QRectF&)
{

    QRectF rect = getImageRect();
    painter->drawImage(rect, m_image);
}
