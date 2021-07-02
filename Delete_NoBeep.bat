@echo off

echo "DELETE TEMP OBJ"

del /s /q .\*.ilk
del /s /q .\*.ncb
rem del /s /q .\*.opt
del /s /q .\lib\*.exp
del /s /q .\src\*.idb
del /s /q .\src\*.obj
del /s /q .\src\*.pch
del /s /q .\src\*.pdb
del /s /q .\src\*.plg
del /s /q .\src\*.res
del /s /q .\src\*.sbr

echo このウィンドウを閉じます．
