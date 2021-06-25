#include <boost/log/trivial.hpp>
#include <boost/date_time.hpp>
#include "json.h"
#include "cinatra.hpp"

using namespace cinatra;
using namespace boost::gregorian;

inline std::string make_json_string(const Json::Value &value)
{
	Json::StreamWriterBuilder writer;
	return Json::writeString(writer, value);
}

int main()
{
	std::string s("2001-10-9"); //2001-October-09
	date d(from_simple_string(s));
	BOOST_LOG_TRIVIAL(info) << to_simple_string(d);

	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
	BOOST_LOG_TRIVIAL(error) << "An error severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

	int max_thread_num = std::thread::hardware_concurrency();
	http_server server(max_thread_num);
	server.listen("0.0.0.0", "8080");
	server.set_http_handler<GET, POST>("/", [](request &req, response &res)
									   {
										   Json::Value root;
										   root["hello"] = "world";
										   res.set_status_and_content(status_type::ok, make_json_string(root));
									   });

	server.run();
	return 0;
}
