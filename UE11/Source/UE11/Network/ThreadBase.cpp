// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadBase.h"

CThreadBase::CThreadBase()	:
	mLoop(true)
{
}

CThreadBase::~CThreadBase()
{
}

bool CThreadBase::Init()
{
	return true;
}

uint32 CThreadBase::Run()
{
	return uint32();
}

void CThreadBase::Stop()
{
}

void CThreadBase::Exit()
{
}
