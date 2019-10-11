import sys
sys.path.append('./build/lib.linux-x86_64-3.6')
import k4a

dev = k4a.Device()
if k4a.device_open(0, dev):
    # version = k4a.device_get_version(dev)
    # print("version {}\n".format(version))
    config = k4a.DeviceConfiguration()
    config.color_format = k4a.K4A_IMAGE_FORMAT_COLOR_MJPG
    config.color_resolution = k4a.K4A_COLOR_RESOLUTION_2160P
    config.depth_mode = k4a.K4A_DEPTH_MODE_NFOV_UNBINNED
    config.camera_fps = k4a.K4A_FRAMES_PER_SECOND_30
    config.synchronized_images_only = True

    if k4a.device_start_cameras(dev, config):
        print(">> get current color control")
        print(k4a.device_get_color_control(dev, k4a.K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE))
        print(">> set color control's exposure time absolute")
        print(k4a.device_set_color_control(dev, k4a.K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE, k4a.K4A_COLOR_CONTROL_MODE_MANUAL, 83330))
        print(">> check if the settings persists")
        print(k4a.device_get_color_control(dev, k4a.K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE))


k4a.device_stop_cameras(dev)
k4a.device_close(dev)
