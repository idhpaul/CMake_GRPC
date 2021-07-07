/*****************************************************************//**
 * \file   Cloudein_Session_gRPC_Server_Operation.h
 * \brief  Cloudein Session gRPC Server Operation
 *
 * \author idh
 * \date   July 2021
 *********************************************************************/
#ifndef CLOUDEIN_SESSION_GRPC_SERVER_OPERATION_H
#define CLOUDEIN_SESSION_GRPC_SERVER_OPERATION_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "CloudeinAPI.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using cloudeinapi::CloudeinSessionOperation;
using cloudeinapi::OperationRequest;
using cloudeinapi::OperationResponse;


class CloudeinSessionOperationImpl final : public CloudeinSessionOperation::Service 
{
	Status DoOperation(ServerContext* context, const OperationRequest* request, OperationResponse* reply) override;
};

class CloudeinSessionOperationServer
{
public:
	CloudeinSessionOperationServer(std::string ip, std::string port);

	void StartSyncSingleRPC();
	void StopSyncSingleRPC();
private:
	std::string server_address;
	std::unique_ptr<Server> server;
};

#endif
