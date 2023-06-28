// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "Ice.generated.h"

UCLASS()
class PROJECT_E_API AIce : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintImplementableEvent)
		void EIceResetEvent();

	virtual void Reset() override;
};
