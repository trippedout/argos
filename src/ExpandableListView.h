#pragma once

#include "ListView.h"

class ExpandableListView : public ListView
{
public:
	ExpandableListView(Assets* assets);
	~ExpandableListView(void);

	virtual void addItem(ButtonViewRef item);
	virtual void selectItem(int index);
	virtual void handleInput(INPUT_TYPE type);
};

