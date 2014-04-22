#include "Assets.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace ci::qtime;
using namespace std;

Assets::Assets()
{	
	console() << "Assets() loading all assets!" << endl;

	double startTime = getElapsedSeconds();
    
    addAssetDirectory("/Users/anthony.tripaldi/Documents/Development/HALO_ARGOS_2700/trunk/dev/ARGOS_Prototype/assets");

	data = new DataController(JsonTree());

	createAssetIds();
	
	console() << "Assets() completed asset loading! elapsed time: " << getElapsedSeconds() - startTime << endl;
}

void Assets::createAssetIds()
{
	createAsset(MAIN_VIDEO, Asset("main_video", "video.mov", VIDEO) );
	createAsset(PAUSE_VIDEO, Asset("pause_video", "thumbs.mp4", VIDEO) );
	createAsset(FPO, Asset("fpo", "fpo.png", TEXTURE) );
	createAsset(LOWER_BEAM, Asset("lower_beam", "bgs/lower_bg_beam.png", TEXTURE) );
	createAsset(BUTTON_BG, Asset("buttonBG", "buttonBG.png", TEXTURE) );
	createAsset(HOST_ICON, Asset("host_icon", "scenes/host/host_icon.png", TEXTURE) );
	createAsset(HOST_ICON_BG, Asset("host_iconbg", "scenes/host/host_iconbg.png", TEXTURE) );
	createAsset(HOST_CTA, Asset("host_cta", "scenes/host/host_cta.png", TEXTURE) );
	createAsset(PAUSE_BEAM, Asset("pause_beam", "pause_beam.png", TEXTURE) );
	createAsset(PAUSE_TIMELINE_BG, Asset("pause_timeline_bg", "pause_timeline_bg.png", TEXTURE) );
	createAsset(PAUSE_TIMELINE_CTA, Asset("pause_timelineCTA", "pause_timelineCTA.png", TEXTURE) );
	createAsset(SELECTION, Asset("selection", "selection.png", TEXTURE) );
	createAsset(PARTICLE, Asset("particle", "particle.png", TEXTURE) );
	createAsset(BUG, Asset("bug", "bug/bug.png", TEXTURE) );
	createAsset(DATA_ARROW, Asset("data_arrow", "scenes/data/data_arrow.png", TEXTURE) );
	createAsset(BUTTON_LB, Asset("button_lb", "buttons/button_lb.png", TEXTURE) );
	createAsset(BUTTON_RB, Asset("button_rb", "buttons/button_rb.png", TEXTURE) );

	createAsset(DEFAULT_FONT, Asset("default", "fonts/QTypePro-CondLight.otf", FONT) );

    
    
    // SHADERS!?!
    try {
        
        addShader("loc_sphere", GlslProg::create(loadAsset("shaders/loc_sphere.vert"), loadAsset("shaders/loc_sphere.frag") )); console() << "loc_sphere" << endl;
        addShader("loc_sphere_particle", GlslProg::create(loadAsset("shaders/loc_sphere_particle.vert"), loadAsset("shaders/loc_sphere_particle.frag") )); console() << "loc_sphere_particle" << endl;
        addShader("loc_planet", GlslProg::create(loadAsset("shaders/loc_planet.vert"), loadAsset("shaders/loc_planet.frag") )); console() << "loc_planet" << endl;
        
        addShader("bloom", GlslProg::create(loadAsset("shaders/bloom.vert"), loadAsset("shaders/bloom.frag") ));  console() << "bloom" << endl;
        addShader("blur", GlslProg::create(loadAsset("shaders/blur.vert"), loadAsset("shaders/blur.frag") ));  console() << "blur" << endl;
        addShader("rotate", GlslProg::create(loadAsset("shaders/rotate.vert"), loadAsset("shaders/rotate.frag") ));  console() << "rotate" << endl;
        addShader("rotateSimple", GlslProg::create(loadAsset("shaders/rotateSimple.vert"), loadAsset("shaders/rotateSimple.frag") )); console() << "rotateSimple" << endl;

        addShader("data", GlslProg::create(loadAsset("shaders/data.vert"), loadAsset("shaders/data.frag") ));console() << "data" << endl;

        addShader("datadust", GlslProg::create(loadAsset("shaders/datadust.vert"), loadAsset("shaders/datadust.frag") ));console() << "datadust" << endl;
        addShader("datadusthead", GlslProg::create(loadAsset("shaders/datadust.vert"), loadAsset("shaders/datadusthead.frag") )); console() << "datadusthead" << endl;
        addShader("main_particles", GlslProg::create(loadAsset("shaders/particle.vert"), loadAsset("shaders/particle.frag") )); console() << "mainParticles" << endl;
        addShader("small_particles", GlslProg::create(loadAsset("shaders/small_particle.vert"), loadAsset("shaders/small_particle.frag") )); console() << "smallParticles" << endl;
        addShader("loc_big_particle", GlslProg::create(loadAsset("shaders/loc_big_particle.vert"), loadAsset("shaders/loc_big_particle.frag") )); console() << "loc_big_particles" << endl;
        
        addShader("data_item_blur", GlslProg::create(loadAsset("shaders/data_item_blur.vert"), loadAsset("shaders/data_item_blur.frag") )); console() << "data_item_blur" << endl;
        addShader("video_blur", GlslProg::create(loadAsset("shaders/video_blur.vert"), loadAsset("shaders/video_blur.frag") )); console() << "video_blur" << endl;
        
        addShader("data_floor", GlslProg::create(loadAsset("shaders/data_floor.vert"), loadAsset("shaders/data_floor.frag") )); console() << "data_floor" << endl;
        
        addShader( "hb_floor", GlslProg::create(loadAsset("shaders/hb_floor.vert"), loadAsset("shaders/hb_floor.frag") ) ); console() << "hb_floor" << endl;
        addShader( "hb_floorVbo", GlslProg::create(loadAsset("shaders/hb_floorVbo.vert"), loadAsset("shaders/hb_floorVbo.frag") ) ); console() << "hb_floor_Vbo" << endl;
        addShader( "hb_floorVert", GlslProg::create(loadAsset("shaders/hb_vertParticle.vert"), loadAsset("shaders/hb_vertParticle.frag") ) ); console() << "hb_floor_vert" << endl;
        addShader( "hb_floatParticle", GlslProg::create(loadAsset("shaders/hb_floatParticle.vert"), loadAsset("shaders/hb_floatParticle.frag") ) ); console() << "hb_float_particle" << endl;
        
        addShader("handbook_sphere_blur", GlslProg::create(loadAsset("scenes/handbook/sphere/blur.vert"), loadAsset("scenes/handbook/sphere/blur.frag") )); console() << "sphere_blur" << endl;
        addShader("handbook_sphere_ring", GlslProg::create(loadAsset("scenes/handbook/sphere/ring.vert"), loadAsset("scenes/handbook/sphere/ring.frag") )); console() << "sphere_ring" << endl;
        addShader("handbook_sphere_sphere", GlslProg::create(loadAsset("scenes/handbook/sphere/sphere.vert"), loadAsset("scenes/handbook/sphere/sphere.frag") )); console() << "sphere_sphere" << endl;
        addShader("handbook_sphere_particles",  GlslProg::create( loadAsset( "shaders/sphere_particle.vert" ), loadAsset( "shaders/sphere_particle.frag" ) )); console() << "sphere_particles" << endl;
	}
    catch ( const GlslProgCompileExc &e ) {
        console() << "Shader exception: " << e.what() << std::endl;
    }
    
    
//background glyphs
	addTexture( "main_bg_glyph1", new Texture( loadImage(loadAsset( "bgs/glyphs/g1.png" ))));
	addTexture( "main_bg_glyph2", new Texture( loadImage(loadAsset( "bgs/glyphs/g2.png" ))));
	addTexture( "main_bg_glyph3", new Texture( loadImage(loadAsset( "bgs/glyphs/g3.png" ))));
	addTexture( "main_bg_glyph4", new Texture( loadImage(loadAsset( "bgs/glyphs/g4.png" ))));
	addTexture( "main_bg_glyph5", new Texture( loadImage(loadAsset( "bgs/glyphs/g5.png" ))));
	addTexture( "main_bg_glyph6", new Texture( loadImage(loadAsset( "bgs/glyphs/g6.png" ))));
	addTexture( "main_bg_glyph7", new Texture( loadImage(loadAsset( "bgs/glyphs/g7.png" ))));
	addTexture( "main_bg_glyph8", new Texture( loadImage(loadAsset( "bgs/glyphs/g8.png" ))));

//start page
	addVideo("start_intro", new MovieGl( loadAsset("scenes/start/start_intro.mov") ));
	
	//beacon
	addTexture("beacon_tab_bg_on", new Texture( loadImage( loadAsset( "scenes/beacon/tab_bg_on.png" ) ) ) );
	addTexture("beacon_tab_bg_off", new Texture( loadImage( loadAsset( "scenes/beacon/tab_bg_off.png" ) ) ) );
	
	addTexture("received_bg_on", new Texture( loadImage( loadAsset( "scenes/beacon/received_bg_on.png" ) ) ) );
	addTexture("received_bg_off", new Texture( loadImage( loadAsset( "scenes/beacon/received_bg_off.png" ) ) ) );
	addTexture("received_sub_bg", new Texture( loadImage( loadAsset( "scenes/beacon/received_sub_bg.png" ) ) ) );

	addTexture("beacon_icon_off_1", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i1_off.png" ) ) ));
	addTexture("beacon_icon_on_1", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i1_on.png" ) ) ));
	addTexture("beacon_icon_off_2", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i2_off.png" ) ) ));
	addTexture("beacon_icon_on_2", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i2_on.png" ) ) ));
	addTexture("beacon_icon_off_3", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i3_off.png" ) ) ));
	addTexture("beacon_icon_on_3", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i3_on.png" ) ) ));
	addTexture("beacon_icon_off_4", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i4_off.png" ) ) ));
	addTexture("beacon_icon_on_4", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i4_on.png" ) ) ));
	addTexture("beacon_icon_off_5", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i5_off.png" ) ) ));
	addTexture("beacon_icon_on_5", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i5_on.png" ) ) ));
	addTexture("beacon_icon_off_6", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i6_off.png" ) ) ));
	addTexture("beacon_icon_on_6", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i6_on.png" ) ) ));
	addTexture("beacon_icon_off_7", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i7_off.png" ) ) ));
	addTexture("beacon_icon_on_7", new Texture( loadImage( loadAsset( "scenes/beacon/icons/i7_on.png" ) ) ));

//location
	addTexture("loc_bg_nsew", new Texture( loadImage( loadAsset( "scenes/location/bg/nsew.png" ) ) ));
	addTexture("loc_bg_inner_circle", new Texture( loadImage( loadAsset( "scenes/location/bg/inner_circle.png" ) ) ));
	addTexture("loc_bg_inner_shapes", new Texture( loadImage( loadAsset( "scenes/location/bg/inner_shapes.png" ) ) ));
	addTexture("loc_bg_outer_circle", new Texture( loadImage( loadAsset( "scenes/location/bg/outer_circle.png" ) ) ));
	addTexture("loc_bg_static_circles", new Texture( loadImage( loadAsset( "scenes/location/bg/static_circles.png" ) ) ));
	
	addTexture("loc_local_controls", new Texture( loadImage( loadAsset( "scenes/location/controls.png" ) ) ));
	addTexture("loc_local_bg", new Texture( loadImage( loadAsset( "scenes/location/bg.png" ) ) ) );

	addTexture("loc_planet_alpha", new Texture( loadImage( loadAsset( "scenes/location/planet_alpha.png" ) ) ));
	addTexture("loc_planet_noise", new Texture( loadImage( loadAsset( "scenes/location/planet_noise.png" ) ) ));
	addTexture("loc_orbital_light", new Texture( loadImage( loadAsset( "scenes/location/orbital_light.png" ) ) ));
	addTexture("loc_sphere_particle_tex", new Texture( loadImage( loadAsset( "scenes/location/sphere_particle.png" ) ) ));
	
	addVBO("loc_sphere", "scenes/location/sphere.obj");
	addVideo("loc_orbital_aerial", new MovieGl( loadAsset("scenes/location/transitions/orbital-aerial.mp4")));
	addVideo("loc_aerial_local", new MovieGl( loadAsset("scenes/location/transitions/aerial-local.mp4")));
	addVideo("loc_hb_orbital", new MovieGl( loadAsset("scenes/location/transitions/hb_orbital.mp4")));
	//addVideo("loc_loop_aerial", new MovieGl( loadAsset("scenes/location/loops/aerial.mp4")));


//end screen
	//addVideo("end_video_loop", new MovieGl( loadAsset("scenes/endscreen/endmenu.mp4")));
	addTexture("end_next_episode_thumb", new Texture( loadImage( loadAsset( "scenes/endscreen/next_episode_thumb.png" ) ) ) );
	addTexture("end_next_episode_btn_bg", new Texture( loadImage( loadAsset( "scenes/endscreen/next_episode_btn_bg.png" ) ) ) );
	addTexture("end_share_btn_bg", new Texture( loadImage( loadAsset( "scenes/endscreen/share_btn_bg.png" ) ) ) );
	addTexture("end_share_btn_bg_highlight", new Texture( loadImage( loadAsset( "scenes/endscreen/share_btn_bg_highlight.png" ) ) ) );
	addTexture("end_share_title", new Texture( loadImage( loadAsset( "scenes/endscreen/share_title.png" ) ) ) );


//engage
	//addVideo("eng_oni_fail", new MovieGl( loadAsset("scenes/engage/ONI_fail.mp4")));
	addTexture("eng_fail_bg", new Texture( loadImage( loadAsset( "scenes/engage/fail_bg.png" ) ) ) );

	//addVideo("eng_enter", new MovieGl( loadAsset("scenes/engage/enter.mp4")));
	addTexture("eng_enter_bg", new Texture( loadImage( loadAsset( "scenes/engage/enter_bg.png" ) ) ) );
	
	addTexture("eng_bg", new Texture( loadImage( loadAsset( "scenes/engage/bg.png" ) ) ) );
	//addTexture("eng_start_text", new Texture( loadImage( loadAsset( "scenes/engage/start_text.png" ) ) ) );
	//addTexture("eng_keypad_outline", new Texture( loadImage( loadAsset( "scenes/engage/keypad_outline.png" ) ) ) );
	addTexture("eng_keypad_border", new Texture( loadImage( loadAsset( "scenes/engage/keypad_border.png" ) ) ) );
	addTexture("eng_keypad_dashes", new Texture( loadImage( loadAsset( "scenes/engage/keypad_dashes.png" ) ) ) );

	addTexture("eng_keypad_circle", new Texture( loadImage( loadAsset( "scenes/engage/keypad_circle.png" ) ) ) );
	addTexture("eng_keypad_circle_glow", new Texture( loadImage( loadAsset( "scenes/engage/keypad_circle_glow.png" ) ) ) );
	addTexture("eng_keypad_1", new Texture( loadImage( loadAsset( "scenes/engage/keypad_1.png" ) ) ) );
	addTexture("eng_keypad_2", new Texture( loadImage( loadAsset( "scenes/engage/keypad_2.png" ) ) ) );
	addTexture("eng_keypad_3", new Texture( loadImage( loadAsset( "scenes/engage/keypad_3.png" ) ) ) );
	addTexture("eng_keypad_4", new Texture( loadImage( loadAsset( "scenes/engage/keypad_4.png" ) ) ) );
	addTexture("eng_keypad_5", new Texture( loadImage( loadAsset( "scenes/engage/keypad_5.png" ) ) ) );
	addTexture("eng_keypad_6", new Texture( loadImage( loadAsset( "scenes/engage/keypad_6.png" ) ) ) );
	addTexture("eng_keypad_arrow", new Texture( loadImage( loadAsset( "scenes/engage/keypad_arrow.png" ) ) ) );
	addTexture("eng_oni_empty", new Texture( loadImage( loadAsset( "scenes/engage/oni_missing.png" ) ) ) );
	addTexture("eng_oni_1", new Texture( loadImage( loadAsset( "scenes/engage/oni_1.png" ) ) ) );
	addTexture("eng_oni_2", new Texture( loadImage( loadAsset( "scenes/engage/oni_2.png" ) ) ) );
	addTexture("eng_oni_3", new Texture( loadImage( loadAsset( "scenes/engage/oni_3.png" ) ) ) );

//Quick and dirty assets... no ENUM IDs
	addTexture("select_back", new Texture( loadImage( loadAsset( "select_back.png" ) ) ) );
	addTexture("add_back", new Texture( loadImage( loadAsset( "add_back.png" ) ) ) );
	addTexture("back", new Texture( loadImage( loadAsset( "back.png" ) ) ) );

	addTexture( "main_bg_glow", new Texture( loadImage(loadAsset( "bgs/right_beam.png" ))));
	addTexture( "main_bg_circle", new Texture( loadImage(loadAsset( "bgs/main_bg_circle.png" ))));
	addTexture( "main_bg_outer_circle", new Texture( loadImage(loadAsset( "bgs/main_bg_outer_circle.png" ))));
	addTexture( "main_bg_particle_tex", new Texture( loadImage( loadAsset( "particles/main_bg_particle_blur.png" ))));

// host	
	addTexture("host_spinner", new Texture( loadImage( loadAsset( "scenes/host/host_spinner.png" ) ) ) );
	addTexture("host_icon2", new Texture( loadImage( loadAsset( "scenes/host/host_icon2.png" ) ) ) );
	addTexture("host_icon3", new Texture( loadImage( loadAsset( "scenes/host/host_icon3.png" ) ) ) );
	addTexture("host_icon4", new Texture( loadImage( loadAsset( "scenes/host/host_icon4.png" ) ) ) );
	addTexture("host_icon5", new Texture( loadImage( loadAsset( "scenes/host/host_icon5.png" ) ) ) );
	addTexture("host_icon6", new Texture( loadImage( loadAsset( "scenes/host/host_icon6.png" ) ) ) );
	addTexture("host_icon7", new Texture( loadImage( loadAsset( "scenes/host/host_icon7.png" ) ) ) );
	addTexture("host_icon8", new Texture( loadImage( loadAsset( "scenes/host/host_icon8.png" ) ) ) );
	addTexture("host_icon9", new Texture( loadImage( loadAsset( "scenes/host/host_icon9.png" ) ) ) );

// misc data view assets. 
	addTexture("card_bars", new Texture( loadImage(loadAsset("scenes/data/card_bars.png"))) );
	addTexture("data_dust", new Texture( loadImage(loadAsset("scenes/data/data_dust.png"))) );
	addTexture("data_waypoint_item", new Texture( loadImage(loadAsset("scenes/data/data_waypoint_item.png"))) );
	addTexture("data_item_arrow", new Texture( loadImage(loadAsset("scenes/data/data_item_arrow.png"))) );
	addTexture("data_item_arrow_glow", new Texture( loadImage(loadAsset("scenes/data/data_item_arrow_glow.png"))) );
	addTexture("bgBox", new Texture( loadImage(loadAsset("bgBox.png"))) );
	addTexture("data_lightPulse", new Texture( loadImage(loadAsset("scenes/data/data_lightPulse.png"))) );
	addTexture("data_waypoint_buy", new Texture( loadImage(loadAsset("scenes/data/data_waypoint_buy.png"))) );
	addTexture("data_itemTick", new Texture( loadImage(loadAsset("scenes/data/data_itemTick.png"))) );
	
	//addTexture("data_letter", new Texture( loadImage(loadAsset("scenes/data/data_letter.png"))) );
	addTexture("data_letter_small", new Texture( loadImage(loadAsset("scenes/data/data_letter_small.png"))) );
	
	//data floor
	addTexture("data_floor_overlay", new Texture( loadImage(loadAsset("scenes/data/floor_overlay.png"))) );
	

// oni file letters
	addTexture("data_oni_02_01_03_31_lasky_small", new Texture( loadImage(loadAsset("scenes/data/oni/data_oni_02_01_03_31_lasky_small.png"))) );
	addTexture("data_oni_02_01_03_31_mehaffey_small", new Texture( loadImage(loadAsset("scenes/data/oni/data_oni_02_01_03_31_mehaffey_small.png"))) );
	addTexture("data_oni_10_01_11_30_automated_extraction_small", new Texture( loadImage(loadAsset("scenes/data/oni/data_oni_10_01_11_30_automated_extraction_small.png"))) );
	addTexture("data_oni_10_01_11_30_dispatch_small", new Texture( loadImage(loadAsset("scenes/data/oni/data_oni_10_01_11_30_dispatch_small.png"))) );
	addTexture("data_oni_12_01_01_31_automated_extraction_small", new Texture( loadImage(loadAsset("scenes/data/oni/data_oni_12_01_01_31_automated_extraction_small.png"))) );

	addTexture("data_audio_thumb", new Texture( loadImage(loadAsset("scenes/data/data_audio_thumb.png"))) );
	

// data title icons
	addTexture("icon_title_cams", new Texture( loadImage(loadAsset("scenes/data/icon_title_cams.png"))) );
	addTexture("icon_title_cov", new Texture( loadImage(loadAsset("scenes/data/icon_title_cov.png"))) );
	addTexture("icon_title_unsc", new Texture( loadImage(loadAsset("scenes/data/icon_title_unsc.png"))) );
	addTexture("icon_title_oni", new Texture( loadImage(loadAsset("scenes/data/icon_title_oni.png"))) );
	addTexture("icon_title_spart", new Texture( loadImage(loadAsset("scenes/data/icon_title_spart.png"))) );
	
// data list + waypoint icons
	addTexture("data_icon_video", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_video.png"))) );
	addTexture("data_icon_document", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_document.png"))) );
	addTexture("data_icon_object3D", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_object3D.png"))) );
	addTexture("data_icon_audio", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_audio.png"))) );
	
	addTexture("data_icon_figure", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_figure.png"))) );
	addTexture("data_icon_clothes", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_clothes.png"))) );
	addTexture("data_icon_book", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_book.png"))) );
	addTexture("data_icon_ost", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_ost.png"))) );
	addTexture("data_icon_helmet", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_helmet.png"))) );
	addTexture("data_icon_dogtag", new Texture( loadImage(loadAsset("scenes/data/icon/data_icon_dogtag.png"))) );

// Pause 
	addTexture("pause_glow", new Texture( loadImage(loadAsset("pause/pause_glow.png"))) );
	addTexture("pause_icon_data", new Texture( loadImage(loadAsset("pause/pause_icon_data.png"))) );
	addTexture("pause_icon_oni", new Texture( loadImage(loadAsset("pause/pause_icon_oni.png"))) );
	addTexture("pause_timeline_bar", new Texture( loadImage(loadAsset("pause/pause_timeline_bar.png"))) );
	addTexture("pause_icon_social_1", new Texture( loadImage(loadAsset("pause/pause_icon_social_1.png"))) );
	addTexture("pause_icon_social_2", new Texture( loadImage(loadAsset("pause/pause_icon_social_2.png"))) );
	addTexture("pause_icon_social_3", new Texture( loadImage(loadAsset("pause/pause_icon_social_3.png"))) );
	addTexture("pause_icon_social_4", new Texture( loadImage(loadAsset("pause/pause_icon_social_4.png"))) );
	addTexture("pause_icon_social_5", new Texture( loadImage(loadAsset("pause/pause_icon_social_5.png"))) );
	addTexture("pause_icon_social_6", new Texture( loadImage(loadAsset("pause/pause_icon_social_6.png"))) );
	addTexture("pause_icon_social_7", new Texture( loadImage(loadAsset("pause/pause_icon_social_7.png"))) );

//Rotation Shit
	//rotation videos are no longer loaded up-front. they are loaded and disposed of in 'RotationView'
	
//oni toasts!
	addVideo("ONI_toast", new MovieGl( loadAsset("toast/ONI_toast.mp4") ) );
	//addVideo("ONI_fail", new MovieGl( loadAsset("toast/ONI_fail.mp4") ) );

	addTexture("toast_textBG", new Texture( loadImage(loadAsset("toast/toast_textBG.png"))) );
	addTexture("toast_glow", new Texture( loadImage(loadAsset("toast/toast_glow.png"))) );

//videos!
	addTexture("data_video_APRIL", new Texture( loadImage(loadAsset("scenes/data/video/data_video_APRIL.png"))) );
	addTexture("data_video_CADMON_1", new Texture( loadImage(loadAsset("scenes/data/video/data_video_CADMON_1.png"))) );
	addTexture("data_video_CADMON_2", new Texture( loadImage(loadAsset("scenes/data/video/data_video_CADMON_2.png"))) );
	addTexture("data_video_CHYLER", new Texture( loadImage(loadAsset("scenes/data/video/data_video_CHYLER.png"))) );
	addTexture("data_video_COL_LASKY", new Texture( loadImage(loadAsset("scenes/data/video/data_video_COL_LASKY.png"))) );
	addTexture("data_video_DIMAH", new Texture( loadImage(loadAsset("scenes/data/video/data_video_DIMAH.png"))) );
	addTexture("data_video_JJ", new Texture( loadImage(loadAsset("scenes/data/video/data_video_JJ.png"))) );
	addTexture("data_video_LASKY", new Texture( loadImage(loadAsset("scenes/data/video/data_video_LASKY.png"))) );
	addTexture("data_video_SULLY", new Texture( loadImage(loadAsset("scenes/data/video/data_video_SULLY.png"))) );
	addTexture("data_video_VICKERS", new Texture( loadImage(loadAsset("scenes/data/video/data_video_VICKERS.png"))) );
	addTexture("data_video_ONI", new Texture( loadImage(loadAsset("scenes/data/video/data_video_ONI.png"))) );

	addTexture("items_Cytoprethaline_DATA_video", new Texture( loadImage(loadAsset("scenes/data/video/items_Cytoprethaline_DATA_video.png"))) );
	addTexture("oni_armor_unlock", new Texture( loadImage(loadAsset("scenes/data/video/oni_armor_unlock.png"))) );
	//IMPORTANT!
	//video files are no longer loaded up-front. they are loaded and disposed of in 'DataViewMovie'

//heads!
	addTexture("data_CADMON", new Texture( loadImage(loadAsset("scenes/data/overview/data_CADMON.png"))) );
	addTexture("data_CHEN", new Texture( loadImage(loadAsset("scenes/data/overview/data_CHEN.png"))) );
	addTexture("data_CHIEF", new Texture( loadImage(loadAsset("scenes/data/overview/data_CHIEF.png"))) );
	addTexture("data_COL_LASKY", new Texture( loadImage(loadAsset("scenes/data/overview/data_COL_LASKY.png"))) );
	addTexture("data_Corvette", new Texture( loadImage(loadAsset("scenes/data/overview/data_Corvette.png"))) );
	addTexture("data_Cytoprethaline", new Texture( loadImage(loadAsset("scenes/data/overview/data_Cytoprethaline.png"))) );
	addTexture("data_DIMAH", new Texture( loadImage(loadAsset("scenes/data/overview/data_DIMAH.png"))) );
	addTexture("data_Elite_Zealot", new Texture( loadImage(loadAsset("scenes/data/overview/data_Elite_Zealot.png"))) );
	addTexture("data_EnergySword", new Texture( loadImage(loadAsset("scenes/data/overview/data_EnergySword.png"))) );
	addTexture("data_FRED", new Texture( loadImage(loadAsset("scenes/data/overview/data_FRED.png"))) );
	addTexture("data_HUGHES", new Texture( loadImage(loadAsset("scenes/data/overview/data_HUGHES.png"))) );
	addTexture("data_LASKY", new Texture( loadImage(loadAsset("scenes/data/overview/data_LASKY.png"))) );
	addTexture("data_MasterChief", new Texture( loadImage(loadAsset("scenes/data/overview/data_MasterChief.png"))) );
	addTexture("data_MEHAFFEY", new Texture( loadImage(loadAsset("scenes/data/overview/data_MEHAFFEY.png"))) );
	addTexture("data_Needler", new Texture( loadImage(loadAsset("scenes/data/overview/data_Needler.png"))) );
	addTexture("data_ODST", new Texture( loadImage(loadAsset("scenes/data/overview/data_ODST.png"))) );
	addTexture("data_ORENSKI", new Texture( loadImage(loadAsset("scenes/data/overview/data_ORENSKI.png"))) );
	addTexture("data_Rifles", new Texture( loadImage(loadAsset("scenes/data/overview/data_Rifles.png"))) );
	addTexture("data_SILVA", new Texture( loadImage(loadAsset("scenes/data/overview/data_SILVA.png"))) );
	addTexture("data_SOEIV_Pod", new Texture( loadImage(loadAsset("scenes/data/overview/data_SOEIV_Pod.png"))) );
	addTexture("data_SpaceTether", new Texture( loadImage(loadAsset("scenes/data/overview/data_SpaceTether.png"))) );
	addTexture("data_Spartan", new Texture( loadImage(loadAsset("scenes/data/overview/data_Spartan.png"))) );
	addTexture("data_SPARTAN_II", new Texture( loadImage(loadAsset("scenes/data/overview/data_SPARTAN_II.png"))) );
	addTexture("data_SULLY", new Texture( loadImage(loadAsset("scenes/data/overview/data_SULLY.png"))) );
	addTexture("data_VICKERS", new Texture( loadImage(loadAsset("scenes/data/overview/data_VICKERS.png"))) );
	addTexture("data_Warthog", new Texture( loadImage(loadAsset("scenes/data/overview/data_Warthog.png"))) );
	addTexture("data_UNSC", new Texture( loadImage(loadAsset("scenes/data/overview/data_UNSC.png"))) );
	addTexture("data_ONI", new Texture( loadImage(loadAsset("scenes/data/overview/data_ONI.png"))) );
	addTexture("data_HASTATI", new Texture( loadImage(loadAsset("scenes/data/overview/data_HASTATI.png"))) );

	addTexture("data_letter_head", new Texture( loadImage(loadAsset("scenes/data/overview/data_letter_head.png"))) );
	addTexture("data_video_ONI_head", new Texture( loadImage(loadAsset("scenes/data/overview/data_video_ONI_head.png"))) );
	addTexture("data_oni_armor", new Texture( loadImage(loadAsset("scenes/data/overview/data_oni_armor.png"))) );

//waypoint! =_=
	addTexture("DATA_Waypoint_Avatar_Arbiter_Mask_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Arbiter_Mask_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Corbulo_Academy_T-Shirt_Green_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Corbulo_Academy_T-Shirt_Green_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Elite_Costume_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Elite_Costume_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Energy_Sword_Shirt_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Energy_Sword_Shirt_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Energy_Sword_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Energy_Sword_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Lasky_CAMS_Formal_Dress_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Lasky_CAMS_Formal_Dress_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Needler_Prop_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Needler_Prop_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_ODST_Drop_Pod_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_ODST_Drop_Pod_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_RC_Warthog_Prop_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_RC_Warthog_Prop_Still.png"))) );
	addTexture("DATA_Waypoint_Avatar_Warthog_Prop_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_Avatar_Warthog_Prop_Still.png"))) );
	addTexture("DATA_Waypoint_bag-halo-unscmessenger-front_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_bag-halo-unscmessenger-front_Still.png"))) );
	addTexture("DATA_Waypoint_book-halo-ghostsofonyx-angle_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_book-halo-ghostsofonyx-angle_Still.png"))) );
	addTexture("DATA_Waypoint_etc-costume-halo-deluxemasterchief-full_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_etc-costume-halo-deluxemasterchief-full_Still.png"))) );
	addTexture("DATA_Waypoint_etc-figure-halo-sqenix-masterchief-back_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_etc-figure-halo-sqenix-masterchief-back_Still.png"))) );
	addTexture("DATA_Waypoint_etc-figure-halo-sqenix-masterchief-full_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_etc-figure-halo-sqenix-masterchief-full_Still.png"))) );
	addTexture("DATA_Waypoint_etc-figure-halo-sqenix-redspartan-back_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_etc-figure-halo-sqenix-redspartan-back_Still.png"))) );
	addTexture("DATA_Waypoint_etc-figure-halo-sqenix-redspartan-kneel_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_etc-figure-halo-sqenix-redspartan-kneel_Still.png"))) );
	addTexture("DATA_Waypoint_hat-halo-unscsnapback-side_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_hat-halo-unscsnapback-side_Still.png"))) );
	addTexture("DATA_Waypoint_hat-halo-unscsnapback-top_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_hat-halo-unscsnapback-top_Still.png"))) );
	addTexture("DATA_Waypoint_hoodie-halo-unsc-COMBO_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_hoodie-halo-unsc-COMBO_Still.png"))) );
	addTexture("DATA_Waypoint_megablocks_cyrobay_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_megablocks_cyrobay_Still.png"))) );
	addTexture("DATA_Waypoint_other_droppod-rookie_packaging_01_dp_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_other_droppod-rookie_packaging_01_dp_Still.png"))) );
	addTexture("DATA_Waypoint_tee-halo-cams-COMBO_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_tee-halo-cams-COMBO_Still.png"))) );
	addTexture("DATA_Waypoint_tee-halo-imhere-flat_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_tee-halo-imhere-flat_Still.png"))) );
	addTexture("DATA_Waypoint_tee-halo-masterchiefillustration-flat_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_tee-halo-masterchiefillustration-flat_Still.png"))) );
	addTexture("DATA_Waypoint_tee-halo-misriah-flat_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_tee-halo-misriah-flat_Still.png"))) );
	addTexture("DATA_Waypoint_toy-halo-jda-banshee-packagefront_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_toy-halo-jda-banshee-packagefront_Still.png"))) );
	addTexture("DATA_Waypoint_toy-halo-jda-warthogce-packagefront_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_toy-halo-jda-warthogce-packagefront_Still.png"))) );
	addTexture("DATA_Waypoint_toy-halo-jda-warthog-packagefront_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_toy-halo-jda-warthog-packagefront_Still.png"))) );
	addTexture("DATA_Waypoint_toy-halo-mcf-s4cryotube-package_1_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_toy-halo-mcf-s4cryotube-package_1_Still.png"))) );
	addTexture("DATA_Waypoint_toy-halo-mcf-s4elitezealot-package_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_toy-halo-mcf-s4elitezealot-package_Still.png"))) );
	addTexture("DATA_Waypoint_toy-halo-mcf-s4masterchief-package_Still", new Texture( loadImage(loadAsset("scenes/data/waypoint/DATA_Waypoint_toy-halo-mcf-s4masterchief-package_Still.png"))) );


//bug!
	addTexture("bug_base", new Texture( loadImage(loadAsset("bug/bug_base.png"))) );
	addTexture("bug_arrow", new Texture( loadImage(loadAsset("bug/bug_arrow.png"))) );
	addTexture("bug_arrow_glow", new Texture( loadImage(loadAsset("bug/bug_arrow_glow.png"))) );
	addTexture("bug_bar", new Texture( loadImage(loadAsset("bug/bug_bar.png"))) );
	addTexture("bug_dot", new Texture( loadImage(loadAsset("bug/bug_dot.png"))) );

	addTexture("bug_bar_glow", new Texture( loadImage(loadAsset("bug/bug_bar_glow.png"))) );
	addTexture("bug_oni", new Texture( loadImage(loadAsset("bug/bug_oni.png"))) );
	addTexture("bug_oni_glow", new Texture( loadImage(loadAsset("bug/bug_oni_glow.png"))) );

	addTexture("bug_progress_base", new Texture( loadImage(loadAsset("bug/bug_progress_base.png"))) );
	addTexture("bug_progress_bg", new Texture( loadImage(loadAsset("bug/bug_progress_bg.png"))) );
	addTexture("bug_progress_glow", new Texture( loadImage(loadAsset("bug/bug_progress_glow.png"))) );
	addTexture("bug_progress_highlight", new Texture( loadImage(loadAsset("bug/bug_progress_highlight.png"))) );


//fonts!
	addFont("data_overview_head", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 50.0f) );
	addFont("data_overview_subhead", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 18.0f) );
	addFont("data_overview_body", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 24.0f) );

	addFont("data_icon_label", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 24.0f) );

	addFont("data_waypoint_head", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 24.0f) );
	addFont("data_waypoint_body", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 20.0f) );

	addFont("pause_timecode", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 30.0f) );
	addFont("pause_subtitle", new Font( loadAsset("fonts/QTypePro-CondLight.otf"), 20.0f) );
	addFont("pause_beacon", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 20.0f) );

	addFont("ARGOS_button", new Font( loadAsset("fonts/QTypePro-CondLight.otf"), 24.0f) );
	addFont("bug_progress", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 20.0f) );

	addFont("toast_text", new Font( loadAsset("fonts/QTypePro-CondLight.otf"), 30.0f) );
	addFont("handbook_listItem", new Font( loadAsset("fonts/QTypePro-CondLight.otf"), 20.0f) );
	addFont("handbook_listSubItem", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 24.0f) );
	addFont("handbook_tab", new Font( loadAsset("fonts/BlenderPro-Book.otf"), 16.0f) );
	
	addFont("location_map_area", getFontByID("pause_timecode") );


	//TODO: find a better solution for this? 
	string n; 
	for(int i=0; i<40; i++){
		n = ""; 
		if(i<10) n+= "00";
		else if(i<100) n+= "0";
		n += cinder::toString(i);
		addTexture("thumbs_00" + n, new Texture( loadImage(loadAsset("pauseThumbs/thumbs_00" + n + ".png"))) );
	}

	
