/*!
 * \file    display.cpp
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

#include "display.h"

#include <QHBoxLayout>


Display::Display(QWidget* parent)
    : QWidget(parent)
{
    m_scene = new CustomGraphicsScene(this);

    m_view = new QGraphicsView;
    m_view->setScene(m_scene);

    auto* layout = new QHBoxLayout;
    layout->addWidget(m_view);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
}

void Display::onImageReceived(QImage image)
{
    m_scene->setSceneRect(image.rect());
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    
    m_scene->setImage(std::move(image));
}

CustomGraphicsScene::CustomGraphicsScene(QWidget* parent)
    : QGraphicsScene(parent)
{}

void CustomGraphicsScene::setImage(QImage image)
{
    QMutexLocker lock{ &m_mutex };
    m_image = std::move(image);
    update();
}

void CustomGraphicsScene::drawBackground(QPainter* painter, const QRectF&)
{
    QMutexLocker lock{ &m_mutex };
    painter->drawImage(0, 0, m_image);
}
