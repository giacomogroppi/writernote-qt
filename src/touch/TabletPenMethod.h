#pragma once

#include "touch/tools/Tools.h"
#include "touch/pen/PenMethod.h"
#include "touch/rubber/RubberMethod.h"
#include "touch/highlighter/HighligterMethod.h"
#include "touch/square/SquareMethod.h"
#include "touch/laser/LaserMethod.h"

class TabletPenMethod {
private:
    int _method{};
#define PrivateTabletMethod_Rubber          0
#define PrivateTabletMethod_Pen             1
#define PrivateTabletMethod_Selection       2
#define PrivateTabletMethod_Text            3
#define PrivateTabletMethod_Highlighter     4
#define PrivateTabletMethod_Laser           5

    void load();

    Tools *_currentTools;

    PenMethod *_penMethod;
    RubberMethod *_rubberMethod;
    SquareMethod *_squareMethod;
    HighligterMethod *_highligterMethod;
    LaserMethod *_laserMethod;
public:
    explicit TabletPenMethod(bool FirstLoad,
                             PenMethod *pen,
                             RubberMethod *rubber,
                             SquareMethod *square,
                             HighligterMethod *highligter,
                             LaserMethod *laser);
    TabletPenMethod();
    TabletPenMethod(const TabletPenMethod &t);
    void save() const;

    ~TabletPenMethod() = default;

    void setRubber();
    void setPen();           
    void setSelection();
    void setText();
    void setHighlighter();
    void setLaser();
    
    [[nodiscard]] bool isInsert() const;
    [[nodiscard]] bool isRubber() const;
    [[nodiscard]] bool isPen() const;
    [[nodiscard]] bool isSelection() const;
    [[nodiscard]] bool isText() const;
    [[nodiscard]] bool isHighlighter() const;
    [[nodiscard]] bool isLaser() const;

    [[nodiscard]] Tools *method() const;
};

inline void TabletPenMethod::setRubber()        
{ 
    _method = PrivateTabletMethod_Rubber;
    _currentTools = _rubberMethod;
}

inline void TabletPenMethod::setPen()           
{ 
    _method = PrivateTabletMethod_Pen;
    _currentTools = _penMethod;
}

inline void TabletPenMethod::setSelection()     
{ 
    _method = PrivateTabletMethod_Selection; 
    _currentTools = _squareMethod;
}

inline void TabletPenMethod::setText()          
{ 
    _method = PrivateTabletMethod_Selection;
    _currentTools = nullptr;
    W_ASSERT(0);
}

inline void TabletPenMethod::setHighlighter()   
{ 
    _method = PrivateTabletMethod_Highlighter;
    _currentTools = _highligterMethod;
}

inline void TabletPenMethod::setLaser()         
{ 
    _method = PrivateTabletMethod_Laser;
    _currentTools = _laserMethod;
}

inline bool TabletPenMethod::isRubber() const
{
    return _method == PrivateTabletMethod_Rubber;
}

inline bool TabletPenMethod::isPen() const
{
    return _method == PrivateTabletMethod_Pen;
}

inline bool TabletPenMethod::isSelection() const
{
    return _method == PrivateTabletMethod_Selection;
}

inline bool TabletPenMethod::isText() const
{
    return _method == PrivateTabletMethod_Text;
}

inline bool TabletPenMethod::isHighlighter() const
{
    return _method == PrivateTabletMethod_Highlighter;
}

inline bool TabletPenMethod::isLaser() const
{
    return _method == PrivateTabletMethod_Laser;
}

inline bool TabletPenMethod::isInsert() const
{
    return isPen() || isHighlighter() || isLaser();
}

inline Tools *TabletPenMethod::method() const
{
    W_ASSERT(this->_currentTools);
    return this->_currentTools;
}
