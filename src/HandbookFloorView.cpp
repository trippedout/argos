#include "HandbookFloorView.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;

HandbookFloorView::HandbookFloorView(Assets* assets) : View(assets),
	mAlpha( assets->getTextureByID("hb_floor_alpha") ),
	mBlur( assets->getTextureByID("hb_floor_blur") ),
	mParticle( assets->getTextureByID("hb_floor_particle" ) ),
	mFloorShader( assets->getShaderByID("hb_floor") ),
	mFloorVboShader( assets->getShaderByID("hb_floorVbo" ) ),
	floorRotation(ci::Vec3f(111,4,41)),
	floorOffset(ci::Vec3f(975,500,1500)),
	mPointCount(782),
	mMaxPoints(2),
	//animation
	mCenterPointIndex(782),
	mGridWidth(22),
	mGridHeight(52),
	mNumRecursive(0), 
	mMaxRecursive(1),
	mMaxDistance(1150),
	//particles
	mParticles(NULL)
{
	createFloorMesh();
	createLightBeamMeshes();
	createFBOs();	

	mCenterPoint = mFloorVertices.at(mCenterPointIndex);

	mParticles = FloorParticlesRef( new HandbookFloorParticles(assets, &mAnimatingVertices, mCenterPointIndex) );

	//set size for use in shaders/transformations
	this->setSize(ci::Vec3f(1920,1080,0));	
}

void HandbookFloorView::createFloorMesh()
{	
	mFloorMesh = *mAssets->getMeshByID("handbook_floor");
		
	//grab the base floor vertices so we know what to animate back to
	mFloorVertices = mFloorMesh.getVertices();
	mExtraVboData = new ci::Colorf[mFloorVertices.size()];

	//setup animation files
	for(unsigned int i = 0; i < mFloorVertices.size(); ++i)
	{
		mAnimatingVertices.push_back( mFloorVertices.at(i));
		float movement = Rand::randFloat() * 0.025f;
		if(movement < 0.008f) movement = 0.008f;

		mExtraVboData[i] = ci::Colorf(Rand::randFloat(), movement, 0.f);
	}

	mFloorMesh.appendColorsRgb(mExtraVboData, mFloorVertices.size());

	//create our floor mesh
	mFloorVbo = ci::gl::VboMesh( mFloorMesh );
}

void HandbookFloorView::createLightBeamMeshes()
{
	std::vector<ci::TriMesh*>* lightBeams = mAssets->getLightBeamMeshes();
	mLightBeams = new std::vector<LightBeamRef>;

	//for each mesh, iterate and create a new LightBeamRef
	for(unsigned int i = 0; i < lightBeams->size(); ++i)
	{
		std::vector<ci::Vec3f*>* points = new std::vector<ci::Vec3f*>;
		
		for( unsigned int j = 0; j < lightBeams->at(i)->getVertices().size() / 2; ++j )
		{
			int index = getClosestVertexIndex(lightBeams->at(i)->getVertices().at(j));
			points->push_back(&mAnimatingVertices.at(index).value());
		} 

		mLightBeams->push_back(LightBeamRef( new LightBeam(points, ci::Vec4f(101.f/255.f, 198.f/255.f, 228.f/255.f, 0.85f), 1.5f ) ) );
	}
}

int HandbookFloorView::getClosestVertexIndex(ci::Vec3f lightPoint)
{
	ci::Vec3f distance;

	for(unsigned int i = 0; i < mAnimatingVertices.size(); ++i)
	{
		distance = mAnimatingVertices.at(i).value() - lightPoint;
		float length = distance.length();

		if( length < 10.f )
			return i;
	}

	return -1;
}

void HandbookFloorView::createFBOs()
{
	Fbo::Format format;
	format.setSamples(16);
	mFbo1 = new Fbo(1920,1080,format);
	mFbo2 = new Fbo(1920,1080,format);
}

HandbookFloorView::~HandbookFloorView(void)
{
	mAlpha->reset();
	delete mAlpha;

	mBlur->reset();
	delete mBlur;

	mParticle->reset();
	delete mParticle;

	mFloorShader->reset();
	mFloorShader.reset();

	mFloorVboShader->reset();
	mFloorVboShader.reset();

	mAnimatingVertices.clear();

	mFloorVertices.clear();

	mParticles.reset();

	delete mExtraVboData;

	delete mFbo1; mFbo1 = NULL;
	delete mFbo2; mFbo2 = NULL;
	
	for( unsigned int i = 0; i < mLightBeams->size(); ++i )
	{
		mLightBeams->at(i).reset();
	}
	mLightBeams->clear();
	delete mLightBeams;
}

