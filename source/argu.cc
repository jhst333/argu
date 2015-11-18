#include "argu.hpp"

namespace argu
{ argument_map_t parse(int32_t _argument_count, char* _argument_vector[],
                       std::initializer_list<switch_pair_t> _switch_initializer_list) throw (std::runtime_error)
  { validation::main_arguments(_argument_count, _argument_vector);
    validation::path(_argument_vector[0]);
    arguments_t arguments(algorithms::convert(_argument_count, _argument_vector));
    switches_t switches(algorithms::convert(_switch_initializer_list));
    validation::switches(switches);
    validation::switch_duplication(switches);

    argument_map_t argument_map;
    extraction::path(_argument_vector[0], argument_map);
    for (auto arguments_iterator = arguments.cbegin() + 1,
         last_arguments_iterator = arguments.cend();
         arguments_iterator != last_arguments_iterator; ++arguments_iterator)
    { if (recognition::short_single_switch((*arguments_iterator), switches))
      { if (!validation::next_argument(arguments_iterator, last_arguments_iterator))
         extraction::short_single_switch(std::string(""), (*arguments_iterator),
                                         _switch_initializer_list, argument_map);
        else
        { auto next_argument_iterator = arguments_iterator + 1;
          if (validation::switch_((*next_argument_iterator)))
           extraction::short_single_switch(std::string(""), (*arguments_iterator),
                                           _switch_initializer_list, argument_map);
          else
          { extraction::short_single_switch((*next_argument_iterator), (*arguments_iterator),
                                            _switch_initializer_list, argument_map);
            arguments_iterator = next_argument_iterator; } } }
      else if (recognition::short_multi_switch((*arguments_iterator), _switch_initializer_list))
       extraction::short_multi_switch((*arguments_iterator), _switch_initializer_list, argument_map);
      else if (recognition::long_switch((*arguments_iterator), switches))
      { if (recognition::attached_value((*arguments_iterator)))
        { std::string attached_switch = extraction::attached_switch((*arguments_iterator));
          std::string attached_value = extraction::attached_value((*arguments_iterator));
          extraction::long_switch(attached_value, attached_switch, _switch_initializer_list, argument_map); }
        else
        { if (validation::next_argument(arguments_iterator, last_arguments_iterator))
          { auto next_argument_iterator = arguments_iterator + 1;
            if (validation::switch_((*next_argument_iterator)))
             extraction::long_switch(std::string(""), (*arguments_iterator),
                                     _switch_initializer_list, argument_map);
            else
            { extraction::long_switch((*next_argument_iterator), (*arguments_iterator),
                                      _switch_initializer_list, argument_map);
              arguments_iterator = next_argument_iterator; } }
          else extraction::long_switch(std::string(""), (*arguments_iterator),
                                       _switch_initializer_list, argument_map); } }
      else reporting::throw_("Invalid switch '%s'", (*arguments_iterator)); }
    return argument_map; }

  namespace extraction
  { std::string attached_value(const std::string& _string) noexcept
    { size_t position = _string.find("=");
      if (position == std::string::npos) return "";
      return _string.substr(position + 1); }

    std::string attached_switch(const std::string& _string) noexcept
    { size_t position = _string.find("=");
      if (position == std::string::npos) return "";
      return _string.substr(0, position); }

    void path(const char* _path, argument_map_t& _argument_map) throw (std::runtime_error)
    { _argument_map.emplace("path", value_t(_path, true)); }

    void long_switch(const std::string& _value, const std::string& _switch,
                     const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                     argument_map_t& _argument_map)
    { short_single_switch(_value, _switch, _switch_initializer_list, _argument_map); }

    void short_multi_switch(const std::string& _switch, const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                            argument_map_t& _argument_map) noexcept
    { for (auto string_iterator = _switch.cbegin() + 1, last_string_iterator = _switch.cend();
           string_iterator != last_string_iterator; ++string_iterator)
      { std::string _switch("-");
        _switch.push_back((*string_iterator));
        const auto& switch_pair = algorithms::find(_switch, _switch_initializer_list);
        if (switch_pair == _switch_initializer_list.end()) reporting::throw_("Unknown switch '%s'.", _switch);
        if (_argument_map.count(_switch)) reporting::throw_("Switch '%s' already set.", _switch);
        value_t value("", switch_pair->second(""));
        _argument_map.emplace(_switch, value); } }

