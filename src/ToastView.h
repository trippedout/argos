#pragma once
#include "View.h"
#include "ImageView.h"
#include "TextView.h"
#include "ScrubVideoObject.h"

#include <string>
#include <vector>

typedef boost::shared_ptr<class ToastView> ToastViewRef;

class ToastView :
	public View
{
public:
	ToastView(Assets* assets);
	~ToastView(void);

	void toast(std::string copy); 
	void endToast(); 

	void update(float dt); 
	void hideSelf(); 


protected: 
	bool toasting;
	int current; 

	float duration;
	std::vector<float> times;

	ImageViewRef bgBox; 
	ImageViewRef glow; 
	ImageViewRef textBG; 
	ScrubVideoRef video; 
	TextViewRef text;
	DataController* data; 
};

