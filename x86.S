GLOBAL rdmsr
rdmsr:
    push ebp
    mov ebp, esp

    push esi
    mov esi, [ebp+12]

    push edi
    mov edi, [ebp+16]

    mov ecx, [ebp+8]
    rdmsr
    mov [esi], edx
    mov [edi], eax

    pop edi
    pop esi

    pop ebp
    ret

GLOBAL wrmsr
wrmsr:
    mov ecx, [esp+4]
    mov edx, [esp+8]
    mov eax, [esp+12]
    wrmsr
    ret

GLOBAL outb
outb:
    mov dx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret

GLOBAL inb
inb:
    mov dx, [esp+4]
    xor eax, eax
    in al, dx
    ret

GLOBAL outsb
outsb:
    mov ecx, [esp+12]
    mov esi, [esp+8]
    mov dx, [esp+4]
    cld
    rep outsb
    ret

GLOBAL insb
insb:
    mov ecx, [esp+12]
    mov edi, [esp+8]
    mov dx, [esp+4]
    cld
    rep insb
    ret

GLOBAL lcr3
lcr3:
    mov eax, [esp+4]
    mov cr3, eax
    ret

GLOBAL invlpg
invlpg:
    mov eax, [esp+4]
    invlpg [eax]
    ret

GLOBAL ltr
ltr:
    ltr [esp+4]
    ret

GLOBAL rcr2
rcr2:
    mov eax, cr2
    ret

GLOBAL lgdt
EXTERN gdtp
lgdt:
    lgdt [gdtp]
    mov ax, 0x10        ; offset into GDT of first byte of Data segment
    mov ds, ax
    mov es, ax
    mov ss, ax
    jmp 0x08:lgdt_ret   ; 0x08 offset into GDT of Code segment
                        ; this is done because the Code segment
                        ; cannot be set manually
lgdt_ret:
    ret

GLOBAL lidt
EXTERN idtp
lidt:
    lidt [idtp]
    ret

; this is where fork returns in the child
; makes it look like fork returned 0 (by setting EAX)
; and then continues with the system call epilogue
GLOBAL fork_child_ret
fork_child_ret:
    mov eax, 0
    jmp sysenter_eip_epilogue

GLOBAL sysenter_eip
GLOBAL sysenter_eip_epilogue
EXTERN syscall_handle
EXTERN fgtask
EXTERN sys_prints
sysenter_eip:
    sti
    mov esp, [fgtask]       ; load the kernel stack of the foreground task.
                            ; see the definition of `struct task` in task.h.
                            ; we do this because when arriving from `sysenter`
                            ; a predefined stack is loaded; however, we need to
                            ; save per-task information on the stack when
                            ; yielding, otherwise we don't know where to resume
                            ; a task.
    push eax
    call syscall_handle

sysenter_eip_epilogue:
    push eax
    mov ax, 0x23
    mov ds, ax
    pop eax                 ; system call return value
    pop ecx                 ; user ESP
    mov edx, [ecx+4]        ; user EIP
    add ecx, 8              ; sysexit does not pop what we pushed in user mode
    sysexit
