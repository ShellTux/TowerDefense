#ifndef INCLUDE_INCLUDE_OBJMODEL_HPP_
#define INCLUDE_INCLUDE_OBJMODEL_HPP_

#include "types.hpp"

#include <filesystem>
#include <map>
#include <ostream>
#include <string>
#include <vector>

class OBJModel {
      public:
	struct Color {
		f32 r;
		f32 g;
		f32 b;
	};
	struct Vec3 {
		f32 x;
		f32 y;
		f32 z;

		friend std::ostream &operator<<(std::ostream &os,
		                                const Vec3 &vec)
		{
			os << "(" << vec.x << ", " << vec.y << ", " << vec.z
			   << ")";
			return os;
		}
	};

	OBJModel()                            = default;
	OBJModel(OBJModel &&)                 = default;
	OBJModel(const OBJModel &)            = default;
	OBJModel &operator=(OBJModel &&)      = default;
	OBJModel &operator=(const OBJModel &) = default;
	~OBJModel()                           = default;

	friend std::ostream &operator<<(std::ostream &os, const OBJModel &model)
	{
		os << "{" << std::endl;

		{
			for (const auto &[material, color] :
			     model.mMaterialMap) {
				os << material << ": " << color.r << ", "
				   << color.g << ", " << color.b << std::endl;
			}
		}

		{
			os << "vertices: ";
			for (const auto &vertex : model.mVertexData) {
				os << vertex << " ";
			}
			os << std::endl;
		}

		os << "}";
		return os;
	}

	static OBJModel &Get(const std::filesystem::path &path);

	void draw() const;

	[[nodiscard]] std::vector<f32> getVertexData() const;
	[[nodiscard]] i32 getVertexCount() const;

	void LoadFromFile(const std::filesystem::path &filename);

      private:
	std::map<std::string, Color> mMaterialMap;
	std::vector<f32> mVertexData;

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
