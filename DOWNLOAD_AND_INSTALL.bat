@echo off
echo ========================================
echo MSYS2 Installation Helper
echo ========================================
echo.

REM Check if already installed
if exist "C:\msys64\mingw64.exe" (
    echo [INFO] MSYS2 appears to be installed at C:\msys64
    echo.
    echo Opening MSYS2 MinGW 64-bit terminal...
    start "" "C:\msys64\mingw64.exe"
    echo.
    echo If MSYS2 opens, run these commands:
    echo   pacman -Syu
    echo   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
    pause
    exit /b 0
)

echo Downloading MSYS2 installer...
echo This may take a few minutes...
echo.

REM Download MSYS2 installer
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://github.com/msys2/msys2-installer/releases/latest/download/msys2-x86_64-latest.exe' -OutFile '%USERPROFILE%\Downloads\msys2-installer.exe'}"

if exist "%USERPROFILE%\Downloads\msys2-installer.exe" (
    echo.
    echo [SUCCESS] Download complete!
    echo.
    echo ========================================
    echo Installation Instructions:
    echo ========================================
    echo.
    echo 1. The installer is in your Downloads folder
    echo 2. Double-click: msys2-installer.exe
    echo 3. Install to: C:\msys64 (default)
    echo 4. DO NOT check "Run MSYS2 now" at the end
    echo.
    echo After installation:
    echo - Open "MSYS2 MinGW 64-bit" from Start Menu
    echo - Run: pacman -Syu
    echo - Then: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
    echo.
    pause
    
    REM Ask to open installer
    set /p open="Open Downloads folder? (y/n): "
    if /i "%open%"=="y" (
        start "" "%USERPROFILE%\Downloads"
    )
) else (
    echo.
    echo [ERROR] Download failed!
    echo.
    echo Please download manually:
    echo 1. Go to: https://www.msys2.org/
    echo 2. Download the installer
    echo 3. Run it and install to C:\msys64
    echo.
    pause
)

