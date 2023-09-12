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
