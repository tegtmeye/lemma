/**
 *  Copyright (c) 2017, Mike Tegtmeyer
 *  All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of the author nor the names of its contributors may
 *        be used to endorse or promote products derived from this software
 *        without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LEMMA_CMD_OPTIONS_H
#define LEMMA_CMD_OPTIONS_H

#if __cplusplus < 201703L
#include <boost/any.hpp>
#endif

#include <map>
#include <regex>
#include <tuple>
#include <utility>
#include <vector>
#include <stack>
#include <string>
#include <cassert>
#include <sstream>
#include <codecvt>
#include <functional>

#include <iostream>

namespace lemma {

namespace cmd_options {

/*
  Currently uses boost::any but will eventually transition to std::any in C++17
*/
#if __cplusplus < 201703L
  using any = boost::any;

  /*
    Checking if any has a value is different between boost and C++17.

    Provide an abstraction
  */
  inline bool is_empty(const any &_val)
  {
    return _val.empty();
  }

  /*
    Convenience syntax for any_cast between boost::any and std::any

    Would rather use a "using any_cast = boost::any_cast" syntax but aliases
    are not available for functions
  */
  template<typename T, typename AnyT>
  inline T any_cast(AnyT && _val)
  {
    return boost::any_cast<T>(std::forward<AnyT>(_val));
  }

  /*
    Convenience for bad_any_cast between boost::any and std::any
  */
  using bad_any_cast = boost::bad_any_cast;
#else
  using any = std::any;
#endif

namespace detail {

// In the future (post C++14) use std::mismatch
template<class InputIt1, class InputIt2>
std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
    while (first1 != last1 && first2 != last2 && *first1 == *first2) {
        ++first1, ++first2;
    }
    return std::make_pair(first1, first2);
}

}

/*
  Exception class hierarchy inherited from std::runtime error used to
  indicate command option errors.

  In the single argument versions, the \c option or \c argument method
  is simply a mapping to \c std::runtime_error's what() method. In
  \c unexpected_argument_error, the unexpected argument is contained in
  a private string accessible via the \c argument method.
*/
class command_option_error : public std::runtime_error {
  public:
    command_option_error(const std::string &what)
        :std::runtime_error(what) {}
};

class unknown_option_error : public command_option_error {
  public:
    unknown_option_error(const std::string &option)
        :command_option_error(option) {}

    const char * option(void) const noexcept {
      return what();
    }
};

class missing_argument_error : public command_option_error {
  public:
    missing_argument_error(const std::string &option)
        :command_option_error(option) {}

    const char * option(void) const noexcept {
      return what();
    }
};

class unexpected_argument_error : public command_option_error {
  public:
    unexpected_argument_error(const std::string &option,
      const std::string &arg) :command_option_error(option), _arg(arg) {}

    const char * option(void) const noexcept {
      return what();
    }

    const char * argument(void) const noexcept {
      return _arg.c_str();
    }

  private:
    std::string _arg;
};

class unexpected_positional_error : public command_option_error {
  public:
    unexpected_positional_error(const std::string &arg)
        :command_option_error(arg) {}

    const char * positional(void) const noexcept {
      return what();
    }
};






template<typename CharT>
using basic_variable_map = std::multimap<std::basic_string<CharT>,any>;

/*
  Structure describing an unpacked argument.

  bool value_provided;
  string_type prefix;
  string_type raw_key;
  std::vector<string_type> packed_arguments;
  string_type value;

  The elements represent:
    - \c value_provided A boolean value indicating that the argument
      contained a packed value. This is necessary because it is possible
      to explicitly provide an empty value

    - \c prefix A string specifying the option prefix, ie '-' or '--'

    - \c raw_key A string containing the raw key. Return the empty string to
    indicate that this option does not handle this option.

    - \c packed_arguments A vector of strings containing any remaining
    packed arguments if present. If the arguments require a prefix, they
    must be prepended to each argument otherwise it will be considered a
    positional argument. For example, if you wish to allow "-abcd" to be
    equivalent to "-a -b --cat=dog -d", then the raw key is "a" and the
    packed options are: {"-b","--cat=dog","d"}. An appropriate unpack
    method could add packed arguments for other reasons. For example, if
    "-b" always implies a hidden (deprecated) "--bar_old", then raw_key
    is "b" and include "--bar_old" to the packed arguments;

    - \c value A string containing the value if present.



  This structure is intended to represent an unpacked argument, not it's
  semantic value and is a byproduct of the may different ways that an
  option argument can be packed together. For example: here are some
  known ways that option arguments can be represented:

      1) --foo bar    = long option, key=foo, value=bar
                      : prefix = "--"
                      : raw_key = "foo"
                      : packed_arguments = ""
                      : value = ""
                      : value_provided = false.

      2) --foo=bar    = long option, key=foo, value=bar
                      : prefix = "--"
                      : raw_key = "foo"
                      : packed_arguments = ""
                      : value = "bar"
                      : value_provided = true

      3) -foo bar     = long option, key=foo, value=bar
                      : prefix = "-"
                      : raw_key = "foo"
                      : packed_arguments = ""
                      : value = ""
                      : value_provided = false

      2) -foo=bar     = long option, key=foo, value=bar
                      : prefix = "-"
                      : raw_key = "foo"
                      : packed_arguments = ""
                      : value = "bar"
                      : value_provided = true

      5) -f bar       = short option, key=f, value=bar
                      : prefix = "-"
                      : raw_key = "f"
                      : packed_arguments = ""
                      : value = ""
                      : value_provided = false

      6) -fBar        = short option, key=f, value=Bar
                      : prefix = "-"
                      : raw_key = "f"
                      : packed_arguments = ""
                      : value = "Bar"
                      : value_provided = true

      7) -fbar        = 4 short options with keys: 'f', 'b', 'a', and 'r'
                      : prefix = "-"
                      : raw_key = "f"
                      : packed_arguments = {"-b","-a","-r"}
                      : value = ""
                      : value_provided = false

      8) -nobar       = long flag, key=bar, value=false
                      : prefix = "-"
                      : raw_key = "bar"
                      : packed_arguments = ""
                      : value = "false"
                      : value_provided = true
*/
template<typename CharT>
struct basic_option_pack {
  typedef std::basic_string<CharT> string_type;
  typedef std::vector<string_type> packed_arg_seq;

  bool value_provided;
  string_type prefix;
  string_type raw_key;
  packed_arg_seq packed_arguments;
  string_type value;
};

template<typename CharT>
std::ostream & operator<<(std::ostream &out,
  const basic_option_pack<CharT> &option_pack)
{
  out << "process opt:\n"
    << "\tvalue_provided: '" << option_pack.value_provided << "'\n"
    << "\tprefix: '" << option_pack.prefix << "'\n"
    << "\traw_key: '" << option_pack.raw_key << "'\n"
    << "\tpacked_arguments: {";
  for(auto &&arg : option_pack.packed_arguments)
    out << "'" << arg << "' ";
  out
    << "}\n"
    << "\tvalue: '" << option_pack.value << "'\n";

  return out;
}

