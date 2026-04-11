/*
 * Simplified AHV CPU initialization
 */

#include "bindings.h"
#include "cpu_aarch64.h"

void cpu_init(void)
{
    /* For AHV, we don't set up exception vectors - the hypervisor handles everything */
    log(INFO, "Solo5 AHV: CPU initialized\n");
}

/* keeps track of cpu_intr_disable() depth */
int cpu_intr_depth = 1;

void cpu_intr_disable(void)
{
    __asm__ __volatile__("msr daifset, #2");
    cpu_intr_depth++;
}

void cpu_intr_enable(void)
{
    assert(cpu_intr_depth > 0);
    if (--cpu_intr_depth == 0)
        __asm__ __volatile__("msr daifclr, #2");
}

void cpu_halt(void)
{
    cpu_intr_disable();
    while (1) {
        __asm__ __volatile("wfi");
    }
}