#include <myGL/glew.h>
#include <toolkit.h>
#include <QApplication>
#include <qsurfaceformat.h>
#include <nsfile_os.h>
#include <nsstring.h>

#ifdef NSDEBUG
#include <nslog_file.h>
#include <nsdebug.h>
#ifdef WIN32
#include <Windows.h>
int filter_func(void *);
#else
#include <sys/resource.h>
#endif
void qt_run(QApplication & a, int & ret_code);
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

int filter_func(void * param)
{
    return nsdebug_dump::save(nsfile_os::cwd()+"logs/crash_dump.dmp", param, nsdebug_dump::info_level_small);
}
#else
void qt_run(QApplication & a, int & ret_code)
{
    struct rlimit corelim;
    corelim.rlim_cur = -1;
    corelim.rlim_max = -1;
    if (setrlimit (RLIMIT_CORE, &corelim) != 0)
        nslog_file::write_to("Could not write core dump","core_d.log", nslog_file::wm_overwrite);
    ret_code = a.exec();
}
#endif
#endif
