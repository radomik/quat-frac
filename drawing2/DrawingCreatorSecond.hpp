/*
* File:   DrawingCreatorSecond.hpp
* Author: darek
*
* Created on 11 December 2014, 18:27
*/
#ifndef QFRACTAL_DRAWING_DRAWINGCREATORSECOND_HPP
#define QFRACTAL_DRAWING_DRAWINGCREATORSECOND_HPP

#include "utils.hpp"
#include "DrawingCreator.hpp"
#include "Quat.hpp"
#include "gfx.hpp"

class GLdrawing;
class OptDrawingCreator;
class OptDrawingCreatorSecond;

class DrawingCreatorSecond : public DrawingCreator 
{
	GLfloat*					m_vertices;
	GLubyte*					m_colors;
	ValueQuatf					m_step;
	
public:
	DrawingCreatorSecond(const OptGenerator& opt) 
		: DrawingCreator(opt), 
		  m_vertices(NULL), m_colors(NULL) { }
		
	virtual			~DrawingCreatorSecond();
	

protected:
	virtual unsigned int getVersion() const { return 2; }
	virtual const char*  getName() const { return "DrawingCreatorSecond (vertex arrays)"; }
	virtual size_t       getDrawingElementByteSize() const { return QUATF_BIN_SIZE + sizeof(range_iter_t); }
	virtual int 		 checkOpt(const OptDrawingCreator& opt) const;
	//virtual void		 destroyDisplayObjects(GLdrawing& drawing);

	virtual int createCustom(
		GLdrawing&					drawing,
		unsigned char*				file_data,
		const OptDrawingCreator&	opt_drawing,
		unsigned int				count_data_points);

private:
	static const GLubyte VERTEX_COUNT_PER_ELEMENT;
	static const GLubyte VERTEX_COORDS_COUNT_PER_ELEMENT;
	static const GLubyte COLOR_CHANNEL_COUNT_PER_ELEMENT;
						
	void _createVerticesAndColors(
		unsigned char*					file_data,
		unsigned int					count_data_points,
		const OptDrawingCreatorSecond&	opt_drawing);

	int _allocateVerticesAndColors(
		unsigned int					count_data_points);
};
#endif
