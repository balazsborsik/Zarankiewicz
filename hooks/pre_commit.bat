@echo off
REM Call your custom batch
call "%~dp0..\..\scripts\update_current_results.bat"

SETLOCAL ENABLEDELAYEDEXPANSION

for /L %%a in (2,1,6) do (
    for /L %%b in (%%a,1,6) do (
        set "folder=K%%a%%b"
        set "GRAPH_DIR=!PROJECT_ROOT!\output\!folder!\graphs"
        set "RESULT_FILE=!GRAPH_DIR!\..\current_results.txt"

        REM Stage the generated result file in git
        if exist "!RESULT_FILE!" (
            git add "!RESULT_FILE!"
            echo Staged !RESULT_FILE!
        ) else (
            echo WARNING: !RESULT_FILE! was not created.
	    exit /b 1
        )
    )
)

ENDLOCAL

REM Exit 0 to allow the commit to proceed
exit /b 0