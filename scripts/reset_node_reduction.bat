@echo off
set "test1_KST=K22"
set "test2_KST=K22"

set "test1_DIR=%~dp0..\experiments\nodetest1output\%test1_KST%"
set "test2_DIR=%~dp0..\experiments\nodetest2output\%test2_KST%"

set "destination_DIR=%~dp0..\output"

rmdir /s /q "%test1_DIR%"
mkdir "%test1_DIR%\graphs"

rmdir /s /q "%test2_DIR%"
mkdir "%test2_DIR%\graphs"

call copy_no_replace.bat "%destination_DIR%\%test1_KST%\graphs" "%test1_DIR%\graphs"
call copy_no_replace.bat "%destination_DIR%\%test2_KST%\graphs" "%test2_DIR%\graphs"

call .\util\shuffle_rows.exe "%destination_DIR%\%test2_KST%\graphs"

call cleanup_output.bat
call .\util\get_results.exe output\K22\graphs experiments\current_results.txt