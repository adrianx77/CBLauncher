cd /d %0\..
cd ..\Release4399

ren 4399¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EXE 4399¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\Release4399\4399¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EXE ..\Release4399\4399¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~

cd ..\Release4399

del 4399¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~


pause