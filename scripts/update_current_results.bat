@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

REM ------------------------------------------------------------
REM Set project root (one directory up from this .bat)
REM ------------------------------------------------------------
FOR %%A IN ("%~dp0..") DO SET "PROJECT_ROOT=%%~fA"

REM ------------------------------------------------------------
REM Build get_results.cpp using g++
REM ------------------------------------------------------------
if not exist "!PROJECT_ROOT!\scripts\util\get_results.exe" (
    echo Building get_results.cpp...
    g++ "!PROJECT_ROOT!\scripts\util\get_results.cpp" ^
        "!PROJECT_ROOT!\src\core\Graph.cpp" ^
        "!PROJECT_ROOT!\src\util\Util.cpp" ^
        "!PROJECT_ROOT!\src\structure\*.cpp" ^
        -I"!PROJECT_ROOT!\include" ^
        -o "!PROJECT_ROOT!\scripts\util\get_results.exe"

    if errorlevel 1 (
        echo Build failed.
        pause
        exit /b 1
    )
) else (
    echo get_results.exe already exists, skipping build.
)

REM ------------------------------------------------------------
REM Loop through all K{a}{b} directories where 2 <= a <= b <= 6
REM ------------------------------------------------------------
for /L %%a in (2,1,6) do (
    for /L %%b in (%%a,1,6) do (
        set "folder=K%%a%%b"
        set "GRAPH_DIR=!PROJECT_ROOT!\output\!folder!\graphs"

        echo.
        echo.
        echo Running get_results.exe with:
        echo   !GRAPH_DIR! "!GRAPH_DIR!\..\current_results.txt"
        echo.

        "!PROJECT_ROOT!\scripts\util\get_results.exe" "!GRAPH_DIR!" "!GRAPH_DIR!\..\current_results.txt"
    )
)

ENDLOCAL
