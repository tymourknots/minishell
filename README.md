# 🐚 Minishell — A Minimal Unix-Style Shell in C

> A lightweight, educational shell that executes commands, changes directories, and handles `SIGINT` gracefully.  
> Implements core Unix process control with **fork**, **execvp**, and **waitpid**.

---

## 🎥 Demo Preview

_Coming soon — short terminal walkthrough GIF showing command execution and Ctrl-C handling._

---

## 🔎 What the project does

- **Core functionality**
  - Prints the current working directory in a colored prompt: `[ /home/user ]$`
  - Executes external commands using **fork() + execvp()**
  - Waits for foreground processes via **waitpid()**
  - Handles **Ctrl-C** with a custom `SIGINT` handler that returns cleanly to the prompt

- **Built-ins**
  - `cd [path]` — changes the current directory  
    - Supports relative paths and `~` for home directory  
    - Example: `cd ~/Documents`
  - `exit` — terminates the shell loop and exits cleanly

- **Error handling**
  - Prints detailed errors for every failed system call (`chdir`, `malloc`, `fork`, `waitpid`, `execvp`, `getcwd`)
  - Detects malformed quotes in `cd` arguments (`"unclosed"` → prints error)

---

## 🧱 Tech stack 

### Language & System
- **C17** (GCC / Clang)
- **POSIX APIs:** `<unistd.h>`, `<sys/wait.h>`, `<pwd.h>`, `<signal.h>`

### System calls & libraries
- **Process control:** `fork`, `execvp`, `waitpid`
- **Directory & path:** `chdir`, `getcwd`, `getpwuid`
- **Signal handling:** `signal(SIGINT, handler)`
- **Memory:** dynamic `malloc` / `free`
- **Error reporting:** `errno` + `strerror()`

---

## 🗂️ Main features mapped to code

### 1) Prompt & signal handling
- **Where:** `main()` and `signal_handler()`  
  - Uses `getcwd()` to display the current working directory.  
  - Registers a `SIGINT` handler that catches Ctrl-C and prints a newline instead of exiting.  
  - Prints colored prompt using ANSI escape codes (`\x1b[34;1m` for blue, `\x1b[0m` to reset).

### 2) Directory management (`cd`)
- **Where:** `change_directory()` and `parse()`  
  - Supports `cd [path]`, `cd ~`, and `cd ~/subdir`.  
  - Resolves `~` via `getpwuid(getuid())`.  
  - Uses `parse()` to safely extract quoted or unquoted arguments.  
  - Prints `Error: Malformed command.` for bad quoting.

### 3) Command execution
- **Where:** `tokenize()` and the REPL loop  
  - Splits input with `strtok()` into tokens.  
  - `fork()` spawns a child to run `execvp(tokens[0], tokens)`.  
  - Parent process synchronously waits using `waitpid()`.  
  - Handles all fork/exec/wait errors.

### 4) Error resilience
- Checks every system call’s return code.  
- Catches empty or malformed input gracefully.  
- Resets prompt after interrupts.

---

## 🧩 Example session

```
[/home/tymour]$ pwd
/home/tymour
[/home/tymour]$ cd /
[/]$ echo "hello world"
hello world
[/]$ cd ~/Desktop
[/home/tymour/Desktop]$ sleep 5
^C
[/home/tymour/Desktop]$ exit
```

---

## 🔎 Known limitations

- No pipelines (`|`), redirection (`>`, `<`), or background execution (`&`)
- No environment variable expansion (`$VAR`)
- Quoting support exists **only** in `cd`; other commands use raw tokenization
- No command history, tab completion, or job control

---

## 🛠️ Build & run

### Requirements
- **GCC** or **Clang**  
- **Linux**, **macOS**, or **WSL** (uses POSIX system calls)

### Commands
```bash
cd src
make        # builds minishell
./minishell # runs it
```

### Note
Windows builds using `cl.exe` will fail — use **WSL** or any Unix-like environment.

---

## 🧭 Parsing roadmap (next steps)

### 1️⃣ Improved tokenizer
- Split input correctly while honoring **quotes** and **operators**  
- Keep `"quoted text"` as one token  
- Support escaped quotes `\"`  
- Recognize operators (`|`, `<`, `>`, `>>`, `2>`, `&`) as separate tokens

### 2️⃣ Basic redirection & pipe support
- Implement `>` and `<` using `dup2()` before `execvp()`  
- Add a single pipeline (`cmd1 | cmd2`) using one pipe and two children

### 3️⃣ Built-in polish
- Support `cd` with no args → `$HOME`  
- Add `cd -` for previous directory  
- Support `exit N` to return a custom exit code

> These steps evolve Minishell into a fully functional multi-process interpreter while keeping the design minimal and readable.

---

## ✍️ Lessons learned
- **Signal safety:** only async-safe calls like `write()` inside signal handlers  
- **Process hygiene:** use `waitpid()` to prevent zombies  
- **Design clarity:** separate `parse()` logic simplifies testing and future extensions  
- **Error discipline:** consistent messages ease debugging  
- **Learning value:** small, well-scoped shells are ideal for mastering process control and POSIX fundamentals

---

## 👤 Author
**Tymour Aidabole** — Former U.S. Army Ranger • B.A. Computer Science, Columbia University  
**LinkedIn:** [https://www.linkedin.com/in/tymour-aidabole-1284b0159/](https://www.linkedin.com/in/tymour-aidabole-1284b0159/)  
**GitHub:** [https://github.com/tymourknots](https://github.com/tymourknots)

---
