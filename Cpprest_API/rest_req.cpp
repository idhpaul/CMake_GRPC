#include "g_config.h"
#include "rest_req.h"

//uri_builder uri_builder <-set scheme,host,port ,path
// uri _uri(uri_builder.to_uri())
//http_request reqPacket(method::POST)
// reqPacket.set_request_uri(uri)
// reqPacket.set_body(obj.serialize().c_str(),"application/json")
//http_client client(uri)
//client.request(reqPacket.get()).then~
void rest_requset(const string_t scheme,const string_t host,int port, const string_t path, json::value obj) {
	uri_builder builder;
	builder.set_scheme(scheme);
	builder.set_host(host);
	builder.set_port(port);
	builder.set_path(path);
	uri _uri(builder.to_uri());
	http_request reqPacket(methods::POST);
	
	cout << "obj : " <<obj.serialize() << endl;
	//reqPacket.set_request_uri(_uri);
	reqPacket.set_body(obj.serialize(), "application/json");
	http_client client(_uri);

	client.request(reqPacket).then([](http_response resp) { //method
		cout << U("STATUS : ") << resp.status_code() << endl;
		cout << "content-type : " << resp.headers().content_type() << endl;
		
		resp.extract_string(true).then([](string_t sBoby) {
			cout << sBoby << endl;
			}).wait();

		}).wait();
}

void rest_requset(const string_t path, json::value obj) {
	uri_builder builder;
	builder.set_scheme("http");
	builder.set_host(rm_ip);
	builder.set_port(rm_port);
	builder.set_path(path);
	uri _uri(builder.to_uri());
	http_request reqPacket(methods::POST);

	cout << "obj : " << obj.serialize() << endl;
	//reqPacket.set_request_uri(_uri);
	reqPacket.set_body(obj.serialize(), "application/json");
	http_client client(_uri);

	client.request(reqPacket).then([](http_response resp) { //method
		cout << U("STATUS : ") << resp.status_code() << endl;
		cout << "content-type : " << resp.headers().content_type() << endl;

		resp.extract_string(true).then([](string_t sBoby) {
			cout << sBoby << endl;
			}).wait();
		}).wait();
}
void rest_requset_session_connect(const char* state, const char* subscriptionKey) {
	uri_builder builder;
	builder.set_scheme("http");
	builder.set_host(rm_ip);
	builder.set_port(rm_port);
	builder.set_path("/mslm/session-connection");
	uri _uri(builder.to_uri());
	http_request reqPacket(methods::POST);

	json::value obj;
	obj["connection-info"]["session-state"] = json::value::string(state);
	obj["connection-info"]["subscriptionKey"] = json::value::string(subscriptionKey);

	cout << "obj : " << obj.serialize() << endl;
	reqPacket.set_body(obj.serialize(), "application/json");
	http_client client(_uri);

	client.request(reqPacket).then([](http_response resp) { //method
		cout << U("STATUS : ") << resp.status_code() << endl;
		cout << "content-type : " << resp.headers().content_type() << endl;

		resp.extract_string(true).then([](string_t sBoby) {
			cout << sBoby << endl;
			}).wait();

		}).wait();
}