    void short_single_switch(const std::string& _value, const std::string& _switch,
                             const std::initializer_list<switch_pair_t>& _switch_initializer_list,
                             argument_map_t& _argument_map) throw (std::runtime_error)
    { const auto& switch_pair = algorithms::find(_switch, _switch_initializer_list);
      if (switch_pair == _switch_initializer_list.end()) reporting::throw_("Unknown switch '%s'.", _switch);
      if (_argument_map.count(_switch)) reporting::throw_("Switch '%s' already set.", _switch);
      value_t value(_value, switch_pair->second(_value));
      _argument_map.emplace(_switch, value); } }

  namespace reporting
  { void throw_(const std::string& _message, const std::string& _additional) throw (std::runtime_error)
    { if (!_additional.empty())
      { uint32_t error_buffer_size = _message.length() + _additional.length();
        char* error_buffer = new char [error_buffer_size + 1];
        error_buffer[error_buffer_size] = '\0';
        std::snprintf(error_buffer, error_buffer_size, _message.c_str(), _additional.c_str());
        std::string error_string(error_buffer);
        delete [] error_buffer;
        throw (std::runtime_error(error_string.c_str())); }
      else throw (std::runtime_error(_message.c_str())); } }

  namespace recognition
  { bool short_single_switch(const std::string& _string, const switches_t& _switches) noexcept
    { if (_string.length() < 2) return false;
      if (_string[0] != '-') return false;
      if (_string[1] == '-') return false;
      for (auto& _switch : _switches)
      { if (_string == _switch) return true; }
      return false; }

    bool short_multi_switch(const std::string& _string,
                            const std::initializer_list<switch_pair_t>& _switch_initializer_list) noexcept
    { if (_string.length() < 3) return false;
      if (_string[0] != '-') return false;
      if (_string[1] == '-') return false;
      switches_t short_switches(algorithms::collect_short_switches(_switch_initializer_list));
      for (auto string_iterator = _string.cbegin() + 1, last_string_iterator = _string.cend();
           string_iterator != last_string_iterator; ++string_iterator)
      { bool exists = false;
        for (auto _switch = short_switches.begin(), last_switch = short_switches.end();
             _switch != last_switch; ++_switch)
        { if ((*_switch)[1] == (*string_iterator))
          { exists = true;
            _switch = short_switches.erase(_switch);
            break; } }
        if (!exists) return false; }
      return true; }

    bool long_switch(const std::string& _string, const switches_t& _switches) noexcept
    { if (_string.length() < 3) return false;
      if (_string[0] != '-' || _string[1] != '-') return false;
      if (extraction::attached_value(_string).empty())
      { for (auto& _switch : _switches)
        { if (_string == _switch) return true; } }
      else
      { std::string valueless_switch(_string.substr(0, _string.find('=')));
        for (auto& _switch : _switches)
        { if (valueless_switch == _switch) return true; } }
      return false; }

    bool attached_value(const std::string& _string) noexcept
    { for (auto symbol : _string)
      { if (symbol == '=') return true; }
      return false; } }

  namespace validation
  { void main_arguments(int32_t _argument_count, char* _argument_vector[]) throw (std::runtime_error)
    { if (_argument_count < 0) reporting::throw_("Negative argument count.");
      if (!_argument_vector) reporting::throw_("Non-allocated argument vector.");
      for (uint32_t index = 0; index < static_cast<uint32_t>(_argument_count); index++)
      { if (!_argument_vector[index])
         reporting::throw_("Non-allocated entry from argument vector.");
        if (!validation::string(_argument_vector[index]))
         reporting::throw_("Some command-line arguments are not null terminated."); } }

    void path(const char* _path) throw (std::runtime_error)
    { if (!string(_path)) reporting::throw_("Path to executable is invalid."); }

    bool string(const char* _string) noexcept
    { for (uint32_t index = 0; _string[index]; ++index)
      { if (_string[index] < 31 || _string[index] > 126) return false; }
      return true; }

    bool switch_(const std::string& _string) noexcept
    { static const boost::regex pattern("^^(?:\\-[A-Za-z0-9\\-]+|\\-\\-[A-Za-z0-9\\-]+)$");
      return boost::regex_match(_string, pattern); }

    void switches(const switches_t& _switches) throw (std::runtime_error)
    { for (auto& _switch : _switches)
      { if (!switch_(_switch)) reporting::throw_("Switch '%s' is invalid.", _switch); } }

    void switch_duplication(const switches_t& _switches) throw (std::runtime_error)
    { for (auto switchI = _switches.cbegin(), last_switch = _switches.cend();
           switchI != last_switch; ++switchI)
      { for (auto switchJ : _switches)
        { if (std::count(switchI, last_switch, switchJ) > 1)
           reporting::throw_("Switch '%s' is duplicated.", switchJ); } } }

