#ifndef WRITERNOTE_WOBJECT_H
#define WRITERNOTE_WOBJECT_H

#include "core/WVector.h"
#include <functional>
#include <iostream>

#define W_EMITTABLE_PRIVATE_LIST(name_signals, ...) \
    private:                                   \
    WVector<std::function<void( __VA_ARGS__)>> w_object_observer_##name_signals; \

#define W_EMITTABLE_PRIVATE_REGI(name_signals, ...) \
    private: \
    void reg##name_signals(std::function<void(__VA_ARGS__)> nameFunc) {                    \
        this->w_object_observer_##name_signals.append(nameFunc);                                \
    }

#define W_EMITTABLE_PRIVATE_FUNC_0(name_signals) \
    /* funzione chiamata internamente quando il segnale viene lanciato */        \
    void name_signals() {                                             \
        for (const auto &object: w_object_observer_##name_signals) {             \
            object();                                                  \
        }                                                                        \
    };

#define W_EMITTABLE_PRIVATE_FUNC_1(name_signals, type, name) \
    /* funzione chiamata internamente quando il segnale viene lanciato */        \
    void name_signals(type name) {                                             \
        for (const auto &object: w_object_observer_##name_signals) {             \
            object(name);                                                  \
        }                                                                        \
    }

#define W_EMITTABLE_PRIVATE_FUNC_2(name_signals, type1, name1, type2, name2) \
    void name_signals(type1 name1, type2 name2) {                                           \
        for (const auto &object: w_object_observer_##name_signals) {         \
            object(name1, name2);                                                     \
        }                                                                \
    }

#define W_EMITTABLE_0(name_signals)             \
    W_EMITTABLE_PRIVATE_LIST(name_signals);     \
    W_EMITTABLE_PRIVATE_FUNC_0(name_signals)    \
    W_EMITTABLE_PRIVATE_REGI(name_signals)

#define W_EMITTABLE_1(name_signals, type) \
    W_EMITTABLE_PRIVATE_LIST(name_signals, type) \
    W_EMITTABLE_PRIVATE_FUNC_1(name_signals, type, name) \
    W_EMITTABLE_PRIVATE_REGI(name_signals, type)

#define W_EMITTABLE_2(name_signals, type1, type2) \
    W_EMITTABLE_PRIVATE_LIST(name_signals, type1, type2) \
    W_EMITTABLE_PRIVATE_FUNC_2(name_signals, type1, name1, type2, name2) \
    W_EMITTABLE_PRIVATE_REGI(name_signals, type1, type2)                                              \

class WObject {
private:
    explicit WObject(WObject *parent);
    virtual ~WObject();

    W_EMITTABLE_0(positionChanged);
    W_EMITTABLE_1(positionOpt, int);
    W_EMITTABLE_2(positione_cambiata, int, int);
};

#endif //WRITERNOTE_WOBJECT_H
