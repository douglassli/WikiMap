import store_data
import ast
import hashlib


def prep_node_dict(file_name):
    print("PREPPING DICT")
    num_read = 0
    nodes = {}
    index = 0

    while True:
        print(num_read)
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 10000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        for tp in tpls:
            temp_hash = hashlib.md5(str(tp[0]).encode()).digest()
            if temp_hash not in nodes:
                nodes[temp_hash] = index
                index += 1

            for link_title in ast.literal_eval(tp[1]):
                lt_hash = hashlib.md5(str(link_title).encode()).digest()
                if lt_hash not in nodes:
                    nodes[lt_hash] = index
                    index += 1

    return nodes


def prep_search_file(file_name):
    node_dict = prep_node_dict(file_name)
    print("PREPPING FILE")
    num_read = 0
    search_file = open("search_file.txt", "w")
    added_nodes = set()

    while True:
        print(num_read)
        df = store_data.read_partial_section(file_name, ["Page Title", "Out-link Titles"], 10000, num_read)
        tpls = list(df.itertuples(index=False, name=None))

        if len(tpls) == 0:
            break

        num_read += len(tpls)

        for tp in tpls:
            temp_hash = hashlib.md5(str(tp[0]).encode()).digest()
            added_nodes.add(node_dict[temp_hash])
            search_file.write(str(node_dict[temp_hash]) + " ")

            for link_title in ast.literal_eval(tp[1]):
                search_file.write(str(node_dict[hashlib.md5(str(link_title).encode()).digest()]) + " ")

            search_file.write("\n")

    for node_val in node_dict.values():
        if node_val not in added_nodes:
            search_file.write(str(node_val) + " \n")
    search_file.write("*END")

    search_file.close()


if __name__ == '__main__':
    prep_search_file("Data Sets/medium2_output.csv")
