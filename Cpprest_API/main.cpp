#include "g_config.h"
#include "rest_req.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

//global value
unsigned char g_stop_server = 0;

//thread
pthread_t thread_RM_receiver;
pthread_t thread_TX_receiver;
pthread_t thread_state_checker;

FILE* fd_log;

int start_RM_receiver(void)
{
    int rc;

    rc = pthread_create(&thread_RM_receiver, NULL, RM_receiver, NULL);
    if (rc) {
        fprintf(fd_log, "ERROR; [%s] RM_receiver start failed %d\n", __FUNCTION__, rc);
        fflush(fd_log);
        return -1;
    }

    return 0;
}
int start_state_checker(void)
{
    int rc;

    rc = pthread_create(&thread_state_checker, NULL, state_checker, NULL);
    if (rc) {
        fprintf(fd_log, "ERROR; [%s] state_checker start failed %d\n", __FUNCTION__, rc);
        fflush(fd_log);

        return -1;
    }

    return 0;
}

void stop_handler(void) {
    g_stop_server = 1;
}
void init_data(void) {
    memset(&rm_ip, 0x00, sizeof(rm_ip));
    memset(&rm_port, 0x00, sizeof(rm_port));
}
int main(int argc, char* argv[])
{
    char* log_file_name = (char*)DEFAULT_LOG_FILE_NAME;
    char buf[256];
    time_t t = time(0);
    struct tm* now = localtime(&t);
    if (now != NULL)
    {
        sprintf(buf, "%s_%04d%02d%02d_%02d%02d%02d", DEFAULT_LOG_FILE_NAME, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    }
    else
    {
        sprintf(buf, "%s", DEFAULT_LOG_FILE_NAME);
    }
    log_file_name = buf;

    init_data();

    start_RM_receiver();
    start_state_checker();

    while (g_stop_server != 1);
    return 0;
}
