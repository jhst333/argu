#//: General
 cmake_minimum_required (VERSION 2.6)
 project                (argu)
 set                    (argu_VERSION_MAJOR 1)
 set                    (argu_VERSION_MINOR 0)

#//: Packages
 find_package(Boost COMPONENTS unit_test_framework regex)

#//: libargu.so [RELEASE]
 add_library                (argu source/argu.cc)
 target_include_directories (argu PUBLIC include/)
 target_compile_options     (argu PUBLIC -Wall -Wextra -std=c++11 -fPIC -Ofast)
 target_link_libraries      (argu LINK_PUBLIC ${Boost_REGEX_LIBRARY})

#//: libargu_debug.so [DEBUG]
 if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_library                (argu_debug source/argu.cc)
  target_include_directories (argu_debug PUBLIC include/)
  target_compile_options     (argu_debug PUBLIC -Wall -Wextra -std=c++11 -g -DDEBUG)
  target_link_libraries      (argu_debug LINK_PUBLIC ${Boost_REGEX_LIBRARY})
 endif()

#//: argu_example [RELEASE]
 add_executable             (argu_example source/main.cc)
 target_include_directories (argu_example PUBLIC include/)
 target_compile_options     (argu_example PUBLIC -Wall -Wextra -std=c++11 -fPIC -Ofast)
 target_link_libraries      (argu_example LINK_PUBLIC argu)

#//: argu_example_debug [DEBUG]
 if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_executable             (argu_example_debug source/main.cc)
  target_include_directories (argu_example_debug PUBLIC include/)
  target_compile_options     (argu_example_debug PUBLIC -Wall -Wextra -std=c++11 -g -DDEBUG)
  target_link_libraries      (argu_example_debug LINK_PUBLIC argu_debug)
 endif()

 #//: argu_test [DEBUG]
  if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
   add_executable             (argu_test test/test.cc)
   target_include_directories (argu_test PUBLIC include/)
   target_compile_options     (argu_test PUBLIC -Wall -Wextra -std=c++11 -g -DDEBUG)
   target_link_libraries      (argu_test LINK_PUBLIC argu_debug ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  endif()

INSTALL (FILES include/argu.hpp DESTINATION include)
INSTALL_TARGETS (/lib argu)
