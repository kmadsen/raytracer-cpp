#ifndef Parser_h
#define Parser_h

#include "Vector.h"
#include "Point.h"
#include "Color.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>

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
  std::shared_ptr<Material> default_material;
  std::map< std::string, std::shared_ptr<Material>> defined_materials;
  std::map< std::string, Object * > defined_objects;

  void throwParseException(
    std::string const &message ) const;

  void readNextToken();
  bool peek(
    Token::type const type );
  bool peek(
    std::string const &keyword );
  Token match(
    Token::type const type,
    std::string const &failure_message );
  Token match(
    std::string const &keyword,
    std::string const &failure_message );

  std::string parseString();
  bool parseBoolean();
  int parseInteger();
  double parseReal();
  Vector const parseVector();
  Point const parsePoint();
  Color const parseColor();
  Camera *parsePinholeCamera();
  Camera *parseOrthographicCamera();
  Camera *parseThinlensCamera();
  Camera *parseCamera();

  Background *parseConstantBackground();
  Background *parseBackground();

  Light *parsePointLight();
  Light *parseAreaLight();
  Light *parseLight();

  std::shared_ptr<Material> parseDielectricMaterial();
  std::shared_ptr<Material> parseLambertianMaterial();
  std::shared_ptr<Material> parseGlossymetalMaterial();
  std::shared_ptr<Material> parsePhongMaterial();
  std::shared_ptr<Material> parseMetalMaterial();
  std::shared_ptr<Material> parseMaterial();

  Object *parseGroupObject();
  Object *parseBVHGroupObject();
  Object *parsePlaneObject();
  Object *parseSphereObject();
  Object *parseBoxObject();
  Object *parseDiskObject();
  Object *parseRingObject();
  Object *parseTriangleObject();
  Object *parseObject();

 public:

  Parser(std::istream &input);
  virtual ~Parser();

  Scene *parseScene(std::string &filename);

};

#endif
