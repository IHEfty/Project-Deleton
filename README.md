# Project-Deleton
This script is designed as a **scambaiting tool** for those who want to fight back against scammers who may have stolen or gained access to your personal data.

### Why this script exists:

* **Scammers often collect your private files and information to exploit or intimidate you.**

* If you’re a **scambaiter** (someone who plays tricks on scammers), this script lets you send a strong message back by **overwriting targeted files with a warning message**.

* Instead of simply deleting files, this script replaces the contents of important files with a **clear, visible warning:**

  > *"Wooh Delete fool....Scammer hahaha...."*

* This causes a psychological impact on scammers: they see every file they stole, but **cannot access the original data anymore**.

* It’s like a nightmare for them **worse than deleting files silently**, because they face the constant reminder every time they try to open any stolen document or image.
---

## ⚙️ Features

1. **Full Delete Dropper (`delete.exe`)**  
   - Installs itself as `delpod.exe` in  
     `C:\Program Files (x86)\Microsoft\Edge\Application\`  
   - Registers in your current user’s **Run** registry key so it auto-starts at login.  
   - Runs silently in the background, watching drives `C:`–`G:` for new or modified files.  
   - Recursively overwrites any matching extension (`.txt`, `.pdf`, `.jpg`, etc.) with the taunt.

2. **Safe Test Utility (`safe.exe`)**  
   - Overwrites **only** files in its own folder (no recursion).  
   - Displays a progress list in the console for every file it processes.  
   - Ideal for verifying behavior before deploying the full delete dropper.

---

## 📥 Getting Started

> **Recommendation:** Download the pre-compiled **Release** ZIP from this repo. Using the release binaries avoids compilation issues or accidental alterations.  
> (If you do clone and build yourself, follow the instructions below.)

### From Releases

1. Grab the latest **Release** ZIP.
2. Extract and run:
   - `git clone https://github.com/IHEfty/Project-Deleton.git`
   - `cd Project-Deleton`

### From Source

```bash
# Build Safe Test
gcc safe.c -mwindows -o safe.exe

# Build Full Delete Dropper
gcc delete.c -mwindows -lshlwapi -o delete.exe
````
### Safe “Test” Version

If you’d like to try this in a controlled environment first, there’s also a safe test variant that you can find in the releases:

**What this test version does:**

* Detects the folder where the script or executable runs.
* Lists files in **only that folder** (no subfolders).
* Overwrites all matching files with the warning message.
* Shows a progress bar for the files found.

This is perfect for safely testing the behavior on a small, controlled folder without risking data on other drives.

### Use responsibly:

* This tool is meant for **emergency situations** and for users aware of the consequences data overwritten by this script **cannot be recovered**.
* Only run this if you are certain your files are compromised and you want to **take a stand against scammers**.
* Always keep backups of important data elsewhere to avoid accidental loss.
* If you find an executable version of this tool in the releases, **do NOT run it on your own PC unless you have a full backup** of your important files.

---
> ![1600593720\_1shutterstock\_406413301](https://github.com/user-attachments/assets/09c49d2a-c8bc-4ecb-9262-7f52bf7393f4)

© 2025 IHEfty — use at your own risk.
