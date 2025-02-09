#include <stdint.h>
#include <xprintf.h>
#include <jl_regs.h>


void uputc(int c) {
	reg32_write(UART0_base+UARTx_BUF, c);
	while (!reg32_rsmask(UART0_base+UARTx_CON0_TPND));
	reg32_wsmask(UART0_base+UARTx_CON0_CLRTPND, 1);
}


void hexdump(void *ptr, int len) {
	for (int i = 0; i < len; i += 16) {
		xprintf("%08x: ", ptr+i);

		for (int j = 0; j < 16; j++) {
			if (i+j < len)
				xprintf("%02X ", *(uint8_t*)(ptr+i+j));
			else
				xputs("-- ");
		}

		xputs(" |");

		for (int j = 0; j < 16; j++) {
			uint8_t c = ' ';
			if (i+j < len) {
				c = *(uint8_t*)(ptr+i+j);
				if (c < 0x20 || c >= 0x7f) c = '.';
			}
			xputc(c);
		}

		xputs("|\n");
	}
}


void delay(uint32_t ms) {
	while (ms--) // TODO!
		for (volatile int i = 1000; i; i--);
}



uint32_t fmtx_buf[2][128];

void fmtx_on(void) {
	reg32_wsmask(FM_base+FM_TX_CON0, 7, 1, 1);
	reg32_wsmask(FM_base+FM_TX_CON0, 0, 1, 1);
}

void fmtx_off(void) {
	reg32_wsmask(FM_base+FM_TX_CON0, 7, 1, 0);
	reg32_wsmask(FM_base+FM_TX_CON0, 0, 1, 0);
}

void fmtx_init(void) {
	reg32_wsmask(FM_base+FM_CON, 14, 1, 0); // enable fm tx regs!

	reg32_write(FM_base+FM_TX_BASE_ADR, (uint32_t)fmtx_buf);

	// effectively fmtx_on
	reg32_wsmask(FM_base+FM_TX_CON0, 7, 1, 1);
	reg32_wsmask(FM_base+FM_TX_CON0, 0, 1, 1);

	reg32_write(FM_base+FM_TX_MUL,      59);
	reg32_write(FM_base+FM_TX_PILOT,    166);
	reg32_write(FM_base+FM_TX_SYN_GAIN, 0x1000);
	reg32_write(FM_base+FM_TX_FREQ,     0x15400000);

	//----------------------------------------------------

	reg32_write(FM_base+FM_TX_LEN,      0);

	for (int i = 160; i >= 0; i--) {
		reg32_write(FM_base+FM_TX_ADR, i);
		delay(5);
		reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
		delay(5);
	}

	//------------------

	reg32_write(FM_base+FM_TX_LEN, -13);

	reg32_write(FM_base+FM_TX_ADR, 256);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	reg32_write(FM_base+FM_TX_ADR, 296);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	//------------------

	reg32_write(FM_base+FM_TX_LEN, 13);

	reg32_write(FM_base+FM_TX_ADR, 260);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	reg32_write(FM_base+FM_TX_ADR, 292);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	//------------------

	reg32_write(FM_base+FM_TX_LEN, -39);

	reg32_write(FM_base+FM_TX_ADR, 264);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	reg32_write(FM_base+FM_TX_ADR, 288);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	//------------------

	reg32_write(FM_base+FM_TX_LEN, 24);

	reg32_write(FM_base+FM_TX_ADR, 268);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	reg32_write(FM_base+FM_TX_ADR, 284);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	//------------------

	reg32_write(FM_base+FM_TX_LEN, -451);

	reg32_write(FM_base+FM_TX_ADR, 272);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	reg32_write(FM_base+FM_TX_ADR, 280);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	//------------------

	reg32_write(FM_base+FM_TX_LEN, 1945);

	reg32_write(FM_base+FM_TX_ADR, 276);
	delay(5);
	reg32_wsmask(FM_base+FM_TX_CON0, 5, 1, 1);
	delay(5);

	//----------------------------------------------------

	delay(200);

	reg32_wsmask(FM_base+FM_TX_CON0, 0, 1, 1);
	delay(10);

	reg32_wsmask(FM_base+FM_TX_CON1, 7, 1, 1);
	delay(10);

	reg32_wsmask(FM_base+FM_TX_CON0, 1, 1, 1); // <--- stereo flag

	reg32_wsmask(FM_base+FM_TX_CON1, 0, 1, 1);
	reg32_wsmask(FM_base+FM_TX_CON1, 0, 0xf, 0x0);
	reg32_wsmask(FM_base+FM_TX_CON1, 3, 1, 1);

	reg32_write(FM_base+FM_TX_FREQ, 0x15400000);

	reg32_write(FM_base+FM_TX_LEN, 128);
	delay(500);

	reg32_wsmask(FM_base+FM_TX_CON0, 7, 1, 1);
}

void fmtx_set_freq(int freq) {

	// super calcuations

	/*reg32_wsmask(CLOCK_base+CLOCK_PLL_CON1, 31, 1, 0);

	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON,  11, 1, 1);
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON1, 22, 0x3, 0x1);
	reg32_wsmask(ANA_base+ANA_WLA_CON19, 21, 1, 1);

	reg32_wsmask(CLOCK_base+CLOCK_CLK_CON2,  2, 0xf, 0x8);

	reg32_wsmask(CLOCK_base+CLOCK_CLK_CON2,  0, 0x3, 0x3);
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON,  24, 0x7, 0x1);
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON,  27, 0x7, 0x3);
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON,  10, 1, 1);
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON1, 19, 1, 1);

	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON1, 31, 1, 1);

	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON, 0, 1, 1);
	delay(150);
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON, 1, 1, 1);*/

	reg32_write(FM_base+FM_TX_FREQ, (0x15 << 24) | 0x400000); // %18, %26
	delay(30);
}

