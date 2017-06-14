cd /d %0\..
cd ..\ReleaseSkyGame

ren ÓÆÑó¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EXE ÓÆÑó¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\ReleaseSkyGame\ÓÆÑó¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EXE ..\ReleaseSkyGame\ÓÆÑó¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~

cd ..\ReleaseSkyGame

del ÓÆÑó¡¶»­Æ¤2¡·ÓÎÏ·µÇÂ½Æ÷.EX~


pause