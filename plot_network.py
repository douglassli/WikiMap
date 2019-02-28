import store_data
import ast
import pandas


def get_nodes_attributes(file_name, output_file):
    num_read = 0
    store_data.initialize_csv(output_file)

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        titles = [(tp[0], len(ast.literal_eval(tp[1]))) for tp in tpls]
        df = pandas.DataFrame(titles)
        df.to_csv(output_file, index=False, mode="a", header=["Node", "Num Out-links"])


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


if __name__ == '__main__':
    get_nodes_attributes("output.csv", "nodes.csv")
    prepare_edges("output.csv", "edges.csv")
