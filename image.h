#include <Python.h>
#include <structmember.h>
#include <k4a/k4a.h>

#include "types.h"

#ifndef __PY_K4A_IMAGE__
#define __PY_K4A_IMAGE__

PyObject* image_create(PyObject* self, PyObject* args);
PyObject* py_image_get_buffer(PyObject* self, PyObject* args);
PyObject* py_image_get_width_pixels(PyObject* self, PyObject* args);
PyObject* py_image_get_height_pixels(PyObject* self, PyObject* args);
PyObject* py_image_get_stride_bytes(PyObject* self, PyObject* args);
PyObject* image_to_array(PyObject* self, PyObject* args);

#endif
