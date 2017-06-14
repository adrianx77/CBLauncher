cd /d %0\..
cd ..\ReleaseFunshion

ren 风行《画皮2》游戏登陆器.EXE 风行《画皮2》游戏登陆器.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\ReleaseFunshion\风行《画皮2》游戏登陆器.EXE ..\ReleaseFunshion\风行《画皮2》游戏登陆器.EX~

cd ..\ReleaseFunshion

del 风行《画皮2》游戏登陆器.EX~


pause