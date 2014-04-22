#include "DataInnerViewSimple.h"

using namespace ci;
using namespace ci::app;

DataInnerViewSimple::DataInnerViewSimple(boost::shared_ptr<DataNode> data,  Assets* assets):View(assets)
{
	//setScale(ci::Vec3f(1.5f, 1.5f, 1.5f));
	blur = ci::Anim<float>(0.0f); 

	title = boost::shared_ptr<DataViewTitle>( new DataViewTitle(data, assets) ); 
	title->setPosition(ci::Vec3f( -title->getSize()->value().x/2.0f, 35.0f, 0 ));
	title->project3D(false);

	mBarTop	=	ImageViewRef( new ImageView(assets->getTextureByID("card_bars")) );
	mBarTop->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mBarTop->setScale(ci::Vec3f(0.4f,1.0f,0)); 
	mBarTop->setPosition(ci::Vec3f(0,0,0));
	//mBarTop->setAlpha(0);

	mBarBottom	=	ImageViewRef( new ImageView(assets->getTextureByID("card_bars")) );
	mBarBottom->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mBarBottom->setScale(ci::Vec3f(0.4f,1.0f,0)); 
	mBarBottom->setPosition(ci::Vec3f(0,160.0f-35,0));
	//mBarBottom->setAlpha(0);
	
	shader = assets->getShaderByID( "data_item_blur" );
	size = ci::Vec3f(768.0f, 256.0f, 0);
	gl::Fbo::Format format;
	format.setSamples(16);
	fbo = gl::Fbo( size.value().x, size.value().y, format );

	setCenter(ci::Vec3f(0.5f, 0.5f,0)); 

	addChild(title);
	addChild(mBarTop);
	addChild(mBarBottom);

	prerender();
}

DataInnerViewSimple::~DataInnerViewSimple(void)
{

}

void DataInnerViewSimple::prerender()
{
	glPushMatrix();

	//bind FBO, render children. 

		gl::SaveFramebufferBinding bindingSaver;
		fbo.bindFramebuffer();

		float fov = 60.0f; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * 1;
	
		CameraPersp cam( size.value().x, size.value().y, fov );
		cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.0f );
	
		ci::Vec3f camPos = ci::Vec3f( 0, 0, camZ );
		ci::Vec3f camTarget = ci::Vec3f( 0, 0, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );
		gl::setViewport( fbo.getBounds() );

		gl::enableAlphaBlending();
		
		gl::clear(ci::ColorA(0,0,0,0));

		glPushMatrix();
			
			for ( ChildIterator it = children.begin(); it != children.end(); it++ ){
				(*it)->render();
			}
			//render children

		glPopMatrix();
		
		fbo.unbindFramebuffer();
	//unbind FBO
	glPopMatrix();
}

void DataInnerViewSimple::render()
{
	if(alpha->value() <= 0) return; 

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
		draw();
	glPopMatrix(); 
}

void DataInnerViewSimple::draw()
{

	const ci::gl::Texture fbotex = fbo.getTexture();
	
	gl::enableAdditiveBlending(); 

	fbotex.enableAndBind(); 

	float a = alpha->value();
	if(parent != NULL){
		a*= parent->getAlpha()->value();
	}

	if(a <= 0) return; 

	//draw fbo with shader
	shader->bind();
	shader->uniform( "alpha", a );
	shader->uniform( "blur", blur.value() );
	
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
	
	shader->unbind(); 
	fbotex.unbind(); 

	gl::enableAlphaBlending(); 
}


void DataInnerViewSimple::show(bool snap)
{
	//ci::app::console()<<"DataInnerViewSimple::show"<<std::endl; 
	float m = 1.0;
	if(snap){
		m=0;
	}
	
	timeline().apply(title->getAlpha(), 0.6f, 0.1f*m, EaseOutQuint()).delay(0.1f*m);
	timeline().apply(mBarTop->getAlpha(), 1.0f, 0.25f*m, EaseOutQuint()).delay(0.2f*m);
	timeline().apply(mBarBottom->getAlpha(), 1.0f, 0.25f*m, EaseOutQuint()).delay(0.2f*m);
}

void DataInnerViewSimple::hide(bool snap)
{
	//ci::app::console()<<"DataInnerViewSimple::hide"<<std::endl; 

	float t = 0.25f;
	float d = 0.5f;
	if(snap){
		t=0;
		d=0;
	}
	
	timeline().apply(title->getAlpha(), 0.0f, 0.0f, EaseOutQuint());
	timeline().apply(mBarTop->getAlpha(), 0.0f, 0.0f, EaseOutQuint());
	timeline().apply(mBarBottom->getAlpha(), 0.0f, 0.0f, EaseOutQuint());
}