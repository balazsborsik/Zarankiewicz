@echo off
setlocal EnableDelayedExpansion

REM ------------------------------------------------------------
REM Get the directory of this BAT file
REM ------------------------------------------------------------
FOR %%A IN ("%~dp0.") DO SET PROJECT_ROOT=%%~dpA

set FLAG=-l

REM ------------------------------------------------------------
REM Change to util folder (where check_output.cpp is)
REM ------------------------------------------------------------
set UTIL_DIR=%PROJECT_ROOT%scripts\util
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
REM Loop through all K{a}{b} directories where 2 <= a <= b <= 6
REM ------------------------------------------------------------
for /L %%a in (2,1,6) do (
    for /L %%b in (%%a,1,6) do (
        set folder=K%%a%%b
        set GRAPH_DIR=%PROJECT_ROOT%output\!folder!\graphs

        echo.
        echo.
        echo Running check_output.exe with:
        echo   !GRAPH_DIR! %FLAG%
        echo.

        check_output "!GRAPH_DIR!" %FLAG%
    )
)

echo.
echo Done.
pause
endlocal
