cd /d %0\..
cd ..\ReleaseFunshion

ren ���С���Ƥ2����Ϸ��½��.EXE ���С���Ƥ2����Ϸ��½��.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\ReleaseFunshion\���С���Ƥ2����Ϸ��½��.EXE ..\ReleaseFunshion\���С���Ƥ2����Ϸ��½��.EX~

cd ..\ReleaseFunshion

del ���С���Ƥ2����Ϸ��½��.EX~


pause