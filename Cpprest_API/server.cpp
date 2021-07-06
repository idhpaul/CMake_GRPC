#include "g_config.h"
#include "rest_req.h"
#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <list>

#include <pthread.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

char rm_ip[32];
int rm_port;

static struct tx_info tx_received;
std::list<struct tx_info> tx_info_list;
pthread_mutex_t tx_info_list_lock = PTHREAD_MUTEX_INITIALIZER;


void lock_tx_info_list(void)
{
    pthread_mutex_lock(&tx_info_list_lock);
}

void unlock_tx_info_list(void)
{
    pthread_mutex_unlock(&tx_info_list_lock);
}
void display_tx_list_ip() {
    for (struct tx_info tmp : tx_info_list) {
        cout << "tx-ip : " << tmp.tx_ip << endl;
    }
}
int if_there_is_not_data_tx_info_save(char* tx_ip) {
    
    // 스레드 락 
    for (struct tx_info tmp : tx_info_list) 
    {
        if (strcmp(tmp.tx_ip, tx_ip) == 0) 
        {
            //이미 가지고 있는 데이터
            cout << "\t Already exists TX information \n" << endl;
            return 0;
        }
    }
    tx_info_list.push_back(tx_received);
    return 1;
}
void tx_info_state_init(char* tx_ip)
{
    std::list<struct tx_info>::iterator itor;
    for (itor = tx_info_list.begin(); itor != tx_info_list.end(); itor++)
    {
        if (strcmp((*itor).tx_ip, tx_ip) == 0) 
        {
            (*itor).state = TX_ST_INIT;
            (*itor).init_time = current_time_sec();
            return;
        }
    }
    cout << "stat 변경 실패" << endl;
}


void recvdata_copy(json::value obj) {

    //TODO: 210625 스레드 락
    strncpy(tx_received.tx_ip, obj["set-tx-configuration"]["tx-ip"].as_string().c_str(), sizeof(tx_received.tx_ip));
    strncpy(tx_received.port_1, obj["set-tx-configuration"]["port-1"].as_string().c_str(), sizeof(tx_received.port_1));
    strncpy(tx_received.port_2, obj["set-tx-configuration"]["port-2"].as_string().c_str(), sizeof(tx_received.port_2));
    strncpy(tx_received.port_3, obj["set-tx-configuration"]["port-3"].as_string().c_str(), sizeof(tx_received.port_3));
    strncpy(tx_received.port_4, obj["set-tx-configuration"]["port-4"].as_string().c_str(), sizeof(tx_received.port_4));
    strncpy(tx_received.port_5, obj["set-tx-configuration"]["port-5"].as_string().c_str(), sizeof(tx_received.port_5));
    strncpy(tx_received.userKey, obj["set-tx-configuration"]["userKey"].as_string().c_str(), sizeof(tx_received.userKey));
    strncpy(tx_received.subscriptionKey, obj["set-tx-configuration"]["subscriptionKey"].as_string().c_str(), sizeof(tx_received.subscriptionKey));
    strncpy(tx_received.iLOport, obj["set-tx-configuration"]["iLOport"].as_string().c_str(), sizeof(tx_received.iLOport));
    strncpy(tx_received.zone, obj["set-tx-configuration"]["zone"].as_string().c_str(), sizeof(tx_received.zone));
    //TODO: 210625 스레드 언락
}

void configuration_handle(http_request request)
{
    json::value req;
    json::value res;
    request.extract_json().then([&](pplx::task<json::value> task) {
        if (strcmp(rm_ip,"\0")==0 && rm_port == NULL) {
            cout << "rm_ip && rm_port null" << endl;
            strncpy(rm_ip, request.get_remote_address().c_str(), sizeof(rm_ip));
            auto header = request.headers();
            auto it = header.find(U("Port"));
            rm_port = std::stoi(it->second);
            cout << "rm_ip : " << rm_ip << endl;
            cout << "rm_port : " << rm_port << endl;
        }
        
        req = task.get();        
        
        recvdata_copy(req);
        lock_tx_info_list();
        if_there_is_not_data_tx_info_save(tx_received.tx_ip);
        tx_info_state_init(tx_received.tx_ip);
        display_tx_list_ip();
        unlock_tx_info_list();
        
        
        cout << req.serialize() << endl;
        cout << "tx-ip : " << req["set-tx-configuration"]["tx-ip"] << endl;
        
        res["set-tx-configuration-resp"]["zone"] = req["set-tx-configuration"]["zone"];
        res["set-tx-configuration-resp"]["userKey"] = req["set-tx-configuration"]["userKey"];

        }).wait();
    request.reply(http::status_codes::OK, res);
    
    rest_requset_session_connect("connect", "2");
}

void operation_handle(http_request request)
{
    json::value req;
    json::value res;
    request.extract_json().then([&](pplx::task<json::value> task) {
        req = task.get();

        cout << req.serialize() << endl;
        
        }).wait();
        request.reply(http::status_codes::OK, res);
}

void* RM_receiver(void* ptr)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    http_listener configuration_listener(U("http://0.0.0.0:9090/set-tx-configuration"));
    configuration_listener.support(methods::POST, configuration_handle);
	
    http_listener perform_listener(U("http://0.0.0.0:9090/perform-operation"));
    perform_listener.support(methods::POST, operation_handle);

    try
    {
        configuration_listener
            .open()
            .then([&configuration_listener]() {cout << "\nstarting to listen" << endl; })
            .wait();
        while (g_stop_server != 1);
    }
    catch (exception const& e)
    {
        cout << "main exception" << endl;
        cout << e.what() << endl;
    }

	pthread_exit(0); /* exit */
	return 0;
}