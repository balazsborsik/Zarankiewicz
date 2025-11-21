@echo off
set "test1_KST=K46"
set "test2_KST=%test1_KST%"

set "test1_DIR=%~dp0..\experiments\nodetest1output\%test1_KST%"
set "test2_DIR=%~dp0..\experiments\nodetest2output\%test2_KST%"

set "destination_DIR=%~dp0..\output"

call copy_no_replace.bat "%test1_DIR%\graphs" "%destination_DIR%\%test1_KST%\graphs"
call copy_no_replace.bat "%test2_DIR%\graphs" "%destination_DIR%\%test2_KST%\graphs"

rmdir /s /q "%test1_DIR%"
mkdir "%test1_DIR%\graphs"

rmdir /s /q "%test2_DIR%"
mkdir "%test2_DIR%\graphs"

call cleanup_output.bat
call .\util\get_results.exe "%destination_DIR%\%test1_KST%\graphs" "%~dp0..\experiments\current_results.txt"