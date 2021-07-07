#include "Cloudein_Session_gRPC_Server_Operation.h"

Status CloudeinSessionOperationImpl::DoOperation(ServerContext* context, const OperationRequest* request, OperationResponse* reply)
{
	std::cout << "DoOperation region : " << request->region() << std::endl;
	std::cout << "DoOperation tx_ip : " << request->tx_ip() << std::endl;
	std::cout << "DoOperation operation_type : " << request->operation_type() << std::endl;

	std::cout << "PC OS 종료 명령 수행 중.............." << std::endl;
	std::cout << ".\n.\n.\n.\n.\n.\n.\n";
	std::cout << "PC OS 종료 명령 수행 완료............" << std::endl;


	reply->set_region(context->peer());
	return Status::OK;
}

CloudeinSessionOperationServer::CloudeinSessionOperationServer(std::string ip, std::string port)
{
	std::string connectinfo = ip + ":" + port;
	server_address = connectinfo;
}

void CloudeinSessionOperationServer::StartSyncSingleRPC()
{
	std::cout << "--- [front] Start Sync Single Rpc" << std::endl;

	CloudeinSessionOperationImpl service;

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	server = std::move(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	server->Wait();

	std::cout << "--- [end] Start Sync Single Rpc" << std::endl;
}

void CloudeinSessionOperationServer::StopSyncSingleRPC()
{
	std::cout << "--- [front] Stop Sync Single Rpc" << std::endl;

	server->Shutdown();

	std::cout << "--- [end] Stop Sync Single Rpc" << std::endl;
}

