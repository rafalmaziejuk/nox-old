#!/usr/bin/env python3

from auxiliary import OUTPUT_DIR
from build_runner import BuildRunner
from clang_format_runner import ClangFormatRunner
from clang_tidy_runner import ClangTidyRunner

from pathlib import Path
from sys import (argv, 
                 exit)

def parse_options():
    from argparse import (ArgumentParser,
                          ArgumentDefaultsHelpFormatter)
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument("-b", "--build", action="store_true", help="build targets")
    parser.add_argument("-f", "--clang-format", action="store_true", help="run clang-format")
    parser.add_argument("-t", "--clang-tidy", action="store_true", help="run clang-tidy")
    return parser.parse_args()

def main():
    args = parse_options()

    if not len(argv) > 1:
        print('No arguments provided, exiting...')
        exit()

    try:
        if args.build:
            runner = BuildRunner()

        if args.clang_format:
            runner = ClangFormatRunner()
        
        if args.clang_tidy:
            runner = ClangTidyRunner()
    except Exception as e:
        print(str(e))
        exit()

    if args.clang_tidy or args.build:
        Path(OUTPUT_DIR).mkdir(exist_ok=True)
    
    runner.run()

if __name__ == "__main__":
     main()
