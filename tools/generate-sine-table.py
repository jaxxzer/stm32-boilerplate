# make a table of 512 16-bit entries (1024 bytes) representing
# a single quadrant (pi/2) of sine look up table
# use reflections etc to reproduce entire sine wave
import math

amplitude = 0xfff
base = amplitude/2
max = amplitude/2
angle = 0

resolution = 256
step = 1024/resolution

lookup = []
print("uint16_t sinLookup[] = {")

max = 0xfff

totalAngle = math.pi/2
amplitude = 0xfff/2

for i in range(resolution):
    angle = amplitude * math.sin(i*totalAngle/resolution)
    angle = int(angle)
    print(angle, ",")
    lookup.append(angle)
print("}")

#input 0~0x1024 (4x resolution)
def sin(angle=0):
    if angle > resolution*4:
        angle = resolution*4
    # 0~1024
    if angle < resolution:
        return 2048 + lookup[angle]
    # 1024~2048
    elif angle < resolution*2:
        angle = angle - resolution
        return 2048 + lookup[resolution - angle - 1]
    # 2048~3072 
    elif angle < resolution*3:
        angle = angle - 2*resolution
        return 2048 - lookup[angle]
    # 3072~4095
    elif angle < resolution*4:
        angle = angle - 3*resolution
        return 2048 - lookup[resolution - angle - 1]

for n in range(resolution*4):
    break
    print("angle:", sin(n))
