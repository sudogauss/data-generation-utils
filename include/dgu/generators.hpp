#include <vector>
#include <limits>
#include <type_traits>

#include <boost/generator_iterator.hpp>
#include <boost/random.hpp>

#pragma once

namespace dgu::generators
{
    // ! anonymous namespace underlying generator engine implementations
    namespace
    {

        /**
         * @brief Boost generator iterator underlying randomized engine.
         *
         * This impl class is a boost generator iterator compatible generator which is capable
         * of producing random data. It uses mt19937 random number engine fed to the
         * uniform_real_distribution generator.
         */
        class random_generator_impl
        {
        public:
            /**
             * @brief Result_type typedef required by boost::generator_iterator.
             */
            typedef double result_type;

            /**
             * @brief Construct a new random generator impl object with zero initial state.
             *
             * @param min minimal double random value.
             * @param max maximal double random value.
             */
            random_generator_impl(double min, double max) : state(0.0),
                                                            _generator(min, max) {}

            /**
             * @brief Generate next random value.
             *
             * Operator overloading is required by boost::generator_iterator to produce
             * next iterator value.
             *
             * @return double next random value.
             */
            inline double operator()()
            {
                state = _rand_num();
                return state;
            }

        private:
            /**
             * @brief Current generated value.
             */
            double state;

            /**
             * @brief Random number generator engine.
             */
            boost::random::mt19937 _eng;

            /**
             * @brief Uniform real number generator based on _eng.
             */
            boost::random::uniform_real_distribution<double> _generator;

            /**
             * @brief Returns a real random number provided by generator.
             *
             * @return double random value.
             */
            inline double _rand_num()
            {
                return _generator(_eng);
            }
        };

        /**
         * @brief Boost generator iterator underlying vectorized engine.
         *
         * This impl class is a boost generator iterator compatible generator which is capable
         * of producing data from provided cyclic vector.
         */
        class vectorized_generator_impl
        {
        public:
            /**
             * @brief Result_type typedef required by boost::generator_iterator.
             */
            typedef double result_type;

            /**
             * @brief Construct a new vectorized generator impl object with provided vector.
             *
             * @param vec vector with predefined data.
             */
            vectorized_generator_impl(const std::vector<double> &vec) : _generator(vec),
                                                                        _vec_it(_generator.begin()) {}

            /**
             * @brief Generate next value from cyclic vector iterator.
             *
             * Operator overloading is required by boost::generator_iterator to produce
             * next iterator value.
             *
             * @return double next value in cyclic vector.
             */
            inline double operator()() { return _vec_it == _generator.end() ? _vec_it = _generator.begin(), *(_vec_it++) : *(_vec_it++); }

        private:
            /**
             * @brief Vector with predefined data used as generator.
             */
            std::vector<double> _generator;

            /**
             * @brief Vector iterator allowing to access predefined data.
             */
            std::vector<double>::iterator _vec_it;
        };
    } // * end of anonymous namespace

    /**
     * @brief Generates infinite, lazy-evaluated stream of random data
     */
    class random_generator
    {
    public:
        random_generator(double min, double max) : _gen(min, max), _it(boost::make_generator_iterator<>(_gen)) {}
        inline random_generator &operator++(int)
        {
            _it++;
            return *this;
        }
        inline double operator*() { return *_it; }

    private:
        random_generator_impl _gen;
        boost::generator_iterator_generator<random_generator_impl>::type _it;
    };

    /**
     * @brief Generates infinite, lazy-evaluated stream of predefined cyclic data
     */
    class vectorized_generator
    {
    public:
        vectorized_generator(const std::vector<double> &vec) : _gen(vec),
                                                               _it(boost::make_generator_iterator<>(_gen)) {}
        inline vectorized_generator &operator++(int)
        {
            _it++;
            return *this;
        }
        inline double operator*() { return *_it; }

    private:
        vectorized_generator_impl _gen;
        boost::generator_iterator_generator<vectorized_generator_impl>::type _it;
    };
}