void fmtx_analog_init(void) {
	reg32_wsmask(ANA_base+ANA_WLA_CON18,  1, 1, 1);
	reg32_wsmask(ANA_base+ANA_WLA_CON21, 11, 1, 0);
}

void fm_emitter_init(void) {
	reg32_wsmask(CLOCK_base+CLOCK_CLK_CON1, 24, 0x3, 0x1); // clk sel
	reg32_wsmask(FM_base+FM_TX_CON1, 3, 1, 1);
	fmtx_init();

	// for port=0
	reg32_wsmask(CLOCK_base+CLOCK_CLK_CON1, 12, 0x3, 0x1); // clk sel
	reg32_wsmask(PORTB_base+PORTx_DIRn(1), 0); // PB1 = out
}



void JieLi(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3) {
	// init UART0 on PB5
	reg32_write(UART0_base+UARTx_CON0, 1); // 8n1, en
	reg32_write(UART0_base+UARTx_BAUD, (48000000 / 4 / 921600) - 1);
	reg32_wsmask(IOMAP_base+IOMAP_CON0, 3, 3, 2); // UART0 to PB5
	reg32_wsmask(IOMAP_base+IOMAP_CON3, 2, 1, 0); // UART0 ... IO SEL -> IOMUX ?
	reg32_wsmask(IOMAP_base+IOMAP_CON3, 3, 1, 1); // UART0 I/O enable
	reg32_wsmask(PORTB_base+PORTx_DIRn(5), 0); // PB5 out

	xdev_out(uputc);
	xputs("\e[H\e[2J\e[3J"); // clear screen
	xputs("\e[1;37;41m==== JieLi AC6965A! "__DATE__" "__TIME__" ====\e[0m\n");
	xprintf("r0: <%08x>  r1: <%08x>  r2: <%08x>  r3: <%08x>\n", r0,r1,r2,r3);

	/*==================================================================*/

	for (int i = 0; i < 128; i++)
		fmtx_buf[0][i] = reg32_read(RAND_base+RAND_R64L);

	fm_emitter_init();
	fmtx_set_freq(9000); // 90 mhz sorta like that

	/*==================================================================*/

	#if 0
	///----------- dac init
	reg32_wsmask(CLOCK_base+CLOCK_PLL_CON1, 30, 1, 1);

	reg32_wsmask(ANA_base+ANA_DAA_CON0, 19, 0x3, 0x3);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 18, 1, 1);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 17, 1, 1);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 16, 1, 1);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 15, 1, 0);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 14, 1, 1);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 8, 1, 0);
	reg32_wsmask(ANA_base+ANA_DAA_CON0, 6, 1, 0);

	///----------- dac vdd power on
	//reg32_wsmask(ANA_base+ANA_DAA_CON2, 11, 0x7, 0x0);

	//reg32_wsmask(ANA_base+ANA_DAA_CON0, 25, 0x7, 0x0); // vdd voltage

	//reg32_wsmask(ANA_base+ANA_DAA_CON0, 22, 1, 1);
	//reg32_wsmask(ANA_base+ANA_DAA_CON0, 24, 1, 1);
	#endif

	/*==================================================================*/

	#if 0
	///----------- open linein
	reg32_wsmask(PORTA_base+PORTx_DIRn(3), 1); // PA3 in
	reg32_wsmask(PORTA_base+PORTx_DIEn(3), 0); // PA3 digital in disable
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux0l_en, 1); // AMUX0L enable

	reg32_wsmask(PORTA_base+PORTx_DIRn(4), 1); // PA4 in
	reg32_wsmask(PORTA_base+PORTx_DIEn(4), 0); // PA4 digital in disable
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux0r_en, 1); // AMUX0R enable

	reg32_wsmask(PORTB_base+PORTx_DIRn(6), 1); // PB6 in
	reg32_wsmask(PORTB_base+PORTx_DIEn(6), 0); // PB6 digital in disable
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux1l_en, 1); // AMUX1L enable

	reg32_wsmask(PORTB_base+PORTx_DIRn(7), 1); // PB7 in
	reg32_wsmask(PORTB_base+PORTx_DIEn(7), 0); // PB7 digital in disable
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux1r_en, 1); // AMUX1R enable

	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux_en, 1); // AMUX enable

	///----------- gain
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux_gain, 0); // 2x gain

	///----------- mute
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux_mute, 0); // mute

	///----------- combine
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux_lrmix, 0x0);

	///----------- linein bias
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux0l_biasen, 0); // AMUX0L bias
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux0r_biasen, 0); // AMUX0R bias
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux1l_biasen, 0); // AMUX1L bias
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux1r_biasen, 0); // AMUX1R bias

	///----------- amux bias
	reg32_wsmask(ANA_base+ANA_DAA_CON1_amux_biasen, 0); // amux bias
	#endif
}
