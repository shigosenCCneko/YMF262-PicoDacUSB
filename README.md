# YMF262-PicoDacUSB

YMF262を専用DACであるYAC512を使わずにRasberry pi picoを使いUSB-AUDIOへ出力して鳴らす。

## note
PicoのPIOを使い２つのYMF262出力を合成してステレオとして出力
YMF262とUSB-AUDIOのサンプリングレートの変換は  
49.7159:48を29:28として変換  
適当なUSB-AUDIOのライブラリが無かったのでMbed用のライブラリを使用
## 接続
#define PIN_DAC 10      //channel R  
#define PIN_DAC2 14     //channel L

順番は  
DATA  
SMT1  
SMT2  
CLK  



## Arudionのボード設定
USB-Audioのライブラリを使う為  
Arduino Mbed OS RP2040 board ->Rasberry Pi Pico　を使用

## レベルシフタ必須
YMF262は5V出力,Picoは5Vトレラントではないので
レベルシフタが必要です。


This project replaces the YAC512 DAC for the Yamaha YMF262 (OPL3) 
using Raspberry Pi Pico PIO and outputs USB Audio directly. 
It is designed for people who cannot obtain the YAC512 anymore.


### YMF262 Digital Output Timing (SMT1 / SMT2 / CLK / DATA)
OPL3 (YMF262) outputs 16‑bit Left and 16‑bit Right frames using a bit‑clock (CLK) and two strobe signals:
- SMT1 = Left channel start
- SMT2 = Right channel start
- CLK = Bit clock (data valid on rising edge)
- DATA = Serial output (MSB first



YMF262 Digital Output Timing
============================

1 frame = 16bit Left + 16bit Right

SMT1:  ____|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|______________________________
SMT2:  ____________________________|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|______

CLK :  _|‾|_|‾|_|‾|_|‾|_ ... (16 pulses) ... _|‾|_|‾|_

DATA:   b15 b14 b13 ... b1 b0      b15 b14 ... b1 b0
        <---- 15bit loop ---->     <---- 15bit loop ---->
        (PIO: set x,14 / jmp x--)  (PIO: set x,14 / jmp x--)

PIO behavior:
  1. Wait for SMT2 (Right frame start)
  2. Read 15 bits in a loop (wait CLK low → in pins → wait CLK high)
  3. Read final bit separately
  4. Wait for SMT1 (Left frame start)
  5. Same 16‑bit read sequence
  6. autopush = 32bit (Left + Right)



