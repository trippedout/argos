#include "SphereView.h"

using namespace ci;
using namespace ci::app;

SphereView::SphereView( Assets* assets ):DisplayObject(),
	innerSphere(ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("handbook_innerSphere")) )),
	//constellations
	mCurrentConst(NULL), mNextConst(NULL), mPrevConst(NULL),
	//particles
	mParticleShader(assets->getShaderByID("handbook_sphere_particles")),
	mSet1RotVal(0.f), mSet2RotVal(0.f), mSet3RotVal(0.f),
	mSpinSet1Speed(0.f), mSpinSet2Speed(0.f), mSpinSet3Speed(0.f), idle(true)
{
	cAlpha1 = ci::Anim<float>(0);
	cSolidAlpha1 = ci::Anim<float>(0);
	cBorderAlpha1 = ci::Anim<float>(0);

	cAlpha2 = ci::Anim<float>(0);
	cSolidAlpha2 = ci::Anim<float>(0);
	cBorderAlpha2 = ci::Anim<float>(0);
	
	mSphereAlpha = 0.f;
	
	currentConst = 10; 
	prevConst = 10; 
	nextConst = 10; 
	rotateDelay = 0; 
	sphereRotation = ci::Vec3f(); 

	waitingToSpin = false; 
	
	//constellations
	std::vector<Assets::Constellation> consts = assets->getConstellations(); 
	constellations = new std::vector<SphereConstellationRef>;
	for(unsigned int i = 0; i < consts.size(); ++i)
	{
		constellations->push_back( SphereConstellationRef(  new SphereConstellation( consts.at(i).mesh, consts.at(i).outline, consts.at(i).rotation )));
	}

	vbo = assets->getVBOByID( "handbook_sphere" );
	centerTex = assets->getTextureByID("handbook_sphere_texture");
	ringTex = assets->getTextureByID("handbook_sphere_ring");

	bloomShader = assets->getShaderByID("handbook_sphere_blur");
	centerShader = assets->getShaderByID("handbook_sphere_sphere");
	ringShader = assets->getShaderByID("handbook_sphere_ring");


	size = ci::Vec3f(1200, 900, 0);

	gl::Fbo::Format format;
	format.setSamples(16);
	fbo = new gl::Fbo( (int)size.value().x, (int)size.value().y, format );

	frame = 0; 
	prevFrame = 0; 

	//setAlpha(0);
	
	innerSphere->setCenter(ci::Vec3f(1.0f,0,0));
	innerSphere->setScale(ci::Vec3f(2.12f, 2.12f, 0)); 
	innerSphere->setPosition(ci::Vec3f(1200.0f,0,0)); 
	innerSphere->setLoop(0,2.0f); 
	innerSphere->play(); 
	innerSphere->setAlpha(0);

	sphereOffset = ci::Vec3f(-110.0f, -250.0f,0); 

	//particles
	mParticlesRotation = ci::Vec3f(90.f, 0.f, 90.f);
	mSet1Rot = -9.f;
	mSet2Rot = 54.f;
	mSet3Rot = -60.f;

	mParticleYOffset = 0.f;
	mParticleDrawDistance = 1200.f;
	mParticleAlpha = 0.f;

	ci::TriMesh* particleMesh = assets->getMeshByID("handbook_sphere_particles");
	mNumParticles = (int)particleMesh->getVertices().size();
	mPositions = new ci::Vec4f[mNumParticles];
	mParticleExtras = new ci::Vec4f[mNumParticles];

	mParticles = new std::vector<SphereParticleRef>(mNumParticles);

	for(int i = 0; i < mNumParticles; ++i)
	{
		mParticles->at(i) = SphereParticleRef( new HBSphereParticle( particleMesh->getVertices().at(i) ) );
		mParticleExtras[i] = ci::Vec4f();
		
		//random position seed and incremental to separate from particle seed
		mParticleExtras[i].y = Rand::randFloat();
		mParticleExtras[i].z = Rand::randFloat() * 0.05f;
		if( mParticleExtras[i].z < 0.03f ) mParticleExtras[i].z = 0.03f;
	}
	
	delete particleMesh;
}

