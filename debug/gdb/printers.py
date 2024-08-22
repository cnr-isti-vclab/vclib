# pretty print for debug

import gdb
import sys
import os

# get file directory
dir_path = os.path.dirname(os.path.realpath(__file__))

sys.path.insert(0, dir_path + '/../../external/eigen-3.4.0/debug/gdb')
from printers import register_eigen_printers
register_eigen_printers(None)


class BitSetPrinter:
    def __init__(self, val):
        self.val = val

    def to_string (self):
        # the size of the type of mBits
        size = self.val['mBits'].type.sizeof * 8
        return "BitSet of size " + str(size)
    
    def children (self):
        # display the bitset as a vector of bools
        # each bool is a bit in the bitset
        # that is the ith bit of the mBits value
        mBits = self.val['mBits']
        size = mBits.type.sizeof * 8
        retList = []
        for i in range(size):
            bit = mBits & (1 << i)
            retList.append(('[%d]' % i, bit != 0))
        return retList


def lookup_type (val):
    if 'vcl::BitSet' in str(val.type):
        return BitSetPrinter(val)
    return None

gdb.pretty_printers.append(lookup_type)