copy ..\output\K22\graphs\* ..\experiments\test1output\K22\graphs\
copy ..\output\K22\graphs\* ..\experiments\test2output\K22\graphs\
copy ..\output\K22\graphs\* ..\experiments\test3output\K22\graphs\
copy ..\output\K22\graphs\* ..\experiments\test4output\K22\graphs\



start "test1" cmd /k "cd .. && .\bin\program.exe test1 && .\scripts\util\compare experiments\test1output\K22\results\finalresults0.txt experiments\current_results.txt"
start "test2" cmd /k "cd .. && .\bin\program.exe test2 && .\scripts\util\compare experiments\test2output\K22\results\finalresults0.txt experiments\current_results.txt
start "test3" cmd /k "cd .. && .\bin\program.exe test3 && .\scripts\util\compare experiments\test3output\K22\results\finalresults0.txt experiments\current_results.txt"
start "test4" cmd /k "cd .. && .\bin\program.exe test4 && .\scripts\util\compare experiments\test4output\K22\results\finalresults0.txt experiments\current_results.txt"