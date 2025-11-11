🐚 Minishell — A Minimal Unix-Style Shell in C

A lightweight, educational shell that executes commands, changes directories, and handles SIGINT gracefully.
Implements core Unix process control with fork, execvp, and waitpid.

⚙️ What the project does

Core functionality

Prints the current working directory in a colored prompt: [ /home/user ]$

Executes external commands using fork() + execvp()

Waits for foreground processes via waitpid()

Handles Ctrl-C with a custom SIGINT handler (returns safely to prompt)

Built-ins

cd [path] — changes the current directory

Supports relative paths and ~ for home directory

Example: cd ~/Documents

exit — terminates the shell loop and exits cleanly

Error handling

Descriptive error messages for every failed system call (chdir, malloc, fork, waitpid, execvp, getcwd)

Detects malformed quotes in cd arguments ("unclosed" → prints error)

🧱 Tech stack
Language & System

C17 (GCC / Clang)

POSIX APIs: <unistd.h>, <sys/wait.h>, <pwd.h>, <signal.h>

System calls & libraries

Process control: fork, execvp, waitpid

Directory & path: chdir, getcwd, getpwuid

Signal handling: signal(SIGINT, handler)

Memory: dynamic malloc / free

Error reporting: errno + strerror()

🗂️ Code structure
Component	Purpose
signal_handler()	Catches Ctrl-C, prints newline, sets interrupt flag
change_directory()	Executes chdir() and prints errors if it fails
parse()	Safely extracts first quoted or unquoted argument (used for cd)
tokenize()	Splits input by spaces using strtok()
main()	Core REPL: prints prompt, reads input, routes built-ins or forks processes

Prompt format

[<blue working directory>]$ 


Uses ANSI escape codes for color (\x1b[34;1m for bright blue, \x1b[0m to reset).

🧩 Example session
[/home/tymour]$ pwd
/home/tymour
[/home/tymour]$ cd /
[/]$ echo "hello world"
hello world
[/]$ cd ~/Desktop
[/home/tymour/Desktop]$ sleep 5
^C
[/home/tymour/Desktop]$ exit

🔎 Known limitations

No pipelines (|), redirection (>, <), or background execution (&)

No environment variable expansion ($VAR)

Quoting support exists only in cd parsing; other commands use raw strtok()

No command history, tab completion, or advanced job control

🛠️ Build & run
Requirements

GCC or Clang

Linux, macOS, or WSL (uses POSIX system calls)

Commands
cd src
make        # builds minishell
./minishell # runs it

Note

Windows builds using cl.exe will fail — use WSL or any Unix-like environment.

🧭 Parsing roadmap (next steps)
1️⃣ Improved tokenizer

Split input correctly while honoring quotes and operators.

Keep "quoted text" as one token.

Support escaped quotes \".

Recognize operators (|, <, >, >>, 2>, &) as separate tokens.

2️⃣ Basic redirection & pipe support

Implement > and < using dup2() before execvp().

Add a single pipeline (cmd1 | cmd2) using one pipe and two child processes.

3️⃣ Built-in polish

Support cd with no args → $HOME.

Add cd - for previous directory.

Support exit N to return custom exit code.

These incremental steps evolve Minishell into a functional multi-process interpreter while keeping the design clean and minimal.

✍️ Lessons learned

Signal handling must use async-safe functions like write() inside handlers.

Using waitpid prevents zombies from accumulating.

Keeping parse() isolated simplifies testing and future token expansion.

Defensive error messages make debugging user input and system calls trivial.

Simple, well-scoped shells are perfect exercises for understanding process control.

👤 Author

Tymour Aidabole — Former U.S. Army Ranger • B.A. Computer Science, Columbia University
LinkedIn: https://www.linkedin.com/in/tymour-aidabole-1284b0159/

GitHub: https://github.com/tymourknots