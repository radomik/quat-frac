/*
* File:   DrawingCreator.hpp
* Author: darek
*
* Created on 10 December 2014, 14:28
*/
#ifndef QFRACTAL_DRAWING_DRAWINGCREATOR_HPP
#define QFRACTAL_DRAWING_DRAWINGCREATOR_HPP

#include "utils.hpp"
#include "GLdrawing.hpp"

class OptGenerator;
class OptDrawingCreator;

class DrawingCreator {
	GLdrawing				 m_drawing;

public:
	virtual ~DrawingCreator() { }
	
	int create(
		FILE*						fractal_data_file, 
		const OptDrawingCreator&	opt_drawing);

	//void		 destroyDisplayObjects() {
	//	destroyDisplayObjects(m_drawing); 
	//}

	GLdrawing&       drawing()            { return m_drawing; }
	const GLdrawing& drawingConst() const { return m_drawing; }

protected:
	const OptGenerator& m_opt;
	
	DrawingCreator(const OptGenerator& opt) : m_opt(opt) { }
	
	virtual unsigned int getVersion() const = 0;
	virtual const char*  getName() const = 0;
	virtual size_t		 getDrawingElementByteSize() const = 0;
	virtual int 		 checkOpt(const OptDrawingCreator& opt) const = 0;
	//virtual void		 destroyDisplayObjects(GLdrawing& drawing) = 0;
	
	virtual int createCustom(
		GLdrawing&					drawing,
		unsigned char*				file_data,
		const OptDrawingCreator&	opt_drawing,
		unsigned int				count_data_points)=0;

private:
	static int _st_checkVersions(
		unsigned int				opt_generator_version, 
		const OptDrawingCreator&	opt_drawing);

	unsigned char* _readFileIntoArray(
		FILE			*fractal_data_file,
		unsigned int	count_data_points);
};

#endif

