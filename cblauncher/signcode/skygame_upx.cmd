cd /d %0\..
cd ..\ReleaseSkyGame

ren ���󡶻�Ƥ2����Ϸ��½��.EXE ���󡶻�Ƥ2����Ϸ��½��.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\ReleaseSkyGame\���󡶻�Ƥ2����Ϸ��½��.EXE ..\ReleaseSkyGame\���󡶻�Ƥ2����Ϸ��½��.EX~

cd ..\ReleaseSkyGame

del ���󡶻�Ƥ2����Ϸ��½��.EX~


pause