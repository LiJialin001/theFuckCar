import sensor, image, time
import led


Down_ROI = (0, 110, 160, 10)    # 设置ROI区域

Right_ROI = (125,73,16,40)
Left_ROI = (20,73,16,40)


def CROSSFLAG1 ():  # 判断是否为十字路口
    Center_ROI = (0, 15, 160, 45)    # 设置ROI区域
    clock = time.clock()
    clock.tick()
    img = sensor.snapshot()
    #img.draw_rectangle(Center_ROI)
    #statistics = img.get_statistics(roi = Center_ROI)    # 统计图像中的颜色，roi为统计区域，roi需要具体调整

    img.draw_rectangle(Right_ROI)
    img.draw_rectangle(Left_ROI)

    statistics_left = img.get_statistics(roi = Left_ROI)
    statistics_right = img.get_statistics(roi = Right_ROI)
    #if 2<statistics.a_mean():   # 判断是否为红色
    if statistics_left.l_min()<40 and statistics_right.l_min()<40 and statistics_left.a_max()>40 and statistics_right.a_max()>40:
        print("stop")
        return 1
    else:
        return 0    # 给TX停车信号，然后需要启动search

def CROSSFLAG2 ():    # 判断是否为十字路口
    clock = time.clock()
    clock.tick()
    img = sensor.snapshot()
    img.draw_rectangle(Down_ROI)
    statistics = img.get_statistics(roi = Down_ROI)
    if 0<statistics.l_mode()<100 and 0<statistics.a_mode()<120 and 0<statistics.b_mode()<120:
        return 0
    else:
        return 1     # 给TX信号传到stm32，由dir通知电机转向或直行





