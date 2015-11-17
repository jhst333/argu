#include "argu.hpp"
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE argu_test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ArguTest001)
{ argu::debug::argument_chain_t chain = { "./argu_test", "-x", "0123456789" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  argu::argument_map_t map(argu::parse(argument_pair.first, argument_pair.second,
                                       { { "-x", argu::value::numerical } } ));
  BOOST_CHECK(map["-x"].value() == "0123456789" &&
              map["-x"].valid() == true);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest002)
{ argu::debug::argument_chain_t chain = { "./argu_test", "-x" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  argu::argument_map_t map = argu::parse(argument_pair.first, argument_pair.second,
                                         { { "-x", argu::value::numerical } } );
  BOOST_CHECK(map["-x"].valid() == false &&
              map["-x"].value() == "");
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest003)
{ argu::debug::argument_chain_t chain = { "./argu_test", "-x", "-x" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "-x", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest004)
{ argu::debug::argument_chain_t chain = { "./argu_test", "-x\x01a", "1" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "-x", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest005)
{ argu::debug::argument_chain_t chain = { "./argu_test", "-xa" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  argu::argument_map_t map = argu::parse(argument_pair.first, argument_pair.second,
                                         { { "-x", argu::value::numerical },
                                           { "-a", argu::value::none } } );
  BOOST_CHECK(map["-x"].valid() == false &&
              map["-x"].value() == "");
  BOOST_CHECK(map["-a"].valid() == true &&
              map["-a"].value() == "");
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest006)
{ argu::debug::argument_chain_t chain = { "./argu_test", "-xa", "1" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "-x", argu::value::numerical },
                                  { "-a", argu::value::none } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest007)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch=" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest008)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-\x01switch=1" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-\x01switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest009)
{ argu::debug::argument_chain_t chain = { "./argu_test", "\xFF--long-switch=1" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "\xFF--long-switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest010)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch=1" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  argu::argument_map_t map = argu::parse(argument_pair.first, argument_pair.second,
                                         { { "--long-switch", argu::value::numerical } } );
  BOOST_CHECK(map["--long-switch"].value() == "1" &&
              map["--long-switch"].valid() == true);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest011)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch", "1", "--long-switch=1" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest012)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch", "aasbaoifgosbudnfgibwen\xFF\x10\x01" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest013)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch", "aasbaoifgosbudnfąćńęgibwen" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest014)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch", "aasbaoifgosbudnfąćńęgibwen" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-switch", argu::value::numerical },
                                  { "--long-switch", argu::value::numerical } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest015)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long-switch", "aasbaoifgosbudnfąćńęgibwen" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long-switch", argu::value::numerical },
                                  { "--long-switch", argu::value::none } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest016)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long=123", "-long" };
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  argu::argument_map_t map = argu::parse(argument_pair.first, argument_pair.second,
                                         { { "--long", argu::value::numerical },
                                           { "-l", argu::value::none },
                                           { "-o", argu::value::none },
                                           { "-n", argu::value::none },
                                           { "-g", argu::value::none } } );
  BOOST_CHECK(map["--long"].value() == "123" &&
              map["--long"].valid() == true &&
              map["-l"].value() == "" &&
              map["-l"].valid() == true &&
              map["-o"].value() == "" &&
              map["-o"].valid() == true &&
              map["-n"].value() == "" &&
              map["-n"].valid() == true &&
              map["-g"].value() == "" &&
              map["-g"].valid() == true);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest017)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--long=123", "-long" "-ngol"};
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  BOOST_CHECK_THROW(argu::parse(argument_pair.first, argument_pair.second,
                                { { "--long", argu::value::numerical },
                                  { "-l", argu::value::none },
                                  { "-o", argu::value::none },
                                  { "-n", argu::value::none },
                                  { "-g", argu::value::none } } ), std::runtime_error);
  argu::debug::deallocate(argument_pair); }

BOOST_AUTO_TEST_CASE(ArguTest018)
{ argu::debug::argument_chain_t chain = { "./argu_test", "--dummy"};
  argu::debug::argument_pair_t argument_pair = argu::debug::convert(chain);
  argu::argument_map_t argument_map = argu::parse(argument_pair.first, argument_pair.second,
                                                  { { "--dummy", argu::value::none } } );
  BOOST_CHECK(argument_map["path"].value() == "./argu_test" &&
              argument_map["path"].valid() == true);
  argu::debug::deallocate(argument_pair); }
