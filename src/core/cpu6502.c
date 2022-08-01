#include <stdlib.h>

#include "util/log.h"

#include "core/bus.h"
#include "core/cpu6502.h"

struct OpCodeLUT
{
  char *mnemonic;
  uint8_t cycles;
  uint8_t (*addrMode)(struct CPU6502 *cpu);
  uint8_t (*instruction)(struct CPU6502 *cpu);
};

/* Adressing mode */
static uint8_t CPU6502_imp(struct CPU6502 *cpu); /* Implied */
static uint8_t CPU6502_imm(struct CPU6502 *cpu); /* Immediate */
static uint8_t CPU6502_zpg(struct CPU6502 *cpu); /* Zero Page */
static uint8_t CPU6502_zpx(struct CPU6502 *cpu); /* Zero Page with X Offset */
static uint8_t CPU6502_zpy(struct CPU6502 *cpu); /* Zero Page with Y Offset */
static uint8_t CPU6502_rel(struct CPU6502 *cpu); /* Relative */
static uint8_t CPU6502_abs(struct CPU6502 *cpu); /* Absolute */
static uint8_t CPU6502_abx(struct CPU6502 *cpu); /* Absolute with X Offset */
static uint8_t CPU6502_aby(struct CPU6502 *cpu); /* Absolute with Y Offset */
static uint8_t CPU6502_ind(struct CPU6502 *cpu); /* Indirect */
static uint8_t CPU6502_izx(struct CPU6502 *cpu); /* Indirect X */
static uint8_t CPU6502_izy(struct CPU6502 *cpu); /* Indirect Y */

