#include <fbxsdk.h>

int main(int argc, char** argv) {
	const char* lFilename = "file.fbx";

	FbxManager* manager = FbxManager::Create();
	FbxScene scene = FbxScene::ContentReadFrom(&lFilename);
	scene.GetAnimationEvaluator();
	scene.GetMaterial(0);
	scene.GetTextureCount();
	scene.GetTexture(lFilename);
	
}