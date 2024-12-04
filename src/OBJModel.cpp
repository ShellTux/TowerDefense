#include "OBJModel.hpp"

#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


std::map<std::filesystem::path, OBJModel> OBJModel::GlobalModels = {};

OBJModel::OBJModel(const std::filesystem::path &path)
{
	LoadFromFile(path);
}

std::ostream &operator<<(std::ostream &os, const OBJModel &model)
{
	os << "{" << std::endl;

	{
		for (const auto &[material, color] : model.mMaterialMap) {
			os << material << ": " << color.r << ", " << color.g
			   << ", " << color.b << std::endl;
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

void OBJModel::LoadFromFile(const std::filesystem::path &filename)
{
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Error loading obj file: " << filename
		          << std::endl;
		return;
	}

	std::vector<Vec3> vertices{};
	std::vector<Vec3> normals{};
	std::string line;
	std::string currentMtlName;

	while (std::getline(file, line)) {
		if (StartWith(line, "mtllib")) {
			std::istringstream iss(line);
			std::string keyword;
			std::string mtlFileName;

			iss >> keyword >> mtlFileName;
			LoadMaterialFile(filename.parent_path() / mtlFileName);
		} else if (StartWith(line, "v ")) {
			f64 x = 0;
			f64 y = 0;
			f64 z = 0;

			std::istringstream iss(line);
			iss.ignore(2);
			iss >> x >> y >> z;
			vertices.emplace_back(x, y, z);
		} else if (StartWith(line, "vn ")) {
			f64 nX = 0;
			f64 nY = 0;
			f64 nZ = 0;

			std::istringstream iss(line);
			iss.ignore(3);
			iss >> nX >> nY >> nZ;
			normals.emplace_back(nX, nY, nZ);
		} else if (StartWith(line, "usemtl")) {
			std::istringstream iss(line);
			std::string keyword;
			iss >> keyword >> currentMtlName;
		} else if (StartWith(line, "f ")) {
			std::istringstream iss(line);
			iss.ignore(2);

			for (int i = 0; i < 3; ++i) {
				int position = 0;
				int texture  = 0;
				int normal   = 0;
				if (!(iss >> position)) {
					std::cerr
					    << "Face format error: " << line
					    << std::endl;
					break;
				}

				iss.ignore(1);

				if (!(iss >> texture)) {
					std::cerr
					    << "Face format error: " << line
					    << std::endl;
					break;
				}

				iss.ignore(1);

				if (!(iss >> normal)) {
					std::cerr
					    << "Face format error: " << line
					    << std::endl;
					break;
				}

				addVertexData(position,
				              normal,
				              currentMtlName.c_str(),
				              vertices,
				              normals);
			}
		}
	}
}

std::vector<f64> OBJModel::getVertexData() const
{
	return mVertexData;
}

i32 OBJModel::getVertexCount() const
{
	return static_cast<i32>(mVertexData.size() / 9);
}

void OBJModel::LoadMaterialFile(const std::filesystem::path &filename)
{
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Loading Material file failed: " << filename
		          << std::endl;
		return;
	}

	std::string line;
	std::string mtlName;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string keyword;

		iss >> keyword;

#ifdef DEBUG
		std::cout << "keyword: " << keyword << " ";
#endif

		if (keyword == "newmtl") {
			iss >> mtlName;
#ifdef DEBUG
			std::cout << mtlName << std::endl;
#endif
		} else if (keyword == "Kd") {
			Color &color = mMaterialMap[mtlName];
			iss >> color.r >> color.g >> color.b;

#ifdef DEBUG
			std::cout << color.r << ", " << color.g << ", "
			          << color.b << std::endl;
#endif
		}

		std::cout << std::endl;
	}
}

bool OBJModel::StartWith(const std::string &line, const char *text)
{
	return line.find(text) == 0;
}

void OBJModel::addVertexData(const i32 vIdx,
                             const i32 nIdx,
                             const char *const mtl,
                             const std::vector<Vec3> &vertices,
                             const std::vector<Vec3> &normals)
{
	Color c = mMaterialMap.at(mtl);

	const auto &[pX, pY, pZ] = vertices[vIdx - 1].getCoordinates();
	const auto &[nX, nY, nZ] = normals[nIdx - 1].getCoordinates();

	mVertexData.push_back(pX);
	mVertexData.push_back(pY);
	mVertexData.push_back(pZ);
	mVertexData.push_back(c.r);
	mVertexData.push_back(c.g);
	mVertexData.push_back(c.b);
	mVertexData.push_back(nX);
	mVertexData.push_back(nY);
	mVertexData.push_back(nZ);
}

void OBJModel::draw() const
{
#if 1
	const std::vector<f64> &vertices = getVertexData();

	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	{
		glColor3ub(255, 0, 255);

		for (int i = 0; i < getVertexCount(); ++i) {
			const int index = i * 9;

			const f64 &pX = vertices[index + 0];
			const f64 &pY = vertices[index + 1];
			const f64 &pZ = vertices[index + 2];

			const struct Color color = {vertices[index + 3],
			                            vertices[index + 4],
			                            vertices[index + 5]};

			const f64 &nX = vertices[index + 6];
			const f64 &nY = vertices[index + 7];
			const f64 &nZ = vertices[index + 8];

			glColor3d(color.r, color.g, color.b);
			glNormal3d(nX, nY, nZ);
			glVertex3d(pX, pY, pZ);
		}
	}
	glEnd();
	glPopAttrib();
#else
	const std::vector<f32> &vertices = getVertexData();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 9 * sizeof(f32), vertices.data());
	glColorPointer(3, GL_FLOAT, 9 * sizeof(f32), vertices.data() + 3);
	glNormalPointer(GL_FLOAT, 9 * sizeof(f32), vertices.data() + 6);

	glDrawArrays(GL_TRIANGLE_FAN, 0, getVertexCount() * 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopAttrib();
#endif
}

OBJModel &OBJModel::Get(const std::filesystem::path &path)
{
	if (GlobalModels.find(path) == GlobalModels.end()) {
		GlobalModels[path] = OBJModel(path);
	}

	return GlobalModels.at(path);
}
