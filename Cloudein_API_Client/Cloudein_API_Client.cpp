// Cloudein_API_Client.cpp: 대상의 소스 파일입니다.
//

#include "Cloudein_API_Client.h"

#include <iostream>
#include <memory>

int main()
{

	auto context(std::make_unique<CloudeinSessionContext>(SessionInfo(SEOUL_DRAFT, "WSL")));
	auto state_allocate(std::make_shared<StateAllocate>());
	auto state_prepare(std::make_shared<StatePrepare>());
	auto state_connect(std::make_shared<StateConnect>());
	auto state_release(std::make_shared<StateRelease>());

	context->setState(state_allocate);
	context->request();
	std::cout << context->response() << std::endl;

	context->setState(state_prepare);
	context->request();
	std::cout << context->response() << std::endl;

	context->setState(state_connect);
	context->request();
	std::cout << context->response() << std::endl;
	
	context->setState(state_release);
	context->request();
	std::cout << context->response() << std::endl;


	return 0;
}
