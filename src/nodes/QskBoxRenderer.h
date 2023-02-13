/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_BOX_RENDERER_H
#define QSK_BOX_RENDERER_H

#include "QskGlobal.h"

class QskBoxBorderMetrics;
class QskBoxBorderColors;
class QskBoxShapeMetrics;
class QskGradient;

class QSGGeometry;
class QRectF;

namespace QskBox
{
    /*
        Filling the geometry without any color information:
            see QSGGeometry::defaultAttributes_Point2D()

        - clip nodes
        - using shaders setting the color information
     */

    QSK_EXPORT void renderBorderGeometry( const QRectF&,
        const QskBoxShapeMetrics&, const QskBoxBorderMetrics&, QSGGeometry& );

    QSK_EXPORT void renderFillGeometry( const QRectF&,
        const QskBoxShapeMetrics&, const QskBoxBorderMetrics&, QSGGeometry& );

    QSK_EXPORT void renderFillGeometry( const QRectF&,
        const QskBoxShapeMetrics&, QSGGeometry& );

    /*
        Filling the geometry usually with color information:
            see QSGGeometry::defaultAttributes_ColoredPoint2D()
     */
    QSK_EXPORT bool isGradientSupported( const QskBoxShapeMetrics&, const QskGradient& );

    QSK_EXPORT void renderBox( const QRectF&,
        const QskBoxShapeMetrics&, const QskBoxBorderMetrics&,
        const QskBoxBorderColors&, const QskGradient&, QSGGeometry& );

    QSK_EXPORT void renderBox( const QRectF&,
        const QskBoxShapeMetrics&, const QskGradient&, QSGGeometry& );
}

#endif
