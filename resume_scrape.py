import store_data
import time
import hashlib
import sys
import ast
import pandas


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
            keys_set.add(hashlib.md5(str(title).encode()).digest())

    return keys_set


def find_num_read_frontier2(file_name):
    keys = get_keys(file_name)
    num_read = 0
    num_read_frontier = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 100000, num_read)
        links = [(ll[0], ast.literal_eval(ll[1])) for ll in list(df.itertuples(index=False, name=None))]

        if len(links) == 0:
            break

        num_read += len(links)

        for tpl in links:
            for link in tpl[1]:
                if hashlib.md5(str(link).encode()).digest() not in keys:
                    print("Page not in keys: ", link)
                    print("Page containing unexpanded node: ", tpl[0])
                    print(tpl[1])
                    return num_read_frontier
            num_read_frontier += 1
            print(num_read_frontier)

    raise ValueError("MAPPING IS COMPLETE, ALL OUT LINKS ACCOUNTED FOR")


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


def append_to_master(master_file, output_file):
    keys = get_keys(master_file)
    num_read = 0

    while True:
        df = pandas.read_csv(output_file, nrows=50000, skiprows=num_read,
                             names=["Page Title", "Page URL", "Out-link Titles", "Out-link URLs",
                                    "Total Time", "Get Time", "Parse Time", "Analysis Time",
                                    "Page Depth", "Time of Expansion"])
        tpls = list(df.itertuples(index=False, name=None))
        new_tpls = []

        if len(tpls) == 0:
            break
        num_read += len(tpls)

        for tpl in tpls:
            hash_title = hashlib.md5(tpl[0].encode()).digest()
            if hash_title not in keys:
                keys.add(hash_title)
                new_tpls.append(tpl)

        store_data.append_map_to_csv(new_tpls, master_file)


if __name__ == '__main__':
    start = time.time()
    num_from_frontier = find_num_read_frontier2("Data Sets/Master Scrape/master_output.csv")
    print("Num read from frontier: ", num_from_frontier)
    total = time.time() - start
    print("Time: {0:.1f}".format(total))
