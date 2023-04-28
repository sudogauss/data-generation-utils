#include <vector>
#include <limits>
#include <type_traits>

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

#include "detail/tuple_struct.hpp"

#pragma once

namespace dgu
{
    /**
     * @brief Class which allows to generate infinite lazy-evaluated data.
     * 
     * Supported data types are: arithmetic types, aggregates of previous types, vectors of previous types.
     * 
     * @tparam Gen a custom generator used to provide arithmetic data.
     */
    template <typename Gen>
    class DataGenerator
    {
    public:
        /**
         * @brief Construct a new Data Mock object.
         * 
         * @param it generator 
         */
        DataGenerator(Gen it) : _it(it) {}

        /**
         * @brief Generates either arithmetic or aggregate data depending on given layout.
         * 
         * Increments iterator in order to path to new data.
         * 
         * @tparam U structure layout or arithmetic data type
         * @tparam Args data types contained by structure layout or empty
         * @return U generated data
         */
        template <typename U, typename... Args>
        U next()
        {
            static_assert(std::is_trivial<U>::value && !std::is_array<U>::value);
            if constexpr (std::is_arithmetic_v<U>)
            {
                auto ret = *_it;
                _it++;
                return static_cast<U>(ret);
            }
            else
                return detail::generate_struct<U, decltype(_it), Args...>(_it);
        }

        /**
         * @brief Fills vector with provided underlying type
         * 
         * @tparam U structure layout or arithmetic data type
         * @tparam Args data types contained by structure layout or empty
         * @param vec a reference to vector to fill
         */
        template <typename UDT, typename... Args>
        void fill_vector(std::vector<UDT> &vec)
        {
            for (size_t i = 0; i < vec.size(); i++)
                vec[i] = next<UDT, Args...>();
        }

    private:
        /**
         * @brief generator
         */
        Gen _it;
    };
}
