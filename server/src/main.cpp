#include "json.h"
#include "cinatra.hpp"
using namespace cinatra;

inline std::string make_json_string(const Json::Value &value)
{
	Json::StreamWriterBuilder writer;
	return Json::writeString(writer, value);
}

int main()
{
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
