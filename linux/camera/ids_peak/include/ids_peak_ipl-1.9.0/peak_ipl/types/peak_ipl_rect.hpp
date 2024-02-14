/*!
 * \file    peak_ipl_rect.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-10-17
 * \since   1.6
 *
 * Copyright (c) 2022 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once

#include <type_traits>
#include <cmath>
#include <limits>
#include <tuple>

#include <peak_ipl/types/peak_ipl_simple_types.hpp>
/*!
 * \brief The "peak::ipl" namespace contains the whole image processing library.
 */
namespace peak
{
namespace ipl
{
/*!
 * \brief Rectangle type
 */
class Rect2D
{
    Point2D m_point{};
    Size2D m_size{};

public:
    constexpr Rect2D(size_t x, size_t y, size_t width, size_t height)
        : Rect2D({ x, y }, { width, height })
    {}

    constexpr Rect2D(const Point2D& point, const Size2D& size)
        : m_point(point)
        , m_size(size)
    {}

    /*! Returns the position of the top-left corner of the rectangle */
    PEAK_IPL_NO_DISCARD constexpr Point2D topLeft() const
    {
        return m_point;
    }

    /*! Returns the size of the rectancle */
    PEAK_IPL_NO_DISCARD constexpr Size2D size() const
    {
        return m_size;
    }

    /*! Returns the left position of the rectangle */
    PEAK_IPL_NO_DISCARD constexpr size_t left() const
    {
        return m_point.x;
    }

    /*! Returns the right position of the rectangle */
    PEAK_IPL_NO_DISCARD constexpr size_t right() const
    {
        return m_point.x + m_size.width;
    }

    /*! Returns the top position of the rectangle */
    PEAK_IPL_NO_DISCARD constexpr size_t top() const
    {
        return m_point.y;
    }

    /*! Returns the bottom position of the rectangle */
    PEAK_IPL_NO_DISCARD constexpr size_t bottom() const
    {
        return m_point.y + m_size.height;
    }

    /*! Sets a new size for the rectangle */
    void setSize(const Size2D& size)
    {
        m_size = size;
    }

    /*! Sets the rectangle width */
    void setWidth(size_t width)
    {
        m_size.width = width;
    }

    /*! Sets the rectangle height */
    void setHeight(size_t height)
    {
        m_size.height = height;
    }

    /*! Sets the left position of the rectangle */
    void setX(size_t x)
    {
        m_point.x = x;
    }

    /*! Sets the top position of the rectangle */
    void setY(size_t y)
    {
        m_point.y = y;
    }
};

inline bool operator==(const Rect2D& lhs, const Rect2D& rhs)
{
    return (lhs.topLeft() == rhs.topLeft() && lhs.size() == rhs.size());
}

inline bool operator!=(const Rect2D& lhs, const Rect2D& rhs)
{
    return !(lhs == rhs);
}

} /* namespace ipl */
} /* namespace peak */
