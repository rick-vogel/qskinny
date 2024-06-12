/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#ifndef QSK_SELECTION_WINDOW_H
#define QSK_SELECTION_WINDOW_H

#include "QskDialogWindow.h"

class QskTextOptions;
class QskLinearBox;

class QSK_EXPORT QskSelectionWindow : public QskDialogWindow
{
    Q_OBJECT

    Q_PROPERTY( QString infoText READ infoText
        WRITE setInfoText NOTIFY infoTextChanged )

    Q_PROPERTY( QskTextOptions infoTextOptions READ infoTextOptions
        WRITE setInfoTextOptions NOTIFY infoTextOptionsChanged )

    Q_PROPERTY( QStringList entries READ entries
        WRITE setEntries NOTIFY entriesChanged FINAL )

    Q_PROPERTY( int selectedRow READ selectedRow
        WRITE setSelectedRow NOTIFY selectedRowChanged FINAL )

    using Inherited = QskDialogWindow;

  public:
    QskSelectionWindow( QWindow* parent = nullptr );
    ~QskSelectionWindow() override;

    void setInfoTextOptions( const QskTextOptions& );
    QskTextOptions infoTextOptions() const;

    QString infoText() const;

    Q_INVOKABLE QStringList entries() const;
    Q_INVOKABLE int selectedRow() const;
    Q_INVOKABLE QString selectedEntry() const;

  public Q_SLOTS:
    void setInfoText( const QString& );
    void setEntries( const QStringList& );
    void setSelectedRow( int row );

  Q_SIGNALS:
    void infoTextChanged( const QString& );
    void infoTextOptionsChanged( const QskTextOptions& );
    void selectedRowChanged( int );
    void selectedEntryChanged( const QString& );
    void entriesChanged();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

class QSK_EXPORT QskColorSelectionWindow : public QskSelectionWindow
{
    Q_OBJECT
  public:
      QskColorSelectionWindow(QWindow* parent = nullptr);
  public Q_SLOTS:
      void setEntries(const QVector<QColor>& entries);
  private:
      using QskSelectionWindow::setEntries;
      QskLinearBox* m_colors = nullptr;
};

#endif