//handbook
	addTexture("hb_list_item_divider", new Texture( loadImage( loadAsset( "scenes/handbook/list_item_divider.png" ) ) ) );
	addTexture("hb_lock", new Texture( loadImage( loadAsset( "scenes/handbook/lock.png" ) ) ) );
	addTexture("hb_buttonBG", new Texture( loadImage( loadAsset( "scenes/handbook/buttonBG.png" ) ) ) );
	addTexture("hb_tab1", new Texture( loadImage( loadAsset( "scenes/handbook/tab1.png" ) ) ) );
	addTexture("hb_tab2", new Texture( loadImage( loadAsset( "scenes/handbook/tab2.png" ) ) ) );
	addTexture("hb_tab3", new Texture( loadImage( loadAsset( "scenes/handbook/tab3.png" ) ) ) );
	addTexture("hb_tab4", new Texture( loadImage( loadAsset( "scenes/handbook/tab4.png" ) ) ) );
	addTexture("hb_tabSelect1", new Texture( loadImage( loadAsset( "scenes/handbook/tabSelect1.png" ) ) ) );
	addTexture("hb_tabSelect2", new Texture( loadImage( loadAsset( "scenes/handbook/tabSelect2.png" ) ) ) );
	addTexture("hb_tabSelect3", new Texture( loadImage( loadAsset( "scenes/handbook/tabSelect3.png" ) ) ) );
	addTexture("hb_tabSelect4", new Texture( loadImage( loadAsset( "scenes/handbook/tabSelect4.png" ) ) ) );
	
	addTexture("hb_floor_alpha", new Texture( loadImage( loadAsset( "scenes/handbook/alpha_vignette.png" ) ) ) );
	addTexture("hb_floor_blur", new Texture( loadImage( loadAsset( "scenes/handbook/blur_vignette.png" ) ) ) );
	addTexture("hb_floor_particle", new Texture( loadImage( loadAsset( "scenes/handbook/floor_particle.png" ) ) ) );
	addTexture("hb_beam", new Texture( loadImage( loadAsset( "scenes/handbook/beam.png" ) ) ) );
	addTexture("hb_colorBlur", new Texture( loadImage( loadAsset( "scenes/handbook/colorBlur.png" ) ) ) );


