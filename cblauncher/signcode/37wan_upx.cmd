cd /d %0\..
cd ..\Release37Wan

ren 37wan����Ƥ2����Ϸ��½��.EXE 37wan����Ƥ2����Ϸ��½��.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\Release37Wan\37wan����Ƥ2����Ϸ��½��.EXE ..\Release37Wan\37wan����Ƥ2����Ϸ��½��.EX~

cd ..\Release37Wan

del 37wan����Ƥ2����Ϸ��½��.EX~


pause