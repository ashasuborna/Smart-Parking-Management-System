# Building on Windows with MSYS2

## Prerequisites

1. **Install MSYS2**
   - Download from: https://www.msys2.org/
   - Install to default location (C:\msys64)
   - Run the installer and follow instructions

2. **Open MSYS2 MinGW 64-bit Terminal**
   - **IMPORTANT**: Use "MSYS2 MinGW 64-bit" terminal (NOT regular MSYS2)
   - Start Menu → MSYS2 → MSYS2 MinGW 64-bit

## Installation Steps

### Step 1: Update MSYS2
```bash
pacman -Syu
```
(Press Enter when prompted, wait for completion. If it asks to close terminal, close and reopen)

### Step 2: Install Required Packages
```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-pkg-config
pacman -S make
```

For each package, type `Y` and press Enter when prompted.

### Step 3: (Optional) Install MySQL Support
If you want MySQL support:
```bash
pacman -S mingw-w64-x86_64-mysql
```

## Building the Project

### Step 1: Navigate to Project Directory
```bash
cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
```

### Step 2: Build Without MySQL
```bash
make
```

### Step 3: Build With MySQL (Optional)
```bash
make ENABLE_MYSQL=1
```

### Step 4: Run the Application
```bash
./smart_parking.exe
```

## Troubleshooting

### "pkg-config: command not found"
- Make sure you're using MSYS2 MinGW 64-bit terminal
- Verify installation: `pacman -S mingw-w64-x86_64-pkg-config`

### "GTK+ not found"
- Install GTK3: `pacman -S mingw-w64-x86_64-gtk3`
- Verify: `pkg-config --modversion gtk+-3.0`

### DLL errors when running
- Make sure you're running from MSYS2 MinGW 64-bit terminal
- Or add MSYS2 bin to PATH: `C:\msys64\mingw64\bin`

### Build errors
- Ensure all dependencies are installed
- Try: `make clean` then `make` again

## Alternative: Using Code::Blocks

1. Open Code::Blocks
2. File → New → Project → Empty Project
3. Add all files from `src/` directory
4. Project → Build options → Compiler settings:
   - Other compiler options: `$(shell pkg-config --cflags gtk+-3.0)`
5. Project → Build options → Linker settings:
   - Other linker options: `$(shell pkg-config --libs gtk+-3.0)`
6. Build and Run (F9)

