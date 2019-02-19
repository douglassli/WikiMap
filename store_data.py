import pandas
import ast

def initialize_csv(file_name):
    df = pandas.DataFrame()
    df.to_csv(file_name, index=False, header=False)
