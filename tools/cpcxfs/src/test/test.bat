SETLOCAL EnableDelayedExpansion
del test.dsk
del test2.dsk
del test3.dsk
del test4.dsk
..\..\cpcxfsw
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -?
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -h
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -nd test.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -ns test.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -nv test.dsk
if %ERRORLEVEL% neq 0 goto :ok5
goto :error
:ok5
..\..\cpcxfsw -f -e format -f DATA test.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -f -e format -d test2.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -f -e format -s test3.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -f -nd "test2.dsk"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -f -ns "test3.dsk"
if %ERRORLEVEL% neq 0 goto :error
rem ..\..\cpcxfsw -nv "test4.dsk"
rem if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw test.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o 
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA test.dsk
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA "test2.dsk"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test.dsk" -p smalldoc.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test.dsk" -b -f -p smalldoc.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test.dsk" -b -f -p "smalldoc.txt" "small1"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test.dsk" -t -f -p "smalldoc.txt" "small2"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test.dsk" -t -f -p "smalldoc.txt" "small.txt"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA "test2.dsk" -p "smalldoc.txt"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA "test2.dsk" -d
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA "test2.dsk" -s
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA "test2.dsk" -x "instruct.txt"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -o DATA "test2.dsk" -o
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -g "UNKN"
if %ERRORLEVEL% neq 0 goto :ok3
goto :error
:ok3
..\..\cpcxfsw "test2.dsk" -f -p "UNKN"
if %ERRORLEVEL% neq 0 goto :ok4
goto :error
:ok4
..\..\cpcxfsw "test2.dsk" -f -mp "smalldo*"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -mp smalldo*
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -mg "smalldo*"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -p "smalldoc.txt" "small.txt"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -p "smalldoc.txt" "small"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -p "smalldoc.txt" "small."
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -d
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -s
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -t -p "small.txt" smallfile.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -b -p "small.txt" smallfile.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -g "small.txt" smallfile.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -g "small" smallfile.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -g "small." smallfile.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -f -mg "small*"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test2.dsk" -e ""dir;put "smalldoc.txt" "small.txt";get "small.txt";dir;close;quit""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -f -nd "test5.dsk"
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""bye""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""exit""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""quit""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""force""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""formats""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""help""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""?""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""user 0""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""cls""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""mode bin""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""mode text""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""mode auto""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""del UNKN""
if %ERRORLEVEL% neq 0 goto :ok6
goto :error
:ok6
..\..\cpcxfsw "test5.dsk" -e ""era UNKN""
if %ERRORLEVEL% neq 0 goto :ok7
goto :error
:ok7
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -e ""del smalldoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -e ""era smalldoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""echo test""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""stat""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""dpb""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""page""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""page 50""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""prompt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""prompt aaaa""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""comment test""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -e ""comment""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -f -e ""nuke smalldoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -f -e ""nuke smalldoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -f -e ""nuke UNKN.txt""
if %ERRORLEVEL% neq 0 goto :ok10
goto :error
:ok10
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -f -e ""copy smalldoc.txt sdoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -f -e ""copy smalldoc.txt sdoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -f -e ""copy UNKN.txt sdoc.txt""
if %ERRORLEVEL% neq 0 goto :ok9
goto :error
:ok9
..\..\cpcxfsw "test5.dsk" -f -e ""copy smalldoc.txt 1:""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt
..\..\cpcxfsw "test5.dsk" -f -e ""type smalldoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -f -e ""type -b smalldoc.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -f -e ""clean""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt sdoc2.txt
..\..\cpcxfsw "test5.dsk" -f -e ""ren sdoc2.txt sdoc3.txt""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -p smalldoc.txt sdoc4.txt
..\..\cpcxfsw "test5.dsk" -f -e ""ren sdoc4.txt 2:""
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw "test5.dsk" -f -e ""ren UNKN.txt 3:""
if %ERRORLEVEL% neq 0 goto :ok11
goto :error
:ok11
echo.>commands.txt
echo dir >>commands.txt
echo put "smalldoc.txt" "small.txt" >>commands.txt
echo get "smalldoc.txt" "small.txt" >>commands.txt
echo dir >>commands.txt
echo close >>commands.txt
echo quit >>commands.txt
..\..\cpcxfsw "test2.dsk" -x commands.txt
if %ERRORLEVEL% neq 0 goto :error
..\..\cpcxfsw -f -e format -f UNKN test.dsk
if %ERRORLEVEL% neq 0 goto :ok
goto :error
:ok
..\..\cpcxfsw -f -nd test.dsk
if %ERRORLEVEL% neq 0 goto :error

FOR /L %%G IN (1,1,64) DO (
del file%%G
echo.>file%%G
..\..\cpcxfsw test.dsk -f -p file%%G
if %ERRORLEVEL% neq 0 goto :error
 )
del file65
echo.>file65
..\..\cpcxfsw test.dsk -f -p file65
if %ERRORLEVEL% neq 0 goto :ok2b
goto :error
:ok2b
del file66
echo.>file66
..\..\cpcxfsw test.dsk -f -p file66
if %ERRORLEVEL% neq 0 goto :ok2c
goto :error
:ok2c
..\..\cpcxfsw -f -nd test2.dsk
if %ERRORLEVEL% neq 0 goto :error

FOR /L %%G IN (1,1,11) DO (
del file%%G
fsutil file createnew file%%G 16384
..\..\cpcxfsw test2.dsk -f -p file%%G
if %ERRORLEVEL% neq 0 goto :error
 )
del file12
fsutil file createnew file12 16384
..\..\cpcxfsw test2.dsk -f -p file12
if %ERRORLEVEL% neq 0 goto :ok4
goto :error
:ok4
 
goto :end

:error
echo "Error"
:end


