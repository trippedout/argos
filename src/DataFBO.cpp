#include "DataFBO.h"

using namespace ci;
using namespace ci::app;

DataFBO::DataFBO(Assets* assets, VideoViewRef vid) : InputView(assets)
{
	animationDelay = 0.0f; 
	video = vid; 
	dataController = assets->getData(); 

	currentCards = new std::vector<DataViewCardRef>();

	//gl::Fbo::Format format;
	//format.setSamples(4);
	//fbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );

	currentCard = 0;
	shader = assets->getShaderByID("data"); 
	
	holder = DisplayObjectContainerRef(new DisplayObjectContainer()); 
	holder->setPosition(ci::Vec3f(360.f + 1280.0f/2.0f, 160.f,0));
	
	int i; 
	std::vector< boost::shared_ptr<DataNode> > nodes = *dataController->getDataNodes();
	int size = nodes.size(); 

	for( i=0; i<size; i++){
		cards.push_back( DataViewCardRef( new DataViewCard(nodes[i], assets) ) );
		cards[i]->getInputSignal()->connect(boost::bind(&DataFBO::onContentInputBorder, this, boost::arg<1>::arg(), boost::arg<2>::arg()));
	}
	for( i=0; i<20; i++){
		ticks.push_back( ImageViewRef( new ImageView(assets->getTextureByID("data_itemTick"))) ); 
		ticks[i]->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
		ticks[i]->setAdditiveBlending(true);
	}

	tickHolder = DisplayObjectContainerRef( new DisplayObjectContainer() );
	tickHolder->setPosition(ci::Vec3f(-20.0f, 710.0f,0));
	holder->addChild(tickHolder);

	currentPos = ci::Vec3f(); 
	nextPos = ci::Vec3f(1200.0f - 300.0f, 175.0f + 100.0f, 300.0f); 
	prevPos = ci::Vec3f(-600.0f - 480.0f, 175.0f + 100.0f, 300.0f);  

	currentRot = ci::Vec3f(); 
	nextRot = ci::Vec3f(0, 90.0f, 0); 
	prevRot = ci::Vec3f(0, -90.0f, 0);

	addChild(holder);
}

void DataFBO::onContentInputBorder(InputView* view, InputView::InputState state)
{
	switch(state){
	case InputView::InputState::BORDER_LEFT:
		//ci::app::console() << "DataFBO::onContentInputBorder BORDER_LEFT" << std::endl;
		prev(); 
		break;

	case InputView::InputState::BORDER_RIGHT:
		//ci::app::console() << "DataFBO::onContentInputBorder BORDER_RIGHT" << std::endl;
		next(); 
		break;
	}
}

void DataFBO::render()
{
	renderBasic(); 
	//renderShader();

}

void DataFBO::onAnimateIn()
{
	std::vector<DataViewCardRef> ccs = *currentCards;

	int size = ccs.size(); 
	for(int i=0; i<size; i++){
		if(holder->contains(ccs[i])) holder->removeChild(ccs[i]);
	}
	
	delete currentCards;

	currentCards = new std::vector<DataViewCardRef>();

	//'unlock' current scene for the pause menu. 
	dataController->getCurrentScene( video->getVideo()->getCurrentTime() )->setLocked(false);

	std::vector< boost::shared_ptr<DataNode> > nodes = *dataController->getCurrentSceneDataNodes( video->getVideo()->getCurrentTime() );

	size = nodes.size(); 
	int numCards = cards.size(); 

	//ci::app::console() << "[DataFBO](onAnimateIn) video time: " << video->getVideo()->getCurrentTime() <<" / total cards: " << numCards << " / current nodes: " << size << std::endl;

	for(int i=size-1; i>=0; i--){
		for(int j=0; j<numCards; j++){
			if(cards[j]->getDataNode() == nodes[i]){
				currentCards->push_back(cards[j]);
				cards[j]->reset();
				holder->addChild( cards[j] );
			}
		}
	}

	for( unsigned int i=0; i<ticks.size(); i++){
		tickHolder->removeChild(ticks[i]);
	}

	for( int i=0; i<size; i++){
		ticks[i]->setPosition( ci::Vec3f(-10.0f*(float)size + 20.0f*(float)i,0,0) ); 
		tickHolder->addChild(ticks[i]);
	}
	
	if(size > 0){
		currentCard = 0;
		updatePositions(true);
	}

	for( int i=0; i<size; i++){
		currentCards->at(i)->onAnimateIn(); 
	}

	holder->setPosition(ci::Vec3f(360.f + 1280.0f/2.0f, 160.f,-500.0f));
	timeline().apply(holder->getPosition(), ci::Vec3f(360.f + 1280.0f/2.0f, 160.f,0), 1.0f, EaseOutQuint() );

	animationDelay = 1.0f;
}

