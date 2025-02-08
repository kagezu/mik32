// #include <mik32_memory_map.h>
// #include <power_manager.h>
// #include <pad_config.h>
// #include <timer32.h>
// #include <epic.h>
#include <scr1_csr_encoding.h>
#include <csr.h>

extern uint32_t isr_vector;

void set_isr(void handler())
{
  isr_vector = (uint32_t)handler;
}

void sei()
{
  set_csr(mstatus, MSTATUS_MIE);
  set_csr(mie, MIE_MEIE);
}

void cli()
{
  clear_csr(mie, MIE_MEIE);
}

