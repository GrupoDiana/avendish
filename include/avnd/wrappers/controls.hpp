#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <avnd/common/for_nth.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/widgets.hpp>
#include <cmath>

#include <string>

namespace avnd
{
template <typename F>
static constexpr void init_controls(avnd::effect_container<F>& effect)
{
  for(auto& state : effect.full_state()) {
      avnd::for_each_field_ref(
          state.inputs,
          [&]<typename T>(T& ctl) {
            if constexpr (avnd::has_range<T>)
            {
              constexpr auto c = avnd::get_range<T>();
              if_possible(ctl.value = c.init) // Default case
              else if_possible(ctl.value = c.values[c.init]) // For string enums

              if_possible(ctl.update(state.effect));
            }
      });
  }
}

/**
 * @brief Used for the case where the "host" can handle any
 * range of value: the plug-in will report the range,
 * and then we just have to clip to be safe
 */
template <typename T>
static constexpr void apply_control(T& ctl, std::floating_point auto v)
{
  // Apply the value
  ctl.value = v;

  // Clamp
  if constexpr (avnd::parameter_with_minmax_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();
    if (ctl.value < c.min)
      ctl.value = c.min;
    else if (ctl.value > c.max)
      ctl.value = c.max;
  }
}
/*
static void apply_control(auto& ctl, std::string&& v)
{
  // Apply the value
  ctl.value = std::move(v);

  // Clamp in range
  TODO;
}
*/
/**
 * @brief Used for the case where the "host" works in a fixed [0. ; 1.] range
 */
template <avnd::float_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  if constexpr (avnd::has_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();
    return c.min + v * (c.max - c.min);
  }
  else
  {
    return v;
  }
}
template <avnd::int_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  if constexpr (avnd::has_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();
    return c.min + v * (c.max - c.min);
  }
  else
  {
    return v;
  }
}

template <avnd::string_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  return decltype(T::value){};
}

template <avnd::bool_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  return v < 0.5 ? false : true;
}

template <avnd::enum_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  int res = std::round(v * (avnd::get_enum_choices_count<T>() - 1));
  return static_cast<decltype(T::value)>(res);
}

template <typename T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  static_assert(std::is_void_v<T>, "Error: unhandled control type");
}

template <avnd::float_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  // Apply the value
  double v{};
  if constexpr (avnd::has_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();

    v = (value - c.min) / double(c.max - c.min);
  }
  else
  {
    v = value;
  }
  return v;
}

template <avnd::int_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  // Apply the value
  double v{};
  if constexpr (avnd::has_range<T>)
  {
    // TODO generalize
    static_assert(avnd::get_range<T>().max != avnd::get_range<T>().min);
    constexpr auto c = avnd::get_range<T>();

    v = (value - c.min) / double(c.max - c.min);
  }
  else
  {
    v = value;
  }
  return v;
}

template <avnd::string_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  double v{};
  // TODO if we have a range, use it. Otherwise.. ?
  return v;
}

template <avnd::bool_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  return value ? 1. : 0.;
}

template <avnd::enum_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  static_assert(avnd::get_enum_choices_count<T>() > 0);
  return ((static_cast<int>(value) + 0.5) / avnd::get_enum_choices_count<T>());
}

template <typename T>
static constexpr auto map_control_to_01(const auto& value)
{
  static_assert(std::is_void_v<T>, "Error: unhandled control type");
}

template <avnd::parameter T>
static constexpr auto map_control_to_01(const T& ctl)
{
  return map_control_to_01<T>(ctl.value);
}
}
