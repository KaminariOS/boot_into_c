
ENTRY(start)
  
SECTIONS {
  . = 1M; /* Tells GRUB to load the kernel starting at the 1MB mark */

  .rodata :
  {
    /* ensure that the multiboot header is at the beginning */
    KEEP(*(.multiboot_header))
    *(.rodata .rodata.*) 
    . = ALIGN(4K);
  }

  .text :
  { 
    *(.text .text.*)
    . = ALIGN(4K);
  } 
    
  .data :
  { 
    *(.data .data.*)
    . = ALIGN(4K);
  }   
    
  .bss :
  {   
    *(.bss .bss.*)
    . = ALIGN(4K);
  }     
} 
