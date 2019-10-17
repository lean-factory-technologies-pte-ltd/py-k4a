#include "image.h"

PyObject* py_image_get_buffer(PyObject* self, PyObject* args)
{
    ImageObject* obj;
    PyArg_ParseTuple(args, "O", &obj);

    uint8_t* buffer = k4a_image_get_buffer(obj->image);
    size_t size = k4a_image_get_size(obj->image);

    return PyByteArray_FromStringAndSize((const char*) buffer, size);
}

PyObject* py_image_get_width_pixels(PyObject* self, PyObject* args)
{
    ImageObject* obj;
    PyArg_ParseTuple(args, "O", &obj);

    return PyLong_FromLong(k4a_image_get_width_pixels(obj->image));
}

PyObject* py_image_get_height_pixels(PyObject* self, PyObject* args)
{
    ImageObject* obj;
    PyArg_ParseTuple(args, "O", &obj);

    return PyLong_FromLong(k4a_image_get_height_pixels(obj->image));
}

PyObject* py_image_get_stride_bytes(PyObject* self, PyObject* args)
{
    ImageObject* obj;
    PyArg_ParseTuple(args, "O", &obj);

    return PyLong_FromLong(k4a_image_get_stride_bytes(obj->image));
}


PyObject* image_create(PyObject* self, PyObject* args)
{
  ImageObject* obj;
  int width, height, strides;
  k4a_image_format_t format;

  PyArg_ParseTuple(args, "iiiiO", &format, &width, &height, &strides, &obj);
  return PyLong_FromLong(k4a_image_create(format, width, height, strides, &obj->image));
}

PyObject* image_to_array(PyObject* self, PyObject* args)
{
  ImageObject* obj;
  int width;
  int height;
  int strides;

  PyArg_ParseTuple(args, "O", &obj);

  height = k4a_image_get_height_pixels(obj->image);
  width = k4a_image_get_width_pixels(obj->image);
  strides = k4a_image_get_stride_bytes(obj->image);

  PyObject* row = Py_BuildValue("[]");
  uint16_t *buf = (uint16_t *)(void *)k4a_image_get_buffer(obj->image);
  for (int i = 0; i < height; i++) {
    PyObject* line = Py_BuildValue("[]");
    for (int j = 0; j < width; j++) {
      int idx = (width * i) + j;
      int data;
      data = buf[idx];
      if (buf[idx] > 255) {
        data = 255;
      }
      PyList_Append(line, Py_BuildValue("i", data));
    }
    PyList_Append(row, line);
  }

  return row;
}