//ghetto background images. 
	addTexture("hb_character", new Texture( loadImage( loadAsset( "scenes/handbook/itemBackgrounds/character.png" ) ) ) );
	addTexture("hb_object", new Texture( loadImage( loadAsset( "scenes/handbook/itemBackgrounds/object.png" ) ) ) );
	addTexture("hb_platform", new Texture( loadImage( loadAsset( "scenes/handbook/itemBackgrounds/platform.png" ) ) ) );
	addTexture("hb_sphere", new Texture( loadImage( loadAsset( "scenes/handbook/itemBackgrounds/sphere.png" ) ) ) );
	addTexture("hb_video", new Texture( loadImage( loadAsset( "scenes/handbook/itemBackgrounds/video.png" ) ) ) );

	addVideo("handbook_platform", new MovieGl( loadAsset("scenes/handbook/handbook_platform.mp4")) );
	addVideo("handbook_innerSphere", new MovieGl( loadAsset("scenes/handbook/handbook_innerSphere.mp4")) );
	//
	addVideoSurface("handbook_innerSphere", new MovieSurface( loadAsset("scenes/handbook/handbook_innerSphere.mp4")) );



// handbook sphere
	addTexture("handbook_sphere_texture", new Texture( loadImage( loadAsset( "scenes/handbook/sphere/handbook_sphere_texture.png" ) ) ) );
	addTexture("handbook_sphere_ring", new Texture( loadImage( loadAsset( "scenes/handbook/sphere/handbook_sphere_ring.png" ) ) ) );
	
	addVBO("handbook_sphere", "scenes/handbook/sphere/sphere.obj");
	addVBO("handbook_sphere_particles", "scenes/handbook/sphere/particles.obj");
	addVBO("handbook_floor", "scenes/handbook/floor/handbook_floor.obj");
    


	//constellations
	vector<ci::Vec3f> rots; 
	rots.push_back(ci::Vec3f(-33, -34, -38)); 
	rots.push_back(ci::Vec3f(-8, -48, 0)); 
	rots.push_back(ci::Vec3f(-73, -165, -58)); 
	rots.push_back(ci::Vec3f(-148, 0, 0)); 
	rots.push_back(ci::Vec3f(-34, 116, 0)); 
	rots.push_back(ci::Vec3f(18, 64, -65)); 
	rots.push_back(ci::Vec3f(60, 50, -58)); 
	rots.push_back(ci::Vec3f(17, -7, 24)); 
	rots.push_back(ci::Vec3f(20, -75, -20)); 
	rots.push_back(ci::Vec3f(1, -138, 0)); 
	rots.push_back(ci::Vec3f(22, 159, -43)); 
	rots.push_back(ci::Vec3f(33, 119, -37)); 
	rots.push_back(ci::Vec3f(270, 0, 0)); 
	rots.push_back(ci::Vec3f(-55, -39, 0)); 
	rots.push_back(ci::Vec3f(26, 29, 0)); 
	rots.push_back(ci::Vec3f(13, -54, -44)); 
	rots.push_back(ci::Vec3f(-2, -145, -54)); 
	rots.push_back(ci::Vec3f(32, 109, 0)); 
	rots.push_back(ci::Vec3f(0, -116, -41)); 
	rots.push_back(ci::Vec3f(118, 0, 0)); 
	rots.push_back(ci::Vec3f(67, -29, -19)); 
	rots.push_back(ci::Vec3f(0, -90, 0)); 
	rots.push_back(ci::Vec3f(-21, -115, 43)); 
	rots.push_back(ci::Vec3f(72, -89, 0)); 
	rots.push_back(ci::Vec3f(76, 27, -26)); 
	rots.push_back(ci::Vec3f(13, 202, -79)); 
	rots.push_back(ci::Vec3f(-56, 5, -3)); 
	rots.push_back(ci::Vec3f(0, 180, 0)); 
	rots.push_back(ci::Vec3f(-33, -72, 0)); 
	rots.push_back(ci::Vec3f(45, 123, -127)); 
	rots.push_back(ci::Vec3f(-28, 75, 0)); 
	rots.push_back(ci::Vec3f(-14, -11, 0)); 
	rots.push_back(ci::Vec3f(-15, 128, 0)); 
	rots.push_back(ci::Vec3f(-66, 75, 0)); 
	rots.push_back(ci::Vec3f(-39, -109, -61)); 
	rots.push_back(ci::Vec3f(-65, -91, 0)); 
	rots.push_back(ci::Vec3f(90, 0, -90)); 
	
	TriMesh* mesh;
	for( int i=1; i<38;i++){
		mesh = new TriMesh(); 
		ObjLoader* loaderShape = new ObjLoader( loadAsset("scenes/handbook/sphere/shape " + toString(i) + ".obj") );
		loaderShape->load( mesh );
		gl::VboMesh* mVBOShape = new gl::VboMesh( *mesh );
		
		TriMesh* outlineMesh = new TriMesh(); 
		ObjLoader* loaderOutline = new ObjLoader( loadAsset("scenes/handbook/sphere/shape " + toString(i) + " outline.obj") );
		loaderOutline->load( outlineMesh );
		//gl::VboMesh* mVBOOutline = new gl::VboMesh( *outlineMesh );

		constellations.push_back( Constellation( mVBOShape, outlineMesh, rots[i-1] ) ); 

		delete loaderShape;
		delete loaderOutline;
		//delete mesh;
	}
	//floor lights
	for(int i = 1; i <= 8; i++)
	{
		mesh = new TriMesh;
		
		ObjLoader* loader = new ObjLoader( loadAsset( "scenes/handbook/floor/light_" + toString(i) + ".obj" ) );
		loader->load(mesh);
		
		lightBeams.push_back(mesh); 
		
		delete loader;
	}
}

