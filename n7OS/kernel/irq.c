#include <inttypes.h>
#include <n7OS/irq.h>

// fonction qui initialise la ligne irq_num avec addr
void init_irq_entry(int irq_num, uint32_t addr)
{
    idt_entry_t *entry;
    entry = (idt_entry_t *)&(idt[irq_num]);
    entry->offset_inf = addr;
    // KERNEL_CS est dans n7OS/segment.h
    entry->sel_segment = KERNEL_CS;
    entry->zero = 0;
    // 1 00 0 E(14) -> 8E
    entry->type_attr = 0x8E;
    // addr décalée de 16 bits, ou bien ((addr>>16)&0xFFFF), ou bien addr/(2^16)
    entry->offset_sup = addr >> 16;
    // P=1, DPL=00, S=0, Type=1111
}
