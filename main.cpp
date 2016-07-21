#include <map>
#include <memory>
#include <iostream>
#include <functional>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "log.h"

typedef boost::posix_time::ptime datetime_t;
namespace fs = boost::filesystem;

typedef std::function<void(const fs::path&)> snapshot_rm_fn;
typedef std::function<datetime_t(const datetime_t&)> slotter_fn;

struct strategy
{
	virtual ~strategy() {}
	virtual void push(const fs::path&,const datetime_t&) = 0;
	virtual bool create_snapshot(const datetime_t&) = 0;

	typedef std::shared_ptr<strategy> ptr;
};

struct now_strategy : strategy
{
	
	static constexpr const char* LOG_C = "now-strategy";

	size_t max_snapshots;
	snapshot_rm_fn rm_fn;

	std::map<datetime_t,fs::path> snapshots;

	now_strategy(size_t max_snapshots,const snapshot_rm_fn& rm_fn)
	: max_snapshots(max_snapshots), rm_fn(rm_fn)
	{}

	void push(const fs::path& p,const datetime_t& t)
	{
		if( !snapshots.insert(std::make_pair(t,p)).second )
			throw std::runtime_error(str(boost::format("duplicate timestamp: '%1%' @%2%") % p % to_iso_string(t)));

		if( max_snapshots>0 && snapshots.size()>max_snapshots )
		{
			auto i(snapshots.begin());
			fs::path snapshot( i->second );

			LOG_S(info,LOG_C) << "removing superfluous snapshot '" << snapshot << "'";
			rm_fn(snapshot);

			snapshots.erase(i);
		}
	}

	bool create_snapshot(const datetime_t&) { return true; }

};

struct once_per_base : strategy
{
	const char* log_c;
	size_t max_snapshots;
	snapshot_rm_fn rm_fn;

	std::map<datetime_t,fs::path> snapshots;
	typedef std::map<datetime_t,std::pair<datetime_t,fs::path>> slots_type;
	slots_type slots;

	once_per_base(const char* log_c,size_t max_snapshots,const snapshot_rm_fn& rm_fn)
	: log_c(log_c)
	, max_snapshots(max_snapshots)
	, rm_fn(rm_fn)
	{}

	virtual datetime_t slotter(const datetime_t& t) = 0;

	void push(const fs::path& p,const datetime_t& t)
	{
		if( !snapshots.insert(std::make_pair(t,p)).second )
			throw std::runtime_error(str(boost::format("duplicate timestamp: '%1%' @%2%") % p % to_iso_string(t)));

		datetime_t slot(slotter(t));

		slots_type::iterator i(slots.find(slot));
		
		if( i==slots.end() )
			slots.insert( std::make_pair( slot, std::make_pair(t,p) ) );
		else
		{
			fs::path snapshot;

			if( i->second.first<t )
				snapshot = p;
			else
			{
				snapshot = i->second.second;
				i->second = std::make_pair(t,p);
			}

			LOG_S(info,log_c) << "removing superfluous snapshot '" << snapshot << "'";
			rm_fn(snapshot);
		}

		if( max_snapshots>0 && slots.size()>max_snapshots )
		{
			auto i(slots.begin());
			fs::path snapshot( i->second.second );

			LOG_S(info,log_c) << "removing superfluous snapshot '" << snapshot << "'";
			rm_fn(snapshot);

			slots.erase(i);
		}
	}

	bool create_snapshot(const datetime_t& now)
	{
		if( slots.empty() )
			return true;

		return slots.rbegin()->first != slotter(now);
	}
};

struct once_per_slotter_fn : once_per_base
{
	slotter_fn slotter_;

	once_per_slotter_fn(const char* log_c,size_t max_snapshots,const slotter_fn& slotter_,const snapshot_rm_fn& rm_fn)
	: once_per_base(log_c,max_snapshots,rm_fn) {}

	datetime_t slotter(const datetime_t& t) { return slotter_(t); }

};

// once per day

slotter_fn slotter_once_per_day = [](const datetime_t& t){ return datetime_t(t.date(),boost::posix_time::hours(0)); };
strategy::ptr make_once_per_day(size_t max_snapshots,const snapshot_rm_fn& rm_fn) { return std::make_shared<once_per_slotter_fn>("strategy-once-per-day",max_snapshots,slotter_once_per_day,rm_fn); }

