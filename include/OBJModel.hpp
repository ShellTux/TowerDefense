#ifndef INCLUDE_INCLUDE_OBJMODEL_HPP_
#define INCLUDE_INCLUDE_OBJMODEL_HPP_

#include "Vec3.hpp"
#include "types.hpp"

#include <filesystem>
#include <map>
#include <ostream>
#include <string>
#include <vector>

class OBJModel {
      public:
	struct Color {
		f64 r;
		f64 g;
		f64 b;
	};

	OBJModel()                            = default;
	OBJModel(OBJModel &&)                 = default;
	OBJModel(const OBJModel &)            = default;
	OBJModel &operator=(OBJModel &&)      = default;
	OBJModel &operator=(const OBJModel &) = default;
	~OBJModel()                           = default;

	friend std::ostream &operator<<(std::ostream &os,
	                                const OBJModel &model);

	static OBJModel &Get(const std::filesystem::path &path);

	void draw() const;

	[[nodiscard]] std::vector<f64> getVertexData() const;
	[[nodiscard]] i32 getVertexCount() const;

	void LoadFromFile(const std::filesystem::path &filename);

      private:
	std::map<std::string, Color> mMaterialMap;
	std::vector<f64> mVertexData;

	OBJModel(const std::filesystem::path &path);

	static std::map<std::filesystem::path, OBJModel> GlobalModels;

	void LoadMaterialFile(const std::filesystem::path &filename);
	bool StartWith(const std::string &line, const char *text);
	void addVertexData(const i32 vIdx,
	                   const i32 nIdx,
	                   const char *const mtl,
	                   const std::vector<Vec3> &vertices,
	                   const std::vector<Vec3> &normals);
};

#endif // INCLUDE_INCLUDE_OBJMODEL_HPP_
