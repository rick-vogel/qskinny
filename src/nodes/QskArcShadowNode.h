/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#ifndef QSK_ARC_SHADOW_NODE_H
#define QSK_ARC_SHADOW_NODE_H

#include "QskGlobal.h"
#include <qsgnode.h>

class QskArcMetrics;
class QskShadowMetrics;

class QskArcShadowNodePrivate;

class QskArcShadowNode : public QSGGeometryNode
{
  public:
    QskArcShadowNode();
    ~QskArcShadowNode() override;

    void update( const QRectF& rect, const QskArcMetrics& arcMetrics, const QColor& shadowColor,
        const QskShadowMetrics& shadowMetrics, qreal borderWidth );

  private:
    Q_DECLARE_PRIVATE( QskArcShadowNode )
};

#endif