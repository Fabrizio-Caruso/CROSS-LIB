color 07
copy ..\*.rom .
set PATH=%PATH%;c:\tools
pasmo --amsdos test.asm test.bin
if %ERRORLEVEL% NEQ 0 goto :errorend
cpcxfsw -f -nd test.dsk
if %ERRORLEVEL% NEQ 0 goto :errorend
cpcxfsw test.dsk -f -p test.bin test.bin
if %ERRORLEVEL% NEQ 0 goto :errorend
..\nocart test.dsk test.cpr -c RUN"""test
if %ERRORLEVEL% NEQ 0 goto :errorend
..\nocart -f
if %ERRORLEVEL% NEQ 0 goto :errorend
cpcxfsw -f -e format -f "DA3A" da3a.dsk
if %ERRORLEVEL% NEQ 0 goto :errorend
cpcxfsw da3a.dsk -f -p test.bin test.bin
if %ERRORLEVEL% NEQ 0 goto :errorend
..\nocart -f "DA3A" da3a.dsk test.cpr -c RUN"""test
if %ERRORLEVEL% NEQ 0 goto :errorend
..\nocart -f "UNKN" da3a.dsk test.cpr -c RUN"""test
if %ERRORLEVEL% EQU 0 goto :errorend
..\nocart test2.dsk test.cpr -c RUN"""test
if %ERRORLEVEL% EQU 0 goto :errorend
..\nocart test2.dsk -c RUN"""test
if %ERRORLEVEL% EQU 0 goto :errorend
..\nocart test.dsk test.cpr -b test.bin
if %ERRORLEVEL% NEQ 0 goto :errorend
..\nocart test.dsk test.cpr -b da3a.dsk 
if %ERRORLEVEL% EQU 0 goto :errorend

goto :ok
:errorend
color c7
%COMSPEC% /C exit 1 >nul
goto :end
:ok
color a0

%COMSPEC% /C exit 0 >nul
:end



