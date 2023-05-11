#pragma once

class Serializable {
public:
    virtual Serializable() = default;
    virtual ~Serializable() = default;

    virtual int write(void *data) const;
}
