//
// Created by Giacomo Groppi on 28/04/23.
//

#include "WObject.h"
#include "utils/WCommonScript.h"

WObject::WObject(WObject *parent)
    : _parent(parent)
{
    if (parent) {
        parent->_children.append(this);
    }
}

WObject::~WObject()
{
    WDebug(true, "Call with children size: " << _children.size() << " and parent = " << _parent);
    for (const auto *children: this->_children) {
        delete children;
    }

    if (_parent) {
        _parent->_children.remove(this);
    }
}
