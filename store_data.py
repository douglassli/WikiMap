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


def get_branching_analytics(file_name):
    df = read_partial(file_name, ["Out-link Titles"])
    num_rows = get_file_length(file_name)
    total_branches = 0
    max_branch = 0
    for i in range(num_rows):
        branching_factor = len(ast.literal_eval(df["Out-link Titles"][i]))
        total_branches += branching_factor
        if branching_factor > max_branch:
            max_branch = branching_factor

    branching_analytics = {"avg_branch": total_branches / float(num_rows), "max_branch": max_branch}
    return branching_analytics


def get_timing_analytics(file_name):
    df = read_partial(file_name, ["Total Time", "Get Time", "Parse Time", "Analysis Time"])
    num_rows = get_file_length(file_name)
    total_time = 0
    total_get_time = 0
    total_parse_time = 0
    total_analysis_time = 0
    max_time = 0
    for i in range(num_rows):
        page_total_time = df["Total Time"][i]
        total_time += page_total_time
        total_get_time += df["Get Time"][i]
        total_parse_time += df["Parse Time"][i]
        total_analysis_time += df["Analysis Time"][i]
        if page_total_time > max_time:
            max_time = page_total_time

    time_analytics = {"total_time": total_time,
                      "max_time": max_time,
                      "avg_time": total_time / num_rows,
                      "avg_get": total_get_time / num_rows,
                      "avg_parse": total_parse_time / num_rows,
                      "avg_analysis": total_analysis_time / num_rows,
                      "percent_get": total_get_time / total_time,
                      "percent_parse": total_parse_time / total_time,
                      "percent_analysis": total_analysis_time / total_time}

    return time_analytics


def get_analytics(file_name):
    analytics = {**get_branching_analytics(file_name), **get_timing_analytics(file_name)}
    analytics["depth_nums"] = get_num_per_depth(file_name)
    return analytics
