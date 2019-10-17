import sys
sys.path.append('./build/lib.linux-x86_64-3.6')
import k4a
import png
from sklearn.preprocessing import MinMaxScaler
import faulthandler
from color import ColorControl

faulthandler.enable()

def capture_3D(capture, filename):
    img = k4a.capture_get_depth_image(capture)

    if img:
        w = k4a.image_get_width_pixels(img)
        h = k4a.image_get_height_pixels(img)
        s = k4a.image_get_stride_bytes(img)
        print('Capture: {} x {} @ {}'.format(h, w, s))
        img_arr = k4a.image_to_array(img)
        scaler = MinMaxScaler(copy=True, feature_range=(0, 255))
        scaler.fit(img_arr)
        scaled_image = scaler.transform(img_arr)
        print(scaled_image[0])
        print(scaled_image[1])
        png.from_array(scaled_image.astype(int).tolist(), 'L').save('test.png')

def capture_2D(capture, filename):
    img = k4a.capture_get_color_image(capture)
    if img:
        w = k4a.image_get_width_pixels(img)
        h = k4a.image_get_height_pixels(img)
        s = k4a.image_get_stride_bytes(img)
        print('Capture: {} x {} @ {}'.format(h, w, s / w))

        try:
            with open(filename, 'wb') as fp:
                fp.write(k4a.image_get_buffer(img))
                fp.flush()
                fp.close()
        except:
            import sys
            print("Unexpected error:", sys.exc_info()[0])
    else:
        print('get image failed!')


def capture(dev, filename='test.jpg'):
    capture = k4a.Capture()
    res = k4a.device_get_capture(dev, capture, 1000)
    if res == k4a.K4A_WAIT_RESULT_SUCCEEDED:
        capture_3D(capture, filename)
    else:
        print('Capture failed!')


def main():
    dev = k4a.Device()

    if k4a.device_open(0, dev):
        # color control settings
        color_control = ColorControl(dev)
        params = dict()
        params['exposure_time_absolute'] = 100000
        params['brightness'] = 100

        color_control.apply_settings(params)
        color_control.show_settings()

        serial = k4a.device_get_serialnum(dev)
        version = k4a.device_get_version(dev)
        print("Opened device {} {}\n".format(serial, version))

        config = k4a.DeviceConfiguration()
        config.color_format = k4a.K4A_IMAGE_FORMAT_COLOR_MJPG
        config.color_resolution = k4a.K4A_COLOR_RESOLUTION_2160P
        config.depth_mode = k4a.K4A_DEPTH_MODE_NFOV_UNBINNED
        config.camera_fps = k4a.K4A_FRAMES_PER_SECOND_30
        config.synchronized_images_only = True

        if k4a.device_start_cameras(dev, config):
            capture(dev, f"exposure.jpg")
        else:
            print("Failed to start cameras!")

        k4a.device_stop_cameras(dev)
        k4a.device_close(dev)
        print("Closed")
    else:
        print("Failed to open device")

if __name__ ==  "__main__":
    main()

