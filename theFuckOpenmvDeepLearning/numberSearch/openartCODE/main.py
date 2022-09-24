
import pyb
import sensor, image, time, math
import tf
from  pyb import LED

from machine import UART
import os
#from ucollections import deque
...

...
#---------------------变量定义-----------------
white = LED(4)  # 定义一个LED4   照明灯
blue = LED(3)  #工作指示灯
uart = UART(2, baudrate=115200)     # 初始化串口 波特率设置为115200 TX是B12 RX是B13
COL = 320 #宽
ROW = 240 #高
#red_threshold=(17, 76, 6, 43, 6, 34)#正式赛道
red_threshold=(15, 76, 20, 66, 18, 68) #赛道红线阈值
#red_threshold=(5, 62, 27, 71, 6, 71) #测试环境红线阈值
NUM={'1':['A','a'],'2':['B','b'],'3':['C','c'],'4':['D','d'],'5':['E','e'],'6':['F','f'],'7':['G','g'],'8':['H','h']} #数字位置信息
classify_times = 3 #识别次数
numbers=['0' for i in range(11)]
numbers1=['0' for i in range(classify_times)]
numbers2=['0' for i in range(classify_times)]
numbers3=['0' for i in range(classify_times)]
numbers4=['0' for i in range(classify_times)]

#---------------------模型加载-----------------
net1_path = "trained_init.tflite"                      # 定义模型的路径
net1 = tf.load(net1_path, load_to_fb=True)              # 加载模型1
labels_init =[line.rstrip('\n') for line in open("/sd/labels_init.txt")]

net_path = "trained_normal.tflite"                      # 定义模型的路径
net = tf.load(net_path, load_to_fb=True)              # 加载模型2
labels_normal =[line.rstrip('\n') for line in open("/sd/labels_normal.txt")]

#net2_path = "trained_edge.tflite"                      # 定义模型的路径
#net2 = tf.load(net2_path, load_to_fb=True)              # 加载模型2
#labels_edge =[line.rstrip('\n') for line in open("/sd/labels_edge.txt")]


#---------------------函数定义-----------------

def sensor_init(): #初始化
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 500)
    sensor.set_auto_gain(False) # must be turned off for color tracking
    sensor.set_auto_whitebal(False) # must be turned off for color tracking

def find_red_cross(img):#搜索红色十字
    for blob in img.find_blobs([red_threshold], pixels_threshold=200, area_threshold=200, merge=True):
        # These values depend on the blob not being circular - otherwise they will be shaky.
        #if blob.elongation() > 0.5:
            #img.draw_edges(blob.min_corners(), color=(255,0,0))
            #img.draw_line(blob.major_axis_line(), color=(0,255,0))
            #img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        ## These values are stable all the time.
        #img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        # Note - the blob rotation is unique to 0-180 only.
        #img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        #print(blob.cx(),blob.cy())
        #return (blob.cx(), blob.cy())
        return (150,65)
    return None

def find_number_init(img):#初始识别给定数字
    roi_search=(101,40,120,160)   #初始识别区域
    for i in range(11):
        for obj in tf.classify(net1,img,roi_search,min_scale=1.0,scale_mul=0.5,x_overlap=0.5,y_overlap=0.5):
            sorted_list = sorted(zip(labels_init, obj.output()), key = lambda x: x[1], reverse = True)
            if(sorted_list[0][1]>0.6):#确保高准确率
                #print(sorted_list[0][0])
                numbers[i]=sorted_list[0][0]
                #return (sorted_list[0][0],roi_search)
            else:
                numbers[i]='0' #识别失败
    a = sorted(set(numbers), key=lambda x: numbers.count(x), reverse=True)#按出现次数排序
    if (a[0] != '0' and a[0] != 0):
        return (a[0], roi_search)
    else:
        return ('0', (0, 0, 0, 0))


