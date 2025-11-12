# Simple Shell

A tiny UNIX command-line interpreter in C.

 # What it does:
- It Shows a prompt and reads a line
- Runs **built-ins**:
  - `exit` → terminate the shell
  - `env`  → print the current environment
- Runs **external programs** by searching `$PATH` and calling `execve`
- Handles empty lines and EOF (Ctrl-D)
- Works in both interactive and non-interactive modes

# Files
| File        | Purpose |
|-------------|---------|
| `main.c`    | REPL loop: prompt → getline → trim → handle built-ins → execute |
| `exec.c`    | `fork()` child and `execve()` program, parent `wait()`s |
| `path.c`    | Build full path by scanning `$PATH` and checking executability |
| `env.c`     | Implementation of the `env` built-in |
| `shell.h`   | Prototypes, includes, constants |

> Repo also contains runtime/test artifacts: `hsh` (binary), `hbtn_ls` (checker helper), `simple_shell` (wrapper).

## What did we learn ? 
1. How to create and manage processes with fork and execve

2. How environment variables work and how PATH lookup is performed

3. Parsing user input and splitting strings safely

4. Memory handling in C (allocating and freeing properly)

5. Building our own “mini-terminal” from scratch

The Authors 👩‍💻 :
Nawal Alharbi — @nawalsamer04
Alanoud Alanazi — @alanooud1983


At the end ...
“The best way to understand a system… is to build it.”

Developed with ❤️ and a lot of debugging caffeine ☕ as part of Holberton School’s Simple Shell project.


# Build
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh



