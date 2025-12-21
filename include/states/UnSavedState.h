#pragma once
#include "DocumentState.h"

class UnSavedState : public DocumentState {
public:
    virtual bool isSaved() const override {
        return false;
    }
};