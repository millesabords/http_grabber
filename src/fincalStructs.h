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
