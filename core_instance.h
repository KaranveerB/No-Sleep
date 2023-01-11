#pragma once

#include <QApplication>
#include <QPainter>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QtSvg>
#include <QSvgRenderer>
#include <QWidget>

#include "sleep_controller.h"

class CoreInstance : public QWidget {
Q_OBJECT

public:
    CoreInstance();

private slots:

    void enableNoSleep();

    void disableNoSleep();

    void handleClickedAction(QSystemTrayIcon::ActivationReason reason);

    void quitProgram();

private:
    void createIcons();

    void createActions();

    void connectSignals();

    void createTrayIcon();


    void initState();

    void setSystemTrayIcon(const QPixmap& icon);

    void createRunningNotification();

    QSystemTrayIcon *trayIcon;

    QAction *enableAction;
    QAction *disableAction;
    QAction *quitAction;

    QPixmap disabledPixmap;
    QPixmap enabledPixmap;

    SleepController sleepController;
};