/*
  A description for a single option
*/
template<typename CharT>
struct basic_option_description {
  typedef CharT                     char_type;
  typedef std::basic_string<CharT>  string_type;
  typedef basic_variable_map<CharT> variable_map;
  typedef basic_option_pack<CharT>  option_pack;

  /*
    Unpack the raw option pack. If \c unpack_option is not
    provided, then the this description is assumed to describe a
    positional argument.

    If the option cannot be unpacked by this description, then return
    the empty string in the \c raw_key position (or an empty option_pack).

    This function is intended to essentially parse the option
    into its constitute properties but it can also be used to inject
    hidden options that can be useful. For example, if --foo is given
    implies --bar, even if --bar was not provided, then the unpacked
    option for "foo" can return "bar" in the \c packed_arguments field.

    A note on positional values... If the options group _only_ contains
    descriptions that do not have the \c unpack_option set, then any
    argument is considered a positional---even if looks like an option.
    For example, if there are no option_descriptions with
    \c unpack_option set, then '--foo', which looks like a option flag,
    is considered a positional. This may not be what was intended.
    Reasonable machinery in this case is to include a 'dummy' option
    that has \c unpack_option set and where \c mapped_key throws a
    \c command_option_error indicating the unknown option.
  */
  std::function<option_pack(const string_type &option)> unpack_option;

  /*
    For the given raw key (without any prefix), return the mapped_key
    that is placed in the the corresponding variable_map if the raw_key
    is associated with this option_description or the empty string
    otherwise. If \c mapped_key is not set, then this description will
    use the raw key as the mapped_key. The byproduct of omitting \c
    mapped_key is that this option_description will match any option
    that the provided \c unpack_option accepts. For example, to accept
    all unknown options, then provide an option_description with the
    appropriate \c unpack_option and no \c mapped_key set as the
    last description in an \c options_group.
  */
  std::function<string_type(const string_type &raw_key,
    const variable_map &vm)> mapped_key;

  /*
    Return the human-readable description for this option. This does not
    necessarily need to be the result provided in \c mapped_key
    (although it often does). The value is not used for options parsing
    but typically used in the help message. For example, supposed this
    option is given by --foo, return "--foo". This value can be any
    meaningful string based on the need. For example, suppose for some
    nonstandard syntax the key --foo1bar --foo2bar is possible, a
    reasonable response from this function could be: "foo[integer]bar".
    Another example is in the case of long and short options. Typically
    the short option is hidden and the long option is fully specified
    with a description containing the associated short option. That is:
    --foo, -f.
  */
  std::function<string_type(void)> key_description;

  /*
    Return a human-readable concise description of this option's
    functionality.
  */
  std::function<string_type(void)> extended_description;

  /*
    If \c unpack_option is not provided, then the option is a \c
    positional option. In this case, the option may mean different
    things based on where it exists in the option list. The argument to
    the given function is the number of the positional option not
    including the non-positional options. For example, in the option
    list where '--foo', '--bar', '-f', and '-g' do not take options:
      "--foo one --bar two -f -g three"
    the positional options are: one, two, and three with position 1, 2,
    and 3 respectively. If set, the function can return a unique key
    that will appear in the variable_map for each positional option or
    the same if users choose to. If the function is not set, then all
    positional options will be available using the default-constructed
    \c string_type as the variable_map key. Like \c mapped_key, if this
    description does not define the given positional option, then return
    the empty string. For example, if this description handles only the
    XXth positional option.
  */
  std::function<string_type(std::size_t i)> implicit_key;


  /*
    Non-positional option cases:
      -- strictly do not provide a value (no make_value)
        -- option has an implicit, possibly empty, value (uses implicit_value)
        -- no implicit value, use default constructed

      -- provided value is optional (make_value exists)
        -- if given, option uses provided value (calls make_value)
        -- if missing, use an implicit, possibly empty, value
            (uses implicit_value)
        -- no implicit value, use default constructed

      -- providing a value is mandatory (make_value exists, no implicit_value)
        -- use the one provided (calls make_value)

    If set, return the implicit value of the option as a any
    object. This value is used whenever the option either explicitly
    forbids a value or it is optional and one is not provided. If not
    set, the value defaults to \c string_type().

    The function's first parameter is the string given as the option's
    key exactly as provided to the option without the long- or
    short_option_flag prefix. This is useful to deal with nonstandard
    syntaxes. For example, given -frtti vs -fno-rtti, the variable_map
    key could be 'rtti' with a boolean value. The option forbids values
    so the mapped value must be determined by the key (specifically the
    presence or absence of the 'no' prefix.
  */
  std::function<
    any(const string_type &key, const variable_map &vm)> implicit_value;

  /*
    Return the human-readable description for this option's implicit
    value. This description is not used for parsing but rather automatic
    help messages. For example, suppose the option --foo has an implicit
    value of '5' if another value was not provided. A reasonable return
    value from this function would be "5".
  */
  std::function<string_type(void)> implicit_value_description;

  /*
    Positional option case:
      -- interpret the option value (make_value is set)
      -- use default constructed value (make_value is not set)

    If set, return the value of the argument as a any as
    determined by the contents of the second parameter. If not set, the
    option explicitly forbids option values. I.e. --foo=bar or --foo bar
    will return an error unless the 'bar' is a valid positional option
    in the later case.

    The function's first parameter is the string given as the option's
    mapped_key exactly as provided by the \c map_long_key and/or \c
    map_short_key function. This can be useful for checking the state of
    the vm and throwing an error if too many instances of a certain
    option has been given or if certain combinations of options are
    mutually-exclusive or inclusive.

    If the option was a positional one, then the
    first parameter is the result of \c implicit_key if set or a
    default-constructed string_type if not set.

    The function's second parameter is the string given as the options
    value. It is possible for this value to be empty in the case where
    the given option is explicitly set to be empty. For example, the
    value of --foo "" is explicitly given (not missing) to be the empty
    string. The \c make_value function must be prepared to handle this
    case. This does not happen in the case of compound arguments. For
    example, the values in  -f"" and --foo="" are considered missing.

    The provided variable map may be used to selectively set the value
    based on the current state of the argument parsing or examine the
    current contents of the map to ensure validity before returning the
    value. For example, if only a certain number of options with the
    same key are valid or if certain combinations of options are
    mutually-exclusive.

    command_option_error substitution is available for '%@', '%L', '%S'
  */

  std::function<
    any(const string_type &mapped_key, const string_type &value,
      const variable_map &vm)> make_value;

  /*
    If set, this function is called for each option_description by the
    parse_* functions after all options are parsed. A typical use of
    this function is to inspect the state of the variable_map argument
    for option-specific inconsistencies or illegal combinations. For
    example, if a minimum number of options are expected, the
    variable_map can be queried to see if it has been satisfied and
    throw an error if not.

    command_option_error substitution is available for '%L' and '%S'
  */
  std::function<void(const variable_map &vm)> finalize;

};

