# -*- coding=utf-8 -*-
from requests import Session
import time
import sys
from lxml import html
import re
import store_data
import datetime
from multiprocessing import Pool
import ast

wiki_map = {}
num_repeats = 0
num_pages = 0
errors = []
frontier = []
num_read_from_frontier = 0
keys = set()
time_spent_csv = 0.0
pool_parse_time = 0.0
map_size_time = 0.0
add_page_time = 0.0
inner_loop_time = 0.0
initialize_time = 0.0
getting_time = 0.0
in_keys_time = 0.0
next_title_time = 0.0
datetime_time = 0.0


def map_size():
    global wiki_map, map_size_time
    map_size_start = time.time()
    length = 0
    for dkey in wiki_map.keys():
        length += len(wiki_map[dkey])
    map_size_time += (time.time() - map_size_start)
    return length


def add_page(new_page, depth):
    global num_pages, wiki_map, keys
    if depth not in wiki_map:
        wiki_map[depth] = {}
    if new_page[0] not in wiki_map[depth]:
        keys.add(new_page[0])
        wiki_map[depth][new_page[0]] = new_page
        num_pages += 1
        analytics_string = "Page Number: {0:7d} | {1}"
        print(analytics_string.format(num_pages, get_page_analytics_string(new_page)))


def print_errors():
    global errors
    for et in errors:
        print("Error occurred at page number: " + str(et[0]))
        print(et[1])


def store_map():
    global wiki_map, time_spent_csv
    store_map_start = time.time()
    store_data.append_map_to_csv(wiki_map, "output.csv")
    wiki_map = {}
    time_spent_csv += (time.time() - store_map_start)


def read_frontier():
    global frontier, time_spent_csv, num_read_from_frontier
    frontier_read_start = time.time()
    tpls = list(store_data.read_partial_section("output.csv", None,
                                                1000, num_read_from_frontier).itertuples(index=False, name=None))
    frontier = [(tp[0], tp[1], ast.literal_eval(tp[2]), ast.literal_eval(tp[3]),
                 tp[4], tp[5], tp[6], tp[7], tp[8], tp[9]) for tp in tpls]
    num_read_from_frontier += len(frontier)
    time_spent_csv += (time.time() - frontier_read_start)


def initialize_map_search(initial_url, session):
    global frontier, initialize_time
    initialize_start = time.time()

    initial_title = initial_url.replace("https://en.wikipedia.org/wiki/", "").replace("_", " ")
    time_of_get = str(datetime.datetime.now())
    get_start = time.time()
    raw_html = session.get(initial_url).content
    get_time = time.time() - get_start
    t = (raw_html, initial_url, initial_title, get_time, time_of_get)
    initial_page = parse_page(t)

    add_page(initial_page, 0)
    initialize_time = (time.time() - initialize_start)
    store_map()


def not_in_keys(title):
    global keys, in_keys_time
    in_keys_start = time.time()
    output = title not in keys
    in_keys_time += (time.time() - in_keys_start)
    return output


def map_wiki(depth_cutoff, initial_url):
    global wiki_map, num_repeats, num_pages, errors, frontier, \
        keys, num_read_from_frontier, time_spent_csv, pool_parse_time, \
        add_page_time, inner_loop_time, getting_time, in_keys_time, next_title_time, datetime_time

    session = Session()
    initialize_map_search(initial_url, session)

    while True:
        if len(frontier) == 0:
            read_frontier()
            if len(frontier) == 0:
                store_map()
                break

        cur_node = frontier.pop(0)

        try:
            if cur_node[8] < depth_cutoff:
                inner_loop_start = time.time()
                page_tuple_list = []
                for i, next_url in enumerate(cur_node[3]):
                    next_title_start = time.time()
                    next_title = cur_node[2][i]
                    next_title_time += (time.time() - next_title_start)

                    if not_in_keys(next_title):
                        datetime_start = time.time()
                        time_of_get = str(datetime.datetime.now())
                        datetime_time += (time.time() - datetime_start)

                        get_start = time.time()
                        raw_html = session.get(next_url).content
                        get_time = time.time() - get_start
                        getting_time += get_time

                        t = (raw_html, next_url, next_title, get_time, time_of_get)
                        page_tuple_list.append(t)
                    else:
                        num_repeats += 1
                inner_loop_time += (time.time() - inner_loop_start)

                pool_parse_start = time.time()
                with Pool() as p:
                    page_list = p.map(parse_page, page_tuple_list)
                pool_parse_time += (time.time() - pool_parse_start)

                add_page_start = time.time()
                for page in page_list:
                    add_page(page, cur_node[8] + 1)
                add_page_time += (time.time() - add_page_start)

                store_map()
        except:
            e = sys.exc_info()[0]
            errors.append((num_pages, e))
            print(e)


