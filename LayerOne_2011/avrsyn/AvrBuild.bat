@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "E:\032\LayerOne_2011\avrsyn\labels.tmp" -fI -W+ie -C V2E -o "E:\032\LayerOne_2011\avrsyn\avrsyn.hex" -d "E:\032\LayerOne_2011\avrsyn\avrsyn.obj" -e "E:\032\LayerOne_2011\avrsyn\avrsyn.eep" -m "E:\032\LayerOne_2011\avrsyn\avrsyn.map" "E:\032\LayerOne_2011\avrsyn\avrsyn.asm"
