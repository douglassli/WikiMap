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


def get_analytics_strings(file_name):
    analytics = get_analytics(file_name)
    analytic_format_strings = {"total_expanded": "Total Pages Expanded:       {:d}",
                               "depth_nums": "Num Pages Depth {0:3d}:        {1:d}",
                               "avg_branch": "Average Branching Factor:   {0:5.1f}",
                               "max_branch": "Maximum Branching Factor:   {:d}",
                               "total_time": "Total Time Expanding Pages: {:.1f} sec",
                               "max_time": "Maximum Time on Page:       {:6.4f} sec",
                               "avg_time": "Average Time Per Page:      {:6.4f} sec",
                               "avg_get": "Average Get Time:           {:6.4f} sec",
                               "avg_parse": "Average Parse Time:         {:6.4f} sec",
                               "avg_analysis": "Average Analysis Time:      {:6.4f} sec",
                               "percent_get": "Time Spent Getting:         {0:4.1%}",
                               "percent_parse": "Time Spent Parsing:         {0:4.1%}",
                               "percent_analysis": "Time Spent Analysing:       {0:4.1%}"}
    for key in analytic_format_strings.keys():
        if key == "total_expanded":
            total = 0
            for depth_key in analytics["depth_nums"].keys():
                total += analytics["depth_nums"][depth_key]
            analytic_format_strings[key] = analytic_format_strings[key].format(total)
        elif key == "depth_nums":
            depth_str = ""
            for depth_key in analytics[key].keys():
                depth_str += analytic_format_strings[key].format(depth_key, analytics[key][depth_key]) + "\n"
            analytic_format_strings[key] = depth_str
        else:
                analytic_format_strings[key] = analytic_format_strings[key].format(analytics[key])

    return analytic_format_strings


def print_analytics(file_name):
    analytic_strings = get_analytics_strings(file_name)
    out_put_format = "{0[total_expanded]}\n{0[depth_nums]}\n{0[avg_branch]}\n{0[max_branch]}\n\n{0[total_time]}\n" \
                     "{0[max_time]}\n{0[avg_time]}\n{0[avg_get]}\n{0[avg_parse]}\n{0[avg_analysis]}\n\n" \
                     "{0[percent_get]}\n{0[percent_parse]}\n{0[percent_analysis]}"
    print(out_put_format.format(analytic_strings))


def append_to_frontier(page_list, file_name):
    df = pandas.DataFrame(page_list)
    df.to_csv(file_name, index=False, mode="a", header=False)


def read_some_frontier(file_name, rows_to_skip):
    df = pandas.read_csv(file_name, nrows=1000, skiprows=rows_to_skip, names=["Page", "Depth"])
    tpls = list(df.itertuples(index=False, name=None))
    pages = [(ast.literal_eval(i[0]), i[1]) for i in tpls]
    return pages


def append_single_page(page, depth, file_name):
    data = [prepare_page_data(page, depth)]
    df = pandas.DataFrame(data)
    df.to_csv(file_name, index=False, mode="a", header=False)


def append_map_to_csv(map_dict_tree, file_name):
    data_list = []
    for depth_key in map_dict_tree.keys():
        for page_key in map_dict_tree[depth_key].keys():
            data_list.append(prepare_page_data(map_dict_tree[depth_key][page_key], depth_key))

    df = pandas.DataFrame(data_list)
    df.to_csv(file_name, index=False, mode="a", header=False)
