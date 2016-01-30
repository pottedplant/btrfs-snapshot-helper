#ifndef LOG_CORE_H
#define LOG_CORE_H

#include <ostream>
#include <chrono>
#include <string>
#include <functional>
#include <sstream>
#include <memory>
#include <map>
#include <vector>
#include <cassert>

namespace logging
{

	namespace levels
	{
		enum level_t { trace,debug,info,warn,error,fatal, };
	}

	typedef std::map<std::string,std::string> attrs_t;

	struct scope
	{
		std::string name;
		attrs_t attrs;
	};

	namespace scopes
	{
		void push(const scope*);
		void pop();

		std::vector<const scope*>* current();

		struct guard
		{
			guard(const scope* s)
			{ assert(s); push(s); }

			~guard()
			{ pop(); }
		};
	}

	struct info
	{
		typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;
		typedef levels::level_t level_t;
		typedef std::string category_t;
		typedef std::vector<const scope*> scopes_t;

		time_point_t time_point;
		level_t level;
		category_t category;
		const scopes_t* scopes;
	};

	struct message
	{
		virtual ~message();
		virtual void write(std::ostream&) const = 0;
	};

	template<typename S>
	S& operator << (S& s,const message& m)
	{ m.write(s); return s; }

	struct string_message : message
	{
		string_message(const std::string& content)
		: content(content) {}

		~string_message() {}

		void write(std::ostream& s) const
		{ s << content; }

		std::string content;
	};

	struct lambda_message : message
	{
		typedef std::function<void(std::ostream&)> callback_t;

		lambda_message(const callback_t& callback)
		: callback(callback) {}

		~lambda_message() {}

		void write(std::ostream& s) const
		{ callback(s); }

		callback_t callback;
	};

	typedef std::function<void(std::ostream&,const info&,const message&)> formatter;

	formatter default_formatter();

	struct logger
	{
		virtual ~logger();

		virtual void log(const info&,const message&) = 0;

		virtual void set_level(const levels::level_t&) = 0;
	};

	struct stream_adapter
	{
		stream_adapter(logger* l,const info& i)
		: l(l),i(i) {}

		~stream_adapter()
		{ l->log(i,string_message(stream.str())); }
		
		logger* l;
		const info& i;
		std::stringstream stream;
	};

	inline const char* to_string(const levels::level_t& l)
	{
		switch(l)
		{
		case levels::trace: return "trace";
		case levels::debug: return "debug";
		case levels::info : return "info" ;
		case levels::warn : return "warn" ;
		case levels::error: return "error";
		case levels::fatal: return "fatal";

		default: return "unknown";
		}
	}

	struct no_such_level {};

	inline levels::level_t level_from_string(const std::string& v)
	{
		if( v=="trace" ) return levels::trace;
		if( v=="debug" ) return levels::debug;
		if( v=="info" ) return levels::info;
		if( v=="warn" ) return levels::warn;
		if( v=="error" ) return levels::error;
		if( v=="fatal" ) return levels::fatal;
		
		throw no_such_level();
	}

	namespace global
	{
		logger* get();
	}

}

#endif

