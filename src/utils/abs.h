#ifndef ABS_H
#define ABS_H

inline double __abs(double &val){
    return (val > (double)0.0) ? val : -val;
}

#endif // ABS_H
