#include "toolkit.h"
#include <QApplication>
#include <nsglobal.h>
#include <qsurfaceformat.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Toolkit w;
	w.init();
	w.show();
	return a.exec();
}
