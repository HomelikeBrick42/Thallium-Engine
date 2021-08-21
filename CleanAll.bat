@echo off

echo "Cleaning everything..."

make -f "Makefile.Engine.Windows.mak" clean
if %ERRORLEVEL% neq 0 (echo Error: %ERRORLEVEL% && exit)

make -f "Makefile.Game.Windows.mak" clean
if %ERRORLEVEL% neq 0 (echo Error: %ERRORLEVEL% && exit)

echo "All assemblies cleaned successfuly."
