/*
 * File:   OptDrawingCreatorSecond.hpp
 * Author: darek
 *
 * Created on 11 December 2014, 17:25
 */
#ifndef QFRACTAL_DRAWING_OPTDRAWINGCREATORSECOND_HPP
#define QFRACTAL_DRAWING_OPTDRAWINGCREATORSECOND_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Color.hpp"
#include "OptDrawingCreator.hpp"
#include "CoordMap.hpp"

class OptDrawingCreatorSecond : public OptDrawingCreator {
protected:
	virtual int	customReadText(Parser& parser);
	virtual int	customSaveText(Parser& parser) const;
	virtual int customReadBin(FILE* f);
	virtual int customSaveBin(FILE* f) const;
	virtual void customSetDefault();
	virtual void customSet(const OptDrawingCreator& other);
	
public:
	CoordMap			coord_map;
	unsigned int		color_multiplier;
	
	virtual ~OptDrawingCreatorSecond() { }
};

#endif

