# Argu
*Argu* is a library designed in *C++* to handle command-line arguments in a modern way.
- Enforces usage of user-defined functions which are validating input against expected patterns.
- Extremely easy to use library interface.
- Exceptions are thrown in case of pathological command-line input, duplicated and invalid switches.

##Working example

    #include <argu.hpp>
    #include <iostream>
    
    int main(int argc, char* argv[])
    { argu::argument_map_t map = argu::parse(argc, argv,
                                             { { "-help", argu::value::numerical },
                                               { "-h", argu::value::none },
                                               { "-e", argu::value::none },
                                               { "-l", argu::value::none },
                                               { "-p", argu::value::none },
                                               { "-v", argu::value::numerical },
                                               { "--long-switch", argu::value::alphanumerical } } );
      for (auto& map_entry : map)
       std::cout << map_entry.first << " => ("
                 << map_entry.second.value() << ") (Valid: "
                 << map_entry.second.valid() << ")\n";
      return 0; }


##Code description

    #include <argu.hpp>
Includes *argu* header file.

    argu::argument_map_t map = argu::parse(argc, argv, /* ... */

Creates an *argument_map_t* object and assigns into it a result of parsing command-line arguments..

      /* ... */
      { { "-help", argu::value::numerical },
        { "-h", argu::value::none },
        { "-e", argu::value::none },
        { "-l", argu::value::none },
        { "-p", argu::value::none },
        { "-v", argu::value::numerical },
        { "--long-switch", argu::value::alphanumerical } } );

Array of pairs which instructs library how to parse command-line arguments.

    { "-help", argu::value::numerical }

This lines tells library that it should search for "-help" switch and then extract the value next to it. If a value was successfully extracted, then it executes *value::numerical* function which checks (in this case) if a value is assembled from digits. You can write your own function for much complex patterns. The *argu::value::none* function should be used if You don't expect any input.

      for (auto& map_entry : map)
       std::cout << map_entry.first << " => ("
                 << map_entry.second.value() << ") (Valid: "
                 << map_entry.second.valid() << ")\n"; 
 This loop iterates over whole associative array, reference to each pair is stored in *map_entry*. You can get to know switch by displaying *map_entry.first* variable. Representation of value is stored in *value_t* instance. *value_t* has 2 public methods, *.value()* and *.valid()*. *.valid()* returns true if value matches to expected pattern, false otherwise.
 
Alternative and more straight-forward method to access value is to use *std::map* interface.

    if (map["--long-switch"].valid() == true &&
        map["--long-switch"].value() == '4lph4num3r1c')

You may also access special value *path*, which will return path of your program.

    std::cout << "PATH => " << map["path"].value() << std::endl;


##User-defined pattern matching function
Your function as argument must have constant reference to *std::string* and return *bool* type. 
This is valid definition of simple validator:

     bool my_validator(const std::string& _string)
     { if (_string.length() == 46) return true;
       return false; }
       

##Compilation
CMake >= 2.6 and development files for *boost_regex* library are required in order to compile this library.

     git clone https://github.com/jhst333/argu.git
     cd argu/
     mkdir build/
     cd build/ 
     cmake ..
     make
     sudo make install