/* Instructions */
static uint8_t CPU6502_adc(struct CPU6502 *cpu); /* Add with Carry */
static uint8_t CPU6502_and(struct CPU6502 *cpu); /* AND */
static uint8_t CPU6502_asl(struct CPU6502 *cpu); /* Arithmetic shift one CPU6502_bit left */
static uint8_t CPU6502_bcc(struct CPU6502 *cpu); /* Branch on Carry clear */
static uint8_t CPU6502_bcs(struct CPU6502 *cpu); /* Branch on Carry set */
static uint8_t CPU6502_beq(struct CPU6502 *cpu); /* Branch if equal */
static uint8_t CPU6502_bit(struct CPU6502 *cpu); /* Bit test */
static uint8_t CPU6502_bmi(struct CPU6502 *cpu); /* Branch if reslut minus */
static uint8_t CPU6502_bne(struct CPU6502 *cpu); /* Branch if not equal */
static uint8_t CPU6502_bpl(struct CPU6502 *cpu); /* Branch if relult plus */
static uint8_t CPU6502_brk(struct CPU6502 *cpu); /* Break */
static uint8_t CPU6502_bvc(struct CPU6502 *cpu); /* Branch on overflow clear */
static uint8_t CPU6502_bvs(struct CPU6502 *cpu); /* Branch on overflow set */
static uint8_t CPU6502_clc(struct CPU6502 *cpu); /* Clear carry flag */
static uint8_t CPU6502_cld(struct CPU6502 *cpu); /* Clear CPU6502_decimal mode */
static uint8_t CPU6502_cli(struct CPU6502 *cpu); /* Clear interrupt disable flag */
static uint8_t CPU6502_clv(struct CPU6502 *cpu); /* Clear overflow flag */
static uint8_t CPU6502_cmp(struct CPU6502 *cpu); /* Compare memory CPU6502_and accumulator */
static uint8_t CPU6502_cpx(struct CPU6502 *cpu); /* Compare memory CPU6502_and X register */
static uint8_t CPU6502_cpy(struct CPU6502 *cpu); /* Compare memory CPU6502_and Y register */
static uint8_t CPU6502_dec(struct CPU6502 *cpu); /* Decrement memory or accumulator by one */
static uint8_t CPU6502_dex(struct CPU6502 *cpu); /* Decrement X by one */
static uint8_t CPU6502_dey(struct CPU6502 *cpu); /* Decrement Y by one */
static uint8_t CPU6502_eor(struct CPU6502 *cpu); /* Exclusice or memory or accumulator by one */
static uint8_t CPU6502_inc(struct CPU6502 *cpu); /* Increment memory or accumulator by one */
static uint8_t CPU6502_inx(struct CPU6502 *cpu); /* Increment X register by one */
static uint8_t CPU6502_iny(struct CPU6502 *cpu); /* Increment Y register by one */
static uint8_t CPU6502_jmp(struct CPU6502 *cpu); /* Jump to new location */
static uint8_t CPU6502_jsr(struct CPU6502 *cpu); /* Jump to new location saving return */
static uint8_t CPU6502_lda(struct CPU6502 *cpu); /* Load accumulator with memory */
static uint8_t CPU6502_ldx(struct CPU6502 *cpu); /* Load X register with memory */
static uint8_t CPU6502_ldy(struct CPU6502 *cpu); /* Load Y register with memory */
static uint8_t CPU6502_lsr(struct CPU6502 *cpu); /* Logical shift one CPU6502_bit right memory or accumulator */
static uint8_t CPU6502_nop(struct CPU6502 *cpu); /* No operation */
static uint8_t CPU6502_ora(struct CPU6502 *cpu); /* Or memory with accumulator */
static uint8_t CPU6502_pha(struct CPU6502 *cpu); /* Push accumulator on stack */
static uint8_t CPU6502_php(struct CPU6502 *cpu); /* Push processor status on stack */
static uint8_t CPU6502_pla(struct CPU6502 *cpu); /* Pull accumulator from stack */
static uint8_t CPU6502_plp(struct CPU6502 *cpu); /* Pull processor status from stack */
static uint8_t CPU6502_rol(struct CPU6502 *cpu); /* Rotate one CPU6502_bit left memory or accumulator */
static uint8_t CPU6502_ror(struct CPU6502 *cpu); /* Rotate one CPU6502_bit right memory or accumulator */
static uint8_t CPU6502_rti(struct CPU6502 *cpu); /* Return from interrupt */
static uint8_t CPU6502_rts(struct CPU6502 *cpu); /* Return from subroutine */
static uint8_t CPU6502_sbc(struct CPU6502 *cpu); /* Substract memory from accumulator with borrow (carry) */
static uint8_t CPU6502_sec(struct CPU6502 *cpu); /* Set carry */
static uint8_t CPU6502_sed(struct CPU6502 *cpu); /* Set CPU6502_decimal mode */
static uint8_t CPU6502_sei(struct CPU6502 *cpu); /* Set interrupt flag */
static uint8_t CPU6502_sta(struct CPU6502 *cpu); /* Store accumulator in memory */
static uint8_t CPU6502_stx(struct CPU6502 *cpu); /* Store X register in memory */
static uint8_t CPU6502_sty(struct CPU6502 *cpu); /* Store Y register in memory */
static uint8_t CPU6502_tax(struct CPU6502 *cpu); /* Transfer the accumulator to the X register */
static uint8_t CPU6502_tay(struct CPU6502 *cpu); /* Transfer the accumulator to the Y register */
static uint8_t CPU6502_tsx(struct CPU6502 *cpu); /* Transfer the stack pointer to the Y register */
static uint8_t CPU6502_txa(struct CPU6502 *cpu); /* Transfer the X register the accumulator */
static uint8_t CPU6502_txs(struct CPU6502 *cpu); /* Transfer the X register the stack pointer */
static uint8_t CPU6502_tya(struct CPU6502 *cpu); /* Transfer the Y register the accumulator */

static uint8_t CPU6502_xxx(struct CPU6502 *cpu); /* Illegal OpCode */

