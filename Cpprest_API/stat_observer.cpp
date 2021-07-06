#include "g_config.h"

long long current_time_sec(void)
{
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	return te.tv_sec;
}

long long diff_times_sec(long long t_new, long long t_old)
{
	if (t_new < t_old)
	{
		fprintf(fd_log, "\n\n ---------------------- Warning. t_new(%lld) < t_old(%lld). Wraparound required\n\n", t_new, t_old);
		fflush(fd_log);
	}
	return (t_new - t_old);
}
void state_checker_loop(void)
{
	long long now_time_sec;
	long long diff_time_sec;
	std::list<struct tx_info>::iterator itor;

	lock_tx_info_list();
	for(itor=tx_info_list.begin();itor != tx_info_list.end();itor++)
	{
		if ((*itor).state == TX_ST_INIT)
		{
			now_time_sec = current_time_sec();
			diff_time_sec = diff_times_sec(now_time_sec, (*itor).init_time);
			//if (diff_time_sec > TX_INFO_INIT_TIMEOUT_SEC)
			if (diff_time_sec > 10)
			{
				

				//해당 정보 삭제 
				tx_info_list.erase(itor);
				break;
			}
			
		}
		else if ((*itor).state == TX_ST_READY)
		{
			now_time_sec = current_time_sec();
			diff_time_sec = diff_times_sec(now_time_sec, (*itor).init_time);
			if (diff_time_sec > TX_INFO_READY_TIMEOUT_SEC)
			{
				
				//해당 정보 삭제 
				tx_info_list.erase(itor);
				break;
			}
		}
		else if ((*itor).state == TX_ST_CONNECTED)
		{
			now_time_sec = current_time_sec();
			diff_time_sec = diff_times_sec(now_time_sec, (*itor).init_time);
			if (diff_time_sec > TX_INFO_KEEP_ALIVE_TIMEOUT_SEC)
			{
				
				//해당 정보 삭제 
				tx_info_list.erase(itor);
				break;
			}
		}
		else if ((*itor).state == TX_ST_CONNECTING)
		{
			//reboot
		}
	}
	unlock_tx_info_list();
}
void* state_checker(void* ptr)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	while (g_stop_server != 1)
	{
		state_checker_loop();
	}
	pthread_exit(0); /* exit */
}