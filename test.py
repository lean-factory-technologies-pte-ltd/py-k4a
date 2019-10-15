import sys
sys.path.append('./build/lib.linux-x86_64-3.6')
import k4a
from color import ColorControl

dev = k4a.Device()
color_control = ColorControl(dev)
params = { 'exposure_time_absolute': 100000, 'brightness': 100 }

color_control.apply_settings(params)
color_control.show_settings()

if k4a.device_open(0, dev):

    print(color_control.get('exposure_time_absolute'))
    color_control.set('exposure_time_absolute', 500)
    print(color_control.get('exposure_time_absolute'))

k4a.device_stop_cameras(dev)
k4a.device_close(dev)