void Assets::createAsset(FileID id, Asset a)
{
	asset_ids.insert(make_pair(id, a));

	switch(a.type)
	{
	case VIDEO:
		addVideo(getId(id), new MovieGl( loadAsset(getFilename(id))) );
		break;
	case TEXTURE:
		addTexture(getId(id), new Texture( loadImage(loadAsset(getFilename(id)))) );
		break;
	case FONT:
		try{
			addFont(getId(id), new Font( loadAsset(getFilename(id)), 30.0f) );
		} catch(AssetLoadExc e) {
			console() << e.mMessage << endl;
		}
		break;
            
        default:
            break;
	}
}

void Assets::addTexture(const string &id, Texture* asset) 
{
	textures.insert( pair<string, Texture*>(id, asset) );
}

void Assets::addVideo(const string &id, MovieGl* asset) 
{
	// qtime videos need to be 'played' before they can be rendered. 
	//asset->play(); 
	//asset->stop(); 

	videos.insert( pair<string, MovieGl*>(id, asset) );
}

void Assets::addVideoSurface(const string &id, MovieSurface* asset) 
{
	// qtime videos need to be 'played' before they can be rendered. 
	//asset->play(); 
	//asset->stop(); 

	videoSurfaces.insert( pair<string, MovieSurface*>(id, asset) );
}

