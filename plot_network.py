import igraph as ig
import store_data


def get_nodes(file_name):
    df = store_data.read_partial(file_name, ["Page Title"])
    titles = list(df["Page Title"])

    num_read = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Out-link Titles"], 1000, num_read)
        out_link_lists = list(df["Out-link Titles"])

        if len(out_link_lists) == 0:
            break

        num_read += len(out_link_lists)

        for link_list in out_link_lists:
            temp = [i for i in link_list if i not in titles]
            titles += temp

    return titles