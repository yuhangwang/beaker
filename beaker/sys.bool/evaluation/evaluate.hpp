// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_EVALUATION_EVALUATE_HPP
#define BEAKER_SYS_BOOL_EVALUATION_EVALUATE_HPP

#include <beaker/sys.bool/fwd.hpp>

#include <beaker/base/evaluation/evaluate.hpp>

namespace beaker {
namespace sys_bool {

// -------------------------------------------------------------------------- //
// Errors

/// An assertion error occures when the operand of an assertion is false.
struct assertion_error : evaluation_error
{
  assertion_error(const expr&);

  const expr* expr_;
};

inline 
assertion_error::assertion_error(const expr& e)
  : evaluation_error("assert"), expr_(&e) 
{ }


// -------------------------------------------------------------------------- //
// Overrides

value evaluate(evaluator&, const bool_expr&);
value evaluate(evaluator&, const and_expr&);
value evaluate(evaluator&, const or_expr&);
value evaluate(evaluator&, const xor_expr&);
value evaluate(evaluator&, const not_expr&);
value evaluate(evaluator&, const imp_expr&);
value evaluate(evaluator&, const eq_expr&);
value evaluate(evaluator&, const if_expr&);
value evaluate(evaluator&, const and_then_expr&);
value evaluate(evaluator&, const or_else_expr&);
value evaluate(evaluator&, const assert_expr&);

} // namespace sys_bool
} // namespace beaker


#endif
