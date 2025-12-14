@echo off
setlocal EnableDelayedExpansion

set "SUMMARY_FILE=summary_execution_times_test1.txt"

echo Execution Times (Test 1 Only) > "%SUMMARY_FILE%"
echo ----------------------------- >> "%SUMMARY_FILE%"

for /L %%a in (2,1,6) do (
    for /L %%b in (%%a,1,6) do (
        
        set "CUR_KST=K%%a%%b"
        
        echo.
        echo ==================================================
        :: FIX: Added ^ before ) to prevent breaking the loop
        echo Processing Configuration: !CUR_KST! (Test 1^)
        echo ==================================================

        set "test1_DIR=%~dp0..\experiments\!CUR_KST!_test1output\!CUR_KST!"

        if exist "!test1_DIR!" rmdir /s /q "!test1_DIR!"
        mkdir "!test1_DIR!\graphs"
        mkdir "!test1_DIR!\logs"
        mkdir "!test1_DIR!\results"
        
        if not exist "!test1_DIR!\..\runstats.txt" (
            echo id = 0 > "!test1_DIR!\..\runstats.txt"
            echo secondsRun = 0 >> "!test1_DIR!\..\runstats.txt"
        )
        echo id = 0 > "!test1_DIR!\runstats.txt"
        echo secondsRun = 0 >> "!test1_DIR!\runstats.txt"
        echo. > "!test1_DIR!\execution_time.txt"

        if exist "..\output\!CUR_KST!\graphs" (
            copy "..\output\!CUR_KST!\graphs\*" "..\experiments\!CUR_KST!_test1output\!CUR_KST!\graphs\" >nul
        )

        echo Running Program...
        start /wait "test1" cmd /c "cd .. && .\bin\program.exe !CUR_KST!_test1 && .\scripts\util\compare experiments\!CUR_KST!_test1output\!CUR_KST!\results\finalresults0.txt output\!CUR_KST!\current_results.txt"
        
        set "t1_val=Error"
        for /f "tokens=3" %%v in ('find "secondsRun" "!test1_DIR!\..\runstats.txt"') do set "t1_val=%%v"
        
        echo !CUR_KST!_test1 = !t1_val!
        echo !CUR_KST!_test1 = !t1_val! >> "%SUMMARY_FILE%"
    )
)

echo.
echo ==================================================
echo Test 1 experiments completed.
echo Results saved in: %SUMMARY_FILE%
echo ==================================================
pause