import sensor, image, time, search, crossflag,output,chuan
from pid import PID
from image import SEARCH_EX, SEARCH_DS
import led

#sensor.reset()
#sensor.set_contrast(1)
#sensor.set_gainceiling(16)
#sensor.set_framesize(sensor.QQVGA)
#sensor.set_pixformat(sensor.GRAYSCALE)

sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 1000)


THRESHOLD = (13,100,-18,30,-2,23)

templates = search.tupian()   # 加载模板图

clock = time.clock()    # 初始化时钟

subject=search.SEARCH0()  # 初始化搜索 （第一次搜索，搜不到不进循环）

flag=0
dire=0
out=70   #初始速度
Sig=1     #初始信号

crossing = 0  #第几个十字路口
step = 0  #0寻找十字路口 1 转向 2 转向后直行

while(True):
    dire = 0
    if step == 0:
        if crossflag.CROSSFLAG1():   # 是否是交叉口
            time.sleep_ms(200)
            out = 0
            Sig = 1
            chuan.chuan_output(Sig, out,dire)
            led.led_red.on()   # 红灯亮
            led.led_green.on()   # 绿灯亮
            led.led_blue.on()   # 蓝灯亮

            crossing += 1

            time.sleep_ms(700)
            print('crossing = ', crossing)

            if crossing == 1:
                if subject == 0:
                    dire = 1
                    step = 1
                    chuan.chuan_output(Sig, out, dire)
                    time.sleep_ms(700)
                elif subject == 1:
                    dire = 2
                    step = 1
                    chuan.chuan_output(Sig, out, dire)
                    time.sleep_ms(700)
            elif crossing > 1:
                while(True):
                    led.led_red.off()   # 红灯亮
                    led.led_green.off()   # 绿灯亮
                    led.led_blue.off()   # 蓝灯亮
                    out=0
                    Sig=1
                    chuan.chuan_output(Sig, out, dire)
                    dire=search.SEARCHX(subject,templates)   # 搜索是否有模板图
                    if dire != 0 :     # 如果不是直走则如跳出循环
                        led.led_blue.on()   # 蓝灯亮
                        chuan.chuan_output(Sig, out, dire)
                        time.sleep_ms(700)
                        print('转向dire = ', dire)
                        step = 1
                        break



        if dire==0:     # 如果是直走(没有检测到模板图)    亮红灯
            led.led_green.off()   # 绿灯灭
            chuan.chuan_dir(dire)   # 传输方向
            #while(crossflag.CROSSFLAG2()):   # 如果还是交叉口
                #chuan.chuan_output(3,0)  # 向前走一些
            out=output.output()
            chuan.chuan_output(Sig, out, dire)
        else :                                # 如果不是直走（说明检测到了模版图）  亮绿灯
            led.led_red.off()                 # 红灯灭
            while(crossflag.CROSSFLAG2()):    # 如果还是交叉口，继续加速转
                chuan.chuan_output(3,0)
            #out=100*(dire)
            chuan.chuan_output(out)
            flag=1
            chuan.chuan_dir(dire,flag)  # 传输方向
            #time.sleep(0.5)
            flag=0
    if step == 1:
       out = 0
       Sig = 1
       dire = 0
       chuan.chuan_output(Sig, out, dire)
       step = 0
    if dire == 0:
        out=output.output()
        step = 0
    chuan.chuan_output(Sig, out, dire)
