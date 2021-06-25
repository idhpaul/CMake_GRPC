#include "Cloudein_Session_gRPC_Client.h"

CloudeinSessionClient::CloudeinSessionClient(std::shared_ptr<Channel> channel) 
	: stub_(CloudeinSession::NewStub(channel))
{

}

void CloudeinSessionClient::DoAllocate(const SessionInfo session_info) {

	AllocateRequest request;
	request.set_region(std::to_string(session_info.region));
	request.set_tx_name("tx-DoAllocate");

	AsyncClientCall_DoAllocate* call = new AsyncClientCall_DoAllocate;
	call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
	call->context.set_deadline(call->deadline_);

	tag_push(CLOUDEIN_GRPC_TAG::DoAllocateAPI, (void*)call);

	call->response_reader_allocate = stub_->PrepareAsyncDoAllocateAPI(&call->context, request, &cq_);
	call->response_reader_allocate->StartCall();
	call->response_reader_allocate->Finish(&call->resp, &call->status, (void*)call);
}

void CloudeinSessionClient::DoPrepare(const SessionInfo session_info) {

	PrepareRequest request;
	request.set_region(std::to_string(session_info.region));
	request.set_tx_name("tx-DoPrepare");

	AsyncClientCall_DoPrepare* call = new AsyncClientCall_DoPrepare;
	call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
	call->context.set_deadline(call->deadline_);

	tag_push(CLOUDEIN_GRPC_TAG::DoPrepareAPI, (void*)call);

	call->response_reader_prepare = stub_->PrepareAsyncDoPrepareAPI(&call->context, request, &cq_);
	call->response_reader_prepare->StartCall();
	call->response_reader_prepare->Finish(&call->resp, &call->status, (void*)call);
}

void CloudeinSessionClient::DoConnect(const SessionInfo session_info) {

	ConnectRequest request;
	request.set_region(std::to_string(session_info.region));
	request.set_tx_name("tx-DoConnect");

	AsyncClientCall_DoConnect* call = new AsyncClientCall_DoConnect;
	call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
	call->context.set_deadline(call->deadline_);

	tag_push(CLOUDEIN_GRPC_TAG::DoConnectAPI, (void*)call);

	call->response_reader_connect = stub_->PrepareAsyncDoConnectAPI(&call->context, request, &cq_);
	call->response_reader_connect->StartCall();
	call->response_reader_connect->Finish(&call->resp, &call->status, (void*)call);
}

void CloudeinSessionClient::DoRelease(const SessionInfo session_info) {

	ReleaseRequest request;
	request.set_region(std::to_string(session_info.region));
	request.set_tx_name("tx-DoRelease");

	AsyncClientCall_DoRelease* call = new AsyncClientCall_DoRelease;
	call->deadline_ = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
	call->context.set_deadline(call->deadline_);

	tag_push(CLOUDEIN_GRPC_TAG::DoReleaseAPI, (void*)call);

	call->response_reader_release = stub_->PrepareAsyncDoReleaseAPI(&call->context, request, &cq_);
	call->response_reader_release->StartCall();
	call->response_reader_release->Finish(&call->resp, &call->status, (void*)call);
}

void CloudeinSessionClient::StartAsyncCompleteRpc() {

	std::cout << "--- [front] Start Async ComplteRpc" << std::endl;

	void* got_tag;
	bool ok = false;

	while (cq_.Next(&got_tag, &ok))
	{

		auto grpc_tag = tag_pop(got_tag);

		switch (grpc_tag)
		{
		case DoAllocateAPI:
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
		break;
		case DoPrepareAPI:
		{
			AsyncClientCall_DoPrepare* call = static_cast<AsyncClientCall_DoPrepare*>(got_tag);

			//GPR_ASSERT(ok);

			if (call->status.ok())
				std::cout << "DoPrepareAPI-resp OK region: " << call->resp.region() << std::endl;
			else
				std::cout << "RPC failed" << std::endl;

			delete call;
		}
		break;
		case DoConnectAPI:
		{
			AsyncClientCall_DoConnect* call = static_cast<AsyncClientCall_DoConnect*>(got_tag);

			//GPR_ASSERT(ok);

			if (call->status.ok())
				std::cout << "DoPrepareAPI-resp OK region: " << call->resp.region() << std::endl;
			else
				std::cout << "RPC failed" << std::endl;

			delete call;
		}
		break;
		case DoReleaseAPI:
		{
			AsyncClientCall_DoRelease* call = static_cast<AsyncClientCall_DoRelease*>(got_tag);

			//GPR_ASSERT(ok);

			if (call->status.ok())
				std::cout << "DoPrepareAPI-resp OK region: " << call->resp.region() << std::endl;
			else
				std::cout << "RPC failed" << std::endl;

			delete call;
		}
		break;
		default:
			break;
		}

	}

	// Delete Client Call Structs
	std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>>::iterator it;
	for (it = list_tag_idx_.begin(); it != list_tag_idx_.end(); it++) {
		delete it->second;
	}
	list_tag_idx_.clear();

	std::cout << "--- [end] Start Async ComplteRpc" << std::endl;

}

void CloudeinSessionClient::StopAsyncCompleteRpc()
{
	std::cout << "--- [front] Stop Async ComplteRpc" << std::endl;

	cq_.Shutdown();

	std::cout << "--- [end] Stop Async ComplteRpc" << std::endl;

}

void CloudeinSessionClient::tag_push(CLOUDEIN_GRPC_TAG tag_idx, void* objAddr)
{
	std::lock_guard<std::mutex> lock_guard(mutex_list_tag_idx_);

	auto tag_idx_pair = std::make_pair(tag_idx, objAddr);
	list_tag_idx_.push_back(tag_idx_pair);
}

CLOUDEIN_GRPC_TAG CloudeinSessionClient::tag_pop(void* objAddr)
{
	std::lock_guard<std::mutex> lock_guard(mutex_list_tag_idx_);

	std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>>::iterator it =
		std::find_if(list_tag_idx_.begin(),
			list_tag_idx_.end(),
			[objAddr](std::pair<CLOUDEIN_GRPC_TAG, void*> pair) -> CLOUDEIN_GRPC_TAG {
				if (pair.second == objAddr)
					return pair.first;
			});

	auto return_idx = it->first;
	std::cout << "tag : " << return_idx << std::endl;

	list_tag_idx_.erase(it);

	return return_idx;
}