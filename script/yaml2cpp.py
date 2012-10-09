#!/usr/bin/env python

import sys
import yaml

def main():
    f = open(sys.argv[1])
    d = yaml.load(f)

if __name__ == "__main__":
    main()
