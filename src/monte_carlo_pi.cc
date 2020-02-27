#include <cstddef>
#include <thread>
#include <future>
#include <chrono>
#include <limits>
#include <tuple>
#include <vector>
#include <iostream>
#include <sstream>

#include "rng.h"

using namespace std;

size_t get_circle_hits(size_t samples)
{
    my::DoubleRNG rng (0, 1);
    size_t in = 0;
    for (size_t i = 0; i < samples; ++i)
    {
        double x = rng(), y = rng();
        if (x * x + y * y < 1) ++in;
    }
    return in;
}

double estimate_pi(unsigned concurrency, size_t samples)
{
    size_t chunk = samples / concurrency;

    vector<decltype(async(get_circle_hits, samples))> futures(concurrency);
    for (unsigned i = 0; i < concurrency; ++i)
    {
        futures[i] = async(get_circle_hits, chunk);
    }
    size_t in = 0;
    for (unsigned i = 0; i < concurrency; ++i)
    {
        in += futures[i].get();
    }
    return 4. * in / samples;
}

template <typename Func, typename ... Args>
auto measure(Func func, Args && ... args)
{
    auto start = chrono::high_resolution_clock::now();
    auto result = func(std::move(args) ...);
    auto end = chrono::high_resolution_clock::now();
    return tuple(result, chrono::duration_cast<chrono::milliseconds>(end - start));
}

ostream& operator<<(ostream &o, chrono::milliseconds v)
{
    return o << v.count() << "ms";
}

void run(unsigned concurrency, size_t samples)
{
    cout << "Estimating PI:" << endl;
    cout << "  Concurrency: " << concurrency << endl;
    cout << "  Samples: " << samples << endl;
    cout << "  ... " << endl;

    auto [result, duration] = measure(estimate_pi, concurrency, samples);

    cout << "  Pi: " << result << endl;
    cout << "  Time: " << duration << endl;
}

int main(int argc, char **argv)
{
    unsigned concurrency = thread::hardware_concurrency();
    size_t samples = 10000000000; // numeric_limits<size_t>::max() / (4 * concurrency);

    if (argc == 3)
    {
        stringstream ss;
        ss << argv[1] << " " << argv[2];
        ss >> concurrency >> samples;
    }

    run(concurrency, samples);

    return 0;
}
