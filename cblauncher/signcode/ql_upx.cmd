cd /d %0\..
cd ..\ReleaseQL

ren ���롶��Ƥ2����Ϸ��½��.EXE ���롶��Ƥ2����Ϸ��½��.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\ReleaseQL\���롶��Ƥ2����Ϸ��½��.EXE ..\ReleaseQL\���롶��Ƥ2����Ϸ��½��.EX~

cd ..\ReleaseQL

del ���롶��Ƥ2����Ϸ��½��.EX~


pause