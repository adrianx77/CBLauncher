cd /d %0\..
cd ..\Release

ren 百度《画皮2》游戏登陆器.EXE 百度《画皮2》游戏登陆器.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\Release\百度《画皮2》游戏登陆器.EXE ..\Release\百度《画皮2》游戏登陆器.EX~

cd ..\Release

del 百度《画皮2》游戏登陆器.EX~


pause