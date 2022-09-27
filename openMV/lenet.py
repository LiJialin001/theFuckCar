# LetNet数字识别例程
import sensor, image, time, os, nn

sensor.reset()                         # 复位并初始化传感器。

sensor.set_contrast(3)
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
#设置图像色彩格式，有RGB565色彩图和GRAYSCALE灰度图两种

sensor.set_framesize(sensor.QVGA)      # 将图像大小设置为QVGA (320x240)

sensor.set_windowing((128, 128))       # 设置128 x128窗口。
sensor.skip_frames(time=100)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False)

# 加载lenet神经网络模型
net = nn.load('/lenet.network')
labels = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']

clock = time.clock()                # 创建一个时钟对象来跟踪FPS帧率。
while(True):
    clock.tick()                    # 更新FPS帧率时钟。
    img = sensor.snapshot()         # 拍一张照片并返回图像。
    out = net.forward(img.copy().binary([(150, 255)], invert=True))
    max_idx = out.index(max(out))
    score = int(out[max_idx]*100)
    if (score < 70):
        score_str = "??:??%"
    else:
        score_str = "%s:%d%% "%(labels[max_idx], score)
    img.draw_string(0, 0, score_str)

    print(clock.fps())             
    # 注意: 当连接电脑后，OpenMV会变成一半的速度。当不连接电脑，帧率会增加。
    #打印当前的帧率。