template<typename CharT>
using basic_options_group = std::vector<basic_option_description<CharT> >;


/*
  POSIX flag syntax. Unpack alphanumeric arguments in the form:

    "-f", "-fabcde"

  if uses_packed_flags is true then:

    "-fabcde" is equivalent to "-f -a -b -c -d -e"

  otherwise

    "f" is the key and "abcde" is the value

  The Single Unix Specification Utility Syntax Guideline 3 states that a
  POSIX option name should be a single alphanumeric character from the
  portable character set. A question then arises what does it mean when
  a non-alphanumeric character is encountered? If the unpack function
  chooses to report that it cannot unpack the option, then the option
  may get picked up as a positional which would be incorrect unless
  there is some "fall-through" handler that intercepts the incorrect
  option and indicates the error.  If, on the other hand, this
  unpack-function reports it as an error, then it precludes the
  possibility of some later, non-SUS conforming unpack function handling
  the option. The solution here is that this function will unpack the
  flag and let the option description determine if it will accept the
  non-conforming option or not.

  N.B. any leading and trailing whitespace is assumed to be handled
    appropriately prior to this call. That is, " -f" is not the same as "-f".
*/
template<bool uses_packed_flags, typename CharT>
basic_option_pack<CharT> unpack_posix(const std::basic_string<CharT> &str)
{
  typedef basic_option_pack<CharT> option_pack;
  typedef typename option_pack::string_type string_type;
  typedef typename option_pack::packed_arg_seq packed_arg_seq;

  static const string_type sprefix("-");

  auto &&res =
    detail::mismatch(sprefix.begin(),sprefix.end(),str.begin(),str.end());
  if(res.first != sprefix.end() || res.second == str.end())
    return option_pack();

  if(res.second+1 == str.end())
    return option_pack{false,sprefix,{res.second,res.second+1},{},{}};

  if(uses_packed_flags) {
    string_type raw_key{res.second,++res.second};
    packed_arg_seq packed_arguments;
    while(res.second != str.end())
      packed_arguments.push_back(sprefix+(*(res.second++)));

    return option_pack{false,sprefix,raw_key,packed_arguments};
  }

  return option_pack{true,sprefix,{res.second,res.second+1},{},
    {res.second+1,str.end()}};
}

/*
  GNU extension (long option) to POSIX argument syntax. Unpack arguments in the
  form:

  POSIX:
    if uses_packed_flags is true then:

      "-fabcde" is equivalent to "-f -a -b -c -d -e"

    otherwise

      "f" is the key and "abcde" is the value

  GNU:
    "--key"
    "--key=value"

    where:

      first form: "key" is the key and "" is the value
      second form: "key" is the key and "value" is the value

  Herein lies a design decision derived from ambiguity in the GNU long
  option specification (if one really exists). I believe that this is
  unpacking method of least surprise and one that is used on every tool
  that I've checked. Specifically what is the proper handling for an
  long option the does not accept value but one is given. For example
  given "--key=value", what should the correct behavior be?

    - accept the option where the long option is the entire string? ie
      the option is "key=value"
    - accept the option decomposing it into
      the key-value even though we've specified that this option doesn't
      accept arguments (values).

  We are choosing the second option as it appears to lead to the most
  expected behavior, the downside of this is that if someone chooses \c
  unpack_gnu_flag by accident when they would like to accept arguments,
  the parser will correctly parse long options with values but fail to
  accept short options with values.

  N.B. any leading and trailing whitespace is assumed to be handled
    appropriately prior to this call. That is, " -f" is not the same as "-f".
*/
template<bool uses_packed_flags, typename CharT>
basic_option_pack<CharT> unpack_gnu(const std::basic_string<CharT> &str)
{
  typedef basic_option_pack<CharT> option_pack;
  typedef std::basic_string<CharT> string_type;

  static const string_type lprefix("--");
  static const string_type assignment("=");

  auto &&res =
    detail::mismatch(lprefix.begin(),lprefix.end(),str.begin(),str.end());
  if(res.first != lprefix.end() || res.second == str.end())
    return unpack_posix<uses_packed_flags>(str);

  typename string_type::const_iterator assign_loc =
    std::search(res.second,str.end(),assignment.begin(),assignment.end());

  if(assign_loc == str.end())
    return option_pack{false,lprefix,{res.second,str.end()},{},{}};

  return option_pack{true,lprefix,{res.second,assign_loc},{},
    {assign_loc+1,str.end()}};
}



