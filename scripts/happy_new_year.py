# this script replaces the past year in the header with the current year

import re
import datetime

def replace_year(string):
    # get the current year
    current_year = datetime.datetime.now().year
    past_year = current_year - 1
    # look for the past year in the string and replace it with the current year
    year = re.search(str(past_year), string).group()
    # replace the year in the string
    new_string = string.replace(year, str(current_year))
    return new_string

def replace_header(file_path):
    # Read in the file
    with open(file_path, 'r') as file :
        file_string = file.read()

    file_string = replace_year(file_string)

    with open(file_path, 'w') as file:
        file.write(file_string)

if __name__ == '__main__':
    replace_header('templates/header.txt')
