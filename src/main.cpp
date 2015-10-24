#include <myGL/glew.h>
#include <toolkit.h>
#include <QApplication>
#include <qsurfaceformat.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    Toolkit tk;
    tk.init();
    tk.show();
	return a.exec();
}
