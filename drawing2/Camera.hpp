/*
* File:   Camera.hpp
* Author: darek
*
* Created on 23 November 2014, 22:25
*/
#ifndef QFRACTAL_CAMERA_HPP
#define QFRACTAL_CAMERA_HPP

#include "utils.hpp"
#include "OptCamera.hpp"
#include "OptCameraPos.hpp"
#include "Vec.hpp"
#include "gfx.hpp"

class Camera {
public:
	/** (public) Camera zoom in/out */
	void zoom(bool zoom_in, bool ctrl_hold);

	/** (public) Move camera using mouse pointer
	*/
	void mouseMove(int mouse_x, int mouse_y);

	/** (public) Translate camera left (e.g. with WSAD keys)
	*/
	void transLeft() {
		_strafe(-m_opt.trans_speed.left_right);
	}

	/** (public) Translate camera right (e.g. with WSAD keys)
	*/
	void transRight() {
		_strafe(m_opt.trans_speed.left_right);
	}

	/** (public) Translate camera up (e.g. with WSAD keys)
	*/
	void transUp() {
		_changeAltitude(m_opt.trans_speed.up_down);
	}

	/** (public) Translate camera down (e.g. with WSAD keys)
	*/
	void transDown() {
		_changeAltitude(-m_opt.trans_speed.up_down);
	}

	void applyLook() const {
		gluLookAt(_posConst().xConst(), _posConst().yConst(), _posConst().zConst(),
			_targetConst().xConst(), _targetConst().yConst(), _targetConst().zConst(),
			_upConst().xConst(), _upConst().yConst(), _upConst().zConst());
	}

	OptCamera& opt() { return m_opt; }
	const OptCamera& optConst() const { return m_opt; }

	OptCameraPos& pos() { return m_pos; }
	const OptCameraPos& posConst() const { return m_pos; }


private:
	OptCamera		m_opt;
	OptCameraPos	m_pos;

	Vec3f&	_pos()    { return m_pos.pos; }
	Vec3f&	_target() { return m_pos.target; }
	Vec3f&	_up()     { return m_pos.up; }

	const Vec3f&	_posConst() const    { return m_pos.pos; }
	const Vec3f&	_targetConst() const { return m_pos.target; }
	const Vec3f&	_upConst() const     { return m_pos.up; }

	void _move(float speed);
	void _strafe(float speed);
	void _changeAltitude(float speed);
	void _rotateView(float speed);
};

#endif