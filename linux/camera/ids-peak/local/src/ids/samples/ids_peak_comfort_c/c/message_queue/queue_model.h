/*!
 * \file    queue_model.h
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

#ifndef QUEUE_MODEL_H
#define QUEUE_MODEL_H

#include <QAbstractTableModel>
#include <QMutex>
#include <QVariant>
#include <cstdint>

struct MessageData
{
    uint64_t system_timestamp{};
    uint64_t messageID{};
    QString camera{};
    int32_t eventID{};
    QVariant data{};
};

Q_DECLARE_METATYPE(MessageData)

class QueueModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QueueModel(QObject* parent = nullptr);
    ~QueueModel() override = default;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void addMessage(uint64_t system_timestamp, uint64_t messageID, const QString& camera, int32_t eventID, const QVariant& data);
    void addMessage(const MessageData& data);
    void flush();

signals:
    void newMessageInserted();

private:
    mutable QMutex m_mutex;
    QList<MessageData> m_messages{};
    const QStringList m_header{ tr("Timestamp"), tr("ID"), tr("Camera"), tr("Event"), tr("Data") };
};


#endif // QUEUE_MODEL_H
