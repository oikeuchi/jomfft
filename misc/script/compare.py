import math, cmath, random, re, subprocess, argparse

USE_SINGLE = False
MAX_DIMENSION = 4
MAX_SIZE = 1 << 25
BLULESTEIN_SINGLE_MAX_SIZE = 1 << 10
BLULESTEIN_DOUBLE_MAX_SIZE = 1 << 20
SEED_MAX = 1 << 31

MATCH_RESULT_STR = r"mean: \((.+),(.+)\), stdev: \((.+),(.+)\), max_abs: (.+)"
MATCH_RESULT = re.compile(MATCH_RESULT_STR)

def sample_power_of_two(max_size):
    m = int(math.floor(math.log2(max_size)))
    e = random.randrange(1, m)
    return 1 << e

def sample_power_of(base, max_size):
    m = int(math.floor(math.log(max_size) / math.log(base)))
    e = random.randrange(1, m)
    return int(math.pow(base, e))

def sample_mixed_radix(max_size):
    max_size = min(max_size, MAX_SIZE // 2)
    c = random.sample([2, 3, 5, 7], 4);
    o = 1
    for p in c:
        q = sample_power_of(p, max_size)
        if o * q <= max_size:
            o = o * q
    return o

def sample_general_size(max_size, single):
    if single:
        max_size = min(max_size, BLULESTEIN_SINGLE_MAX_SIZE)
    else:
        max_size = min(max_size, BLULESTEIN_DOUBLE_MAX_SIZE)
    return random.randrange(2, max_size)

def sample_size(max_size, single):
    match random.choice([0, 0, 0, 0, 1, 1, 2]):
        case 0:
            return sample_power_of_two(max_size)
        case 1:
            return sample_mixed_radix(max_size)
        case 2:
            return sample_general_size(max_size, single)

def sample_cube(dimension, single):
    max_size = int(math.floor(math.pow(MAX_SIZE, 1. / dimension)))
    max_size = max_size
    s = 1
    c = []
    for _ in range(dimension):
        n = sample_size(max_size, single)
        if s * n <= MAX_SIZE:
            c.append(str(n))
            s = s * n
    return c

def sample_args_single(single):
    d = random.randrange(1, MAX_DIMENSION)
    return sample_cube(d, single)

def sample_args_outer_batch(single):
    d = random.randrange(2, MAX_DIMENSION)
    c = sample_cube(d, single)
    o = ['-b'] + c
    return o

def sample_args_inner_batch(single):
    d = random.randrange(2, MAX_DIMENSION)
    c = sample_cube(d, single)
    o = ['-t', '-b'] + c
    return o

def sample_extra_args():
    o = []
    if random.choice([0, 1]):
        o.append('-r')
    if random.choice([0, 1]):
        o.append('-c')
    if random.choice([0, 1]):
        o.append('-n')
    c = random.choice([0, 0, 0, 1, 2, 3]) 
    if c & 1:
        o.append('-x')
        o.append(str(random.randrange(1, 8)))
    if c & 2:
        o.append('-y')
        o.append(str(random.randrange(1, 8)))
    c = random.choice([0, 0, 0, 0, 0, 1, 2, 3]) 
    if c & 1:
        o.append('-u')
        o.append(str(random.randrange(1, 4)))
    if c & 2:
        o.append('-v')
        o.append(str(random.randrange(1, 4)))
    return o

def sample_args(single):
    if single:
        o = ['./jomfftf_compare']
    else:
        o = ['./jomfft_compare']
    match random.choice([0, 1, 2]):
        case 0:
            func = sample_args_single
        case 1:
            func = sample_args_outer_batch
        case 2:
            func = sample_args_outer_batch
    o.extend(func(single))
    o.extend(sample_extra_args())
    o.append('-s')
    o.append(str(random.randrange(1, SEED_MAX)))
    return o

def parse_result(text):
    m = MATCH_RESULT.match(text)
    mean = complex(float(m.group(1)), float(m.group(2)))
    stdev = complex(float(m.group(3)), float(m.group(4)))
    max_abs = float(m.group(5))
    return {'mean': mean, 'stdev': stdev, 'max_abs': max_abs}

def check_complex_value(v, eps):
    return abs(v.real) <= eps and abs(v.imag) <= eps

def check_result(result, single):
    if single:
        mean_eps = 1.e-2
        stdev_eps = 1.e-1
    else:
        mean_eps = 1.e-7
        stdev_eps = 1.e-5
    m = check_complex_value(result['mean'], mean_eps)
    s = check_complex_value(result['stdev'], stdev_eps)
    return m and s

def do_main():
    parser = argparse.ArgumentParser(
            description="Compare results of random-sampled fft problem against fftw.")
    parser.add_argument('-i', '--iterations', metavar='<integer>', default=1,
                        type=int, help="Number of iterations.")
    parser.add_argument('--seed', type=int, default=123456,
                        help="Seed for random number generation.")
    parser.add_argument('-s', '--single', action='store_true', default=False,
                        help="Use single precision executable (jomfftf_compare).")
    parser.add_argument('-v', '--verbose', action='store_true', default=False,
                        help="Enable verbose logging.")

    args = parser.parse_args()

    iterations = args.iterations
    seed = args.seed
    single = args.single
    verbose = args.verbose

    random.seed(seed)
    for _ in range(iterations):
        c = sample_args(single)
        if args.verbose:
            print(c)
        t = subprocess.run(c, check=True,
                           capture_output=True).stdout.decode('ascii').strip()
        if verbose:
            print(t)
        r = parse_result(t)
        if not check_result(r, single):
            print("[Failed] Args: ", end='')
            print(c, end=',')
            print("Stdout: ", end='')
            print(t)

if __name__ == '__main__':
    do_main()
