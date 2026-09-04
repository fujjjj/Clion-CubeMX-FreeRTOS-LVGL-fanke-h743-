/*
 * STM32H743IITx linker script translated from Keil scatter
 * (MDK-ARM/FK743M1/FK743M1.sct) for the FK743M1 target:
 *
 *   LR_IROM1 0x08000000 2MB  -> FLASH (code + normal RO data)
 *   RW_IRAM1 0x20000000 128K -> RAM   (RW + ZI)
 *   LR_QSPI  0x90000000 8MB  -> QSPI  (ui_img_*.o / ui_font_*.o RO data)
 *
 * SDRAM (0xC0000000, 32MB) is runtime-initialized and holds framebuffers and
 * the LVGL memory pool; it has no load-time image, so it is not listed here.
 */
ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);
_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2M
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  AXIRAM (xrw): ORIGIN = 0x24000000, LENGTH = 512K
  QSPI  (rx)  : ORIGIN = 0x90000000, LENGTH = 8M
}

PHDRS
{
  flash PT_LOAD FLAGS(5);   /* R + X */
  qspi  PT_LOAD FLAGS(5);   /* R + X */
  data  PT_LOAD FLAGS(6);   /* R + W */
}

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH :flash

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >FLASH :flash

  /* Keil LR_QSPI: UI image/font read-only data lives in memory-mapped W25Q64. */
  .qspi_ui :
  {
    . = ALIGN(4);
    __qspi_ui_start = .;
    *ui_img_*(.rodata*)
    *ui_img_*(.constdata*)
    *ui_font_*(.rodata*)
    *ui_font_*(.constdata*)
    . = ALIGN(4);
    __qspi_ui_end = .;
  } >QSPI :qspi

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH :flash

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH :flash

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH :flash

  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } >FLASH :flash

  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } >FLASH :flash

  .fini_array :
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } >FLASH :flash

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    *(.RamFunc)
    *(.RamFunc*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT> FLASH :data

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM :data

  ._user_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    PROVIDE(_end = .);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM :data

  /* Audio PCM double buffer for the SAI DMA. Placed in AXI SRAM on purpose:
   * DTCM is not reliably reachable by DMA2 and caused transfer errors. */
  .audio_buf (NOLOAD) :
  {
    . = ALIGN(32);
    KEEP(*(.audio_buf))
    . = ALIGN(32);
  } >AXIRAM :data

  .ARM.attributes 0 (INFO) :
  {
    *(.ARM.attributes)
  }
}

ASSERT(ADDR(.isr_vector) == ORIGIN(FLASH), "vector table must start at flash origin")
ASSERT(SIZEOF(.isr_vector) > 0, "vector table is empty")
ASSERT(SIZEOF(.qspi_ui) <= LENGTH(QSPI), "ui data exceeds 8MB QSPI flash")
