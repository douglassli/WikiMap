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
