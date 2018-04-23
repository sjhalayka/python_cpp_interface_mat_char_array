import ctypes as ct
import numpy as np
import cv2

ptr = ct.c_char_p()
s = "dove.png"
width = ct.pointer(ct.c_int())
height = ct.pointer(ct.c_int())
img_id = ct.pointer(ct.c_size_t())
lib = ct.CDLL("void_dll.dll")
alloc_img = lib.alloc_img
free_img = lib.free_img
get_img_from_char_array = lib.get_img_from_char_array

alloc_img.restype = ct.c_char_p

bytes_array = alloc_img(ct.c_char_p(s.encode("utf-8")), width, height, img_id)
numpy_array = np.frombuffer(bytes_array, dtype=np.uint8)
numpy_array.shape = (width.contents.value, height.contents.value)

cppbytes = numpy_array.tobytes()
get_img_from_char_array(cppbytes, width, height);

free_img(img_id)

