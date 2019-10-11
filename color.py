import k4a
from enum import IntEnum

class ColorCommand(IntEnum):
    EXPOSURE_TIME_ABSOLUTE = k4a.K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE,
    AUTO_EXPOSURE_PRIORITY = k4a.K4A_COLOR_CONTROL_AUTO_EXPOSURE_PRIORITY
    BRIGHTNESS = k4a.K4A_COLOR_CONTROL_BRIGHTNESS
    CONTRAST = k4a.K4A_COLOR_CONTROL_CONTRAST
    SATURATION = k4a.K4A_COLOR_CONTROL_SATURATION
    SHARPNESS = k4a.K4A_COLOR_CONTROL_SHARPNESS
    WHITEBALANCE = k4a.K4A_COLOR_CONTROL_WHITEBALANCE
    BACKLIGHT_COMPENSATION = k4a.K4A_COLOR_CONTROL_BACKLIGHT_COMPENSATION
    GAIN = k4a.K4A_COLOR_CONTROL_GAIN
    POWERLINE_FREQUENCY = k4a.K4A_COLOR_CONTROL_POWERLINE_FREQUENCY

class ColorControl:
    command_mappings = {
        'exposure_time_absolute' : k4a.K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE,
        'auto_exposure_priority' : k4a.K4A_COLOR_CONTROL_AUTO_EXPOSURE_PRIORITY,
        'brightness' : k4a.K4A_COLOR_CONTROL_BRIGHTNESS,
        'contrast' : k4a.K4A_COLOR_CONTROL_CONTRAST,
        'saturation' : k4a.K4A_COLOR_CONTROL_SATURATION,
        'sharpness' : k4a.K4A_COLOR_CONTROL_SHARPNESS,
        'whitebalance' : k4a.K4A_COLOR_CONTROL_WHITEBALANCE,
        'backlight_compensation' : k4a.K4A_COLOR_CONTROL_BACKLIGHT_COMPENSATION,
        'gain' : k4a.K4A_COLOR_CONTROL_GAIN,
        'powerline_frequency' : k4a.K4A_COLOR_CONTROL_POWERLINE_FREQUENCY,
    }

    def __init__(self, device):
        self.device = device
        self.mode   = k4a.K4A_COLOR_CONTROL_MODE_MANUAL

    def get(self, command):
        return k4a.device_get_color_control(self.device, self.command_mappings[command])

    def set(self, command, val):
        result = k4a.device_set_color_control(self.device, self.command_mappings[command], self.mode, val)
        # restart device
        return result


    def apply_settings(self, params):
        for command, val in params.items():
            self.set(command, val)

    def show_settings(self):
        for command in self.command_mappings.items():
            val = self.get(command[0])
            print(f"{command[0]} = {val[1]}")