SphereView::~SphereView(void)
{
	constellations->clear();
	delete constellations;

	vbo->reset();
	delete vbo;

	centerTex->reset();
	delete centerTex;

	ringTex->reset();
	delete ringTex;

	bloomShader->reset();
	bloomShader.reset();

	centerShader->reset();
	centerShader.reset();

	ringShader->reset();
	ringShader.reset();

	delete[] mPositions;
	delete[] mParticleExtras;

	for(int i = 0; i < mNumParticles; ++i)
	{
		mParticles->at(i).reset();
	}
	mParticles->clear();
	delete mParticles;
}

void SphereView::onAnimateIn()
{ 
	idle = true; 
	ci::app::console() << "SphereView::onAnimateIn" << std::endl; 

	//if(mCurrentConst)mCurrentConst->animateIn(&mSphereAlpha);

	timeline().removeTarget(&mSphereAlpha);
	timeline().removeTarget(innerSphere->getAlpha());

	timeline().apply(&mSphereAlpha, 1.0f, 1.0f, EaseOutQuint())	
		.finishFn(boost::bind(&SphereView::animateParticlesIn, this));
	
	timeline().apply(innerSphere->getAlpha(), 0.5f, 1.5f, EaseOutQuint());
}

void SphereView::onAnimateOut()
{ 

	ci::app::console() << "SphereView::onAnimateOut" << std::endl; 

	timeline().removeTarget(&mSphereAlpha);
	timeline().removeTarget(innerSphere->getAlpha());

	if(mCurrentConst)mCurrentConst->animateOut();

	animateParticlesOut();

	timeline().apply(&mSphereAlpha, 0.0f, 1.5f, EaseOutQuint());
	timeline().apply(innerSphere->getAlpha(), 0.0f, 0.5f, EaseOutQuint());
}

void SphereView::setConstellation(int val)
{ 
	if(constellations->at(val) == mNextConst) return; 


	//ci::app::console() << "SphereView::setConstellation() id: " << val << std::endl;

	waitingToSpin = true; 
	rotateDelay = 0.5f;
	mNextConst = constellations->at(val); 
}

void SphereView::changeConstellation()
{
	idle = false;

	//vars
	float delay = 0; 
	float t = 2.0f;
	
	//play sound regardless
	SoundController::getInstance().playSound( SoundController::HB_GLOBE_SPIN );
	
	//spin that sphere
	timeline().apply( getSphereRotation(), mNextConst->getRotation(), t, EaseInOutQuint() ).delay(delay);
	
	//check current constellation and animate in the fuck out
	if(mCurrentConst)
	{
		mPrevConst = mCurrentConst;
		//TODO - do we need to animate out? mCurrentConst->getAnimateOutCompleteSignal()->connect(boost::bind
		mPrevConst->animateOut();
	}


	//current done, set next
	mCurrentConst = mNextConst;
	mCurrentConst->animateIn(&mSphereAlpha);
}

void SphereView::update(float dt)
{
	if(idle){
		sphereRotation.value().x += dt * 5.0f; 
		sphereRotation.value().y += dt * -8.0f;  
		sphereRotation.value().z += dt * 4.0f;  
	}
	innerSphere->update(dt); 

	//constellations
	if(rotateDelay > 0) 
	{
		rotateDelay -= dt; 
		
		if(rotateDelay <= 0 && waitingToSpin){
			waitingToSpin = false;
			changeConstellation();
		}
	}

	if(mPrevConst) mPrevConst->update(dt);
	if(mCurrentConst) mCurrentConst->update(dt);
	
	//particle animations
	for(int i = 0; i < mNumParticles; ++i)
	{
		mParticles->at(i)->update();
		mPositions[i] = mParticles->at(i)->getPosition();
		
		//set extras ci::Vec4f( mainAlpha, x, y, z );
		mParticleExtras[i].x = mParticleAlpha;
	
		//increment new seed
		mParticleExtras[i].y += mParticleExtras[i].z;
	}

	//constant rotation 
	mSet1RotVal += mSpinSet1Speed;
	mSet2RotVal += mSpinSet2Speed;
	mSet3RotVal += mSpinSet3Speed;
}

