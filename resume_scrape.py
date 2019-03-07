import pandas
import store_data
import ast


def get_num_read_frontier(file_name):
    num_read = 0
    num_read_frontier = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Depth"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)
