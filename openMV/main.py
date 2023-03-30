import sensor, image, time, search, crossflag,output,chuan
from pid import PID
from image import SEARCH_EX, SEARCH_DS
import led

sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 1000)

THRESHOLD = (13,100,-18,30,-2,23)

# 转向矩阵
TurnMatrix = [[1,0,0,0], [2,0,0,0], [0,1,0,0], [0,2,0,0], [0,0,1,2], [0,0,2,2], [0,0,1,1], [0,0,2,1]]

templates = search.tupian()   # 加载模板图
clock = time.clock()    # 初始化时钟

subject=search.SEARCH0()  # 初始化搜索 （第一次搜索，搜不到不进循环）
print(subject)

flag=0
dire=0      # dire = 1 左转，2 右转，3返回
out=0       # 初始速度0
Sig=1       # 初始信号 （Sig-1）判断巡线偏离
count = subject  # 检测到的数字（n-1）
chuan.chuan_output(Sig, out, dire, count)
step = 0  # 0直线 1 十字路口 2 返回
time.sleep_ms(500)

isGoHome = False

turnIndex = 0

while(True):
    if turnIndex==0 and isGoHome==True:
        step = 4
    if step == 0:                         # 直线模式，判断是否为十字路口，判断是否需要返回
        out = output.output()             # pid寻线并输出
        dire = 0
        chuan.chuan_output(Sig, out, dire, count)
        if crossflag.CROSSFLAG1():   # 是否是交叉口
            time.sleep_ms(430)
            out = 0
            Sig = 1
            chuan.chuan_output(Sig,out,dire,count)      # 停车
            if isGoHome:
                step = 3
            else:
                step = 1
        elif crossflag.CROSSFLAG2():    # 是否返回路口
            time.sleep_ms(430)
            out = 0
            Sig = 1
            chuan.chuan_output(Sig,out,dire,count)
            step = 2
    elif step == 1:                          # 前进十字路口模式，要转的方向向传给32，并等待至出十字路口
        time.sleep_ms(1000)
        dire = TurnMatrix[count][turnIndex]
        if turnIndex < 4:
            turnIndex += 1
        chuan.chuan_output(Sig,out,dire,count) # 传递十字路口信号
        step = 0
        print('cross')
        time.sleep_ms(1500)     # 等待转向
    elif step == 2:             # 更改运行模式为 go home（转向反向
        isGoHome = True
        dire = 3
        chuan.chuan_output(Sig,out,dire,count) # 传递返回信号
        step = 0
        print('back')
        time.sleep_ms(1500)
    elif step == 3:          # 返回的转向口
        time.sleep_ms(1000)
        if turnIndex > 0:
            turnIndex -= 1
        if TurnMatrix[count][turnIndex] == 1:
            dire = 2
        elif TurnMatrix[count][turnIndex] == 2:
            dire = 1
        else:
            dire = 0
            out = output.output()
        chuan.chuan_output(Sig,out,dire,count) # 传递十字路口信号
        step = 0
    else:                   # 停车结束运行
        out = 0
        dire = 0
        chuan.chuan_output(Sig,out,dire,count)














