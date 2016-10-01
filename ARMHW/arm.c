//
//  arm.c
//  ARMHW
//
//  Created by callum taylor on 21/10/2015.
//
//

#include "arm.h"

#define READ_WB_REG_CASE(OP2, M, VAL)			\
case ((OP2 << 4) + M):				\
ARM_DBG_READ(c0, c ## M, OP2, VAL);	\
break

#define WRITE_WB_REG_CASE(OP2, M, VAL)			\
case ((OP2 << 4) + M):				\
ARM_DBG_WRITE(c0, c ## M, OP2, VAL);	\
break

#define GEN_READ_WB_REG_CASES(OP2, VAL)		\
READ_WB_REG_CASE(OP2, 0, VAL);		\
READ_WB_REG_CASE(OP2, 1, VAL);		\
READ_WB_REG_CASE(OP2, 2, VAL);		\
READ_WB_REG_CASE(OP2, 3, VAL);		\
READ_WB_REG_CASE(OP2, 4, VAL);		\
READ_WB_REG_CASE(OP2, 5, VAL);		\
READ_WB_REG_CASE(OP2, 6, VAL);		\
READ_WB_REG_CASE(OP2, 7, VAL);		\
READ_WB_REG_CASE(OP2, 8, VAL);		\
READ_WB_REG_CASE(OP2, 9, VAL);		\
READ_WB_REG_CASE(OP2, 10, VAL);		\
READ_WB_REG_CASE(OP2, 11, VAL);		\
READ_WB_REG_CASE(OP2, 12, VAL);		\
READ_WB_REG_CASE(OP2, 13, VAL);		\
READ_WB_REG_CASE(OP2, 14, VAL);		\
READ_WB_REG_CASE(OP2, 15, VAL)

#define GEN_WRITE_WB_REG_CASES(OP2, VAL)	\
WRITE_WB_REG_CASE(OP2, 0, VAL);		\
WRITE_WB_REG_CASE(OP2, 1, VAL);		\
WRITE_WB_REG_CASE(OP2, 2, VAL);		\
WRITE_WB_REG_CASE(OP2, 3, VAL);		\
WRITE_WB_REG_CASE(OP2, 4, VAL);		\
WRITE_WB_REG_CASE(OP2, 5, VAL);		\
WRITE_WB_REG_CASE(OP2, 6, VAL);		\
WRITE_WB_REG_CASE(OP2, 7, VAL);		\
WRITE_WB_REG_CASE(OP2, 8, VAL);		\
WRITE_WB_REG_CASE(OP2, 9, VAL);		\
WRITE_WB_REG_CASE(OP2, 10, VAL);	\
WRITE_WB_REG_CASE(OP2, 11, VAL);	\
WRITE_WB_REG_CASE(OP2, 12, VAL);	\
WRITE_WB_REG_CASE(OP2, 13, VAL);	\
WRITE_WB_REG_CASE(OP2, 14, VAL);	\
WRITE_WB_REG_CASE(OP2, 15, VAL)

static uint32_t read_wb_reg(int n)
{
    uint32_t val = 0;
    
    switch (n) {
            GEN_READ_WB_REG_CASES(ARM_OP2_BVR, val);
            GEN_READ_WB_REG_CASES(ARM_OP2_BCR, val);
            GEN_READ_WB_REG_CASES(ARM_OP2_WVR, val);
            GEN_READ_WB_REG_CASES(ARM_OP2_WCR, val);
    }
    
    return val;
}

static void write_wb_reg(int n, uint32_t val)
{
    switch (n) {
            GEN_WRITE_WB_REG_CASES(ARM_OP2_BVR, val);
            GEN_WRITE_WB_REG_CASES(ARM_OP2_BCR, val);
            GEN_WRITE_WB_REG_CASES(ARM_OP2_WVR, val);
            GEN_WRITE_WB_REG_CASES(ARM_OP2_WCR, val);
    }
    asm volatile("isb" : : : "memory");
}

/* Determine number of BRP registers available. */
static int get_num_brp_resources(void)
{
    uint32_t didr;
    ARM_DBG_READ(c0, c0, 0, didr);
    return ((didr >> 24) & 0xf) + 1;
}

static int monitor_mode_enabled(void)
{
    uint32_t dscr;
    ARM_DBG_READ(c0, c1, 0, dscr);
    return !!(dscr & ARM_DSCR_MDBGEN);
}

/*
 * arm_hw_breakpoint_register: exec breakpoint
 * */
int arm_hw_breakpoint_register(unsigned long addr)
{
    uint32_t mismatch = 0;
    uint32_t len = ARM_BREAKPOINT_LEN_4;
    uint32_t type = ARM_BREAKPOINT_EXECUTE;
    uint32_t privilege = ARM_BREAKPOINT_PRIV;
    uint32_t enabled = 1;
    uint32_t ctrl = (mismatch << 22) | (len << 5) | (type << 3) |
    (privilege << 1) | enabled;
    
    /* write address register */
    write_wb_reg(ARM_BASE_BVR + 0, addr);
    /* write control register */
    write_wb_reg(ARM_BASE_BCR + 0, ctrl);
    return 0;
}

int arm_hw_breakpoint_unregister(unsigned long addr)
{
    ;
}