import sensor, image, time, pyb
from image import SEARCH_EX, SEARCH_DS
import led, chuan

clock = time.clock()
def SEARCH0 ():
    led.led_blue.on()
    templates0 = ["/1.pgm", "/2.pgm", "/3.pgm","/4.pgm","/5.pgm","/6.pgm","/7.pgm","/8.pgm"]
    while (True):
        clock.tick()
        img = sensor.snapshot()
        img = img.to_grayscale()      # 灰度化
        count=0
        for t in templates0:    #遍历模板
            template = image.Image(t)   #加载模板
            #对每个模板遍历进行模板匹配
            r = img.find_template(template, 0.70, step=4, search=SEARCH_EX) # roi=(10, 0, 60, 60))
        #  find_template(template, threshold, [roi, step, search])
        #  threshold中的0.7是相似度阈值,roi是进行匹配的区域（左上顶点为（10，0），长80宽60的矩形区域）
        #  roi的大小要比模板图片大，比frambuffer小
            if r:        #如果匹配成功
                img.draw_rectangle(r)
                print('模板名字',t)
                print('匹配的坐标',count)
                led.led_blue.off()
                chuan.chuan_count(count)
                time.sleep(2)
                return count
            count=count+1
        #print(clock.fps())

def tupian ():
    templates1 = ["/1_1.pgm", "/1_2.pgm", "/1_3.pgm"]
    templates2 = ["/2_1.pgm", "/2_2.pgm", "/2_3.pgm"]
    templates3 = ["/3_1.pgm", "/3_2.pgm", "/3_3.pgm", "/3hg.pgm", "/3l.pgm", "/3r.pgm"]
    templates4 = ["/4_1.pgm", "/4_2.pgm", "/4_3.pgm", "/4hg.pgm", "/4l.pgm", "/4r.pgm"]
    templates5 = ["/5_1.pgm", "/5_2.pgm", "/5_3.pgm", "/5hg.pgm", "/5l.pgm", "/5r.pgm", "/5-h1.pgm"]
    templates6 = ["/6_1.pgm", "/6_2.pgm", "/6_3.pgm", "/6hg.pgm", "/6l.pgm", "/6r.pgm"]
    templates7 = ["/7_1.pgm", "/7_2.pgm", "/7_3.pgm", "/7hg.pgm", "/7l.pgm", "/7r.pgm"]
    templates8 = ["/8_1.pgm", "/8_2.pgm", "/8_3.pgm", "/8hg.pgm", "/8l.pgm", "/8r.pgm"]
    templates = [templates1,templates2,templates3,templates4,templates5,templates6,templates7,templates8]
    return templates

def SEARCHX (subject,templates):   #subject是要匹配的图像，templates是模板
    img = sensor.snapshot()
    img = img.to_grayscale()
    y = templates[subject]
    dire = 0     #dire是方向  0是直  1是左  2是右
    if subject>1:
        for x in y:
            template = image.Image(x)
            r = img.find_template(template, 0.70, step=4, search=SEARCH_EX)
            if r:
                img.draw_rectangle(r)
                print(x)
                if 0<r[0]<50:
                    dire=1       #left
                elif 50<r[0]<160:
                    dire=2       #right
    print('searchx_dire:', dire)
    return dire















