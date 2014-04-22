#include "DataFloorView.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;

DataFloorView::DataFloorView(Assets* assets) :
	mShader( assets->getShaderByID("data_floor") ),
	mOverlay( assets->getTextureByID("data_floor_overlay") ),
	mFOV(15.f), mRotationSpeed(0.1f),
	mFloorMesh(NULL), mFloorVbo(NULL), 
	mFloorOffset(ci::Vec3f(0.f, 120.f, 0.f)), mFloorRotation(ci::Vec3f(108.f, 0.f, 0.f)), mFloorScale(ci::Vec3f(1.5f,1.5f,1.0f)),
	mRot1(0.f), mRot2(0.f), mRot3(0.f), mRot4(0.f)
{
	this->setSize(ci::Vec3f(768,512,0));
	this->setCenter(ci::Vec3f(0.5f, 1.f, 0.f));
	this->setPosition(ci::Vec3f(getWindowWidth()/2, getWindowHeight(), 0.f));

	Fbo::Format format;
	format.setSamples(16);
	mFloorFbo = new Fbo(size.value().x,size.value().y,format);

	//create verts
	float verts = 90.f;
	float inner_rad = 150.f, outer_rad = 225.f;
	float deg = 360.0f / verts;
//	float TWO_PI = M_PI * 2.f;
	float angle(0.f);
	
	//degtorad
	deg *=  0.0174532925f;

	mVertices = new std::vector<FloorVerticeRef>;

	for(int i = 0; i < verts; i++)
	{
		angle = (float)i * deg;
		mVertices->push_back(FloorVerticeRef( new FloorVertice(ci::Vec3f(inner_rad * cos(angle), inner_rad * sin(angle), 0.f))));
		mVertices->push_back(FloorVerticeRef( new FloorVertice(ci::Vec3f(outer_rad * cos(angle), outer_rad * sin(angle), 0.f))));

		angle = (float)(i+1) * deg;
		mVertices->push_back(FloorVerticeRef( new FloorVertice(ci::Vec3f(outer_rad * cos(angle), outer_rad * sin(angle), 0.f))));
		mVertices->push_back(FloorVerticeRef( new FloorVertice(ci::Vec3f(inner_rad * cos(angle), inner_rad * sin(angle), 0.f))));
	} 
	
	//create circle particles
	mNumCirclePoints		=	48;
	mInnerCirclePositions	=	new ci::Vec2f[mNumCirclePoints];
	mMiddleCirclePositions	=	new ci::Vec2f[mNumCirclePoints];
	mOuterCirclePositions	=	new ci::Vec2f[mNumCirclePoints];
	mLargestCirclePositions =	new ci::Vec2f[mNumCirclePoints];
	
	const float DEG2RAD = (float)M_PI/180.0f;
	deg = 360.0f / (float)mNumCirclePoints;
	const int innerRadius = 76;
	const int middleRadius = 175;
	const int outerRadius = 280;
	const int largestRadius = 456;
	ci::Vec2f pos;

	for(int i = 0; i < mNumCirclePoints; ++i)
	{
		float degInRad = ((float)i*deg) * DEG2RAD;
		pos = ci::Vec2f(cos(degInRad) * innerRadius, sin(degInRad) * innerRadius);
		mInnerCirclePositions[i] = pos;

		pos = ci::Vec2f(cos(degInRad) * middleRadius, sin(degInRad) * middleRadius);
		mMiddleCirclePositions[i] = pos;

		pos = ci::Vec2f(cos(degInRad) * outerRadius, sin(degInRad) * outerRadius);
		mOuterCirclePositions[i] = pos;

		pos = ci::Vec2f(cos(degInRad) * largestRadius, sin(degInRad) * largestRadius);
		mLargestCirclePositions[i] = pos;
	}

	
	mPos = ci::Vec3f(0.f, -200.f, 0.f);	
	//mScale = ci::Vec3f(1,1,1);
}


DataFloorView::~DataFloorView(void)
{	
	delete[] mInnerCirclePositions;
	delete[] mMiddleCirclePositions;
	delete[] mOuterCirclePositions;

	mShader->reset();
	mShader.reset();

	mOverlay->reset();
	delete mOverlay;

	for(unsigned int i = 0; i < mVertices->size(); ++i)
	{
		mVertices->at(i)->update();
	}
	mVertices->clear();
	delete mVertices;
}

void DataFloorView::animateIn()
{
	ci::app::console() << "DataFloorView::onAnimateIn()" << std::endl;

	for(unsigned int i = 0; i < mVertices->size(); i+=4)
	{
		float delay = Rand::randFloat() * 0.003f;
		if(delay < 0.0015f) delay = 0.0015f;
		
		//for movie delay
		float d = 0.35f;

		mVertices->at(i)->animateIn(i * delay + d);
		mVertices->at(i+1)->animateIn(i * (delay + 0.00045f) + d);
		mVertices->at(i+2)->animateIn(i * (delay + 0.00045f) + d);
		mVertices->at(i+3)->animateIn(i * delay + d);
	}
}

void DataFloorView::animateOut()
{
	for(unsigned int i = 0; i < mVertices->size(); i+=4)
	{
		float delay = Rand::randFloat() * 0.0025f;
		if(delay < 0.001f) delay = 0.001f;

		mVertices->at(i)->animateOut(i * delay);
		mVertices->at(i+1)->animateOut(i * (delay + 0.00025f));
		mVertices->at(i+2)->animateOut(i * (delay + 0.00025f));
		mVertices->at(i+3)->animateOut(i * delay);
	}
}

