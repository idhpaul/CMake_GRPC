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
	auto state_disconnect(std::make_shared<StateDisconnect>());
	auto state_timeout(std::make_shared<StateTimeout>());
	auto state_preparefail(std::make_shared<StatePrepareFail>());
	auto state_connectfail(std::make_shared<StateConnectFail>());
	auto state_rebootfail(std::make_shared<StateRebootFail>());
	auto state_authfail(std::make_shared<StateAuthFail>());

	context->setGRPC("192.168.0.102", "50051");

	bool run = true;
	int num;
	while (run)
	{
		std::cout << "Enter 0(gRPC Shutdown) / 1(call Allocate) / 2(call prepare) / 3(call connect) / 4(call release) "
			<< "5(call Disconnect) / 6(call Timeout) / 7(call Prepare-Fail) / 8(call Connect-Fail) / 9(call Reboot-Fail) / 10(call Auth-Fail) =>> ";

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
		case 5:
			context->setState(state_disconnect);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 6:
			context->setState(state_timeout);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 7:
			context->setState(state_preparefail);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 8:
			context->setState(state_connectfail);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 9:
			context->setState(state_rebootfail);
			context->request();
			std::cout << context->response() << std::endl;
			break;
		case 10:
			context->setState(state_authfail);
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