void Assets::addFont(const string &id, Font* asset)
{
	fonts.insert( pair<string, Font*>(id, asset) );
}

void Assets::addShader(const string &id, GlslProgRef asset)
{
	shaders.insert( pair<string, GlslProgRef>(id, asset) );
}

void Assets::addVBO(const string &id, const string &path)
{
	TriMesh* mesh = new TriMesh(); 
	ObjLoader* loader = new ObjLoader( loadAsset(path) );
	loader->load( mesh );
	gl::VboMesh* vbo = new gl::VboMesh( *mesh );
	
	vbos[id] = vbo;
	meshs[id] = mesh;

	delete loader;
}

gl::VboMesh* Assets::getVBOByID(const string &id)
{
	return vbos[id];
}

TriMesh* Assets::getMeshByID(const string &id)
{
	return meshs[id];
}


Texture* Assets::getNewTexture(const string &path)
{
	return new Texture( loadImage( loadAsset( path ) ) );
}

MovieGl* Assets::getVideoByID(const string &id)
{
	return videos[id];
}

MovieSurface* Assets::getVideoSurfaceByID(const string &id)
{
	return videoSurfaces[id];
}

Texture* Assets::getTextureByID(const string &id)
{
	return textures[id];
}

Font* Assets::getFontByID(const string &id)
{
	return fonts[id];
}

Font* Assets::getFontByID(const FileID& id)
{
	return getFontByID(getId(id));
}

GlslProgRef Assets::getShaderByID(const string &id)
{
	return shaders[id];
}

DataController* Assets::getData()
{
	return data; 
}

void Assets::disposeTextures()
{
	GLuint ids[255];
	int i = 0; 
	map< string, Texture* >::iterator iter;

    for (iter = textures.begin(); iter != textures.end(); ++iter) {
		ids[i] = iter->second->getId();
		++i;
    }
	
	glDeleteTextures(i, ids); 
}

void Assets::disposeVideos()
{
	map< string, MovieGl* >::iterator iter;
	map< string, MovieSurface* >::iterator siter;

    for (iter = videos.begin(); iter != videos.end(); ++iter) {
		iter->second->stop();
    }

	for (siter = videoSurfaces.begin(); siter != videoSurfaces.end(); ++siter) {
		siter->second->stop();
    }
	
	//TODO: anyting else i need to do here? 
}
