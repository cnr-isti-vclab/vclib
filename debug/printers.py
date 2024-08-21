# pretty print for debug

import gdb
import sys
import os

# get file directory
dir_path = os.path.dirname(os.path.realpath(__file__))

sys.path.insert(0, dir_path + '/../external/eigen-3.4.0/debug/gdb')
from printers import register_eigen_printers
register_eigen_printers(None)