# Prime Number Threads

## [Task Description] (HW2b.pdf)
The objective is to process an endless stream of numbers and count prime numbers efficiently. The provided random number generator `generator.c` simulates this endless stream. 
The task is to achieve a performance that is at least 4 times faster than the standard prime algorithm without modifications, or 10 times faster with changes to the algorithm.
The solution must be highly performant and memory efficient, using only 2MB of RAM

## Implementation Details

### Standard Prime Algorithm - [primeCounter](primeCounter.c)

The standard prime algorithm implemented in the `primeCounter.c` file, uses a simple approach to check if a number is
prime by iterating over potential divisors up to the square root of the number.

### Optimized Prime Algorithm - [myPrimeCount](myPrimeCount.c)

The optimized prime algorithm implemented in the `myPrimeCount.c` file utilizes the
**[Miller-Rabin primality test](https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/)**, a
probabilistic algorithm that efficiently determines whether a given number is prime. The algorithm is **parallelized**
using **pthreads** to process numbers in **batches**, **reducing** function **calls** to isPrime and **minimizing
mutex**
contention for improved performance.
