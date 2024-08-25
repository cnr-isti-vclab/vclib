from dumper import *


def get_eigen_matrix_data(value):
    inner_type = value.type[0]
    arg_row = value.type[1]
    arg_col = value.type[2]
    options = value.type[3]
    row_major = (int(options) & 0x1)
    if arg_col == 10000 and arg_row == 10000:
        arg_col = -1
        arg_row = -1
    if arg_col != -1 and arg_row != -1:
        nrows = arg_row
        ncols = arg_col
        p = value.address()
    else:
        storage = value['m_storage']
        nrows = storage['m_rows'].integer() if arg_row == -1 else arg_row
        ncols = storage['m_cols'].integer() if arg_col == -1 else arg_col
        p = storage['m_data'].pointer()

    # return a tuple containing, in order:
    # inner_type, nrows, ncols, row_major, p
    return inner_type, nrows, ncols, row_major, p


def get_ith_eigen_value(d, value, i):
    inner_type, nrows, ncols, row_major, p = get_eigen_matrix_data(value)
    inner_size = inner_type.size()

    v = d.createValue(p + i * inner_size, inner_type)
    return v


def get_list_eigen_values(d, value):
    inner_type, nrows, ncols, row_major, p = get_eigen_matrix_data(value)

    inner_size = inner_type.size()
    retList = []
    for i in range(nrows * ncols):
        v = d.createValue(p + i * inner_size, inner_type)
        retList.append(v)
    return retList
