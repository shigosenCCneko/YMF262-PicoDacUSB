# YMF262-PicoDacUSB

専用DACであるYAC512を使わずにRasberry pi picoを使いUSB-AUDIOへ出力してYMF262を鳴らす。

## 接続
#define PIN_DAC 10  
#define PIN_DAC2 14  

２ｃｈの入力ピンを指定、順番は  
DATA  
SMT1  
SMT2  
CLK  



## Arudionのボード設定
USB-Audioのライブラリを使う為  
Arduino Mbed OS RP2040 board ->Rasberry Pi Pico　を使用
