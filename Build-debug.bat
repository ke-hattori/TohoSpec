@echo off
time /t
echo "build all clean"

rem msdev .\nanospec.dsw /make "all - debug" /clean
msdev .\nanospec.dsw /make "all" /clean

echo 
echo 
echo 
echo 
echo 
echo "build all debug" | .\tee .\.Build-debug.log

time /t | .\tee -a .\.Build-debug.log

msdev .\nanospec.dsw /make "all - debug" | .\tee -a .\.Build-debug.log

echo: | .\tee -a .\.Build-debug.log
echo: | .\tee -a .\.Build-debug.log
echo: | .\tee -a .\.Build-debug.log
echo "Error & Warning report **********************************************" | .\tee -a .\.Build-debug.log
findstr "error warning ¥◊∞ åxçê" .\.Build-debug.log | .\tee -a .\.Build-debug.log
echo: | .\tee -a .\.Build-debug.log

echo 
echo 
echo 
echo 
echo 

time /t | .\tee -a .\.Build-debug.log

echo finished
pause
