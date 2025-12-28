#pragma once
#include "DocumentState.h"

class SavedState : public DocumentState {
public:
    bool isSaved() const override {
        return true;
    }
};