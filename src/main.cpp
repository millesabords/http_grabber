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
//#include "boost/spirit/home/phoenix/stl/container/container.hpp"

//#include <string>
//#include <vector>

#include <boost/spirit/include/support_multi_pass.hpp>

using namespace boost::filesystem;
//using namespace  std;

namespace spirit = boost::spirit;
namespace qi      = boost::spirit::qi;
namespace ascii   = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

    //  using spirit::ascii::space;
    // using spirit::ascii::char_;
    // using spirit::qi::double_;
    // using spirit::qi::eol;
    using ascii::space;

template <typename Iterator>
struct grammarIntoMap
  : qi::grammar<Iterator, double()>//std::map<std::string, double>()>
{
  grammarIntoMap()
    : grammarIntoMap::base_type(start)
  {
    
    //start = qi::lit("Date,Open,High,Low,Close,Volume,Adj Close") >> qi::eol >> qi::double_[phoenix::ref(yaha) = _1] >> qi::eol;
    start = qi::lit("Date,Open,High,Low,Close,Volume,Adj Close") >> qi::eol >> qi::double_ >> qi::eol;
    // start =  pair >> *((qi::lit(';') | '&') >> pair);
    // pair  =  key >> -('=' >> value);
    // key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
    // value = +qi::char_("a-zA-Z_0-9");
// rule = alpha > exclamation;
// on_error<fail>(rule,
//    std::cerr << val("Expected ") << _4 << " at offset "
//       << (_3 - _1) << " in \" << std::string(_1, _2) << '"'
//       << std::endl);
  }
  qi::rule<Iterator, double()> start;
  //qi::rule<Iterator, std::map<std::string, double> start;
  // qi::rule<Iterator, std::pair<std::string, double> pair;
  // qi::rule<Iterator, std::string()> key;
  // qi::rule<Iterator, double> value;
};

struct g : qi::grammar<std::string::const_iterator, std::vector<double> >
{
    g() : g::base_type( start )
    {
      start = +(qi::double_ >> ',') >> qi::double_ >> qi::eol;
    }

  qi::rule<std::string::const_iterator, std::vector<double> > start;
};

