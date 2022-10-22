import time, sensor, image
from image import SEARCH_EX, SEARCH_DS



############定义为一个函数SEARCH
def SEARCH(templates):
    clock = time.clock()
    clock.tick()
    img = sensor.snapshot()
    count=0
    while(count == 0):
        for t in templates:
            template = image.Image(t)
            r = img.find_template(template, 0.70, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
            if r:
                img.draw_rectangle(r)
                count = count + 1
                break
    print(count)
    return count
