#include "qdatetime.h"
#include "qapplication.h"
#include "InterfaceModule/Headers/InterfaceModule.h"

using namespace interfacemodule;

int main(int argc, char *argv[]) {
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	QApplication app(argc, argv);
	InterfaceModule interf;
	interf.showMaximized();

	return app.exec();
}
