from subprocess import Popen, PIPE
from argparse import ArgumentParser
from numpy import median
import math
import random
import itertools

FAIL = "\033[91m\033[1m"
GREEN = "\033[32m\033[1m"
ENDC = "\033[0m"
BOLD = "\033[1m"


def main():
    parser = ArgumentParser()
    parser.add_argument("-e", "--executable", help="Path to the test.sh script.", type=str, default="./test.sh")
    parser.add_argument("-l", "--limit", help="The number of tests for each input size.", type=int, default=15)
    parser.add_argument("-i", "--minInputSize", help="The minimum input size to test.", type=int, default=1)
    parser.add_argument("-I", "--maxInputSize", help="The maximum input size to test.", type=int, default=31)
    parser.add_argument("-v", "--verbose", help="Enables verbosity.", action="store_true")


    argv = parser.parse_args()
    if not argv.executable:
        return

    altitudes = [random.randint(1, 1024)]

    args = [argv.executable, ""]
    successsfull = 0
    failed = 0
    averages = []
    for inputSize in range(argv.minInputSize, argv.maxInputSize):
        if (argv.verbose):
            print(f"[Input size] {inputSize}")
        averages.append([])
        for i in range(0, min(argv.limit, math.factorial(inputSize))):
            if i == 0:
                altitudes.sort()
            elif i == 1:
                altitudes.sort(reverse=True)
            else:
                random.shuffle(altitudes)

            args[1] = ",".join(map(str, altitudes))
            angles = [-math.inf] + [
                math.atan((x - altitudes[0]) / float(i))
                for i, x in enumerate(altitudes[1:], 1)
            ]

            max_prescan = [-math.inf] + list(itertools.accumulate(angles, max))
            max_prescan.pop()
            visibilities = [
                "v" if (angle > max_prev_angle) else "u"
                for (angle, max_prev_angle) in zip(angles, max_prescan)
            ]
            visibilities[0] = "_"

            ref_output = ",".join(visibilities)
            process = Popen(args, stdout=PIPE, stderr=PIPE)
            output = process.communicate()[0].decode("utf-8").rstrip("\n").split('\n')
            times = [k for k in output if '.' in k]
            res = '_'
            for k in output:
                if ',' in k:
                    res = k
                    break
            averages[-1] += times
            if res != ref_output:
                print(f"[Input string] {args[1]}")
                print(f"{FAIL}FAIL: {ENDC}{FAIL}{output}{ENDC} != {GREEN}{ref_output}{ENDC}")
                failed += 1
            else:
                successsfull += 1
                if (argv.verbose):
                    print(f"[Input string] {args[1]}")
                    print(f"{GREEN}GOOD{ENDC}")

        averages[-1] = [float(k) for k in averages[-1]]
        altitudes.append(random.randint(1, 1024))
    graph = [median(k) for k in averages]
    print(f"Successfull: {GREEN}{successsfull}{ENDC}, Failed: {FAIL}{failed}{ENDC}")
    print(graph)

if __name__ == "__main__":
    main()
