# Frequent Path-Traversal Pattern Recognition on Streaming Data

C++ program to list all frequent path-traversal patterns with increasing size given a minimum support threshold s and a steam of web-click data. Each web server logs all activities into a stream (or a log file in batch mode) in the format of tuples, each containing an integer i (as user ID), and a reference string r (as web URL). A web-click sequence WCS for a user i can be extract from the web-click data as wcsi = [r1, r2, …, rk]. A maximal forward reference MFR is a forward reference path without any backward reference. So, each wcsi can be converted into several MFRs, i.e., wcsi = [mfr1, mfr2, …, mfrj]. The frequent pathtraversal patterns are MFRs with support ≥ s. The program supports multiple web-click streams via sockets (each socket contains an IP address or the hostname, and a port number) and supports ad-hoc queries with support threshold s. Both input and output follow CSV format.

## Input from stdin Example:

    129.210.16.80, 9999
    129.210.16.88, 33333
    3
    linux10617.scudc.scu.edu, 55555
    7
    23

## Input from Socket Example:

    100, http://www.cse.scu.edu/~mwang2/mining/index.heml
    100, http://www.cse.scu.edu/~mwang2/mining/Syllabus281.pdf
    200, http://www.cse.scu.edu/~mwang2/mining/index.heml
    ...

## Output Example:

Whenever an input from a socket, it should be output to stdout immediately (though not shown below.)

    With support 3, the frequent path-traversal patterns with length 1 are:
    1) http://www.cse.scu.edu/~mwang2/mining/index.html
    2) ...
    With support 3, the frequent path-traversal patterns with length 2 are:
    1) http://www.cse.scu.edu/~mwang2/mining/index.html, http://www.cse.scu.edu/~mwang2/mining/Syllabus.pdf
    2) ...
    ...
    With support 7, the frequent path-traversal patterns with length ...
    ...