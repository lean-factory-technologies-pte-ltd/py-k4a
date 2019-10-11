#include <Python.h>
#include <structmember.h>
#include <k4a/k4a.h>

#include "types.h"

static PyTypeObject DeviceType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "k4a.Device",
    .tp_doc = "Kinect for Azure Device",
    .tp_basicsize = sizeof(DeviceObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

static PyMemberDef DeviceConfiguration_members[] = {
    {"color_format", T_UINT, offsetof(DeviceConfiguration, color_format), 0, "color format enum"},
    {"color_resolution", T_UINT, offsetof(DeviceConfiguration, color_resolution), 0, "color resolution enum"},
    {"depth_mode", T_UINT, offsetof(DeviceConfiguration, depth_mode), 0, "depth mode enum"},
    {"camera_fps", T_UINT, offsetof(DeviceConfiguration, camera_fps), 0, "camera fps enum"},
    {"synchronized_images_only", T_BOOL, offsetof(DeviceConfiguration, synchronized_images_only), 0, "synchronized images only"},
    {"delay_depth_off_color_usec", T_INT, offsetof(DeviceConfiguration, depth_delay_off_color_usec), 0, "depth delay off color usec"},
    {"wired_sync_mode", T_UINT, offsetof(DeviceConfiguration, wired_sync_mode), 0, "wired sync mode enum"},
    {"subordinate_delay_off_master_usec", T_UINT, offsetof(DeviceConfiguration, subordinate_delay_off_master_usec), 0, "subordinate delay off master usec"},
    {"disable_streaming_indicator", T_BOOL, offsetof(DeviceConfiguration, disable_streaming_indicator), 0, "disable streaming indicator"},
    {NULL} /* Sentinel */
};

static int
DeviceConfiguration_init(DeviceConfiguration *self, PyObject *args, PyObject *kwds)
{
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    self->color_format = config.color_format;
    self->color_resolution = config.color_resolution;
    self->depth_mode = config.depth_mode;
    self->depth_delay_off_color_usec = config.depth_delay_off_color_usec;
    self->camera_fps = config.camera_fps;
    self->disable_streaming_indicator = config.disable_streaming_indicator;
    self->subordinate_delay_off_master_usec = config.subordinate_delay_off_master_usec;
    self->synchronized_images_only = config.synchronized_images_only;
    self->wired_sync_mode = config.wired_sync_mode;

    return 0;
}

static PyTypeObject DeviceConfigurationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "k4a.DeviceConfiguration",
    .tp_doc = "Kinect for Azure Device Configuration",
    .tp_basicsize = sizeof(DeviceConfiguration),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_members = DeviceConfiguration_members,
    .tp_init = (initproc) DeviceConfiguration_init,
};

static PyMemberDef Version_members[] = {
    {"major", T_UINT, offsetof(VersionInfo, major), 0,
     "major version"},
    {"minor", T_UINT, offsetof(VersionInfo, minor), 0,
     "minor version"},
    {"iteration", T_INT, offsetof(VersionInfo, iteration), 0,
     "iteration version"},
    {NULL} /* Sentinel */
};

static PyTypeObject VersionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "k4a.VersionInfo",
    .tp_doc = "Kinect for Azure Version Info",
    .tp_basicsize = sizeof(VersionInfo),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_members = Version_members,
};

static PyMemberDef Hardware_members[] = {
    {"rgb", T_OBJECT_EX, offsetof(HardwareVersion, rgb), 0,
     "rgb version"},
    {NULL} /* Sentinel */
};

void Hardware_dealloc(HardwareVersion *self)
{
    Py_XDECREF(self->rgb);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyTypeObject HardwareType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "k4a.HardwareVersion",
    .tp_doc = "Kinect for Azure Hardware Version Info",
    .tp_basicsize = sizeof(HardwareVersion),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)Hardware_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_members = Hardware_members,
};

void Capture_dealloc(CaptureObject* self) {
    if (self->capture != NULL) {
        k4a_capture_release(self->capture);
    }
}

void Capture_init(CaptureObject* self, PyObject* args, PyObject* kwds) {
    self->capture = NULL;
}

static PyTypeObject CaptureType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "k4a.Capture",
    .tp_doc = "Kinect for Azure Device",
    .tp_basicsize = sizeof(CaptureObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor) Capture_dealloc,
};

void Image_dealloc(ImageObject* self) {
    if (self->image) {
        k4a_image_release(self->image);
    }
}

