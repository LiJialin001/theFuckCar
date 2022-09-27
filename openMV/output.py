import sensor, image, time
from pyb import LED
from pid import PID
rho_pid = PID(p=0.4, i=0)
theta_pid = PID(p=0.001, i=0)

#sensor.reset()
#sensor.set_vflip(True)
#sensor.set_hmirror(True)
#sensor.set_pixformat(sensor.RGB565)
#sensor.set_framesize(sensor.QQVGA)
##sensor.set_windowing([0,20,80,40])
#sensor.skip_frames(time = 500)
clock = time.clock()

THRESHOLD = (16, 38, 27, 50, 13, 42)  

def output ():
    # LED(1).on()
    # LED(2).on()
    # LED(3).on() 
    #sensor.reset()
    #sensor.set_vflip(True)
    #sensor.set_hmirror(True)
    #sensor.set_pixformat(sensor.RGB565)
    #sensor.set_framesize(sensor.QQVGA)
    #sensor.skip_frames(time = 100)

    clock.tick()
    img = sensor.snapshot().binary([THRESHOLD])  
    line = img.get_regression([(100,100)], robust = True)  # 找线
    if (line):                                             # 如果找到线
        rho_err = abs(line.rho())-img.width()/2            # 计算偏差
        if line.theta()>90:                                # 如果线夹角大于90度
            theta_err = line.theta()-180                   # 计算误差角度
        else:
            theta_err = line.theta()                       # 计算误差角度
        img.draw_line(line.line(), color = 127)            # 在图像上画出线
        #print(rho_err,line.magnitude(),rho_err)
        if line.magnitude()>8:                             # 如果线的长度大于8
            #if -40<b_err<40 and -30<t_err<30:
            rho_output = rho_pid.get_pid(rho_err,1)         # 计算输出
            theta_output = theta_pid.get_pid(theta_err,1)
            output = rho_output+theta_output               # 计算最终输出    rho_output+theta_output
        else:
            output=100   #停车
    else:
        output=200   #原地转
    return output
    #print(clock.fps())