int main(int argc,char** argv)
{
	static const char* LOG_C = "main";

	// program option parsing
	
	namespace po = boost::program_options;
	po::variables_map ovm;

	{
		po::options_description odesc("program options");

		odesc.add_options()
			("help,h","show help")
			("subvolume,v",po::value<std::string>()->value_name("path")->required(),"source subvolume to take snapshot of")
			("snapshots-dir,d",po::value<std::string>()->value_name("path")->required(),"snapshot storage directory")
			("strategy,s",po::value<std::string>()->value_name("name")->required(),"now,once-per-day")
			("max-snapshots,m",po::value<size_t>()->value_name("number")->default_value(0),"use 0 for unlimited")
			("log-level,l",po::value<std::string>()->value_name("level")->default_value("warn"))
		;

		po::positional_options_description pdesc;

		try
		{
			store(po::command_line_parser(argc,argv).options(odesc).positional(pdesc).run(),ovm);

			if( ovm.count("help") )
			{
				std::cout << odesc;
				return 0;
			}

			notify(ovm);

			logging::global::get()->set_level(logging::level_from_string(ovm["log-level"].as<std::string>()));
		}
		catch(const po::error& e)
		{
			std::cerr << "error: " << e.what() << std::endl;
			return -1;
		}

	}
	
	try
	{

		// strategy & setup
		
		datetime_t now(boost::posix_time::second_clock::universal_time());

		auto exec = [](const std::string& cmd) {
			int code(system(cmd.c_str()));
			if( code )
				throw std::runtime_error(str(boost::format("exec failed(%1%): %2%")%code%cmd));
		};
		
		auto rm_fn = [&exec](const fs::path& p) {
			exec(str(boost::format("/sbin/btrfs property set -ts %1% ro false") % p));
			exec(str(boost::format("/sbin/btrfs subvolume delete %1%") % p));
		};
	
		auto create_fn = [&exec](const fs::path& p,const fs::path& d) {
			exec(str(boost::format("/sbin/btrfs subvolume snapshot -r %1% %2%")%p%d));
		};
	
		std::string strategy_name(ovm["strategy"].as<std::string>());
		size_t max_snapshots(ovm["max-snapshots"].as<size_t>());

		strategy::ptr s;
		if( strategy_name=="now" )
			s = std::make_shared<now_strategy>( max_snapshots, rm_fn );
		else if( strategy_name=="once-per-day" )
			s = make_once_per_day( max_snapshots, rm_fn );
		else
			throw std::runtime_error(str(boost::format("no strategy named '%1%' available")%strategy_name));
	
		// scan and clean directory
		
		fs::path snapshots_dir(ovm["snapshots-dir"].as<std::string>());

		if( !is_directory(snapshots_dir) )
			throw std::runtime_error(str(boost::format("invalid or missing snapshots directory: '%1%'") % snapshots_dir));
		
		{
			LOG_S(info,LOG_C) << "scanning directory for previous snapshots (" << snapshots_dir << ")";
			
			for(fs::directory_entry e : fs::directory_iterator(snapshots_dir))
				if( is_directory(e) )
				{
					std::string name(e.path().filename().string());
					LOG_S(trace,LOG_C) << "found directory: " << e;
	
					datetime_t t;
	
					try
					{ t = boost::date_time::parse_iso_time<datetime_t>(name,'T'); }
					catch(const boost::bad_lexical_cast&)
					{
						LOG_S(trace,LOG_C) << "parsing of '" << name << "' failed. skipping.";
						continue;
					}
	
					LOG_S(debug,LOG_C) << "found datetime: " << to_iso_string(t);
	
					if( t>=now )
						throw std::runtime_error(str(boost::format("clock error: snapshot timestamp in the future (%s>=%s)") % to_iso_string(t) % to_iso_string(now)));
	
					s->push(e,t);
				}
		}
	
		// create snapshot
		
		fs::path subvolume(ovm["subvolume"].as<std::string>());
		now = boost::posix_time::second_clock::universal_time();

		if( !is_directory(subvolume) )
			throw std::runtime_error(str(boost::format("invalid or missing subvolume directory: '%1%'") % subvolume));

		if( !s->create_snapshot(now) )
		{
			LOG_S(info,LOG_C) << "not creating new snapshot";
			return 0;
		}
	
		{
			fs::path snapshot( snapshots_dir / fs::path(to_iso_string(now)) );
			create_fn( subvolume, snapshot );

			LOG_S(info,LOG_C) << "created new snapshot of '" << subvolume << "' at '" << snapshot << "'";

			s->push(snapshot,now);
		}

	}
	catch(const std::exception& e)
	{
		std::cerr << "error: " << e.what() << std::endl;
		return -2;
	}
}

