#pragma once
# include <string>

struct Node {

};

class Fbx_Model {
public:
	friend class FbxLoader;
private:
	std::string name;
};