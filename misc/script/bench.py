import re, subprocess, argparse

MATCH_RESULT_STR = r"Elapsed: (.*) seconds, Score: (.*),"
MATCH_RESULT = re.compile(MATCH_RESULT_STR)

def run_bench_(module, level, real, conjugate,
              iterations, batch_size, dimension,
               dft_size, single, fftw_export, verbose):
    if single:
        args = ['./jomfftf_bench']
    else:
        args = ['./jomfft_bench']
    args.append('-m')
    args.append(str(module))
    if level > 0:
        args.append('-l')
        args.append(str(level))
        if fftw_export:
            args.append('-x')
    if real:
        args.append('-r')
    if conjugate:
        args.append('-c')
    if iterations > 1:
        args.append('-i')
        args.append(str(iterations))
    if batch_size > 1:
        args.append('-b')
        args.append(str(batch_size))
    for _ in range(dimension):
        args.append(str(dft_size))
    if verbose:
        print(args)
    return subprocess.run(args, check=True,
                       capture_output=True).stdout.decode('ascii').strip()

def run_bench(module, level, real, conjugate,
              problem_exponent, batch_size, dimension, exponent, single, verbose):
    dft_size = 1 << exponent
    if module == 1 and level > 1:
        run_bench_(module, level, real, conjugate, 1,
                   batch_size, dimension, dft_size, single,
                   True, False)
        level = 4
    problem_size_ = batch_size
    for _ in range(dimension):
        problem_size_ = problem_size_ * dft_size
    iterations = max(int((1 << problem_exponent) / problem_size_), 1)
    t = run_bench_(module, level, real, conjugate, iterations,
                   batch_size, dimension, dft_size, single,
                   False, verbose)
    m = MATCH_RESULT.match(t)
    seconds = float(m.group(1)) / iterations
    score = m.group(2)
    text = "{},{:.4e},{}".format(exponent, seconds, score)
    print(text)

def do_main():
    parser = argparse.ArgumentParser(
            description="Compare result of random-sampled fft problem against fftw")
    parser.add_argument('-d', '--dimension', metavar='<integer>', default=1,
                        type=int, help="Dimension.")
    parser.add_argument('--start', metavar='<integer>', default=4,
                        type=int, help="The first DFT size in exponent of 2.")
    parser.add_argument('--end', metavar='<integer>', default=4,
                        type=int, help="The last DFT size in exponent of 2.")
    parser.add_argument('-m', '--module', metavar='<integer>', default=0,
                        type=int, help="Module ID (0:Jomfft,1:FFTW,2:MKL).")
    parser.add_argument('-l', '--level', metavar='<integer>', default=2,
                        type=int, help="Optimization level of FFTW (1:Estimate,2:Measure,3:Patient).")
    parser.add_argument('-b', '--batch_size', metavar='<integer>', default=1,
                        type=int, help="Batch size.")
    parser.add_argument('-p', '--problem_size', metavar='<integer>', default=30,
                        type=int, help="Problem size in exponent of two.")
    parser.add_argument('-r', '--real', action='store_true', default=False,
                        help="Calculate real DFT.")
    parser.add_argument('-c', '--conjugate', action='store_true', default=False,
                        help="Calculate backward (conjugate) DFT.")
    parser.add_argument('-s', '--single', action='store_true', default=False,
                        help="Use single precision executable (jomfftf_bench).")
    parser.add_argument('-v', '--verbose', action='store_true', default=False,
                        help="Enable verbose logging.")

    args = parser.parse_args()
    for e in range(args.start, args.end + 1):
        run_bench(args.module, args.level, args.real, args.conjugate,
                  args.problem_size, args.batch_size, args.dimension, e,
                  args.single, args.verbose)

if __name__ == '__main__':
    do_main()