static PyTypeObject ImageType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "k4a.Image",
    .tp_doc = "Kinect for Azure Device",
    .tp_basicsize = sizeof(ImageObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor) Image_dealloc,
};

ImageObject* newImageObject() {
    ImageObject* res = PyObject_New(ImageObject, &ImageType);
    PyObject_Init((PyObject*) res, &ImageType);

    return res;
}

bool initTypes()
{
    if (PyType_Ready(&DeviceType) < 0 ||
        PyType_Ready(&DeviceConfigurationType) < 0 ||
        PyType_Ready(&CaptureType) < 0 ||
        PyType_Ready(&ImageType) < 0
    )
    {
        return false;
    }
    return true;
}

bool addTypes(PyObject *mod)
{
    Py_INCREF(&DeviceType);
    Py_INCREF(&DeviceConfigurationType);
    Py_INCREF(&CaptureType);
    Py_INCREF(&ImageType);
    if (PyModule_AddObject(mod, "Device", (PyObject *)&DeviceType) < 0 ||
        PyModule_AddObject(mod, "DeviceConfiguration", (PyObject *)&DeviceConfigurationType) < 0 ||
        PyModule_AddObject(mod, "Capture", (PyObject*) &CaptureType) < 0 ||
        PyModule_AddObject(mod, "Image", (PyObject*) &ImageType) < 0
    )
    {
        Py_DECREF(&DeviceType);
        Py_DECREF(&DeviceConfigurationType);
        Py_DECREF(&CaptureType);
        Py_DECREF(&ImageType);
        return false;
    }
    return true;
}

