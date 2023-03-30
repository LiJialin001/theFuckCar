import sensor, image, time
from pyb import LED
from pid import PID
rho_pid = PID(p=1.5)
theta_pid = PID(p=0.01)

clock = time.clock()

THRESHOLD = (30, 50, 27, 70, 0, 45)

def output ():

    clock.tick()
    img = sensor.snapshot().binary([THRESHOLD])
    line = img.get_regression([(100,100)], robust = True)  # 找线
    if (line):                                             # 如果找到线
        rho_err = abs(line.rho())-img.width()/2            # 计算偏差
        if line.theta()>90:                                # 如果线夹角大于90度
            theta_err = line.theta()-180                   # 计算误差角度
        elif line.theta()>45:
            rho_err = 0
            theta_err = 0
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
            output=0.01
    else:
        output=0.01
    return output
    #print(clock.fps())
