#pragma once

class Document;

class DocumentState {
public:
    virtual ~DocumentState() = default;

    virtual bool isSaved() const = 0;
};