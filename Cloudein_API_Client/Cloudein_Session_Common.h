/*****************************************************************//**
 * \file   CloudeinSessionCommon.h
 * \brief  Cloudein Session Common eum and struct
 * 
 * \author idh
 * \date   June 2021
 *********************************************************************/

#ifndef CLOUDEIN_SESSION_COMMON_H
#define CLOUDEIN_SESSION_COMMON_H

#include <stdint.h>

#include <string>

enum CloudeinRegion
{
	SEOUL_PRODUCT = 0,
	SEOUL_DEVELOPE = 1,
	SEOUL_DRAFT = 2
};

struct SessionInfo
{
	CloudeinRegion region;
	std::string tx_name;
	uint32_t allocate_port1;
	uint32_t allocate_port2;
	uint32_t allocate_port3;
	uint32_t allocate_port4;
	uint32_t allocate_port5;

	SessionInfo()
	{
		region = SEOUL_DEVELOPE;
		tx_name = "SEOUL_DEV_TX";
		allocate_port1 = 0;
		allocate_port2 = 0;
		allocate_port3 = 0;
		allocate_port4 = 0;
		allocate_port5 = 0;
	}
	SessionInfo(const CloudeinRegion reg, const std::string name)
	{
		region = reg;
		tx_name = name;
		allocate_port1 = 0;
		allocate_port2 = 0;
		allocate_port3 = 0;
		allocate_port4 = 0;
		allocate_port5 = 0;
	}
};

enum CLOUDEIN_GRPC_TAG
{
	DoAllocateAPI = 1,
	DoPrepareAPI = 2,
	DoConnectAPI = 3,
	DoReleaseAPI = 4
};
#endif

