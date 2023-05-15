
#pragma once

#include "CoreMinimal.h"

/**
 * 
 
class HOLODEMTECT_API FThreadDataset : public FRunnable
{

public:
    FThreadDataset(UDatasetGenerator* InGenerator) : Generator(InGenerator) {}
    ~FThreadDataset() {};

    virtual bool Init() override
    {
        // Initialize thread here
        return true;
    }
    virtual uint32 Run() override {

        while (!Stopping) {

            if (Generator->semaforo && Generator->timeseries.Num() > 0) {
                Generator->sendTimeseries();
            }
        }
        return 0;
    }

    virtual void Stop() override{
        Stopping = true;
    }

    virtual void Exit() override {

    }

    private:
        bool Stopping = false;
        UDatasetGenerator* Generator;
};
*/