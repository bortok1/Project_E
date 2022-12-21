// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/ECameraShake.h"

UECameraShake::UECameraShake()
{
	OscillationDuration = 1.0f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;

	LocOscillation.X.Amplitude = FMath::RandRange(5.0f, 20.0f);
	LocOscillation.X.Frequency = 30;
	LocOscillation.Z.Amplitude = FMath::RandRange(5.0f, 20.0f);
	LocOscillation.Z.Frequency = 30;

}
