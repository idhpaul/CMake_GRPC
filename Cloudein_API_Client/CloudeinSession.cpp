#include "CloudeinSession.h"

#include <iostream>

SessionInfo CloudeinState::info_;


CloudeinSessionContext::CloudeinSessionContext()
{
    state_->info_ = SessionInfo();
}

CloudeinSessionContext::CloudeinSessionContext(SessionInfo info)
{
    state_->info_ = info;
}

CloudeinSessionContext::~CloudeinSessionContext()
{
    if (state_)
    {
        state_.reset();
    }
}

void CloudeinSessionContext::setGRPC(const std::string ip, const std::string port)
{
    std::cout << "[front]Call SetGPC" << std::endl;
    std::string connectinfo = ip + ":" + port;

    grpcClient_ = std::make_shared<CloudeinClient>(grpc::CreateChannel(connectinfo, grpc::InsecureChannelCredentials()));

    thread_grpc_resp_cq = std::thread(&CloudeinClient::StartAsyncCompleteRpc, grpcClient_.get());
    std::cout << "[end]Call SetGPC" << std::endl;

}

void CloudeinSessionContext::unsetGRPC()
{
    std::cout << "[front]Call unsetGRPC" << std::endl;

    grpcClient_->StopAsyncCompleteRpc();

    if (thread_grpc_resp_cq.joinable())
    {
        thread_grpc_resp_cq.join();
    }

    std::cout << "[end]Call unsetGRPC" << std::endl;

}

void CloudeinSessionContext::setState(std::shared_ptr<CloudeinState> state)
{
    if (state_)
    {
        state_.reset();
    }
    state_ = state;
}

void CloudeinSessionContext::request()
{
    state_->handle(this);
}

std::string CloudeinSessionContext::response()
{
    return state_->return_handle();
}



/*
 *     Allocate API Request/Response message struct
 */
void StateAllocate::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoAllocate(this->info_);
}

std::string StateAllocate::return_handle()
{
    return "resp-Allocate";
}

/*
 *     Prepare API Request/Response message struct
 */

void StatePrepare::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoPrepare(this->info_);
}

std::string StatePrepare::return_handle()
{
    return "resp-Prepare";
}

/*
 *     Connect API Request/Response message struct
 */

void StateConnect::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoConnect(this->info_);
}

std::string StateConnect::return_handle()
{
    return "resp-Connect";
}

/*
 *     Release API Request/Response message struct
 */

void StateRelease::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoRelease(this->info_);
}

std::string StateRelease::return_handle()
{
    return "resp-Release";
}


void CloudeinClient::tag_push(CLOUDEIN_GRPC_TAG tag_idx, void* objAddr)
{
    std::lock_guard<std::mutex> lock_guard(mutex_list_tag_idx_);

    auto tag_idx_pair = std::make_pair(tag_idx, objAddr);
    list_tag_idx_.push_back(tag_idx_pair);
}

CLOUDEIN_GRPC_TAG CloudeinClient::tag_pop(void* objAddr)
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