def get_page_analytics_string(pt):
    analytics_format_string = "Num Out-links: {0:4d} | Time: {6} | Total Time: {1:6.4f} | Get Time: {2:6.4f} | " \
                              "Parse Time: {3:6.4f} | Analysis Time: {4:6.4f} | Page Title: {5}"
    return analytics_format_string.format(len(pt[2]), pt[4], pt[5], pt[6], pt[7], pt[0], pt[8][11:21])


def parse_page(tpl):
    raw_html, search_url, title, get_time, time_of_get = tpl

    parse_start = time.time()
    root = html.fromstring(raw_html)
    parse_time = time.time() - parse_start

    analysis_start = time.time()

    out_links = []
    out_titles = []
    a_tags = root.cssselect("a[title][href^='/wiki/']")

    for a in a_tags:
        temp_url = a.get("href")
        temp_title = a.get("title")

        if temp_title == "Help:Category":
            break

        if temp_title not in out_titles and not bool(re.search("([*\S]:[*\S])", temp_title + temp_url)):
            out_links.append("https://en.wikipedia.org" + temp_url)
            out_titles.append(temp_title)

    analysis_time = time.time() - analysis_start
    total_time = get_time + parse_time + analysis_time

    page = (title, search_url, out_titles, out_links, total_time, get_time, parse_time, analysis_time, time_of_get)

    return page


if __name__ == "__main__":
    sys.stdout = open("stdout.txt", mode='w')
    store_data.initialize_csv("output.csv")

    url_med = "https://en.wikipedia.org/wiki/IPhone"               # 855 out-links Depth 1: ~45 sec Depth 2: ~1 hr
    url_small = "https://en.wikipedia.org/wiki/Contract_manufacturer"  # 50 out-links Depth 1: ~3 sec Depth 2: ~8.5 min

    start = time.time()
    map_wiki(3, url_small)
    end = time.time()

    print_errors()
    print("\n\nAnalytics\n")
    print("Total Time Elapsed:         {0:.3f} sec\n".format(end - start))
    print("Time Spent CSV:             {0:.3f} sec".format(time_spent_csv))
    print("Pool Parse Time:            {0:.3f} sec".format(pool_parse_time))
    print("Map Size Time:              {0:.3f} sec".format(map_size_time))
    print("Add Page Time:              {0:.3f} sec".format(add_page_time))
    print("Inner Loop Time:            {0:.3f} sec".format(inner_loop_time))
    print("  - Getting Time:             {0:.3f} sec".format(getting_time))
    print("  - In Keys Time:             {0:.3f} sec".format(in_keys_time))
    print("  - Next Title Time:          {0:.3f} sec".format(next_title_time))
    print("  - Datetime Time:            {0:.3f} sec\n".format(datetime_time))
    timing_total = time_spent_csv + pool_parse_time + map_size_time + add_page_time + inner_loop_time + initialize_time
    print("Timing Total:               {0:.3f} sec\n".format(timing_total))
    print("Number of Errors:           {0:d}".format(len(errors)))
    print("Number of Repeats:          {0:d}".format(num_repeats))

    print("-" * 100)
    sys.stdout.flush()
    store_data.print_analytics("output.csv")
