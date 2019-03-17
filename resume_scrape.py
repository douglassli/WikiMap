import store_data
import time
import hashlib
import sys


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
            try:
                keys_set.add(hashlib.md5(str(title).encode()).digest())
            except AttributeError:
                print(str(title))

    return keys_set


def find_max_depth(file_name):
    num_read = 0
    max_depth = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Page Depth"], 1000000, num_read)
        depths = list(df["Page Depth"])

        if len(depths) == 0:
            break

        num_read += len(depths)

        max_d = depths.pop()
        if max_d > max_depth:
            max_depth = max_d

    return max_depth


def get_num_read_frontier(file_name):
    num_read = 0
    num_read_frontier = 0

    max_depth = find_max_depth(file_name)

    while True:
        df = store_data.read_partial_section(file_name, ["Page Depth"], 1000000, num_read)
        depths = list(df["Page Depth"])

        if len(depths) == 0:
            break

        num_read += len(depths)

        for d in depths:
            if d < max_depth - 1:
                num_read_frontier += 1
            else:
                return num_read_frontier
    return num_read_frontier


if __name__ == '__main__':
    start = time.time()
    s = get_keys("Data Sets/Master Scrape/master_output.csv")
    print("Size: ", sys.getsizeof(s))
    total = time.time() - start
    print("Time: {0:.1f}".format(total))