void HandbookFloorView::pingVerts()
{
	mNumRecursive = 0;
	mUsedIndices.clear();

	getNearestPoints(mCenterPointIndex);
}

void HandbookFloorView::getNearestPoints(int index)
{

	//ci::app::console() << "HandbookFloorView::getNearestPoints() " << index << std::endl;
	//get points ahead
	int ahead = index + mGridWidth + 1; 
	if( checkIndice(ahead) ) checkPoint(index, ahead);
	if( checkIndice(ahead-1) ) checkPoint(index, ahead-1);
	if( checkIndice(ahead-2) ) checkPoint(index, ahead-2);

	//next to
	if( checkIndice(index + 1) ) checkPoint(index,  index + 1 );
	if( checkIndice(index - 1) ) checkPoint(index,  index - 1 );

	//behind
	int behind = index - mGridWidth - 1; 
	if( checkIndice(behind) ) checkPoint(index, behind);
	if( checkIndice(behind+1) ) checkPoint(index, behind+1);
	if( checkIndice(behind+2) ) checkPoint(index, behind+2);
}

void HandbookFloorView::checkPoint(int index, int closest)
{
	//ci::app::console() << "\tcheckPoint() " << index << ", close: " << closest << std::endl;

	ci::Vec3f* center = &mFloorVertices.at(index);
	ci::Vec3f* point = &mFloorVertices.at(closest);
	ci::Vec3f distance = *point - *center;
	float length = distance.length();

	if(length < 250.f)
	{
		addPoint(closest);

		distance = *point - mCenterPoint;
		length = distance.length();

		if(length < mMaxDistance)
		{
			float delay = .5f * (length / mMaxDistance);
			float time = .2f;
			
			float offset = Rand::randFloat() * 150.f - 75.f;

			timeline().apply(&mAnimatingVertices.at(closest), ci::Vec3f(point->x, point->y, point->z + offset), time*0.5f, EaseOutSine() ).delay( delay );
			timeline().appendTo(&mAnimatingVertices.at(closest), ci::Vec3f(point->x, point->y, point->z - offset*0.5f), time, EaseInOutSine() );
			timeline().appendTo(&mAnimatingVertices.at(closest), ci::Vec3f(point->x, point->y, point->z), time*1.5f, EaseInOutSine() );

			getNearestPoints(closest);

		}
	}

	center = NULL;
	delete center;

	point = NULL;
	delete point;
}

void HandbookFloorView::addPoint(int index)
{
	//ci::app::console() << "HandbookFloorView::addPoint() : " << index << std::endl;
	mJustAdded.push_back(index);
	mUsedIndices.push_back(index);
}

bool HandbookFloorView::checkIndice(int index)
{
	//ci::app::console() << "HandbookFloorView::checkIndice() : " << index << std::endl;

	const unsigned int maxIndex = mFloorVertices.size() - 1;

	if( index <= maxIndex && index >= 0 ) 
	{
		//ci::app::console() << "\tin range!" << std::endl;
		return !isIndexUsed( index );
	}
	return false;
}

bool HandbookFloorView::isIndexUsed(int index)
{
	if(std::find(mUsedIndices.begin(), mUsedIndices.end(), index) != mUsedIndices.end())
		return true;
	else
		return false;	
}

void HandbookFloorView::update(float dt)
{
	std::vector<ci::Colorf> colors = mFloorMesh.getColorsRGB();
	
	for(unsigned int i = 0; i < mFloorVertices.size(); ++i)
	{
		mFloorMesh.getColorsRGB().at(i).r += colors[i].g;
		float phase = sinf(mFloorMesh.getColorsRGB().at(i).r);

		mAnimatingVertices.at(i).value().z += phase * 0.055f;

		mFloorMesh.getVertices().at(i) = mAnimatingVertices.at(i);
	}	
	mFloorVbo = ci::gl::VboMesh( mFloorMesh );

	//update light beams
	for( unsigned int i = 0; i < mLightBeams->size(); ++i )
	{
		mLightBeams->at(i)->update(dt);
	}

	mParticles->update();

	//randomly fire beam
	if( getElapsedFrames() % 65 == 0 )
	{
		getLightBeam()->animateInOut(1.f);
	}
}

LightBeamRef HandbookFloorView::getLightBeam()
{
	LightBeamRef beam = mLightBeams->at( floorf(Rand::randFloat() * mLightBeams->size()) );

	if(beam->getIsAnimating())
		return getLightBeam();
	else
		return beam;
}

