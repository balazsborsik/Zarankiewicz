@echo off
set "test1_KST=K22"
set "test2_KST=K22"
set "test3_KST=K33"
set "test4_KST=K33"

cd ..
REM ==== Loop 40 times ====
for /L %%i in (0,1,39) do (
cd scripts
    ..\scripts\util\shuffle_graphs.exe ..\experiments\test1output\%test1_KST%\graphs
    ..\scripts\util\shuffle_graphs.exe ..\experiments\test2output\%test2_KST%\graphs
    ..\scripts\util\shuffle_graphs.exe ..\experiments\test3output\%test3_KST%\graphs
    ..\scripts\util\shuffle_graphs.exe ..\experiments\test4output\%test4_KST%\graphs
cd ..

    .\bin\program.exe test1
    .\bin\program.exe test2
    .\bin\program.exe test3
    .\bin\program.exe test4

)
cd scripts
REM ==== Compare final results ====
util\compare ..\experiments\test1output\%test1_KST%\results\finalresults51.txt ..\output\%test1_KST%\current_results.txt
util\compare ..\experiments\test2output\%test2_KST%\results\finalresults51.txt ..\output\%test2_KST%\current_results.txt
util\compare ..\experiments\test3output\%test3_KST%\results\finalresults51.txt ..\output\%test3_KST%\current_results.txt
util\compare ..\experiments\test4output\%test4_KST%\results\finalresults51.txt ..\output\%test4_KST%\current_results.txt
pause
