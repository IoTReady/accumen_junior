/*!
 * \file    camera_dialog.cpp
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

#include "camera_dialog.h"
#include "backend.h"

#include <QAbstractButton>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>

CameraListDialog::CameraListDialog(QWidget* parent)
    : QDialog(parent)
{
    m_table = new QTableView(this);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_table->setSortingEnabled(true);
    m_table->setSelectionBehavior(QTableView::SelectRows);
    m_table->setSelectionMode(QTableView::ExtendedSelection);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setDefaultSectionSize(m_table->verticalHeader()->minimumSectionSize());
    m_table->verticalHeader()->hide();
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setHighlightSections(false);
    m_table->horizontalHeader()->setSortIndicatorShown(false);

    m_model = new CameraListModel(this);
    m_table->setModel(m_model);

    auto* layout = new QVBoxLayout();
    layout->addWidget(new QLabel(tr("Select cameras to enable messages:")));
    layout->addWidget(m_table);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

    layout->addWidget(m_buttonBox);
    setLayout(layout);

    m_table->resizeColumnsToContents();

    m_buttonBox->buttons().first()->setEnabled(m_model->cameraCount() != 0);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_table, &QTableView::doubleClicked, this, &QDialog::accept);
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &CameraListDialog::onSelectionChanged);

    setMinimumSize(500, 200);

    setWindowTitle(tr("Select cameras to enable messages"));

    if (parent)
    {
        QPoint dialogCenter = mapToGlobal(rect().center());
        QPoint parentWindowCenter = parent->window()->mapToGlobal(parent->window()->rect().center());
        move(parentWindowCenter - dialogCenter);
    }
}

void CameraListDialog::accept()
{
    auto selected_rows = m_table->selectionModel()->selectedRows();
    for (auto& index : selected_rows)
    {
        auto camID = m_model->data(m_model->index(index.row(), 0)).toULongLong();
        auto model = m_model->data(m_model->index(index.row(), 3)).toString();
        auto serial = m_model->data(m_model->index(index.row(), 4)).toString();

        m_selectedCamera.push_back(QPair<QString, peak_camera_id>{ model + " " + serial,
            static_cast<peak_camera_id>(camID) });
    }

    QDialog::accept();
}

void CameraListDialog::onSelectionChanged()
{
    auto selected_rows = m_table->selectionModel()->selectedRows();
    m_buttonBox->buttons().first()->setEnabled(!selected_rows.empty());
}
