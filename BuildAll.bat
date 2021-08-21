@echo off

echo "Building everything..."

make -f "Makefile.Engine.Windows.mak" all
if %ERRORLEVEL% neq 0 (echo Error: %ERRORLEVEL% && exit)

make -f "Makefile.Game.Windows.mak" all
if %ERRORLEVEL% neq 0 (echo Error: %ERRORLEVEL% && exit)

echo "All assemblies built successfuly."
