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
  std::cout << "PATH => " << map["path"].value() << std::endl;
  return 0; }
