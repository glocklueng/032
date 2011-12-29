avrdude -Cavrdude.conf -v -patmega2560 -cstk500v2 -P\\.\COM%2 -b115200 -D -Uflash:w:%1:i 
