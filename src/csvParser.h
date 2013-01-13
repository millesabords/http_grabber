#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
//#include <boost/spirit/include/support_istream_iterator.hpp>
#include "boost/spirit/include/qi.hpp"
#include <boost/spirit/include/phoenix_fusion.hpp>
#include "boost/spirit/include/phoenix_core.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
// #include <boost/spirit/include/phoenix_bind.hpp>
// #include <boost/fusion/adapted/adt/adapt_adt.hpp>
// #include <boost/fusion/adapted/struct/define_struct.hpp> 
#include <boost/spirit/include/support_multi_pass.hpp>
#include <vector>
#include "boost/bind.hpp"
//#include "boost/timer.hpp"
#include <ctime>

#define WAX(N) std::cout << "WAX" << N << std::endl;

int parse_directory(const char* pathname);
