import time, sensor, image
from image import SEARCH_EX, SEARCH_DS
import A

sensor.reset()
sensor.set_contrast(1)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.QQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
templates0 = ["/1.pgm", "/2.pgm", "/3.pgm", "/4.pgm"] #保存多个模板
#加载模板图

clock = time.clock()


while (True):
    count = A.SEARCH(templates0)
    print(count)
    
