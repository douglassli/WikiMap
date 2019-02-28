import store_data
import ast
import pandas


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


def prepare_edges(file_name, output_file):
    num_read = 0
    store_data.initialize_csv(output_file)

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        edges = []

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        for page in tpls:
            temp_title = page[0]
            for target in ast.literal_eval(page[1]):
                edges.append((temp_title, target))

        df = pandas.DataFrame(edges)
        df.to_csv(output_file, index=False, mode="a", header=["Source", "Target"])

    return list(edges)


if __name__ == '__main__':
    prepare_edges("output.csv", "edges.csv")
