#include <stdint.h>

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <queue>
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

enum CloudeinRegion
{
	SEOUL_PRODUCT = 0,
	SEOUL_DEVELOPE = 1,
	SEOUL_DRAFT = 2
};

struct SessionInfo
{
	CloudeinRegion region;
	std::string tx_name;
	uint32_t allocate_port1;
	uint32_t allocate_port2;
	uint32_t allocate_port3;
	uint32_t allocate_port4;
	uint32_t allocate_port5;

	SessionInfo()
	{
		region = SEOUL_DEVELOPE;
		tx_name = "SEOUL_DEV_TX";
		allocate_port1 = 0;
		allocate_port2 = 0;
		allocate_port3 = 0;
		allocate_port4 = 0;
		allocate_port5 = 0;
	}
	SessionInfo(const CloudeinRegion reg, const std::string name)
	{
		region = reg;
		tx_name = name;
		allocate_port1 = 0;
		allocate_port2 = 0;
		allocate_port3 = 0;
		allocate_port4 = 0;
		allocate_port5 = 0;
	}
};

enum CLOUDEIN_GRPC_TAG
{
	DoAllocateAPI = 1,
	DoPrepareAPI = 2,
	DoConnectAPI = 3,
	DoReleaseAPI = 4
};

class CloudeinClient {
public:
	explicit CloudeinClient(std::shared_ptr<Channel> channel)
		: stub_(CloudeinSession::NewStub(channel)) {}

	void DoAllocate(const SessionInfo session_info) {

		AllocateRequest request;
		request.set_region(std::to_string(session_info.region));
		request.set_tx_name(session_info.tx_name);

		AsyncClientCall_DoAllocate* call = new AsyncClientCall_DoAllocate;
		call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
		call->context.set_deadline(call->deadline_);

		Enqueue_tag_idx(CLOUDEIN_GRPC_TAG::DoAllocateAPI, (void*)call);

		call->response_reader_allocate = stub_->PrepareAsyncDoAllocateAPI(&call->context, request, &cq_);
		call->response_reader_allocate->StartCall();
		call->response_reader_allocate->Finish(&call->resp, &call->status, (void*)call);
	}

	void DoPrepare(const SessionInfo session_info) {

		PrepareRequest request;
		request.set_region(std::to_string(session_info.region));
		request.set_tx_name(session_info.tx_name);

		AsyncClientCall_DoPrepare* call = new AsyncClientCall_DoPrepare;
		call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
		call->context.set_deadline(call->deadline_);

		Enqueue_tag_idx(CLOUDEIN_GRPC_TAG::DoPrepareAPI, (void*)call);

		call->response_reader_prepare = stub_->PrepareAsyncDoPrepareAPI(&call->context, request, &cq_);
		call->response_reader_prepare->StartCall();
		call->response_reader_prepare->Finish(&call->resp, &call->status, (void*)call);
	}

	void DoConnect(const SessionInfo session_info) {

		ConnectRequest request;
		request.set_region(std::to_string(session_info.region));
		request.set_tx_name(session_info.tx_name);

		AsyncClientCall_DoConnect* call = new AsyncClientCall_DoConnect;
		call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
		call->context.set_deadline(call->deadline_);

		Enqueue_tag_idx(CLOUDEIN_GRPC_TAG::DoConnectAPI, (void*)call);

		call->response_reader_connect = stub_->PrepareAsyncDoConnectAPI(&call->context, request, &cq_);
		call->response_reader_connect->StartCall();
		call->response_reader_connect->Finish(&call->resp, &call->status, (void*)call);
	}

	void DoRelease(const SessionInfo session_info) {

		ReleaseRequest request;
		request.set_region(std::to_string(session_info.region));
		request.set_tx_name(session_info.tx_name);

		AsyncClientCall_DoRelease* call = new AsyncClientCall_DoRelease;
		call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
		call->context.set_deadline(call->deadline_);

		Enqueue_tag_idx(CLOUDEIN_GRPC_TAG::DoReleaseAPI, (void*)call);

		call->response_reader_release = stub_->PrepareAsyncDoReleaseAPI(&call->context, request, &cq_);
		call->response_reader_release->StartCall();
		call->response_reader_release->Finish(&call->resp, &call->status, (void*)call);
	}

