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
        df = store_data.read_partial_section(file_name, ["Out-link Titles"], 100000, num_read)
        temp_list = [len(ast.literal_eval(i)) + 1 for i in list(df["Out-link Titles"])]
        if len(temp_list) == 0:
            break
        else:
            num_read += len(temp_list)
            num_out_list += temp_list
            print(num_read)
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


def plot_out_vs(file_name, timing_name):
    df = store_data.read_partial(file_name, [timing_name])
    parse_list = list(df[timing_name])
    num_out_list = get_num_out_list(file_name)
    plt.plot(num_out_list, parse_list, color="red", marker="o", markersize=0.3, linewidth=0)
    plt.ylabel(timing_name + " (sec)")
    plt.xlabel("Number of Out Links")
    plt.show()


def outlink_hist(file_name):
    branching_list = store_data.get_branching_analytics(file_name)["branching_list"]
    plt.hist(branching_list)


if __name__ == '__main__':
    outlink_hist("Data Sets/small1_output.csv")