void registerEnums(PyObject *mod)
{
    // Generated by extract_enums.py
    PyModule_AddObject(mod, "K4A_RESULT_SUCCEEDED", PyLong_FromLong(K4A_RESULT_SUCCEEDED));
    PyModule_AddObject(mod, "K4A_RESULT_FAILED", PyLong_FromLong(K4A_RESULT_FAILED));
    PyModule_AddObject(mod, "K4A_BUFFER_RESULT_SUCCEEDED", PyLong_FromLong(K4A_BUFFER_RESULT_SUCCEEDED));
    PyModule_AddObject(mod, "K4A_BUFFER_RESULT_FAILED", PyLong_FromLong(K4A_BUFFER_RESULT_FAILED));
    PyModule_AddObject(mod, "K4A_BUFFER_RESULT_TOO_SMALL", PyLong_FromLong(K4A_BUFFER_RESULT_TOO_SMALL));
    PyModule_AddObject(mod, "K4A_WAIT_RESULT_SUCCEEDED", PyLong_FromLong(K4A_WAIT_RESULT_SUCCEEDED));
    PyModule_AddObject(mod, "K4A_WAIT_RESULT_FAILED", PyLong_FromLong(K4A_WAIT_RESULT_FAILED));
    PyModule_AddObject(mod, "K4A_WAIT_RESULT_TIMEOUT", PyLong_FromLong(K4A_WAIT_RESULT_TIMEOUT));
    PyModule_AddObject(mod, "K4A_LOG_LEVEL_CRITICAL", PyLong_FromLong(K4A_LOG_LEVEL_CRITICAL));
    PyModule_AddObject(mod, "K4A_LOG_LEVEL_ERROR", PyLong_FromLong(K4A_LOG_LEVEL_ERROR));
    PyModule_AddObject(mod, "K4A_LOG_LEVEL_WARNING", PyLong_FromLong(K4A_LOG_LEVEL_WARNING));
    PyModule_AddObject(mod, "K4A_LOG_LEVEL_INFO", PyLong_FromLong(K4A_LOG_LEVEL_INFO));
    PyModule_AddObject(mod, "K4A_LOG_LEVEL_TRACE", PyLong_FromLong(K4A_LOG_LEVEL_TRACE));
    PyModule_AddObject(mod, "K4A_LOG_LEVEL_OFF", PyLong_FromLong(K4A_LOG_LEVEL_OFF));
    PyModule_AddObject(mod, "K4A_DEPTH_MODE_OFF", PyLong_FromLong(K4A_DEPTH_MODE_OFF));
    PyModule_AddObject(mod, "K4A_DEPTH_MODE_NFOV_2X2BINNED", PyLong_FromLong(K4A_DEPTH_MODE_NFOV_2X2BINNED));
    PyModule_AddObject(mod, "K4A_DEPTH_MODE_NFOV_UNBINNED", PyLong_FromLong(K4A_DEPTH_MODE_NFOV_UNBINNED));
    PyModule_AddObject(mod, "K4A_DEPTH_MODE_WFOV_2X2BINNED", PyLong_FromLong(K4A_DEPTH_MODE_WFOV_2X2BINNED));
    PyModule_AddObject(mod, "K4A_DEPTH_MODE_WFOV_UNBINNED", PyLong_FromLong(K4A_DEPTH_MODE_WFOV_UNBINNED));
    PyModule_AddObject(mod, "K4A_DEPTH_MODE_PASSIVE_IR", PyLong_FromLong(K4A_DEPTH_MODE_PASSIVE_IR));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_OFF", PyLong_FromLong(K4A_COLOR_RESOLUTION_OFF));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_720P", PyLong_FromLong(K4A_COLOR_RESOLUTION_720P));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_1080P", PyLong_FromLong(K4A_COLOR_RESOLUTION_1080P));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_1440P", PyLong_FromLong(K4A_COLOR_RESOLUTION_1440P));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_1536P", PyLong_FromLong(K4A_COLOR_RESOLUTION_1536P));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_2160P", PyLong_FromLong(K4A_COLOR_RESOLUTION_2160P));
    PyModule_AddObject(mod, "K4A_COLOR_RESOLUTION_3072P", PyLong_FromLong(K4A_COLOR_RESOLUTION_3072P));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_COLOR_MJPG", PyLong_FromLong(K4A_IMAGE_FORMAT_COLOR_MJPG));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_COLOR_NV12", PyLong_FromLong(K4A_IMAGE_FORMAT_COLOR_NV12));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_COLOR_YUY2", PyLong_FromLong(K4A_IMAGE_FORMAT_COLOR_YUY2));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_COLOR_BGRA32", PyLong_FromLong(K4A_IMAGE_FORMAT_COLOR_BGRA32));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_DEPTH16", PyLong_FromLong(K4A_IMAGE_FORMAT_DEPTH16));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_IR16", PyLong_FromLong(K4A_IMAGE_FORMAT_IR16));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_CUSTOM8", PyLong_FromLong(K4A_IMAGE_FORMAT_CUSTOM8));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_CUSTOM16", PyLong_FromLong(K4A_IMAGE_FORMAT_CUSTOM16));
    PyModule_AddObject(mod, "K4A_IMAGE_FORMAT_CUSTOM", PyLong_FromLong(K4A_IMAGE_FORMAT_CUSTOM));
    PyModule_AddObject(mod, "K4A_TRANSFORMATION_INTERPOLATION_TYPE_NEAREST", PyLong_FromLong(K4A_TRANSFORMATION_INTERPOLATION_TYPE_NEAREST));
    PyModule_AddObject(mod, "K4A_TRANSFORMATION_INTERPOLATION_TYPE_LINEAR", PyLong_FromLong(K4A_TRANSFORMATION_INTERPOLATION_TYPE_LINEAR));
    PyModule_AddObject(mod, "K4A_FRAMES_PER_SECOND_5", PyLong_FromLong(K4A_FRAMES_PER_SECOND_5));
    PyModule_AddObject(mod, "K4A_FRAMES_PER_SECOND_15", PyLong_FromLong(K4A_FRAMES_PER_SECOND_15));
    PyModule_AddObject(mod, "K4A_FRAMES_PER_SECOND_30", PyLong_FromLong(K4A_FRAMES_PER_SECOND_30));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE", PyLong_FromLong(K4A_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_AUTO_EXPOSURE_PRIORITY", PyLong_FromLong(K4A_COLOR_CONTROL_AUTO_EXPOSURE_PRIORITY));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_BRIGHTNESS", PyLong_FromLong(K4A_COLOR_CONTROL_BRIGHTNESS));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_CONTRAST", PyLong_FromLong(K4A_COLOR_CONTROL_CONTRAST));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_SATURATION", PyLong_FromLong(K4A_COLOR_CONTROL_SATURATION));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_SHARPNESS", PyLong_FromLong(K4A_COLOR_CONTROL_SHARPNESS));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_WHITEBALANCE", PyLong_FromLong(K4A_COLOR_CONTROL_WHITEBALANCE));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_BACKLIGHT_COMPENSATION", PyLong_FromLong(K4A_COLOR_CONTROL_BACKLIGHT_COMPENSATION));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_GAIN", PyLong_FromLong(K4A_COLOR_CONTROL_GAIN));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_POWERLINE_FREQUENCY", PyLong_FromLong(K4A_COLOR_CONTROL_POWERLINE_FREQUENCY));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_AUTO", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_AUTO));
    PyModule_AddObject(mod, "K4A_COLOR_CONTROL_MODE_MANUAL", PyLong_FromLong(K4A_COLOR_CONTROL_MODE_MANUAL));
    PyModule_AddObject(mod, "K4A_WIRED_SYNC_MODE_STANDALONE", PyLong_FromLong(K4A_WIRED_SYNC_MODE_STANDALONE));
    PyModule_AddObject(mod, "K4A_WIRED_SYNC_MODE_MASTER", PyLong_FromLong(K4A_WIRED_SYNC_MODE_MASTER));
    PyModule_AddObject(mod, "K4A_WIRED_SYNC_MODE_SUBORDINATE", PyLong_FromLong(K4A_WIRED_SYNC_MODE_SUBORDINATE));
    PyModule_AddObject(mod, "K4A_CALIBRATION_TYPE_UNKNOWN", PyLong_FromLong(K4A_CALIBRATION_TYPE_UNKNOWN));
    PyModule_AddObject(mod, "K4A_CALIBRATION_TYPE_DEPTH", PyLong_FromLong(K4A_CALIBRATION_TYPE_DEPTH));
    PyModule_AddObject(mod, "K4A_CALIBRATION_TYPE_COLOR", PyLong_FromLong(K4A_CALIBRATION_TYPE_COLOR));
    PyModule_AddObject(mod, "K4A_CALIBRATION_TYPE_GYRO", PyLong_FromLong(K4A_CALIBRATION_TYPE_GYRO));
    PyModule_AddObject(mod, "K4A_CALIBRATION_TYPE_ACCEL", PyLong_FromLong(K4A_CALIBRATION_TYPE_ACCEL));
    PyModule_AddObject(mod, "K4A_CALIBRATION_TYPE_NUM", PyLong_FromLong(K4A_CALIBRATION_TYPE_NUM));
    PyModule_AddObject(mod, "K4A_CALIBRATION_LENS_DISTORTION_MODEL_UNKNOWN", PyLong_FromLong(K4A_CALIBRATION_LENS_DISTORTION_MODEL_UNKNOWN));
    PyModule_AddObject(mod, "K4A_CALIBRATION_LENS_DISTORTION_MODEL_THETA", PyLong_FromLong(K4A_CALIBRATION_LENS_DISTORTION_MODEL_THETA));
    PyModule_AddObject(mod, "K4A_CALIBRATION_LENS_DISTORTION_MODEL_POLYNOMIAL_3K", PyLong_FromLong(K4A_CALIBRATION_LENS_DISTORTION_MODEL_POLYNOMIAL_3K));
    PyModule_AddObject(mod, "K4A_CALIBRATION_LENS_DISTORTION_MODEL_RATIONAL_6KT", PyLong_FromLong(K4A_CALIBRATION_LENS_DISTORTION_MODEL_RATIONAL_6KT));
    PyModule_AddObject(mod, "K4A_CALIBRATION_LENS_DISTORTION_MODEL_BROWN_CONRADY", PyLong_FromLong(K4A_CALIBRATION_LENS_DISTORTION_MODEL_BROWN_CONRADY));
    PyModule_AddObject(mod, "K4A_FIRMWARE_BUILD_RELEASE", PyLong_FromLong(K4A_FIRMWARE_BUILD_RELEASE));
    PyModule_AddObject(mod, "K4A_FIRMWARE_BUILD_DEBUG", PyLong_FromLong(K4A_FIRMWARE_BUILD_DEBUG));
    PyModule_AddObject(mod, "K4A_FIRMWARE_SIGNATURE_MSFT", PyLong_FromLong(K4A_FIRMWARE_SIGNATURE_MSFT));
    PyModule_AddObject(mod, "K4A_FIRMWARE_SIGNATURE_TEST", PyLong_FromLong(K4A_FIRMWARE_SIGNATURE_TEST));
    PyModule_AddObject(mod, "K4A_FIRMWARE_SIGNATURE_UNSIGNED", PyLong_FromLong(K4A_FIRMWARE_SIGNATURE_UNSIGNED));
}
