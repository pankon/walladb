"""
Python Api

TODO: https://dbader.org/blog/python-ctypes-tutorial-part-2
	  https://www.sysleaf.com/nodejs-ffi/
"""

from ctypes import *
lib = cdll.LoadLibrary('./libwalladb.so')

class WallaDbServer(object):
	def __init__(self):
		self.obj = lib.WallaDbCreateServer()

class WallaPos(object):
	def __init__(self, x, y, z):
		c_x = c_long(x)
		c_y = c_long(y)
		c_z = c_long(z)
		self.obj = lib.WallaPosCreate(c_x, c_y, c_z)

	def free(self):
		lib.WallaPosDestroy(self.obj)

class WallaNode(object):
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
node = WallaNode(None, 0, 1, pos)
print node