import sensor, image, time


def CROSSFLAG1 ():  #判断是否为十字路口
    clock = time.clock()
    clock.tick()
    img = sensor.snapshot()
    statistics = img.get_statistics(roi=(0,0,160,10))    #统计图像中的颜色，roi为统计区域，roi需要具体调整
    if 0<statistics.l_mode()<100 and 0<statistics.a_mode()<120 and 0<statistics.b_mode()<120:
        print("stop")
        return 1
    else:
        return 0
        ##给TX停车信号，然后需要启动search

def CROSSFLAG2 ():    #判断是否为十字路口
    clock = time.clock()
    clock.tick()  
    img = sensor.snapshot()
    statistics = img.get_statistics(roi=(0,110,160,10))   #统计图像中的颜色，roi为统计区域
    if 0<statistics.l_mode()<100 and 0<statistics.a_mode()<120 and 0<statistics.b_mode()<120:
        return 0
    else:
        return 1
##给TX信号传到stm32，由dir通知电机转向或直行




