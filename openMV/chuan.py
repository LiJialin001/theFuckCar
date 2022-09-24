import sensor, image, time
from pyb import UART
import json



clock = time.clock() # Tracks FPS.

uart = UART(3, 115200)   #串口3，波特率115200
uart.init(115200, bits=8, parity=None, stop=1)  #8位数据位，无校验位，1位停止位

def chuan_output(X): 
    clock.tick() # Track elapsed milliseconds between snapshots().

    #  FH = bytearray([0xb3,0xb3])
    #  uart.write(FH)     #打印帧头
    X=int(X)
    data = bytearray([0xb3,0xb3,X,0x5b,0x5b])
    uart.write(data)


def chuan_dir(d,flag):   #d为方向，flag为十字路口标志
    clock.tick()
    d=int(d)
    flag=int(flag)
    data = bytearray([0xb3,0xb3,d,flag,0x4b])   
    uart.write(data)
