#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRING PARSING
//---------------------------------------------------------------------------------------------------------------------------
IntVector2 XMLUtils::ParseIntVector2FromXML(const String& attribValue) {
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, ',', tokens);

	return IntVector2(stoi(tokens[0]), stoi(tokens[1]));
}
//---------------------------------------------------------------------------------------------------------------------------
int XMLUtils::ParseIntFromXML(const String& attribValue) {
	return stoi(attribValue);
}
//---------------------------------------------------------------------------------------------------------------------------
char XMLUtils::ParseCharFromXML(const String& attribValue) {
	return attribValue[0];
}
//---------------------------------------------------------------------------------------------------------------------------
RGBA XMLUtils::ParseHexColorFromXML(const String& attribValue) {

	String strToParse = StringUtils::GetRidOfAllOneCharacter(attribValue, ' ');

	//Should look like: (0, 255, 128, 100)
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(strToParse, ',', tokens);

	return RGBA((float)stoi(tokens[0]) / 255.f, (float)stoi(tokens[1]) / 255.f, (float)stoi(tokens[2]) / 255.f, (float)stoi(tokens[3]) / 255.f);
}
//---------------------------------------------------------------------------------------------------------------------------
RGBA XMLUtils::ParseFloatColorFromXML(const String& attribValue) {
	String strToParse = StringUtils::GetRidOfAllOneCharacter(attribValue, ' ');

	//Should look like: (0, 255, 128, 100)
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(strToParse, ',', tokens);

	return RGBA(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
}
//---------------------------------------------------------------------------------------------------------------------------
RangeInt XMLUtils::ParseRangeIntFromXML(const String& attribValue) {
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, '~', tokens);

	return (tokens.size() == 1) ? (RangeInt(stoi(tokens[0]), stoi(tokens[0]))) : (RangeInt(stoi(tokens[0]), stoi(tokens[1])));
}
//---------------------------------------------------------------------------------------------------------------------------
RangeInt XMLUtils::ParseRangeIntFromXML(const String& attribValue, char delimiter) {
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, delimiter, tokens);

	return (tokens.size() == 1) ? (RangeInt(stoi(tokens[0]), stoi(tokens[0]))) : (RangeInt(stoi(tokens[0]), stoi(tokens[1])));
}
//---------------------------------------------------------------------------------------------------------------------------
float XMLUtils::ParseFloatFromXML(const String& attribValue) {
	float retVal = 0.f;

	try {
		retVal = std::stof(attribValue);
	}
	catch (int e) {
		UNREFERENCED_PARAMETER(e);
		ERROR_AND_DIE("ERROR: Could not parse float from XML attribute value.");
	}

	return retVal;
}

//---------------------------------------------------------------------------------------------------------------------------
//ATTRIBUTES
//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::GetAttributeByName(const String& name, const XMLNode& node, XMLUtils::Attribute& outAttrib) {

	for (int i = 0; i < node.nAttribute(); i++) {
		String attrib = node.getAttributeValue(i);
		if (name == attrib) {
			outAttrib = XMLUtils::Attribute(node.getAttributeName(i), node.getAttributeValue(i));
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::DoesAttributeExist(const String& attribName, const std::vector<Attribute>& attribPairs) {
	bool exists = false;

	for (unsigned int i = 0; i < attribPairs.size(); i++) {
		String attribNameLower;
		StringUtils::ToLowerCase(attribNameLower);

		if (attribPairs[i].name == attribName) {
			exists = true;
		}
	}

	return exists;
}
//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::GetAllAttributesOfNode(const XMLNode& node, std::vector<Attribute>& outAttribPairs) {

	if (!node.isEmpty()) {
		for (int i = 0; i < node.nAttribute(); i++) {
			outAttribPairs.push_back(Attribute(node.getAttributeName(i), node.getAttributeValue(i)));
		}
		return true;
	}
	else {
		DebuggerPrintf("XMLUtils: Cannot get attributes from NULL node.");
		return false;
	}
}