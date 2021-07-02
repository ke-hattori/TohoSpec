@echo off
time /t
echo "build all clean"

rem msdev .\nanospec.dsw /make "all - release" /clean
msdev .\nanospec.dsw /make "all" /clean

echo 
echo 
echo 
echo 
echo 
echo "build all release" | .\tee .\.Build-release.log

time /t | .\tee -a .\.Build-release.log

msdev .\nanospec.dsw /make "all - release" | .\tee -a .\.Build-release.log

echo: | .\tee -a .\.Build-release.log
echo: | .\tee -a .\.Build-release.log
echo: | .\tee -a .\.Build-release.log
echo "Error & Warning report **********************************************" | .\tee -a .\.Build-release.log
findstr "error warning ¥◊∞ åxçê" .\.Build-release.log | .\tee -a .\.Build-release.log
echo: | .\tee -a .\.Build-release.log

echo 
echo 
echo 
echo 
echo 

time /t | .\tee -a .\.Build-release.log

echo finished
pause
