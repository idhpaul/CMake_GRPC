#include "CloudeinSession.h"

#include <iostream>

SessionInfo CloudeinState::info_;

CloudeinSessionContext::CloudeinSessionContext()
{
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
    state_->handle();
}

std::string CloudeinSessionContext::response()
{
    return state_->return_handle();
}

/*
 *     Allocate API Request/Response message struct
 */
void StateAllocate::handle()
{
    this->info_.allocate_port1 = 1001;
}

std::string StateAllocate::return_handle()
{
    std::cout << "info allocate " << this->info_.allocate_port1 << std::endl;
    return "StateAllocate";
}

/*
 *     Prepare API Request/Response message struct
 */

void StatePrepare::handle()
{
    this->info_.allocate_port2 = 2002;
}

std::string StatePrepare::return_handle()
{
    std::cout << "info allocate " << this->info_.allocate_port2 << std::endl;
    return "StateAllocate";
}

/*
 *     Connect API Request/Response message struct
 */

void StateConnect::handle()
{
    this->info_.allocate_port3 = 3003;
}

std::string StateConnect::return_handle()
{
    std::cout << "info allocate " << this->info_.allocate_port3 << std::endl;
    return "StateAllocate";
}

/*
 *     Release API Request/Response message struct
 */

void StateRelease::handle()
{
    this->info_.allocate_port4 = 5004;
}

std::string StateRelease::return_handle()
{
    std::cout << "info allocate " << this->info_.allocate_port4 << std::endl;
    return "StateAllocate";
}
