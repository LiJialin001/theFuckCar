import sensor, image, time, search, crossflag,output,chuan
from pid import PID
from image import SEARCH_EX, SEARCH_DS

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

templates = search.tupian()   #加载模板图

clock = time.clock()    #初始化时钟

subject=search.SEARCH0()   # 初始化搜索 （第一次搜索，搜不到不进循环）

flag=0
out=100   #初始速度
while(True):
    out=output.output()
    if crossflag.CROSSFLAG1():   # 是否是交叉口
        out=100  
        chuan.chuan_output(out)   # 传输输出
        for i in range(1,11):   
            dire=search.SEARCHX(subject,templates)   # 搜索
            chuan.chuan_output(out+50)  # 传输输出
            if dire != 0 :     # 如果不是直走则如跳出循环
                break
        if dire==0:     # 如果是直走
            chuan.chuan_dir(dire,flag)   # 传输方向
            while(crossflag.CROSSFLAG2()):   # 如果还是交叉口
                out=0    # 停车
                chuan.chuan_output(out+50)  
            out=output.output()
            chuan.chuan_output(out+50)
        else :        # 如果不是直走
            while(crossflag.CROSSFLAG2()):    # 如果还是交叉口，继续转
                chuan.chuan_output(out+50)    
            out=100
            chuan.chuan_output(out)
            flag=1     
            chuan.chuan_dir(dire,flag)  # 传输方向
            #time.sleep(0.5)
            flag=0
    out=output.output()
    chuan.chuan_output(out+50)
    print(out+50)
    #print(clock.fps())

