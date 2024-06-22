#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

#define NUM_THREADS 4
#define BATCH_SIZE 1000

static unsigned long int next = 1;

typedef struct {
    uint32_t local_counter;
} ThreadData;

int myRand(void) {
    next = next * 1103515245 + 12345;
    int result = (int) (next / 65536) % 32768;
    return result;
}

// Function to perform modular exponentiation (x^y % p)
uint32_t power(uint64_t base, uint32_t exponent, uint32_t mod) {
    uint64_t result = 1;
    base = base % mod;  // Update base if it is more than or equal to mod

    while (exponent > 0) {
        // If exponent is odd, multiply base with the result
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        // Exponent must be even now
        exponent = exponent >> 1;  // Divide the exponent by 2
        base = (base * base) % mod;
    }
    return (uint32_t)result;
}

// Miller-Rabin primality test
uint8_t isPrime(uint32_t n) {
    if (n <= 1 || n == 4) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }

    // Find d such that n = 2^r * d + 1 for some r >= 1
    uint32_t d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }

    // Perform 5 rounds of testing
    for (uint32_t i = 0; i < 5; i++) {
        uint32_t a = 2 + myRand() % (n - 4);  // Pick a random number in [2...n-2]
        uint64_t x = power(a, d, n);  // Compute a^d % n

        if (x == 1 || x == n - 1) {
            continue;
        }

        uint8_t is_composite = 1;
        for (uint32_t r = d; r != n - 1; r *= 2) {  // Repeat squaring
            x = (x * x) % n;
            if (x == 1) {
                return 0;
            }
            if (x == n - 1) {
                is_composite = 0;
                break;
            }
        }

        if (is_composite) {
            return 0;
        }
    }
    return 1;
}

void *processNumbers(void *arg) {
    ThreadData *thread_data = (ThreadData *)arg;
    uint32_t number;
    uint32_t prime_count = 0;

    uint32_t numbers_batch[BATCH_SIZE];
    uint32_t batch_index = 0;

    for (; scanf("%u", &number) != EOF; batch_index++) {
        numbers_batch[batch_index] = number;
        if (batch_index == BATCH_SIZE - 1) {
            // Process the batch
            for (uint32_t i = 0; i < BATCH_SIZE; i++) {
                if (isPrime(numbers_batch[i])) {
                    prime_count++;
                }
            }
            batch_index = -1;  // Reset batch index for the next batch
        }
    }

    // Process remaining numbers in the last batch
    for (uint32_t i = 0; i < batch_index; i++) {
        if (isPrime(numbers_batch[i])) {
            prime_count++;
        }
    }

    thread_data->local_counter = prime_count;  // Store the local counter in the thread data

    return NULL;
}


int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // Create threads
    uint8_t i = 0;
    while (i < NUM_THREADS) {
        thread_data[i].local_counter = 0;
        pthread_create(&threads[i], NULL, processNumbers, &thread_data[i]);
        i++;
    }

    // Wait for threads to finish
    i = 0;
    while (i < NUM_THREADS) {
        pthread_join(threads[i], NULL);
        i++;
    }

    uint32_t total_primes = 0;

    // Combine results from all threads
    i = 0;
    while (i < NUM_THREADS) {
        total_primes += thread_data[i].local_counter;
        i++;
    }

    printf("%u total primes.\n", total_primes);

    return 0;
}