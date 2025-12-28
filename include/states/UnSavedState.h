#pragma once
#include "DocumentState.h"

class UnSavedState : public DocumentState {
public:
    bool isSaved() const override {
        return false;
    }
};