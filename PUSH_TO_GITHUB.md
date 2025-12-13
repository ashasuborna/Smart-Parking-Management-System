# How to Push to GitHub

## Option 1: Create Repository on GitHub First (Recommended)

### Step 1: Create Repository on GitHub
1. Go to: https://github.com/new
2. Repository name: `Smart-Parking-Management-System`
3. Description: `Smart Parking Management System - University Project in C with GTK+3`
4. Choose: **Public** or **Private**
5. **DO NOT** check "Initialize with README" (we already have one)
6. Click "Create repository"

### Step 2: Push Your Code
After creating the repository, run in your terminal:

```bash
git push -u origin master
```

If it asks for credentials:
- Username: Your GitHub username
- Password: Use a **Personal Access Token** (not your password)
  - Get token from: https://github.com/settings/tokens
  - Create token with "repo" permissions

---

## Option 2: Use SSH (If you have SSH keys set up)

1. Change remote URL:
```bash
git remote set-url origin git@github.com:ashasuborna/Smart-Parking-Management-System.git
```

2. Push:
```bash
git push -u origin master
```

---

## Option 3: Create Repository via GitHub CLI (if installed)

```bash
gh repo create Smart-Parking-Management-System --public --source=. --remote=origin --push
```

---

## After Pushing

Your repository will be available at:
https://github.com/ashasuborna/Smart-Parking-Management-System

