@echo off
setlocal EnableDelayedExpansion

set "SUMMARY_FILE=summary_execution_times_test2.txt"

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

        set "test2_DIR=%~dp0..\experiments\!CUR_KST!_test2output\!CUR_KST!"

        if exist "!test2_DIR!" rmdir /s /q "!test2_DIR!"
        mkdir "!test2_DIR!\graphs"
        mkdir "!test2_DIR!\logs"
        mkdir "!test2_DIR!\results"
        
        if not exist "!test2_DIR!\..\runstats.txt" (
            echo id = 0 > "!test2_DIR!\..\runstats.txt"
            echo secondsRun = 0 >> "!test2_DIR!\..\runstats.txt"
        )
        echo id = 0 > "!test2_DIR!\runstats.txt"
        echo secondsRun = 0 >> "!test2_DIR!\runstats.txt"
        echo. > "!test2_DIR!\execution_time.txt"

        if exist "..\output\!CUR_KST!\graphs" (
            copy "..\output\!CUR_KST!\graphs\*" "..\experiments\!CUR_KST!_test2output\!CUR_KST!\graphs\" >nul
        )

        echo Running Program...
        start /wait "test2" cmd /c "cd .. && .\bin\program.exe !CUR_KST!_test2 && .\scripts\util\compare experiments\!CUR_KST!_test2output\!CUR_KST!\results\finalresults0.txt output\!CUR_KST!\current_results.txt"
        
        set "t1_val=Error"
        for /f "tokens=3" %%v in ('find "secondsRun" "!test2_DIR!\..\runstats.txt"') do set "t1_val=%%v"
        
        echo !CUR_KST!_test2 = !t1_val!
        echo !CUR_KST!_test2 = !t1_val! >> "%SUMMARY_FILE%"
    )
)

echo.
echo ==================================================
echo Test 1 experiments completed.
echo Results saved in: %SUMMARY_FILE%
echo ==================================================
pause