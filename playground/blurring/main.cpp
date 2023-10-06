/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include <SkinnyNamespace.h>

#include <QskSizePolicy.h>
#include <QskFunctions.h>
#include <QskControl.h>
#include <QskSetup.h>
#include <QskWindow.h>
#include <QskRgbValue.h>
#include <QskSkinManager.h>
#include <QskGridBox.h>
#include <QskLinearBox.h>
#include <QskTextLabel.h>
#include <QskPushButton.h>
#include <QskQuickItem.h>
#include <QskQuick.h>

#include <QApplication>
#include <QListWidget>
#include <QDebug>

#include <functional>
#include <vector>

#include <SkinnyShortcut.h>

#include <private/qquickshadereffect_p.h>
#include <private/qquickshadereffectsource_p.h>
#include <private/qquickitem_p.h>

#include <QQuickItem>
#include <QTimer>

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
        lowp vec4 tex = texture2D(source, coord);
        gl_FragColor = vec4(vec3(0.0, tex.yz), tex.a) * qt_Opacity;
        // gl_FragColor = vec4(1.0);
    })";

class MainView : public QskControl
{
    static void addBlurredBox(QQuickItem* item)
    {     
        auto* const layer = QQuickItemPrivate::get(item)->layer();
        layer->setProperty( "samples", { 1 } );
        // layer->setProperty( "sourceRect", QRectF{ 0, 0, 400, 400 } );
        // layer->setProperty( "textureSize", QSize{ 400, 400 } );
        layer->setProperty( "mipmap", { true } );
        layer->setProperty( "smooth", { true } );
        layer->setProperty( "enabled", { true } );        

        auto* const window = item->window();

        // - contentItem
        //  - mainView : gradient
        //   - root
        //    + blurring
        //    - box
        //     - buttonA
        //     - buttonB
        //     - buttonC

        const auto children = item->children();
        const auto childItems = item->childItems();

        auto* const source = new QQuickShaderEffectSource(item);
        item->childItems()[0]->stackBefore(source);
        // source->setPosition({100,100});
        // source->setSize({400,400});
        source->setSourceItem(item);
        source->setMipmap(true);
        source->setSourceRect({0,0,400,400});        
        source->setLive(true);
        source->setRecursive(false);
        source->setHideSource(true);
        source->setSamples(1);
       
        auto* const effect = new QQuickShaderEffect(item);
        effect->setSize({400,400});
        qskSetItemGeometry(effect, 0,0,400,400);
        #if true
        effect->setVertexShader(vertexShader);
        effect->setFragmentShader(fragmentShader);
        auto sourceVariant = QVariant::fromValue<QQuickShaderEffectSource*>(source);
        effect->setProperty("source", sourceVariant);  
        effect->update();
        #endif

        qDebug() << item;
        qDebug() << source;
        qDebug() << effect;

        item->update();
    }

public:
    explicit MainView(QQuickItem* parent = nullptr) : QskControl(parent)
    {
        auto* const root = new QskBox(this);
        auto* const box = new QskBox(root); // item
        {
            // background
            auto* const effect = new QQuickShaderEffect(box);

            // foreground
            auto* const layout = new QskLinearBox(box); // child[0]    
            (void) new QskPushButton("A", layout);
            (void) new QskPushButton("B", layout);
            (void) new QskPushButton("C", layout);
        }
        box->setAutoLayoutChildren(true);
        box->setGradientHint(QskBox::Panel, {Qt::red, Qt::blue});
        box->setFixedSize(400,400);
        box->setGeometry(100,100,400,400);

        QTimer::singleShot(1000, this, [box](){addBlurredBox(box);});
    }
};

int main( int argc, char** argv )
{
    qskSkinManager->setPluginPaths({ R"(C:\repositories\qskinny_rveh-install-win\plugins)" });

    QGuiApplication app( argc, argv );

    Skinny::init(); // we need a skin
    SkinnyShortcut::enable( SkinnyShortcut::AllShortcuts );
    qskSetup->setSkin( "squiek" );

    QskWindow window;
    window.setColor( QskRgb::Wheat );
    window.addItem( new MainView(window.contentItem()) );
    window.resize( 800, 600 );
    window.show();

    return app.exec();
}
