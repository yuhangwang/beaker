// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "type.hpp"
#include "expr.hpp"


namespace beaker {
namespace sys_bool {

builder::builder(module& m)
  : factory(m),
    bool_(&make_singleton_set<bool_type>(get_language_allocator())) 
{ }

/// Returns a new boolean type.
bool_type&
builder::get_bool_type()
{
  return bool_->get();
}

/// Returns a new boolean literal `true`.
bool_expr&
builder::make_true_expr()
{
  return make<bool_expr>(get_bool_type(), value(1));
}

// Returns a new boolean literal `false`.
bool_expr&
builder::make_false_expr()
{
  return make<bool_expr>(get_bool_type(), value(0));
}

// Returns a new literal corresponding to the integer value `v`. The literal 
// is `true` when `v` is nonzero and `false` otherwise. The type of the 
// expression is `bool`.
bool_expr&
builder::make_bool_expr(const value& v)
{
  assert(v.is_integer());
  if (v.get_int())
    return make_true_expr();
  else
    return make_false_expr(); 
}

/// Returns a new expression `e1 & e2`.
and_expr&
builder::make_and_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<and_expr>(get_bool_type(), e1, e2);
}

/// Returns a new expression `e1 | e2`.
or_expr&
builder::make_or_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<or_expr>(get_bool_type(), e1, e2);
}

/// Returns a new expression `e1 ^ e2`.
xor_expr&
builder::make_xor_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<xor_expr>(get_bool_type(), e1, e2);
}

/// Returns a new expression `!e1`
not_expr&
builder::make_not_expr(expr& e1)
{
  assert(is_boolean_expression(e1));
  return make<not_expr>(get_bool_type(), e1);
}

/// Returns a new expression `e1 => e2`.
imp_expr&
builder::make_imp_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<imp_expr>(get_bool_type(), e1, e2);
}

/// Returns a new expression `e1 <=> e2`.
eq_expr&
builder::make_eq_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<eq_expr>(get_bool_type(), e1, e2);
}

/// Returns a new expression `e1 ? e2 : e3`.
if_expr&
builder::make_if_expr(expr& e1, expr& e2, expr& e3)
{
  assert(is_boolean_expression(e1));
  assert(equal(e2.get_type(), e3.get_type()));
  return make<if_expr>(e2.get_type(), e1, e2, e3);
}

/// Returns a new expression `e1 && e2`.
and_then_expr&
builder::make_and_then_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<and_then_expr>(get_bool_type(), e1, e2);
}

/// Returns a new expression `e1 || e2`.
or_else_expr&
builder::make_or_else_expr(expr& e1, expr& e2)
{
  assert(is_boolean_expression(e1));
  assert(is_boolean_expression(e2));
  return make<or_else_expr>(get_bool_type(), e1, e2);
}

/// Returns a new declaration `assert e` with module context.
assert_expr&
builder::make_assert_expr(expr& e1)
{
  assert(is_boolean_expression(e1));
  return make<assert_expr>(get_bool_type(), e1);
}

} // namespace sys_bool
} // namespace beaker