/*
  Parse the arguments contained in \c argv with size \c argc according
  to the option description group \c grp. If \c partial is false, then
  finalize parsing. Options are added to a copy of the variable map
  \c _vm and returned.

  Weird cases:

  option takes an argument but the given argument is another (possibly invalid)
  option. For example:

  utility_name -f option_argument

  Given

  utility_name -f -b

  Should:
    1) option_argument be "-b"?
    2) Complain that -b is an unknown option?
    3) Complain that -f requires an option but one was not given?

  We are going with (3) for efficiency reasons. Ie if _any_ option_description
  can successfully unpack "-b" then "-b" is considered an option and not an
  option_argument even if it is an invalid option
*/
template<typename CharT>
basic_variable_map<CharT>
parse_arguments(std::size_t _argc, const CharT *_argv[],
  const basic_options_group<CharT> &grp, const basic_variable_map<CharT> &vm,
  const std::basic_string<CharT> &end_of_options, bool partial)
{
  typedef std::basic_string<CharT> string_type;
  typedef basic_option_pack<CharT> option_pack_type;
  typedef basic_options_group<CharT> options_group_type;
  typedef basic_variable_map<CharT> variable_map;

  typedef std::vector<string_type> args_list;
  typedef std::vector<args_list> cmd_stack_type;

  variable_map _vm = vm;

  cmd_stack_type cmd_stack{
    args_list{
      std::reverse_iterator<const CharT **>(_argv+_argc),
      std::reverse_iterator<const CharT **>(_argv)
    }
  };

  std::size_t pos_count = 0;

  string_type arg;
  string_type mapped_key;
  option_pack_type option_pack;

  typename options_group_type::const_iterator desc;

  int state = 0;
  while(!cmd_stack.empty()) {
std::cerr << "cmd stack\n";
int depth = 0;
for(auto &al : cmd_stack) {
  std::cerr << std::string(depth,' ') << ": ";
  for(auto &cmd : al)
    std::cerr << cmd << ", ";
  std::cerr << "\n";
  ++depth;
}


    args_list &current_cmdlist = cmd_stack.back();
    if(current_cmdlist.empty()) {
      cmd_stack.pop_back();
      continue;
    }

    std::cerr << "state=" << state << "\n";
    switch(state) {
      case 0: {
        // pull arg off cmdlist and determine if it is an option or a
        // positional

        std::cerr << "Processing cmd arg: " << current_cmdlist.back() << "\n";

        if(cmd_stack.size() == 1 && current_cmdlist.back() == end_of_options) {
          current_cmdlist.pop_back();
          state = 5;
std::cerr << "end of options: " << option_pack << "\n";
          break;
        }

        state = 4; // assume a positional

        for(desc = grp.begin(); desc != grp.end(); ++desc) {
          if(!desc->unpack_option)
            continue;

          option_pack = std::move(desc->unpack_option(current_cmdlist.back()));

          if(option_pack.raw_key.empty())
            continue;
std::cerr << option_pack << "\n";
          if(desc->mapped_key) {
// std::cerr << "A\n";
            mapped_key = desc->mapped_key(option_pack.raw_key,_vm);
            if(!mapped_key.empty()) {
              state = 1;
              break;
            }
// std::cerr << "B\n";
          }
          else {
// std::cerr << "C\n";
            mapped_key = option_pack.raw_key;
            state = 1;
            break;
          }

          std::cerr << "cmd arg: '" << option_pack.raw_key
            << "' has mapped_key '"
            << mapped_key << "'\n";

          state = 2; // is option
        }
      } break;

      case 1: {
        // option is found pull off and handle it
        arg = std::move(current_cmdlist.back());
        current_cmdlist.pop_back();

        if(!desc->make_value) {
  std::cerr << "'" << option_pack.raw_key << "' strictly takes no values\n";
          // strictly no values
          if(option_pack.value_provided)
            throw unexpected_argument_error(option_pack.raw_key,
              option_pack.value);

  std::cerr << "'" << option_pack.raw_key
    << "' is a flag. adding to _vm as '" << mapped_key << "'\n";
          // handle the flag only
          _vm.emplace(mapped_key,any());
        }
        else {
          // required or optional value
          // is it embedded in the option pack?
          if(option_pack.value_provided) {
            // handle the provided value
            _vm.emplace(mapped_key,
              desc->make_value(mapped_key,option_pack.value,_vm));
          }
          else if(current_cmdlist.empty()) {
            // no more items in the current pack, use optional if available
            if(desc->implicit_value)
              _vm.emplace(mapped_key,desc->implicit_value(mapped_key,_vm));
            else
              throw missing_argument_error(arg);
          }
          else {
            // try to use the next argument on the command list. If any
            // desc can unpack it, then it is a (possibly ill-formed)
            // option and not an argument
            typename options_group_type::const_iterator next;
            for(next = grp.begin(); next != grp.end(); ++next) {
              if(!next->unpack_option)
                continue;

              option_pack =
                std::move(next->unpack_option(current_cmdlist.back()));

              if(!option_pack.raw_key.empty())
                break;
            }

            if(next != grp.end()) {
              if(desc->implicit_value)
                _vm.emplace(mapped_key,desc->implicit_value(mapped_key,_vm));
              else
                throw unexpected_argument_error(arg,string_type(arg));
            }
            else {
              _vm.emplace(mapped_key,
                desc->make_value(mapped_key,current_cmdlist.back(),_vm));
                current_cmdlist.pop_back();
            }
          }
        }
//        state = 3;
        // see if there were any packed options
        if(!option_pack.packed_arguments.empty()) {
std::cerr << "Adding packed args: \n";
for(auto &arg : option_pack.packed_arguments)
  std::cerr << arg << ", ";
std::cerr << "\n";

          std::reverse(option_pack.packed_arguments.begin(),
            option_pack.packed_arguments.end());
          cmd_stack.emplace_back(std::move(option_pack.packed_arguments));
        }
        state = 0;
      } break;

      case 2: {
        // was unpacked as an option but couldn't find desc to handle it
        throw unknown_option_error(arg);
      } break;

#if 0
      case 3: {
        // see if there were any packed options
        if(!option_pack.packed_arguments.empty()) {
std::cerr << "Adding packed args: \n";
for(auto &arg : option_pack.packed_arguments)
  std::cerr << arg << "\n";
std::cerr << "\n";

          std::reverse(option_pack.packed_arguments.begin(),
            option_pack.packed_arguments.end());
          cmd_stack.emplace_back(std::move(option_pack.packed_arguments));
        }
        state = 0;
      } break;
#endif

      case 4:
        state = 0;
        // note the fall through so only execute case 5 once

      case 5: {
        // positional, if end_of_option is set, then we continually repeat here
        arg = std::move(current_cmdlist.back());
        current_cmdlist.pop_back();

        // always process as positional, try to find desc to handle it
        for(desc = grp.begin(); desc != grp.end(); ++desc) {
          if(desc->unpack_option)
            continue;

          string_type pos_key;

          if(desc->implicit_key) {
            pos_key = desc->implicit_key(pos_count);
            if(pos_key.empty())
              continue;
          }

          // handle this positional option
          if(desc->make_value)
            _vm.emplace(pos_key,desc->make_value(pos_key,arg,_vm));
          else
            _vm.emplace(pos_key,any());

          ++pos_count;

          break;
        }

        if(desc == grp.end())
          throw unexpected_positional_error(arg);
      } break;

      default:
        abort();
    }
  }

  return _vm;
}











/*
  Parse the arguments contained in \c argv with size \c argc according
  to the option description group \c grp. If
  \c partial is true, then finalize parsing. Options are returned in a
  new variable map.
*/
template<typename CharT>
inline basic_variable_map<CharT>
parse_arguments(std::size_t argc, const CharT * argv[],
  const basic_options_group<CharT> &grp, bool partial = false)
{
  return parse_arguments(argc,argv,grp,basic_variable_map<CharT>(),
    std::basic_string<CharT>("--"),partial);
}

#if 0
/*
  Parse the arguments contained in \c argv with size \c argc according
  to the option description group \c grp with parse_options \c opt. If
  \c partial is true, then finalize parsing. Options are added to a copy
  of the variable map \c _vm and returned.
*/
template<typename CharT>
inline basic_variable_map<CharT>
parse_arguments(std::size_t argc, const CharT *argv[],
  const basic_options_group<CharT> &grp, const basic_variable_map<CharT> &_vm,
  bool partial = false)
{
  std::size_t endc;
  return parse_arguments(argc,argv,endc,grp,_vm,partial);
}

/*
  Parse the arguments contained in \c argv with size \c argc according
  to the option description group \c grp with parse_options \c opt. If
  \c partial is true, then finalize parsing. Options are returned in a
  new variable map. If parsing is halted due to a "end of parse"
  indicator (normally '--') then \c endc is updated to the index of the
  first non-parsed argument.
*/
template<typename CharT>
inline basic_variable_map<CharT>
parse_arguments(std::size_t argc, const CharT * argv[],
  std::size_t &endc, const basic_options_group<CharT> &grp,
  bool partial = false)
{
  return parse_arguments(argc,argv,endc,grp,basic_variable_map<CharT>(),
    partial);
}
#endif

typedef basic_option_description<char> option_description;
typedef std::vector<basic_option_description<char> > options_group;
typedef std::multimap<std::basic_string<char>,any> variable_map;

typedef basic_option_description<wchar_t> woption_description;
typedef std::vector<basic_option_description<wchar_t> > woptions_group;
typedef std::multimap<std::basic_string<wchar_t>,any> wvariable_map;








