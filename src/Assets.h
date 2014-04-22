#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "cinder/Font.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/Json.h"
#include "DataController.h"

//using namespace ci;
//using namespace ci::app;
//using namespace ci::gl;
//using namespace ci::qtime;
//using namespace std;

class Assets
{
	//normal Assets class stuff
	void createAssetIds();

public:
	Assets(void);
	~Assets(void);

	void addVideo(const std::string &id, ci::qtime::MovieGl* asset);
	void addVideoSurface(const std::string &id, ci::qtime::MovieSurface* asset);
	void addTexture(const std::string &id, ci::gl::Texture* asset); 
	void addFont(const std::string &id, ci::Font* asset);
	void addShader(const std::string &id, ci::gl::GlslProgRef asset); 
	void addVBO(const std::string &id, const std::string &path); 
		
	ci::gl::Texture* getTextureByID(const std::string &id); 
    ci::qtime::MovieGl* getVideoByID(const std::string &id);
	ci::qtime::MovieSurface* getVideoSurfaceByID(const std::string &id);
	ci::Font* getFontByID(const std::string &id); 
	
	ci::gl::GlslProgRef getShaderByID(const std::string &id);
	ci::gl::VboMesh* getVBOByID(const std::string &id); 
	ci::TriMesh* getMeshByID(const std::string &id);

	ci::gl::Texture* getNewTexture(const std::string &path);

	DataController* getData(); 
	
	void disposeTextures(); 
	void disposeVideos(); 
	
	struct Constellation {
		ci::gl::VboMesh* mesh;
		ci::TriMesh* outline;
		ci::Vec3f rotation; 

		Constellation(ci::gl::VboMesh* mesh, ci::TriMesh* outline, ci::Vec3f rotation) 
			: mesh(mesh), outline(outline), rotation(rotation) {}
	};
	
	std::vector<Constellation> getConstellations() { return constellations; };  
	std::vector<ci::TriMesh*>* getLightBeamMeshes() { return &lightBeams; }; 

protected:
	std::map< std::string, ci::gl::Texture* > textures; 
	std::map< std::string, ci::qtime::MovieGl* > videos;
	std::map< std::string, ci::qtime::MovieSurface* > videoSurfaces; 
	std::map< std::string, ci::Font* > fonts; 
	std::map< std::string, ci::gl::GlslProgRef > shaders; 
	std::map< std::string, ci::gl::VboMesh* > vbos;
	std::map< std::string, ci::TriMesh* > meshs;
	std::vector<Constellation> constellations;
	std::vector<ci::TriMesh*> lightBeams;

	DataController* data; 

	//specific Asset loading helper functions/enums/structs
public:
	enum FileType
	{
		VIDEO, TEXTURE, FONT, SHADER, JSON
	};
	
	enum FileID
	{
		MAIN_VIDEO, PAUSE_VIDEO,
		FPO,
		
		//backgrounds
		LOWER_BEAM,

		//textures
		SELECTION,
		BUTTON_BG, 
		BUG, 
		HOST_ICON, HOST_ICON_BG, HOST_CTA, 
		PAUSE_BEAM, PAUSE_TIMELINE_BG, PAUSE_TIMELINE_CTA,
		DATA_ARROW, 
		BUTTON_LB, BUTTON_RB,
		PARTICLE,

		//fonts
		DEFAULT_FONT,
		START_MENU_FONT,

		//shaders 
		ROTATE_SHADER
	};

	struct Asset 
	{
		std::string id;
		std::string filename;
		FileType type;

		Asset(std::string id, std::string fn, FileType ft) : id(id), filename(fn), type(ft) 
		{ 
		}

		Asset(void)
		{
		}
	};


	void createAsset(FileID id, Asset a);
	

	std::string getId(FileID id)
	{
		return asset_ids[id].id;
	}

	std::string getFilename(FileID id)
	{
		return asset_ids[id].filename;
	}
	
	ci::Font* getFontByID(const FileID &id); 



protected:		
	std::map< FileID, Asset > asset_ids;
	
};
