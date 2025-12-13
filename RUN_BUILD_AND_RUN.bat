@echo off
echo ========================================
echo Smart Parking - Build and Run
echo ========================================
echo.

cd /d "%~dp0"

REM Check if already built
if exist "smart_parking.exe" (
    echo [INFO] Executable found!
    echo.
    choice /C YN /M "Run the application now"
    if errorlevel 2 goto :end
    if errorlevel 1 goto :run
)

:build
echo Building project...
echo Opening MSYS2 MinGW 64-bit terminal...
echo.
echo In the terminal that opens, the build will run automatically.
echo.

REM Create build script
(
echo cd '/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System'
echo echo "========================================"
echo echo "Building Smart Parking Management System"
echo echo "========================================"
echo echo.
echo make clean
echo make
echo echo.
echo echo "========================================"
if exist smart_parking.exe (
    echo echo "Build SUCCESSFUL!"
    echo echo "========================================"
    echo echo.
    echo echo "To run: ./smart_parking.exe"
) else (
    echo echo "Build FAILED - Check errors above"
    echo echo "========================================"
)
echo echo.
echo read -p "Press Enter to close..."
) > %TEMP%\auto_build.sh

start "" "C:\msys64\mingw64.exe" -c "bash %TEMP%\auto_build.sh"

timeout /t 3 >nul

:wait
if not exist "smart_parking.exe" (
    echo Waiting for build to complete...
    timeout /t 2 >nul
    goto :wait
)

echo.
echo [SUCCESS] Build complete! smart_parking.exe created.
echo.

:run
if exist "smart_parking.exe" (
    echo Starting application...
    echo.
    echo NOTE: The application must run from MSYS2 MinGW 64-bit terminal
    echo to have access to GTK+ libraries.
    echo.
    echo Opening MSYS2 terminal to run the app...
    
    (
    echo cd '/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System'
    echo ./smart_parking.exe
    echo echo.
    echo echo "Application closed."
    echo read -p "Press Enter to exit..."
    ) > %TEMP%\auto_run.sh
    
    start "" "C:\msys64\mingw64.exe" -c "bash %TEMP%\auto_run.sh"
) else (
    echo [ERROR] smart_parking.exe not found!
    echo Please build manually in MSYS2 MinGW 64-bit terminal.
)

:end
echo.
pause

