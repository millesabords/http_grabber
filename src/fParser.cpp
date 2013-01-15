#include "csvParser.h"

using namespace boost::filesystem;

namespace spirit = boost::spirit;
namespace qi      = boost::spirit::qi;
//namespace ascii   = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
//  using spirit::ascii::space;
// using spirit::ascii::char_;
// using spirit::qi::double_;
// using spirit::qi::eol;
//using ascii::space;

void add_la_saucisse_i(int toBePrintedAnyway)
{
  std::cout << "value get: " << toBePrintedAnyway << std::endl;
}

void add_la_saucisse_d(double toBePrintedAnyway)
{
  std::cout << "value get: " << toBePrintedAnyway << std::endl;
}

struct s_dv
{
  s_dv():year(0){}
  s_dv(const short& i):year(i){}
  s_dv(const short& i1, const short& i2, const short& i3, const double& d):year(i1), month(i2), day(i3), val(d){}
  s_dv(const s_dv& e):year(e.year),month(e.month),day(e.day),val(e.val)
  {}

  short year;
  short month;
  short day;
  double val;
};

struct date_and_value
{
  std::vector<s_dv> vals;
};

BOOST_FUSION_ADAPT_STRUCT(s_dv, (short, year)(short, month)(short, day)(double, val));
BOOST_FUSION_ADAPT_STRUCT(date_and_value, (std::vector<s_dv>, vals));

template <typename Iterator>
struct grammarIntoMap2 : qi::grammar<Iterator, date_and_value()>
{
  grammarIntoMap2()
    : grammarIntoMap2::base_type(g_start)
  {
    g_start = g_header >> g_lines;

    g_header = qi::lit("Date,Open,High,Low,Close,Volume,Adj Close");

    g_lines %= *(g_datas);
   
    g_datas = qi::eol >> qi::short_[phoenix::at_c<0>(qi::_val) = qi::_1] >>
      '-' >> qi::short_[phoenix::at_c<1>(qi::_val) = qi::_1] >>
      '-' >> qi::short_[phoenix::at_c<2>(qi::_val) = qi::_1] >>
      qi::repeat(5)[',' >> qi::double_] >> ',' >>
      qi::double_[phoenix::at_c<3>(qi::_val) = qi::_1];

    BOOST_SPIRIT_DEBUG_NODE(g_start);
    BOOST_SPIRIT_DEBUG_NODE(g_header);
    BOOST_SPIRIT_DEBUG_NODE(g_lines);
    BOOST_SPIRIT_DEBUG_NODE(g_datas);
  }

  qi::rule<Iterator, date_and_value()> g_start;
  qi::rule<Iterator> g_header;
  qi::rule<Iterator, std::vector<s_dv>()> g_lines;
  qi::rule<Iterator, s_dv()> g_datas;
};

template<typename Tk, typename Tv>
struct ticks
{
  ticks():ref(std::string("unknown")){}
  ticks(const std::string& ref_given):ref(std::string(ref_given)){}
  ~ticks(){}
  std::string ref;//cpy code name
  std::map<Tk, Tv> spots;
};

template<>
struct ticks<time_t, double>
{
  ticks():ref(std::string("unknown")){}
  ticks(const std::string& ref_given):ref(std::string(ref_given)){}
  ~ticks(){}

  void format_data(const date_and_value& dispersed_data)
  {
    struct tm buf={0};
    time_t key=0;

    for(std::vector<s_dv>::const_iterator iter = dispersed_data.vals.begin(); iter != dispersed_data.vals.end(); iter++)
      {
	buf.tm_mday = iter->day;
	buf.tm_mon = iter->month;
	buf.tm_year = iter->year;
	key = mktime(&buf);
	if(key == -1)
	  std::cerr << "problem with date: " << iter->year << "-" << iter->month << "-" << iter->day << "during format_data" << std::endl;
	spots[key] = iter->val;
      }
  }

  std::string ref;//cpy code name
  std::map<time_t, double> spots;
};

