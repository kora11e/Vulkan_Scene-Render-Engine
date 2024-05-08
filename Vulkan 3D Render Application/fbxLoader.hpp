#pragma once

#ifndef _FBX_LOADER_
#define _FBX_LOADER_

#include <fbxsdk.h>

class fbxLoader {
private:
	
public:
	struct object3D {
		int number;
		FbxNode* lMeshNode;
		FbxMesh* lMesh;
		FbxMaterialConverter* lMaterial;
	};
};

#endif