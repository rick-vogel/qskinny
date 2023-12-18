/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "QskSelectionSubWindow.h"
#include "QskLinearBox.h"
#include "QskSimpleListBox.h"
#include "QskTextLabel.h"

namespace
{
    class TextLabel final : public QskTextLabel
    {
      public:
        TextLabel( QskSelectionSubWindow* subWindow )
        {
            setObjectName( QStringLiteral( "QskSelectionSubWindowTextLabel" ) );
            initSizePolicy( QskSizePolicy::Preferred, QskSizePolicy::Constrained );

            setAlignment( Qt::AlignLeft | Qt::AlignTop );
            setWrapMode( QskTextOptions::WordWrap );

            connect( this, &QskTextLabel::textChanged,
                subWindow, &QskSelectionSubWindow::infoTextChanged );

            connect( this, &QskTextLabel::textOptionsChanged,
                subWindow, &QskSelectionSubWindow::infoTextOptionsChanged );
        }
    };

    class ListBox final : public QskSimpleListBox
    {
      public:
        ListBox( QskSelectionSubWindow* subWindow )
        {
            setObjectName( QStringLiteral( "QskSelectionSubWindowListBox" ) );

            connect( this, &QskSimpleListBox::selectedRowChanged,
                subWindow, &QskSelectionSubWindow::selectedRowChanged );

            connect( this, &QskSimpleListBox::selectedEntryChanged,
                subWindow, &QskSelectionSubWindow::selectedEntryChanged );

            connect( this, &QskSimpleListBox::entriesChanged,
                subWindow, &QskSelectionSubWindow::entriesChanged );
        }
    };
}

class QskSelectionSubWindow::PrivateData
{
  public:
    QskTextLabel* textLabel;
    QskSimpleListBox* listBox;
};

QskSelectionSubWindow::QskSelectionSubWindow( QQuickItem* parent )
    : Inherited( parent )
    , m_data( new PrivateData )
{
    m_data->textLabel = new TextLabel( this );
    m_data->textLabel->setVisible( false );

    m_data->listBox = new ListBox( this );
#if 1
    m_data->listBox->setPreferredSize( 500, 500 );
#endif

    auto box = new QskLinearBox( Qt::Vertical );
    box->setSpacing( 10 ); // hint
    box->addItem( m_data->textLabel );
    box->addItem( m_data->listBox );
    box->setStretchFactor( 1, 10 );

    setContentItem( box );
    setDialogActions( QskDialog::Ok | QskDialog::Cancel );    
}

QskSelectionSubWindow::~QskSelectionSubWindow()
{
}

void QskSelectionSubWindow::setInfoText( const QString& text )
{
    m_data->textLabel->setText( text );
    m_data->textLabel->setVisible( !text.isEmpty() );
}

QString QskSelectionSubWindow::infoText() const
{
    return m_data->textLabel->text();
}

void QskSelectionSubWindow::setInfoTextOptions( const QskTextOptions& options )
{
    if ( options != infoTextOptions() )
    {
        m_data->textLabel->setTextOptions( options );
        Q_EMIT infoTextOptionsChanged( options );
    }
}

QskTextOptions QskSelectionSubWindow::infoTextOptions() const
{
    return m_data->textLabel->textOptions();
}

void QskSelectionSubWindow::setEntries( const QStringList& entries )
{
    m_data->listBox->setEntries( entries );
}

QStringList QskSelectionSubWindow::entries() const
{
    return m_data->listBox->entries();
}

void QskSelectionSubWindow::setSelectedRow( int row )
{
    m_data->listBox->setSelectedRow( row );
    Q_EMIT selectedRowChanged( row );
}

int QskSelectionSubWindow::selectedRow() const
{
    return m_data->listBox->selectedRow();
}

QString QskSelectionSubWindow::selectedEntry() const
{
    return m_data->listBox->selectedEntry();
}

#include <QskScrollArea.h>
#include <QskLinearBox.h>
#include <QskGridBox.h>
#include <QskPushButton.h>
#include <QskBoxBorderColors.h>
#include <QskBoxBorderMetrics.h>

QskColorSelectionSubWindow::QskColorSelectionSubWindow( QQuickItem* parent )
    : QskSelectionSubWindow( parent )
{
    auto* const layout = static_cast<QskLinearBox*>(contentItem());
    layout->itemAtIndex(layout->elementCount() - 1)->setVisible(false);

    auto* const scroll = new QskScrollArea(layout);
    scroll->setMinimumHeight(60 * 6);
    scroll->setMinimumWidth( 60 * 6 );
    m_colors = new QskGridBox(scroll);
    m_colors->setDefaultAlignment( Qt::AlignCenter );
    scroll->setScrolledItem(m_colors);
    scroll->setItemResizable(true);
    layout->setStretchFactor(1, 0);
    layout->setStretchFactor(2, 10);
}

void QskColorSelectionSubWindow::setEntries( const QVector< QColor >& entries )
{
    m_colors->clear();    
    int index = 0;

    const auto cols = 6;
    const auto rows = 6;

    for ( const auto& color : qAsConst( entries ) )
    {
        using Q = QskPushButton;
        auto* const box = new Q;
        box->setColor( Q::Panel, color );
        box->setColor( Q::Splash, Qt::transparent );
        box->setFixedSize( 48, 48 );
        box->setCheckable( true );
        box->setBoxBorderColorsHint(Q::Panel | Q::Checked, Qt::black);
        box->setBoxBorderMetricsHint(Q::Panel | Q::Checked, 2);

        m_colors->addItem(box, index / cols, index % cols);

        connect(box, &QskPushButton::checkedChanged, this, [this, index](bool checked){
            if(checked)
                setSelectedRow( index );
        });

        connect(this, &QskColorSelectionSubWindow::selectedRowChanged, box, [box, index](int selected){
            box->setChecked(index == selected);
        });

        index++;
    }

    m_colors->setMaximumHeight(entries.count() / cols * 60);
}

#include "moc_QskSelectionSubWindow.cpp"
