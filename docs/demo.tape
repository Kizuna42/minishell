# Reproducible terminal demo for README — render with:  vhs docs/demo.tape
# (run from the repository root after `make`)
Output docs/demo.gif

Set Shell "bash"
Set FontSize 18
Set Width 1100
Set Height 660
Set Theme "Dracula"
Set Padding 24
Set TypingSpeed 55ms

Hide
Type "clear" Enter
Type "./minishell" Enter
Sleep 700ms
Show

Type 'echo "Hello $USER — welcome to minishell"' Enter
Sleep 900ms
Type "ls *.md *.sh | sort" Enter
Sleep 900ms
Type "export NAME=42 && echo built in C at $NAME" Enter
Sleep 900ms
Type "cat << EOF" Enter
Sleep 250ms
Type "heredoc expands variables:" Enter
Sleep 200ms
Type "  HOME=$HOME" Enter
Sleep 200ms
Type "EOF" Enter
Sleep 1200ms
Type 'false; echo "exit status -> $?"' Enter
Sleep 900ms
Type "echo pipes and wildcards work | cat | wc -w" Enter
Sleep 900ms
Type "exit" Enter
Sleep 900ms
