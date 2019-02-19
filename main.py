# -*- coding=utf-8 -*-
from requests import Session
import time
import sys
from lxml import html
import re
import store_data
import datetime
from multiprocessing import Pool
import pysize

initial_url = ""
wiki_map = {}
num_repeats = 0
num_pages = 0
errors = []
frontier = []
keys = []


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
