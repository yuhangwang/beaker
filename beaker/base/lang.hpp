// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_LANGUAGE_HPP
#define BEAKER_BASE_LANGUAGE_HPP

#include <cassert>
#include <typeindex>
#include <unordered_map>


namespace beaker {

struct name;
struct type;
struct expr;
struct decl;
struct stmt;


/// Represents an algorithm that operates on an AST.
///
/// This is an abstract base class for concrete algorithms. Each class deriving
/// from this class defines an algorithm that operates on a subset of nodes
/// of the entire language. A language feature may subclass that algorithm
/// in order to provide a dispatch table for the terms it defines.
///
/// Note that each deriving class must provide an empty nested type named 
/// `tag`, which is used to provide unique type information for the algorithm.
/// This is used for storage and lookup by the feature.
struct algorithm
{
  virtual ~algorithm() = default;
};


/// A feature defines a container of algorithms that operate on a related
/// set of terms. This essentially defines a collection of dispatch tables
/// for those algorithms.
struct feature
{
  using algorithm_set = std::unordered_map<std::type_index, algorithm*>;

  feature(int);
  virtual ~feature();

  int get_id() const;

  template<typename T>
  void add_algorithm();

  template<typename T>
  const T& get_algorithm() const;

  int id_;
  algorithm_set algos_;
};

inline feature::feature(int id) : id_(id) { }

inline feature::~feature() { for (auto p : algos_) delete p.second; }

/// Returns the id 
inline int feature::get_id() const { return id_; }

/// Adds a new algorithm of the given type.
template<typename T>
void
feature::add_algorithm()
{
  using X = typename T::tag;
  assert(algos_.count(typeid(X)) == 0);
  algos_.emplace(typeid(X), new T());
}

/// Returns the algorithm with the given type. 
template<typename T>
const T&
feature::get_algorithm() const
{
  using X = typename T::tag;
  assert(algos_.count(typeid(X)) != 0);
  algorithm* algo = algos_.find(typeid(X))->second;
  return *static_cast<T*>(algo);
}


/// A helper class that provides the feature id as an integer constant.
template<int N>
struct basic_feature : feature
{
  static constexpr int feature_id = N;

  basic_feature();
};

template<int N>
inline basic_feature<N>::basic_feature() : feature(N) { }


// -------------------------------------------------------------------------- //
// Language

/// Represents a set of language features.
///
/// This class is a singleton. Exactly one object must be created (typically
/// in main(), and that can be used throughout a program.
///
/// A client program must register the set of features needed by the language
/// at program startup.
struct language
{
  using feature_set = std::unordered_map<int, feature*>;

  language();
  ~language();

  static language& get_instance();

  template<typename T>
  void add_feature();

  static feature& get_feature(int);
  static feature& get_feature(const name&);
  static feature& get_feature(const type&);
  static feature& get_feature(const expr&);
  static feature& get_feature(const decl&);
  static feature& get_feature(const stmt&);

  feature_set feat_;
};

/// Add the feature to the language. If the feature has already been added
/// then no action is taken.
template<typename T>
inline void
language::add_feature()
{
  feature *f = new T();
  feat_.emplace(f->get_id(), f);
}

// -------------------------------------------------------------------------- //
// Language and term identification

// Maximum number of terms per kind per language subset. Note that
// 100 is a very generous allocation.
constexpr int lang_block_size = 100;


// Assign a unique identifier to each language.
enum {
#define def_lang(l) l##_lang,
#include "lang.def"
  last_lang
};


// Subsets of languages are uniquely identified by one of these values.
// Nodes (of different kind) are grouped into blocks of 100 (for now).
enum {
#define def_lang(l) l##_lang_block = l##_lang * lang_block_size,
#include "lang.def"
};


/// Returns the language classifier for the kind.
constexpr int
get_language(int k) { return k / lang_block_size; }


} // namespace


#endif
