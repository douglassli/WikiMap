import matplotlib.pyplot as plt
import store_data


def seconds_from_date(date_string):
    hours = float(date_string[11:13])
    minutes = float(date_string[14:16])
    seconds = float(date_string[17:])
    total_secs = (3600 * hours) + (60 * minutes) + seconds
    return total_secs


def get_average_per_ten_sec(get_list, time_list):
    gets_dict = {}
    for i, time in enumerate(time_list):
        if round(time, -1) not in gets_dict:
            gets_dict[round(time, -1)] = []
        gets_dict[round(time, -1)].append(get_list[i])

    avg_dict = {}
    for key in gets_dict.keys():
        avg_dict[key] = sum(gets_dict[key]) / len(gets_dict[key])

    return avg_dict


def plot_get_vs_time(file_name):
    df = store_data.read_partial(file_name, ["Get Time", "Time of Expansion"])
    get_list = list(df["Get Time"])
    time_list = [seconds_from_date(i) for i in list(df["Time of Expansion"])]
    avg_dict = get_average_per_ten_sec(get_list, time_list)
    plt.plot(avg_dict.keys(), avg_dict.values())
    plt.ylabel("Get Time (sec)")
    plt.xlabel("Time of Get (sec)")
    plt.show()


if __name__ == '__main__':
    plot_get_vs_time("output.csv")
