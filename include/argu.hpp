#ifndef ARGU_H_
#define ARGU_H_
#define ARGU_MAJOR @argu_VERSION_MAJOR@
#define ARGU_MINOR @argu_VERSION_MINOR@

#include <cstdint>
#include <map>
#include <string>
#include <stdexcept>
#include <cstdio>
#include <boost/regex.hpp>

namespace argu
{ //#:- Forward declaration
  class value_t;

  using argument_map_t = std::map<std::string, value_t>;
  using switch_pair_t = std::pair<std::string, bool (*)(const std::string&)>;
  using switches_t = std::vector<std::string>;
  using arguments_t = std::vector<std::string>;
  argument_map_t parse(int32_t _argument_count, char* _argument_vector[],
                       std::initializer_list<switch_pair_t> _switch_initializer_list) throw (std::runtime_error);

  namespace extraction
  { inline std::string attached_value(const std::string& _string) noexcept;
    inline std::string attached_switch(const std::string& _string) noexcept;
    inline void long_switch(const std::string& _value, const std::string& _switch,
                            const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                            argument_map_t& _argument_map);
    inline void short_multi_switch(const std::string& _switch, const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                                   argument_map_t& _argument_map) noexcept;
    inline void short_single_switch(const std::string& _value, const std::string& _switch,
                                    const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                                    argument_map_t& _argument_map) throw (std::runtime_error);
    inline void path(const char* _path, argument_map_t& _argument_map) throw (std::runtime_error); }
  namespace reporting
  { inline void throw_(const std::string& _message,
                       const std::string& _additional = "") throw (std::runtime_error); }
  namespace recognition
  { inline bool short_single_switch(const std::string& _string, const switches_t& _switches) noexcept;
    bool short_multi_switch(const std::string& _string,
                            const std::initializer_list<switch_pair_t>& _switch_initializer_list) noexcept;
    inline bool long_switch(const std::string& _string, const switches_t& _switches) noexcept;
    inline bool attached_value(const std::string& _string) noexcept; }
  namespace validation
  { inline void main_arguments(int32_t _argument_count, char* _argument_vector[]) throw (std::runtime_error);
    inline void path(const char* _path) throw (std::runtime_error);
    inline bool string(const char* _string) noexcept;
    inline bool switch_(const std::string& _string) noexcept;
    inline void switches(const switches_t& _switches) throw (std::runtime_error);
    inline void switch_duplication(const switches_t& _switches) throw (std::runtime_error);
    inline bool next_argument(arguments_t::const_iterator& _begin,
                              arguments_t::const_iterator& _end) noexcept; }
  namespace algorithms
  { inline switches_t collect_short_switches(const std::initializer_list<switch_pair_t>& _switch_initializer_list) noexcept;
    inline switches_t convert(const std::initializer_list<switch_pair_t>& _switch_initializer_list) noexcept;
    inline arguments_t convert(int32_t _argument_count, char* _argument_vector[]) noexcept;
    inline std::initializer_list<switch_pair_t>::iterator find(const std::string& _switch,
                                                               const std::initializer_list<switch_pair_t>& _switch_initializer_list)
                                                               noexcept;
    inline std::string encode_non_printable_symbols(const char* _string) noexcept; }
  class value_t
  { public:
     //#:- Construction
     value_t() { ; };
     value_t(const value_t& _value) noexcept;
     value_t(value_t&& _value) noexcept;
     value_t& operator=(const value_t& _value) noexcept;
     //#:- Interface
     std::string value() const noexcept;
     bool valid() const noexcept;
     //#:- Friends
     friend void extraction::short_multi_switch(const std::string& _switch, const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                                                argument_map_t& _argument_map) noexcept;
     friend void extraction::short_single_switch(const std::string& _value, const std::string& _switch,
                                                 const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                                                 argument_map_t& _argument_map) throw (std::runtime_error);
     friend void extraction::path(const char* _path, argument_map_t& _argument_map) throw (std::runtime_error);
     ~value_t() { ; }
    private:
     //#:- Construction
     value_t(const std::string& _value, bool _valid) noexcept;
     //#:- Attributes
     std::string value_m;;
     bool state_m; };
  namespace value
  { bool none(const std::string& _string) noexcept;
    bool numerical(const std::string& _string) noexcept;
    bool alphanumerical(const std::string& _string) noexcept; }

  #ifdef DEBUG
   namespace debug
   { using argument_pair_t = std::pair<int32_t, char**>;
     using argument_chain_t = std::vector<std::string>;
     argument_pair_t convert(const argument_chain_t& _chain) throw (std::runtime_error);
     void deallocate(argument_pair_t& _argument_pair) throw (std::runtime_error); }
  #endif
}

#endif
