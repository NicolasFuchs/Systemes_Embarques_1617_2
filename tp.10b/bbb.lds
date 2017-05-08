OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

/* The memory map */
MEMORY {
    SRAM :     o = 0x402F0400,  l = 0x0000FC00  /* 64KB internal SRAM */
    L3OCMC0 :  o = 0x40300000,  l = 0x00010000  /* 64KB L3 OCMC SRAM */
    M3SHUMEM : o = 0x44D00000,  l = 0x00004000  /* 16KB M3 Shared Unified Code Space */
    M3SHDMEM : o = 0x44D80000,  l = 0x00002000  /* 8KB M3 Shared Data Memory */
    DDR0 :     o = 0x80000000,  l = 0x20000000  /* 512MB external DDR Bank 0 */
    DDR0_dma : o = 0x80f00000,  l = 0x00100000  /* 1MB on external DDR Bank 0 */
}

SECTIONS
{
	.text : 
	{
		. = . + 0x1000;
		*(.text_startup)
		*(.text*)   
	} > DDR0

	.rodata : 
	{ 
		. = ALIGN(0x100);
		*(.rodata*) 
	} >DDR0

	.data : 
	{ 
		. = ALIGN(0x100);
		*(.data*)
	} >DDR0
  
	. = ALIGN(0x100);
	__exidx_start = .;
	.ARM.exidx : 
	{ 
		*(.ARM.exidx* .gnu.linkonce.armexidx.*) 
	} > DDR0
	__exidx_end = .;
  
	.bss :
	{
		. = ALIGN (0x100);
		__bss_start__ = . ;
		*(.bss*) 
		*(COMMON)
		. = ALIGN (0x100);
		__bss_end__ = .;
	} >DDR0

	_end = ALIGN(0x100);
	_stack_bottom = ORIGIN(DDR0) + LENGTH(DDR0) - 0x80000;
	__c_stack_top__ = ORIGIN(DDR0) + LENGTH(DDR0);

	.dmasafe :
	{
		*(.dmasafe)
	} >DDR0_dma
} 
