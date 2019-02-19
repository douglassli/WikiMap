import pandas
import ast


def initialize_csv(file_name):
    df = pandas.DataFrame()
    df.to_csv(file_name, index=False, header=False)


def prepare_page_data(wiki_page, depth):
    return (wiki_page[0], wiki_page[1], wiki_page[2], wiki_page[3],
            wiki_page[4], wiki_page[5], wiki_page[6], wiki_page[7],
            depth, wiki_page[8])


def read_partial(file_name, cols):
    df = pandas.read_csv(file_name, usecols=cols, names=["Page Title", "Page URL", "Out-link Titles", "Out-link URLs",
                                                         "Total Time", "Get Time", "Parse Time", "Analysis Time",
                                                         "Page Depth", "Time of Expansion"])
    return df


def get_file_length(file_name):
    df = read_partial(file_name, ["Page Depth"])
    num_rows = len(df.index)
    return num_rows


def get_num_per_depth(file_name):
    df = read_partial(file_name, ["Page Depth"])
    num_rows = get_file_length(file_name)
    num_per_depth_dict = {}
    for i in range(num_rows):
        element = df["Page Depth"][i]
        if element not in num_per_depth_dict.keys():
            num_per_depth_dict[element] = 0
        num_per_depth_dict[element] += 1

    return num_per_depth_dict
