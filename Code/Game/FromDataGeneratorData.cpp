#include "Game/FromDataGeneratorData.hpp"

STATIC EnvironmentGeneratorDataRegistration FromDataGeneratorData::s_fromDataGenDataRegistration("FromData", &FromDataGeneratorData::CreateGeneratorData);

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
FromDataGeneratorData::FromDataGeneratorData(const String& name, const String& fileName)
	: EnvironmentGeneratorData(name, 1)
	, m_fileName(fileName)
{ 
	
}

String FromDataGeneratorData::GetFileName() const {
	return m_fileName;
}


//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC EnvironmentGeneratorData* FromDataGeneratorData::CreateGeneratorData(const XMLNode& envGenDataNode) {

	String generatorName = envGenDataNode.getAttribute(0).lpszValue;
	String fileName = envGenDataNode.getAttribute(1).lpszValue;

	return new FromDataGeneratorData(generatorName, fileName);
}