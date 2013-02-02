echo off
echo η¨αβª  src
del /F *.vcproj
del /F *.user
del /F /A:H *.suo
del /F *.suo
del /F *.sln
del /F *.ncb
del /F *.idb
del /F *.pdb
rd /S /Q obj
del /F bin\*.pdb
del /F bin\*.ilk
del /F bin\*.exe