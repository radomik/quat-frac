/*
 * File:   OptDrawingCreator.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:29
 */
#ifndef QFRACTAL_DRAWING_OPTDRAWINGCREATOR_HPP
#define QFRACTAL_DRAWING_OPTDRAWINGCREATOR_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Color.hpp"

class OptDrawingCreator : public ISerializable {
protected:
  virtual void customReadJson(...) = 0;
  virtual void customSaveJson(...) const = 0;
  virtual void customReadBin(...) = 0;
  virtual void customSaveBin(...) const = 0;
  virtual void customSetDefault() = 0;
  virtual void customSet(const OptDrawingCreator& other) = 0;

public: /// common fields
  // algorithm version
  unsigned int  version;

  // point color
  Color4f     color;

  // inverse color ?
  bool      color_negative;

  // initial display list index
  unsigned int  disp_list_ind;

  virtual ~OptDrawingCreator() { }

  // read common + custom fields
  virtual void readJson(const nlohmann::json &json, const char *tag);

  // read common + custom fields
  virtual void readBin(FILE *file, const char *tag);

  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;

  void setDefault();

  void set(const OptDrawingCreator& other);

  /// create new OptDrawing of appropiate type (must be deleted by caller)
  /// returns NULL on error
  static OptDrawingCreator* readNewByText(Parser& parser);

  /// create new OptDrawing of appropiate type (must be deleted by caller)
  /// returns NULL on error
  static OptDrawingCreator* readNewByBin(FILE* f);

  /// create new OptDrawing of appropiate type (must be deleted by caller)
  /// returns NULL on error
  static OptDrawingCreator* newByVersion(unsigned int version);

  /// create new OptDrawing of appropiate type (must be deleted by caller)
  /// returns NULL on error
  static OptDrawingCreator* newByCopy(const OptDrawingCreator& other);

private:
  /// read common fields
  /// @note version is already read
  /// @see readNewByText
  void readJsonCommon(const nlohmann::json &json, const char *tag);

  /// read common fields
  /// @note version is already read
  /// @see readNewByBin()
  void _readBinCommon(FILE *file, const char *tag);

  /// save common fields (also version)
  virtual nlohmann::json saveJsonCommon(const char *tag) const;

  /// save common fields (also version)
  void _saveBinCommon(FILE *file, const char *tag) const;

  static void _st_unexpectedItem(const char* id, const char* id_type);

  static OptDrawingCreator* _st_readJson(const nlohmann::json &json, const char *tag, unsigned int version);

};

#endif