void DataFBO::onAnimateOut()
{
	//ci::app::console() <<"DataFBO::onAnimateOut()" <<std::endl;

	int size = cards.size(); 
	for(int i=0; i<size; i++){
		cards[i]->closeView(); 
	}
	
	timeline().apply(holder->getPosition(), ci::Vec3f(360.f + 1280.0f/2.0f, 160.f,-300.0f), 0.5f, EaseInQuint() ); 
	animationDelay = 0.5f;

	if(currentCards->size() <=0) return; 

	currentCards->at(currentCard)->onAnimateOut(); 

	for( unsigned int i=0; i<currentCards->size(); i++){
		if(i != currentCard ){
			timeline().apply(currentCards->at(i)->getAlpha(), 0.0f, 0.5);
		}
	}
	
}

void DataFBO::updatePositions(bool snap)
{
	//TODO: collapse animation for previous and next, expand animation for current. 
	
	std::vector<DataViewCardRef> ccs = *currentCards;

	float d = 0.25f;
	float t = 0.25f;
	
	if(snap) {
		t = 0;
		d = 0; 
	}

	int size = ccs.size();
	if(size ==0) return; 
	
	//update data ticks
	for( unsigned int i=0; i<size; i++){
		
		if( i == currentCard){
			timeline().apply(ticks[i]->getAlpha(), 0.9f, 0.25f, EaseOutQuint());
			timeline().apply(ticks[i]->getColor(), ci::Vec3f(1.0f,1.0f,1.0f), 0.25f, EaseOutQuint());
		}else{
			timeline().apply(ticks[i]->getColor(), ci::Vec3f(0, 0.5f, 0.99f), 0.25f, EaseOutQuint());
			timeline().apply(ticks[i]->getAlpha(), 0.5f, 0.25f, EaseOutQuint());
		}
	}

	//move cards
	timeline().apply(ccs[currentCard]->getPosition(), currentPos, t, EaseOutQuint()).delay(d);
	timeline().apply(ccs[currentCard]->getRotation(), currentRot, t, EaseOutQuint()).delay(d);
	ccs[currentCard]->setAlpha(1.0f);
	
	if(currentCard-1 >= 0){
		timeline().apply(ccs[currentCard-1]->getPosition(), prevPos, t, EaseOutQuint()).delay(d);
		timeline().apply(ccs[currentCard-1]->getRotation(), prevRot, t*0.75f, EaseOutQuint()).delay(d);
		holder->addChild(ccs[currentCard-1]);
	}
	if(currentCard+1 <= size-1){
		timeline().apply(ccs[currentCard+1]->getPosition(), nextPos, t, EaseOutQuint()).delay(d);
		timeline().apply(ccs[currentCard+1]->getRotation(), nextRot, t*0.75f, EaseOutQuint()).delay(d);
		holder->addChild(ccs[currentCard+1]);
	}

	//hack, always make current card on top. 
	holder->addChild(ccs[currentCard]);

	ci::Vec3f offset = ci::Vec3f(0,0,300);
	for( int i=0; i<size; i++){
		if(i == currentCard ){
			ccs[i]->show(snap);
		}else{
			ccs[i]->hide(snap);
		}

		if(i == currentCard || i == currentCard + 1 || i == currentCard - 1){
			timeline().apply(ccs[i]->getAlpha(), 1.0f, t).delay(d);
					//.updateFn(std::bind(&DisplayObjectContainer::setInternalAlpha, cards[i])).delay(d);
		}else{
			timeline().apply(ccs[i]->getAlpha(), 0.0f, t).delay(d);
					//.updateFn(std::bind(&DisplayObjectContainer::setInternalAlpha, cards[i])).delay(d);

			if( i > currentCard)
				timeline().apply(ccs[i]->getPosition(), nextPos + offset, t, EaseInQuint()).delay(d);
			else
				timeline().apply(ccs[i]->getPosition(), prevPos + offset, t, EaseInQuint()).delay(d);
		}
	}

}

bool DataFBO::showLeft()
{
	return currentCard > 0;
}

