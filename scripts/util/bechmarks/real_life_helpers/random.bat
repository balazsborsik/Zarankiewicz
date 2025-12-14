@echo off
setlocal EnableDelayedExpansion

for /L %%a in (2,1,6) do (
    for /L %%b in (%%a,1,6) do (
        
        set "CUR_KST=K%%a%%b"
        
        echo.
        echo ==================================================
        :: FIX: Added ^ before ) to prevent breaking the loop
        echo Processing Configuration: !CUR_KST! (Test 1^)
        echo ==================================================

        set "test1_DIR=%~dp0..\experiments\!CUR_KST!_test1output

	set "destination_DIR=%~dp0..\output"

	call copy_no_replace.bat "%test1_DIR%\graphs" "%destination_DIR%\!CUR_KST!\graphs"

        mkdir "!test1_DIR!"
        if not exist "!test1_DIR!\runstats.txt" (
            echo id = 0 > "!test1_DIR!\runstats.txt"
            echo secondsRun = 0 >> "!test1_DIR!\runstats.txt"
        )

        echo !CUR_KST!_test1 = !t1_val! >> "%SUMMARY_FILE%"
    )
)