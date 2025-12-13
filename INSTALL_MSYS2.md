# MSYS2 Installation Guide

## Why MSYS2?

This project requires:
- GCC compiler (to compile C code)
- GTK+3 libraries (for the GUI)
- pkg-config (to find libraries)

MSYS2 provides all these tools for Windows in one package.

## Installation Steps

### Step 1: Download MSYS2

1. Open your web browser
2. Go to: **https://www.msys2.org/**
3. Click "Download the installer"
4. Download `msys2-x86_64-latest.exe` (or latest version)

### Step 2: Install MSYS2

1. Run the downloaded installer
2. **Install to default location**: `C:\msys64` (recommended)
3. Click "Next" through the installation
4. **DO NOT** check "Run MSYS2 now" at the end
5. Click "Finish"

### Step 3: Open MSYS2 MinGW 64-bit Terminal

**CRITICAL**: You must use the correct terminal!

1. Press `Windows Key`
2. Type: `MSYS2 MinGW 64-bit`
3. Click on **"MSYS2 MinGW 64-bit"** (the one with "MinGW" in the name)
   - NOT "MSYS2 MSYS" (wrong one!)
   - NOT "MSYS2 UCRT64" (wrong one!)

You should see a terminal window with a prompt like:
```
user@computer MINGW64 ~
$
```

### Step 4: Update MSYS2

In the MSYS2 MinGW 64-bit terminal, type:

```bash
pacman -Syu
```

- Press Enter
- Wait for it to download and update (may take 2-5 minutes)
- If it says "terminate MSYS2 without closing?", type `Y` and press Enter
- **Close the terminal** and reopen "MSYS2 MinGW 64-bit"

### Step 5: Install Development Tools

Open MSYS2 MinGW 64-bit terminal again, then run:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
```

- When asked "Proceed with installation? [Y/n]", type `Y` and press Enter
- Wait for installation to complete (may take 5-10 minutes)

### Step 6: Verify Installation

Test that everything is installed:

```bash
gcc --version
pkg-config --version
pkg-config --modversion gtk+-3.0
```

You should see version numbers for each command.

### Step 7: Navigate to Your Project

```bash
cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
```

**Note**: In MSYS2, Windows paths use `/c/` instead of `C:\`

### Step 8: Build the Project

```bash
make
```

This will compile all source files. You should see output like:
```
gcc -Wall -Wextra -std=c11 ... -c src/main.c -o src/main.o
gcc -Wall -Wextra -std=c11 ... -c src/models.c -o src/models.o
...
gcc src/main.o src/models.o ... -o smart_parking.exe
```

### Step 9: Run the Application

```bash
./smart_parking.exe
```

The GUI application should open!

## Common Issues

### Issue: "pacman: command not found"
- **Solution**: You're in the wrong terminal. Use "MSYS2 MinGW 64-bit", not "MSYS2 MSYS"

### Issue: "pkg-config: command not found"
- **Solution**: Install pkg-config: `pacman -S mingw-w64-x86_64-pkg-config`

### Issue: "GTK+ not found"
- **Solution**: Install GTK3: `pacman -S mingw-w64-x86_64-gtk3`

### Issue: Can't find project directory
- **Solution**: Use forward slashes: `/c/Users/...` not `C:\Users\...`

### Issue: DLL errors when running
- **Solution**: Always run from MSYS2 MinGW 64-bit terminal (it sets up PATH automatically)

## What's Next?

After successful installation:
1. Build: `make`
2. Run: `./smart_parking.exe`
3. Login with: `admin` / `admin123`
4. Start using the application!

## Need Help?

- Check `QUICK_START.md` for quick reference
- Check `README.md` for full documentation
- Check `build_windows_msys2.md` for detailed build instructions

---

**Estimated total time: 10-15 minutes**

