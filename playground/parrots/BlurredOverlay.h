/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <qquickitem.h>

class BlurredOverlayPrivate;

class BlurredOverlay : public QQuickItem
{
    Q_OBJECT

    using Inherited = QQuickItem;

  public:
    BlurredOverlay( QQuickItem* = nullptr );
    ~BlurredOverlay() override;

    QQuickItem* grabbedItem() const;
    void setGrabbedItem( QQuickItem* );

  protected:
    void geometryChange( const QRectF&, const QRectF& ) override;
    QSGNode* updatePaintNode( QSGNode*, UpdatePaintNodeData* ) override;

  private:
    Q_DECLARE_PRIVATE( BlurredOverlay )
};