void SphereView::prerender()
{
//	int width = (int)size.value().x;
//	int height = (int)size.value().y;

	glPushMatrix();

	//bind FBO, render children. 
		
		gl::SaveFramebufferBinding bindingSaver;
		fbo->bindFramebuffer();

		float fov = 60.0f; 
		float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
		CameraPersp cam( 0, 0, fov );
		cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, 10000.0f );
		
		ci::Vec3f camPos = ci::Vec3f( -200.0f, -300.0f, camZ );
		ci::Vec3f camTarget = ci::Vec3f( 0, 0, 0 );
		ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

		cam.lookAt( camPos, camTarget, camUp);
		gl::setMatrices( cam );
		gl::setViewport( fbo->getBounds() );
		gl::clear(ci::ColorA(0,0,0,0));

		gl::enableAdditiveBlending();

		glDisable(GL_TEXTURE_2D);
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLineWidth( 1.0f );

		GLfloat light_positionSolid[]	= { 1000.0f, -400.0f, -1000.0f, 1.0f };
		GLfloat light_positionWire[]	= { -100.0f, 0, -1000.0f, 1.0f };
		GLfloat light_solidColor[]		= { 0.88f * 0.25f, 0.81f * 0.25f, 1.0f * 0.25f, mSphereAlpha };
		GLfloat light_wireColor[]		= { 0.51f, 0.98f, 1.0f, mSphereAlpha };

		GLfloat mat_wireDiffuse[]		= { 0.8f, 0.9f, 1.0f, 0.5f * mSphereAlpha };
		GLfloat mat_wireAmbient[]		= { 0, 0, 0, 0 };

		GLfloat mat_solidDiffuse[]		= { 0.29f, 0.8f, 1.0f, 0.5f * mSphereAlpha };
		GLfloat mat_solidAmbient[]		= { 0.0f, 0.05f, 0.1f, 0.1f * mSphereAlpha };
		

		glColor4f(1.f, 1.f, 1.f, 1.f);

		glLightfv( GL_LIGHT0, GL_POSITION, light_positionSolid );
		
		glPushMatrix();
			glTranslatef(sphereOffset.x, sphereOffset.y, sphereOffset.z); 
			glRotatef(sphereRotation.value().x, 1.0f, 0, 0); 
			glRotatef(sphereRotation.value().y, 0, 1.0f, 0); 
			glRotatef(sphereRotation.value().z, 0, 0, 1.0f); 
			glScalef(0.7f, 0.7f, 0.7f); 

		//draw solid 
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glLightfv( GL_LIGHT0, GL_DIFFUSE, light_solidColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_solidDiffuse );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_solidAmbient );
			gl::draw( *vbo );
		glPopMatrix();

		glLightfv( GL_LIGHT0, GL_POSITION, light_positionWire );

		glPushMatrix();
			glTranslatef(sphereOffset.x, sphereOffset.y,  sphereOffset.z); 
			glRotatef(sphereRotation.value().x, 1.0f, 0, 0); 
			glRotatef(sphereRotation.value().y, 0, 1.0f, 0); 
			glRotatef(sphereRotation.value().z, 0, 0, 1.0f); 
			glScalef(0.7f, 0.7f, 0.7f); 

		//draw wireframe
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
			glLightfv( GL_LIGHT0, GL_DIFFUSE, light_wireColor );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_wireDiffuse );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_wireAmbient );
			gl::draw( *vbo );
			
			glDisable( GL_LIGHTING );
			glDisable( GL_LIGHT0 );
			gl::disableDepthRead();
			gl::disableDepthWrite();

		//draw constellations

			if(mPrevConst) mPrevConst->render();
			if(mCurrentConst) mCurrentConst->render();
			
		glPopMatrix();

		// disable everything, return shit to normal
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDisable( GL_LIGHTING );
		glDisable( GL_LIGHT0 );
		glEnable(GL_TEXTURE_2D);
		gl::enableAlphaBlending();
		
		//draw particles
		drawParticles();

		fbo->unbindFramebuffer();
		

	//unbind FBO
	glPopMatrix();
}

