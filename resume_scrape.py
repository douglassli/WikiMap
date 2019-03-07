import pandas
import store_data
import ast


def find_max_depth(file_name):
    num_read = 0
    max_depth = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Depth"], 1000, num_read)
        depths = list(df["Depth"])

        if len(depths) == 0:
            break

        num_read += len(depths)

        max_d = max(depths)
        if max_d > max_depth:
            max_depth = max_d

    return max_depth


def get_num_read_frontier(file_name):
    num_read = 0
    num_read_frontier = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Depth"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)
