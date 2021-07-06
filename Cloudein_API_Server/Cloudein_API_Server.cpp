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
#include <functional>
#include <memory>
#include <mutex>
#include <list>
#include <string>
#include <thread>
#include <utility>

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

enum CLOUDEIN_GRPC_TAG
{
    DoAllocate_API = 1,
    DoPrepare_API = 2,
    DoConnect_API = 3,
    DoRelease_API = 4
};

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

    class AllocateData {
    public:
        AllocateData(CloudeinSession::AsyncService* service, ServerCompletionQueue* cq, std::function <void(CLOUDEIN_GRPC_TAG, void*)> tag_push, std::function <void(void*)> tag_erase)
            : service_(service), cq_(cq), list_push_func(tag_push), list_erase_func(tag_erase),responder_(&ctx_), status_(CREATE) {
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                std::cout << "##  Allocate-Create call" << std::endl;
                status_ = PROCESS;

                service_->RequestDoAllocateAPI(&ctx_, &request_, &responder_, cq_, cq_, this);
            }
            else if (status_ == PROCESS) {
                std::cout << "##  Allocate-PROCESS call" << std::endl;

                auto register_allocate_data = new AllocateData(service_, cq_, list_push_func, list_erase_func);
                list_push_func(CLOUDEIN_GRPC_TAG::DoAllocate_API, (void*)register_allocate_data);

                auto ip = [](std::string uri) {
                    std::string::size_type first_idx, second_idx;

                    first_idx = uri.find_first_of(":");
                    second_idx = uri.find_last_of(":");

                    return uri.substr(first_idx + 1, second_idx - first_idx - 1);
                };
                std::cout << "## Client ip : " << ip(ctx_.peer()) << std::endl;
               
                std::cout << "##  Client peer uri : " << ctx_.peer() << std::endl;
                std::cout << "##  Get Allocate request message  region : " << request_.region() << std::endl;
                std::cout << "##  Get Allocate request message tx_name : " << request_.tx_name() << std::endl;

                // The actual processing.
                reply_.set_region("WSL-Allocate-resp");
                reply_.set_allocate_port1(50001);
                reply_.set_allocate_port2(50002);
                reply_.set_allocate_port3(50003);
                reply_.set_allocate_port4(50004);
                reply_.set_allocate_port5(50005);

                
                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this);
            }
            else {
                std::cout << "##  Allocate-Finish call" << std::endl;
                GPR_ASSERT(status_ == FINISH);

                list_erase_func(this);

                delete this;
            }
        }

    private:
        
        CloudeinSession::AsyncService* service_;
        ServerCompletionQueue* cq_;
      
        ServerContext ctx_;

        AllocateRequest request_;
        AllocateResponse reply_;
        ServerAsyncResponseWriter<AllocateResponse> responder_;

        std::function <void(CLOUDEIN_GRPC_TAG, void*)> list_push_func;
        std::function <void(void*)> list_erase_func;

        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;
    };
    class PrepareData {
    public:
        // Take in the "service" instance (in this case representing an asynchronous
        // server) and the completion queue "cq" used for asynchronous communication
        // with the gRPC runtime.
        PrepareData(CloudeinSession::AsyncService* service, ServerCompletionQueue* cq, std::function <void(CLOUDEIN_GRPC_TAG, void*)> tag_push, std::function <void(void*)> tag_erase)
            : service_(service), cq_(cq), list_push_func(tag_push), list_erase_func(tag_erase), responder_(&ctx_), status_(CREATE) {
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                std::cout << "##  Prepare-Create call" << std::endl;

                status_ = PROCESS;

                service_->RequestDoPrepareAPI(&ctx_, &request_, &responder_, cq_, cq_, this);
            }
            else if (status_ == PROCESS) {
                std::cout << "##  Prepare-process call" << std::endl;

                auto register_prepare_data = new PrepareData(service_, cq_, list_push_func, list_erase_func);
                list_push_func(CLOUDEIN_GRPC_TAG::DoPrepare_API, (void*)register_prepare_data);

                auto ip = [](std::string uri) {
                    std::string::size_type first_idx, second_idx;

                    first_idx = uri.find_first_of(":");
                    second_idx = uri.find_last_of(":");

                    return uri.substr(first_idx + 1, second_idx - first_idx - 1);
                };
                std::cout << "## Client ip : " << ip(ctx_.peer()) << std::endl;

                std::cout << "##  Get Prepare request message  region : " << request_.region() << std::endl;
                std::cout << "##  Get Prepare request message tx_name : " << request_.tx_name() << std::endl;

                // The actual processing.
                reply_.set_region("WSL-Prepare-resp");


                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this);
            }
            else {
                std::cout << "##  Prepare-Finish call" << std::endl;

                GPR_ASSERT(status_ == FINISH);

                list_erase_func(this);

                delete this;
            }
        }

    private:

        CloudeinSession::AsyncService* service_;
        ServerCompletionQueue* cq_;

        ServerContext ctx_;

        PrepareRequest request_;
        PrepareResponse reply_;
        ServerAsyncResponseWriter<PrepareResponse> responder_;

        std::function <void(CLOUDEIN_GRPC_TAG, void*)> list_push_func;
        std::function <void(void*)> list_erase_func;

        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;
    };
    class ConnectData {
    public:

        ConnectData(CloudeinSession::AsyncService* service, ServerCompletionQueue* cq, std::function <void(CLOUDEIN_GRPC_TAG, void*)> tag_push, std::function <void(void*)> tag_erase)
            : service_(service), cq_(cq), list_push_func(tag_push), list_erase_func(tag_erase), responder_(&ctx_), status_(CREATE) {
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                std::cout << "##  Connect-Create call" << std::endl;

                status_ = PROCESS;

                service_->RequestDoConnectAPI(&ctx_, &request_, &responder_, cq_, cq_, this);
            }
            else if (status_ == PROCESS) {
                std::cout << "##  Connect-Process call" << std::endl;

                auto register_connect_data = new ConnectData(service_, cq_, list_push_func, list_erase_func);
                list_push_func(CLOUDEIN_GRPC_TAG::DoConnect_API, (void*)register_connect_data);

                auto ip = [](std::string uri) {
                    std::string::size_type first_idx, second_idx;

                    first_idx = uri.find_first_of(":");
                    second_idx = uri.find_last_of(":");

                    return uri.substr(first_idx + 1, second_idx - first_idx - 1);
                };
                std::cout << "## Client ip : " << ip(ctx_.peer()) << std::endl;

                std::cout << "Get Connect request message  region : " << request_.region() << std::endl;
                std::cout << "Get Connect request message tx_name : " << request_.tx_name() << std::endl;

                // The actual processing.
                reply_.set_region("WSL-Connect-resp");

                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this);
            }
            else {
                std::cout << "##  Connect-Finish call" << std::endl;

                GPR_ASSERT(status_ == FINISH);

                list_erase_func(this);

                delete this;
            }
        }

    private:

        CloudeinSession::AsyncService* service_;
        ServerCompletionQueue* cq_;

        ServerContext ctx_;

        ConnectRequest request_;
        ConnectResponse reply_;
        ServerAsyncResponseWriter<ConnectResponse> responder_;

        std::function <void(CLOUDEIN_GRPC_TAG, void*)> list_push_func;
        std::function <void(void*)> list_erase_func;

        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;
    };
    class ReleaseData {
    public:
        // Take in the "service" instance (in this case representing an asynchronous
        // server) and the completion queue "cq" used for asynchronous communication
        // with the gRPC runtime.
        ReleaseData(CloudeinSession::AsyncService* service, ServerCompletionQueue* cq, std::function <void(CLOUDEIN_GRPC_TAG, void*)> tag_push, std::function <void(void*)> tag_erase)
            : service_(service), cq_(cq), list_push_func(tag_push), list_erase_func(tag_erase), responder_(&ctx_), status_(CREATE) {
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                std::cout << "##  Release-Create call" << std::endl;

                status_ = PROCESS;

                service_->RequestDoReleaseAPI(&ctx_, &request_, &responder_, cq_, cq_, this);
            }
            else if (status_ == PROCESS) {
                std::cout << "##  Release-Process call" << std::endl;

                auto register_release_data = new ReleaseData(service_, cq_, list_push_func, list_erase_func);
                list_push_func(CLOUDEIN_GRPC_TAG::DoRelease_API, (void*)register_release_data);

                auto ip = [](std::string uri) {
                    std::string::size_type first_idx, second_idx;

                    first_idx = uri.find_first_of(":");
                    second_idx = uri.find_last_of(":");

                    return uri.substr(first_idx + 1, second_idx - first_idx - 1);
                };
                std::cout << "## Client ip : " << ip(ctx_.peer()) << std::endl;

                std::cout << "##  Get Release request message  region : " << request_.region() << std::endl;
                std::cout << "##  Get Release request message tx_name : " << request_.tx_name() << std::endl;

                // The actual processing.
                reply_.set_region("WSL-Release-resp");

                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this);
            }
            else {
                std::cout << "##  Release-Finish call" << std::endl;

                GPR_ASSERT(status_ == FINISH);

                list_erase_func(this);

                delete this;
            }
        }

    private:

        CloudeinSession::AsyncService* service_;
        ServerCompletionQueue* cq_;

        ServerContext ctx_;

        ReleaseRequest request_;
        ReleaseResponse reply_;
        ServerAsyncResponseWriter<ReleaseResponse> responder_;

        std::function <void(CLOUDEIN_GRPC_TAG, void*)> list_push_func;
        std::function <void(void*)> list_erase_func;

        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;
    };

    void HandleRpcs() {



        auto register_allocate_data = new AllocateData(&service_, cq_.get(), 
            [=](CLOUDEIN_GRPC_TAG tag_idx, void* objAddr) {this->tag_push(tag_idx, objAddr);},
            [=](void* objAddr) {this->tag_erase(objAddr); });
        tag_push(CLOUDEIN_GRPC_TAG::DoAllocate_API, (void*)register_allocate_data);

        auto register_prepare_data = new PrepareData(&service_, cq_.get(), 
            [=](CLOUDEIN_GRPC_TAG tag_idx, void* objAddr) {this->tag_push(tag_idx, objAddr); },
            [=](void* objAddr) {this->tag_erase(objAddr); });
        tag_push(CLOUDEIN_GRPC_TAG::DoPrepare_API, (void*)register_prepare_data);

        auto register_connect_data = new ConnectData(&service_, cq_.get(), 
            [=](CLOUDEIN_GRPC_TAG tag_idx, void* objAddr) {this->tag_push(tag_idx, objAddr); },
            [=](void* objAddr) {this->tag_erase(objAddr); });
        tag_push(CLOUDEIN_GRPC_TAG::DoConnect_API, (void*)register_connect_data);

        auto register_release_data = new ReleaseData(&service_, cq_.get(), 
            [=](CLOUDEIN_GRPC_TAG tag_idx, void* objAddr) {this->tag_push(tag_idx, objAddr); },
            [=](void* objAddr) {this->tag_erase(objAddr); });
        tag_push(CLOUDEIN_GRPC_TAG::DoRelease_API, (void*)register_release_data);


        void* tag;
        bool ok;

        while (cq_->Next(&tag, &ok)) {

            GPR_ASSERT(ok);

            auto grpc_tag = tag_pop(tag);

            switch (grpc_tag)
            {
            case DoAllocate_API:
            {
                static_cast<AllocateData*>(tag)->Proceed();
            }
            break;
            case DoPrepare_API:
            {
                static_cast<PrepareData*>(tag)->Proceed();
            }
            break;
            case DoConnect_API:
            {
                static_cast<ConnectData*>(tag)->Proceed();
            }
            break;
            case DoRelease_API:
            {
                static_cast<ReleaseData*>(tag)->Proceed();
            }
            break;
            default:
                break;
            }

        }
    }

    void tag_push(CLOUDEIN_GRPC_TAG tag_idx, void* objAddr) {
        std::lock_guard<std::mutex> lock_guard(mutex_list_tag_idx_);

        std::cout << "tag-push : " << tag_idx << std::endl;
        auto tag_idx_pair = std::make_pair(tag_idx, objAddr);
        list_tag_idx_.push_back(tag_idx_pair);
    };
    CLOUDEIN_GRPC_TAG tag_pop(void* objAddr) {
        std::lock_guard<std::mutex> lock_guard(mutex_list_tag_idx_);

        std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>>::iterator it =
            std::find_if(list_tag_idx_.begin(),
                list_tag_idx_.end(),
                [objAddr](std::pair<CLOUDEIN_GRPC_TAG, void*>& pair) {
                    return pair.second == objAddr;
                });

        auto return_idx = it->first;
        std::cout << "tag-pop : " << return_idx << std::endl;

        return return_idx;
    };

    void tag_erase(void* objAddr)
    {
        std::lock_guard<std::mutex> lock_guard(mutex_list_tag_idx_);

        std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>>::iterator it =
            std::find_if(list_tag_idx_.begin(),
                list_tag_idx_.end(),
                [objAddr](std::pair<CLOUDEIN_GRPC_TAG, void*>& pair) {
                    return pair.second == objAddr;
                });

        auto return_idx = it->first;
        list_tag_idx_.erase(it);
    };

    std::unique_ptr<ServerCompletionQueue> cq_;
    CloudeinSession::AsyncService service_;
    std::unique_ptr<Server> server_;

    std::mutex mutex_list_tag_idx_;
    std::list<std::pair<CLOUDEIN_GRPC_TAG, void*>> list_tag_idx_;
};

int main(int argc, char** argv) {
    ServerImpl server;
    server.Run();

    return 0;
}