set "S=2"
set "T=2"

set "test1_KST=K%S%%T%"
set "test2_KST=K%S%%T%"

set "test1_DIR=%~dp0..\experiments\nodetest1output\%test1_KST%\graphs"
set "test2_DIR=%~dp0..\experiments\nodetest2output\%test2_KST%\graphs"

start "test1" cmd /k "cd .. && .\scripts\util\run_node_reduction.exe %test1_DIR% %S% %T% && .\scripts\util\cleanup_oneside_of_symmetric_output %test1_DIR% -d && .\scripts\util\get_results %test1_DIR% experiments\test1_results.txt && .\scripts\util\compare experiments\test1_results.txt experiments\current_results.txt"
start "test2" cmd /k "cd .. && .\scripts\util\run_node_reduction.exe %test2_DIR% %S% %T% && .\scripts\util\cleanup_oneside_of_symmetric_output %test2_DIR% -d && .\scripts\util\get_results %test2_DIR% experiments\test2_results.txt && .\scripts\util\compare experiments\test2_results.txt experiments\current_results.txt"
