#include <myGL/glew.h>
#include <toolkit.h>
#include <QApplication>
#include <qsurfaceformat.h>
#include <nsfile_os.h>
#include <nsstring.h>

#ifdef NSDEBUG
#include <nsdebug.h>
#endif

#ifdef WIN32
#include <Windows.h>
#else
#endif

void qt_run(QApplication & a, int & ret_code);
void app_exec(QApplication & app, int & ret_code);

int main(int argc, char *argv[])
{
    int ret_code;
	QApplication a(argc, argv);
    Toolkit tk;
    tk.init();
    tk.show();
    qt_run(a,ret_code);
    return ret_code;
}

#ifdef WIN32
void qt_run(QApplication & a, int & ret_code)
{
    __try
    {
        app_exec(a,ret_code);
    }
    __except (1)
    {
        //nsdebug_dump::save(nsfile_os::cwd()+"logs/crash_dump.dmp", GetExceptionInformation())
    }
}
#else
int qt_run(QApplication & a, int & ret_code)
{
    ret_code = a.exec();
}
#endif

void app_exec(QApplication & app, int & ret_code)
{
    ret_code = app.exec();
}
