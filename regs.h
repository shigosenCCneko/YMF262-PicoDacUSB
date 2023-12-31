/* SPDX-License-Identifier: MIT */
#ifndef REGS_H
#define REGS_H

#define OP_RW  (0x0000)
#define OP_XOR (0x1000)
#define OP_SET (0x2000)
#define OP_CLR (0x3000)

//#define RESETS_BASE                 (0x4000C000)

#define RESETS_RESET             (RESETS_BASE+0x0)
#define RESETS_RESET_DONE        (RESETS_BASE+0x8)

//#define SIO_BASE (0xD0000000)

#define SIO_GPIO_OUT_SET (SIO_BASE+0x14)
#define SIO_GPIO_OUT_CLR (SIO_BASE+0x18)
#define SIO_GPIO_OUT_XOR (SIO_BASE+0x1C)
#define SIO_GPIO_OE_SET (SIO_BASE+0x24)
#define SIO_GPIO_OE_CLR (SIO_BASE+0x28)

//#define PADS_BANK0_BASE (0x4001C000)
#define PADS_BANK0_GPIO (0x68)

#define PADS_GPIO25 (PADS_BANK0_BASE + PADS_BANK0_GPIO)

//#define IO_BANK0_BASE (0x40014000)
#define IO_BANK_GPIO25_CTRL (0xCC)

#define IO_GPIO25_CTRL (IO_BANK0_BASE + IO_BANK_GPIO25_CTRL)

#define SYSTICK_CSR (0xE000E010)
#define SYSTICK_RVR (0xE000E014)
#define SYSTICK_CVR (0xE000E018)
#define NVIC_ISER (0xE000E100) /* Interrupt Set Enable Register */
#define NVIC_ICER (0xE000E180) /* Interrupt Clear Enable Register */
#define NVIC_ICPR (0xE000E280) /* Interrupt Clear Pending Register */

//#define XOSC_BASE (0x40024000)
#define XOSC_CTRL (XOSC_BASE+0x00)
#define XOSC_STATUS (XOSC_BASE+0x04)
#define XOSC_STARTUP (XOSC_BASE+0x0c)

//#define CLOCKS_BASE (0x40008000)
#define CLOCKS_REF_CTRL (CLOCKS_BASE+0x30)
#define CLOCKS_SYS_CTRL (CLOCKS_BASE+0x3c)
#define CLOCKS_CLK_SYS_RESUS_CTRL (CLOCKS_BASE+0x78)

//#define TIMER_BASE (0x40054000)
#define TIMER_TIMERRAWL (TIMER_BASE+0x28)
#define TIMER_ALARM0 (TIMER_BASE+0x10)
#define TIMER_ALARM1 (TIMER_BASE+0x14)
#define TIMER_ALARM2 (TIMER_BASE+0x18)
#define TIMER_ALARM3 (TIMER_BASE+0x1C)

#define TIMER_INTR (TIMER_BASE+0x34)
#define TIMER_INTE (TIMER_BASE+0x38)


#endif //REGS_H
