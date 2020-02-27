#include <random>
#include <limits>

namespace my
{
    template <typename T, typename Distribution>
    class Generator
    {
    public:
        Generator(
            T min = std::numeric_limits<T>::min,
            T max = std::numeric_limits<T>::max
        )
            : _engine(std::random_device{}())
            , _distribution(min, max)
        {
        }

        T operator()()
        {
            return _distribution(_engine);
        }
    private:
        std::mt19937 _engine;
        Distribution _distribution;
    };

    using DoubleRNG = Generator<double, std::uniform_real_distribution<double>>;
    using IntRNG = Generator<int, std::uniform_int_distribution<int>>;
}