def find_number_normal(img,area,mode): #识别道路数字
    roi_search=(0,0,0,0)
    #for i in range(classify_times):
    #cross_pt = find_red_cross(img)#找红色十字
        #if cross_pt is None:#没有找到红色十字
            #x_cross = 0
            #y_cross = 0
            #numbers1[i]=0
            #break
        #else:
             #x_cross, y_cross = cross_pt
    x_cross=160
    y_cross=65
    if (mode==1):#是否偏移
        offest=0
    else:
        offest=0

    if (area=='L1'):#选择搜索区域
        roi_search=(x_cross-100,y_cross+offest,100,110)#左侧1识别区域
    #elif (area=='L2'):
        #roi_search=(x_cross-180,y_cross+offest,100,110)#左侧2识别区域
    else:# (area=='R1'):
        roi_search=(x_cross,y_cross+offest,100,110) #右侧1识别区域
    #else:
        #roi_search=(x_cross+90,y_cross+offest,100,110)
    for i in range(3):
        for obj in tf.classify(net,img,roi_search,min_scale=1.0,scale_mul=0.5,x_overlap=0.5,y_overlap=0.5):
                #print("**********\nPredictions at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
            sorted_list = sorted(zip(labels_normal, obj.output()), key = lambda x: x[1], reverse = True)
            if(sorted_list[0][1]>0.6):#确保高准确率
                    #print(sorted_list[0][0])
                numbers1[i]=sorted_list[0][0]
                    #return (sorted_list[0][0],roi_search)
            else:
                numbers1[i]='0' #识别失败
                    #print(0)
                    #return (0,(0,0,0,0))

    a=sorted(set(numbers1), key=lambda x: numbers1.count(x), reverse=True) #按出现次数排序
    if (a[0] != '0' and a[0] != 0):
        return (a[0], roi_search)
    else:
        return ('0', (0, 0, 0, 0))

def find_number_edge(img,area,mode): #识别道路数字
    roi_search=(0,0,0,0)
    x_cross=160
    y_cross=65
    if (mode==1):#是否偏移
        offest=0
    else:
        offest=0


    if (area=='L2'):
        roi_search=(x_cross-180,y_cross+offest,100,110)#左侧2识别区域
    else:
        roi_search=(x_cross+90,y_cross+offest,100,110)

    for i in range(3):
        for obj in tf.classify(net,img,roi_search,min_scale=1.0,scale_mul=0.5,x_overlap=0.5,y_overlap=0.5):
                #print("**********\nPredictions at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
            sorted_list = sorted(zip(labels_normal, obj.output()), key = lambda x: x[1], reverse = True)
            if(sorted_list[0][1]>0.6):#确保高准确率
                    #print(sorted_list[0][0])
                numbers1[i]=sorted_list[0][0]
                    #return (sorted_list[0][0],roi_search)
            else:
                numbers1[i]='0' #识别失败
                    #print(0)
                    #return (0,(0,0,0,0))

    a=sorted(set(numbers1), key=lambda x: numbers1.count(x), reverse=True) #按出现次数排序
    if (a[0] != '0' and a[0] != 0):
        return (a[0], roi_search)
    else:
        return ('0', (0, 0, 0, 0))

flag=0# 延时标志位
def uart_get_order():
    uart_num = uart.any()       # 获取当前串口数据数量
    if(uart_num):
        flag=1
        uart_str = uart.read(uart_num) # 读取串口数据
        #uart.write(uart_str)    # 将读取到的串口数据发回
        if  (uart_str==b'0'): #初始识别
            white.on()
            sensor.skip_frames(time = 200)
            return  0
        elif (uart_str==b'1'): #中端识别
            white.on()
            sensor.skip_frames(time = 300)
            return  1
        elif (uart_str==b'2'): #远端1识别
            white.on()
            sensor.skip_frames(time = 300)

            return  2
        elif (uart_str==b'3'):  #远端2次识别
            sensor.skip_frames(time = 300)
            white.on()
            return  3
        elif (uart_str==b'g'): #远端识别
            white.off()
            return  4
        else:
            None
        print(uart_num,uart_str)
    else :
        return None



