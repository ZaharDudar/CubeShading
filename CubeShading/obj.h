#pragma once
#include <Vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <math.h>
#include "vectFunctions.h"
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using sf::Vector2f;
using sf::Vector3f;

enum lightType {
	GLOBAL_LIGHT = 0,
	POINT_LIGHT = 1,
	NO_LIGHT = -1
};

struct face
{
	vector<Vector3f> points;
	Vector3f normal;
	Vector3f center;
};

struct material {
	Vector3f Ka;
	Vector3f Kd;
	Vector3f Ks;
	float Ns;
	material() {
		Ka = Vector3f(1, 1, 1);
		Kd = Vector3f(1, 1, 1);
		Ks = Vector3f(1, 1, 1);
		Ns = 1;
	}
};


vector<string> split(string a, char delimeter) {
	string tmp;
	vector<string> output;
	for (int ch = 0; ch < a.size(); ch++) {
		if (a[ch] == delimeter) {
			output.push_back(tmp);
			tmp = "";
		}
		else {
			tmp += a[ch];
		}
	}
	output.push_back(tmp);
	tmp = "";
	return output;
}

struct light {
public:
	int type = NO_LIGHT;
	Vector3f pos;
	float radius=0;
	Vector3f dir;
	sf::Color color;
};
struct GlobalLight : light
{
	
	int type = GLOBAL_LIGHT;
	Vector3f dir;
	sf::Color color;
	GlobalLight(Vector3f initDir) {
		dir = initDir;
		normalize(&dir);
		color = sf::Color(0xff, 0xff, 0xff);
	}
	GlobalLight() {
		color = sf::Color(0xff, 0xff, 0xff);
	}
	GlobalLight(Vector3f initDir, sf::Color ic) {
		dir = initDir;
		normalize(&dir);
		color = ic;
	}
};
struct PointLight : light
{
	PointLight(Vector3f initPos) {
		pos=initPos;
		radius = 1;
		type =POINT_LIGHT;
		color = sf::Color(0xff, 0xff, 0xff);
	}
	PointLight(Vector3f initPos, float rad) {
		pos = initPos;
		radius = rad;
		type = POINT_LIGHT;

		color = sf::Color(0xff, 0xff, 0xff);
	}
	PointLight() {
		pos.x = 0;
		pos.x = 0;
		pos.x = 0;
		radius = 1;
		type = POINT_LIGHT;

		color = sf::Color(0xff, 0xff, 0xff);
	}
	PointLight(Vector3f initPos,float rad, sf::Color ic) {
		pos = initPos;
		type = POINT_LIGHT;
		radius = rad;
		color = ic;
	}
};

bool comp(face a, face b) {
	return a.center.z > b.center.z;
}
class obj
{
public:
	float size = 50;
	Vector3f center;
	sf::Vector2f offset;
	vector<Vector3f> vertexes;
	vector<face> faces;
	material self_material;

	vector<Vector2f> to_draw_vert() {
		vector<Vector2f> to_displ;
		Vector3f sized_vert;
		for (int vertId = 0; vertId < vertexes.size(); vertId++) {
			sized_vert = vertexes[vertId] * size;
			to_displ.push_back(Vector2f{sized_vert.x + offset.x, sized_vert.y + offset.y });
		}
		return to_displ;
	}

	vector<Vector2f> to_draw_face(face cface) {
		vector<Vector2f> to_displ;
		Vector3f sized_vert;
		for (int vertId = 0; vertId < cface.points.size(); vertId++) {
			sized_vert = cface.points[vertId] * size;
			to_displ.push_back(Vector2f{ sized_vert.x + offset.x, sized_vert.y + offset.y });
		}
		return to_displ;
	}

	virtual void rotate(float a, float b, float c) {};

};

