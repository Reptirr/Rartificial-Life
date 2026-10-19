#pragma once
#include <Storage.h>

class UIComponent {
protected:
    Storage &storage_;

    explicit UIComponent(Storage &storage) : storage_(storage) {}

public:
    virtual void UIUpdate() {}

    virtual ~UIComponent() = default;
};