/*
  EZ interface

  Be overly descriptive. ie use
    value<int>().occurrences(1,2).mutually_exclusive({"foo"}).
      mutually_inclusive({"bar"});
  rather than
    value<int>{1,2,{"foo"},{"foo"}};

  ie is "foo" inclusive or exclusive? what if don't want to use range?
  therefore lots of constructors that someone needs to be read.
*/


template<typename CharT>
struct constrain {
  typedef std::basic_string<CharT> string_type;

  constrain<CharT> & occurrences(std::size_t n)
  {
    _min = _max = n;
    return *this;
  }

  constrain<CharT> & occurrences(std::size_t n, std::size_t m)
  {
    _min = n;
    _max = m;
    assert(_min <= _max);
    return *this;
  }

  constrain<CharT> &
  mutual_exclusion(const std::vector<string_type> &mapped_key_vec)
  {
    _mutual_exclusion = mapped_key_vec;
    return *this;
  }

  constrain<CharT> &
  mutual_inclusion(const std::vector<string_type> &mapped_key_vec)
  {
    _mutual_inclusion = mapped_key_vec;
    return *this;
  }

  int _position = -1;
  std::size_t _min = 0;
  std::size_t _max = std::numeric_limits<std::size_t>::max();
  std::vector<string_type> _mutual_exclusion;
  std::vector<string_type> _mutual_inclusion;
};


template<typename T>
struct value {
  typedef T value_type;

  value(void) = default;
  value(const T &val) :_implicit(std::make_shared<T>(val)) {}

  std::shared_ptr<value_type> _implicit;
};

template<typename CharT>
const std::basic_string<CharT> & default_long_prefix(void); // undefined

template<>
const std::basic_string<char> & default_long_prefix<char>(void)
{
  static const std::basic_string<char> val("--");

  return val;
}

template<>
const std::basic_string<wchar_t> & default_long_prefix<wchar_t>(void)
{
  static const std::basic_string<wchar_t> val(L"--");

  return val;
}

template<typename CharT>
const std::basic_string<CharT> & default_short_prefix(void); // undefined

template<>
const std::basic_string<char> & default_short_prefix<char>(void)
{
  static const std::basic_string<char> val("-");

  return val;
}

template<>
const std::basic_string<wchar_t> & default_short_prefix<wchar_t>(void)
{
  static const std::basic_string<wchar_t> val(L"-");

  return val;
}






