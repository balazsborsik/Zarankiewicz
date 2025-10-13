@echo off
setlocal

REM ============================================================
REM Build and run compare.cpp with two finalresults files by ID
REM Usage: run_compare.bat <s> <t> <id1> <id2>
REM Example: run_compare.bat 2 2 0 1
REM ============================================================

REM Get the directory of this BAT file
FOR %%A IN ("%~dp0.") DO SET PROJECT_ROOT=%%~dpA

REM ------------------------------------------------------------
REM Validate parameters
REM ------------------------------------------------------------
if "%~1"=="" (
    echo Error: Missing s parameter.
    echo Usage: run_compare.bat ^<s^> ^<t^> ^<id1^> ^<id2^>
    pause
    exit /b 1
)
if "%~2"=="" (
    echo Error: Missing t parameter.
    echo Usage: run_compare.bat ^<s^> ^<t^> ^<id1^> ^<id2^>
    pause
    exit /b 1
)
if "%~3"=="" (
    echo Error: Missing first file ID.
    echo Usage: run_compare.bat ^<s^> ^<t^> ^<id1^> ^<id2^>
    pause
    exit /b 1
)
if "%~4"=="" (
    echo Error: Missing second file ID.
    echo Usage: run_compare.bat ^<s^> ^<t^> ^<id1^> ^<id2^>
    pause
    exit /b 1
)

REM ------------------------------------------------------------
REM Define paths (K{s}{t}\results) and file names
REM ------------------------------------------------------------
set DATA_DIR=%PROJECT_ROOT%output\K%~1%~2\results
set FILE1=%DATA_DIR%\finalresults%~3.txt
set FILE2=%DATA_DIR%\finalresults%~4.txt
set UTIL_DIR=%PROJECT_ROOT%\scripts\util

REM ------------------------------------------------------------
REM Check if the data directory exists
REM ------------------------------------------------------------
if not exist "%DATA_DIR%" (
    echo Error: Data directory "%DATA_DIR%" does not exist.
    pause
    exit /b 1
)

REM ------------------------------------------------------------
REM Change to util folder (where compare.cpp is)
REM ------------------------------------------------------------
cd /d "%UTIL_DIR%"

REM ------------------------------------------------------------
REM Build compare.cpp using g++
REM ------------------------------------------------------------
echo Building compare.cpp...
g++ -o compare compare.cpp

if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

REM ------------------------------------------------------------
REM Run the program with full paths
REM ------------------------------------------------------------
echo Running compare.exe with:
echo   %FILE1%
echo   %FILE2%
echo.

compare "%FILE1%" "%FILE2%"

echo.
echo Done.
pause
endlocal
