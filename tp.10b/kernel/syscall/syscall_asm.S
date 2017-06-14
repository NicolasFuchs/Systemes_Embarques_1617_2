#include "syscall.h"
	.text
	.global syscall
syscall:
	nop
	stmfd	sp!,{r4,lr}
	ldr	r4, [sp, #8]	
	svc	#SYSCALL_ID
	ldmfd	sp!,{r4,pc}
