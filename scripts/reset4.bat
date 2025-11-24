@echo off
set "test1_KST=K56"
set "test2_KST=K56"
set "test3_KST=K56"
set "test4_KST=K56"

set "test1_DIR=%~dp0..\experiments\test1output\%test1_KST%"
set "test2_DIR=%~dp0..\experiments\test2output\%test2_KST%"
set "test3_DIR=%~dp0..\experiments\test3output\%test3_KST%"
set "test4_DIR=%~dp0..\experiments\test4output\%test4_KST%"

set "destination_DIR=%~dp0..\output"

call copy_no_replace.bat "%test1_DIR%\graphs" "%destination_DIR%\%test1_KST%\graphs"
call copy_no_replace.bat "%test2_DIR%\graphs" "%destination_DIR%\%test2_KST%\graphs"
call copy_no_replace.bat "%test3_DIR%\graphs" "%destination_DIR%\%test3_KST%\graphs"
call copy_no_replace.bat "%test4_DIR%\graphs" "%destination_DIR%\%test4_KST%\graphs"


rmdir /s /q "%test1_DIR%"
mkdir "%test1_DIR%\graphs"
mkdir "%test1_DIR%\logs"
mkdir "%test1_DIR%\results"
if not exist "%test1_DIR%\..\runstats.txt" (
    echo id = 0 > "%test1_DIR%\..\runstats.txt"
    echo secondsRun = 0 >> "%test1_DIR%\..\runstats.txt"
)
echo id = 0 > "%test1_DIR%\runstats.txt"
echo secondsRun = 0 >> "%test1_DIR%\runstats.txt"
echo. > "%test1_DIR%\execution_time.txt"

rmdir /s /q "%test2_DIR%"
mkdir "%test2_DIR%\graphs"
mkdir "%test2_DIR%\logs"
mkdir "%test2_DIR%\results"
if not exist "%test2_DIR%\..\runstats.txt" (
    echo id = 0 > "%test2_DIR%\..\runstats.txt"
    echo secondsRun = 0 >> "%test2_DIR%\..\runstats.txt"
)
echo id = 0 > "%test2_DIR%\runstats.txt"
echo secondsRun = 0 >> "%test2_DIR%\runstats.txt"
echo. > "%test2_DIR%\execution_time.txt"

rmdir /s /q "%test3_DIR%"
mkdir "%test3_DIR%\graphs"
mkdir "%test3_DIR%\logs"
mkdir "%test3_DIR%\results"
if not exist "%test3_DIR%\..\runstats.txt" (
    echo id = 0 > "%test3_DIR%\..\runstats.txt"
    echo secondsRun = 0 >> "%test3_DIR%\..\runstats.txt"
)
echo id = 0 > "%test3_DIR%\runstats.txt"
echo secondsRun = 0 >> "%test3_DIR%\runstats.txt"
echo. > "%test3_DIR%\execution_time.txt"

rmdir /s /q "%test4_DIR%"
mkdir "%test4_DIR%\graphs"
mkdir "%test4_DIR%\logs"
mkdir "%test4_DIR%\results"
if not exist "%test4_DIR%\..\runstats.txt" (
    echo id = 0 > "%test4_DIR%\..\runstats.txt"
    echo secondsRun = 0 >> "%test4_DIR%\..\runstats.txt"
)
echo id = 0 > "%test4_DIR%\runstats.txt"
echo secondsRun = 0 >> "%test4_DIR%\runstats.txt"
echo. > "%test4_DIR%\execution_time.txt"

call cleanup_output.bat
call update_current_results_inside_experiments.bat