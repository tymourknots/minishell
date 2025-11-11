🐚 Minishell — A Minimal Unix-Style Shell in C

A lightweight, educational shell that executes commands, changes directories, and handles SIGINT gracefully.
Implements core Unix process control with fork, execvp, and waitpid.

⚙️ What the project does

Core functionality

Prints the current working directory in a colored prompt: [ /home/user ]$

Executes external commands using fork() + execvp()

Waits for foreground processes via waitpid()

Handles Ctrl-C with a custom SIGINT handler (returns to prompt instead of exiting)

Built-ins

cd [path] — changes the current directory

Supports relative paths and ~ for home.

Example: cd ~/Documents

exit — terminates the shell loop and exits cleanly.

Error handling

Descriptive stderr output for every system call failure (chdir, malloc, fork, waitpid, execvp, getcwd).

Detects malformed quotes in cd arguments.

🧱 Tech stack

Language & System

C17 (GCC / Clang)

POSIX APIs (unistd.h, sys/wait.h, pwd.h, signal.h)

System calls & libraries

Process control: fork, execvp, waitpid

Directory & path: chdir, getcwd, getpwuid

Signal handling: signal(SIGINT, handler)

Memory: dynamic malloc / free

Error reporting: errno + strerror()

🗂️ Code overview
Component	Purpose
signal_handler()	Catches Ctrl-C, prints newline, sets interrupt flag
change_directory()	Executes chdir() and prints errors if it fails
parse()	Safely extracts first quoted or unquoted argument (used for cd)
tokenize()	Splits input by spaces using strtok()
main()	Core REPL: prints prompt, reads input, routes built-ins or forks processes

Prompt format

[<blue working directory>]$ 


Uses ANSI escape codes for color (\x1b[34;1m and reset \x1b[0m).

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

Quoting support exists only in cd parsing; all other commands use raw strtok()

No command history or tab completion

🛠️ Build & run
Requirements

GCC or Clang

Linux, macOS, or WSL environment (uses POSIX system calls)

Commands
cd src
make        # builds minishell
./minishell # runs it

Note

Windows builds with cl.exe will fail — use WSL or a Unix-like system.

🧭 Parsing roadmap (next steps)

1️⃣ Improved tokenizer
Split the input correctly while honoring quotes and operators.

Keep "quoted text" as one token.

Support escaped quotes \".

Recognize operators (|, <, >, >>, 2>, &) as separate tokens.

2️⃣ Basic redirection & pipe support
After lexing:

Implement > and < with dup2() before execvp().

Add a single pipeline (cmd1 | cmd2) using one pipe and two child processes.

3️⃣ Built-in polish

Support cd with no args → $HOME

Add cd - for previous directory

Support exit N to return status code N

These incremental steps evolve the current shell into a functional multi-process interpreter while keeping the code lean and readable.

👤 Author

Tymour Aidabole — Former U.S. Army Ranger • B.A. Computer Science, Columbia University
LinkedIn
 • GitHub