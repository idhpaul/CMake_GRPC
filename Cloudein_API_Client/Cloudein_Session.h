/*****************************************************************//**
 * \file   Cloudein_Session.h
 * \brief  Cloudein Session with State Pattern
 * 
 * \author idh
 * \date   June 2021
 *********************************************************************/

#ifndef CLOUDEIN_SESSION_H
#define CLOUDEIN_SESSION_H

#include <memory>
#include <thread>

#include "Cloudein_Session_Common.h"
#include "Cloudein_Session_gRPC_Client.h"
#include "Cloudein_Session_gRPC_Server_Operation.h"

class CloudeinSessionContext;

class CloudeinState
{
public:
	virtual ~CloudeinState() {  }
	virtual void handle(CloudeinSessionContext*) = 0;
	virtual std::string return_handle() = 0;

	static SessionInfo info_;
	std::string state_return_string_;
};

class StateAllocate : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StatePrepare : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateConnect : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateRelease : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateDisconnect : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateTimeout : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StatePrepareFail : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateConnectFail : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateRebootFail : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};
class StateAuthFail : public CloudeinState
{
	void handle(CloudeinSessionContext*);
	std::string return_handle();
};

class CloudeinSessionContext
{
public:
	CloudeinSessionContext();
	CloudeinSessionContext(SessionInfo info);
	~CloudeinSessionContext();

	void set_TS_gRPC(const std::string ip, const std::string port);
	void set_ST_gRPC(const std::string ip, const std::string port);
	void unsetGRPC();

	void setState(std::shared_ptr<CloudeinState> state);

	void request();
	std::string response();

	friend class StateAllocate;
	friend class StatePrepare;
	friend class StateConnect;
	friend class StateRelease;
	friend class StateDisconnect;
	friend class StateTimeout;
	friend class StatePrepareFail;
	friend class StateConnectFail;
	friend class StateRebootFail;
	friend class StateAuthFail;
	// .. add state class
private:
	std::shared_ptr<CloudeinState> state_;

	std::shared_ptr<CloudeinSessionClient>grpcClient_;
	std::shared_ptr<CloudeinSessionOperationServer>grpcServer_;

	std::thread t_TS_gRPC_process;
	std::thread t_ST_gRPC_process;
};

#endif
