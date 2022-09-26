import sensor, time
from pyb import LED

led_red = pyb.LED(1)
led_green = pyb.LED(2)
led_blue = pyb.LED(3)


# whike (True):
#     led.on()
#     time.sleep_ms(150)
#     led.off()
#     time.sleep_ms(100)
#     led.on()
#     time.sleep_ms(150)
#     led.off()
    