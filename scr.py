import serial
import numpy as np
import keyboard

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3'
print(ser);
ser.open();

keys = ['h', 'j', 'u', 'i', 'w', 's', 'a', 'd']
bits = [1, 2, 4, 8, 16, 32, 64, 128]
key_pressed = [False, False, False, False, False, False, False, False]
counter = 0;
while True:
    x = ser.read(1);
    print(counter, end=": ");
    for i in range(0, 8):
        val = int.from_bytes(x, "big", signed=False) & bits[i] != 0;
        print(str(i) + "+" + str(val), end=" ");
        if(val and (not key_pressed[i])):
            keyboard.press(keys[i]);
            key_pressed[i] = True;
        elif((not val) and key_pressed[i]):            
            keyboard.release(keys[i]);
            key_pressed[i] = False;
            

    print("")
    counter += 1;
    