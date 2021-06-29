// Cloudein_API_Client.cpp: 대상의 소스 파일입니다.
//

#include "Cloudein_Session.h"

#include <iostream>
#include <memory>

int main()
{

	auto context(std::make_unique<CloudeinSessionContext>(SessionInfo(SEOUL_DRAFT, "WSL")));
	auto state_allocate(std::make_shared<StateAllocate>());
	auto state_prepare(std::make_shared<StatePrepare>());
	auto state_connect(std::make_shared<StateConnect>());
	auto state_release(std::make_shared<StateRelease>());

	context->setGRPC("localhost", "50051");

	bool run = true;
	int num;
	while (run)
	{
		std::cout << "Enter 0(gRPC Shutdown) / 1(call Allocate) / 2(call prepare) / 3(call connect) / 4(call release) => ";
		std::cin >> num;

		switch (num)
		{
		case 0:
			std::cout << "Cloudein Client is Shutting down" << std::endl;
			run = false;
			break;
		case 1:
			context->setState(state_allocate);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 2:
			context->setState(state_prepare);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 3:
			context->setState(state_connect);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 4:
			context->setState(state_release);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		default:
			std::cout << "Only allow 0" << std::endl;
			break;
		}
	}
	
	context->unsetGRPC();


	return 0;
}
