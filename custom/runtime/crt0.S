.globl _start, main, SystemInit
.weak SystemInit

.globl trap_handler
.weak trap_handler

.altmacro
.macro memcpy src_beg, src_end, dst, tmp_reg
    LOCAL memcpy_1, memcpy_2
    j    memcpy_2
memcpy_1:
    lw   \tmp_reg, (\src_beg)
    sw   \tmp_reg, (\dst)
    add  \src_beg, \src_beg, 4
    add  \dst, \dst, 4
memcpy_2:
    bltu \src_beg, \src_end, memcpy_1
.endm

.macro memset dst_beg, dst_end, val_reg
    LOCAL memset_1, memset_2
    j    memset_2
memset_1:
    sw   \val_reg, (\dst_beg)
    add  \dst_beg, \dst_beg, 4
memset_2:
    bltu \dst_beg, \dst_end, memset_1
.endm


# la использует относительную адресацию ПК (инструкция auipc)
# Явная абсолютная адресация с инструкцией lui используется
# для того, чтобы разрешить запуск кода с любого адреса ПК
# (эквиваленты инструкции la оставлены в комментариях)
    
.macro la_abs reg, address
    .option push
    .option norelax
    lui \reg, %hi(\address)
    addi \reg, \reg, %lo(\address)
    .option pop
.endm


.macro jalr_abs return_reg, address
    .option push
    .option norelax
    lui \return_reg, %hi(\address)
    jalr \return_reg, %lo(\address)(\return_reg)
    .option pop
.endm


.text
_start:
    # Инициализация стека и глобального указателя
    la_abs  sp, __C_STACK_TOP__
    la_abs  gp, _gp
    
    # Установка вектора прерывания
    # в зависимости от результата сборки
    lui t0, %hi(2f)
    csrrw zero, mtvec, t0

    # Перенос кода и переменных в RAM
    la_abs  a1, __DATA_IMAGE_START__
    la_abs  a2, __DATA_IMAGE_END__
    la_abs  a3, __DATA_START__
    memcpy  a1, a2, a3, t0

    # Инициализация обнулением
    la_abs  a1, __BSS_START__
    la_abs  a2, __BSS_END__
    memset a1, a2, zero
    
    jalr_abs ra, SystemInit
    jalr_abs ra, main
1:  wfi
    j 1b

    
// Действия перед main: по умолчанию отсутствуют
// (SystemInit - может быть переопределена)
SystemInit:
    ret

.section .trap_text, "ax"
//.org 0xC0
trap_handler:
2:  j 2b
