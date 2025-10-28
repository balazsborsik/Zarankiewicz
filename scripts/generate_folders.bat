@echo off
setlocal EnableDelayedExpansion
set OUTPUT_DIR=..\output

REM Create output base directory
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM Loop through a = 2 to 6
for /L %%a in (2,1,6) do (
    REM Loop through b = a to 6 (ensures a <= b and b <= 6)
    for /L %%b in (%%a,1,6) do (
        set folder=K%%a%%b
        call :createFolder "!folder!"
    )
)

echo All folders created successfully!
exit /b

:createFolder
set folderName=%~1
set fullPath=%OUTPUT_DIR%\%folderName%

if not exist "%fullPath%" (
    echo Creating %folderName%...
    mkdir "%fullPath%\graphs"
    mkdir "%fullPath%\logs"
    mkdir "%fullPath%\results"

    REM Create empty execution_time.txt
    type nul > "%fullPath%\execution_time.txt"

    REM Correctly write to runstats.txt
    (
        echo id = 0
        echo secondsRun = 0
    ) > "%fullPath%\runstats.txt"

) else (
    echo Skipping %folderName% (already exists)
)
exit /b
