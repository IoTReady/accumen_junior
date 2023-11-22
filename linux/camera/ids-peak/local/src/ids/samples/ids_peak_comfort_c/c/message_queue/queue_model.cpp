/*!
 * \file    queue_model.cpp
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

#include "queue_model.h"
#include "queue_worker.h"

#include <QDateTime>
#include <QDebug>

QueueModel::QueueModel(QObject* parent)
    : QAbstractTableModel(parent)
{}

int QueueModel::rowCount(const QModelIndex& parent) const
{
    return m_messages.size();
}

int QueueModel::columnCount(const QModelIndex& parent) const
{
    return 5;
}

QVariant QueueModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return {};
    }

    if (index.row() >= m_messages.size())
    {
        return {};
    }

    switch (index.column())
    {
    case 0:
        // Time is in nsecs since epoch. See std::chrono::system_clock. Converting it to milliseconds here for QDateTime.
        return QDateTime::fromMSecsSinceEpoch(
            static_cast<qlonglong>(m_messages.at(index.row()).system_timestamp / 1'000'000))
            .toString("hh:mm:ss.zzz");

    case 1:
        return static_cast<qlonglong>(m_messages.at(index.row()).messageID);

    case 2:
        return m_messages.at(index.row()).camera;

    case 3:
        return QString::fromStdString(
            Queue::messageTypeToStr(static_cast<peak_message_type>(m_messages.at(index.row()).eventID)));

    case 4:
        return m_messages.at(index.row()).data;

    default:
        return {};
    }
}

QVariant QueueModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return {};
    }

    if (orientation == Qt::Horizontal)
    {
        if (section < m_header.size())
        {
            return m_header.at(section);
        }
        else
        {
            return {};
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void QueueModel::addMessage(uint64_t system_timestamp, uint64_t messageID, const QString& camera, int32_t eventID, const QVariant& data)
{
    QMutexLocker lck(&m_mutex);
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());

    m_messages.push_back(MessageData{ system_timestamp, messageID, camera, eventID, data });

    endInsertRows();

    newMessageInserted();
}

void QueueModel::addMessage(const MessageData& data)
{
    QMutexLocker lck(&m_mutex);
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());

    m_messages.push_back(data);

    endInsertRows();

    newMessageInserted();
}

void QueueModel::flush()
{
    QMutexLocker lck(&m_mutex);

    emit beginResetModel();

    m_messages.clear();

    emit endResetModel();
}
