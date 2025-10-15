@echo off
setlocal

REM Get the directory of this BAT file
FOR %%A IN ("%~dp0.") DO SET PROJECT_ROOT=%%~dpA

set K22_DIR=%PROJECT_ROOT%output\K22\graphs
set K33_DIR=%PROJECT_ROOT%output\K33\graphs
set K44_DIR=%PROJECT_ROOT%output\K44\graphs
set K55_DIR=%PROJECT_ROOT%output\K55\graphs
set K66_DIR=%PROJECT_ROOT%output\K66\graphs

set FLAG=-l

REM ------------------------------------------------------------
REM Change to util folder (where check_output.cpp is)
REM ------------------------------------------------------------
set UTIL_DIR=%PROJECT_ROOT%\scripts\util
cd /d "%UTIL_DIR%"

REM ------------------------------------------------------------
REM Build check_output.cpp using g++
REM ------------------------------------------------------------
if not exist "check_output.exe" (
    echo Building check_output.cpp...
    g++ check_output.cpp ../../src/core/Graph.cpp ../../src/util/Util.cpp ../../src/structure/*.cpp -I../../include -o check_output
    if errorlevel 1 (
        echo Build failed.
        pause
        exit /b 1
    )
) else (
    echo check_output.exe already exists, skipping build.
)

REM ------------------------------------------------------------
REM Run the program with full paths
REM ------------------------------------------------------------
echo.
echo.
echo Running check_output.exe with:
echo   %K22_DIR% %FLAG%
echo.

check_output %K22_DIR% %FLAG%
echo.
echo.
echo Running check_output.exe with:
echo   %K33_DIR% %FLAG%
echo.

check_output %K33_DIR% %FLAG%
echo.
echo.
echo Running check_output.exe with:
echo   %K44_DIR% %FLAG%
echo.

check_output %K44_DIR% %FLAG%
echo.
echo.
echo Running check_output.exe with:
echo   %K55_DIR% %FLAG%
echo.

check_output %K55_DIR% %FLAG%
echo.
echo.
echo Running check_output.exe with:
echo   %K66_DIR% %FLAG%
echo.

check_output %K66_DIR% %FLAG%

echo.
echo Done.
pause
endlocal