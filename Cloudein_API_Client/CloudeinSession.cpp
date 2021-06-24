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

    thread_grpc_resp_cq.join();
}

void CloudeinSessionContext::setGRPC(const std::string ip, const std::string port)
{

    std::string connectinfo = ip + ":" + port;

    grpcClient_ = std::make_shared<CloudeinClient>(grpc::CreateChannel(connectinfo, grpc::InsecureChannelCredentials()));

    thread_grpc_resp_cq = std::thread(&CloudeinClient::AsyncCompleteRpc, grpcClient_.get());
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
    std::cout << "info allocate resp" << this->info_.allocate_port1 << std::endl;
    return "StateAllocate";
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
    std::cout << "info State resp " << this->info_.allocate_port2 << std::endl;
    return "StatePrepare";
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
    std::cout << "info Connect resp " << this->info_.allocate_port3 << std::endl;
    return "StateConnect";
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
    std::cout << "info Release resp " << this->info_.allocate_port4 << std::endl;
    return "StateRelease";
}

std::string CloudeinState::return_handle()
{
    return std::string();
}

void CloudeinClient::Enqueue_tag_idx(CLOUDEIN_GRPC_TAG tag_idx, void* objAddr)
{
    std::lock_guard<std::mutex> lock_guard(queue_mutex_);

    auto tag_idx_pair = std::make_pair(tag_idx, objAddr);
    queue_tag_idx_.push(tag_idx_pair);
}
CLOUDEIN_GRPC_TAG CloudeinClient::Dequeue_tag_idx(void* objAddr)
{
    std::lock_guard<std::mutex> lock_guard(queue_mutex_);

    auto front_pair = queue_tag_idx_.front();
    queue_tag_idx_.pop();

    if (front_pair.second == objAddr)
        return front_pair.first;

}
