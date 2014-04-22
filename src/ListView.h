#pragma once

#include "InputView.h"

class ListView : public InputView
{
protected:
	std::vector<ButtonViewRef>	mListItems;

	int				mCurrentIndex;
	int				mTotalItems;

public:
	typedef boost::shared_ptr<ListView> ListViewRef;

	ListView(Assets* assets);
	~ListView(void);

	virtual void addItem(ButtonViewRef item);
	virtual void selectItem(int index);
	virtual void handleInput(INPUT_TYPE type);

	virtual int getHeight();
};

