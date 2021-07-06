#pragma once
#include <iostream>
using namespace std;

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#pragma comment(lib, "cpprest120_2_4")	 // windows only
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams


//ip,path,method,body
void rest_requset(const string_t scheme, const string_t host,int port, const string_t path, json::value obj);
void rest_requset(const string_t path, json::value obj);
void rest_requset_session_connect(const char* state, const char* subscriptionKey);