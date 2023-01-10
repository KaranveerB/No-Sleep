#include <QApplication>

#include "core_instance.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	CoreInstance coreInstance;
	return QCoreApplication::exec();
}
