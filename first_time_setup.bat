@echo off
echo Installing Git hooks...
copy "%~dp0hooks\pre-commit" "%~dp0.git\hooks\pre-commit"
echo Done.
echo Creating folders...
call %~dp0scripts\generate_folders.bat
call %~dp0scripts\init.bat
echo Done.
echo Note you may have to compile the cpp files inside .\scripts\util first (the compile commands are in the source codes)