namespace detail {

template <typename CharT>
inline const std::basic_string<CharT> &
to_xstring(std::basic_string<CharT> &str)
{
    return str;
}

/*
  to_xstring taken from:

  http://stackoverflow.com/questions/34961274/implement-to-xstring-to-unite-to-string-and-to-wstring
*/
template <typename CharT, typename T>
inline
typename std::enable_if<std::is_same<CharT, char>::value, std::string>::type
    to_xstring(const T &t)
{
    return std::to_string(t);
}


template <typename CharT, typename T>
inline
typename std::enable_if<!std::is_same<CharT, char>::value, std::wstring>::type
    to_xstring(const T &t)
{
    return std::to_wstring(t);
}

inline std::string as_utf8(const std::string &s)
{
  return s;
}

template<typename CharT>
inline std::string as_utf8(const std::basic_string<CharT> &s)
{
  typedef std::wstring_convert<
    std::codecvt_utf8<CharT>,CharT> converter_type;

  converter_type converter;

  return converter.to_bytes(s);
}

template<typename CharT>
inline std::pair<std::basic_string<CharT>,std::basic_string<CharT> >
split(const std::basic_string<CharT> &str, CharT delim)
{
  typedef std::basic_string<CharT> string_type;

  typename string_type::size_type loc = str.find(delim);
  if(loc == string_type::npos)
    return std::make_pair(str,string_type());

  return std::make_pair(str.substr(0,loc),str.substr(loc+1));
}

template<typename CharT>
std::pair<std::basic_string<CharT>,std::basic_string<CharT> >
add_option_spec(const std::basic_string<CharT> &opt_spec,
  CharT delim, basic_option_description<CharT> &desc)
{
  typedef std::basic_string<char> string_type;
  typedef basic_variable_map<CharT> variable_map;

  string_type long_opt;
  string_type short_opt;
  std::tie(long_opt,short_opt) = detail::split(opt_spec,delim);

  /*
    If both are provided, the long option is the vm key

    If none are provided, then it is unmapped_key
  */
  string_type key_desc;
  if(opt_spec.empty() || (long_opt.empty() && short_opt.empty())) {
    key_desc = default_long_prefix<CharT>() + "[all] ,  "
        + default_short_prefix<CharT>() + "[all]";
  }
  else if(!long_opt.empty() && !short_opt.empty()) {
    desc.mapped_key = [=](const string_type &_opt, const variable_map &)
      {
        if(_opt == long_opt || _opt == short_opt)
          return long_opt;

        return string_type();
      };

    key_desc = default_long_prefix<CharT>() + long_opt + " ,  "
        + default_short_prefix<CharT>() + short_opt;
  }
  else if(!long_opt.empty()) {
    desc.mapped_key = [=](const string_type &_opt, const variable_map &)
      {
        if(_opt == long_opt)
          return _opt;

        return string_type();
      };

    key_desc = default_long_prefix<CharT>() + long_opt;
  }
  else {
    desc.mapped_key = [=](const string_type &_opt, const variable_map &)
      {
        if(_opt == short_opt)
          return _opt;

        return string_type();
      };

    key_desc = default_short_prefix<CharT>() + short_opt;
  }

  desc.key_description = [=](void) { return key_desc; };

  return std::make_pair(long_opt,short_opt);
}









template<typename CharT>
std::pair<std::basic_string<CharT>,std::basic_string<CharT> >
add_hidden_option_spec(const std::basic_string<CharT> &opt_spec,
  CharT delim, basic_option_description<CharT> &desc)
{
  typedef std::basic_string<char> string_type;
  typedef basic_variable_map<CharT> variable_map;

  string_type long_opt;
  string_type short_opt;
  std::tie(long_opt,short_opt) = detail::split(opt_spec,delim);

  /*
    If both are provided, the long option is the vm key

    If none are provided, then it is unmapped_key
  */
  if(!long_opt.empty() && !short_opt.empty()) {
    desc.mapped_key = [=](const string_type &_opt, const variable_map &)
      {
        if(_opt == long_opt || _opt == short_opt)
          return long_opt;

        return string_type();
      };
  }
  else if(!long_opt.empty()) {
    desc.mapped_key = [=](const string_type &_opt, const variable_map &)
      {
        if(_opt == long_opt)
          return _opt;

        return string_type();
      };
  }
  else if(!short_opt.empty()){
    desc.mapped_key = [=](const string_type &_opt, const variable_map &)
      {
        if(_opt == short_opt)
          return _opt;

        return string_type();
      };
  }

  return std::make_pair(long_opt,short_opt);
}









template<typename T, typename CharT>
inline void add_value(const value<T> &val,
  basic_option_description<CharT> &desc)
{
  typedef std::basic_string<CharT> string_type;
  typedef basic_variable_map<CharT> variable_map;

  if(val._implicit) {
    desc.implicit_value = [=](const string_type &, const variable_map &)
    {
      return any(*(val._implicit));
    };

    desc.implicit_value_description = [=](void) {
      return to_xstring<CharT>(*(val._implicit));
    };
  }

  desc.make_value = [](const string_type &,const string_type &val,
    const variable_map &)
  {
    T _val;
    std::basic_stringstream<CharT> in(val);
    in >> _val;
    return any(_val);
  };
}

template<typename CharT>
inline void add_key(const std::basic_string<CharT> &key, int position,
  basic_option_description<CharT> &desc)
{
  assert(!key.empty());

  if(position >= 0) {
    desc.implicit_key = [=](std::size_t i) {
      if(i == position)
        return key;
      return std::basic_string<CharT>();
    };
  }
  else {
    desc.implicit_key = [=](std::size_t) {
      return key;
    };
  }
}



/*
  Used for constraints on options. Cannot be empty string mapped keys.

  Although \c mapped_key and long_opt are the same for the EZ
  interface, it is separated here for completeness
*/
template<typename CharT>
void add_option_constrains(const constrain<CharT> &cnts,
  basic_option_description<CharT> &desc,
  const std::basic_string<CharT> &mapped_key,
  const std::basic_string<CharT> &long_opt,
  const std::basic_string<CharT> &short_opt)
{
  desc.finalize = [=](const basic_variable_map<CharT> &vm) {

    if(vm.count(mapped_key) > cnts._max) {
      std::stringstream err;
      err << "option ";
      if(!long_opt.empty())
        err << "'%L" << as_utf8(long_opt) << "' ";
      if(!long_opt.empty() && !short_opt.empty())
        err << "or ";
      if(!short_opt.empty())
        err << "'%S" << as_utf8(short_opt) << "' ";
      err << "cannot be specified more than "
        << cnts._max << (cnts._max==1?" time":" times");
      throw command_option_error(err.str());
    }

    if(vm.count(mapped_key) < cnts._min) {
      std::stringstream err;
      err << "option ";
      if(!long_opt.empty())
        err << "'%L" << as_utf8(long_opt) << "' ";
      if(!long_opt.empty() && !short_opt.empty())
        err << "or ";
      if(!short_opt.empty())
        err << "'%S" << as_utf8(short_opt) << "' ";
      err << "must be specified at least "
        << cnts._min << (cnts._max==1?" time":" times");
      throw command_option_error(err.str());
    }

    for(auto &exclusive_key : cnts._mutual_exclusion) {
      if(vm.count(exclusive_key) != 0) {
        std::stringstream err;
        err << "option ";
        if(!long_opt.empty())
          err << "'%L" << as_utf8(long_opt) << "' ";
        if(!long_opt.empty() && !short_opt.empty())
          err << "and ";
        if(!short_opt.empty())
          err << "'%S" << as_utf8(short_opt) << "' ";
        err << "cannot be specified along with '"
          << as_utf8(exclusive_key) << "'";
        throw command_option_error(err.str());
      }
    }

    for(auto &inclusive_key : cnts._mutual_inclusion) {
      if(vm.count(inclusive_key) != 0) {
        std::stringstream err;
        err << "option ";
        if(!long_opt.empty())
          err << "'%L" << as_utf8(long_opt) << "' ";
        if(!long_opt.empty() && !short_opt.empty())
          err << "or ";
        if(!short_opt.empty())
          err << "'%S" << as_utf8(short_opt) << "' ";
        err << "must be specified along with '"
          << as_utf8(inclusive_key) << "'";
        throw command_option_error(err.str());
      }
    }
  };
}

/*
  Used for constraints on positional. CAN be empty string mapped keys.
*/
template<typename CharT>
void add_positional_constrains(const constrain<CharT> &cnts,
  const std::basic_string<CharT> &mapped_key,
  basic_option_description<CharT> &desc)
{
  desc.finalize = [=](const basic_variable_map<CharT> &vm) {
    if(vm.count(mapped_key) > cnts._max) {
      std::stringstream err;
      err << "positional values cannot be specified more than "
        << cnts._max << (cnts._max==1?" time":" times");
      throw command_option_error(err.str());
    }

    if(vm.count(mapped_key) < cnts._min) {
      std::stringstream err;
      if(cnts._max==1)
        err << "a positional value ";
      else
        err << "positional values ";
      err << "must be specified at least "
        << cnts._min << (cnts._max==1?" time":" times");
      throw command_option_error(err.str());
    }

    for(auto &exclusive_key : cnts._mutual_exclusion) {
      if(vm.count(exclusive_key) != 0) {
        std::stringstream err;
        err << "option '" << as_utf8(exclusive_key)
            << "' is incompatible with the given positional options";
        throw command_option_error(err.str());
      }
    }

    for(auto &inclusive_key : cnts._mutual_inclusion) {
      if(vm.count(inclusive_key) != 0) {
        std::stringstream err;
        err << "option '" << as_utf8(inclusive_key)
            << "' must be provided with the given positional options";
        throw command_option_error(err.str());
      }
    }
  };
}



}







/*
  The following functions are for convenience only. They provide automatic
  type deduction and therefore implicit conversion from string literals
  to the basic_string equivalents as well as defaults for unused arguments.
*/

/*
  Cases line 2, 4
*/
inline basic_option_description<char>
make_option(const std::basic_string<char> &opt_spec,
  const std::basic_string<char> &extended_desc,
  const constrain<char> &cnts = constrain<char>(), char delim = ',')
{
  typedef std::basic_string<char> string_type;

  basic_option_description<char> desc{unpack_gnu<true,char>};

  string_type long_opt;
  string_type short_opt;
  std::tie(long_opt,short_opt) = detail::add_option_spec(opt_spec,delim,desc);

  detail::add_option_constrains(cnts,desc,long_opt,long_opt,short_opt);

  return desc;
}

/*
  Cases line 3, 5
*/
inline basic_option_description<char>
make_hidden_option(const std::basic_string<char> &opt_spec,
  const constrain<char> &cnts = constrain<char>(), char delim = ',')
{
  typedef std::basic_string<char> string_type;

  basic_option_description<char> desc{unpack_gnu<true,char>};

  string_type long_opt;
  string_type short_opt;
  std::tie(long_opt,short_opt) =
    detail::add_hidden_option_spec(opt_spec,delim,desc);

  detail::add_option_constrains(cnts,desc,long_opt,long_opt,short_opt);

  return desc;
}

/*
  Case line 6, 8, 10, 12
*/
template<typename T>
inline basic_option_description<char>
make_option(const std::basic_string<char> &opt_spec,
  const value<T> &val, const std::basic_string<char> &extended_desc,
  const constrain<char> &cnts = constrain<char>(), char delim = ',')
{
  typedef std::basic_string<char> string_type;

  basic_option_description<char> desc{unpack_gnu<false,char>};

  string_type long_opt;
  string_type short_opt;
  std::tie(long_opt,short_opt) = detail::add_option_spec(opt_spec,delim,desc);

  detail::add_value(val,desc);

  desc.extended_description = [=](void) { return extended_desc; };

  detail::add_option_constrains(cnts,desc,long_opt,long_opt,short_opt);

  return desc;
}

