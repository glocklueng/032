from serial import Serial
import time
ser = Serial("com40"); #replace with your COM port number
ser.baudrate = 115200

#Extract Protection Settings
def SendFlashProtectionData(hexFile, function):
    data_array = ["S"];
    hexFile.seek(0);
    current_pos = hexFile.tell();
    elementsPerHexFileRow = 141;
    hexFileRows = 521;
    deviceFlashRows = 256;
    hexFlashDataRows = 2*deviceFlashRows;
    metaDataLocation = hexFlashDataRows * elementsPerHexFileRow;
    dataBytesPerFlashRow = 128;
    rel_pos = 0;
    hexFile.seek(metaDataLocation);
    current_pos = hexFile.tell();
    hexFile.seek(current_pos);
    for positon_index in range(0,3):
        hexFile.seek(current_pos + len(hexFile.readline()));
        current_pos = hexFile.tell();

    #Extract Protection Data
    protectionDataLoc = current_pos + 9;
    hexFile.seek(protectionDataLoc);
    current_pos = hexFile.tell();
    for rowsize in range(0,32):
        hexFile.seek(current_pos + rel_pos);
        hexfilehigher = ord(hexFile.read(1))
        hexfilelower = ord(hexFile.read(1))
        tempflashvalhigher = hexfilehigher - 0x30;
        if(tempflashvalhigher > 9):
            tempflashvalhigher -= 7;
        tempflashvallower = hexfilelower - 0x30;
        if(tempflashvallower > 9):
            tempflashvallower -= 7;
        flashvalue = (((tempflashvalhigher << 4) & 0xF0)|(tempflashvallower & 0x0F));
        data_array.append(flashvalue)
        rel_pos = rel_pos + 2;
    for i in range(0,97):
        data_array.append(0);
    data_array.append("P");
    if(function == 0):
        data_array.insert(1, "R");
    if(function == 1):
        data_array.insert(1, "T");
    ser.write(data_array);
    print data_array

def SendFlashData(hexFile, flashrow, function):
    data_array = ["S"];
    elementsPerHexFileRow = 141;
    hexFile.seek((2*(flashrow - 1)) * elementsPerHexFileRow);
    current_pos = fo.tell();
    for flashRowToggle in range(0,2):
        hexFile.seek(current_pos);
        rel_pos = 9;
        for rowsize in range(0,64):
            hexFile.seek(current_pos + rel_pos);
            hexfilehigher = ord(hexFile.read(1))
            hexfilelower = ord(hexFile.read(1))
            tempflashvalhigher = hexfilehigher - 0x30;
            if(tempflashvalhigher > 9):
                tempflashvalhigher -= 7;
            tempflashvallower = hexfilelower - 0x30;
            if(tempflashvallower > 9):
                tempflashvallower -= 7;
            flashvalue = (((tempflashvalhigher << 4) & 0xF0)|(tempflashvallower & 0x0F));
            data_array.append(flashvalue)
            rel_pos = rel_pos + 2;
        hexFile.seek(current_pos);
        current_pos = current_pos + len(fo.readline());
    data_array.append("P");
    if(function == 0):
        data_array.insert(1, "P");
    if(function == 1):
        data_array.insert(1, "V");
    data_array.insert(2, flashrow-1)
    ser.write(data_array);
    print data_array

#Send the Silicon ID
def SendSiliconID(hexFile):
    data_array = ["S"];
    hexFile.seek(0);
    current_pos = fo.tell();
    elementsPerHexFileRow = 141;
    hexFileRows = 521;
    deviceFlashRows = 256;
    hexFlashDataRows = 2*deviceFlashRows;
    metaDataLocation = hexFlashDataRows * elementsPerHexFileRow;
    dataBytesPerFlashRow = 128;
    hexFile.seek(metaDataLocation);
    current_pos = fo.tell();
    hexFile.seek(current_pos);
    for positon_index in range(0,5):
        hexFile.seek(current_pos + len(hexFile.readline()));
        current_pos = hexFile.tell();

    #Extract Silicon ID from hex file
    silicon_ID_pos = hexFile.tell() + 13;
    rel_pos = 0;
    for rowsize in range(0,4):
        hexFile.seek(silicon_ID_pos + rel_pos);
        hexfilehigher = ord(hexFile.read(1))
        hexfilelower = ord(hexFile.read(1))
        tempflashvalhigher = hexfilehigher - 0x30;
        if(tempflashvalhigher > 9):
            tempflashvalhigher -= 7;
        tempflashvallower = hexfilelower - 0x30;
        if(tempflashvallower > 9):
            tempflashvallower -= 7;
        flashvalue = (((tempflashvalhigher << 4) & 0xF0)|(tempflashvallower & 0x0F));
        data_array.append(flashvalue)
        rel_pos = rel_pos + 2;
    for i in range(0, 125):
        data_array.append(0);    
    data_array.append("P");
    data_array.insert(1, "I");
    ser.write(data_array);
    print data_array;

