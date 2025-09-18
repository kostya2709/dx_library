#ifndef DX_TOOLS
#define DX_TOOLS


#include "dx_graphics.hpp"
#include <vector>

class dx_painter;
class dx_tool_manager;



class dx_abstract_tool
{
protected:
    dx_tool_manager* tool_manager;
public:
    dx_abstract_tool (dx_tool_manager* tool_manager_) : tool_manager (tool_manager_) {}
    virtual void perform (dx_point position) noexcept = 0;
};


class dx_pencil : public dx_abstract_tool
{
public:
    dx_pencil (dx_tool_manager* tool_manager) : dx_abstract_tool (tool_manager){}
    void perform (dx_point position) noexcept;
};

class dx_eraser : public dx_abstract_tool
{
public:
    dx_eraser (dx_tool_manager* tool_manager) : dx_abstract_tool (tool_manager){}
    void perform (dx_point position) noexcept;
};


class dx_tool_manager
{
    std::vector <dx_abstract_tool*> tools;

public:
    dx_painter* painter;

    enum
    {
        PENCIL, ERASER
    };

    int active_tool = 0;

    void perform (dx_point position) noexcept;

    dx_tool_manager (void);
};


class dx_tool_box : public dx_hoverable_object
{
public:
    int tool_num = 0;
    dx_tool_manager* tool_manager;

    dx_tool_box (dx_point point1 = dx_point (0, 0), dx_point point2 = dx_point (50, 50), int tool_num_ = 0, dx_tool_manager* tool_manager = NULL);
    void operate (dx_point mouse) noexcept override;
};



#endif              // #indef DX_TOOLS