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

rho_pid = PID(p=0.4, i=0)
theta_pid = PID(p=0.001, i=0)
THRESHOLD = (16, 38, 27, 50, 24, 42)

templates = search.tupian()   # 加载模板图

clock = time.clock()    # 初始化时钟

subject=search.SEARCH0()   # 初始化搜索 （第一次搜索，搜不到不进循环）

flag=0
dire=0
out=100   #初始速度
Sig=1     #初始信号

#以下过程正常直走，遇到路口停车查模版，无模版时直走绿灯亮，有模版时搜索红灯亮

while(True):
    out=output.output()
    chuan.chuan_dir(dire,flag)
    if crossflag.CROSSFLAG1():   # 是否是交叉口
        led.led_red.on()   # 红灯亮
        led.led_green.on()   # 绿灯亮
        led.led_blue.on()   # 蓝灯亮
        
        out=0   
        Sig=1      
        chuan.chuan_output(Sig,out)   # 传输输出
        
        for i in range(1,11):         # 亮黄灯
            led.led_blue.off()   # 蓝灯灭
            out=0   
            Sig=1      
            chuan.chuan_output(Sig,out)  
            dire=search.SEARCHX(subject,templates)   # 搜索是否有模板图
            print('转向dire = ', dire)
            Sig = 2.3 
            out = 0
            chuan.chuan_output(Sig ,out)  # 向前走一些
            if dire != 0 :     # 如果不是直走则如跳出循环
                break
        if dire==0:     # 如果是直走(没有检测到模板图)    亮红灯
            led.led_green.off()   # 绿灯灭
            chuan.chuan_dir(dire,flag)   # 传输方向
            while(crossflag.CROSSFLAG2()):   # 如果还是交叉口
                chuan.chuan_output(3,0)  # 向前走一些
            out=output.output()
            chuan.chuan_output(out)
        else :                                # 如果不是直走（说明检测到了模版图）  亮绿灯
            led.led_red.off()                 # 红灯灭
            while(crossflag.CROSSFLAG2()):    # 如果还是交叉口，继续加速转
                chuan.chuan_output(3,0)
            out=100*(dire)
            chuan.chuan_output(out)
            flag=1
            chuan.chuan_dir(dire,flag)  # 传输方向
            #time.sleep(0.5)
            flag=0
    out=output.output()
    chuan.chuan_output(Sig, out)
