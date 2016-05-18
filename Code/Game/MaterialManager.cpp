#include "Game/MaterialManager.hpp"

MaterialManager* g_theMaterialManager = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
MaterialManager::MaterialManager() {
	InitializeMaterials();
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void MaterialManager::UpdateUniformOnMaterial(const String& materialName, Uniform* uniform) {
	ASSERT_OR_DIE(uniform != nullptr, "NULL UNIFORM PASSED IN IN UPDATEUNIFORMFORMATERIAL");

	std::map<string, Material*>::iterator matIterator = m_materials.find(materialName);

	Material* mat = matIterator->second;

	for (unsigned int i = 0; i < mat->m_uniforms.size(); i++) {
		Uniform* currUniform = mat->m_uniforms[i];

		if (currUniform->GetName() == uniform->GetName()) {
			mat->m_uniforms[i] = uniform;
		}
	}
}
void MaterialManager::UpdateUniformOnAllMaterials(Uniform* uniform) {
	std::map<string, Material*>::iterator matIterator;

	for (matIterator = m_materials.begin(); matIterator != m_materials.end(); ++matIterator) {
		Material* currMat = matIterator->second;

		for (unsigned int i = 0; i < currMat->m_uniforms.size(); i++) {
			Uniform* currUniform = currMat->m_uniforms[i];

			if (currUniform->GetName() == uniform->GetName()) {
				currMat->m_uniforms[i] = uniform;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
Material* MaterialManager::GetMaterial(const String& materialName) {
	std::map<string, Material*>::iterator matIterator = m_materials.find(materialName);

	if (matIterator == m_materials.end()) {
		return nullptr;
	}
	else {
		return matIterator->second;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
void MaterialManager::InitializeMaterials() {
	CreateFBOMaterial_1();
}

void MaterialManager::CreateFBOMaterial_1() {
	std::vector<Uniform*> fboUniforms;

	//Sampler
	Sampler2D fboSampler;
	fboSampler.AddTexture("gTexDiffuse", Texture::CreateOrGetTexture(0, 1600, 900, eTextureFormat_RGBA8));
	fboSampler.AddTexture("gTexDepth", Texture::CreateOrGetTexture(1, 1600, 900, eTextureFormat_D24S8));
	Uniform* fboSamplerUniform = new Uniform_SAMPLER2D(fboSampler);
	fboUniforms.push_back(fboSamplerUniform);

	//General uniforms
	fboUniforms.push_back(new Uniform_MAT4("gModel", Matrix4::IDENTITY));
	fboUniforms.push_back(new Uniform_MAT4("gView", Matrix4::IDENTITY));
	fboUniforms.push_back(new Uniform_MAT4("gProj", Matrix4::IDENTITY));

	fboUniforms.push_back(new Uniform_FLOAT("gAge", 0.f));

	Material* fbo = new Material("Data/Shaders/post_process1.vert", "Data/Shaders/post_process1.frag", fboUniforms);
	m_materials.insert(std::pair<string, Material*>("fbo1", fbo));
}