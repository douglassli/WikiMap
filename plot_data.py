import matplotlib.pyplot as plt


def seconds_from_date(date_string):
    hours = float(date_string[11:13])
    minutes = float(date_string[14:16])
    seconds = float(date_string[17:])
    total_secs = (3600 * hours) + (60 * minutes) + seconds
    return total_secs
