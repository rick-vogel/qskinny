/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskGridBox.h>
#include <QskLinearBox.h>

class DashboardPage : public QskLinearBox
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel )

    DashboardPage( QQuickItem* parent );

  private:
    QList< QskLinearBox* > m_columns;
};
