/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include <SkinnyNamespace.h>

#include <QskControl.h>
#include <QskFunctions.h>
#include <QskGridBox.h>
#include <QskLinearBox.h>
#include <QskPushButton.h>
#include <QskQuick.h>
#include <QskQuickItem.h>
#include <QskRgbValue.h>
#include <QskSetup.h>
#include <QskSizePolicy.h>
#include <QskSkinManager.h>
#include <QskTextLabel.h>
#include <QskWindow.h>
#include <QskBoxBorderColors.h>
#include <QskBoxBorderMetrics.h>
#include <QskGraphicLabel.h>
#include <QskGraphic.h>
#include <QskGraphicProvider.h>
#include <QskAnimator.h>

#include <QApplication>
#include <QDebug>
#include <QListWidget>

#include <functional>
#include <vector>

#include <SkinnyShortcut.h>

#include <private/qquickitem_p.h>
#include <private/qquickshadereffect_p.h>
#include <private/qquickshadereffectsource_p.h>

#include <QQuickItem>

const char* vertexShader = R"(
    uniform highp mat4 qt_Matrix;
    attribute highp vec4 qt_Vertex;
    attribute highp vec2 qt_MultiTexCoord0;
    varying highp vec2 coord;
    void main() {
        coord = qt_MultiTexCoord0;
        gl_Position = qt_Matrix * qt_Vertex;
    })";

const char* fragmentShader = R"(
    varying highp vec2 coord;
    uniform sampler2D source;
    uniform lowp float qt_Opacity;
    
    void main() {
        vec2 delta = vec2(0.01, 0.01);
        gl_FragColor =(0.0538 * texture2D(source, coord - 3.182 * delta)
                     + 0.3229 * texture2D(source, coord - 1.364 * delta)
                     + 0.2466 * texture2D(source, coord)
                     + 0.3229 * texture2D(source, coord + 1.364 * delta)
                     + 0.0538 * texture2D(source, coord + 3.182 * delta)) * qt_Opacity;
    })";

class MainView : public QskControl
{
  public:
    explicit MainView( QQuickItem* parent = nullptr )
        : QskControl( parent )
    {
        setAutoLayoutChildren( true );        
       
        auto* const layout = new QskLinearBox(Qt::Vertical, this);
        layout->setPanel(true);
        layout->setGradientHint(QskLinearBox::Panel, {Qt::red, Qt::blue});

        auto* const header = new QskPushButton("Header", layout);

        auto* const graphic = new QskGraphicLabel( layout );
        graphic->setFillMode( QskGraphicLabel::FillMode::Stretch );
        graphic->setAlignment( Qt::AlignCenter );
        graphic->setMaximumSize(400,300);
        const QImage image( QString( R"(C:\Users\vrick\Pictures\cd78c06d5f34aab556aa39ddb493e964.jpg)" ));
        graphic->setGraphic( QskGraphic::fromImage( image ) );

        class Animator : public QObject, public QskAnimator
        {
          public:
            explicit Animator(QskGraphicLabel& label, QObject* parent = nullptr) : QObject(parent), label(label)
            {
            }
          private:
            void advance(qreal v) override
            {
                const auto x = qCos(M_PI * 2.0 * v) * 10;
                const auto y = qSin(M_PI * 2.0 * v) * 10;
                label.setPosition(x,y);
            }

            QskGraphicLabel& label;
        };

        auto* const animator = new Animator(*graphic, graphic);
        animator->setWindow(window());
        animator->setDuration(1000);
        animator->setAutoRepeat(true);
        animator->start();

        auto* const footer = new QskPushButton("Footer", layout);
        {
            // Object Tree
            // - contentItem
            //  - mainView
            //   - layout
            //    - box

            // Object Item Tree
            // - contentItem
            //  - mainView
            //   - layout
            //    - box
            //  - blurred

            auto* const parentItem = this;
            auto* const sourceItem = this;

            // background

            // see: https://doc.qt.io/qt-5/qml-qtquick-item.html#layer.enabled-prop
            auto* const layer = QQuickItemPrivate::get( sourceItem )->layer();
            layer->setProperty( "enabled", { true } );

            auto* const dialog = new QskControl(window()->contentItem());
            dialog->setGeometry( 100, 100, 600, 400 );

            auto* const source = new QQuickShaderEffectSource( dialog );
            source->setSourceItem( sourceItem );
            source->setSourceRect(dialog->geometry());
            source->setLive( true );
            source->setRecursive( true );

            auto* const effect = new QQuickShaderEffect( dialog );
            effect->setVertexShader( vertexShader );
            effect->setFragmentShader( fragmentShader );
            effect->setProperty( "source", QVariant::fromValue< QQuickShaderEffectSource* >( source ) );
            effect->setVisible(true);
            effect->update();

            // foreground
            auto* const layout = new QskLinearBox( dialog );
            ( void ) new QskPushButton( "Foreground", layout );
            ( void ) new QskPushButton( "Foreground", layout );
            ( void ) new QskPushButton( "Foreground", layout );

            layout->setGeometry(dialog->geometry());
            qskSetItemGeometry( effect, layout->geometry() );
        }
    }
};

int main( int argc, char** argv )
{
    qskSkinManager->setPluginPaths( { R"(C:\repositories\qskinny_rveh-install-win\plugins)" } );

    QGuiApplication app( argc, argv );

    Skinny::init(); // we need a skin
    SkinnyShortcut::enable( SkinnyShortcut::AllShortcuts );
    qskSetup->setSkin( "squiek" );

    QskWindow window;
    window.addItem( new MainView( window.contentItem() ) );
    window.resize( 800, 600 );
    window.show();

    return app.exec();
}
