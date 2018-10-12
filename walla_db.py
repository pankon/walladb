"""
Python Api

TODO: https://dbader.org/blog/python-ctypes-tutorial-part-2
	  https://www.sysleaf.com/nodejs-ffi/
todo: json update, write main functions for server, sensor, analysis
      communicate via shared files
"""

from ctypes import *
from ctypes.util import find_library

libwalladb = cdll.LoadLibrary('./libwalladb.so')
libc = CDLL(find_library('c'))

libc.free.argtypes = (c_void_p,)

def wrap_function(lib, funcname, restype, argtypes):
    """Simplify wrapping ctypes functions
    source: https://dbader.org/blog/python-ctypes-tutorial-part-2
    """
    func = lib.__getattr__(funcname)
    func.restype = restype
    func.argtypes = argtypes
    return func

class WallaDbServer(Structure):
	def __init__(self):
		self.obj = lib.WallaDbCreateServer()

class WallaPos(Structure):
	_fields_ = [('x', c_long), ('y', c_long), ('z', c_long)]

	def __repr__(self):
		return '({0}, {1}, {2})'.format(self.x, self.y, self.z)

class WallaNodeInfo(Structure):
	_fields_ = [('pos', WallaPos), ('epoch_start', c_ulong), ('epoch_end', c_ulong), 
				('average', c_double), ('max', c_double), ('min', c_double), ('stdev', c_double)]

	def __init__(self, pos):
		self.pos = pos
		self.epoch_start = 0
		self.epoch_end = 0
		self.average = 0
		self.max = 0
		self.min = 0
		self.stdev = 0
		self.to_json = wrap_function(libwalladb, 'WallaNodeInfoToJson', c_char_p, [WallaNodeInfo])

	def __repr__(self):
		_result = self.to_json(self)
		result = _result.value
		print result
		libc.free(_result)
		return result


class WallaNode(Structure):
	def __init__(self, parent, depth, n_entries, pos):
		c_parent = POINTER(parent.obj) if parent else None
		c_depth = c_int(depth)
		c_n_entries = c_int(n_entries)
		c_pos = POINTER(pos.obj) if pos else None
		self.obj = lib.WallaNodeCreate(c_parent, c_depth, c_n_entries, c_pos)

	def free(self):
		lib.WallaNodeDestroy(self.obj)

	def __str__(self):
		return str(lib.WallaNodeToJson(self.obj))

pos = WallaPos(1, 2, 3)
#node = WallaNode(None, 0, 1, pos)
#print node
info = WallaNodeInfo(pos)
print info