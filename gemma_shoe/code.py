"""
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    Note:
    This code is modified from Michael Wild original code
    Some of the code is harvested and rewritten from Adafruit. Thank you
    Adafruit for all the good ideas!

==============================================================================
This program drives NeoPixels

"""
# pylint: disable-msg=C0103,C0111

import random
import time
import board
import neopixel
import adafruit_dotstar as dotstar
from analogio import AnalogIn

print("Ming's Smart Shoe")

# ######################## Definitions ##############################
analog1in = AnalogIn(board.A2)   # photocell
# analog1in2 = AnalogIn(board.A0)  # vibrator
photocell_day_value = 5000
vibrator_shake = 22000
vibrator_avail = False

pixel_pin = board.D2  # The pin the NeoPixels are connected to
pixel_count = 23    # Number of pixels in your strip
color_change = 32   # Number to increase color change (how fast you want it)
n_repeat = 10
fill_color_delta = 32   # Number to increase color change (how fast you want it)
short_delay = 0.03  # 30ms
long_delay = 0.2    # 500ms
dom_delay = 0.5
pause_time = 3 * 60 # sleep for N min
color_value = 80    # mono color initial value

pixels = neopixel.NeoPixel(pixel_pin, pixel_count,
                           brightness=.4, auto_write=False)
# One pixel connected internally!
dot = dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1, brightness=0.2)

def wheel(pos):
    """
    pos: place on color wheel of 0-255 values
    Helper to give us a nice color swirl
    Thank you Adafruit for the nice wheel routine!

    Input a value 0 to 255 to get a color value.
    The colours are a transition r - g - b - back to r.
    """
    if pos < 0:
        return [0, 0, 0]
    if pos > 255:
        return [0, 0, 0]
    if pos < 85:
        return [int(pos * 3), int(255 - (pos*3)), 0]
    if pos < 170:
        pos -= 85
        return [int(255 - pos*3), 0, int(pos*3)]
    pos -= 170
    return [0, int(pos*3), int(255 - pos*3)]

# ######################## Start up ##############################
# Randomly assign colors using wheel
colors = [random.randint(0, 255) for x in range(0, pixel_count)]

def clear(start_pos, end_pos):
    for x in range(start_pos, end_pos):
        pixels[x] = (0, 0, 0)
        pixels.show()
    time.sleep(long_delay)

def calc_delay():
    if not vibrator_avail:
        delay_time = short_delay
#    else:
#        analog1in2.value < vibrator_shake:
#        delay_time = vibrator / (vibrator_shake * 4)
    return delay_time

def show_ranbow(s_pos, e_pos, d_time, r_times):
    for x in range(s_pos, e_pos):     #initial
        pixels[x] = wheel(random.randint(0, 255))
        pixels.show()
        time.sleep(d_time)
    print("show_ranbow, initiated")
    for m in range(r_times):
        print(m)
#        rdm_pos = random.randint(s_pos, e_pos)
        for idx in range(s_pos, e_pos):
#            idx = (n + rdm_pos) % (e_pos - s_pos)
            x = list(pixels[idx])
            x = [max(0, v - color_change) if v > 0 else v for v in x]
            pixels[idx] = wheel(random.randint(0, 255)) if sum(x) < 20 \
                else tuple(x)
            pixels.show()
            time.sleep(d_time)
    print("done show_ranbow")
    time.sleep(short_delay)

def show_mono(to_color):
    print("show_mono to_color=", to_color)
    pixels.fill(to_color)
    pixels.show()
    time.sleep(short_delay*2)
    done = False
    while not done:
        ii = random.randint(0, pixel_count)
        wait_time = calc_delay()
        for n in range(pixel_count):
            idx = (ii + n) % pixel_count
            x = list(pixels[idx])
            x = [min(255, v + fill_color_delta) if v > 0 else v for v in x]
            pixels[idx] = tuple(x)
            pixels.show()
            time.sleep(wait_time)
            if max(x) == 255:
                print("show_mono + done")
                done = True
                break
    done = False
    while not done:
        wait_time = calc_delay()
        ii = random.randint(0, pixel_count)
        for n in range(pixel_count):
            idx = (ii + n) % pixel_count
            x = list(pixels[idx])
            x = [max(16, v - fill_color_delta) if v > 0 else v for v in x]
            pixels[idx] = tuple(x)
            pixels.show()
            time.sleep(wait_time)
            if max(x) == 16:
                print("show_mono - done")
                done = True
                break

mono_colors = [(color_value, 0, 0), (0, color_value, 0), (0, 0, color_value),  \
    (color_value, color_value, 0), (0, color_value, color_value), \
    (color_value, 0, color_value), (color_value, color_value, color_value)]
mono_num = len(mono_colors) - 1  # randint is inclusive
def do_lights():
    wait_time = calc_delay()
    lucky_value = random.randint(0, mono_num * 2)
    if lucky_value <= mono_num:
        show_mono(mono_colors[lucky_value])
        show_ranbow(0, pixel_count, wait_time, n_repeat)
    else:
        show_ranbow(0, pixel_count, wait_time, n_repeat)
        lucky_value = random.randint(0, mono_num)
        show_mono(mono_colors[lucky_value])

# ######################## MAIN LOOP ##############################
print("start ... 1")
time.sleep(long_delay)
print("start ... 2")
time.sleep(long_delay)
print("start ... 3")
time.sleep(long_delay)
do_lights()
i = 0
downtime = -1.0
while True:
    # spin internal LED around!
    # This is a cool idea from Adafruit to make it look like the dot runs
    # by itself!
    dot[0] = wheel(i)
    dot.show()
    photocell = analog1in.value
#    print("photocell=", photocell)
#    if photocell < photocell_day_value or vibrator < vibrator_shake:
    if photocell < photocell_day_value:
        print("do_lights ...photocell=", photocell)
        do_lights()
    else:
        if downtime == -1.0:
            print("into sleep mode ...photocell=", photocell)
            clear(0, pixel_count-2)
            downtime = pause_time
        else:
            clear(0, pixel_count-2)
            for pos_n in [-2, -1]:
                apixel = list(pixels[pos_n])
                apixel = [max(0, v - 8) if v > 0 else v for v in apixel]
                pixels[pos_n] = wheel(random.randint(0, 255)) if sum(apixel) < 20 \
                    else tuple(apixel)
                pixels.show()
                time.sleep(dom_delay)
                downtime -= dom_delay
            if downtime <= 0:              # wake uo show loghts
                print("out sleep mode ...")
                downtime = -1.0
                do_lights()
    i = (i+1) % 256  # run from 0 to 255
