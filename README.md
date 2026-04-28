# Secure-Coding

A collection of secure-coding labs and course projects (primarily in **C**) covering common software vulnerabilities, mitigations, and secure development practices.

## Repository Contents

This repo is organized by topic/module. You’ll find items such as:

- **`Numeric Overflow/`** — examples and notes around integer/numeric overflow issues.
- **`BufferOverflow/`** — buffer overflow demonstrations and safer handling patterns.
- **`SQL_Injection/`** — SQL injection examples and defensive techniques (e.g., parameterized queries).
- **`M5 Encryption/`** — encryption-related exercises and implementations.
- **`M5_static_analysis_vs/`** — static analysis work (Visual Studio tooling / reports / project files).
- **`Security Policy.docx`** — written security policy document.
- **`Project Script.docx`** — supporting course/project documentation.

## Getting Started (C projects)

Many folders contain small C programs. A typical build/run flow on Linux/macOS looks like:

```bash
cd "<folder>"
gcc -Wall -Wextra -O2 -o program *.c
./program
```

If a folder contains Visual Studio project files, open the solution in Visual Studio and build/run from there.

### Helpful compiler flags (optional)

For debugging and catching memory issues:

```bash
gcc -g -Wall -Wextra -fsanitize=address,undefined -o program *.c
./program
```

## Safety / Disclaimer

Some code in this repository may be **intentionally vulnerable** for learning purposes (e.g., buffer overflow, injection patterns).  
Use only in a safe, local environment and **do not deploy** vulnerable examples to production systems.

## Goals / Learning Outcomes

- Recognize common vulnerability classes (overflow, memory corruption, injection, etc.)
- Apply secure coding practices in C
- Use static analysis and compiler/runtime defenses
- Document security requirements and policies
