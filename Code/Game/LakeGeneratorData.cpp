#include "Game/LakeGeneratorData.hpp"

STATIC EnvironmentGeneratorDataRegistration LakeGeneratorData::s_LakeGenDataRegistration("Lake", &LakeGeneratorData::CreateGeneratorData);

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
LakeGeneratorData::LakeGeneratorData(const String& name, int steps, const String& type)
	: EnvironmentGeneratorData(name, steps)
	, m_tileType(type)
{ }

STATIC EnvironmentGeneratorData* LakeGeneratorData::CreateGeneratorData(const XMLNode& envGenDataNode) {

	std::vector<XMLUtils::Attribute> lakeAttribs;
	XMLUtils::GetAllAttributesOfNode(envGenDataNode, lakeAttribs);

	String name = "ERROR";
	int steps = 0;
	String tileType = "water";

	for (unsigned int i = 0; i < lakeAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = lakeAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;
		StringUtils::ToLowerCase(currAttribName);
		//StringUtils::ToLowerCase(currAttribVal);

		if (currAttribName == "generator") {
			name = currAttribVal;
		}
		else if (currAttribName == "steps") {
			steps = XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "type") {
			tileType = currAttribVal;
		}
	}

	return new LakeGeneratorData(name, steps, tileType);
}