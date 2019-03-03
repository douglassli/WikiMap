import store_data
import ast


def prep_node_dict(file_name):
    num_read = 0
    nodes = {}
    index = 0

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        for tp in tpls:
            if tp[0] not in nodes:
                nodes[tp[0]] = index
                index += 1

            for link_title in ast.literal_eval(tp[1]):
                if link_title not in nodes:
                    nodes[link_title] = index
                    index += 1

    return nodes


def prep_search_file(file_name):
    node_dict = prep_node_dict(file_name)
    num_read = 0
    search_file = open("search_file.txt", "w")

    while True:
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 1000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        for tp in tpls:
            search_file.write(str(node_dict[tp[0]]) + " ")

            for link_title in ast.literal_eval(tp[1]):
                search_file.write(str(node_dict[link_title]) + " ")

            search_file.write("\n")

        search_file.write("*END")

    search_file.close()


if __name__ == '__main__':
    prep_search_file("Data Sets/medium2_output.csv")
