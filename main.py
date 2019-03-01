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
import smtplib
import getpass

wiki_map = []
num_repeats = 0
num_pages = 0
errors = []
frontier = []
num_read_from_frontier = 0
keys = set()
time_spent_csv = 0.0
pool_parse_time = 0.0
add_page_time = 0.0
inner_loop_time = 0.0
initialize_time = 0.0
getting_time = 0.0
in_keys_time = 0.0
next_title_time = 0.0
datetime_time = 0.0


def add_page(new_page):
    global num_pages, wiki_map, keys
    # depth = new_page[8]
    # if depth not in wiki_map:
    #     wiki_map[depth] = {}
    # if new_page[0] not in wiki_map[depth]:
    #     keys.add(new_page[0])
    #     wiki_map[depth][new_page[0]] = new_page
    #     num_pages += 1
    #     analytics_string = "Page Number: {0:7d} | {1}"
    #     print(analytics_string.format(num_pages, get_page_analytics_string(new_page)))
    keys.add(new_page[0])
    wiki_map.append(new_page)
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
    wiki_map = []
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
    global initialize_time
    initialize_start = time.time()

    initial_title = initial_url.replace("https://en.wikipedia.org/wiki/", "").replace("_", " ")
    time_of_get = str(datetime.datetime.now())
    get_start = time.time()
    raw_html = session.get(initial_url).content
    get_time = time.time() - get_start
    t = (raw_html, initial_url, initial_title, get_time, time_of_get, 0)
    initial_page = parse_page(t)

    add_page(initial_page)
    initialize_time = (time.time() - initialize_start)
    store_map()


def not_in_keys(title):
    global keys, in_keys_time
    in_keys_start = time.time()
    output = title not in keys
    in_keys_time += (time.time() - in_keys_start)
    return output


def map_wiki(depth_cutoff, initial_url):
    global num_repeats, num_pages, errors, frontier, pool_parse_time, add_page_time, \
        inner_loop_time, getting_time, next_title_time, datetime_time

    session = Session()
    initialize_map_search(initial_url, session)

    while True:
        if len(frontier) == 0:
            store_map()
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

                        t = (raw_html, next_url, next_title, get_time, time_of_get, cur_node[8] + 1)
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
                    add_page(page)
                add_page_time += (time.time() - add_page_start)
        except:
            e = sys.exc_info()[0]
            errors.append((num_pages, e))
            print(e)


def get_page_analytics_string(pt):
    analytics_format_string = "Num Out-links: {0:4d} | Time: {6} | Total Time: {1:6.4f} | Get Time: {2:6.4f} | " \
                              "Parse Time: {3:6.4f} | Analysis Time: {4:6.4f} | Page Title: {5}"
    return analytics_format_string.format(len(pt[2]), pt[4], pt[5], pt[6], pt[7], pt[0], pt[9][11:21])


def parse_page(tpl):
    raw_html, search_url, title, get_time, time_of_get, depth = tpl

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

    page = (title, search_url, out_titles, out_links, total_time,
            get_time, parse_time, analysis_time, depth, time_of_get)

    return page


def get_global_analytics_string(true_time):
    global num_repeats, errors, time_spent_csv, pool_parse_time, \
        add_page_time, inner_loop_time, getting_time, in_keys_time, next_title_time, datetime_time
    format_string = "Analytics\n\n" \
                    "Total Time Elapsed:         {0:.3f} sec\n\n" \
                    "Time Spent CSV:             {1:.3f} sec\n" \
                    "Pool Parse Time:            {2:.3f} sec\n" \
                    "Add Page Time:              {3:.3f} sec\n" \
                    "Inner Loop Time:            {4:.3f} sec\n" \
                    "  - Getting Time:             {5:.3f} sec\n" \
                    "  - In Keys Time:             {6:.3f} sec\n" \
                    "  - Next Title Time:          {7:.3f} sec\n" \
                    "  - Datetime Time:            {8:.3f} sec\n\n" \
                    "Timing Total:               {9:.3f} sec\n\n" \
                    "Number of Errors:           {10:d}\n" \
                    "Number of Repeats:          {11:d}\n" \
                    + "-" * 100 + "\n"
    timing_total = time_spent_csv + pool_parse_time + add_page_time + inner_loop_time + initialize_time
    return format_string.format(true_time, time_spent_csv, pool_parse_time, add_page_time,
                                inner_loop_time, getting_time, in_keys_time, next_title_time,
                                datetime_time, timing_total, len(errors), num_repeats)


def send_termination_email(email, password, analytics):
    sender_email = email
    reciever_email = email
    message = "PROGRAM HAS TERMINATED\n" + analytics

    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.ehlo()
    server.starttls()
    server.ehlo()
    server.login(email, password)
    server.sendmail(sender_email, reciever_email, message)
    server.quit()


if __name__ == "__main__":
    if len(sys.argv) == 3:
        user_email = sys.argv[1]
        user_password = sys.argv[2]
    else:
        user_email = input("Please enter email: ")
        user_password = getpass.unix_getpass("Please enter password: ")

    # sys.stdout = open("stdout.txt", mode='w')
    store_data.initialize_csv("output.csv")

    url_med = "https://en.wikipedia.org/wiki/IPhone"               # 855 out-links Depth 1: ~45 sec Depth 2: ~1 hr
    url_small = "https://en.wikipedia.org/wiki/Contract_manufacturer"  # 50 out-links Depth 1: ~3 sec Depth 2: ~8.5 min

    start = time.time()
    map_wiki(1, url_small)
    end = time.time()

    analysis_string = store_data.get_analytics_string("output.csv")
    global_string = get_global_analytics_string(end - start)
    print(global_string + analysis_string, file=open("analytics.txt", "w"))

    send_termination_email(user_email, user_password, global_string + analysis_string)

    print("-" * 100)
    print("\nPROCESS COMPLETE\n")
    print("-" * 100)
