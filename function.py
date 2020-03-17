import os
import ctypes

_sum = ctypes.CDLL(os.path.join(os.getcwd(), 'lib.so'))
_sum.myFunction.argtypes = (ctypes.c_int, ctypes.POINTER(ctypes.c_int))

def my_function(numbers):
    global _sum
    num_numbers = len(numbers)
    array_type = ctypes.c_int * num_numbers
    result = _sum.myFunction(ctypes.c_int(num_numbers), array_type(*numbers))
    return int(result)

print(my_function([5, 4, -3]))
