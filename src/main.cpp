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

#include "amor.h"
#include "amorsessionwidget.h"
#include "version.h"

#include <cstdio>

#include <QtDBus>
#include <QApplication>
#include <QString>
#include <QCommandLineParser>

#include <KLocalizedString>
#include <KAboutData>

static const char description[] = I18N_NOOP("KDE creature for your desktop");

int main(int argc, char *argv[])
{
    //QApplication::setGraphicsSystem( QLatin1String( "native" ) );
    QApplication app(argc, argv); // PORTING SCRIPT: move this to before the KAboutData initialization

    KAboutData about( QLatin1String("amor"), i18n( "amor" ), QLatin1String(AMOR_VERSION) );
    about.setLicense( KAboutLicense::GPL );
    about.setShortDescription( i18n( description ) );
    about.setCopyrightStatement( i18n( "1999 by Martin R. Jones\n2010 by Stefan Böhmann" ) );

    about.addAuthor(
        i18n( "Stefan Böhmann" ),
        i18n( "Current maintainer" ),
        QLatin1String("kde@hilefoks.org"),
        QLatin1String("http://www.hilefoks.org"),
        QLatin1String("hilefoks")
    );

    about.addAuthor(
        i18n("Martin R. Jones"),
        QLatin1String(),
        QLatin1String("mjones@kde.org")
    );

    about.addAuthor(
        i18n( "Gerardo Puga" ),
        QLatin1String(),
        QLatin1String("gpuga@gioia.ing.unlp.edu.ar")
    );

    QCommandLineParser parser;
    KAboutData::setApplicationData(about);
    parser.addVersionOption();
    parser.addHelpOption();
    //PORTING SCRIPT: adapt aboutdata variable if necessary
    about.setupCommandLine(&parser);
    parser.process(app); // PORTING SCRIPT: move this to after any parser.addOption
    about.processCommandLine(&parser);
/*  LC: probably safe to ignore
    if( !KUniqueApplication::start() ) {
        std::fprintf( stderr, "%s is already running!\n", qPrintable( about.appName() ) );
        exit( 0 );
    }
*/
//    KUniqueApplication app;
    //AmorSessionWidget *sessionWidget = new AmorSessionWidget; // session management. LC: It seems does almost nothing, commenting out

    QDBusConnection::sessionBus().registerObject( QLatin1String( "/Amor" ),new Amor() );
    return app.exec();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