void HandbookFloorView::prerender()
{
	float width = size.value().x;
	float height = size.value().y;
	
	//GLfloat light_positionWire[]	= { width/2.0f - 100.0f, height/2.0f, -1000.f, 1.0f };
	//GLfloat light_wireColor[]		= { 49.f/255.f, 145.f/255.f, 160.f/255.f, 1.f };

	glPushMatrix();

	//bind FBO, render children. 

		mFbo1->bindFramebuffer();

		float fov = 60.0f; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
		CameraPersp cam( (int)size.value().x, (int)size.value().y, fov );
		cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.f );
		ci::Vec3f camPos = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, camZ );
		ci::Vec3f camTarget = ci::Vec3f( size.value().x/2.0f, size.value().y/2.0f, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );
		gl::setViewport( mFbo1->getBounds() );
		gl::clear(ci::ColorA(0,0,0,0));

		gl::enableAdditiveBlending();

		glDisable(GL_TEXTURE_2D);
		glColor4f(49.f/255.f, 135.f/255.f, 160.f/255.f, .65f);
		glLineWidth( 0.2f );
		
		glPushMatrix();
			glTranslatef(width/2.0f - floorOffset.value().x, height/2.0f - floorOffset.value().y, floorOffset.value().z); 
			glRotatef(floorRotation.value().x, 1.0f, 0, 0); 
			glRotatef(floorRotation.value().y, 0, 1.0f, 0); 
			glRotatef(floorRotation.value().z, 0, 0, 1.0f); 
			
			//draw wireframe
			glDisable(GL_TEXTURE_2D);
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
			
			if(mFloorVboShader)
			{
				mFloorVboShader->bind();

				//glEnableClientState(GL_COLOR_ARRAY);
				//glColorPointer(3, GL_FLOAT, 0, mExtraVboData);

				gl::draw( mFloorVbo );		
				mFloorVboShader->unbind();
			}

			//render light beams
			for( unsigned int i = 0; i < mLightBeams->size(); ++i )
			{
				mLightBeams->at(i)->render();
			}

			//draw particles
			mParticles->render();
			

			// disable everything, return shit to normal for particles
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable(GL_TEXTURE_2D);

		glPopMatrix();
	
		

		gl::enableAlphaBlending();
		
		glEnable(GL_TEXTURE_2D);

		mFbo1->unbindFramebuffer();

	//unbind FBO
	glPopMatrix();

	renderShaders();
}

void HandbookFloorView::renderShaders()
{
	if(mFloorShader) 
	{
		glPushMatrix();
		
			gl::SaveFramebufferBinding bindingSaver;
			mFbo2->bindFramebuffer();

				gl::enableAlphaBlending();
				gl::clear( ci::ColorA( 0, 0, 0, 0 ) ); 
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
				ci::gl::Texture* texture = &mFbo1->getTexture(0);
				texture->setFlipped(true);

				mFloorShader->bind();
				texture->bind(0);
				mAlpha->bind(1);
				mBlur->bind(2);
		
				//draw fbo1 with shader first pass
				if(mFloorShader)
				{
					mFloorShader->uniform("direction",0);
					mFloorShader->uniform("mapTexture",0);
					mFloorShader->uniform("vignetteAlpha",1);
					mFloorShader->uniform("vignetteBlur",2);
				}
				gl::draw( *texture, ci::Rectf( 0.f, 0.f, 1920.f, 1080.f) );
		
			
				mAlpha->unbind();
				mBlur->unbind();
				texture->unbind();
				mFloorShader->unbind();

			mFbo2->unbindFramebuffer();
		
		glPopMatrix();
	}
}

void HandbookFloorView::render()
{
	if(alpha->value() <= 0) return; 
	
	glPushMatrix(); 
	{
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
		
		draw();
	}
	glPopMatrix(); 
}

void HandbookFloorView::draw()
{	
	if(mFloorShader) 
	{
		gl::enableAdditiveBlending(); 
	
		//start second pass of shader and render to screen
		ci::gl::Texture* texture = &mFbo2->getTexture(0);
		texture->setFlipped(true);

		mFloorShader->bind();
		texture->bind(0);
		mAlpha->bind(1);
		mBlur->bind(2);
		
		if(mFloorShader)
		{
			mFloorShader->uniform("direction",1);
			mFloorShader->uniform("mapTexture",0);
			mFloorShader->uniform("vignetteAlpha",1);
			mFloorShader->uniform("vignetteBlur",2);
		}
		
		//move whole thing down 200 so that lookat is centered along bottom 
		//gl::translate(0,200,0);
		drawQuad();
		//gl::draw( *texture );//, ci::Rectf( 0.f, 0.f, 1920.f, (float)FBO_HEIGHT ) );
		
		mAlpha->unbind();
		mBlur->unbind();
		texture->unbind();
		mFloorShader->unbind();

		gl::enableAlphaBlending(); 
	}
}

void HandbookFloorView::drawQuad()
{
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}