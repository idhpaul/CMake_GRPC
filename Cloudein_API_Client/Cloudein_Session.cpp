#include "Cloudein_Session.h"

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

    grpcClient_ = std::make_shared<CloudeinSessionClient>(grpc::CreateChannel(connectinfo, grpc::InsecureChannelCredentials()));

    thread_grpc_resp_cq = std::thread(&CloudeinSessionClient::StartAsyncCompleteRpc, grpcClient_.get());
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

void StateDisconnect::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoDisconnect(this->info_);
}

std::string StateDisconnect::return_handle()
{
    return "resp-Disconnect";
}

void StateTimeout::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoTimeout(this->info_);
}

std::string StateTimeout::return_handle()
{
    return "resp-Timeout";
}

void StatePrepareFail::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoPrepareFail(this->info_);
}

std::string StatePrepareFail::return_handle()
{
    return "resp-ReadyFail";
}

void StateConnectFail::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoConnectFail(this->info_);
}

std::string StateConnectFail::return_handle()
{
    return "resp-ConnectFail";
}

void StateRebootFail::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoRebootFail(this->info_);
}

std::string StateRebootFail::return_handle()
{
    return "resp-RebootFail";
}

void StateAuthFail::handle(CloudeinSessionContext* session_context)
{
    session_context->grpcClient_->DoAuthFail(this->info_);
}

std::string StateAuthFail::return_handle()
{
    return "resp-AuthFail";
}
