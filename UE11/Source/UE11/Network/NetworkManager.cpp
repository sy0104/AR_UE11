// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkManager.h"

CNetworkManager* CNetworkManager::mInst = nullptr;

CNetworkManager::CNetworkManager()
{
}

CNetworkManager::~CNetworkManager()
{
}

bool CNetworkManager::Init()
{
	return true;
}
