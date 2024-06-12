/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "DialogPage.h"

#include <QskDialog.h>
#include <QskLinearBox.h>
#include <QskPushButton.h>
#include <QskStandardSymbol.h>
#include <QskBoxShapeMetrics.h>

namespace
{
    class Button : public QskPushButton
    {
      public:
        Button( const QString& text, QQuickItem* parent = nullptr )
            : QskPushButton( text, parent )
        {
        }
    };

    class ButtonBox : public QskLinearBox
    {
      public:
        ButtonBox( QQuickItem* parent = nullptr )
            : QskLinearBox( Qt::Horizontal, 2, parent )
        {
            setObjectName( "ButtonBox" );

            setMargins( 10 );
            setSpacing( 20 );

            auto messageButton = new Button( "Message", this );
            connect( messageButton, &Button::clicked, this, &ButtonBox::execMessage );

            auto informationButton = new Button( "Information", this );
            connect( informationButton, &Button::clicked, this, &ButtonBox::execInformation );

            auto questionButton = new Button( "Question", this );
            connect( questionButton, &Button::clicked, this, &ButtonBox::execQuestion );

            auto warningButton = new Button( "Warning", this );
            connect( warningButton, &Button::clicked, this, &ButtonBox::execWarning );

            auto criticalButton = new Button( "Critical", this );
            connect( criticalButton, &Button::clicked, this, &ButtonBox::execCritical );

            auto selectButton = new Button( "Selection", this );
            connect( selectButton, &Button::clicked, this, &ButtonBox::execSelection );

            auto colorButton = new Button( "Colors", this );
            connect( colorButton, &Button::clicked, this, &ButtonBox::execColorSelection );

            setExtraSpacingAt( Qt::BottomEdge );
        }

      private:
        void execMessage()
        {
            qskDialog->message( "Message", "Request vector, over.",
                QskStandardSymbol::NoSymbol, QskDialog::Close );
        }

        void execInformation()
        {
            qskDialog->information( "Information",
                "We have clearance, Clarence." );
        }

        void execQuestion()
        {
            qskDialog->question( "Question",
                "Roger, Roger. Do we have a vector, Victor ?" );
        }

        void execWarning()
        {
            qskDialog->warning( "Warning", "We have clearance, Clarence." );
        }

        void execCritical()
        {
            qskDialog->critical( "Critical", "That's Clarence Oveur. Over." );
        }

        void execSelection()
        {
            // of course we all love "The Teens"
            const QStringList entries =
            {
                "Give Me More",
                "Gimme Gimme Your Love",
                "1-2-3-4 Red Light",
                "New York",
                "If You Walk Away",
                "Eloise",
                "On The Radio",
                "We Are The Teens",
                "Never Gonna Tell No Lie To You",
                "Walking On Sunshine ",
                "Get Out Of My Mind",
                "Cover Girl ",
                "Here I Stand",
                "Gypsy Caravan",
                "It´s Good To Have A Friend",
                "We´ll Have A Party Tonite ´nite",
                "Automatic World",
                "Gimme Gimme Gimme Gimme Gimme Your Love"
            };

            qskDialog->select( "Here we go ...", "Hot Hot Hot", entries, 7 );
        }

        void execColorSelection()
        {
            static const QVector<QColor> colors {
"#ffff0000","#ffff8700","#ffffd300","#ffdeff0a","#ffa1ff0a","#ff0aff99","#ff0aefff","#ff147df5","#ff580aff","#ffbe0aff"
            };

            qskDialog->select("Select a color...", "Select from palette...", colors);
        }
    };
}

DialogPage::DialogPage( QQuickItem* parent )
    : Page( Qt::Horizontal, parent )
{
    ( void ) new ButtonBox( this );
}