###############################程序运行############
sensor_init()
clock = time.clock()
order_num=None
while(True):
    clock.tick()
    #uart.write("uart test\r\n")
    t=uart_get_order()#串口获取指令
    #my_delay()
    img = sensor.snapshot()
    ##order_num=1;
    if (t!=None):
        order_num=t

    if (order_num==0): #初始识别
        #img = sensor.snapshot()
        number_find1,roi_finded1=find_number_init(img)
        if (number_find1!='0'):
            uart.write(NUM[number_find1][0])
            blue.toggle()
        img.draw_rectangle(roi_finded1, color = (0, 0, 255)) #画出识别区域
    elif (order_num==1): #中端识别
        #white.on()

        #img = sensor.snapshot()
        number_find1,roi_finded1=find_number_normal(img,'L1',0)
        if (number_find1 != '0' ):  # 左侧
            blue.toggle()
            print(number_find1)
            uart.write(NUM[number_find1][1])
            K1=number_find1
        number_find2,roi_finded2=find_number_normal(img,'R1',0)
        if (number_find2 != '0' ):#右侧
            blue.toggle()
            print(number_find2)
            uart.write(NUM[number_find2][0])
            K2=number_find2
        img.draw_rectangle(roi_finded1, color = (0, 0, 255)) #画出识别区域
        img.draw_rectangle(roi_finded2, color = (0, 0, 255)) #画出识别区域
    elif (order_num==2): #远端1次识别
        #white.on()

        #img = sensor.snapshot()
        number_find1,roi_finded1=find_number_normal(img,'R1',1)#68,60,70,90
        if (number_find1 !='0' ):#and number_find1!=K1 and number_find1 !=K2 ):  # 右侧 1
            uart.write(NUM[number_find1][0])
            blue.toggle()
            K3=number_find1
        else:
            K3=None

        number_find2,roi_finded2=find_number_edge(img,'R2',1)#180,60,70,90
        if (number_find2 != '0' ):#and number_find2!=K1 and number_find2 !=K2):      #右侧1
            uart.write(NUM[number_find2][0])
            blue.toggle()
            #pyb.mdelay(300)
            K4=number_find2
        else:
            K4=None
        number_find3,roi_finded3=find_number_normal(img,'L1',1)#68,60,70,90
        if (number_find3 !='0' ):#and number_find1!=K1 and number_find1 !=K2 ):  # 左侧 1
            uart.write(NUM[number_find3][1])
            blue.toggle()
            K5=number_find3
        else:
            K5=None

        number_find4,roi_finded4=find_number_edge(img,'L2',1)#180,60,70,90
        if (number_find4 != '0'):# and number_find2!=K1 and number_find2 !=K2):      #左侧1
            uart.write(NUM[number_find4][1])
            blue.toggle()
            #pyb.mdelay(300)
            K6=number_find4
        else:
            K6=None
        #if(K3!=None and K4!= None): #左侧数都识别成功 发送右侧数字
            #t=list("345678")
            #t.remove(K1)
            #t.remove(K2)
            #t.remove(K3)
            #t.remove(K4)
            #K5=t[0]
            #K6=t[1]
            #uart.write(NUM[K5][1])
            #blue.toggle()
            #pyb.mdelay(300)
            #uart.write(NUM[K6][1])
        img.draw_rectangle(roi_finded1, color = (0, 0, 255)) #画出识别区域
        img.draw_rectangle(roi_finded2, color = (0, 0, 255)) #画出识别区域
        img.draw_rectangle(roi_finded3, color = (0, 0, 255)) #画出识别区域
        img.draw_rectangle(roi_finded4, color = (0, 0, 255)) #画出识别区域2
    elif (order_num==3  ):#and K3!=None and K4!=None): #远端2次识别
        #white.on()
        #img = sensor.snapshot()
        number_find1,roi_finded1=find_number_normal(img,'L1',1)#左1侧 #68,60,70,90
        if (number_find1 !='0' and number_find1!=K1 and number_find1 !=K2 ):  # 左侧 1
            uart.write(NUM[number_find1][1])
            blue.toggle()
        #number_find2,roi_finded2=find_number_normal(img,'R1',0)
        #if (number_find2 != '0' and number_find1!=K1 and number_find1 !=K2):#右侧
            #uart.write(NUM[number_find2][0])
            #blue.toggle()
        if (number_find1 !='0'):
            if(number_find1==K3):       #左侧 1
                uart.write(NUM[K3][0])
                pyb.mdelay(200)
                uart.write(NUM[K4][1])
            elif(number_find1==K4):
                uart.write(NUM[K3][1])
                pyb.mdelay(200)
                uart.write(NUM[K4][0])
            elif(number_find1==K5):
                uart.write(NUM[K5][0])
                pyb.mdelay(200)
                uart.write(NUM[K6][1])
            else:# (number_find1==K6)
                uart.write(NUM[K5][1])
                pyb.mdelay(200)
                uart.write(NUM[K6][0])
    elif (order_num==4):
        white.off()
    #print(order_num)

