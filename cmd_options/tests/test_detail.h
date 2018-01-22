#ifndef CMD_OPTIONS_TEST_TEST_DETAIL_H
#define CMD_OPTIONS_TEST_TEST_DETAIL_H

#include "cmd_options.h"

#include <iostream>


namespace lemma {
namespace cmd_options {

template<typename CharT>
bool operator==(const basic_option_pack<CharT> &lhs,
  const basic_option_pack<CharT> &rhs)
{
  return lhs.value_provided == rhs.value_provided
    && lhs.prefix == rhs.prefix
    && lhs.raw_key == rhs.raw_key
    && lhs.packed_arguments == rhs.packed_arguments
    && lhs.value == rhs.value;
}

}
}

namespace co = lemma::cmd_options;

namespace detail {

#ifndef CMD_OPT_TEST_CHART
#define CMD_OPT_TEST_CHART char
#endif

#ifndef LITERAL_PREFIX
#define LITERAL_PREFIX
#endif

// call _LIT(...) to automatically add 'L' or other CharT literal
// qualifier to string literals
#define _CAT_LITERAL(x,y) x##y
#define _MAKE_LITERAL(lit,str) _CAT_LITERAL(lit,str)
#define _LIT(string) _MAKE_LITERAL(LITERAL_PREFIX,string)


typedef CMD_OPT_TEST_CHART check_char_t;

template<typename CharT>
struct std_stream_select;

template<>
struct std_stream_select<char> {
  static constexpr std::basic_ostream<char> &cout = std::cout;
  static constexpr std::basic_ostream<char> &cerr = std::cerr;
};

template<>
struct std_stream_select<wchar_t> {
  static constexpr std::basic_ostream<wchar_t> &cout = std::wcout;
  static constexpr std::basic_ostream<wchar_t> &cerr = std::wcerr;
};

template<typename CharT>
inline bool is_empty(const co::basic_option_description<CharT> &desc)
{
  return (
    !desc.unpack_option && !desc.mapped_key &&
    !desc.key_description && !desc.extended_description &&
    !desc.implicit_value &&
    !desc.implicit_value_description && !desc.make_value &&
    !desc.finalize);
}

/*
  Aligned to cmd_options.xlsx starting at row 2
*/
template<typename CharT>
inline bool is_mapped_isolated_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && desc.mapped_key && desc.key_description
    && !desc.make_value);
}

template<typename CharT>
inline bool is_hidden_mapped_isolated_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && desc.mapped_key && !desc.key_description
    && !desc.make_value);
}

template<typename CharT>
inline bool is_raw_isolated_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && !desc.mapped_key && desc.key_description
    && !desc.make_value);
}

/*
  Row 5
*/
template<typename CharT>
inline bool is_hidden_raw_isolated_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && !desc.mapped_key && !desc.key_description
    && !desc.make_value);
}

template<typename CharT>
inline bool is_mapped_reqired_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && desc.mapped_key && desc.key_description
    && desc.make_value && !desc.implicit_value);
}

template<typename CharT>
inline bool is_hidden_mapped_reqired_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && desc.mapped_key && !desc.key_description
    && desc.make_value && !desc.implicit_value);
}

template<typename CharT>
inline bool is_raw_reqired_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && !desc.mapped_key && desc.key_description
    && desc.make_value && !desc.implicit_value);
}

template<typename CharT>
inline bool is_raw_mapped_reqired_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && !desc.mapped_key && !desc.key_description
    && desc.make_value && !desc.implicit_value);
}

/*
  Row 10
*/
template<typename CharT>
inline bool is_mapped_optional_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && desc.mapped_key && desc.key_description
    && desc.make_value && desc.implicit_value);
}

template<typename CharT>
inline bool is_hidden_mapped_optional_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && desc.mapped_key && !desc.key_description
    && desc.make_value && desc.implicit_value);
}

template<typename CharT>
inline bool is_raw_optional_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && !desc.mapped_key && desc.key_description
    && desc.make_value && desc.implicit_value);
}

template<typename CharT>
inline bool is_raw_mapped_optional_option(
  const co::basic_option_description<CharT> &desc)
{
  return (desc.unpack_option && !desc.mapped_key && !desc.key_description
    && desc.make_value && desc.implicit_value);
}

