//
// Created by Giacomo Groppi on 28/04/23.
//

#include "WObject.h"
#include "utils/WCommonScript.h"

WObject::WObject(WObject *parent)
    : _parent(parent)
{
    if (parent) {
        parent->_children.push_back(this);
    }
}

WObject::~WObject()
{
    WDebug(true, "Call with children size: " << _children.size() << " and parent = " << _parent);

    while (_children.size()) {
        delete _children.at(0);
    }

    if (_parent) {
        auto position = std::find(_parent->_children.begin(), _parent->_children.end(), this);

        W_ASSERT(position != _parent->_children.end());

        _parent->_children.erase(position);
    }
}
