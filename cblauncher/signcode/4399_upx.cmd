cd /d %0\..
cd ..\Release4399

ren 4399����Ƥ2����Ϸ��½��.EXE 4399����Ƥ2����Ϸ��½��.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\Release4399\4399����Ƥ2����Ϸ��½��.EXE ..\Release4399\4399����Ƥ2����Ϸ��½��.EX~

cd ..\Release4399

del 4399����Ƥ2����Ϸ��½��.EX~


pause