/*
  In the EZ interface for row 14, 15, 18, and 19 mapped key is set to
  allow for position and argument number constraints. That is row 14 and
  row 15 are actually another occurrences of row 16 and row 17 and row
  18 and 19 are another occurrences of row 20 and 21. The mapped key
  simply provides the raw key which is the default behavior if it wasn't
  present. So this check is not used and here for completeness only.
*/
template<typename CharT>
inline bool is_interpret_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && !desc.mapped_key && desc.extended_description
    && desc.make_value && !desc.implicit_value);
}

/*
  Row 15
*/
template<typename CharT>
inline bool is_hidden_interpret_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && !desc.mapped_key && !desc.extended_description
    && desc.make_value && !desc.implicit_value);
}

template<typename CharT>
inline bool is_keyed_interpret_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && desc.mapped_key && desc.extended_description
    && desc.make_value && !desc.implicit_value);
}

template<typename CharT>
inline bool is_hidden_keyed_interpret_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && desc.mapped_key && !desc.extended_description
    && desc.make_value && !desc.implicit_value);
}

template<typename CharT>
inline bool is_empty_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && !desc.mapped_key && desc.extended_description
    && !desc.make_value);
}

template<typename CharT>
inline bool is_hidden_empty_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && !desc.mapped_key && !desc.extended_description
    && !desc.make_value);
}

/*
  Row 20
*/
template<typename CharT>
inline bool is_empty_keyed_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && desc.mapped_key && desc.extended_description
    && !desc.make_value);
}

template<typename CharT>
inline bool is_hidden_empty_keyed_operand(
  const co::basic_option_description<CharT> &desc)
{
  return (!desc.unpack_option && desc.mapped_key && !desc.extended_description
    && !desc.make_value);
}











template<typename CharT>
inline std::basic_string<CharT>
to_string(const co::basic_option_description<CharT> &desc)
{
  std::basic_stringstream<CharT> out;
  out
    << _LIT("unpack_option: ") << bool(desc.unpack_option)
      << _LIT("\n")
    << _LIT("mapped_key: ")
      << bool(desc.mapped_key) << _LIT("\n")
    << _LIT("key_description: ") << bool(desc.key_description)
      << _LIT("\n")
    << _LIT("extended_description: ") << bool(desc.extended_description)
      << _LIT("\n")
    << _LIT("implicit_value: ") << bool(desc.implicit_value)
      << _LIT("\n")
    << _LIT("implicit_value_description: ")
      << bool(desc.implicit_value_description) << _LIT("\n")
    << _LIT("make_value: ") << bool(desc.make_value)
      << _LIT("\n")
    << _LIT("finalize: ") << bool(desc.finalize)
      << _LIT("\n");

  return out.str();
}

template<typename CharT, typename T>
std::basic_string<CharT>
to_string(const co::basic_variable_map<CharT> &vm, const co::value<T> &)
{
  std::basic_stringstream<CharT> out;

  for(auto && pair : vm) {
    out << pair.first << _LIT(" -> ");
    if(co::is_empty(pair.second))
      out << _LIT("[empty]\n");
    else
      out << co::any_cast<T>(pair.second) << _LIT("\n");
  }

  return out.str();
}


