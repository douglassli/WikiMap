# WikiMap

NOTE: This project is a work in progress. It does not have full functionality yet.

### Overview
This is a program to scrape and map the connectivity of Wikipedia pages to each
other through embedded links in each page. Once this data is gathered, the next
goal of the program is to determine the optimal distance between two given
Wikipedia pages by travelling by following embedded links in the current page to other
Wikipedia pages. Once this functionality is achieved, the goal of the program is 
to determine which two Wikipedia pages have the largest optimal distance between 
them.

The web scraping portion of this program is achieved by beginning at a given 
Wikipedia page, retrieving the HTML, parsing it, and gathering all of the embedded 
links that link to another Wikipedia page. All of the resulting pages are stored
in the frontier until they are also expanded. This results in the creation of 
a search tree starting at the given Wikipedia page. By current estimates determined
by running smaller scale tests up to 500,000 pages, scraping all 5.8 million pages
on the english Wikipedia will take approximately 4 days of continuous scraping. For this
reason when the full data gathering is carried out, it will be done on a remote machine.

The second part of this project is searching the state space that was gathered. NOTE: 
This portion of the project is not yet fully implemented. This program will implement
uninformed search algorithms initially such as BFS, DFS, IDS, and UCS. Since BFS and DFS
are not necessarily optimal, IDS and UCS will likely be the best choices for the uninformed search
algorithms. Then some informed search algorithms such as Greedy search and A* search will 
be implemented. These algorithms will require the use of admissible heuristics. Therefore
a significant portion of this project will be dedicated to the development of effective and 
efficient heuristics.