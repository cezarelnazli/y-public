%macro SYSCALL 2
    GLOBAL %1
    %1:
        push %2
        mov eax, esp
        sysenter
%endmacro


SYSCALL print,  0
SYSCALL yield,  1
SYSCALL exit,   2
SYSCALL fork,   3
SYSCALL exec,   4
SYSCALL twait,  5
SYSCALL prints, 6
SYSCALL read,   7
