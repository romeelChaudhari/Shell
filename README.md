Implemented my  own Shell or Command Line Interpreter (e.g. to replace /bin/bash for simple interactions with the Linux Kernel.).

It supports the following functions :
• Execute a single command with up to four command line arguments (including command line arguments with associated flags). For example:
Myshell> ls –l
Myshell> cat myfile
Myshell> ls –al /usr/src/linux
• Execute a command in background. For example: Myshell> ls -­­l &
Myshell> ls –al /usr/src/linux &
• Redirect the standard output of a command to a file. For example:
Myshell> ls -­­l > outfile
Myshell> ls -­­l >> outfile
Myshell> ls –al /usr/src/linux > outfile2 Myshell> ls –al /usr/src/linux >>outfile2
• Redirect the standard input of a command to come from a file. For example: Myshell> grep disk < outfile
Myshell> grep linux < outfile2
• Execute multiple commands connected by a single shell pipe. For example:
Myshell> ls –al /usr/src/linux | grep linux
• Execute the cd and pwd commands
Myshell> cd some_path
Myshell> pwd
