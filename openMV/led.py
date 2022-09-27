import sensor, time
from pyb import LED

led_red = pyb.LED(1)
led_green = pyb.LED(2)
led_blue = pyb.LED(3)


def blink_green(isblink):
    while (isblink):
        led_green.on()
        time.sleep_ms(150)
        led_green.off()
        time.sleep_ms(100)
        led_green.on()
        time.sleep_ms(150)
        led_green.off()


    