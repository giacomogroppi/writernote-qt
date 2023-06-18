#pragma once

class Serializable {
public:
    Serializable() = default;
    virtual ~Serializable() = default;

    virtual int write(void *data) const;
};
