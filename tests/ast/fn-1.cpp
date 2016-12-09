// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/base/module.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/printing/print.hpp>

#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/logic/construction/builder.hpp>

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/numeric/construction/builder.hpp>

#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/core/construction/builder.hpp>

#include <llvm/IR/Type.h>
#include <llvm/IR/Module.h>

#include <iostream>


using namespace beaker;

int
main(int argc, char* argv[])
{
  // Define the module.
  sequential_allocator<> alloc;
  symbol_table syms;
  module mod(alloc, syms);
  
  logic::builder& lb = mod.get_builder<logic::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();
  core::builder& cb = mod.get_builder<core::builder>();

  type& b = lb.get_bool_type();
  type& i32 = nb.get_int_type(32);
  type& i1024 = nb.get_int_type(1024);

  expr& t = lb.get_true_expr();

  // Some declarations
  decl* vars[] {
    &cb.make_var_decl(cb.get_name("a"), i32),
    &cb.make_var_decl(cb.get_name("b"), b),
    &cb.make_var_decl(cb.get_name("c"), i1024),
    &cb.make_var_decl(cb.get_name("r"), i32)
  };

  { // f1 : (int32, bool, int1024) -> i32
    decl_seq parms {vars[0], vars[1], vars[2]};
    decl& ret = *vars[3];
    type& type = cb.get_fn_type(parms, ret);
    decl& fn = cb.make_fn_decl(cb.get_name("f1"), type, parms, ret, t);
    mod.add_declaration(fn);
  }

  { // f2 : (bool) -> int1024
    decl_seq parms {vars[1]};
    decl& ret = *vars[2];
    type& type = cb.get_fn_type(parms, ret);
    decl& fn = cb.make_fn_decl(cb.get_name("f2"), type, parms, ret, t);
    mod.add_declaration(fn);
  }

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);

  // std::cout << "bool: " << gen.get_type(b).is_indirect() << '\n';
  // std::cout << "i32: " << gen.get_type(i32).is_indirect() << '\n';

  gen.get_module().dump();
}