const ticks<time_t, double>* parse_numbers(const std::string& pathname)
{
  //using boost::phoenix::push_back;

  typedef std::string::const_iterator iterator;
  std::ifstream in(pathname);
  spirit::istream_iterator begin(in);
  spirit::istream_iterator end;

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


    //try{   
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

    date_and_value  dispersed_data;
    dispersed_data.vals = std::vector<s_dv>();
    grammarIntoMap2<forward_iterator_type> grammarInstance;
    bool r = qi::parse(fwd_begin, fwd_end, grammarInstance, dispersed_data);

    //if (first != last) // fail if we did not get a full match
      //     return false;
    if(r && dispersed_data.vals.size() > 0)
      {
	ticks<time_t, double>* local_data = new ticks<time_t, double>();//todo: give name here in constructor parameter, and give this allocated pointer of tick as return value, and then it will be part of a vector
	local_data->format_data(dispersed_data);

    // std::cout << "vector length: " << dispersed_data.vals.size() << std::endl;
    // for(s_dv d: dispersed_data.vals)
    //   std::cout << d.year << " " << d.month << " " << d.day << " " << d.val << std::endl;

    // std::cout << "map length: " << local_data->spots.size() << std::endl;
    // for(std::pair<time_t,double> pdata: local_data->spots)
    //   std::cout << pdata.first << " " << pdata.second << std::endl;

	return local_data;
      }
    return NULL;
}

int parse_directory(const char* pathname)
{
  try
  {
    path p (pathname);
    std::cout << "looking for csv files in directory: " << p << std::endl;
    if (exists(p) && is_directory(p))
      {
	unsigned short parse_success = 0, parse_fails = 0;
	directory_iterator end;
	std::vector<const ticks<time_t, double>* > results;
	const ticks<time_t, double>* ptr = 0;

	clock_t ct = std::clock();
	for(directory_iterator iter(p); iter != end ;++iter )
	  if (is_regular_file(iter->path()) && iter->path().extension().string().compare(".csv") == 0)
	    {
		std::cout << "Processing file " << iter->path() << std::endl;
		if(file_size(iter->path()) >5000000)
		  {
		    std::cerr << "file too long (" << file_size(iter->path()) << " kb), skipped" << std::endl;
		    ++parse_fails;
		    continue;
		  }

		ptr = parse_numbers(iter->path().c_str());

		if(!ptr)
		  {
		    std::cerr << "file parser error, skipped" << std::endl;
		    ++parse_fails;
		    continue;
		  }

		std::cout << "debug info: ticks-mem=" << (results.size() * (sizeof(const ticks<time_t, double>))) << " success=" << parse_success << " fails=" << parse_fails << " timer=" << ((std::clock() - ct) / (double)CLOCKS_PER_SEC) << std::endl;
		++parse_success;
		results.push_back(ptr);
	    }

	std::cout << "files succesfully processed: " << parse_success << std::endl;
	std::cout << "parser failures: " << parse_fails << std::endl;
	std::cout << "timing: " << ((std::clock() - ct) / (double)CLOCKS_PER_SEC) << std::endl;

	unsigned int total_size = results.size() * (sizeof(const ticks<time_t, double>) +
					   sizeof(std::string) + (8 * sizeof(char)) +
					   sizeof(std::map<time_t, double>) +
					   sizeof(time_t) + sizeof(double));
	std::cout << "approximative total size of memory occupied: " << total_size << std::endl;
	std::cout << "size of struct \'ticks\': " << sizeof(const ticks<time_t, double>) << std::endl;
	std::cout << "size of a string: " << sizeof(std::string) << std::endl;
	std::cout << "size of 8 chars: " << (8 * sizeof(char)) << std::endl;
	std::cout << "size of a map: " << sizeof(std::map<time_t, double>) << std::endl;
	std::cout << "size of a time_t: " << sizeof(time_t) << std::endl;
	std::cout << "size of a double: " << sizeof(double) << std::endl;
	std::cout << "size of a results: " << results.size() << std::endl;

	for(const ticks<time_t, double>* pt: results)
	  std::cout << "before desctruction size=" << pt->spots.size() << std::endl;

	for_each(results.begin(), results.end(), [](const ticks<time_t, double>* pt) {delete[] pt;});


	for(const ticks<time_t, double>* pt: results)
	  std::cout << "after desctruction size=" << pt->spots.size() << std::endl;

	//even more elegant:
// std::transform(
// 		       Vec.begin(),
// 		       Vec.end(),
// 		       Vec.begin(),
// 		       nullify<TSmart>() );//functor-destructor to build
	return 0;
      }
    
    std::cerr << "error with path given" << std::endl;
    return 1;
  }

  catch (const filesystem_error& ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
}
