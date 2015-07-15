@echo off
cd "f:/Users/DreamInColor/My Documents/EmacsProjects/bbtoolkit"

if "%1"=="/c" (
    rm -r build
	mkdir build
)

cd build
call "F:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall"
qmake -spec win32-msvc2013 ..
msbuild /maxcpucount:8
cd ..