#Send Checksum values
def SendCheckSumData(hexFile):
    hexFile.seek(0);
    current_pos = hexFile.tell();
    elementsPerHexFileRow = 141;
    hexFileRows = 521;
    deviceFlashRows = 256;
    hexFlashDataRows = 2*deviceFlashRows;
    metaDataLocation = hexFlashDataRows * elementsPerHexFileRow;
    dataBytesPerFlashRow = 128;
    hexFile.seek(metaDataLocation);
    current_pos = fo.tell();
    hexFile.seek(current_pos + len(hexFile.readline()));
    current_pos = fo.tell();

    #Extract Checksum from hex file
    checkSumDataPos = current_pos + 9;
    rel_pos = 0;
    data_array = ["S"];
    for rowsize in range(0,4):
        hexFile.seek(checkSumDataPos + rel_pos);
        hexfilehigher = ord(hexFile.read(1))
        hexfilelower = ord(hexFile.read(1))
        tempflashvalhigher = hexfilehigher - 0x30;
        if(tempflashvalhigher > 9):
            tempflashvalhigher -= 7;
        tempflashvallower = hexfilelower - 0x30;
        if(tempflashvallower > 9):
            tempflashvallower -= 7;
        flashvalue = (((tempflashvalhigher << 4) & 0xF0)|(tempflashvallower & 0x0F));
        data_array.append(flashvalue)
        rel_pos = rel_pos + 2;
    for i in range(0, 125):
        data_array.append(0); 
    data_array.append("P");
    data_array.insert(1, "S");
    ser.write(data_array);
    print data_array;

#Acquire target
def SendAcquireCommand():
    data_array = ["S", "A"];
    for i in range(0, 129):
        data_array.append(0);
    data_array.append("P");
    ser.write(data_array);
    print data_array;

#Erase All Flash
def SendEraseAllCommand():
    data_array = ["S", "E"];
    for i in range(0, 129):
        data_array.append(0);
    data_array.append("P");
    ser.write(data_array);
    print data_array;

def SendChecksumPrivilegedCommand():
    data_array = ["S", "C"];
    for i in range(0, 129):
        data_array.append(0);
    data_array.append("P");
    ser.write(data_array);
    print data_array;

#Reset the target
def ExitProgramming():
    data_array = ["E"];
    for i in range(0, 130):
        data_array.append(0);
    data_array.append("P");
    ser.write(data_array);
    print data_array;

#main code
#provide path to your .hex file
fo = open("C:\\Users\\rnjt\\Desktop\\Analog\\blinker\\blinker_red.cydsn\\CortexM0\\ARM_GCC_473\\Debug\\blinker_red.hex", "rb");
SendAcquireCommand();
time.sleep(3);
SendSiliconID(fo);
time.sleep(3);
SendEraseAllCommand();
time.sleep(3);
SendChecksumPrivilegedCommand();
time.sleep(3);
for index in range(1, 257):
    SendFlashData(fo, index, 0);
    time.sleep(0.05);
for index in range(1, 257):
    SendFlashData(fo, index, 1);
    time.sleep(0.05);
SendFlashProtectionData(fo, 0);
time.sleep(2);
SendFlashProtectionData(fo, 1);
time.sleep(2);
SendCheckSumData(fo);
time.sleep(2);
ExitProgramming();