void SphereView::drawParticles()
{
	if(mParticleShader)
	{
		glPushMatrix();
		{
			float fov = 60.0f; 
			float camZ = (size.value().y*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1;
	
			CameraPersp cam( 0, 0, fov );
			cam.setPerspective( 60.0f, size.value().x/size.value().y, 1.0f, mParticleDrawDistance );
		
			ci::Vec3f camPos = ci::Vec3f( -200.0f, -300.0f, camZ );
			ci::Vec3f camTarget = ci::Vec3f( 0, 0, 0 );
			ci::Vec3f camUp = ci::Vec3f( 0, 1.0f, 0 );

			cam.lookAt( camPos, camTarget, camUp);
			gl::setMatrices( cam );
		
			gl::enableAdditiveBlending();
			glColor4f(1.f, 1.f, 1.f, 1.f);

			mParticleShader->bind();
			//mParticleShader->uniform("tex", 0);

			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

			//set vertex arrays of my particles
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(4, GL_FLOAT, 0, mPositions);

			//set extra arrays using color
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, mParticleExtras);

			//uniform scale and rotation
			glTranslatef(sphereOffset.x + 45.f, sphereOffset.y, sphereOffset.z);
			glRotatef(mParticlesRotation.value().x, 1.0f, 0, 0); 
			glRotatef(mParticlesRotation.value().y, 0, 1.0f, 0); 
			glScalef(0.7f, 0.7f, 0.7f); 
			
			//draw first set
			glPushMatrix(); 
				
				glRotatef(mSet1RotVal, 0, 0, 1.0f); 
							
				glDrawArrays(GL_POINTS, 0, mNumParticles);
			
				glPopMatrix();

			//draw second set
			glPushMatrix();
				glTranslatef(0.f, 12.5f, 0.f); 
				glRotatef(mSet2RotVal, 0, 0, 1.0f); 
				glScalef(1.002f, 1.002f, 1.002f);

				glDrawArrays(GL_POINTS, 0, mNumParticles);

			glPopMatrix();

			//draw third set
			glPushMatrix();
				glRotatef(mSet3RotVal, 0, 0, 1.0f); 

				glDrawArrays(GL_POINTS, 0, mNumParticles);

			glPopMatrix();

			//draw third set
			glPushMatrix();
				glRotatef(mSet1RotVal, 0, 0, 1.0f); 

				glDrawArrays(GL_POINTS, 0, mNumParticles);

			glPopMatrix();
			
			//kill my client states
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

			mParticleShader->unbind();

			gl::enableAlphaBlending();
		}
		glPopMatrix();
	}
}

void SphereView::animateParticlesIn()
{
	mSet1RotVal = -9.f;
	mSet2RotVal = 54.f;
	mSet3RotVal = -60.f;

	mSpinSet1Speed = 0.025f;
	mSpinSet2Speed = 0.045f;
	mSpinSet3Speed = 0.03f;

	timeline().apply(&mParticleAlpha, 0.f, 1.f, .35f )
		.finishFn(boost::bind(&SphereView::onAnimateParticlesInComplete, this));

	timeline().apply(&mSet1Rot, 1.05f, 0.f, 4.f, EaseOutCubic());
	timeline().apply(&mSet2Rot, 1.055f, 0.f, 4.f, EaseOutCubic());
	timeline().apply(&mSet3Rot, 1.045f, 0.f, 4.f, EaseOutCubic())
		.updateFn(boost::bind(&SphereView::onAnimateParticlesInUpdate, this));
}

void SphereView::onAnimateParticlesInUpdate()
{
	mSet1RotVal += mSet1Rot.value();
	mSet2RotVal += mSet2Rot.value();
	mSet2RotVal += mSet3Rot.value();
}

void SphereView::onAnimateParticlesInComplete()
{
	
}

void SphereView::animateParticlesOut()
{
	mSpinSet1Speed = 0.f;
	mSpinSet2Speed = 0.f;
	mSpinSet3Speed = 0.f;
	mParticleAlpha = 1.f;

	for(int i = 0; i < mNumParticles; ++i)
	{
		mParticles->at(i)->setVelocity(ci::Vec4f(0.f, 0.f, -Rand::randFloat() * 0.5f, 0.f));
		mParticles->at(i)->setPosUpdateVal(-Rand::randFloat() * 0.25f);
	}

	timeline().apply(&mParticleYOffset, 0.f, 0.35f)
		.finishFn(boost::bind(&SphereView::onSwitchParticleDirection, this));
}

