#include "stdafx.h"
#include "Camera.hpp"
#include "GeomMath.hpp"

/** (public) Camera zoom in/out */
void Camera::zoom(bool zoom_in, bool ctrl_hold) {
	float zoom_speed = ctrl_hold
		? m_opt.zoom_speed.with_ctrl
		: m_opt.zoom_speed.without_ctrl;

	if (!zoom_in) zoom_speed = -zoom_speed;

	/*fprintf(stderr, "zoom_in: %s, ctrl hold: %s, zoom_speed: %f, with_ctrl: %f, without_ctrl: %f\n",
	sbool(zoom_in), sbool(ctrl_hold), zoom_speed,
	m_opt.zoom_speed.with_ctrl,
	m_opt.zoom_speed.without_ctrl);*/
	_move(zoom_speed);
}

/** (public) Move camera using mouse pointer
*/
void Camera::mouseMove(int mouse_x, int mouse_y) {
	/* Window center point coordinates */
	int mid_x = gfx::windowWidth() >> 1;
	int mid_y = gfx::windowHeight() >> 1;

	if ((mouse_x == mid_x) && (mouse_y == mid_y)) // mouse in the middle of window
		return;
	else {
		const float limit_miny = -50.0f;
		const float limit_maxy = 50.0f;

		const float _y = _target().yConst() - _pos().yConst();
		float angle_x = (float)(mid_x - mouse_x) / m_opt.rotate.sensitivity;
		float angle_y = (float)(mid_y - mouse_y) / m_opt.rotate.sensitivity;

		// set mouse cursor in the center of window
		gfx::warpPointer(mid_x, mid_y);

		// The higher the value is the faster the camera looks around.
		_target().y() += angle_y * m_opt.rotate.speed;

		// limit the rotation
		if (Float::cmp(_y, limit_maxy) > 0) {
			_target().y() = _pos().yConst() + limit_maxy;
		}

		if (Float::cmp(_y, limit_miny) < 0)
			_target().y() = _pos().yConst() - limit_miny;

		// rotate
		_rotateView(-angle_x);
	}
}

void Camera::_move(float speed) {
	// CAMERA ---> TARGET vector (vVector)
	ValueVec3f vVector;
	vec3::sub(vVector.v, _target().dataConst(), _pos().dataConst());

	// forward positive camera speed and backward negative camera speed.
	vec3::mul(vVector.v, speed);

	vec3::add(_pos().data(), vVector.v);
	vec3::add(_target().data(), vVector.v);
}

void Camera::_strafe(float speed) {
	ValueVec3f vVector, vOrthoVector;

	// CAMERA ---> TARGET vector (vVector)
	vec3::sub(vVector.v, _target().dataConst(), _pos().dataConst());

	// Orthogonal vector for the view vector (vOrthoVector)
	vOrthoVector.x = -vVector.z;
	vOrthoVector.z = vVector.x;

	vOrthoVector.x *= speed;
	vOrthoVector.z *= speed;

	// left positive cameraspeed and right negative -cameraspeed.
	_pos().x() += vOrthoVector.x;
	_pos().z() += vOrthoVector.z;
	//
	_target().x() += vOrthoVector.x;
	_target().z() += vOrthoVector.z;
}

void Camera::_changeAltitude(float speed) {
	_pos().y() += speed;
	_target().y() += speed;
}

void Camera::_rotateView(float speed) {
	if (Float::cmp(fabs(speed), 0.0f) > 0) {
		ValueVec3f vVector;
		const float sin_speed = sinf(speed);
		const float cos_speed = cosf(speed);

		// CAMERA ---> TARGET vector (vVector)
		vec3::sub(vVector.v, _target().dataConst(), _pos().dataConst());

		_target().z() = _pos().zConst() + sin_speed * vVector.x + cos_speed * vVector.z;
		_target().x() = _pos().xConst() + cos_speed * vVector.x - sin_speed * vVector.z;
	}
}
