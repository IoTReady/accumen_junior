/*!
 * \file    peak_ipl_pixel_line.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-08-12
 * \since   1.5
 *
 * Copyright (c) 2022 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <type_traits>
#include <cmath>
#include <limits>
#include <tuple>

/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{

/*!
 * \brief Range support and convenience class
 */
template <typename T>
class Range
{
    T m_min{}; //!< The minimum value for the range
    T m_max{}; //!< The maximum value for the range
    T m_inc{}; //!< The increment value for the range

    template <typename Q = T, typename std::enable_if_t<std::is_integral<Q>::value, int> = 0>
    static constexpr bool almostEqual(Q v1, Q v2)
    {
        return v1 == v2;
    }

    template <typename Q = T, typename std::enable_if_t<std::is_floating_point<Q>::value, int> = 0>
    static constexpr Q almostEqualMaxHelper(Q v1, Q v2)
    {
        return std::max<Q>({ 1.0, std::fabs(v1), std::fabs(v2) });
    }

    template <typename Q = T, typename std::enable_if_t<std::is_floating_point<Q>::value, int> = 0>
    static constexpr bool almostEqual(Q v1, Q v2)
    {
        return std::fabs(v1 - v2) <= std::numeric_limits<Q>::epsilon() * almostEqualMaxHelper(v1, v2);
    }

    template <typename Q = T, typename std::enable_if_t<std::is_integral<Q>::value, int> = 0>
    constexpr bool validateInc(Q v) const
    {
        if (m_inc != 0 && (v % m_inc) != 0)
        {
            return false;
        }
        return true;
    }

    template <typename Q = T, typename std::enable_if_t<std::is_floating_point<Q>::value, int> = 0>
    constexpr bool validateInc(Q v) const
    {
        if (!almostEqual<Q>(m_inc, 0) && !almostEqual<Q>(std::fmod(v, m_inc), 0))
        {
            return false;
        }
        return true;
    }

public:
    /*!
     * \brief Constructor for the Range
     *
     * \param min  The minimum value for the range.
     * \param max  The minimum value for the range.
     * \param inc  The increment value for the range.
     *
     * \throws InvalidArgumentException
     */
    constexpr Range(T min, T max, T inc)
        : m_min{ min }
        , m_max{ max }
        , m_inc{ inc }
    {
        if (min > max)
        {
            throw InvalidArgumentException("min > max is not allowed!");
        }
        if (max - min < inc)
        {
            throw InvalidArgumentException("inc > max - min is not allowed!");
        }
        if (inc < 0)
        {
            throw InvalidArgumentException("inc < 0 is not allowed!");
        }
        if (almostEqual(min, max))
        {
            throw InvalidArgumentException("min == max is not allowed!");
        }
    };

    /*!
     * \brief Get the minimum value for the range
     *
     * \returns the minimum value.
     */
    PEAK_IPL_NO_DISCARD constexpr T Minimum() const
    {
        return m_min;
    }

    /*!
     * \brief Get the maximum value for the range
     *
     * \returns the maximum value.
     */
    PEAK_IPL_NO_DISCARD constexpr T Maximum() const
    {
        return m_max;
    }

    /*!
     * \brief Get the increment value for the range
     *
     * \returns the increment value.
     */
    PEAK_IPL_NO_DISCARD constexpr T Increment() const
    {
        return m_inc;
    }

    /*!
     * \brief Get the range as tuple
     *
     * \returns the Range as tuple with <minimum, maximum, increment> in that order.
     */
    PEAK_IPL_NO_DISCARD constexpr std::tuple<T, T, T> GetRange() const
    {
        return std::make_tuple(Minimum(), Maximum(), Increment());
    }

    /*!
     * \brief Check if the value is in range
     *
     * \param value The value to check.
     *
     * \returns true if in range, false otherwise.
     */
    PEAK_IPL_NO_DISCARD constexpr bool InRange(T value) const
    {
        if (value < m_min || value > m_max)
        {
            return false;
        }

        return validateInc<T>(value);
    }
};

} /* namespace ipl */
} /* namespace peak */