bool DataFBO::showRight() 
{
	int size = currentCards->size();
	return currentCard < size-1; 
}

void DataFBO::next()
{
	if(animationDelay > 0 || Overlay::getInstance().getActive()) return; 

	int size = currentCards->size();
	if(size==0) return; 

	currentCard++;
	if(currentCard >= size){
		currentCard = size-1;
		return;
	}

	updatePositions(false);

	animationDelay = 0.51f;
}

void DataFBO::prev()
{
	if(animationDelay > 0 || Overlay::getInstance().getActive()) return; 

	int size = currentCards->size();
	if(size==0) return; 

	currentCard--;
	if(currentCard < 0){
		currentCard = 0;
		return;
	}

	
	updatePositions(false);

	animationDelay = 0.5f;
}

void DataFBO::handleInput(INPUT_TYPE type)
{
	int size = currentCards->size();
	if(size==0) return; 

	std::vector<DataViewCardRef> ccs = *currentCards;
	ccs[currentCard]->handleInput(type);
}

#ifdef _WIN32
void DataFBO::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	int size = currentCards->size();
	if(size==0) return; 

	std::vector<DataViewCardRef> ccs = *currentCards;
	ccs[currentCard]->handleGamepad(gamepad);
}
#endif

void DataFBO::update(float dt)
{
	InputView::update(dt);
	if(animationDelay > 0) animationDelay-=dt;
}

void DataFBO::prerender()
{
	//pre-render inner views first. 
	int size = currentCards->size();
	if(size>0){
		std::vector<DataViewCardRef> ccs = *currentCards;

		for(int i=0; i<size; i++){
			ccs[i]->prerender();
		}
	}

	/*
	//bind FBO, render children. 

	gl::SaveFramebufferBinding bindingSaver;
	fbo.bindFramebuffer();

	float fov = 60.0f; 
	float camZ = (FBO_HEIGHT*0.5f) / tan(fov * 0.5f * (float)0.0174532925);
	
	CameraPersp cam( FBO_WIDTH, FBO_HEIGHT, fov );
	cam.setPerspective( 60.0f, (float)FBO_WIDTH/(float)FBO_HEIGHT, 1.0f, 10000.0f );
	ci::Vec3f camPos = ci::Vec3f( (float)FBO_WIDTH/2.0f, (float)FBO_HEIGHT/2.0f, camZ );
	ci::Vec3f camTarget = ci::Vec3f( (float)FBO_WIDTH/2.0f, (float)FBO_HEIGHT/2.0f, 0 );
	ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

	

	cam.lookAt( camPos, camTarget, camUp);
	gl::setMatrices( cam );
	gl::setViewport( fbo.getBounds() );
	gl::enableAlphaBlending();
	gl::clear(ci::ColorA(0,0,0,0));
	gl::enableAdditiveBlending();

	for ( ChildIterator it = children.begin(); it != children.end(); it++ ){
		(*it)->render();
	}
	
	gl::enableAlphaBlending();
	//unbind FBO
	*/
}


void DataFBO::renderBasic()
{
	InputView::render(); 
	/*
	fbo.bindTexture();
	
	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * FBO_WIDTH, scale.value().y * FBO_HEIGHT, scale.value().z);
		
		glBegin( GL_QUADS );
			glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
			glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
			glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
			glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
		glEnd();

	glPopMatrix();
	*/
}

void DataFBO::renderShader()
{
	// apply shader
	/*
	fbo.bindTexture();
	
	gl::enableAdditiveBlending();

	shader->bind();
	shader->uniform( "alpha", 0.1f ); 
	

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * FBO_WIDTH, scale.value().y * FBO_HEIGHT, scale.value().z);
		
		glBegin( GL_QUADS );
			glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
			glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
			glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
			glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
		glEnd();
		
		shader->uniform( "alpha", 0.2f ); 
		glTranslatef(0, 0, -10);
		glBegin( GL_QUADS );
			glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
			glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
			glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
			glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
		glEnd();
		
		shader->uniform( "alpha", 1.0f ); 
		glTranslatef(0, 0, -10);
		glBegin( GL_QUADS );
			glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
			glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
			glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
			glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
		glEnd();

	glPopMatrix();
	
	shader->unbind(); 

	
	gl::enableAlphaBlending();
	*/
}

DataFBO::~DataFBO(void)
{
}
