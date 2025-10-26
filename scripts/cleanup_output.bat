@echo off
setlocal

if "%~1"=="" (
    echo Error: Missing maxGraphsToKeep parameter defaulting with 5.
    echo Usage: cleanup_output.bat ^<maxGraphsToKeep^>
    set "maxGraphsToKeep=5"
) else (
    set "maxGraphsToKeep=%~1"
)

echo Using maxGraphsToKeep=%maxGraphsToKeep%

REM Get the directory of this BAT file
FOR %%A IN ("%~dp0.") DO SET PROJECT_ROOT=%%~dpA

set K22_DIR=%PROJECT_ROOT%output\K22\graphs
set K23_DIR=%PROJECT_ROOT%output\K23\graphs
set K33_DIR=%PROJECT_ROOT%output\K33\graphs
set K44_DIR=%PROJECT_ROOT%output\K44\graphs
set K55_DIR=%PROJECT_ROOT%output\K55\graphs
set K66_DIR=%PROJECT_ROOT%output\K66\graphs

set FLAG=-d

REM ------------------------------------------------------------
REM Change to util folder (where cleanup_output.cpp is)
REM ------------------------------------------------------------
set UTIL_DIR=%PROJECT_ROOT%\scripts\util
cd /d "%UTIL_DIR%"

REM ------------------------------------------------------------
REM Build cleanup_output.cpp using g++
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

REM ------------------------------------------------------------
REM Run the program with full paths
REM ------------------------------------------------------------
echo.
echo.
echo Running cleanup_output.exe with:
echo   %K22_DIR% %FLAG% %maxGraphsToKeep%
echo.

cleanup_output %K22_DIR% %FLAG% %maxGraphsToKeep%
echo.
echo.
echo Running cleanup_output.exe with:
echo   %K23_DIR% %FLAG% %maxGraphsToKeep%
echo.

cleanup_output %K23_DIR% %FLAG% %maxGraphsToKeep%
echo.
echo.
echo Running cleanup_output.exe with:
echo   %K33_DIR% %FLAG% %maxGraphsToKeep%
echo.

cleanup_output %K33_DIR% %FLAG% %maxGraphsToKeep%
echo.
echo.
echo Running cleanup_output.exe with:
echo   %K44_DIR% %FLAG% %maxGraphsToKeep%
echo.

cleanup_output %K44_DIR% %FLAG% %maxGraphsToKeep%
echo.
echo.
echo Running cleanup_output.exe with:
echo   %K55_DIR% %FLAG% %maxGraphsToKeep%
echo.

cleanup_output %K55_DIR% %FLAG% %maxGraphsToKeep%
echo.
echo.
echo Running cleanup_output.exe with:
echo   %K66_DIR% %FLAG% %maxGraphsToKeep%
echo.

cleanup_output %K66_DIR% %FLAG% %maxGraphsToKeep%

echo.
echo Done.
pause
endlocal