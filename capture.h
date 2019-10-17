#include <Python.h>
#include <structmember.h>
#include <k4a/k4a.h>

#include "types.h"

#ifndef __PY_K4A_CAPTURE__
#define __PY_K4A_CAPTURE__

PyObject* device_get_capture(PyObject* self, PyObject* args);
PyObject* py_capture_get_depth_image(PyObject* self, PyObject* args);
PyObject* py_capture_get_color_image(PyObject* self, PyObject* args);
#endif
