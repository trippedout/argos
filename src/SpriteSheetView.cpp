#include "SpriteSheetView.h"



SpriteSheetView::SpriteSheetView(ci::gl::Texture* tex, ci::Vec2i spriteSize, int totalFrames, float framerate):
	ImageView(tex),
	spriteSize(spriteSize),
	totalFrames(totalFrames),
	frameRate(frameRate),
	frameTime(0),
	frame(0)
{
	renderRect.x1 = 0; 
	renderRect.x2 = spriteSize.x;
	renderRect.y1 = 0; 
	renderRect.y2 = spriteSize.y;
	setSize(ci::Vec3f(spriteSize.x, spriteSize.y, 0)); 
}

SpriteSheetView::~SpriteSheetView(void)
{
}

void SpriteSheetView::update(float dt)
{
	/*
	frameTime += dt; 
	if( frameTime > 1.0f/frameRate){
		frameTime -= 1.0f/frameRate;
		frame++; 
		if(frame >= totalFrames ){
			frame=0; 
		}
	}*/
}

void SpriteSheetView::drawQuad()
{
	//TODO: this is hacks... for some reason spritesheet wasnt updating. 1 hour till submit so uh... yeah. 
	frame++; 
	if(frame >= totalFrames ){
		frame=0; 
	}

	ci::Rectf bounds = ci::Rectf( texture->getBounds() );
	float tw = bounds.x2 - bounds.x1; 
	float th = bounds.y2 - bounds.y1; 

	int framesPerRow = floor(tw/spriteSize.x);
	
	float sw = spriteSize.x / tw;
	float sh = spriteSize.y / th;

	float x = (frame % framesPerRow ) * sw;
	float y = floor((float)frame/(float)framesPerRow) * sh;
	//ci::app::console()<< "SpriteSheetView::drawQuad() frame "<< frame << " pos: " << x << " , " << y <<std::endl;

	glBegin( GL_QUADS );
		glTexCoord2f(x, y);			glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(x+sw, y);		glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(x+sw, y+sh);		glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(x, y+sh);		glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}
