#pragma once
#include <pthread.h>
#include <list>
#include <iostream>
#include <sys/time.h>
//extern value
extern unsigned char	g_stop_server;
extern void* RM_receiver(void* ptr);
extern void* state_checker(void* ptr);
extern std::list<struct tx_info> tx_info_list;
extern FILE* fd_log;

extern char rm_ip[32];
extern int rm_port;
//file_name
#define DEFAULT_LOG_FILE_NAME		"sm_server.log"
#define MAX_TOKEN_STRING_LENGTH	64

//stat_time
#define TX_INFO_INIT_TIMEOUT_SEC			(300) // 5 minutes for storage boot, 2018.03.23
#define TX_INFO_READY_TIMEOUT_SEC			(60)
#define TX_INFO_KEEP_ALIVE_TIMEOUT_SEC		(15) // (10) // 10 seconds //(20) // 20 seconds
#define TX_INFO_REBOOT_TIMEOUT_SEC			(170) // (70) // 70 seconds //  reboot
#define TX_INFO_REBOOT_CONNECT_TIME_SEC		(150)

typedef enum
{
	TX_ST_EMPTY = 0,
	TX_ST_INIT,
	TX_ST_READY,
	TX_ST_READY_FAIL,
	TX_ST_AUTH_FAIL,
	TX_ST_CONNECTED,
	TX_ST_CONNECT_FAIL,
	TX_ST_TIMEOUT,
	TX_ST_DISCONNECTED,
	TX_ST_RELEASE,
	TX_ST_REBOOT,
	TX_ST_REBOOT_FAIL,
	TX_ST_INACTIVE_OUT,
	TX_ST_MAX,

	TX_ST_CONNECTING
} enum_tx_state;

struct tx_info { // 768 bytes
	char tx_ip[MAX_TOKEN_STRING_LENGTH];
	char port_1[MAX_TOKEN_STRING_LENGTH];
	char port_2[MAX_TOKEN_STRING_LENGTH];
	char port_3[MAX_TOKEN_STRING_LENGTH];
	char port_4[MAX_TOKEN_STRING_LENGTH];
	char port_5[MAX_TOKEN_STRING_LENGTH];
	char zone_id[MAX_TOKEN_STRING_LENGTH];
	char userKey[MAX_TOKEN_STRING_LENGTH];
	char subscriptionKey[MAX_TOKEN_STRING_LENGTH];
	char iLOport[MAX_TOKEN_STRING_LENGTH];
	char zone[MAX_TOKEN_STRING_LENGTH];
	long long init_time; // milli seconds
	long long ready_to_readyfail_time; // milli seconds
	long long keep_alive_received_time; // milli seconds
	int keep_alive_received;
	int locked;
	int reboot_check;
	enum_tx_state state;
};

long long current_time_sec(void);
void lock_tx_info_list(void);
void unlock_tx_info_list(void);