    bool next_argument(arguments_t::const_iterator& _begin,
                       arguments_t::const_iterator& _end) noexcept
    { if (_begin + 1 == _end) return false;
      return true; } }

  namespace algorithms
  { switches_t collect_short_switches(const std::initializer_list<switch_pair_t>& _switch_initializer_list) noexcept
    { switches_t short_switches;
      for (auto switch_pair : _switch_initializer_list)
      { if (switch_pair.first.length() == 2)
         short_switches.push_back(switch_pair.first); }
      return short_switches; }

    switches_t convert(const std::initializer_list<switch_pair_t>& _switch_initializer_list) noexcept
    { switches_t vector;
      for (auto& switch_pair : _switch_initializer_list) vector.push_back(switch_pair.first);
      return vector; }

    arguments_t convert(int32_t _argument_count, char* _argument_vector[]) noexcept
    { arguments_t arguments;
      for (uint32_t index = 0; index != static_cast<uint32_t>(_argument_count); index++)
       arguments.emplace_back(_argument_vector[index]);
      return arguments; }

    std::initializer_list<switch_pair_t>::iterator find(const std::string& _switch,
                                                        const std::initializer_list<switch_pair_t>& _switch_initializer_list)
                                                        noexcept
    { for (auto switch_map_iterator = _switch_initializer_list.begin(),
           last_switch_map_iterator = _switch_initializer_list.end();
           switch_map_iterator != last_switch_map_iterator; switch_map_iterator++)
      { if (switch_map_iterator->first == _switch) return switch_map_iterator; }
      return _switch_initializer_list.end(); } }

  value_t::value_t(const value_t& _value) noexcept
  { value_m = _value.value_m;
    state_m = _value.state_m; }

  value_t::value_t(value_t&& _value) noexcept
  { value_m = _value.value_m;
    state_m = _value.state_m; }

  value_t& value_t::operator=(const value_t& _value) noexcept
  { value_m = _value.value_m;
    state_m = _value.state_m;
    return (*this); }

  std::string value_t::value() const noexcept
  { return value_m; }

  bool value_t::valid() const noexcept
  { return state_m; }

  value_t::value_t(const std::string& _value, bool _valid) noexcept
  { value_m = _value;
    state_m = _valid; }

  namespace value
  { bool none(const std::string& _string) noexcept
    { if (_string.empty()) return true;
      return false; }

    bool numerical(const std::string& _string) noexcept
    { if (_string.empty()) return false;
      for (auto symbol : _string)
      { if (48 > static_cast<char>(symbol) ||
            57 < static_cast<char>(symbol)) return false; }
      return true; }

    bool alphanumerical(const std::string& _string) noexcept
    { if (_string.empty()) return false;
      for (auto symbol : _string)
      { if (!((48 < static_cast<char>(symbol) && 57  > static_cast<char>(symbol)) ||
              (64 < static_cast<char>(symbol) && 91  > static_cast<char>(symbol)) ||
              (96 < static_cast<char>(symbol) && 123 > static_cast<char>(symbol))))
         return false; }
      return true; } }

  #ifdef DEBUG
   namespace debug
   { argument_pair_t convert(const argument_chain_t& _chain) throw (std::runtime_error)
     { uint32_t size = _chain.size();
       if (!size) reporting::throw_("[DEBUG {Argu.cc:263}] Empty argument chain.");
       argument_pair_t argument_pair;
       argument_pair.first = size;
       argument_pair.second = new char* [size];
       for (uint32_t index = 0; index != size; ++index)
       { uint32_t string_length = _chain[index].length();
         argument_pair.second[index] = new char [string_length + 1];
         argument_pair.second[index][string_length] = '\0';
         std::strncpy(argument_pair.second[index], _chain[index].c_str(), string_length); }
       return argument_pair; }

     void deallocate(argument_pair_t& _argument_pair) throw (std::runtime_error)
     { if (!_argument_pair.second)
        reporting::throw_("[DEBUG {Argu.cc:276}] Non-allocated argument pair.");
       for (uint32_t index = 0; index < static_cast<uint32_t>(_argument_pair.first); ++index)
       { if (!_argument_pair.second[index]) reporting::throw_("[DEBUG {Argu.cc:278}] Invalid string.");
         delete [] _argument_pair.second[index];
         _argument_pair.second[index] = nullptr; }
       delete [] _argument_pair.second;
       _argument_pair.second = nullptr;
       _argument_pair.first = 0; } }
  #endif
}
