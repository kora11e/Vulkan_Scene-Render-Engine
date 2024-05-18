#pragma once

#ifndef _FBX_LOADER_
#define _FBX_LOADER_

#include <fbxsdk.h>

class fbxLoader {
private:
	FbxManager* Manager;
	FbxImporter* Importer;
	FbxScene Scene;
	static char* lFilename ;
public:
	struct object3D {
		int number;
		FbxNode* lMeshNode;
		FbxMesh* lMesh;
		FbxMaterialConverter* lMaterial;
		FbxNode* node;
		static char* getlFilename() { return lFilename; };
		void setlFilename(char* filename);
	};
};

#endif