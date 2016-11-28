#ifndef MEDUSA_BITFIELD_HPP
#define MEDUSA_BITFIELD_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

#include <type_traits>

// src: https://github.com/TheWisp/SC2API/blob/master/Shared/Flag.h
#define MEDUSA_ENUM_CLASS_BITMASK(Enum) \
  inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = static_cast<Enum>(static_cast<std::underlying_type<Enum>::type>(Lhs) | static_cast<std::underlying_type<Enum>::type>(Rhs)); } \
  inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = static_cast<Enum>(static_cast<std::underlying_type<Enum>::type>(Lhs) & static_cast<std::underlying_type<Enum>::type>(Rhs)); } \
  inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = static_cast<Enum>(static_cast<std::underlying_type<Enum>::type>(Lhs) ^ static_cast<std::underlying_type<Enum>::type>(Rhs)); } \
  inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return static_cast<Enum>(static_cast<std::underlying_type<Enum>::type>(Lhs) | static_cast<std::underlying_type<Enum>::type>(Rhs)); } \
  inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return static_cast<Enum>(static_cast<std::underlying_type<Enum>::type>(Lhs) & static_cast<std::underlying_type<Enum>::type>(Rhs)); } \
  inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return static_cast<Enum>(static_cast<std::underlying_type<Enum>::type>(Lhs) ^ static_cast<std::underlying_type<Enum>::type>(Rhs)); } \
  inline constexpr bool  operator! (Enum  E)             { return !static_cast<std::underlying_type<Enum>::type>(E); } \
  inline constexpr Enum  operator~ (Enum  E)             { return static_cast<Enum>(~static_cast<std::underlying_type<Enum>::type>(E)); } \
  inline           bool  to_bool(Enum E)                 { return static_cast<std::underlying_type<Enum>::type>(E) != 0 ? true : false; }

#endif // !MEDUSA_BITFIELD_HPP
