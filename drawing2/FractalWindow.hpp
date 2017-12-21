/*
 * File:   FractalWindow.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 19:59
 */
#ifndef QFRACTAL_FRACTALWINDOW_HPP
#define QFRACTAL_FRACTALWINDOW_HPP

#include "Camera.hpp"
#include "FractalDrawing.hpp"
#include "OptDisplay.hpp"
#include "OptDrawing.hpp"
#include "utils.hpp"
#include "gfx.hpp"
#include <vector>

class OptAppDrawingParser;
class ProgramArgs;
class OptDrawingCreator;

struct LightInfo {
	GLfloat				pos[4];
	GLfloat				diffuse[4];
	GLfloat				spot_dir[3];

	GLfloat				spot_cut_off;
	GLfloat				spot_exponent;

	bool				enabled;
};

/** 
 * Singleton with sole instance in FractalWindowLoader 
 **/
class FractalWindow {
public:
	int init();

	void initGfx(OptAppDrawingParser& settings, ProgramArgs& args);
	
	int createDrawing(
		FILE*						fractal_data_file, 
		const OptGenerator&			opt_generator, 
		const OptDrawingCreator&	opt_drawing);
	
	void start() const;
	
private:
	static FractalWindow*	self;
	static const GLenum		LIGHT_IDS[];
	Camera					m_camera;
	FractalDrawing			m_drawing;
	OptDisplay				m_opt_display;
	OptDrawing				m_opt_drawing;
	LightInfo				m_light[3];
	unsigned char			m_keystate[256];
	std::string				m_png_filename_prefix;
	unsigned int			m_png_filename_index;
	int						m_window;
	bool					m_paused;
	bool					m_is_alpha_enabled;
	bool					m_is_lighting_enabled;
	bool					m_is_light0_enabled;
	bool					m_is_texture2d_enabled;
	bool					m_is_colormaterial_enabled;
	unsigned char			m_curr_light;
	char					m_buf[256];
	
	void _toggleCameraRotate();
	void _rotate(unsigned int coord_ind, bool value_up);
	void _mouseWheel(bool wheel_up);
	void _initGlutWindow(OptAppDrawingParser& settings, ProgramArgs& args);
	void _updateOptAppDrawingParser(const OptAppDrawingParser& settings);
	void _initGraphics(OptAppDrawingParser& settings);
	void _save();
	void _initLights(OptAppDrawingParser& settings);
	void _applyLights() const;
	
	char _lightOn();
	char _lightOn(unsigned int ind);
	void _showLightPosInfo();
	void _showLightDiffuseInfo();
	void _showLightSpotDir();
	void _showLightSpotCutOff();
	void _showLightSpotExponent();
	void _showSelectedLight();
	void _showLightState();
	void _toggleLight(unsigned int ind);
	void _changeLightPos(unsigned int coord_ind, bool wheel_up);
	void _changeLightDiffuse(unsigned int coord_ind, bool wheel_up);
	void _changeLightSpotDir(unsigned int coord_ind, bool wheel_up);
	void _changeLightSpotCutOff(bool wheel_up);
	void _changeLightExponent(bool wheel_up);
	
	static int _st_readPixels(
		std::vector<unsigned char>& pixel_data, 
		unsigned int				&bytes_per_line,
		unsigned int 				w,
		unsigned int 				h);
	
	static int _st_revertPixelData(
		std::vector<unsigned char>& 		pixel_data_rvt,
		const std::vector<unsigned char>&   pixel_data,
		unsigned int						bytes_per_line,
		unsigned int						h);
		
	static int  _st_exportPng();

	static void _st_mouse(int button, int state, int x, int y);
	static void _st_mouseMove(int x, int y);
	static void _st_keyboard(unsigned char key, int x, int y);
	static void _st_keyboardUp(unsigned char key, int x, int y);
	static void _st_keyboardSpecial(int key, int x, int y);
	static void _st_display();
	static void _st_resize(int new_w, int new_h);
	static void _st_setCallbacks();
	static bool _st_ctrl_pressed();
	static void _st_sigint(int sig);
};

#endif
