#pragma once

class TabletPenMethod {
private:
    int _method;
#define PrivateTabletMethod_Rubber          0
#define PrivateTabletMethod_Pen             1
#define PrivateTabletMethod_Selection       2
#define PrivateTabletMethod_Text            3
#define PrivateTabletMethod_Highlighter     4
#define PrivateTabletMethod_Laser           5

    void load();

public:
    explicit TabletPenMethod(bool FirstLoad);
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

    [[nodiscard]] bool isRubber() const;
    [[nodiscard]] bool isPen() const;
    [[nodiscard]] bool isSelection() const;
    [[nodiscard]] bool isText() const;
    [[nodiscard]] bool isHighlighter() const;
    [[nodiscard]] bool isLaser() const;
};

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