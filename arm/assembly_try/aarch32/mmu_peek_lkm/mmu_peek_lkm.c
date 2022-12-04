/*
 * mmu_peek_lkm
 * Meant to be run on the Raspberry Pi.
 * 
 * Dump some ARM CP15 (MMU/TLB control) registers.
 * ARM Inline assembler help: http://www.ethernut.de/en/documents/arm-inline-asm.html
 * 
 * See ARM1176JZF-S TRM pg 3-2 onwards for ARM system control coprocessor (CP15) 
 * control/config registers description & usage.
 * 
 * General syntax of MRC : Move to ARM Register from Coprocessor [read] :
 *  MRC    <copro>, <op1>, Rd, Cn, Cm, <op2>
 * 
 *  <copro> is usually CP15 (written as 'p15')
 *  Rd = dest register (%0 => r0)
 * 
 * (c) Kaiwan NB, kaiwanTECH
 */
#include <linux/init.h>
#include <linux/module.h>

#define MODNAME  "mmu_peek_lkm"
MODULE_LICENSE("Dual BSD/GPL");

static inline u32 getreg_cp15_c0(void)
{
  u32 rv;
  /* read Main ID register, CP15:c0 [see ARM1176jzf-s TRM, pg 3-20] reg into r0 */
  //    MRC <copro>, <op1>, Rd, Cn, Cm, <op2>
  asm volatile("mrc p15, 0, %0, c0, c0, 0" : "=r" (rv));
  return rv;
}

static inline u32 getreg_cp15_c1(void)
{
  u32 rv;
/* read MMU CP15:c1 control register [see ASDG, Sloss, pg 514; TRM pg 3-44] reg into r0 */
  //    MRC <copro>, <op1>, Rd, Cn, Cm, <op2>
  asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r" (rv));
  return rv;
}

static inline u32 get_ttbr0(void)
{
  u32 rv;
/* read TTRB0 register, CP15:c2 [see ARM1176jzf-s TRM, pg 3-58] reg into r0 */
  //    MRC <copro>, <op1>, Rd, CRn, CRm, <op2>
  asm volatile("mrc p15, 0, %0, c2, c0, 0" : "=r" (rv));
  return rv;
}


static int __init mmu_peek_init(void)
{
    volatile u32 reg=0x0;

    reg = getreg_cp15_c0();
    pr_info("%s: mmu_peek: Main ID register CP15:c0=0x%08x\n",
                MODNAME, reg);
    reg = getreg_cp15_c1();
    pr_info("%s: mmu_peek: MMU control register CP15:c1=0x%08x\n",
                MODNAME, reg);
    reg = get_ttbr0();
    pr_info("%s: mmu_peek: TTBR0=0x%08x\n",
                MODNAME, reg);

    return 0; // success
}

static void __exit mmu_peek_exit(void)
{
    pr_info("%s: Done.\n", MODNAME);
}

module_init(mmu_peek_init);
module_exit(mmu_peek_exit);
