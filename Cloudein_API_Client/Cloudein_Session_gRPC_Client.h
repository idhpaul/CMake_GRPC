/*****************************************************************//**
 * \file   Cloudein_Session_gRPC_Client.h
 * \brief  Cloudein Session gRPC Client(async) with state pattern
 * 
 * \author idh
 * \date   June 2021
 *********************************************************************/

#ifndef CLOUDEIN_SESSION_GRPC_CLIENT_H
#define CLOUDEIN_SESSION_GRPC_CLIENT_H

#include "Cloudein_Session_Common.h"

#include <memory>
#include <list>
#include <mutex>
#include <utility>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "CloudeinAPI.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using cloudeinapi::CloudeinSession;
using cloudeinapi::AllocateRequest;
using cloudeinapi::AllocateResponse;
using cloudeinapi::PrepareRequest;
using cloudeinapi::PrepareResponse;
using cloudeinapi::ConnectRequest;
using cloudeinapi::ConnectResponse;
using cloudeinapi::ReleaseRequest;
using cloudeinapi::ReleaseResponse;
using cloudeinapi::DisconnectRequest;
using cloudeinapi::DisconnectResponse;
using cloudeinapi::TimeoutRequest;
using cloudeinapi::TimeoutResponse;
using cloudeinapi::PrepareFailRequest;
using cloudeinapi::PrepareFailResponse;
using cloudeinapi::ConnectFailRequest;
using cloudeinapi::ConnectFailResponse;
using cloudeinapi::RebootFailRequest;
using cloudeinapi::RebootFailResponse;
using cloudeinapi::AuthFailRequest;
using cloudeinapi::AuthFailResponse;

class CloudeinSessionClient {
public:
	explicit CloudeinSessionClient(std::shared_ptr<Channel> channel);

	void StartAsyncCompleteRpc();
	void StopAsyncCompleteRpc();

	void DoAllocate(const SessionInfo session_info);
	void DoPrepare(const SessionInfo session_info);
	void DoConnect(const SessionInfo session_info);
	void DoRelease(const SessionInfo session_info);
	void DoDisconnect(const SessionInfo session_info);
	void DoTimeout(const SessionInfo session_info);
	void DoPrepareFail(const SessionInfo session_info);
	void DoConnectFail(const SessionInfo session_info);
	void DoRebootFail(const SessionInfo session_info);
	void DoAuthFail(const SessionInfo session_info);
	// .. add gRPC calls based on state


private:

	// TODO : Use Templete
	struct AsyncClientCall_DoAllocate
	{
		AllocateResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<AllocateResponse>> response_reader_allocate;
	};
	struct AsyncClientCall_DoPrepare
	{
		PrepareResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<PrepareResponse>> response_reader_prepare;
	};
	struct AsyncClientCall_DoConnect
	{
		ConnectResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<ConnectResponse>> response_reader_connect;
	};
	struct AsyncClientCall_DoRelease
	{
		ReleaseResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<ReleaseResponse>> response_reader_release;
	};
	struct AsyncClientCall_DoDisconnect
	{
		DisconnectResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<DisconnectResponse>> response_reader_disconnect;
	};
	struct AsyncClientCall_DoTimeout
	{
		TimeoutResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<TimeoutResponse>> response_reader_timeout;
	};
	struct AsyncClientCall_DoPrepareFail
	{
		PrepareFailResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<PrepareFailResponse>> response_reader_preparefail;
	};
	struct AsyncClientCall_DoConnectFail
	{
		ConnectFailResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<ConnectFailResponse>> response_reader_connectfail;
	};
	struct AsyncClientCall_DoRebootFail
	{
		RebootFailResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<RebootFailResponse>> response_reader_rebootfail;
	};
	struct AsyncClientCall_DoAuthFail
	{
		AuthFailResponse resp;
		ClientContext context;
		std::chrono::system_clock::time_point deadline_;
		Status status;
		std::unique_ptr<ClientAsyncResponseReader<AuthFailResponse>> response_reader_authfail;
	};
	// .. add gRPC calls based on state

	void tag_push(CLOUDEIN_GRPC_TAG tag_idx, void* objAddr);
	CLOUDEIN_GRPC_TAG tag_pop(void* objAddr);

	std::unique_ptr<CloudeinSession::Stub> stub_;
	CompletionQueue cq_;

	std::mutex mutex_list_tag_idx_;
	std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>> list_tag_idx_;
};

#endif // !CLOUDEIN_SESSION_CLIENT_H

