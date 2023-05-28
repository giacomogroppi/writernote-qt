#include "TabletPenMethod.h"
#include "utils/WCommonScript.h"
#include "utils/setting_define.h"
#include "core/WOptionSettings.h"

void TabletPenMethod::save() const
{
    WOptionSettings settings;
    settings.begin();

    settings.setValue(KEY_METHOD_TOUCH, _method);
    settings.save();
}

void TabletPenMethod::load()
{
    WOptionSettings setting;
    setting.begin();

    const auto s = setting.value(KEY_METHOD_TOUCH, PrivateTabletMethod_Pen).toInt();

    _method = s;

    switch (_method) {
        case PrivateTabletMethod_Rubber:        this->setRubber(); break;
        case PrivateTabletMethod_Highlighter:   this->setHighlighter(); break;
        case PrivateTabletMethod_Pen:           this->setPen(); break;
        case PrivateTabletMethod_Selection:     this->setSelection(); break;
        case PrivateTabletMethod_Text:          this->setText(); break;
        case PrivateTabletMethod_Laser:         this->setLaser(); break;
        default: W_ASSERT(0);
    }

    setting.save();
}

TabletPenMethod::TabletPenMethod(const TabletPenMethod &t):
    _method(t._method),
    _currentTools(t._currentTools),
    _penMethod(t._penMethod),
    _rubberMethod(t._rubberMethod),
    _squareMethod(t._squareMethod),
    _highligterMethod(t._highligterMethod),
    _laserMethod(t._laserMethod)
{
}

TabletPenMethod::TabletPenMethod(bool FirstLoad,
                                 PenMethod *pen,
                                 RubberMethod *rubber,
                                 SquareMethod *square,
                                 HighligterMethod *highligter,
                                 LaserMethod *laser) :
        _currentTools(nullptr),
        _penMethod(pen),
        _rubberMethod(rubber),
        _squareMethod(square),
        _highligterMethod(highligter),
        _laserMethod(laser)
{
    W_ASSERT(FirstLoad == true);
    this->load();
}

TabletPenMethod::TabletPenMethod()
{
    this->_method = PrivateTabletMethod_Pen;
}

TabletPenMethod &TabletPenMethod::operator=(const TabletPenMethod &other)
{
    if (this == &other)
        return *this;

    this->_method = other._method;
    this->_highligterMethod = other._highligterMethod;
    this->_rubberMethod = other._rubberMethod;
    this->_penMethod = other._penMethod;
    this->_laserMethod = other._laserMethod;
    this->_squareMethod = other._squareMethod;
    this->_currentTools = other._currentTools;

    rep();

    return *this;
}
