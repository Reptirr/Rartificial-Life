#pragma once
#include <UIStorage.h>

class UIComponent {
protected:
    UIStorage &storage_;

    explicit UIComponent(UIStorage &storage) : storage_(storage) {}

public:
    virtual void setData(Data *) = 0;
    virtual void UIUpdate() {}

    virtual ~UIComponent() = default;
};
