
#include <Arduino.h>
#include "mbed.h"
#include "USBAudio.h"
//#include <pico/stdlib.h>
#include "regs.h"

#include "hardware/pio.h"
#include "picoasm.h"
#include "pico/multicore.h"




#define PCBUFFSIZE 96

uint32_t sample_r = 0;

#define PIN_DAC        10
#define PIN_DAC2       14

PIO gPio;
uint gPio_SM;
uint gPio_SM2;


int data_cnt = 0;
int data_offset = 28;

volatile int16_t dac_data_r = 0;
volatile int16_t dac_data_l = 0;

volatile  int16_t l_out, r_out;
volatile  int16_t prev_l_out, prev_r_out;

volatile int setup_flag = 0;


USBAudio *audio;


volatile  int16_t usbBuffer_1[96 * 2];
volatile  int16_t usbBuffer_2[96 * 2];
volatile  int16_t *pcBuffer16;
volatile  int16_t *outputBuffer;
volatile  int     pcBufferNo = 1;
volatile  uint16_t pcCounter = 0;
volatile  uint16_t nBytes = 0;

int16_t  srcBuffer_l[812];
int16_t  srcBuffer_r[812];
int readCntStep29;
int writeCntStep28;


void init_timer(void);
void buffWrite(int16_t , int16_t );
void core1_worker(void);










void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  gPio = pio1;
  //アセンブラのPIOプログラムへのアドレス
  uint offset = pio_add_program(gPio, &YMF262Input_program);

  // 空いているステートマシンの番号を取得
  gPio_SM  = pio_claim_unused_sm(gPio, true);
  gPio_SM2 = pio_claim_unused_sm(gPio, true);

  pio_sm_restart(gPio, gPio_SM);
  pio_sm_restart(gPio, gPio_SM2);


  // PIOプログラムを初期化
  YMF262Input_init(gPio, gPio_SM, offset, PIN_DAC);
  YMF262Input_init(gPio, gPio_SM2, offset, PIN_DAC2);


  init_timer();  //Timer 停止

  setup_flag = 1;


  audio = new USBAudio(true, 48000, 2, 48000, 2);



  pcBuffer16 =  usbBuffer_1;
  pcBufferNo = 1;
  prev_l_out = 0;
  prev_r_out = 0;

  readCntStep29 = -58;
  writeCntStep28 = 0;
}

void loop() {
  // put your main code here, to run repeatedly:



  int outputBufferNo = 1;
  multicore_launch_core1(core1_worker);


  while (1) {

    if (outputBufferNo != pcBufferNo) {

      audio->write((uint8_t * )outputBuffer, PCBUFFSIZE);
      outputBufferNo = pcBufferNo;

    }
  }
}




void setup1() {

  init_timer();
  while (setup_flag == 0);
}


void core1_worker() {

  init_timer();


  while (1) {

    if (pio_sm_is_rx_fifo_empty(gPio, gPio_SM) != true) {
      sample_r = pio_sm_get(gPio, gPio_SM);
      dac_data_r = sample_r & 0x0000ffff;
    }

    if (pio_sm_is_rx_fifo_empty(gPio, gPio_SM2) != true) {
      sample_r = pio_sm_get(gPio, gPio_SM2);
      dac_data_l =  sample_r & 0x0000ffff;
    }



    if (dac_data_r != 0 ) {

      r_out = dac_data_r;
      dac_data_r = 0;

      l_out = dac_data_l;


      //      r_out -= 0x8000;
      //      l_out -= 0x8000;

      /* キャプチャデータは13bitの２の補数? */
#define SHIFT 3

      r_out <<=  SHIFT;
      l_out <<=  SHIFT;
      buffWrite(l_out, r_out);
    }


  }
}



void write_reg(uint32_t addr, uint32_t value) {
  *((volatile uint32_t *)addr) = value;
}
void write_reg_op(uint32_t addr, uint32_t value, uint32_t op) {
  write_reg(addr | op, value);
}
uint32_t read_reg(uint32_t addr) {
  return (*((volatile uint32_t *)addr));
}

void init_timer() {

  /* reset TIMER */
  write_reg_op(RESETS_RESET, (1 << 21), OP_CLR);
  /* wait for reset TIMER to be done */
  while (!read_reg(RESETS_RESET_DONE) & (1 << 21)) ;


  /* reset TIMER */
  write_reg_op(RESETS_RESET, (1 << 21), OP_CLR);
  /* wait for reset TIMER to be done */
  while (!read_reg(RESETS_RESET_DONE) & (1 << 21)) ;

  //        /* Enable timer interrupt */
  write_reg_op(TIMER_INTE, 0, OP_SET);
  irq_set_enabled(0, false); /* TIMER_IRQ_0=0 */
  irq_set_enabled(0, false); /* TIMER_IRQ_0=0 */
  irq_set_enabled(0, false); /* TIMER_IRQ_0=0 */
  irq_set_enabled(0, false); /* TIMER_IRQ_0=0 */

}

//int data_cnt = 0;
//int data_offset = 28;

void buffWrite(int16_t left, int16_t right) {


  int d_l = (left - prev_l_out) >> 5;
  int d_r = (right - prev_r_out) >> 5;


 left   = left - d_l * data_offset;
 right   = right - d_r * data_offset

  prev_l_out = left;
  prev_r_out = right;

  data_osset--;

  if(data_offset == 0){
    data_offset =28;
  }






    pcBuffer16[pcCounter] = left;
    pcCounter++;
    nBytes += 2;

    pcBuffer16[pcCounter] = right;
    pcCounter++;
    nBytes += 2;



    if (nBytes > PCBUFFSIZE - 1) {
      pcCounter = 0;
      nBytes = 0;
      outputBuffer = pcBuffer16;

      if (pcBufferNo == 1) {

        pcBuffer16 = usbBuffer_2;
        pcBufferNo = 2;
      } else {

        pcBuffer16 = usbBuffer_1;
        pcBufferNo = 1;
      }
    }

  }

}