class Cube : public obj
{
public:
	Cube() {
		vertexes = { {1,0,0},{0,0,0},{0,1,0},{1,1,0},{1,0,1},{0,0,1},{0,1,1},{1,1,1} };
		for (int vert = 0; vert < vertexes.size(); vert++) {
			center += vertexes[vert];
		}
		center = center / (float)vertexes.size();
		face Fstface;
		for (int vert = 0; vert < vertexes.size(); vert++) {
			vertexes[vert] -= center;
			if (vert < 4)
				Fstface.points.push_back(vertexes[vert]);
		}
		Fstface.normal = Vector3f{ 0,0,-1 };
		Fstface.center = avarage(Fstface.points);
		faces.push_back(Fstface);
		Fstface.points = { vertexes[4], vertexes[5], vertexes[6], vertexes[7] };
		Fstface.normal = Vector3f{ 0,0,1 };
		Fstface.center = avarage(Fstface.points);
		faces.push_back(Fstface);


		Fstface.points = { vertexes[2], vertexes[3], vertexes[7], vertexes[6] };
		Fstface.normal = Vector3f{ 0,1,0 };
		Fstface.center = avarage(Fstface.points);
		faces.push_back(Fstface);

		Fstface.points = { vertexes[0], vertexes[3], vertexes[7], vertexes[4] };
		Fstface.normal = Vector3f{ 1,0,0 };
		Fstface.center = avarage(Fstface.points);
		faces.push_back(Fstface);

		Fstface.points = { vertexes[0], vertexes[1], vertexes[5], vertexes[4] };
		Fstface.normal = Vector3f{ 0,-1,0 };
		Fstface.center = avarage(Fstface.points);
		faces.push_back(Fstface);

		Fstface.points = { vertexes[1], vertexes[2], vertexes[6], vertexes[5] };
		Fstface.normal = Vector3f{ -1,0,0 };
		Fstface.center = avarage(Fstface.points);
		faces.push_back(Fstface);
	}
	void rotate(float a, float b, float c) override;	
};


void Cube::rotate(float a, float b, float c) {
	for (int vert = 0; vert < vertexes.size(); vert++) {
		vertexes[vert] = rotateVect(vertexes[vert], a, b, c);
	}
	for (int faceId = 0; faceId < faces.size(); faceId++) {
		for (int vert = 0; vert < faces[faceId].points.size(); vert++) {
			faces[faceId].points[vert] = rotateVect(faces[faceId].points[vert], a, b, c);
		}
		faces[faceId].normal = rotateVect(faces[faceId].normal, a, b, c);
	}
}


class inputObj : public obj 
{
private:
	vector<Vector3f> normals;
	string name_material;
	string mtl_lib;
public:
	inputObj(char* fileName, Vector2f inOffset) {
		offset = inOffset;
		/*______________Чтение геометрии______________*/
		std::ifstream file(fileName);
		string raw_line;
		vector<string> line;
		if (file.fail()) { cout << "ERROR OPENING FILE"; exit(1); }
		while (std::getline(file, raw_line)) {
			line = split(raw_line, ' ');
			if (line[0] == "v") {
				vertexes.push_back(Vector3f(std::stof(line[1]), std::stof(line[2]), std::stof(line[3])));
			}
			else if (line[0] == "vn") {
				Vector3f tmp_norm(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
				normalize(&tmp_norm);
				normals.push_back(tmp_norm);
				
			}
			else if (line[0] == "f") {
				face tmp_face;
				vector<string> indexes;
				for (int vertex = 1; vertex < line.size(); vertex++) {
					indexes = split(line[vertex], '/');
					tmp_face.points.push_back(vertexes[std::stoi(indexes[0])-1]);
				}
				tmp_face.normal = normals[ std::stoi(indexes[2])-1];
				tmp_face.center = avarage(tmp_face.points);
				faces.push_back(tmp_face);
			}
			else if (line[0] == "usemtl") {
				name_material = line[1];
			}
			else if (line[0] == "mtllib") {
				mtl_lib = "./" + line[1];
			}
		}
		file.close();
		/*__________Чтение материала____________*/
		file.open(mtl_lib);
		if (!file.bad()) {
			while (std::getline(file, raw_line)) {
				line = split(raw_line, ' ');
				if (line[0] == "Ka") {
					self_material.Ka = Vector3f(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
				}
				else if (line[0] == "Kd") {
					self_material.Kd = Vector3f(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
				}
				else if (line[0] == "Ks") {
					self_material.Ks = Vector3f(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
				}
				else if (line[0] == "Ns") {
					self_material.Ns = std::stof(line[1]);
				}
			}
		}
		file.close();
		
		Vector3f center;
		for (int i = 0; i < vertexes.size(); i++) {
			center += vertexes[i];
		}
		center = center/(float)vertexes.size();
		for (int i = 0; i < vertexes.size(); i++) {
			vertexes[i] -= center; 
		}
	}
	void rotate(float a, float b, float c) override;
};
void inputObj::rotate(float a, float b, float c) {
	for (int vert = 0; vert < vertexes.size(); vert++) {
		vertexes[vert] = rotateVect(vertexes[vert], a, b, c);
	}
	for (int faceId = 0; faceId < faces.size(); faceId++) {
		for (int vert = 0; vert < faces[faceId].points.size(); vert++) {
			faces[faceId].points[vert] = rotateVect(faces[faceId].points[vert], a, b, c);
		}
		faces[faceId].normal = rotateVect(faces[faceId].normal, a, b, c);
	}
	std::sort(faces.begin(), faces.end(), comp);
}