template<typename CharT>
bool check_exclusive(const co::basic_option_description<CharT> &desc,
  bool(*fn)(const co::basic_option_description<CharT> &))
{
  typedef bool(*fn_t)(const co::basic_option_description<CharT> &);
  typedef std::pair<std::basic_string<CharT>,fn_t> pair_type;

  static const std::vector<pair_type> cases{
    {_LIT("is_mapped_isolated_option"),
      is_mapped_isolated_option<CharT>},
    {_LIT("is_hidden_mapped_isolated_option"),
      is_hidden_mapped_isolated_option<CharT>},
    {_LIT("is_raw_isolated_option"),
      is_raw_isolated_option<CharT>},
    {_LIT("is_hidden_raw_isolated_option"),
      is_hidden_raw_isolated_option<CharT>},
    {_LIT("is_mapped_reqired_option"),
      is_mapped_reqired_option<CharT>},
    {_LIT("is_hidden_mapped_reqired_option"),
      is_hidden_mapped_reqired_option<CharT>},
    {_LIT("is_raw_reqired_option"),
      is_raw_reqired_option<CharT>},
    {_LIT("is_raw_mapped_reqired_option"),
      is_raw_mapped_reqired_option<CharT>},
    {_LIT("is_mapped_optional_option"),
      is_mapped_optional_option<CharT>},
    {_LIT("is_hidden_mapped_optional_option"),
      is_hidden_mapped_optional_option<CharT>},
    {_LIT("is_raw_optional_option"),
      is_raw_optional_option<CharT>},
    {_LIT("is_raw_mapped_optional_option"),
      is_raw_mapped_optional_option<CharT>},
    {_LIT("is_interpret_operand"),
      is_interpret_operand<CharT>},
    {_LIT("is_hidden_interpret_operand"),
      is_hidden_interpret_operand<CharT>},
    {_LIT("is_keyed_interpret_operand"),
      is_keyed_interpret_operand<CharT>},
    {_LIT("is_hidden_keyed_interpret_operand"),
      is_hidden_keyed_interpret_operand<CharT>},
    {_LIT("is_empty_operand"),
      is_empty_operand<CharT>},
    {_LIT("is_hidden_empty_operand"),
      is_hidden_empty_operand<CharT>},
    {_LIT("is_empty_keyed_operand"),
      is_empty_keyed_operand<CharT>},
    {_LIT("is_hidden_empty_keyed_operand"),
      is_hidden_empty_keyed_operand<CharT>}
  };

  std::vector<pair_type> exclusive = cases;
  typename std::vector<pair_type>::iterator loc =
    std::remove_if(exclusive.begin(),exclusive.end(),
      [&](const pair_type &val) {
        return val.second == fn;
      }
    );

  assert(loc != exclusive.end());

  exclusive.erase(loc,exclusive.end());

#if 1
  if(!fn(desc)) {
    std_stream_select<CharT>::cerr
      << _LIT("Given function returned false\n")
      << to_string(desc) << _LIT("\n");
    return false;
  }

  for(std::size_t i=0; i<exclusive.size(); ++i) {
    if(exclusive[i].second(desc)) {
      std_stream_select<CharT>::cerr
        << _LIT("check_exclusive is true for : ") << exclusive[i].first
        << _LIT("\n") << to_string(desc) << ("\n");
      return false;
    }
  }
  return true;
#endif

  return fn(desc) && std::none_of(exclusive.begin(),exclusive.end(),
    [&](const pair_type &val) {return val.second(desc);});
}

template<typename T, typename VariableMap>
bool contents_equal(const VariableMap &lhs, const VariableMap &rhs)
{
  typedef typename VariableMap::key_type::value_type char_type;
  typename VariableMap::const_iterator lcur = lhs.begin();
  typename VariableMap::const_iterator rcur = rhs.begin();

  while(lcur != lhs.end() && rcur != rhs.end()) {
    if(lcur->first != rcur->first) {
      std_stream_select<char_type>::cerr
        << _LIT("lhs vm key '") << lcur->first
        << _LIT("' != rhs vm key '") << rcur->first
        << _LIT("'\n");
      return false;
    }

    if(!(co::is_empty(lcur->second) && co::is_empty(rcur->second))) {
      if(co::is_empty(lcur->second) && !co::is_empty(rcur->second)) {
        std_stream_select<char_type>::cerr
          << _LIT("lhs vm value for key '") << lcur->first
          << _LIT("' is empty but rhs vm value for key '")
          << rcur->first << _LIT("' is not\n");
        return false;
      }

      if(!co::is_empty(lcur->second) && co::is_empty(rcur->second)) {
        std_stream_select<char_type>::cerr
          << _LIT("lhs vm value for key '") << lcur->first
          << _LIT("' is not empty but rhs vm value for key '")
          << rcur->first << _LIT("' is\n");
        return false;
      }

      try {
        if(co::any_cast<T>(lcur->second) != co::any_cast<T>(rcur->second))
          return false;
      }
      catch(const co::bad_any_cast &ex) {
        std_stream_select<char_type>::cerr
          << _LIT("vm values are not given type T: ") << ex.what()
          << _LIT("\n");
        throw;
      }
    }

    ++lcur;
    ++rcur;
  }

  return lcur == lhs.end() && rcur == rhs.end();
}

}

#endif
