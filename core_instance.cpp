#include "core_instance.h"

CoreInstance::CoreInstance() : QWidget() {
    createActions();
    createTrayIcon();
}

void CoreInstance::createActions() {
    clickedAction = new QAction(tr("&clicked"), this);
}

void CoreInstance::createTrayIcon() {
    auto *trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(clickedAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon favicon = QIcon(":/icons/temp.png");
    if (favicon.availableSizes().length() == 0) {
        throw std::runtime_error("couldn't load icon");
    }
    trayIcon->setIcon(favicon);
    trayIcon->setVisible(true);

    createRunningNotification();
}

void CoreInstance::createRunningNotification() {
    trayIcon->showMessage("No Sleep is running in the background",
                          "Sleep is currently disabled. It can be re-enabled by clicking the tray icon, or by quitting the app or shutting down");
}