static struct OpCodeLUT opcodes [] = {
/* 0x0x */
  { "BRK", 7, CPU6502_imp, CPU6502_brk },
  { "ORA", 6, CPU6502_izx, CPU6502_ora },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ORA", 3, CPU6502_zpg, CPU6502_ora },
  { "ASL", 5, CPU6502_zpg, CPU6502_asl },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "PHP", 3, CPU6502_imp, CPU6502_php },
  { "ORA", 2, CPU6502_imm, CPU6502_ora },
  { "ASL", 2, CPU6502_imp, CPU6502_asl },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ORA", 4, CPU6502_abs, CPU6502_ora },
  { "ASL", 6, CPU6502_abs, CPU6502_asl },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x1x */
  { "BPL", 2, CPU6502_rel, CPU6502_bpl },
  { "ORA", 5, CPU6502_izy, CPU6502_ora },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ORA", 4, CPU6502_zpx, CPU6502_ora },
  { "ASL", 6, CPU6502_zpx, CPU6502_asl },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CLC", 2, CPU6502_imp, CPU6502_clc },
  { "ORA", 4, CPU6502_aby, CPU6502_ora },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ORA", 4, CPU6502_abx, CPU6502_ora },
  { "ASL", 7, CPU6502_abx, CPU6502_asl },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x2x */
  { "JSR", 6, CPU6502_abs, CPU6502_jsr },
  { "AND", 6, CPU6502_izx, CPU6502_and },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "",    0, CPU6502_imp, CPU6502_xxx },
  { "BIT", 2, CPU6502_zpg, CPU6502_bit },
  { "AND", 3, CPU6502_zpg, CPU6502_and },
  { "ROL", 5, CPU6502_zpg, CPU6502_rol },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "PLP", 4, CPU6502_imp, CPU6502_plp },
  { "AND", 2, CPU6502_imm, CPU6502_and },
  { "ROL", 2, CPU6502_imp, CPU6502_rol },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "BIT", 4, CPU6502_abs, CPU6502_bit },
  { "AND", 4, CPU6502_abs, CPU6502_and },
  { "ROL", 6, CPU6502_abs, CPU6502_rol },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x3x */
  { "BMI", 2, CPU6502_rel, CPU6502_bmi },
  { "AND", 5, CPU6502_izy, CPU6502_and },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "AND", 4, CPU6502_zpx, CPU6502_and },
  { "ROL", 6, CPU6502_zpx, CPU6502_rol },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "SEC", 2, CPU6502_imp, CPU6502_sec },
  { "AND", 4, CPU6502_aby, CPU6502_and },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "AND", 4, CPU6502_abx, CPU6502_and },
  { "ROL", 7, CPU6502_abx, CPU6502_rol },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x4x */
  { "RTI", 6, CPU6502_imp, CPU6502_rti },
  { "EOR", 6, CPU6502_izx, CPU6502_eor },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "EOR", 3, CPU6502_zpg, CPU6502_eor },
  { "LSR", 5, CPU6502_zpg, CPU6502_lsr },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "PHA", 3, CPU6502_imp, CPU6502_pha },
  { "EOR", 2, CPU6502_imm, CPU6502_eor },
  { "LSR", 2, CPU6502_imp, CPU6502_lsr },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "JMP", 3, CPU6502_abs, CPU6502_jmp },
  { "EOR", 4, CPU6502_abs, CPU6502_eor },
  { "LSR", 6, CPU6502_abs, CPU6502_lsr },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x5x */
  { "BVC", 2, CPU6502_rel, CPU6502_bvc },
  { "EOR", 5, CPU6502_izy, CPU6502_eor },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "EOR", 4, CPU6502_zpx, CPU6502_eor },
  { "LSR", 6, CPU6502_zpx, CPU6502_lsr },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CLI", 2, CPU6502_imp, CPU6502_cli },
  { "EOR", 4, CPU6502_aby, CPU6502_eor },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "EOR", 4, CPU6502_abx, CPU6502_eor },
  { "LSR", 7, CPU6502_abx, CPU6502_lsr },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x6x */
  { "RTS", 6, CPU6502_imp, CPU6502_rts },
  { "ADC", 6, CPU6502_izx, CPU6502_adc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ADC", 3, CPU6502_zpg, CPU6502_adc },
  { "ROR", 5, CPU6502_zpg, CPU6502_ror },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "PLA", 4, CPU6502_imp, CPU6502_pla },
  { "ADC", 2, CPU6502_imm, CPU6502_adc },
  { "ROR", 2, CPU6502_imp, CPU6502_ror },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "JMP", 5, CPU6502_ind, CPU6502_jmp },
  { "ADC", 4, CPU6502_abs, CPU6502_adc },
  { "ROR", 6, CPU6502_abs, CPU6502_ror },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x7x */
  { "BVS", 2, CPU6502_rel, CPU6502_bvs },
  { "ADC", 5, CPU6502_izy, CPU6502_adc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ADC", 4, CPU6502_zpx, CPU6502_adc },
  { "ROR", 6, CPU6502_zpx, CPU6502_ror },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "SEI", 2, CPU6502_imp, CPU6502_sei },
  { "ADC", 4, CPU6502_aby, CPU6502_adc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "ADC", 4, CPU6502_abx, CPU6502_adc },
  { "ROR", 7, CPU6502_abx, CPU6502_ror },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x8x */
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "STA", 6, CPU6502_izx, CPU6502_sta },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "STY", 3, CPU6502_zpg, CPU6502_sty },
  { "STA", 3, CPU6502_zpg, CPU6502_sta },
  { "STX", 3, CPU6502_zpg, CPU6502_stx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "DEY", 2, CPU6502_imp, CPU6502_dey },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "TXA", 2, CPU6502_imp, CPU6502_txa },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "STY", 4, CPU6502_abs, CPU6502_sty },
  { "STA", 4, CPU6502_abs, CPU6502_sta },
  { "STX", 4, CPU6502_abs, CPU6502_stx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0x9x */
  { "BCC", 2, CPU6502_rel, CPU6502_bcc },
  { "STA", 6, CPU6502_izy, CPU6502_sta },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "STY", 4, CPU6502_zpx, CPU6502_sty },
  { "STA", 4, CPU6502_zpx, CPU6502_sta },
  { "STX", 4, CPU6502_zpy, CPU6502_stx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "TYA", 2, CPU6502_imp, CPU6502_tya },
  { "STA", 5, CPU6502_aby, CPU6502_sta },
  { "TXS", 2, CPU6502_imp, CPU6502_txs },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "STA", 5, CPU6502_abx, CPU6502_sta },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0xAx */
  { "LDY", 2, CPU6502_imm, CPU6502_ldy },
  { "LDA", 6, CPU6502_izx, CPU6502_lda },
  { "LDX", 2, CPU6502_imm, CPU6502_ldx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "LDY", 3, CPU6502_zpg, CPU6502_ldy },
  { "LDA", 3, CPU6502_zpg, CPU6502_lda },
  { "LDX", 3, CPU6502_zpg, CPU6502_ldx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "TAY", 2, CPU6502_imp, CPU6502_tay },
  { "LDA", 2, CPU6502_imm, CPU6502_lda },
  { "TAX", 2, CPU6502_imp, CPU6502_tax },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "LDY", 4, CPU6502_abs, CPU6502_ldy },
  { "LDA", 4, CPU6502_abs, CPU6502_lda },
  { "LDX", 4, CPU6502_abs, CPU6502_ldx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0xBx */
  { "BCS", 2, CPU6502_rel, CPU6502_bcs },
  { "LDA", 5, CPU6502_izy, CPU6502_lda },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "LDY", 4, CPU6502_zpx, CPU6502_ldy },
  { "LDA", 4, CPU6502_zpx, CPU6502_lda },
  { "LDX", 4, CPU6502_zpy, CPU6502_ldx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CLV", 2, CPU6502_imp, CPU6502_clv },
  { "LDA", 4, CPU6502_aby, CPU6502_lda },
  { "TSX", 2, CPU6502_imp, CPU6502_tsx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "LDY", 4, CPU6502_abx, CPU6502_ldy },
  { "LDA", 4, CPU6502_abx, CPU6502_lda },
  { "LDX", 4, CPU6502_aby, CPU6502_ldx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0xCx */
  { "CPY", 2, CPU6502_imm, CPU6502_cpy },
  { "CMP", 6, CPU6502_izx, CPU6502_cmp },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CPY", 3, CPU6502_zpg, CPU6502_cpy },
  { "CMP", 3, CPU6502_zpg, CPU6502_cmp },
  { "DEC", 5, CPU6502_zpg, CPU6502_dec },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "INY", 2, CPU6502_imp, CPU6502_iny },
  { "CMP", 2, CPU6502_imm, CPU6502_cmp },
  { "DEX", 2, CPU6502_imp, CPU6502_dex },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CPY", 4, CPU6502_abs, CPU6502_cpy },
  { "CMP", 4, CPU6502_abs, CPU6502_cmp },
  { "DEC", 6, CPU6502_abs, CPU6502_dec },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0xDx */
  { "BNE", 2, CPU6502_rel, CPU6502_bne },
  { "CMP", 5, CPU6502_izy, CPU6502_cmp },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CMP", 4, CPU6502_zpx, CPU6502_cmp },
  { "DEC", 6, CPU6502_zpx, CPU6502_dec },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CLD", 2, CPU6502_imp, CPU6502_cld },
  { "CMP", 4, CPU6502_aby, CPU6502_cmp },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CMP", 4, CPU6502_abx, CPU6502_cmp },
  { "DEC", 7, CPU6502_abx, CPU6502_dec },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0xEx */
  { "CPX", 2, CPU6502_imm, CPU6502_cpx },
  { "SBC", 6, CPU6502_izx, CPU6502_sbc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CPX", 3, CPU6502_zpg, CPU6502_cpx },
  { "SBC", 3, CPU6502_zpg, CPU6502_sbc },
  { "INC", 5, CPU6502_zpg, CPU6502_inc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "INX", 2, CPU6502_imp, CPU6502_inx },
  { "SBC", 2, CPU6502_imm, CPU6502_sbc },
  { "NOP", 2, CPU6502_imp, CPU6502_nop },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "CPX", 4, CPU6502_abs, CPU6502_cpx },
  { "SBC", 4, CPU6502_abs, CPU6502_sbc },
  { "INC", 6, CPU6502_abs, CPU6502_inc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
/* 0xFx */
  { "BEQ", 2, CPU6502_rel, CPU6502_beq },
  { "SBC", 5, CPU6502_izy, CPU6502_sbc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "SBC", 4, CPU6502_zpx, CPU6502_sbc },
  { "INC", 6, CPU6502_zpx, CPU6502_inc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "SED", 2, CPU6502_imp, CPU6502_sed },
  { "SBC", 4, CPU6502_aby, CPU6502_sbc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
  { "SBC", 4, CPU6502_abx, CPU6502_sbc },
  { "INC", 7, CPU6502_abx, CPU6502_inc },
  { ""   , 0, CPU6502_imp, CPU6502_xxx },
};

static uint8_t CPU6502_fetch(struct CPU6502 *cpu);

/*----------------------------------------------------------------------------*/
struct CPU6502* CPU6502_create(struct Bus* bus, uint8_t (*read)(struct Bus*, uint16_t), void (*write)(struct Bus*, uint16_t, uint8_t))
{
  struct CPU6502 *cpu = NULL;

  log_debug("Create CPU6502");

  cpu = malloc(sizeof(struct CPU6502));
  if(cpu == NULL)
  {
    log_error("Could not allocate memory for struct CPU5602");
    return NULL;
  }

  cpu->read = read;
  cpu->write = write;
  cpu->bus = bus;

  return cpu;
}

/*----------------------------------------------------------------------------*/
int CPU6502_destroy(struct CPU6502 **cpu)
{
  log_debug("Destroy struct CPU6502");

  if(*cpu != NULL)
  {
    free(*cpu);

    *cpu = NULL;
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
int CPU6502_reset(struct CPU6502 *cpu)
{
  log_debug("Reset 6502");

  cpu->Reg.A = 0x0;
  cpu->Reg.Y = 0x0;
  cpu->Reg.X = 0x0;
  cpu->Reg.SP = 0x0;

  cpu->Reg.PSR.CARRY=0;
  cpu->Reg.PSR.ZERO=0;
  cpu->Reg.PSR.IRQB=1;
  cpu->Reg.PSR.DECIMAL=0;
  cpu->Reg.PSR.BRK=1;
  cpu->Reg.PSR.NU=1;
  cpu->Reg.PSR.OVERFLOW=0;
  cpu->Reg.PSR.NEGATIVE=0;

  cpu->cycles = 7;
  cpu->clock_count = 0;

  cpu->opcode = 0;
  cpu->fetched = 0;
  cpu->addr_abs = 0xFFFC;
  cpu->addr_rel = 0;

  cpu->Reg.PCL = cpu->read(cpu->bus, cpu->addr_abs);
  cpu->Reg.PCH = cpu->read(cpu->bus, cpu->addr_abs+1);

  return 0;
}

/*----------------------------------------------------------------------------*/
int CPU6502_clock(struct CPU6502 *cpu)
{
  if(cpu->cycles == 0)
  {
    uint8_t extra_cycles1 = 0;
    uint8_t extra_cycles2 = 0;

    cpu->opcode = cpu->read(cpu->bus, cpu->Reg.PC);
    log_trace("OP <%s> at 0x%04x", opcodes[cpu->opcode].mnemonic, cpu->Reg.PC);

    cpu->Reg.PC++;
    cpu->cycles = opcodes[cpu->opcode].cycles;

    extra_cycles1 = opcodes[cpu->opcode].addrMode(cpu);
    extra_cycles2 = opcodes[cpu->opcode].instruction(cpu);

    cpu->cycles += (extra_cycles1 & extra_cycles2);
  }

  cpu->cycles--;
  cpu->clock_count++;

  return 0;
}

/*----------------------------------------------------------------------------*/
int CPU6502_complete(struct CPU6502 *cpu)
{
  return cpu->cycles == 0;
}

/*----------------------------------------------------------------------------*/
int CPU6502_dumpStatus(struct CPU6502 *cpu)
{
  log_dump("Processor Status\n");
  log_dump("----------------\n");
  log_dump(" A: 0x%02x  Y: 0x%02x     X: 0x%02x\n", cpu->Reg.A, cpu->Reg.Y, cpu->Reg.X);
  log_dump("SP: 0x%02x PC: 0x%04x\n", cpu->Reg.SP, cpu->Reg.PC, cpu->Reg.X);
  log_dump("Flags: N V - B D I Z C\n");
  log_dump("       %c %c   %c %c %c %c %c\n",
           (cpu->Reg.PSR.NEGATIVE == 1 ? '1': '0'),
           (cpu->Reg.PSR.OVERFLOW == 1 ? '1': '0'),
           (cpu->Reg.PSR.BRK      == 1 ? '1': '0'),
           (cpu->Reg.PSR.DECIMAL  == 1 ? '1': '0'),
           (cpu->Reg.PSR.IRQB     == 1 ? '1': '0'),
           (cpu->Reg.PSR.ZERO     == 1 ? '1': '0'),
           (cpu->Reg.PSR.CARRY    == 1 ? '1': '0'));

  return 0;
}

/*----------------------------------------------------------------------------*/
uint8_t CPU6502_fetch(struct CPU6502 *cpu)
{
  if(opcodes[cpu->opcode].addrMode != CPU6502_imp)
  {
    cpu->fetched = cpu->read(cpu->bus, cpu->addr_abs);
  }
  return cpu->fetched;
}

/* Implied */
uint8_t CPU6502_imp(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Implied");
  cpu->fetched = cpu->Reg.A;
  return 0;
}

/* Immediate */
uint8_t CPU6502_imm(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Immediate");
  cpu->addr_abs = cpu->Reg.PC++;
  return 0;
}

/* Zero Page */
uint8_t CPU6502_zpg(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Zero Page");
  return 0;
}

/* Zero Page with X Offset */
uint8_t CPU6502_zpx(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Zero Page X Offset");
  return 0;
}

/* Zero Page with Y Offset */
uint8_t CPU6502_zpy(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Zero Page Y Offset");
  return 0;
}

/* Relative */
uint8_t CPU6502_rel(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Relative");
  return 0;
}

/* Absolute */
uint8_t CPU6502_abs(struct CPU6502 *cpu)
{
  uint8_t lo;
  uint8_t hi;
  log_trace("Addr mode: Absolute");

  lo = cpu->read(cpu->bus, cpu->Reg.PC);
  cpu->Reg.PC++;
  hi = cpu->read(cpu->bus, cpu->Reg.PC);
  cpu->Reg.PC++;

  cpu->addr_abs = (hi << 8 ) | lo;

  return 0;
}

/* Absolute with X Offset */
uint8_t CPU6502_abx(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Absolute X Offset");
  return 0;
}

/* Absolute with Y Offset */
uint8_t CPU6502_aby(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Absolute Y Offset");
  return 0;
}

/* Indirect */
uint8_t CPU6502_ind(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Indirect");
  return 0;
}

/* Indirect X */
uint8_t CPU6502_izx(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Indirect X");
  return 0;
}

/* Indirect Y */
uint8_t CPU6502_izy(struct CPU6502 *cpu)
{
  log_trace("Addr mode: Indirect Y");
  return 0;
}

/* Add with Carry */
uint8_t CPU6502_adc(struct CPU6502 *cpu)
{
  uint16_t temp;
  CPU6502_fetch(cpu);

  log_debug("ADC Add <0x%02x> to A <0x%02x> (C: <0x%02x>)", cpu->fetched, cpu->Reg.A, cpu->Reg.PSR.CARRY);

  temp = cpu->Reg.A + cpu->fetched + cpu->Reg.PSR.CARRY;

  if(temp > 0xff) cpu->Reg.PSR.CARRY = 1;
  if((temp & 0x00FF) == 0) cpu->Reg.PSR.ZERO = 1;
  if(cpu->Reg.A & 0x80) cpu->Reg.PSR.NEGATIVE = 1;
  if((~(cpu->Reg.A ^ cpu->fetched) & (cpu->Reg.A ^ temp)) & 0x0080) cpu->Reg.PSR.OVERFLOW = 1;
  if(temp & 0x80) cpu->Reg.PSR.OVERFLOW = 1;

  cpu->Reg.A = 0x00FF & temp;

  return 1;
}

/* AND */
uint8_t CPU6502_and(struct CPU6502 *cpu)
{
  return 0;
}

/* Arithmetic shift one CPU6502_bit left */
uint8_t CPU6502_asl(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch on Carry clear */
uint8_t CPU6502_bcc(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch on Carry set */
uint8_t CPU6502_bcs(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch if equal */
uint8_t CPU6502_beq(struct CPU6502 *cpu)
{
  return 0;
}

/* Bit test */
uint8_t CPU6502_bit(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch if reslut minus */
uint8_t CPU6502_bmi(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch if not equal */
uint8_t CPU6502_bne(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch if relult plus */
uint8_t CPU6502_bpl(struct CPU6502 *cpu)
{
  return 0;
}

/* Break */
uint8_t CPU6502_brk(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch on overflow clear */
uint8_t CPU6502_bvc(struct CPU6502 *cpu)
{
  return 0;
}

/* Branch on overflow set */
uint8_t CPU6502_bvs(struct CPU6502 *cpu)
{
  return 0;
}

/* Clear carry flag */
uint8_t CPU6502_clc(struct CPU6502 *cpu)
{
  log_debug("CLC Clear Carry Flag");
  cpu->Reg.PSR.CARRY = 0;
  return 0;
}

/* Clear CPU6502_decimal mode */
uint8_t CPU6502_cld(struct CPU6502 *cpu)
{
  return 0;
}

/* Clear interrupt disable flag */
uint8_t CPU6502_cli(struct CPU6502 *cpu)
{
  return 0;
}

/* Clear overflow flag */
uint8_t CPU6502_clv(struct CPU6502 *cpu)
{
  return 0;
}

/* Compare memory CPU6502_and accumulator */
uint8_t CPU6502_cmp(struct CPU6502 *cpu)
{
  return 0;
}

/* Compare memory CPU6502_and X register */
uint8_t CPU6502_cpx(struct CPU6502 *cpu)
{
  return 0;
}

/* Compare memory CPU6502_and Y register */
uint8_t CPU6502_cpy(struct CPU6502 *cpu)
{
  return 0;
}

/* Decrement memory or accumulator by one */
uint8_t CPU6502_dec(struct CPU6502 *cpu)
{
  return 0;
}

/* Decrement X by one */
uint8_t CPU6502_dex(struct CPU6502 *cpu)
{
  return 0;
}

/* Decrement Y by one */
uint8_t CPU6502_dey(struct CPU6502 *cpu)
{
  return 0;
}

/* Exclusice or memory or accumulator by one */
uint8_t CPU6502_eor(struct CPU6502 *cpu)
{
  return 0;
}

/* Increment memory or accumulator by one */
uint8_t CPU6502_inc(struct CPU6502 *cpu)
{
  return 0;
}

/* Increment X register by one */
uint8_t CPU6502_inx(struct CPU6502 *cpu)
{
  return 0;
}

/* Increment Y register by one */
uint8_t CPU6502_iny(struct CPU6502 *cpu)
{
  return 0;
}

/* Jump to new location */
uint8_t CPU6502_jmp(struct CPU6502 *cpu)
{
  log_debug("JMP to address <0x%04x>", cpu->addr_abs);
  cpu->Reg.PC = cpu->addr_abs;
  return 0;
}

/* Jump to new location saving return */
uint8_t CPU6502_jsr(struct CPU6502 *cpu)
{
  return 0;
}

/* Load accumulator with memory */
uint8_t CPU6502_lda(struct CPU6502 *cpu)
{
  CPU6502_fetch(cpu);
  cpu->Reg.A = cpu->fetched;

  if(cpu->Reg.A == 0x00) cpu->Reg.PSR.ZERO = 1;
  if(cpu->Reg.A & 0x80) cpu->Reg.PSR.NEGATIVE = 1;
  
  return 0;
}

/* Load X register with memory */
uint8_t CPU6502_ldx(struct CPU6502 *cpu)
{
  return 0;
}

/* Load Y register with memory */
uint8_t CPU6502_ldy(struct CPU6502 *cpu)
{
  return 0;
}

/* Logical shift one CPU6502_bit right memory or accumulator */
uint8_t CPU6502_lsr(struct CPU6502 *cpu)
{
  return 0;
}

/* No operation */
uint8_t CPU6502_nop(struct CPU6502 *cpu)
{
  return 0;
}

/* Or memory with accumulator */
uint8_t CPU6502_ora(struct CPU6502 *cpu)
{
  return 0;
}

/* Push accumulator on stack */
uint8_t CPU6502_pha(struct CPU6502 *cpu)
{
  return 0;
}

/* Push processor status on stack */
uint8_t CPU6502_php(struct CPU6502 *cpu)
{
  return 0;
}

/* Pull accumulator from stack */
uint8_t CPU6502_pla(struct CPU6502 *cpu)
{
  return 0;
}

/* Pull processor status from stack */
uint8_t CPU6502_plp(struct CPU6502 *cpu)
{
  return 0;
}

/* Rotate one CPU6502_bit left memory or accumulator */
uint8_t CPU6502_rol(struct CPU6502 *cpu)
{
  return 0;
}

/* Rotate one CPU6502_bit right memory or accumulator */
uint8_t CPU6502_ror(struct CPU6502 *cpu)
{
  return 0;
}

/* Return from interrupt */
uint8_t CPU6502_rti(struct CPU6502 *cpu)
{
  return 0;
}

/* Return from subroutine */
uint8_t CPU6502_rts(struct CPU6502 *cpu)
{
  return 0;
}

/* Substract memory from accumulator with borrow (carry) */
uint8_t CPU6502_sbc(struct CPU6502 *cpu)
{
  return 0;
}

/* Set carry */
uint8_t CPU6502_sec(struct CPU6502 *cpu)
{
  return 0;
}

/* Set CPU6502_decimal mode */
uint8_t CPU6502_sed(struct CPU6502 *cpu)
{
  return 0;
}

/* Set interrupt flag */
uint8_t CPU6502_sei(struct CPU6502 *cpu)
{
  return 0;
}

/* Store accumulator in memory */
uint8_t CPU6502_sta(struct CPU6502 *cpu)
{
  log_debug("STA Store content from A <0x%02x> to addr <0x%04x>", cpu->Reg.A, cpu->addr_abs);

  cpu->write(cpu->bus, cpu->addr_abs, cpu->Reg.A);
  return 0;
}

/* Store X register in memory */
uint8_t CPU6502_stx(struct CPU6502 *cpu)
{
  return 0;
}

/* Store Y register in memory */
uint8_t CPU6502_sty(struct CPU6502 *cpu)
{
  return 0;
}

/* Transfer the accumulator to the X register */
uint8_t CPU6502_tax(struct CPU6502 *cpu)
{
  return 0;
}

/* Transfer the accumulator to the Y register */
uint8_t CPU6502_tay(struct CPU6502 *cpu)
{
  return 0;
}

/* Transfer the stack pointer to the Y register */
uint8_t CPU6502_tsx(struct CPU6502 *cpu)
{
  return 0;
}

/* Transfer the X register the accumulator */
uint8_t CPU6502_txa(struct CPU6502 *cpu)
{
  return 0;
}

/* Transfer the X register the stack pointer */
uint8_t CPU6502_txs(struct CPU6502 *cpu)
{
  return 0;
}

/* Transfer the Y register the accumulator */
uint8_t CPU6502_tya(struct CPU6502 *cpu)
{
  return 0;
}

/* Illegal OpCode */
uint8_t CPU6502_xxx(struct CPU6502 *cpu)
{
  return 0;
}
