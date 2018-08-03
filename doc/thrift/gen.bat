@echo on

set OUTPATH=../../src/public/protocol

thrift -r -gen cpp -out %OUTPATH% *.thrift

cd %OUTPATH%

del *.skeleton.cpp

setlocal ENABLEDELAYEDEXPANSION
set suf="*.*"
for /f "delims=" %%i in ('dir /b/s/a-d %suf%') do (
set h="%%~ni"
for %%j in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do set h="!h:%%j=%%j!"
ren "%%i" "!h!"%suf%
)
endlocal

pause