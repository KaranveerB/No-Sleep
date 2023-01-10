#pragma once

#include <QApplication>
#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>

class CoreInstance : private QWidget {
Q_OBJECT

public:
	CoreInstance();
private:
	void createActions();
	void createTrayIcon();
    void createRunningNotification();

	QSystemTrayIcon *trayIcon;

    QAction *enableAction;
    QAction *disableAction;
    QAction *quitAction;

};
