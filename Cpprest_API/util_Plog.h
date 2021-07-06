/*****************************************************************//**
 * \file   util_Plog.h
 * \brief  PLOG 유틸(단일/다중 파일 생성)
 *
 * \author idhpaul
 * \date   February 2021
 *********************************************************************/

#pragma once
#include <plog/Log.h>

 /**
  * 다중 파일 생성.
  * 기존에 있던 로그에 이어서 로깅
  */
void MakePlogFile_Multi(void);

/**
 * 단일 파일 생성.
 * 요청할때 마다 새로운 로그 생성
 */
void MakePlogFile_Single(void);