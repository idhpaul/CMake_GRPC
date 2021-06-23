#include <stdint.h>

#include <memory>
#include <string>
#include <thread>

//#include <grpc/support/log.h>
//#include <grpcpp/grpcpp.h>
//
//#include "CloudeinAPI.grpc.pb.h"
//
//using grpc::Channel;
//using grpc::ClientAsyncResponseReader;
//using grpc::ClientContext;
//using grpc::CompletionQueue;
//using grpc::Status;
//using cloudeinapi::CloudeinSession;
//using cloudeinapi::AllocateRequest;
//using cloudeinapi::AllocateResponse;
//using cloudeinapi::PrepareRequest;
//using cloudeinapi::PrepareResponse;
//using cloudeinapi::ConnectRequest;
//using cloudeinapi::ConnectResponse;
//using cloudeinapi::ReleaseRequest;
//using cloudeinapi::ReleaseResponse;
//
//class GreeterClient {
//public:
//	explicit GreeterClient(std::shared_ptr<Channel> channel)
//		: stub_(CloudeinSession::NewStub(channel)) {}
//	void DoAllocate(const std::string& user) {
//
//		AllocateRequest request;
//		request.region();
//		request.tx_name()
//
//
//		AsyncClientCall* call = new AsyncClientCall;
//
//		call->response_reader = stub_->PrepareAsyncDoAllocate(&call->context, request, &cq_);
//
//		// StartCall initiates the RPC call
//		call->response_reader->StartCall();
//
//		// Request that, upon completion of the RPC, "reply" be updated with the
//		// server's response; "status" with the indication of whether the operation
//		// was successful. Tag the request with the memory address of the call
//		// object.
//		call->response_reader->Finish(&call->reply, &call->status, (void*)call);
//	}
//
//	void AsyncCompleteRpc() {
//		void* got_tag;
//		bool ok = false;
//
//		// Block until the next result is available in the completion queue "cq".
//		while (cq_.Next(&got_tag, &ok)) {
//			// The tag in this example is the memory location of the call object
//			AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
//
//			// Verify that the request was completed successfully. Note that "ok"
//			// corresponds solely to the request for updates introduced by Finish().
//			GPR_ASSERT(ok);
//
//			if (call->status.ok())
//				std::cout << "Greeter received: " << call->reply.message() << std::endl;
//			else
//				std::cout << "RPC failed" << std::endl;
//
//			// Once we're complete, deallocate the call object.
//			delete call;
//		}
//	}
//
//private:
//
//	struct AsyncClientCall {
//
//		AllocateResponse resp;
//		ClientContext context;
//		Status status;
//		std::unique_ptr<ClientAsyncResponseReader<AllocateResponse>> response_reader;
//	};
//
//	std::unique_ptr<CloudeinSession::Stub> stub_;
//	CompletionQueue cq_;
//};



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

class CloudeinState
{
public:
	virtual ~CloudeinState() {  }
	virtual void handle() = 0;
	virtual std::string return_handle() = 0;

	static SessionInfo info_;
	std::string state_return_string_;
};

class StateAllocate : public CloudeinState
{
	void handle();
	std::string return_handle();
};
class StatePrepare : public CloudeinState
{
	void handle();
	std::string return_handle();
};
class StateConnect : public CloudeinState
{
	void handle();
	std::string return_handle();
};
class StateRelease : public CloudeinState
{
	void handle();
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

private:
	std::shared_ptr<CloudeinState> state_;
};
