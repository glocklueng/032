@ECHO OFF
"D:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "D:\032\032\LayerOne_2011\avrsyn\labels.tmp" -fI -W+ie -C V2E -o "D:\032\032\LayerOne_2011\avrsyn\avrsyn.hex" -d "D:\032\032\LayerOne_2011\avrsyn\avrsyn.obj" -e "D:\032\032\LayerOne_2011\avrsyn\avrsyn.eep" -m "D:\032\032\LayerOne_2011\avrsyn\avrsyn.map" "D:\032\032\LayerOne_2011\avrsyn\avrsyn.asm"
