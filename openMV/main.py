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

templates = search.tupian()

clock = time.clock()

subject=search.SEARCH0()

flag=0
out=100
while(True):
    out=output.output()
    if crossflag.CROSSFLAG1():
        out=100
        chuan.chuan_output(out)
        for i in range(1,11):
            dire=search.SEARCHX(subject,templates)
            chuan.chuan_output(out+50)

            if dire != 0 :
                break
        if dire==0:
            chuan.chuan_dir(dire,flag)
            while(crossflag.CROSSFLAG2()):
                out=0
                chuan.chuan_output(out+50)
            out=output.output()
            chuan.chuan_output(out+50)
        else :
            while(crossflag.CROSSFLAG2()):
                out=output.output()
                chuan.chuan_output(out+50)
            out=100
            chuan.chuan_output(out)
            flag=1
            chuan.chuan_dir(dire,flag)
            #time.sleep(0.5)
            flag=0
    out=output.output()
    chuan.chuan_output(out+50)
    print(out+50)
    #print(clock.fps())

