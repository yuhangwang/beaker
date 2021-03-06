// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_FWD_HPP
#define BEAKER_SYS_VAR_FWD_HPP


namespace beaker {
namespace sys_var {

struct feature;
#define def_type(T) struct T##_type;
#define def_expr(E) struct E##_expr;
#define def_init(E) struct E##_init;
#define def_decl(D) struct D##_decl;
#include "type.def"
#include "expr.def"
#include "decl.def"
struct builder;

} // namespace sys_var
} // namespace beaker


#endif
