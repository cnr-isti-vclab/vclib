from dumper import *

def get_ith_eigen_value(d, value, i):
    innerType = value.type[0]
    argRow = value.type[1]
    argCol = value.type[2]
    if argCol == 10000 and argRow == 10000:
        argCol = -1
        argRow = -1
    if argCol != -1 and argRow != -1:
        nrows = argRow
        ncols = argCol
        p = value.address()
    else:
        storage = value['m_storage']
        nrows = storage['m_rows'].integer() if argRow == -1 else argRow
        ncols = storage['m_cols'].integer() if argCol == -1 else argCol
        p = storage['m_data'].pointer()

    innerSize = innerType.size()
    storage = value['m_storage']
    p = value.address()
    v = d.createValue(p + i * innerSize, innerType)
    return v