void DataFloorView::update(float dt)
{
	mFloorRotation.z += mRotationSpeed;

	for(unsigned int i = 0; i < mVertices->size(); ++i)
	{
		mVertices->at(i)->update();
	}

	mRot1 += .125;
	mRot2 += .195;
	mRot3 += .035;
	mRot4 += .095;
}

void DataFloorView::prerender()
{	
	glPushMatrix();
	{
			float fov = mFOV; 
			float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
			CameraPersp cam( 0, 0, fov );
			cam.setPerspective( mFOV, size.value().x/size.value().y, 1.0f, 10000.0f );
		
			ci::Vec3f camPos = ci::Vec3f( 0, 200.0f, camZ );
			ci::Vec3f camTarget = ci::Vec3f( 0, 0, 0 );
			ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

			cam.lookAt( camPos, camTarget, camUp);
			gl::setMatrices( cam );
			gl::setViewport( mFloorFbo->getBounds() );
		
			gl::enableAdditiveBlending();

			mFloorFbo->bindFramebuffer();
			gl::clear(ci::ColorA(0.f,0,0,0.f));

			glDisable(GL_TEXTURE_2D);
			//glEnable( GL_LIGHTING );
			//glEnable( GL_LIGHT0 );
			//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glLineWidth( 2.25f );

//			GLfloat light_positionWire[]	= { 100.0f, 0, 1000.0f, 1.0f };
//			GLfloat light_wireColor[]		= { 0.51f, 0.98f, 1.0f, .5f };
		
			glColor4f(1.f, 1.f, 1.f, 1.f);


			glPushMatrix();
				glTranslatef(mFloorOffset.x, mFloorOffset.y,  mFloorOffset.z); 
				glRotatef(mFloorRotation.x, 1.0f, 0, 0); 
				glRotatef(mFloorRotation.y, 0, 1.0f, 0); 
				glRotatef(mFloorRotation.z, 0, 0, 1.0f); 
				glScalef(mFloorScale.x, mFloorScale.y, mFloorScale.z); 

			//draw wireframe
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
				//glLightfv( GL_LIGHT0, GL_DIFFUSE, light_wireColor );

				for(unsigned int i = 0; i < mVertices->size(); i += 4)
				{
					gl::begin(GL_LINE_LOOP);
					gl::vertex(*mVertices->at(i)->getPosition());					
					gl::vertex(*mVertices->at(i+1)->getPosition());					
					gl::vertex(*mVertices->at(i+2)->getPosition());					
					gl::vertex(*mVertices->at(i+3)->getPosition());
					gl::end();
				} 
					

			glPopMatrix();

			//disable shit
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			//glDisable( GL_LIGHTING );
			//glDisable( GL_LIGHT0 );
			glEnable(GL_TEXTURE_2D);

		
			mFloorFbo->unbindFramebuffer();

			gl::enableAlphaBlending();
		
	}
	glPopMatrix();
}

void DataFloorView::render()
{
	gl::enableAdditiveBlending(); 

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		
		const ci::gl::Texture fbotex = mFloorFbo->getTexture();
		//draw sphere normally
		
		if(mShader)
		{
			mShader->bind();
			fbotex.bind(0); 
			mOverlay->bind(1);
			
			mShader->uniform("tex", 0);
			mShader->uniform("overlay", 1);

			glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
			
			drawQuad();

			mOverlay->unbind(1);
			fbotex.unbind(0);
			mShader->unbind();
		}
	
	glPopMatrix(); 
	
	glPushMatrix(); 
		glColor4f(.52f, .84f, .91f, alpha->value());
		renderCircles();
	glPopMatrix();

	gl::enableAlphaBlending(); 
}

void DataFloorView::renderCircles()
{
	glPushMatrix();
	{
		float fov = mFOV; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
		CameraPersp cam( 0, 0, fov );
		cam.setPerspective( mFOV, size.value().x/size.value().y, 1.0f, 10000.0f );
		
		ci::Vec3f camPos = ci::Vec3f( 0, 200.0f, camZ );
		ci::Vec3f camTarget = ci::Vec3f( 0, 0, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );

		gl::translate(mPos);
		gl::rotate(ci::Vec3f(84.f,0.f,0.f));
		//gl::scale(mScale);

		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glEnableClientState(GL_VERTEX_ARRAY);
	
		glPushMatrix();
		{
			glRotatef(mRot1, 0.f, 0.f, 1.f);
			glVertexPointer(2, GL_FLOAT, 0, mInnerCirclePositions);
			glDrawArrays(GL_POINTS, 0, mNumCirclePoints);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(mRot2, 0.f, 0.f, 1.f);
			glVertexPointer(2, GL_FLOAT, 0, mMiddleCirclePositions);
			glDrawArrays(GL_POINTS, 0, mNumCirclePoints);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(mRot3, 0.f, 0.f, 1.f);
			glVertexPointer(2, GL_FLOAT, 0, mOuterCirclePositions);
			glDrawArrays(GL_POINTS, 0, mNumCirclePoints);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(mRot4, 0.f, 0.f, 1.f);
			glVertexPointer(2, GL_FLOAT, 0, mLargestCirclePositions);
			glDrawArrays(GL_POINTS, 0, mNumCirclePoints);		
		}
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
	}
	glPopMatrix();

}

void DataFloorView::drawQuad()
{
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}



void FloorVertice::update()
{
	mPosition.z = mZOffset;
}

void FloorVertice::animateIn(float delay)
{
	mZOffset = mOffScreen;
	timeline().apply(&mZOffset, mOffScreen, 0.f, .5f, EaseOutBack() ).delay(delay);
}

void FloorVertice::animateOut(float delay)
{
	mZOffset = 0.f;
	timeline().apply(&mZOffset, 0.f, mOffScreen, .25f, EaseInBack()).delay(delay);
}