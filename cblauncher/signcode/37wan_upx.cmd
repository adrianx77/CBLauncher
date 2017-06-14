cd /d %0\..
cd ..\Release37Wan

ren 37wan¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EXE 37wan¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\Release37Wan\37wan¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EXE ..\Release37Wan\37wan¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~

cd ..\Release37Wan

del 37wan¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~


pause