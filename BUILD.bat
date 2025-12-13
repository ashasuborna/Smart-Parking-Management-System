@echo off
echo ========================================
echo Building Smart Parking Management System
echo ========================================
echo.
echo This will open MSYS2 MinGW 64-bit terminal
echo and run the build commands automatically.
echo.
pause

REM Create a temporary script for MSYS2 to run
echo cd '/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System' > %TEMP%\build_script.sh
echo echo "Building project..." >> %TEMP%\build_script.sh
echo make clean >> %TEMP%\build_script.sh
echo make >> %TEMP%\build_script.sh
echo echo. >> %TEMP%\build_script.sh
echo echo "Build complete! If successful, run: ./smart_parking.exe" >> %TEMP%\build_script.sh
echo read -p "Press Enter to exit..." >> %TEMP%\build_script.sh

REM Launch MSYS2 MinGW64 and run the script
start "" "C:\msys64\mingw64.exe" -c "bash %TEMP%\build_script.sh"

echo.
echo MSYS2 terminal opened. The build will run there.
echo Check the terminal window for build output.
echo.

