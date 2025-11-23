set "test1_KST=K55"
set "test2_KST=K55"
set "test3_KST=K55"
set "test4_KST=K55"

goto comment
copy ..\output\%test1_KST%\graphs\* ..\experiments\test1output\%test1_KST%\graphs\
copy ..\output\%test2_KST%\graphs\* ..\experiments\test2output\%test2_KST%\graphs\
copy ..\output\%test3_KST%\graphs\* ..\experiments\test3output\%test3_KST%\graphs\
copy ..\output\%test4_KST%\graphs\* ..\experiments\test4output\%test4_KST%\graphs\

 ..\scripts\util\shuffle_graphs.exe ..\experiments\test1output\%test1_KST%\graphs
 ..\scripts\util\shuffle_graphs.exe ..\experiments\test2output\%test2_KST%\graphs
 ..\scripts\util\shuffle_graphs.exe ..\experiments\test3output\%test3_KST%\graphs
 ..\scripts\util\shuffle_graphs.exe ..\experiments\test4output\%test4_KST%\graphs
:comment
start "test1" cmd /k "cd .. && .\bin\program.exe test1 && .\scripts\util\compare experiments\test1output\%test1_KST%\results\finalresults0.txt experiments\current_results.txt"
start "test2" cmd /k "cd .. && .\bin\program.exe test2 && .\scripts\util\compare experiments\test2output\%test2_KST%\results\finalresults0.txt experiments\current_results.txt
start "test3" cmd /k "cd .. && .\bin\program.exe test3 && .\scripts\util\compare experiments\test3output\%test3_KST%\results\finalresults0.txt experiments\current_results.txt"
start "test4" cmd /k "cd .. && .\bin\program.exe test4 && .\scripts\util\compare experiments\test4output\%test4_KST%\results\finalresults0.txt experiments\current_results.txt"