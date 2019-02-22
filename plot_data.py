import matplotlib.pyplot as plt
import store_data
import math


def seconds_from_date(date_string):
    hours = float(date_string[11:13])
    minutes = float(date_string[14:16])
    seconds = float(date_string[17:])
    total_secs = (3600 * hours) + (60 * minutes) + seconds
    return total_secs


def get_average_per_second(get_list, time_list):
    gets_dict = {}
    for i, time in enumerate(time_list):
        if math.floor(time) not in gets_dict:
            gets_dict[math.floor(time)] = []
        gets_dict[math.floor(time)].append(get_list[i])

    avg_dict = {}
    for key in gets_dict.keys():
        a


def plot_get_vs_time(file_name):
    df = store_data.read_partial(file_name, ["Get Time", "Time of Expansion"])
    get_list = list(df["Get Time"])
    time_list = [seconds_from_date(i) for i in list(df["Time of Expansion"])]
    plt.plot(time_list, get_list)
    plt.ylabel("Get Time (sec)")
    plt.xlabel("Time of Get (sec)")
    plt.show()


if __name__ == '__main__':
    plot_get_vs_time("output.csv")
