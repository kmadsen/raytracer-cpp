#ifndef RASTERPARSER_H_
#define RASTERPARSER_H_


#include <iostream>
#include <string>
#include <map>

#include <glm/glm.hpp>

#include "Vector.h"
#include "Point.h"
#include "Color.h"

namespace raster {

class Camera;
class Background;
class Light;
class Material;
class Object;
class Scene;

class Parser {
  struct Token {
    enum type {
      end_of_file,
      integer, real, string,
      left_brace, right_brace,
      left_bracket, right_bracket,
      comma,
    };
    type token_type;
    std::string string_value;
    int integer_value;
    double real_value;
    int line_number;
    int column_number;
  };
  std::istream &input;
  int line_number;
  int column_number;
  Token next_token;
  Material *default_material;
  std::map< std::string, Material * > defined_materials;
  std::map< std::string, Object * > defined_objects;

  void throwParseException(std::string const &message) const;

  void readNextToken();
  bool peek(Token::type const type);
  bool peek(std::string const &keyword);
  Token match(
    Token::type const type,
    std::string const &failure_message);
  Token match(
    std::string const &keyword,
    std::string const &failure_message);

  std::string parseString();
  bool parseBoolean();
  int parseInteger();
  double parseReal();
  glm::vec3 const parseVector();
  glm::vec3 const parsePoint();
  glm::vec3 const parseColor();

  raster::Camera* parsePinholeCamera();
  raster::Camera* parseOrthographicCamera();
  raster::Camera* parseThinlensCamera();
  raster::Camera* parseCamera();

  // Background *parseConstantBackground();
  // Background *parseBackground();

  raster::Light* parsePointLight();
  // raster::Light* parseAreaLight();
  raster::Light* parseLight();

  // Material *parseDielectricMaterial();
  // Material *parseLambertianMaterial();
  // Material *parseGlossymetalMaterial();
  // Material *parsePhongMaterial();
  // Material *parseMetalMaterial();
  // Material *parseMaterial();

  // Object *parseGroupObject();
  // Object *parseBVHGroupObject();
  // Object *parsePlaneObject();
  // Object *parseSphereObject();
  // Object *parseBoxObject();
  // Object *parseDiskObject();
  // Object *parseRingObject();
  // Object *parseTriangleObject();
  // Object *parseObject();

 public:
  explicit Parser(std::istream &input);

  Scene* parseScene(const std::string &filename);
};

}  // namespace raster

#endif  // RASTERPARSER_H_
