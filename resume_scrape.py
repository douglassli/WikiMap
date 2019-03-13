import pandas
import store_data
import ast
import time


def get_keys(file_name):
    num_read = 0
    keys_set = set()

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title"], 1000000, num_read)
        titles = list(df["Page Title"])

        if len(titles) == 0:
            break

        num_read += len(titles)

        for title in titles:
            keys_set.add(title)

    return keys_set


def find_max_depth(file_name):
    num_read = 0
    max_depth = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Depth"], 1000000, num_read)
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

    max_depth = find_max_depth(file_name)

    while True:
        df = store_data.read_partial_section(file_name, ["Depth"], 1000000, num_read)
        depths = list(df["Depth"])

        if len(depths) == 0:
            break

        num_read += len(depths)

        for d in depths:
            if d < max_depth - 1:
                num_read_frontier += 1

    return num_read_frontier


if __name__ == '__main__':
    start = time.time()
    get_keys("Data Sets/Small3 Test 1/small3_full_output.csv")
    total = time.time() - start
    print("Time: {0:.1f}".format(total))
