#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

class DataViewListIcon : public ButtonView
{

public:
	typedef boost::shared_ptr<DataViewListIcon> DataViewListIconRef;

	enum IconType { OBJECT3D, DOCUMENT, MOVIE, FIGURE, CLOTHES, BOOK, OST, HELMET, DOGTAG, AUDIO };

	DataViewListIcon(Assets* assets, IconType type);
	DataViewListIcon(Assets* assets, IconType type, std::string label);
	~DataViewListIcon(void);

	virtual void	selectView(bool select);

protected:
	ImageViewRef		icon;
	ImageViewRef		icon2;
	ImageViewRef		icon3;

	TextViewRef			label; 
	void				init(Assets* assets, IconType type); 
};

