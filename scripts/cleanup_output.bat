@echo off
setlocal EnableDelayedExpansion

REM ------------------------------------------------------------
REM Handle input parameter
REM ------------------------------------------------------------
if "%~1"=="" (
    echo Error: Missing maxGraphsToKeep parameter, defaulting to 2.
    echo Usage: cleanup_output.bat ^<maxGraphsToKeep^>
    set "maxGraphsToKeep=2"
) else (
    set "maxGraphsToKeep=%~1"
)

echo Using maxGraphsToKeep=%maxGraphsToKeep%
echo.

REM ------------------------------------------------------------
REM Determine project and utility directories
REM ------------------------------------------------------------
FOR %%A IN ("%~dp0.") DO SET PROJECT_ROOT=%%~dpA
set UTIL_DIR=%PROJECT_ROOT%scripts\util
set FLAG=-d

cd /d "%UTIL_DIR%"

REM ------------------------------------------------------------
REM Build cleanup_output.cpp if not already built
REM ------------------------------------------------------------
if not exist "cleanup_output.exe" (
    echo Building cleanup_output.cpp...
    g++ cleanup_output.cpp -I../../include -o cleanup_output
    if errorlevel 1 (
        echo Build failed.
        pause
        exit /b 1
    )
) else (
    echo cleanup_output.exe already exists, skipping build.
)

echo.

REM ------------------------------------------------------------
REM Loop over all K{a}{b} directories (2 <= a <= b <= 6)
REM ------------------------------------------------------------
for /L %%a in (2,1,6) do (
    for /L %%b in (%%a,1,6) do (
        set folder=K%%a%%b
        set graphDir=%PROJECT_ROOT%output\!folder!\graphs

        echo.
        echo Running cleanup_output.exe with:
        echo   !graphDir! %FLAG% %maxGraphsToKeep%
        echo.

        cleanup_output "!graphDir!" %FLAG% %maxGraphsToKeep%
    )
)

echo.
echo Done.
pause
endlocal
