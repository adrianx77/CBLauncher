cd /d %0\..
cd ..\Release

ren �ٶȡ���Ƥ2����Ϸ��½��.EXE �ٶȡ���Ƥ2����Ϸ��½��.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\Release\�ٶȡ���Ƥ2����Ϸ��½��.EXE ..\Release\�ٶȡ���Ƥ2����Ϸ��½��.EX~

cd ..\Release

del �ٶȡ���Ƥ2����Ϸ��½��.EX~


pause