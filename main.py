# -*- coding=utf-8 -*-
from requests import Session
import time
import sys
from lxml import html
import re
import store_data
import datetime
from multiprocessing import Pool

wiki_map = {}
num_repeats = 0
num_pages = 0
errors = []
frontier = []
num_read_from_frontier = 0
keys = []
time_spent_csv = 0.0


def map_size():
    global wiki_map
    length = 0
    for dkey in wiki_map.keys():
        length += len(wiki_map[dkey])
    return length


def add_page(new_page, depth):
    global num_pages, wiki_map, keys
    if depth not in wiki_map:
        wiki_map[depth] = {}
    if new_page[0] not in wiki_map[depth]:
        keys.append(new_page[0])
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


def map_wiki(depth_cutoff, initial_url):
    global wiki_map, num_repeats, num_pages, errors, frontier, keys, num_read_from_frontier, time_spent_csv
    session = Session()

    initial_title = initial_url.replace("https://en.wikipedia.org/wiki/", "").replace("_", " ")
    time_of_get = str(datetime.datetime.now())
    get_start = time.time()
    raw_html = session.get(initial_url).content
    get_time = time.time() - get_start
    t = (raw_html, initial_url, initial_title, get_time, time_of_get)
    initial_page = parse_page(t)

    add_page(initial_page, 0)
    initial_node = (initial_page, 0)
    frontier = [initial_node]

    while True:

        if len(frontier) == 0:
            frontier_read_start = time.time()
            frontier = store_data.read_some_frontier("frontier.csv", num_read_from_frontier)
            num_read_from_frontier += len(frontier)
            time_spent_csv += (time.time() - frontier_read_start)
            if len(frontier) == 0:
                store_map()
                break

        if map_size() > 1000:
            store_map()

        cur_node = frontier.pop(0)

        try:
            if cur_node[1] < depth_cutoff:
                page_tuple_list = []
                for i, next_url in enumerate(cur_node[0][3]):
                    next_title = cur_node[0][2][i]
                    if next_title not in keys:
                        time_of_get = str(datetime.datetime.now())
                        get_start = time.time()
                        raw_html = session.get(next_url).content
                        get_time = time.time() - get_start
                        t = (raw_html, next_url, next_title, get_time, time_of_get)
                        page_tuple_list.append(t)
                    else:
                        num_repeats += 1

                with Pool() as p:
                    page_list = p.map(parse_page, page_tuple_list)

                temp_frontier = []
                for page in page_list:
                    add_page(page, cur_node[1] + 1)
                    temp_frontier.append((page, cur_node[1] + 1))

                frontier_store_start = time.time()
                store_data.append_to_frontier(temp_frontier, "frontier.csv")
                time_spent_csv += (time.time() - frontier_store_start)
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
    store_data.initialize_csv("frontier.csv")

    url_med = "https://en.wikipedia.org/wiki/IPhone"               # 855 out-links Depth 1: ~45 sec Depth 2: ~1 hr
    url_small = "https://en.wikipedia.org/wiki/Contract_manufacturer"  # 50 out-links Depth 1: ~3 sec Depth 2: ~8.5 min

    start = time.time()
    map_wiki(3, url_small)
    end = time.time()

    print_errors()
    print("\n\nAnalytics\n")
    print("\nTotal Time Elapsed:         {0:.1f} sec".format(end - start))
    print("Time Spent CSV:             {0:.2f} sec".format(time_spent_csv))

    print("-" * 100)
    store_data.print_analytics("output.csv")
