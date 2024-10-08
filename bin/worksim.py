#!/usr/bin/env python3

import sys
import time

# Main execution

def main():
    if len(sys.argv) != 2:
        print('Usage: {} duration'.format(sys.argv[0]))
        sys.exit(1)

    duration = int(sys.argv[1])

    for _ in range(duration*100):
        time.sleep(0.01)

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python: