#include <myGL/glew.h>
#include <toolkit.h>
#include <QApplication>
#include <qsurfaceformat.h>
#include <nsfile_os.h>
#include <nsstring.h>

#ifdef NSDEBUG
#include <nsdebug.h>
#ifdef WIN32
#include <Windows.h>
#else
#endif
void qt_run(QApplication & a, int & ret_code);
int filter_func(void *);
#endif

int main(int argc, char *argv[])
{
    int ret_code;
	QApplication a(argc, argv);
    Toolkit tk;
    tk.init();
    tk.show();
#ifdef NSDEBUG
    qt_run(a,ret_code);
    return ret_code;
#else
    return a.exec();
#endif
}

#ifdef NSDEBUG
#ifdef WIN32
void qt_run(QApplication & a, int & ret_code)
{
    __try
    {
        ret_code = a.exec();
    }
    __except (filter_func(GetExceptionInformation()))
    {
    }
}
#else
int qt_run(QApplication & a, int & ret_code)
{

}
#endif

int filter_func(void * param)
{
    return nsdebug_dump::save(nsfile_os::cwd()+"logs/crash_dump.dmp", param, nsdebug_dump::info_level_small);
}
#endif
