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

#include <KAboutData>
#include <KDBusService>
#include <KLocalizedString>

#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QtDBus>

#include <cstdio>


static const char description[] = I18N_NOOP("KDE creature for your desktop");

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("amor"));
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    QDBusConnection::sessionBus().registerObject( QStringLiteral( "/Amor" ),
                                                  new Amor() );
    KDBusService service(KDBusService::Unique);

    KLocalizedString::setApplicationDomain("amor");

    KAboutData about( QStringLiteral("amor"), i18n( "amor" ),
                      QStringLiteral(AMOR_VERSION) );
    about.setLicense( KAboutLicense::GPL );
    about.setShortDescription( i18n( description ) );
    about.setCopyrightStatement( i18n(
        "1999 by Martin R. Jones\n2010 by Stefan Böhmann" ) );

    about.addAuthor(
        i18n( "Stefan Böhmann" ),
        i18n( "Current maintainer" ),
        QStringLiteral("kde@hilefoks.org"),
        QStringLiteral("http://www.hilefoks.org"),
        QStringLiteral("hilefoks")
    );

    about.addAuthor(
        i18n("Martin R. Jones"),
        QStringLiteral(),
        QStringLiteral("mjones@kde.org")
    );

    about.addAuthor(
        i18n( "Gerardo Puga" ),
        QStringLiteral(),
        QStringLiteral("gpuga@gioia.ing.unlp.edu.ar")
    );

    about.addAuthor(
        i18n("Lorenzo Combatti"),
        QStringLiteral(),
        QStringLiteral("combatti@gmail.com")
    );

    QCommandLineParser parser;
    KAboutData::setApplicationData(about);
    parser.addVersionOption();
    parser.addHelpOption();
    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    return QApplication::exec();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
