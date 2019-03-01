import store_data
import ast
import pandas


def pajek_prepare_node_dict(file_name):
    num_read = 0
    nodes = {}
    index = 1

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title"], 1000, num_read)
        titles = list(df["Page Title"])

        if len(titles) == 0:
            break

        for title in titles:
            nodes[title] = index
            index += 1

    return nodes


def get_major_nodes_attributes(file_name, output_file):
    num_read = 0
    store_data.initialize_csv(output_file)

    nodes = []

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        titles = [(tp[0], len(ast.literal_eval(tp[1]))) for tp in tpls if len(ast.literal_eval(tp[1])) > 300]
        nodes += [tp[0] for tp in titles]
        df = pandas.DataFrame(titles)
        df.to_csv(output_file, index=False, mode="a", header=["Node", "Num Out-links"])
    return nodes


def prepare_all_edges(file_name, output_file):
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


def prepare_major_edges(file_name, output_file):
    num_read = 0
    store_data.initialize_csv(output_file)
    major_nodes = set(get_major_nodes_attributes(file_name, "major_nodes.csv"))

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
                if temp_title in major_nodes and target in major_nodes and temp_title != target:
                    edges.append((temp_title, target))

        df = pandas.DataFrame(edges)
        df.to_csv(output_file, index=False, mode="a", header=["Source", "Target"])


if __name__ == '__main__':
    prepare_major_edges("output.csv", "major_edges.csv")
