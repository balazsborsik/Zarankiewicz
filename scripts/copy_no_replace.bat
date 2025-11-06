@echo off
setlocal

if "%~1"=="" (
    echo Usage: %~nx0 source_folder destination_folder
    exit /b
)
if "%~2"=="" (
    echo Usage: %~nx0 source_folder destination_folder
    exit /b
)

set "source=%~1"
set "destination=%~2"

if not exist "%source%" (
    echo Source folder does not exist: %source%
    exit /b
)

if not exist "%destination%" (
    echo Destination folder does not exist: %destination%
    exit /b
)

set /a copied=0

for %%f in ("%source%\*") do (
    if not exist "%destination%\%%~nxf" (
        copy "%%f" "%destination%\" >nul
        set /a copied+=1
    )
)

echo %copied% file(s) copied.
