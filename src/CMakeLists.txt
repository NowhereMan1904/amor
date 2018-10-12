set( amor_SRCS
  main.cpp
  queueitem.cpp
  amorsessionwidget.cpp
  amordialog.cpp
  amor.cpp
  amorwidget.cpp
  amoranimation.cpp
  amorthememanager.cpp
  amorpixmapmanager.cpp
  amorbubble.cpp
  amorconfig.cpp
  amortips.cpp
)

qt5_add_dbus_adaptor( amor_SRCS org.kde.amor.xml amor.h Amor )

add_executable( amor ${amor_SRCS} )
target_link_libraries( amor
    Qt5::Core
    Qt5::Widgets
    Qt5::DBus
    Qt5::X11Extras
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::Crash
    KF5::I18n
    KF5::TextWidgets
    KF5::CoreAddons
    KF5::WidgetsAddons
    KF5::WindowSystem
    KF5::XmlGui
    KF5::IconThemes
    KF5::DBusAddons
)

install( TARGETS amor  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( PROGRAMS amor.desktop     DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES org.kde.amor.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR} )

