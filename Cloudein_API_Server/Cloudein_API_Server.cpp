/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "CloudeinAPI.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
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

class ServerImpl final {
public:
    ~ServerImpl() {
        server_->Shutdown();
        
        cq_->Shutdown();
    }

    void Run() {
        std::string server_address("0.0.0.0:50051");

        ServerBuilder builder;
        
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);
        cq_ = builder.AddCompletionQueue();


        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        HandleRpcs();
    }

private:
    // Class encompasing the state and logic needed to serve a request.
    class CallData {
    public:
        // Take in the "service" instance (in this case representing an asynchronous
        // server) and the completion queue "cq" used for asynchronous communication
        // with the gRPC runtime.
        CallData(CloudeinSession::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                // Make this instance progress to the PROCESS state.
                status_ = PROCESS;

                // As part of the initial CREATE state, we *request* that the system
                // start processing SayHello requests. In this request, "this" acts are
                // the tag uniquely identifying the request (so that different CallData
                // instances can serve different requests concurrently), in this case
                // the memory address of this CallData instance.
                service_->RequestDoAllocateAPI(&ctx_, &request_, &responder_, cq_, cq_,
                    this);
            }
            else if (status_ == PROCESS) {
                // Spawn a new CallData instance to serve new clients while we process
                // the one for this CallData. The instance will deallocate itself as
                // part of its FINISH state.
                new CallData(service_, cq_);

                std::cout << "Get Allocate request message  region : " << request_.region() << std::endl;
                std::cout << "Get Allocate request message tx_name : " << request_.tx_name() << std::endl;

                // The actual processing.
                reply_.set_region("WSL");
                reply_.set_allocate_port1(10001);
                reply_.set_allocate_port2(10002);
                reply_.set_allocate_port3(10003);
                reply_.set_allocate_port4(10004);
                reply_.set_allocate_port5(10005);

                // And we are done! Let the gRPC runtime know we've finished, using the
                // memory address of this instance as the uniquely identifying tag for
                // the event.
                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this);
            }
            else {
                GPR_ASSERT(status_ == FINISH);
                // Once in the FINISH state, deallocate ourselves (CallData).
                delete this;
            }
        }

    private:
        // The means of communication with the gRPC runtime for an asynchronous
        // server.
        CloudeinSession::AsyncService* service_;
        // The producer-consumer queue where for asynchronous server notifications.
        ServerCompletionQueue* cq_;
        // Context for the rpc, allowing to tweak aspects of it such as the use
        // of compression, authentication, as well as to send metadata back to the
        // client.
        ServerContext ctx_;

        // What we get from the client.
        AllocateRequest request_;
        // What we send back to the client.
        AllocateResponse reply_;

        // The means to get back to the client.
        ServerAsyncResponseWriter<AllocateResponse> responder_;

        // Let's implement a tiny state machine with the following states.
        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;  // The current serving state.
    };

    void HandleRpcs() {
        // Spawn a new CallData instance to serve new clients.
        new CallData(&service_, cq_.get());

        void* tag;
        bool ok;

        while (true) {

            GPR_ASSERT(cq_->Next(&tag, &ok));

            GPR_ASSERT(ok);

            static_cast<CallData*>(tag)->Proceed();
        }
    }

    std::unique_ptr<ServerCompletionQueue> cq_;
    CloudeinSession::AsyncService service_;
    std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
    ServerImpl server;
    server.Run();

    return 0;
}