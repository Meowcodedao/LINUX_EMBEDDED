# Lab Exercises — Static Library · Shared Library · Makefile

> **Language:** C &nbsp;|&nbsp; **OS:** Linux &nbsp;|&nbsp; **Compiler:** GCC

---

## Exercise 1 — Build a Static Library with Makefile

### Objective

Create a static library in C on Linux and use a Makefile to automate the build process.

### Directory Structure

```
project/
├── include/    ← header files (.h)
├── src/        ← library source files (.c)
├── app/        ← main program source
├── lib/        ← output: libmathutils.a
├── obj/        ← intermediate object files (.o)
└── Makefile
```

### Library Functions

Create a static library **`libmathutils.a`** that provides the following functions:

- Add two integers
- Subtract two integers
- Compute the factorial of a non-negative integer

### Main Program

Write a program inside `app/` that:

- Reads input from the terminal
- Calls the functions provided by the library
- Prints the results to stdout

### Makefile Requirements

The Makefile must support the following targets:

| Target  | Description                                  |
|---------|----------------------------------------------|
| `all`   | Build the entire project                     |
| `clean` | Remove all build artifacts                   |
| `run`   | Build if necessary, then run the executable  |

### Constraints

- Use `gcc`
- Use a static library `.a` (created with the `ar` command)
- Keep headers (`.h`) and sources (`.c`) in separate directories
- Do not put all code in a single file
- The output executable must be named `main`

### Acceptance Criteria

The following commands must all succeed without errors:

```bash
make        # build succeeds
make run    # program runs, reads input, prints results
make clean  # removes obj/, lib/, and the executable
make        # rebuilds successfully from scratch
```

---

## Exercise 2 — Build a Shared Library on Linux

### Objective

Create a shared library in C on Linux and dynamically link it with an executable.

### Directory Structure

```
project/
├── include/    ← header files (.h)
├── src/        ← library source files (.c)
├── build/      ← intermediate object files (.o)
├── bin/        ← output: libstringutils.so and executable
└── Makefile
```

### Library Functions

Create a shared library **`libstringutils.so`** that provides the following functions:

- Convert a string to uppercase in-place
- Count the number of characters in a string (excluding the null terminator)
- Reverse a string in-place

### Main Program

Write a program that:

- Accepts a string from a **command line argument** (`argv[1]`)
- Calls all three library functions on the input string
- Prints the result of each function to the terminal

### Makefile Requirements

The Makefile must support the following targets:

| Target  | Description                                      |
|---------|--------------------------------------------------|
| `all`   | Build the `.so` library and the executable       |
| `clean` | Remove all build artifacts                       |
| `run`   | Run the program with a sample input string       |

### Constraints

- Use `gcc`
- Compile the library with the `-fPIC` and `-shared` flags
- Keep headers and sources in separate directories
- Do not write the entire build as a single long command — break it into clear steps in the Makefile
- The executable must link against the shared library using `-L` and `-l`

### Acceptance Criteria

```bash
make        # build succeeds
make run    # prints results of all 3 functions on a sample string
make clean  # removes build/ and bin/
```

Additional verification:

```bash
ldd bin/<executable>   # libstringutils.so must appear in the dependency list
```

---

## Exercise 3 — Combining Static Library + Shared Library

### Objective

Build a Linux project that uses both a static library and a shared library, managed by a Makefile with multiple targets.

### Directory Structure

```
project/
├── include/    ← all header files
├── src/        ← sources for both libraries and the main program
├── lib/        ← output: libcalc.a and liblogger.so
├── obj/        ← intermediate object files
├── bin/        ← final executable
└── Makefile
```

### Static Library — `libcalc.a`

Provides basic arithmetic functions for floating-point numbers:

- Addition
- Subtraction
- Multiplication
- Division (must handle division by zero)

### Shared Library — `liblogger.so`

Provides logging functions:

- Write a log entry with arbitrary content to `app.log`
- Print the current timestamp in `YYYY-MM-DD HH:MM:SS` format
- Write an error log entry prefixed with `[ERROR]`

### Main Program

The executable must:

- Use **both libraries**
- Perform several arithmetic operations using `libcalc.a`
- Log results and any errors to **`app.log`** using `liblogger.so`
- Print a summary of results to the terminal

### Makefile Requirements

The Makefile must include all of the following targets:

| Target    | Description                              |
|-----------|------------------------------------------|
| `all`     | Build everything (both libs + executable)|
| `libs`    | Build both libraries                     |
| `static`  | Build `libcalc.a` only                   |
| `shared`  | Build `liblogger.so` only                |
| `run`     | Run the executable after building        |
| `clean`   | Remove all build artifacts               |
| `rebuild` | Run `clean` then `all`                   |

### Constraints

- Use meaningful Makefile variables (`CC`, `CFLAGS`, `LDFLAGS`, etc.)
- Discover source files automatically using **wildcard** (`$(wildcard src/*.c)`)
- Use a **pattern rule** `%.o: %.c` — do not list each `.o` file individually
- Compile the shared library with `-fPIC`
- Do not hardcode individual `.o` filenames anywhere in the Makefile

### Acceptance Criteria

```bash
make static    # produces lib/libcalc.a
make shared    # produces lib/liblogger.so
make           # full build succeeds
make run       # prints results to terminal, creates app.log
make clean     # removes obj/, lib/, bin/
make rebuild   # clean + full rebuild succeeds
```

Additional verification:

```bash
cat app.log           # contains log entries with timestamps
ldd bin/<executable>  # liblogger.so is listed; libcalc is not (static)
nm  lib/libcalc.a     # symbols visible: add, sub, mul, divf
```
