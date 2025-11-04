set "test1_DIR=%~dp0..\experiments\test1output\K33"
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

set "test2_DIR=%~dp0..\experiments\test2output\K22"
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

set "test3_DIR=%~dp0..\experiments\test3output\K22"
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

set "test4_DIR=%~dp0..\experiments\test4output\K23"
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