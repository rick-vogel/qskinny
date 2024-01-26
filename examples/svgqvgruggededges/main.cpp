/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include <QGuiApplication.h>

#include <QskWindow.h>
#include <QskGraphicProvider.h>
#include <QskGraphic.h>
#include <QskGraphicIO.h>
#include <QskGridBox.h>
#include <QskGraphicLabel.h>
#include <QskSlider.h>
#include <QskSetup.h>
#include <QskSkin.h>

class GraphicProvider : public QskGraphicProvider
{
protected:
    const QskGraphic* loadGraphic( const QString& id ) const override
    {
        const auto path = QStringLiteral( ":/%1.qvg" ).arg(id);
        const auto graphic = QskGraphicIO::read( path );
        return graphic.isNull() ? nullptr : new QskGraphic( graphic );
    }
};

class Window : public QskWindow
{
public:
    Window(QWindow* parent = nullptr) : QskWindow(parent)
    {
        auto* const layout = new QskGridBox(contentItem());
        auto* const label = new QskGraphicLabel(QString{"01.13.05h"}, contentItem());
        auto* const sliderX = new QskSlider(layout);
        auto* const sliderY = new QskSlider(Qt::Vertical, layout);

        label->setObjectName("uut");

        sliderX->setValue(0.0);
        sliderY->setValue(0.0);

        layout->addItem(label, 0,0);
        layout->addItem(sliderY, 0,1);
        layout->addItem(sliderX, 1,0);

        label->setFillMode(QskGraphicLabel::FillMode::PreserveAspectFit);
        label->setFixedSize(250,250);

        auto updateLabel = [label, sliderX, sliderY](qreal v){   
            auto x = label->window()->width() * sliderX->value();
            auto y = label->window()->height() * sliderY->value();
            // const auto newGeometry = QRectF{{x,y + 0.1}, label->geometry().size()};
            const auto newGeometry = QRectF{{x,y}, label->geometry().size()};
            qDebug() << "old geometry: " << label->geometry() << "new geometry" << newGeometry;
            label->setGeometry(newGeometry);
        };

        QObject::connect(sliderX, &QskSlider::valueChanged, label, updateLabel);
        QObject::connect(sliderY, &QskSlider::valueChanged, label, updateLabel);
    }
};

int main( int argc, char* argv[] )
{
    QGuiApplication app( argc, argv );
   
    qskSetup->skin()->addGraphicProvider("", new GraphicProvider);

    Window window;
    window.resize( 600, 600 );
    window.show();

    return app.exec();
}
