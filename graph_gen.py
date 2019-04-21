import random


def gen_graph(num_nodes, branch_low, branch_high):
    num_edges = 0
    search_file = open("search_file.txt", "w")

    for node_num in range(num_nodes):
        search_file.write(str(node_num) + " ")

        num_branches = random.randint(branch_low, branch_high)
        num_edges += num_branches
        for branch_num in range(num_branches):
            next_edge = random.randint(0, num_nodes)
            search_file.write(str(branch_num) + " ")

        search_file.write("\n")