/*
  Case line 7, 9, 11, 13
*/
template<typename T>
inline basic_option_description<char>
make_hidden_option(const std::basic_string<char> &opt_spec, const value<T> &val,
  const constrain<char> &cnts = constrain<char>(), char delim = ',')
{
  typedef std::basic_string<char> string_type;

  basic_option_description<char> desc{unpack_gnu<false,char>};

  string_type long_opt;
  string_type short_opt;
  std::tie(long_opt,short_opt) =
    detail::add_hidden_option_spec(opt_spec,delim,desc);

  detail::add_value(val,desc);

  detail::add_option_constrains(cnts,desc,long_opt,long_opt,short_opt);

  return desc;
}

/*
  Case line 14
*/
template<typename T>
inline basic_option_description<char>
make_positional(const std::basic_string<char> &extended_desc,
  const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  desc.extended_description = [=](void) { return extended_desc; };

  detail::add_value(value<T>(),desc);

  detail::add_positional_constrains(cnts,std::basic_string<char>(),desc);

  return desc;
}

/*
  Case line 15
*/
template<typename T>
inline basic_option_description<char>
make_hidden_positional(const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  detail::add_value(value<T>(),desc);

  detail::add_positional_constrains(cnts,std::basic_string<char>(),desc);

  return desc;
}

/*
  Case line 16
*/
template<typename T>
inline basic_option_description<char>
make_positional(const std::basic_string<char> &implicit_key,
  const std::basic_string<char> &extended_desc,
  int position = -1, const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  desc.extended_description = [=](void) { return extended_desc; };

  detail::add_value(value<T>(),desc);

  detail::add_key(implicit_key,position,desc);

  detail::add_positional_constrains(cnts,implicit_key,desc);

  return desc;
}

/*
  Case line 17
*/
template<typename T>
inline basic_option_description<char>
make_hidden_positional(const std::basic_string<char> &implicit_key,
  int position = -1, const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  detail::add_value(value<T>(),desc);

  detail::add_key(implicit_key,position,desc);

  detail::add_positional_constrains(cnts,implicit_key,desc);

  return desc;
}

/*
  Case line 18
*/
inline basic_option_description<char>
make_positional(const std::basic_string<char> &extended_desc,
  const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  desc.extended_description = [=](void) { return extended_desc; };

  detail::add_positional_constrains(cnts,std::basic_string<char>(),desc);

  return desc;
}

/*
  Case line 19
*/
inline basic_option_description<char>
make_hidden_positional(const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  detail::add_positional_constrains(cnts,std::basic_string<char>(),desc);

  return desc;
}

/*
  Case line 20
*/
inline basic_option_description<char>
make_positional(const std::basic_string<char> &implicit_key,
  const std::basic_string<char> &extended_desc,
  int position = -1, const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  desc.extended_description = [=](void) { return extended_desc; };

  detail::add_key(implicit_key,position,desc);

  detail::add_positional_constrains(cnts,implicit_key,desc);

  return desc;
}

/*
  Case line 21
*/
inline basic_option_description<char>
make_hidden_positional(const std::basic_string<char> &implicit_key,
  int position = -1, const constrain<char> &cnts = constrain<char>())
{
  basic_option_description<char> desc;

  detail::add_key(implicit_key,position,desc);

  detail::add_positional_constrains(cnts,implicit_key,desc);

  return desc;
}

/*
  Convenience function to intercept options to make them errors. This is
  only needed in the case where there are only positional arguments and
  you don't want to accept anything that looks like a option.

  For example, If there are _any_ descriptions that have uppack_option
  set and the option is unrecognized, then parsing will complain. For
  example, on my machine, the command 'echo' takes one option, '-n'
  (nonconforming to POSIX actually). If the command 'echo -f' is given,
  the string '-f' is printed meaning that '-f' is interpreted as a
  positional operand and not an invalid option. If when using this
  library you would rather an error be generated that states '-f' was
  unrecognized, use the \c make_options_error function and do not
  include any other non-positional descriptions.
*/
inline basic_option_description<char> make_options_error(void)
{
  typedef std::basic_string<char> string_type;

  return basic_option_description<char>{unpack_gnu<false,char>,
    [](const string_type &raw_key,const variable_map &) -> string_type {
      throw unknown_option_error(raw_key);
    }};
}




/*
  Specializations for wchar_t

  Add me
*/



#if 0
/*
  Convenience default parser

  Parse the arguments contained in \c argv with size \c argc. Options
  are returned in a new variable map. Long options are prefaced with a
  '--' and short options (single character) are prefaced with a '-'. In
  each case, the option is stored as a key in the variable_map. If
  parsing is halted due to a "end of parse" indicator '--' then \c endc
  is updated to the index of the first non-parsed argument.
*/
template<typename CharT>
basic_variable_map<CharT>
parse_arguments(const CharT * const argv[], std::size_t argc,
  std::size_t &endc)
{
  basic_option_description<CharT> option_desc =
    make_option("",value<std::string>(""),"any");

  option_desc.unpack_option = unpack_gnu<CharT,false>;

  basic_options_group<CharT> grp{
    option_desc
  };

  return parse_arguments(argv,argc,endc,grp,basic_variable_map<CharT>(),false);
}

#endif


























template<typename CharT>
bool option_comp(const basic_option_description<CharT> &lhs,
  const basic_option_description<CharT> &rhs)
{
  bool lhs_short = lhs.map_short_key && lhs.short_key_description;
  bool lhs_long = lhs.map_long_key && lhs.long_key_description;
  bool rhs_short = rhs.map_short_key && rhs.short_key_description;
  bool rhs_long = rhs.map_long_key && rhs.long_key_description;

  if(lhs_short && rhs_short)
    return (lhs.short_key_description() < rhs.short_key_description());

  if(!lhs_short && !rhs_short) {
    if(lhs_long && rhs_long)
      return (lhs.long_key_description() < rhs.long_key_description());

    if(!lhs_long && !rhs_long)
      return (&lhs < &rhs);

    return !lhs_long;
  }

  return !lhs_short;
}

template<typename CharT>
std::basic_string<CharT>
option_to_string(const basic_option_description<CharT> &desc)
{
//  typedef std::basic_string<CharT> string_type;

  std::basic_ostringstream<CharT> out;
#if 0
  out << "  ";

  if(desc.map_long_key && desc.map_short_key) {
    if(!(desc.long_key_description && desc.short_key_description))
      return string_type(); // hidden

    out << opt.short_option_flag << desc.short_key_description() << " [ "
      << opt.long_option_flag << desc.long_key_description() << " ]";
  }
  else if(desc.map_long_key) {
    if(!desc.long_key_description)
      return string_type();

    out << opt.long_option_flag << desc.long_key_description();
  }
  else {
    if(!desc.short_key_description)
      return string_type();

    out << opt.short_option_flag << desc.short_key_description();
  }

  if(desc.make_value) {
    if(desc.implicit_value) {
      // value is optional
      out << "  [arg]";
      if(desc.implicit_value_description)
        out << "=(" << desc.implicit_value_description() << ')';
    }
    else
      out << "  <arg>";
  }
#endif
  return out.str();
}