	void AsyncCompleteRpc() {
		void* got_tag;
		bool ok = false;
		std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>>::iterator it;

		while (cq_.Next(&got_tag, &ok))
		{
			auto tag = Dequeue_tag_idx(got_tag);
			
			if (tag == CLOUDEIN_GRPC_TAG::DoAllocateAPI)
			{
				AsyncClientCall_DoAllocate* call = static_cast<AsyncClientCall_DoAllocate*>(got_tag);

				//GPR_ASSERT(ok);

				if (call->status.ok())
				{
					std::cout << "DoALLocateAPI-resp OK region: " << call->resp.region() << std::endl;
					std::cout << "DoALLocateAPI-resp OK alloc-port 1: " << call->resp.allocate_port1() << std::endl;
					std::cout << "DoALLocateAPI-resp OK alloc-port 2: " << call->resp.allocate_port2() << std::endl;
					std::cout << "DoALLocateAPI-resp OK alloc-port 3: " << call->resp.allocate_port3() << std::endl;
					std::cout << "DoALLocateAPI-resp OK alloc-port 4: " << call->resp.allocate_port4() << std::endl;
					std::cout << "DoALLocateAPI-resp OK alloc-port 5: " << call->resp.allocate_port5() << std::endl;
				}
				else if (call->status.error_code() == grpc::StatusCode::NOT_FOUND)
				{
					std::cout << "DoALLocateAPI-resp NotFound please retry" << std::endl;
				}
				else
					std::cout << "DoALLocateAPI-resp failed" << std::endl;

				delete call;
			}
			else if (tag == CLOUDEIN_GRPC_TAG::DoPrepareAPI)
			{
				AsyncClientCall_DoPrepare* call = static_cast<AsyncClientCall_DoPrepare*>(got_tag);

				//GPR_ASSERT(ok);

				if (call->status.ok())
					std::cout << "DoPrepareAPI-resp OK region: " << call->resp.region() << std::endl;
				else
					std::cout << "RPC failed" << std::endl;

				delete call;
			}
			else if (tag == CLOUDEIN_GRPC_TAG::DoConnectAPI)
			{
				AsyncClientCall_DoConnect* call = static_cast<AsyncClientCall_DoConnect*>(got_tag);

				//GPR_ASSERT(ok);

				if (call->status.ok())
					std::cout << "DoPrepareAPI-resp OK region: " << call->resp.region() << std::endl;
				else
					std::cout << "RPC failed" << std::endl;

				delete call;
			}
			else if (tag == CLOUDEIN_GRPC_TAG::DoReleaseAPI)
			{
				AsyncClientCall_DoRelease* call = static_cast<AsyncClientCall_DoRelease*>(got_tag);

				//GPR_ASSERT(ok);

				if (call->status.ok())
					std::cout << "DoPrepareAPI-resp OK region: " << call->resp.region() << std::endl;
				else
					std::cout << "RPC failed" << std::endl;

				delete call;
			}
			
		}
	}

private:

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

	void Enqueue_tag_idx(CLOUDEIN_GRPC_TAG tag_idx, void* objAddr);
	CLOUDEIN_GRPC_TAG Dequeue_tag_idx(void* objAddr);

	std::unique_ptr<CloudeinSession::Stub> stub_;
	CompletionQueue cq_;

	std::mutex queue_mutex_;
	std::queue<std::pair<CLOUDEIN_GRPC_TAG, void*>> queue_tag_idx_;
};


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

class CloudeinSessionContext
{
public:
	CloudeinSessionContext();
	CloudeinSessionContext(SessionInfo info);
	~CloudeinSessionContext();

	void setGRPC(const std::string ip, const std::string port);
	void setState(std::shared_ptr<CloudeinState> state);

	void request();
	std::string response();

	friend class StateAllocate;
	friend class StatePrepare;
	friend class StateConnect;
	friend class StateRelease;
private:
	std::shared_ptr<CloudeinState> state_;

	std::shared_ptr<CloudeinClient>grpcClient_;

	std::thread thread_grpc_resp_cq;
};
