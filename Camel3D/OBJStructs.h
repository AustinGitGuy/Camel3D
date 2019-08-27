#pragma once
//CREDIT TO https://www.dreamincode.net/forums/topic/342023-loading-obj-in-opengl-texture-issue/ for the obj loader

struct Vertex{
	bool used;
	double x, y, z;
};

struct Position{
	float x, y, z, xrot, yrot, zrot;
};

struct TVertex{
	double x, y;
};

struct Image{
	int   type;
	int   id;
	int   width;
	int   height;
	char *data;
};

struct TriFace{
	int a, b, c;
	bool used;
};

struct Map{
	int vI[3];
	int uvI[3];
	int vnI[3];
};

struct Obj{
	std::string name;
	std::string mtl;
	int TotalFaceCount;
	int imgIndex;

	std::vector<Map> fmap;
};

struct ObjLoader{
	int objCount;

	std::vector<TVertex> uv_vert;
	std::vector<Vertex> vert;
	std::vector<Vertex> vn_vert;

	std::vector<Obj> obj;
	std::string mtllib;
};