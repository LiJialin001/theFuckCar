import sensor, image, time
from image import SEARCH_EX, SEARCH_DS


clock = time.clock()
def SEARCH0 ():
    templates0 = ["/1.pgm", "/2.pgm", "/3.pgm","/4.pgm","/5.pgm","/6.pgm","/7.pgm","/8.pgm"]
    while (True):
        clock.tick()
        img = sensor.snapshot()
        img = img.to_grayscale()
        count=0
        for t in templates0:
            template = image.Image(t)
            #对每个模板遍历进行模板匹配
            r = img.find_template(template, 0.70, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
        #find_template(template, threshold, [roi, step, search]),threshold中
        #的0.7是相似度阈值,roi是进行匹配的区域（左上顶点为（10，0），长80宽60的矩形），
        #注意roi的大小要比模板图片大，比frambuffer小。
        #把匹配到的图像标记出来
            if r:
                img.draw_rectangle(r)
                print(t) #打印模板名字
                #print(count)
                return count;
            count=count+1
        #print(clock.fps())

def tupian ():
    templates1 = ["/1_1.pgm", "/1_2.pgm", "/1_3.pgm"]
    templates2 = ["/2_1.pgm", "/2_2.pgm", "/2_3.pgm"]
    templates3 = ["/3_1.pgm", "/3_2.pgm", "/3_3.pgm"]
    templates4 = ["/4_1.pgm", "/4_2.pgm", "/4_3.pgm"]
    templates5 = ["/5_1.pgm", "/5_2.pgm", "/5_3.pgm"]
    templates6 = ["/6_1.pgm", "/6_2.pgm", "/6_3.pgm"]
    templates7 = ["/7_1.pgm", "/7_2.pgm", "/7_3.pgm"]
    templates8 = ["/8_1.pgm", "/8_2.pgm", "/8_3.pgm"]
    templates = [templates1,templates2,templates3,templates4,templates5,templates6,templates7,templates8]
    return templates

def SEARCHX (subject,templates):
    img = sensor.snapshot()
    img = img.to_grayscale()
    y = templates[subject]
    dire = 0
    for x in y:
        template = image.Image(x)
        r = img.find_template(template, 0.70, step=4, search=SEARCH_EX)
        if r:
            img.draw_rectangle(r)
            print(x)
            if 0<r[0]<40:
                dire=1   #left
            elif 120<r[0]<160:
                dire=2   #right
            #elif r[0]==0:
                #dir=0   #straight
    print(dire)
    return dire















