import matplotlib.pyplot as plt
import store_data
import ast


def seconds_from_date(date_string):
    hours = float(date_string[11:13])
    minutes = float(date_string[14:16])
    seconds = float(date_string[17:])
    total_secs = (3600 * hours) + (60 * minutes) + seconds
    return total_secs


def get_average_per_hund_sec(get_list, time_list):
    gets_dict = {}
    for i, time in enumerate(time_list):
        if round(time, -1) not in gets_dict:
            gets_dict[round(time, -1)] = []
        gets_dict[round(time, -1)].append(get_list[i])

    avg_dict = {}
    for key in gets_dict.keys():
        avg_dict[key] = sum(gets_dict[key]) / len(gets_dict[key])

    return avg_dict


def get_num_out_list(file_name):
    num_read = 0
    num_out_list = []
    while True:
        df = store_data.read_partial_section(file_name, ["Out-link Titles"], 1000, num_read)
        temp_list = [len(ast.literal_eval(i)) for i in list(df["Out-link Titles"])]
        if len(temp_list) == 0:
            break
        else:
            num_read += len(temp_list)
            num_out_list += temp_list
    return num_out_list


def plot_get_vs_time(file_name):
    df = store_data.read_partial(file_name, ["Get Time", "Time of Expansion"])
    get_list = list(df["Get Time"])
    time_list = [seconds_from_date(i) for i in list(df["Time of Expansion"])]
    avg_dict = get_average_per_hund_sec(get_list, time_list)
    plt.plot(avg_dict.keys(), avg_dict.values(), color="red", marker="o", markersize=0.3, linewidth=0)
    plt.ylabel("Get Time (sec)")
    plt.xlabel("Time of Get (sec)")
    plt.show()


def plot_out_vs_parse_time(file_name):
    df = store_data.read_partial(file_name, ["Out-link Titles", "Parse Time"])
    parse_list = list(df["Parse Time"])
    num_out_list = [len(ast.literal_eval(i)) for i in list(df["Out-link Titles"])]
    df = None
    plt.plot(num_out_list, parse_list, color="red", marker="o", markersize=0.3, linewidth=0)
    plt.ylabel("Parse Time (sec)")
    plt.xlabel("Number of Out Links")
    plt.show()


def plot_out_vs_analysis_time(file_name):
    df = store_data.read_partial(file_name, ["Out-link Titles", "Analysis Time"])
    parse_list = list(df["Analysis Time"])
    num_out_list = [len(ast.literal_eval(i)) for i in list(df["Out-link Titles"])]
    df = None
    plt.plot(num_out_list, parse_list, color="red", marker="o", markersize=0.3, linewidth=0)
    plt.ylabel("Analysis Time (sec)")
    plt.xlabel("Number of Out Links")
    plt.show()


if __name__ == '__main__':
    plot_out_vs_analysis_time("small3_full_output.csv")
