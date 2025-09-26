#include "stdafx.h"
#include "CharMovePacket.h"
#include "GMMemory.h"

const float CNetworkPos::ms_fToAngle        = 180.0f / 3.141592f;
const float CNetworkPos::ms_fToRadian       = 3.141592f / 180.0f;
const float CNetworkPos::ms_fPositionDetail = 10.0f;
const float CNetworkPos::ms_fVelocityDetail = 1000.0f;