#include <iostream>
#include <tuple>
#include <type_traits>
#include <limits>

#pragma once

namespace dgu::detail
{
    namespace helpers
    {

        /**
         * @brief Tuple to struct overload util
         *
         * @tparam S struct layout (type)
         * @tparam Is unpacked sequence corresponding to tuple size
         * @tparam Tup tuple type
         * @param tup tuple to convert
         * @return S converted struct
         */
        template <class S, std::size_t... Is, class Tup>
        S _to_struct(std::index_sequence<Is...>, Tup &&tup)
        {
            return {std::get<Is>(std::forward<Tup>(tup))...}; // ! {} struct init with forwarding and unpack
        }

        /**
         * @brief Tuple to struct overload util
         *
         * @tparam S struct layout (type)
         * @tparam Tup tuple type
         * @param tup a tuple to convert
         * @return S converted struct
         */
        template <class S, class Tup>
        S _to_struct(Tup &&tup)
        {
            using T = std::remove_reference_t<Tup>; // ! pure tuple type

            return _to_struct<S>(
                std::make_index_sequence<std::tuple_size<T>{}>{},
                std::forward<Tup>(tup)); // ! call previously declared overload util
        }

        /**
         * @brief Function which applies a generator to each element
         *
         * @tparam T type of a value to apply generator to
         * @tparam Gen generator iterator type
         * @param it generator in iterator form
         * @param _v reference to the value to change
         */
        template <typename T, typename Gen>
        void _f(Gen &it, T &_v)
        {
            _v = static_cast<T>(*(it));
            it++;
        }

        /**
         * @brief Creates struct from defaults params
         *
         * @tparam S struct layout (type)
         * @tparam Gen generator iterator type
         * @tparam Args all types present in struct layout
         * @param it generator in iterator form.
         * @param defaults default struct parameters
         * @return S generated struct.
         */
        template <typename S, typename Gen, typename... Args>
        S _struct_from_params(Gen &it, Args &&...defaults)
        {

            (_f(it, defaults), ...); // ! regenerating defaults parameters using fold expression c++17
            auto tup = std::make_tuple<>(std::forward<Args>(defaults)...);
            return _to_struct<S>(tup);
        }
    }

    /**
     * @brief Generates a struct from provided generator, struct type and args.
     *
     * @tparam S struct layout (type)
     * @tparam Gen generator iterator type
     * @tparam Args all types present in struct layout
     * @param it generator in iterator form
     * @return S generated struct
     */
    template <typename S, typename Gen, typename... Args>
    S generate_struct(Gen &it)
    {
        return helpers::_struct_from_params<S>(it, std::numeric_limits<Args>::min()...);
    }
}
