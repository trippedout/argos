#pragma once

class SelectableView
{
public:
	virtual void selectView(bool select) = 0;
	virtual bool isSelected() const { return selected; };

protected:
	bool		selected;

	SelectableView(void) { selected = false; }
};

