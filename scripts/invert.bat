@echo off
setlocal enabledelayedexpansion

REM ============================================================
REM Build and run invert.cpp with two files from /toinvert
REM No arguments needed — automatically uses the first file found
REM ============================================================

REM Get directory of this BAT file
set BAT_DIR=%~dp0

REM Normalize project root
pushd "%BAT_DIR%"
set PROJECT_ROOT=%CD%
popd

REM ------------------------------------------------------------
REM Define paths
REM ------------------------------------------------------------
set UTIL_DIR=%PROJECT_ROOT%\util
set TOINVERT_DIR=%PROJECT_ROOT%\toinvert

REM ------------------------------------------------------------
REM Check if the directory exists
REM ------------------------------------------------------------
if not exist "%TOINVERT_DIR%" (
    echo Error: Directory "%TOINVERT_DIR%" does not exist.
    pause
    exit /b 1
)

REM ------------------------------------------------------------
REM Get the first two files in the toinvert folder
REM ------------------------------------------------------------
set FILECOUNT=0
for %%F in ("%TOINVERT_DIR%\*") do (
    if exist "%%F" (
        set /a FILECOUNT+=1
        if !FILECOUNT! equ 1 set FILE1=%%F
    )
)

REM Check that two files were found
if !FILECOUNT! lss 1 (
    echo Error: No files found in "%TOINVERT_DIR%".
    pause
    exit /b 1
)

echo Found files:
echo   !FILE1!
echo.

REM ------------------------------------------------------------
REM Build only if invert.exe does not exist
REM ------------------------------------------------------------
cd /d "%UTIL_DIR%"

if not exist "invert.exe" (
    echo Building invert.cpp...
    g++ -o invert invert.cpp
    if errorlevel 1 (
        echo Build failed.
        pause
        exit /b 1
    )
) else (
    echo invert.exe already exists, skipping build.
)

REM ------------------------------------------------------------
REM Run the program
REM ------------------------------------------------------------
echo Running invert.exe...
invert "!FILE1!"

echo.
echo Done.
pause
endlocal
