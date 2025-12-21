#pragma once
#include "DocumentState.h"

class SavedState : public DocumentState {
public:
    virtual bool isSaved() const override {
        return true;
    }
};