void SphereView::onSwitchParticleDirection()
{
	for(int i = 0; i < mNumParticles; ++i)
	{
		mParticles->at(i)->setPosUpdateVal(Rand::randFloat() + 0.35);
	}

	timeline().apply(&mParticleAlpha, 1.f, 0.f, .35f )
		.delay( .75f )
		.finishFn(boost::bind(&SphereView::onAnimateParticlesOutComplete, this));
}

void SphereView::onAnimateParticlesOutComplete()
{
	for(int i = 0; i < mNumParticles; ++i)
	{
		mParticles->at(i)->reset();
	}
}


void SphereView::render()
{
if(alpha->value() <= 0) return; 

	gl::enableAdditiveBlending(); 

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z ); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x, scale.value().y, 0);
		
		innerSphere->render(); 

	glPopMatrix(); 

	glPushMatrix(); 
		glTranslatef(position.value().x, position.value().y, position.value().z); 
		glRotatef(rotation.value().x, 1.0f, 0, 0);
		glRotatef(rotation.value().y, 0, 1.0f, 0);
		glRotatef(rotation.value().z, 0, 0, 1.0f);
		glScalef(scale.value().x * size.value().x, scale.value().y * size.value().y, scale.value().z * size.value().z);
		
		const ci::gl::Texture fbotex = fbo->getTexture();
		//draw sphere normally
		fbotex.enableAndBind(); 
		glColor4f(1.0f, 1.0f, 1.0f, alpha->value());
		drawQuad();

		//draw sphere with bloom shader
		bloomShader->bind();
		bloomShader->uniform( "alpha", alpha->value() );
		drawQuad();
		bloomShader->unbind(); 
		fbotex.unbind(); 
	
	glPopMatrix(); 

	
	gl::enableAlphaBlending(); 
}

void SphereView::draw()
{

}

void SphereView::drawQuad()
{
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);    glVertex2f( 0-center.x, 0-center.y);
		glTexCoord2f(1, 0);    glVertex2f( 1-center.x, 0-center.y);
		glTexCoord2f(1, 1);    glVertex2f( 1-center.x, 1-center.y);
		glTexCoord2f(0, 1);    glVertex2f( 0-center.x, 1-center.y);
	glEnd();
}


void SphereView::swapCenterShader(ci::gl::GlslProgRef shader)
{
	centerShader = shader;
}

void SphereView::swapBlurShader(ci::gl::GlslProgRef shader) 
{
	bloomShader = shader;
}

void SphereView::swapRingShader(ci::gl::GlslProgRef shader) 
{
	ringShader = shader;
}




//particles
HBSphereParticle::HBSphereParticle(ci::Vec4f startingPos) :
	mStartingPosition(startingPos), mPosition(startingPos), mVelocity(ci::Vec4f()),
	mSeedUpdateVal(0.f), mPosUpdateVal(0.f)
{
	reset();
}

HBSphereParticle::~HBSphereParticle(void)
{

}

void HBSphereParticle::reset()
{
	mPosUpdateVal = 0.f;
	mVelocity = ci::Vec4f();

	mPosition = mStartingPosition;
	mPosition.x += Rand::randFloat() * 5.f - 2.5f;
	mPosition.y += Rand::randFloat() * 10.f - 5.f;
	mPosition.z += Rand::randFloat() * 5.f - 2.5f;
	mPosition.w = Rand::randFloat();

	mSeedUpdateVal = Rand::randFloat() * 0.05;
	if(mSeedUpdateVal < 0.03) mSeedUpdateVal = 0.03;
}

void HBSphereParticle::update()
{
	//udpate z bit by bit for acceleration
	mVelocity.z += mPosUpdateVal;

	//update manual seed
	mPosition.w += mSeedUpdateVal;
	
	//update overall position
	mPosition += mVelocity;
}