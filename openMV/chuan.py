import sensor, image, time
from pyb import UART
import json



clock = time.clock() # Tracks FPS.

uart = UART(3, 115200)   #串口3，波特率115200
uart.init(115200, bits=8, parity=None, stop=1)  #8位数据位，无校验位，1位停止位

def chuan_output(Sig, X):
    clock.tick() # Track elapsed milliseconds between snapshots().
    X=int(X)
    if X > 0:
        Sig = 2
    elif X < 0:
        Sig = 0
    X=abs(X)
    data = bytearray([0xb3,0xb3,X,Sig,0x5b,0x5b])
    uart.write(data)
    if X != 0:
        print("output:",X,Sig)

def chuan_dir(d,flag):   #d为方向，flag为十字路口标志
    clock.tick()
    d=int(d)
    flag=int(flag)
    data = bytearray([0x77,0x77,d,flag,0x4b,0x4b])
    uart.write(data)
    #print("dir:",d)
    #print("flag:",flag)
    #print('dir_data:', data)


def chuan_count(C):   #数字
    clock.tick()
    C=int(C)
    data = bytearray([0x55,0x55,C,0x2b,0x2b])
    uart.write(data)
    print("count:",C)
