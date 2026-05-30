# Homework — Shell Script & Environment Variables

---

### Exercise 1 — Your First Shell Script

Create a file `info.sh` that prints the following information when executed:

```
Hello, <current logged-in username>!
Your home directory is: <home directory>
Your shell is: <current shell>
Current directory: <current directory>
```

**Requirements:** no hardcoding — must use system variables.

---

### Exercise 2 — Script with Parameters

Create a file `greet.sh` that accepts 2 parameters: name and age. Print:

```
Hello, my name is <name> and I am <age> years old.
Total arguments received: <number of arguments>
```

If fewer than 2 parameters are provided, print usage instructions and exit.

---

### Exercise 3 — Read and Inspect Environment Variables

Open a terminal and perform the following:

- Print the value of `$PATH`, `$HOME`, `$USER`, `$SHELL`
- Count how many directories are in `$PATH` (hint: use `tr` and `wc`)
- Use `printenv` to list all environment variables and count how many there are

---

### Exercise 4 — Variable Scope

Follow each step below and observe the result:

```bash
# Step 1 — create a regular variable
MY_NAME="Quan"
echo $MY_NAME        # visible?

# Step 2 — open a child shell and check
bash
echo $MY_NAME        # visible? why?
exit

# Step 3 — export the variable
export MY_NAME="Quan"
bash
echo $MY_NAME        # visible? why?
exit

# Step 4 — modify variable inside child shell
bash
MY_NAME="Alice"
echo $MY_NAME        # what do you see inside child shell?
exit
echo $MY_NAME        # back in parent shell — what do you see? why?
```

**Goal:** observe and explain why the result is different at each step.