bool parse_numbers(std::string pathname)
{

     // std::string test="2012-12-21 12:10:35";
     // std::string::const_iterator iter=test.begin();
     // std::string::const_iterator end=test.end();
     // qi::rule<std::string::const_iterator> tc_;
     // std::tm tm_;

     // using qi::int_;
     // using qi::space;
     // using qi::_1;
     // tc_     =   int_[phx::ref(tm_.tm_year)=(_1-1900)]>>'-'
     //                  >>int_[phx::ref(tm_.tm_mon)=(_1-1)]>>'-'
     //                 >>int_[phx::ref(tm_.tm_mday)=_1]>>+space
     //                 >>int_[phx::ref(tm_.tm_hour)=_1]>>':'
     //                  >>int_[phx::ref(tm_.tm_min)=_1]>>':'
     //                 >>int_[phx::ref(tm_.tm_sec)=_1];
     // bool r = parse(iter,end,tc_);



  using boost::phoenix::push_back;

    typedef std::string::const_iterator iterator;


    std::ifstream in(pathname);
    spirit::istream_iterator begin(in);
    spirit::istream_iterator end;
    // slower version: bool result = spirit::qi::parse(begin, end, qi::double_);

    // convert input iterator to forward iterator (faster), usable by spirit parser
    typedef std::istreambuf_iterator<char> base_iterator_type;
    typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
    base_iterator_type in_begin(in);
    forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
    forward_iterator_type fwd_end = boost::spirit::make_default_multi_pass(base_iterator_type());

// wrap forward iterator with position iterator, to record the position (for error recognition)
    // typedef spirit::classic::position_iterator2<forward_iterator_type> pos_iterator_type;
    // pos_iterator_type position_begin(fwd_begin, fwd_end, pathname);
    // pos_iterator_type position_end;

    double yaha = 42.42;
    //bool result = spirit::qi::parse(fwd_begin, fwd_end, qi::double_[phoenix::ref(yaha) = _1] >> qi::eol);

// // // // // // // // std::map<std::string, double> dcMap;//map containing date and closure value information
    //grammarIntoMap<std::string::const_iterator> grammarInstance();
    //bool result = spirit::qi::parse(fwd_begin, fwd_end, grammarInstance, yaha);
    // bool result = spirit::qi::parse(fwd_begin, fwd_end, 
    // 				    qi::lit("Date,Open,High,Low,Close,Volume,Adj Close") >> qi::eol >> qi::double_ >> qi::eol,
    // 				    yaha);
    
    //try{   
    //this works:
    // bool result = spirit::qi::parse(fwd_begin, fwd_end, 
    // 				    qi::lit("Date,Open,High,Low,Close,Volume,Adj Close") >> qi::eol >>
    // 				    *(int_[phoenix::ref(tm_.tm_year)=(qi::_1-1900)] >>
    // 				    '-' >> int_[phoenix::ref(tm_.tm_mon)=(qi::_1-1)] >> '-' >>
    // 				    int_[phoenix::ref(tm_.tm_mday)=qi::_1] >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::long_ >> ',' >>
    // 				      qi::double_[phoenix::ref(yaha) = qi::_1] >> qi::eol)
    // 				    );
    // }
    // catch(const qi::expectation_failure<pos_iterator_type>&amp; e)
    //   {
    // 	const spirit::classic::file_position_base<std::string>& pos =
    // 	  e.first.get_position();
    // 	std::stringstream msg;
    // 	msg <<
    // 	  "parse error at file " << pos.file <<
    // 	  " line " << pos.line << " column " << pos.column << std::endl <<
    // 	  "'" << e.first.get_currentline() << "'" << std::endl <<
    // 	  std::setw(pos.column) << " " << "^- here";
    // 	throw std::runtime_error(msg.str());
    //   }
    struct tm tm_;
    std::vector<int> vYears;
    std::vector<int> vMonths;
    std::vector<int> vDays;
    std::vector<double> taist;

    bool r = qi::parse(fwd_begin, fwd_end, 

        //  Begin grammar
        (
	 qi::double_[push_back(phoenix::ref(taist), qi::_1)]
	 >> *(',' >> qi::double_[push_back(phoenix::ref(taist), qi::_1)])
	 ));
    // bool result = spirit::qi::parse(fwd_begin, fwd_end, 
    // 				    qi::lit("Date,Open,High,Low,Close,Volume,Adj Close") >> qi::eol >>
    // 				    *(int_[push_back(vYears)] >>//, qi::_1-1900)] >>
    // 				      '-' >> int_[phoenix::ref(tm_.tm_mon)=(qi::_1-1)] >> '-' >>
    // 				      int_[phoenix::ref(tm_.tm_mday)=qi::_1] >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::double_ >> ',' >>
    // 				      qi::long_ >> ',' >>
    // 				      qi::double_[phoenix::ref(yaha) = qi::_1] >> qi::eol)
    // 				    );

    // // // // // // // //  bool result = qi::parse(first, last,  grammarInstance, mapDC);

    std::cout << "here is yaha: " << yaha << std::endl;
    std::cout << "here is parse res: " << r << std::endl;

    // if (first != last) // fail if we did not get a full match
    //     return false;
    return r;
}

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      std::cout << "  Usage: ./makemeans argpath" << std::endl;
      return 42;
    }

  try
  {
    bool res;
    path p (argv[1]);   // p reads clearer than argv[1] in the following code
    if (exists(p) && is_directory(p))      // is p a directory?
      {

	directory_iterator end;
	for( directory_iterator iter(p); iter != end ;++iter )
	  if ( is_regular_file(iter->path()) && iter->path().extension().string().compare(".csv") == 0)
	      {
		if(file_size(iter->path()) >5000000)
		  {
		    std::cerr << "file "
			 << iter->path()
			 << " too long ("
			 << file_size(iter->path())
			 << "kb), skipped" << std::endl;
		  }

		//slower version:
		// std::vector<double> ddata;
		res = parse_numbers(iter->path().c_str());

		if(!res)
		  return 43;
		return 0;//temporary for debug sesions
	      }
	return 0;
      }

    return 44;
  }

  catch (const filesystem_error& ex)
  {
    std::cout << ex.what() << '\n';
    return 45;
  }
}

