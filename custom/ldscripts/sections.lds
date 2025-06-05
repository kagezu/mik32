
SECTIONS {
    .text ORIGIN(REGION_TEXT) : {
        PROVIDE(__TEXT_START__ = .);
        *crt0.o(.text .text.*)
        . = ORIGIN(REGION_TEXT) + 0xC0;
        KEEP(*crt0.o(.trap_text))

        *(.small_ram_text)
        *(.ram_text)
        *(.text.SystemInit)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)

        # секции для вызова конструкторов, если код на плюсах и есть глобальные объекты,
        # которые требуют вызов конструкторов
        PROVIDE_HIDDEN ( __preinit_array_start = . );
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN ( __preinit_array_end = . );

        PROVIDE_HIDDEN ( __init_array_start = . );
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array .ctors))
        PROVIDE_HIDDEN ( __init_array_end = . );

        . = ALIGN(CL_SIZE);
        PROVIDE(__TEXT_END__ = .);
    } >REGION_TEXT 

    .data : 
    AT( __TEXT_END__ ) {
        PROVIDE(__DATA_START__ = .);
        _gp = .;
        *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        *(.data .data.*)
        . = ALIGN(CL_SIZE);
        PROVIDE(__DATA_END__ = .);
    } >REGION_RAM
    
    __DATA_IMAGE_START__ = LOADADDR(.data);
    __DATA_IMAGE_END__ = LOADADDR(.data) + SIZEOF(.data);

    /* thread-local data segment */
    .tdata : {
        PROVIDE(_tls_data = .);
        PROVIDE(_tdata_begin = .);
        *(.tdata .tdata.*)
        PROVIDE(_tdata_end = .);
        . = ALIGN(CL_SIZE);
    } >REGION_RAM

    .tbss : {
        PROVIDE(__BSS_START__ = .);
        *(.tbss .tbss.*)
        . = ALIGN(CL_SIZE);
        PROVIDE(_tbss_end = .);
    } >REGION_RAM

    /* bss segment */
    .sbss : {
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.scommon)
    } >REGION_RAM

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(CL_SIZE);
        PROVIDE(__BSS_END__ = .);
    } >REGION_RAM

    _end = .;
    PROVIDE(__end = .);

    /* End of uninitalized data segement */

    .stack ORIGIN(REGION_RAM) + LENGTH(REGION_RAM) - STACK_SIZE : {
        FILL(0);
        PROVIDE(__STACK_START__ = .);
        . += STACK_SIZE;
        PROVIDE(__C_STACK_TOP__ = .);
        PROVIDE(__STACK_END__ = .);
    } >REGION_RAM

    /DISCARD/ : {
        *(.eh_frame .eh_frame.*)
    }
}
