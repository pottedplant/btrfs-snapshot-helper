#include <atomic>

#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
#include <boost/date_time/posix_time/posix_time.hpp>

#include "log_core.h"
#include "ansi.h"

namespace logging
{

	namespace
	{
		thread_local info::scopes_t local_scopes;

		template<typename Clock,typename Duration>
		boost::posix_time::ptime to_ptime(const std::chrono::time_point<Clock,Duration>& from)
		{
			typedef std::chrono::nanoseconds duration_t;
			typedef duration_t::rep rep_t;

			rep_t d = std::chrono::duration_cast<duration_t>(from.time_since_epoch()).count();
			rep_t sec = d/1000000000;
			rep_t nsec = d%1000000000;

			return boost::posix_time::from_time_t(0)+boost::posix_time::seconds(static_cast<long>(sec))+boost::posix_time::nanoseconds(nsec);
		}

		void format(std::ostream& s,const info::time_point_t& tp)
		{
			boost::posix_time::ptime t(to_ptime(tp));
			std::stringstream ss;
			ss.imbue(std::locale(s.getloc(),new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S.%f")));
			s << (ss << t).rdbuf();
		}

		void default_formatter_fn(std::ostream& s,const info& i,const message& m)
		{
			namespace a = ansi;

			switch(i.level)
			{
			case levels::trace: s << a::cyan; break;
			case levels::debug: s << a::cyan_br; break;
			case levels::info : s << a::white_br; break;
			case levels::warn : s << a::yellow; break;
			case levels::error: s << a::red; break;
			case levels::fatal: s << a::red << a::bold; break;
			}

			format(s,i.time_point);

			switch(i.level)
			{
			case levels::trace: s << " [trace] "; break;
			case levels::debug: s << " [debug] "; break;
			case levels::info : s << " [ info] "; break;
			case levels::warn : s << " [ warn] "; break;
			case levels::error: s << " [error] "; break;
			case levels::fatal: s << " [fatal] "; break;
			default: s << " [unknown] "; break;
			}

			s << a::white << a::bold;

			s << "[" << i.category << "] ";

			s << a::nobold << a::white_br;


			if( i.scopes && !i.scopes->empty() )
				for(auto& scope : *i.scopes)
				{
					s << "<" << scope->name;
					s << ":{";

					for(auto& i:scope->attrs)
						s << i.first << "=" << i.second << ",";

					s << "}> ";
				}

			s << a::white;
			s << m << a::reset;
		}

	}

	message::~message()
	{}

	formatter default_formatter()
	{ return default_formatter_fn; }

	namespace scopes
	{
		void push(const scope* s)
		{ local_scopes.push_back(s); }

		void pop()
		{ local_scopes.pop_back(); }

		info::scopes_t* current()
		{ return &local_scopes; }
	}

	logger::~logger()
	{}

	struct ostream_logger : logger
	{
		ostream_logger(std::ostream& stream,const formatter& fmtr,const levels::level_t& level)
		: stream(stream), fmtr(fmtr), level(level) {}

		void log(const info& i,const message& m)
		{
			if( i.level<level ) return;
			fmtr(stream,i,m); stream << "\n";
		}

		void set_level(const levels::level_t& level)
		{ this->level=level; }

		std::ostream& stream;
		formatter fmtr;
		levels::level_t level;
	};

	namespace
	{
		logger* global_logger = new ostream_logger(std::cerr,default_formatter_fn,levels::warn);
	}

	namespace global
	{

		logger* get()
		{ return global_logger; }

	}

}

