#include <Python.h>
#include <k4a/k4a.h>

PyObject* device_get_installed_count(PyObject* self, PyObject* args);
PyObject* device_get_serial_num(PyObject* self, PyObject* args);
PyObject* device_open(PyObject* self, PyObject* args);
PyObject* device_get_version(PyObject* self, PyObject* args);
PyObject* device_close(PyObject* self, PyObject* args);
PyObject* device_start_cameras(PyObject* self, PyObject* args);
PyObject* device_stop_cameras(PyObject* self, PyObject* args);

// Device color control
PyObject* device_get_color_control(PyObject* self, PyObject* args);
PyObject* device_set_color_control(PyObject* self, PyObject* args);

// Device calibration
PyObject* device_get_calibration(PyObject* self, PyObject* args);
