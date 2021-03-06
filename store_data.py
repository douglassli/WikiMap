import pandas
import ast


def initialize_csv(file_name):
    df = pandas.DataFrame()
    df.to_csv(file_name, index=False, header=False)


def read_partial(file_name, cols):
    df = pandas.read_csv(file_name, usecols=cols, names=["Page Title", "Page URL", "Out-link Titles", "Out-link URLs",
                                                         "Total Time", "Get Time", "Parse Time", "Analysis Time",
                                                         "Page Depth", "Time of Expansion"])
    return df


def read_partial_section(file_name, cols, num_rows, rows_to_skip):
    df = pandas.read_csv(file_name, usecols=cols, nrows=num_rows, skiprows=rows_to_skip,
                         names=["Page Title", "Page URL", "Out-link Titles", "Out-link URLs",
                                "Total Time", "Get Time", "Parse Time", "Analysis Time",
                                "Page Depth", "Time of Expansion"])
    return df


def get_num_per_depth(file_name):
    num_read = 0
    num_per_depth_dict = {}

    while True:
        df = read_partial_section(file_name, ["Page Depth"], 1000000, num_read)
        depths = list(df["Page Depth"])

        if len(depths) == 0:
            break

        num_read += len(depths)
        for d in depths:
            if d not in num_per_depth_dict.keys():
                num_per_depth_dict[d] = 0
            num_per_depth_dict[d] += 1

    return num_per_depth_dict


def get_branching_analytics(file_name):
    num_read = 0
    total_branches = 0
    max_branch = 0
    branch_list = []

    while True:
        df = read_partial_section(file_name, ["Out-link Titles"], 10000, num_read)
        out_links = [ast.literal_eval(i) for i in list(df["Out-link Titles"])]

        if len(out_links) == 0:
            break

        num_read += len(out_links)
        for links in out_links:
            branching_factor = len(links)
            total_branches += branching_factor
            branch_list.append(branching_factor)
            if branching_factor > max_branch:
                max_branch = branching_factor

    branching_analytics = {"avg_branch": total_branches / float(num_read),
                           "max_branch": max_branch, "branch_list": branch_list}
    return branching_analytics


def get_timing_analytics(file_name):
    total_time = 0
    total_get_time = 0
    total_parse_time = 0
    total_analysis_time = 0
    max_time = 0
    num_read = 0

    while True:
        df = read_partial_section(file_name, ["Total Time", "Get Time", "Parse Time", "Analysis Time"], 1000000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)
        for tpl in tpls:
            page_total_time = tpl[0]
            total_time += page_total_time
            total_get_time += tpl[1]
            total_parse_time += tpl[2]
            total_analysis_time += tpl[3]
            if page_total_time > max_time:
                max_time = page_total_time

    time_analytics = {"total_time": total_time,
                      "max_time": max_time,
                      "avg_time": total_time / num_read,
                      "avg_get": total_get_time / num_read,
                      "avg_parse": total_parse_time / num_read,
                      "avg_analysis": total_analysis_time / num_read,
                      "percent_get": total_get_time / total_time,
                      "percent_parse": total_parse_time / total_time,
                      "percent_analysis": total_analysis_time / total_time}

    return time_analytics


def get_analytics(file_name):
    analytics = get_branching_analytics(file_name)
    timing = get_timing_analytics(file_name)
    for key in timing.keys():
        analytics[key] = timing[key]

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


def get_analytics_string(file_name):
    analytic_strings = get_analytics_strings(file_name)
    out_put_format = "{0[total_expanded]}\n{0[depth_nums]}\n{0[avg_branch]}\n{0[max_branch]}\n\n{0[total_time]}\n" \
                     "{0[max_time]}\n{0[avg_time]}\n{0[avg_get]}\n{0[avg_parse]}\n{0[avg_analysis]}\n\n" \
                     "{0[percent_get]}\n{0[percent_parse]}\n{0[percent_analysis]}"
    return out_put_format.format(analytic_strings)


def append_map_to_csv(map_list, file_name):
    df = pandas.DataFrame(map_list)
    df.to_csv(file_name, index=False, mode="a", header=False)
