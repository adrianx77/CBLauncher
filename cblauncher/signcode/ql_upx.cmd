cd /d %0\..
cd ..\ReleaseQL

ren ўшўыб╢╗н╞д2б╖╙╬╧╖╡╟┬╜╞ў.EXE ўшўыб╢╗н╞д2б╖╙╬╧╖╡╟┬╜╞ў.EX~

cd ..\signcode

upx.exe -9 -k -v -o..\ReleaseQL\ўшўыб╢╗н╞д2б╖╙╬╧╖╡╟┬╜╞ў.EXE ..\ReleaseQL\ўшўыб╢╗н╞д2б╖╙╬╧╖╡╟┬╜╞ў.EX~

cd ..\ReleaseQL

del ўшўыб╢╗н╞д2б╖╙╬╧╖╡╟┬╜╞ў.EX~


pause