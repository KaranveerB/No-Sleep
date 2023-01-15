#include "core_instance.h"

CoreInstance::CoreInstance() : QWidget() {
    createIcons();
    createActions();
    createTrayIcon();
    connectSignals();
    initState();
}

void CoreInstance::createIcons() {
    QSvgRenderer renderer;

    // paint enabled icon
    renderer.load(QLatin1String(":/icons/enabled.svg"));

    // TODO: Scale as needed
    // TODO: Make it work well on dark theme
    enabledPixmap = QPixmap(64, 64);
    enabledPixmap.fill(Qt::transparent);

    QPainter painter(&enabledPixmap);
    renderer.render(&painter);
    painter.end();

    // paint disabled icon
    renderer.load(QLatin1String(":/icons/disabled.svg"));

    disabledPixmap = QPixmap(64, 64);
    disabledPixmap.fill(Qt::transparent);

    painter.begin(&disabledPixmap);
    renderer.render(&painter);
    painter.end();
}


void CoreInstance::createActions() {
    enableAction = new QAction(tr("&Enable"), this);
    enableAction->setCheckable(true);
    disableAction = new QAction(tr("&Disable"), this);
    disableAction->setCheckable(true);
    forceEnableSleepAction = new QAction(tr("Force enable sleep"), this);
    quitAction = new QAction(tr("&Quit"), this);
}

void CoreInstance::connectSignals() {
    connect(enableAction, &QAction::triggered, this, &CoreInstance::enableNoSleep);
    connect(disableAction, &QAction::triggered, this, &CoreInstance::disableNoSleep);
    connect(forceEnableSleepAction, &QAction::triggered, this, &CoreInstance::forceEnableSleepOnLidClose);
    connect(quitAction, &QAction::triggered, this, &CoreInstance::quitProgram);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &CoreInstance::handleClickedAction);
    connect(qApp, &QApplication::aboutToQuit, this, &CoreInstance::quitProgram);
}

void CoreInstance::createTrayIcon() {
    auto *trayIconMenu = new QMenu(this);

    trayIconMenu->addAction(enableAction);
    trayIconMenu->addAction(disableAction);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction(forceEnableSleepAction);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);


    trayIcon->setToolTip("No Sleep enabled");
}

void CoreInstance::initState() {
    // we pretty much just copy enableNoSleep() with a different notification
    enableAction->setChecked(true);
    sleepController.setSleepOnLidClose(true);
    trayIcon->setToolTip("No Sleep enabled");
    setSystemTrayIcon(enabledPixmap);
    trayIcon->setVisible(true);
    createRunningNotification();
}

void CoreInstance::setSystemTrayIcon(const QPixmap &pixmap) {
    trayIcon->setIcon(pixmap);
}

void CoreInstance::createRunningNotification() {
    trayIcon->showMessage("No Sleep is running in the background",
                          "Sleep on lid close is currently disabled. It can be re-enabled by clicking the "
                          "tray icon, quitting the app, or shutting down");
}

void CoreInstance::enableNoSleep() {
    disableAction->setChecked(false);
    enableAction->setChecked(true); // force true as it can be triggered with clicking the icon and not the menu option
    sleepController.setSleepOnLidClose(false);
    setSystemTrayIcon(enabledPixmap);
    trayIcon->setToolTip("No Sleep enabled");
    trayIcon->showMessage("No Sleep enabled",
                          "Your computer will not sleep when you close the lid. If the program terminates "
                          " improperly, click \"Force enable sleep\"");
}

void CoreInstance::disableNoSleep() {
    enableAction->setChecked(false);
    disableAction->setChecked(true); // force true as it can be triggered with clicking the icon and not the menu option
    sleepController.setSleepOnLidClose(true);
    setSystemTrayIcon(disabledPixmap);
    trayIcon->setToolTip("No Sleep disabled");
    trayIcon->showMessage("No Sleep disabled", "Sleep settings restored to their original values.");
}


void CoreInstance::forceEnableSleepOnLidClose() {
    sleepController.forceSetDefaultLidCloseActionToSleep();
    enableAction->setChecked(false);
    disableAction->setChecked(true); // force true as it can be triggered with clicking the icon and not the menu option
    sleepController.setSleepOnLidClose(true);
    setSystemTrayIcon(disabledPixmap);
    trayIcon->setToolTip("No Sleep disabled");
    trayIcon->showMessage("Force enabled sleep on lid close",
                          "No Sleep can now be used as expected.");
}

void CoreInstance::handleClickedAction(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::ActivationReason::Context) {
        // do nothing
        return;
    }

    if (disableAction->isChecked()) {
        enableNoSleep();
    } else {
        // should only hit if enableAction->isChecked, but if there is a bug where neither is checked, better to
        // restore a safe state and disable No Sleep
        disableNoSleep();
    }
}

void CoreInstance::quitProgram() {
    sleepController.setSleepOnLidClose(true);
    trayIcon->showMessage("No Sleep restored system settings",
                          "Sleep settings restored to their original values. Use backup scripts in an emergency.");
    QApplication::processEvents();
    QApplication::quit();
}
