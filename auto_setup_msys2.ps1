# Automated MSYS2 setup script
# This script helps automate the post-installation setup

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MSYS2 Post-Installation Setup" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if MSYS2 is installed
$msys2Path = "C:\msys2\usr\bin\bash.exe"
if (-not (Test-Path $msys2Path)) {
    $msys2Path = "C:\msys64\usr\bin\bash.exe"
}

if (-not (Test-Path $msys2Path)) {
    Write-Host "[ERROR] MSYS2 not found!" -ForegroundColor Red
    Write-Host "Please install MSYS2 first:" -ForegroundColor Yellow
    Write-Host "  1. Run: .\install_msys2.ps1" -ForegroundColor White
    Write-Host "  2. Or download from: https://www.msys2.org/" -ForegroundColor White
    exit 1
}

Write-Host "[OK] MSYS2 found!" -ForegroundColor Green
Write-Host ""

# Create setup script for MSYS2
$setupScript = @"
#!/bin/bash
echo "========================================"
echo "Installing development tools..."
echo "========================================"
echo ""

# Update system
echo "Updating MSYS2 (this may take a few minutes)..."
pacman -Syu --noconfirm

echo ""
echo "Installing GCC, GTK+3, pkg-config, and make..."
pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make

echo ""
echo "========================================"
echo "Verifying installation..."
echo "========================================"
gcc --version
pkg-config --version
pkg-config --modversion gtk+-3.0

echo ""
echo "========================================"
echo "Setup complete!"
echo "========================================"
echo ""
echo "Next steps:"
echo "1. Navigate to project:"
echo "   cd '/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System'"
echo ""
echo "2. Build:"
echo "   make"
echo ""
echo "3. Run:"
echo "   ./smart_parking.exe"
"@

$scriptPath = "$env:TEMP\msys2_setup.sh"
$setupScript | Out-File -FilePath $scriptPath -Encoding UTF8

Write-Host "Created setup script: $scriptPath" -ForegroundColor Green
Write-Host ""
Write-Host "To complete setup:" -ForegroundColor Yellow
Write-Host "1. Open 'MSYS2 MinGW 64-bit' from Start Menu" -ForegroundColor White
Write-Host "2. Run this command:" -ForegroundColor White
Write-Host "   bash $scriptPath" -ForegroundColor Cyan
Write-Host ""
Write-Host "OR manually run these commands in MSYS2 MinGW 64-bit:" -ForegroundColor Yellow
Write-Host "   pacman -Syu" -ForegroundColor Cyan
Write-Host "   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make" -ForegroundColor Cyan
Write-Host ""

$open = Read-Host "Open MSYS2 MinGW 64-bit terminal now? (y/n)"
if ($open -eq "y") {
    $mingwPath = "C:\msys64\mingw64.exe"
    if (Test-Path $mingwPath) {
        Start-Process $mingwPath
    } else {
        Write-Host "Please open 'MSYS2 MinGW 64-bit' manually from Start Menu" -ForegroundColor Yellow
    }
}

