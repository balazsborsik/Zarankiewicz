@echo off
setlocal enabledelayedexpansion

set "S=2"
set "T=3"

set "test1_KST=K%S%%T%"
set "test2_KST=K%S%%T%"

set "test1_DIR=..\experiments\nodetest1output\%test1_KST%\graphs"
set "test2_DIR=..\experiments\nodetest2output\%test2_KST%\graphs"

set "destination_DIR=%~dp0..\output"

call copy_no_replace.bat "%destination_DIR%\%test1_KST%\graphs" "%test1_DIR%"
call copy_no_replace.bat "%destination_DIR%\%test2_KST%\graphs" "%test2_DIR%"

set "TEST1_RESULTS=..\experiments\test1_results.txt"
set "TEST2_RESULTS=..\experiments\test2_results.txt"
set "CURRENT_RESULTS=..\experiments\current_results.txt"

for /l %%i in (1,1,10) do (
    set /a mod=%%i %% 3

if !mod! == 0 (
 ..\scripts\util\shuffle_graphs.exe "%test1_DIR%"
 ..\scripts\util\shuffle_graphs.exe "%test2_DIR%"
)
    if "%S%"=="%T%" (
        ..\scripts\util\run_node_reduction.exe %test1_DIR% %S% %T% 
        ..\scripts\util\cleanup_oneside_of_symmetric_output %test1_DIR% -d >nul 2>&1
    ) else (
        ..\scripts\util\run_node_reduction.exe %test1_DIR% %S% %T% 
    )
    ..\scripts\util\get_results %test1_DIR% %TEST1_RESULTS% 

    if "%S%"=="%T%" (
        ..\scripts\util\run_node_reduction.exe %test2_DIR% %S% %T% 
        ..\scripts\util\cleanup_oneside_of_symmetric_output %test2_DIR% -d >nul 2>&1
    ) else (
        ..\scripts\util\run_node_reduction.exe %test2_DIR% %S% %T% 
    )
    ..\scripts\util\get_results %test2_DIR% %TEST2_RESULTS% 
)

REM Run compare only once in two CMD windows
if exist %TEST1_RESULTS% if exist %TEST2_RESULTS% (
    start "Compare Test2" cmd /k "..\scripts\util\compare %TEST2_RESULTS% %CURRENT_RESULTS% "
    start "Compare Test1" cmd /k "..\scripts\util\compare %TEST1_RESULTS% %CURRENT_RESULTS% "
) else (
    echo ERROR: Results files not found after all iterations
)