#!/usr/bin/env python
# pylint: disable=C0103

"""
This program goes from the toplevel directory recursively downwards and
places a index PHP in each directory.
Then, it's possible to browse files more conveniently with a web browser.
Note, that the PHP files have to be accessed through a web server such
as Apache with PHP support.
"""

import argparse
import os

#########################
# Setup argument parser
#########################

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("toplevelDirectory", help="Path to the toplevel directory, which \
                                               is the starting point for the recursion")
parser.add_argument("inputPHPfile", help="Path to the file source, which is placed \
                                          in each directory")
parser.add_argument("-o", "--outputFilename", default="index.php",
                    help="Name of the output file, which is placed in each directory")
parser.add_argument("-v", "--verbosity", default=1,
                    help="Increase or decrease output verbosity")
args = parser.parse_args()

#########################
# Go through directories
# from given toplevel
# directory and put the
# given PHP file in each
# directory
#########################

# Read input PHP file
with open(args.inputPHPfile, 'r') as f:
    src = f.read()

# Walk downwards through toplevel directory and put given PHP file there
if args.verbosity == 1:
    print('Start from toplevel dir: {}'.format(args.toplevelDirectory))
    print('Output filename: {}'.format(args.outputFilename))

for currentDir, subdirs, files in os.walk(args.toplevelDirectory):
    if args.verbosity == 1:
        print('Current directory: {}'.format(currentDir))
    with open(os.path.join(currentDir, args.outputFilename), 'w') as f:
        f.write(src)
