#pragma once

#include "InputView.h"

class DataViewContentBase : public InputView
{
public:
	DataViewContentBase(Assets* assets);
	~DataViewContentBase(void);

	virtual void		handleInput(INPUT_TYPE type);
	virtual void		disposeTempAssets(); 

	virtual void show(bool snap); 
	virtual void hide(bool snap); 

	virtual void onViewOpen(); 
	virtual void onViewClose(); 
};

