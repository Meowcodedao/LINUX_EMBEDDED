# Exercise 4 — Variable Scope: Observations and Explanations

## Step 1 — Regular variable in current shell
```bash
MY_NAME="Quan"
echo $MY_NAME   # Output: Quan
```
**Result:** `Quan` is printed. The variable exists in the current shell session.

---

## Step 2 — Child shell (without export)
```bash
bash
echo $MY_NAME   # Output: (empty)
exit
```
**Result:** Nothing is printed.  
**Why:** A regular variable (not exported) is local to the shell that created it. Child shells do not inherit it.

---

## Step 3 — Export variable then open child shell
```bash
export MY_NAME="Quan"
bash
echo $MY_NAME   # Output: Quan
exit
```
**Result:** `Quan` is printed inside the child shell.  
**Why:** `export` marks the variable to be passed into the environment of any child processes, including child shells.

---

## Step 4 — Modify variable inside child shell
```bash
bash
MY_NAME="Alice"
echo $MY_NAME   # Output inside child: Alice
exit
echo $MY_NAME   # Output in parent: Quan
```
**Result:** Inside child shell prints `Alice`, but back in parent shell it still prints `Quan`.  
**Why:** A child shell gets a **copy** of the parent's exported variables. Any modification in the child does NOT propagate back to the parent. Shell variables do not flow upward — only downward via export.

---

## Summary Table

| Step | Scenario                         | Result in child | Why                                    |
|------|----------------------------------|-----------------|----------------------------------------|
| 1    | Set without export               | N/A             | Only visible in current shell          |
| 2    | Child shell without export       | Empty           | Not inherited — not exported           |
| 3    | Child shell with export          | Quan            | export passes variable to child env    |
| 4    | Modify exported var in child     | Alice (child), Quan (parent) | Child gets a copy, changes don't go up |