/*
  Adapted from rosettacode.org

  https://www.rosettacode.org/wiki/Word_wrap#C.2B.2B
*/
template<typename CharT>
std::basic_string<CharT>
wrap(const std::basic_string<CharT> &text, std::size_t width)
{
  std::basic_istringstream<CharT> words(text);
  std::basic_ostringstream<CharT> wrapped;
  std::basic_string<CharT> word;

  if (words >> word) {
    wrapped << word;
    size_t space_left = width - word.length();
    while (words >> word) {
      if (space_left < word.length() + 1) {
        wrapped << '\n' << word;
        space_left = width - word.length();
      }
      else {
        wrapped << ' ' << word;
        space_left -= word.length() + 1;
      }
    }
  }

  return wrapped.str();
}

template<typename CharT>
std::basic_string<CharT> make_column(const std::basic_string<CharT> &text,
  const std::basic_string<CharT> &pad, std::size_t width)
{
  typedef std::basic_string<CharT> string_type;

  static const std::basic_regex<CharT> regex("\n",
    std::regex_constants::optimize);

  static const string_type nl("\n");

  string_type wrapped_text = wrap(text,width-pad.size());

  return std::regex_replace(wrapped_text,regex,nl+pad);
}

/*
  Indent is the amount of space from the left edge that the text will
  start to be drawn. That is, in two column format, the column width is
  (width-indent) whereas in a single column (or paragraph-mode), the
  first line starts at indent and is (width-indent) wide.
*/
template<typename CharT>
std::basic_string<CharT>
extended_to_string(const basic_option_description<CharT> &desc,
  std::size_t indent, std::size_t width)
{
  typedef std::basic_string<CharT> string_type;

  static const string_type nl("\n");
  static const std::basic_regex<CharT> regex("\n",
    std::regex_constants::optimize);

  if(desc.extended_description) {
    string_type wrapped_text = wrap(desc.extended_description(),width-indent);

    string_type pad(indent,' ');
    return std::regex_replace(wrapped_text,regex,nl+pad);
  }

  return "no description";
}


/*
  Control the output format when converting to a string

  \c width is the total allowed with of the formatted extents

  \c max_option_width is the maximum width of the option column. If a
  formatted option width exceeds the returned value plust the returned
  value of \c spacing_width, then the remaining columns start on a new
  line. The return value can be dynamically obtained as a percentage of
  the total width.

  \c spacing_width is the blank space between the option column and the
  description column.

  \c fmt_option Return a string containing the fully formatted option
  (just the option part, not the extended description) or the empty
  string if the option should be hidden. Default is \c option_to_string

  \c cmp [OPTIONAL] If set, is used to sort the descriptors according to
  the comparison function.
*/
template<typename CharT>
struct basic_cmd_option_fmt {
  typedef std::basic_string<CharT> string_type;
  typedef basic_option_description<CharT> description_type;

  std::function<std::size_t(void)> width = [](void){ return 80; };
  std::function<std::size_t(void)> max_option_width = [](void){ return 40; };
  std::function<std::size_t(void)> spacing_width = [](void){ return 2; };

  std::function<string_type(const description_type &)> fmt_option =
    option_to_string<CharT>;

  std::function<string_type(const description_type &,
    std::size_t, std::size_t)> fmt_extended = extended_to_string<CharT>;

  std::function<bool(const description_type &,
    const description_type &)> sort_cmp = option_comp<CharT>;
};

typedef basic_cmd_option_fmt<char> cmd_option_fmt;
typedef basic_cmd_option_fmt<wchar_t> wcmd_option_fmt;

template<typename CharT>
std::basic_string<CharT>
to_string(const std::vector<basic_option_description<CharT> > &grp,
  const basic_cmd_option_fmt<CharT> &fmt = basic_cmd_option_fmt<CharT>())
{
  typedef std::basic_string<CharT> string_type;
  typedef basic_option_description<CharT> description_type;

  std::basic_ostringstream<CharT> out;

  typedef std::pair<const description_type *,string_type> output_pair_type;
  std::vector<output_pair_type> output_grp;
  output_grp.reserve(grp.size());

  std::size_t extent = 0;
  for(auto &desc : grp) {
    // hidden means not long or short key descriptions
    if((desc.map_long_key && desc.long_key_description) ||
      (desc.map_short_key && desc.short_key_description))
    {
      output_grp.emplace_back(std::make_pair(&desc,fmt.fmt_option(desc)));
      extent = std::max(extent,output_grp.back().second.size());
    }
  }

  // Sort the results if requested
  if(fmt.sort_cmp) {
    std::sort(output_grp.begin(),output_grp.end(),
      [&](const output_pair_type &lhs, const output_pair_type &rhs) {
        return fmt.sort_cmp(*lhs.first,*rhs.first);
      });
  }

  std::size_t total_width = fmt.width();
  std::size_t max_option_width = fmt.max_option_width();
  std::size_t spacing_width = fmt.spacing_width();
  std::size_t rindent = std::min(extent+spacing_width,max_option_width);
  string_type rindent_pad(rindent,' ');
  for(std::size_t i=0; i<output_grp.size(); ++i) {
    out << output_grp[i].second;

    if(output_grp[i].second.size()+spacing_width > rindent)
      out << '\n' << rindent_pad;
    else
      out << string_type(rindent-output_grp[i].second.size(),' ');

    out << fmt.fmt_extended(*output_grp[i].first,rindent,total_width)
      << '\n';
  }

  return out.str();
}

template<typename CharT>
inline std::basic_string<CharT>
to_string(const std::vector<basic_option_description<CharT> > &grp)
{
  return to_string(grp,basic_cmd_option_fmt<CharT>());
}

template<typename CharT>
inline std::basic_string<CharT>
to_string_debug(const basic_option_description<CharT> &option)
{
  std::basic_ostringstream<CharT> out;

  out
    << "map_long_key: " << bool(option.map_long_key) << "\n"
    << "long_key_description: " << bool(option.long_key_description) << "\n"
    << "map_short_key: " << bool(option.map_short_key) << "\n"
    << "short_key_description: " << bool(option.short_key_description) << "\n"
    << "extended_description: " << bool(option.extended_description) << "\n"
    << "implicit_key: " << bool(option.implicit_key) << "\n"
    << "implicit_value: " << bool(option.implicit_value) << "\n"
    << "implicit_value_description: "
      << bool(option.implicit_value_description) << "\n"
    << "make_value: " << bool(option.make_value) << "\n"
    << "finalize: " << bool(option.finalize) << "\n";

  return out.str();
};




}

}




#endif


