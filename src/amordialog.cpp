/*
 * Copyright 1999 by Martin R. Jones <mjones@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "amordialog.h"

#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include <QSlider>
#include <KTextEdit>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KConfig>
#include <KLocalizedString>
#include <QIcon>

#include <QGridLayout>
#include <QStandardPaths>
#include <KSharedConfig>
#include <QDialogButtonBox>

#include <QPushButton>
#include <QDir>
#include <KConfigGroup>

AmorDialog::AmorDialog(QWidget *parent)
  : QDialog( parent )
{
    QWidget::setWindowTitle( i18n( "Options" ) );
    //setButtons( Ok | Apply | Cancel );
    //setDefaultButton( Ok );

    mConfig.read();
    //QWidget *mainwidget = new QWidget( this );
    //setMainWidget( mainwidget );

    QGridLayout *gridLayout = new QGridLayout( this );
    gridLayout->setMargin( 0 );

    QLabel *label = new QLabel( i18n( "Theme:" ), this );
    gridLayout->addWidget( label, 0, 0 );

    mThemeListView = new QListWidget( this );
    mThemeListView->setIconSize( QSize( 32, 32 ) );
    mThemeListView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    mThemeListView->setAlternatingRowColors( true );
    connect( mThemeListView, SIGNAL(currentRowChanged(int)), SLOT(slotHighlighted(int)) );
    mThemeListView->setMinimumSize( fontMetrics().maxWidth()*20, fontMetrics().lineSpacing()*6 );
    gridLayout->addWidget( mThemeListView, 1, 0 );

    mAboutEdit = new KTextEdit( this );
    mAboutEdit->setReadOnly( true );
    mAboutEdit->setMinimumHeight( fontMetrics().lineSpacing()*4 );
    gridLayout->addWidget( mAboutEdit, 2, 0 );

    // Animation offset
    label = new QLabel( i18n("Offset:"), this );
    gridLayout->addWidget( label, 0, 1 );

    QSlider *slider = new QSlider( Qt::Vertical, this );
    slider->setRange( -40, 40 );
    slider->setPageStep( 5 );
    slider->setValue( mConfig.mOffset );
    connect( slider, SIGNAL(valueChanged(int)), SLOT(slotOffset(int)) );
    gridLayout->addWidget( slider, 1, 1, 2, 1 );

    // Always on top
    QCheckBox *checkBox = new QCheckBox( i18n( "Always on top" ), this );
    connect( checkBox, SIGNAL(toggled(bool)), SLOT(slotOnTop(bool)) );
    checkBox->setChecked( mConfig.mOnTop );
    gridLayout->addWidget( checkBox, 3, 0, 1, 2 );

    checkBox = new QCheckBox( i18n( "Show random tips" ), this );
    connect( checkBox, SIGNAL(toggled(bool)), SLOT(slotRandomTips(bool)) );
    checkBox->setChecked( mConfig.mTips ); // always keep this one after the connect, or the QList would not be grayed when it should
    gridLayout->addWidget( checkBox, 4, 0, 1, 2 );

    checkBox = new QCheckBox( i18n( "Use a random character" ), this );
    connect( checkBox, SIGNAL(toggled(bool)), SLOT(slotRandomTheme(bool)) );
    checkBox->setChecked( mConfig.mRandomTheme );
    gridLayout->addWidget( checkBox, 5, 0, 1, 2 );

    checkBox = new QCheckBox( i18n( "Allow application tips" ), this );
    connect( checkBox, SIGNAL(toggled(bool)), SLOT(slotApplicationTips(bool)) );
    checkBox->setChecked( mConfig.mAppTips );
    gridLayout->addWidget( checkBox, 6, 0, 1, 2 );
    /*
    connect( this, SIGNAL(okClicked()), SLOT(slotOk()) );
    connect( this, SIGNAL(applyClicked()), SLOT(slotApply()) );
    connect( this, SIGNAL(cancelClicked()), SLOT(slotCancel()) );
    */
    readThemes(); // LC: maybe goes to the end?
    
    // LC: trying to replace KDialog::setButtons()
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Apply |
                                          QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    gridLayout->addWidget(buttonBox, 7, 0, 1, 2);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AmorDialog::slotOk);
    auto applyButton = buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, &QPushButton::pressed, this, &AmorDialog::slotApply);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AmorDialog::slotCancel);
    
    this->setLayout(gridLayout);
}


void AmorDialog::readThemes()   // LC: https://community.kde.org/Frameworks/Porting_Notes/KStandardDirs
{
    QStringList files;
    QStringList dirs;

    // Non-recursive search for theme files, with the relative paths stored
    // in files so that absolute paths are not used.
    dirs = QStandardPaths::locateAll(QStandardPaths::AppDataLocation, QLatin1String());

    for (auto d : dirs) {
        const QStringList fileNames = QDir(d).entryList(QStringList() << QStringLiteral("*.rc"));
        for (auto f : fileNames) {
            files.append(f);
        }
    }
    
    for(QStringList::ConstIterator it = files.constBegin(); it != files.constEnd(); ++it) {
        addTheme( *it );
    }
}


void AmorDialog::addTheme(const QString& file)
{
    KConfig config( QStandardPaths::locate(QStandardPaths::AppDataLocation, file ) );
    KConfigGroup configGroup( &config, "Config" );

    QString pixmapPath = configGroup.readPathEntry( "PixmapPath", QString() );
    if( pixmapPath.isEmpty() ) {
        return;
    }

    pixmapPath += QLatin1Char( '/' );
    if( pixmapPath[0] != QLatin1Char( '/' ) ) {
        // relative to config file. We add a / to indicate the dir
        pixmapPath = QStandardPaths::locate(QStandardPaths::DataLocation, pixmapPath);
    }

    QString description = configGroup.readEntry( "Description" );
    QString about = configGroup.readEntry( "About", " " );
    QString pixmapName = configGroup.readEntry( "Icon" );

    pixmapPath += pixmapName;
    QPixmap pixmap( pixmapPath );

    QListWidgetItem *item = new QListWidgetItem( QIcon( pixmap ), description, mThemeListView );
    mThemes.append( file );
    mThemeAbout.append( about );

    if( mConfig.mTheme == file ) {
        mThemeListView->setCurrentItem( item );
    }
}


void AmorDialog::slotHighlighted(int index)
{
    if( index < 0 ) {
        return;
    }

    mConfig.mTheme = mThemes.at( index );
    mAboutEdit->setPlainText( mThemeAbout.at( index ) );
}


void AmorDialog::slotOffset(int off)
{
    mConfig.mOffset = off;
    emit offsetChanged( mConfig.mOffset );
}


void AmorDialog::slotOnTop(bool onTop)
{
    mConfig.mOnTop = onTop;
}


void AmorDialog::slotRandomTips(bool tips)
{
    mConfig.mTips = tips;
}


void AmorDialog::slotRandomTheme(bool randomTheme)
{
    mThemeListView->setEnabled( !randomTheme );
    mConfig.mRandomTheme = randomTheme;
}


void AmorDialog::slotApplicationTips(bool tips)
{
    mConfig.mAppTips = tips;
}


void AmorDialog::slotOk()
{
    mConfig.write();
    emit changed();
    accept();
}


void AmorDialog::slotApply()
{
    mConfig.write();
    emit changed();
}


void AmorDialog::slotCancel()
{
    // restore offset
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup cs( config, "General" );
    emit offsetChanged( cs.readEntry( "Offset", 0 ) );
    reject();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
