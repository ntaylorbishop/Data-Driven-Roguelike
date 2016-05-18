#include "Game/Feature.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/Map.hpp"
#include "Game/TheWorld.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Feature::Feature(const XMLNode& node) 
	: Entity()
{
	PopulateFromXMLNode(node);
}
Feature::Feature(Feature* templateFeature) {
	m_position = templateFeature->GetPosition();
	m_glyph = templateFeature->GetGlyph();
	m_color = templateFeature->GetColor();
	m_backgroundColor = templateFeature->GetBackgroundColor();
	m_name = templateFeature->GetName();
	m_belongingMap = templateFeature->GetBelongingMap();
	m_squirrelFont = templateFeature->m_squirrelFont;

	m_featureType = templateFeature->GetFeatureType();
	m_featureState = templateFeature->GetFeatureState();

	m_glyphActivated = templateFeature->GetGlyphActivated();
	m_glyphDeactivated = templateFeature->GetGlyphDeactivated();
	m_colorActivated = templateFeature->GetColorActivated();
	m_colorDeactivated = templateFeature->GetColorDeactivated();

	m_activeBlocksLOS = templateFeature->m_activeBlocksLOS;
	m_deactiveBlocksLOS = templateFeature->m_deactiveBlocksLOS;
	m_activeBlocksPathing = templateFeature->m_activeBlocksPathing;
	m_deactiveBlocksPathing = templateFeature->m_deactiveBlocksPathing;

	m_id = s_entityIDs++;
}

void Feature::PopulateFromXMLNode(const XMLNode& templateDataNode) {

	String name = FEATURE_DEFAULT_NAME;
	eFeatureType featureType = FEATURE_TYPE_NUM_TYPES;
	char glyphActivated = '?';
	char glyphDeactivated = '?';
	RangeInt blockLOS = RangeInt(0, 0);
	RangeInt blockPathing = RangeInt(0, 0);
	RGBA colActivated = RGBA::WHITE;
	RGBA colDeactivated = RGBA::WHITE;

	std::vector<XMLUtils::Attribute> featureAttribs;
	XMLUtils::GetAllAttributesOfNode(templateDataNode, featureAttribs);

	for (unsigned int i = 0; i < featureAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = featureAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);

		if (currAttrib.name == "name") {
			name = currAttrib.value;
		}
		else if (currAttrib.name == "type") {
			String type = currAttrib.value;
			StringUtils::ToLowerCase(type);

			if (type == "door") {
				featureType = FEATURE_TYPE_DOOR;
			}
		}
		else if (currAttrib.name == "stateglyphs") {
			String glyphs = currAttrib.value;

			std::vector<String> glyphToks;
			StringUtils::GetRidOfAllOneCharacter(glyphs, ' ');
			StringUtils::ToLowerCase(glyphs);
			StringUtils::TokenizeBuffer(glyphs, ',', glyphToks);

			glyphActivated = glyphToks[0][0];
			glyphDeactivated = glyphToks[1][0];
		}
		else if (currAttrib.name == "doesblocklineofsight") {
			blockLOS = XMLUtils::ParseRangeIntFromXML(currAttrib.value, ',');
		}
		else if (currAttrib.name == "doesblockpathing") {
			blockPathing = XMLUtils::ParseRangeIntFromXML(currAttrib.value, ',');
		}
		else if (currAttrib.name == "coloractivated") {
			colActivated = XMLUtils::ParseHexColorFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "colordeactivated") {
			colDeactivated = XMLUtils::ParseHexColorFromXML(currAttrib.value);
		}
	}

	m_name = name;
	m_featureType = featureType;
	m_glyphActivated = glyphActivated;
	m_glyphDeactivated = glyphDeactivated;
	m_colorActivated = colActivated;
	m_colorDeactivated = colDeactivated;

	m_activeBlocksLOS       = (blockLOS.GetMin() == 1)     ? (true) : (false);
	m_deactiveBlocksLOS     = (blockLOS.GetMax() == 1)     ? (true) : (false);
	m_activeBlocksPathing   = (blockPathing.GetMin() == 1) ? (true) : (false);
	m_deactiveBlocksPathing = (blockPathing.GetMax() == 1) ? (true) : (false);

	m_backgroundColor = RGBA(0.5f, 0.5f, 0.5f, 1.f);
	m_color = RGBA::MAGENTA;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void Feature::Run() {

}
void Feature::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	switch (m_featureType) {
	case FEATURE_TYPE_DOOR:
		if (m_featureState == FEATURE_STATE_ACTIVATED) {
			m_glyph = m_glyphActivated;
			m_color = m_colorActivated;
		}
		else if (m_featureState == FEATURE_STATE_DEACTIVATED) {
			m_glyph = m_glyphDeactivated;
			m_color = m_colorDeactivated;
		}
		break;
	default:
		break;
	}
}
void Feature::Render() const {
	//m_belongingMap->RenderGlyphAtLocation(m_glyph, m_position, m_color);
}

//---------------------------------------------------------------------------------------------------------------------------
//USING
//---------------------------------------------------------------------------------------------------------------------------
void Feature::Toggle() {
	switch (m_featureType) {
	case FEATURE_TYPE_DOOR:
		if (m_featureState == FEATURE_STATE_ACTIVATED) {
			m_featureState = FEATURE_STATE_DEACTIVATED;
			LogActivation();
		}
		else if (m_featureState == FEATURE_STATE_DEACTIVATED) {
			m_featureState = FEATURE_STATE_ACTIVATED;
			LogDeactivation();
		}
		else {
			DebuggerPrintf("WARNING: Door is in a state not activated or deactivated");
		}
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//LOGGING
//---------------------------------------------------------------------------------------------------------------------------
void Feature::LogActivation() {
	switch (m_featureType) {
	case FEATURE_TYPE_DOOR:
		g_theMessageLog->LogMessage(StringUtils::Stringf("You slowly open the %s", m_name.c_str()), PRIORITY_HIGH);
		break;
	}
}
void Feature::LogDeactivation() {
	switch (m_featureType) {
	case FEATURE_TYPE_DOOR:
		g_theMessageLog->LogMessage(StringUtils::Stringf("You slowly close the %s", m_name.c_str()), PRIORITY_HIGH);
		break;
	}
}
String Feature::GetInfoText() {
	switch (m_featureType) {
	case FEATURE_TYPE_DOOR:
		if (m_featureState == FEATURE_STATE_ACTIVATED) {
			return "Currently closed.";
		}
		else if (m_featureState == FEATURE_STATE_DEACTIVATED) {
			return "Currently open.";
		}
		else {
			return "ERROR: door in unsupported feature state.";
		}
		break;
	default:
		return "ERROR: Unsupported feature type";
		break;
	}

	return "ERROR: Should not have hit this line in feature GetInfoText()";
}

//---------------------------------------------------------------------------------------------------------------------------
//CHECKS AGAINST FEATURE
//---------------------------------------------------------------------------------------------------------------------------
bool Feature::IsCurrentlyBlockingPathing() const {
	if (m_featureType == FEATURE_TYPE_DOOR) {
		if (m_featureState == FEATURE_STATE_ACTIVATED) {
			return m_activeBlocksPathing;
		}
		else if(m_featureState == FEATURE_STATE_DEACTIVATED) {
			return m_deactiveBlocksPathing;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
bool Feature::IsCurrentlyBlockingLOS() const {
	if (m_featureType == FEATURE_TYPE_DOOR) {
		if (m_featureState == FEATURE_STATE_ACTIVATED) {
			return m_activeBlocksLOS;
		}
		else if (m_featureState == FEATURE_STATE_DEACTIVATED) {
			return m_deactiveBlocksLOS;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVE LOADING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL XMLNode Feature::SaveToXML() const {
	XMLNode featureBlueprint;
	featureBlueprint = featureBlueprint.createXMLTopNode("FeatureBlueprint");

	featureBlueprint.addAttribute("name", m_name.c_str());
	featureBlueprint.addAttribute("position", m_position.ToString().c_str());
	featureBlueprint.addAttribute("glyphActivated", StringUtils::ToString(m_glyphActivated).c_str());
	featureBlueprint.addAttribute("glyphDeactivated", StringUtils::ToString(m_glyphDeactivated).c_str());
	featureBlueprint.addAttribute("colorActivated", m_colorActivated.ToString().c_str());
	featureBlueprint.addAttribute("colorDeactivated", m_colorDeactivated.ToString().c_str());

	featureBlueprint.addAttribute("featureType", std::to_string(m_featureType).c_str());
	featureBlueprint.addAttribute("featureState", std::to_string(m_featureState).c_str());

	featureBlueprint.addAttribute("activeBlocksLOS", StringUtils::ToString(m_activeBlocksLOS).c_str());
	featureBlueprint.addAttribute("deactiveBlocksLOS", StringUtils::ToString(m_deactiveBlocksLOS).c_str());
	featureBlueprint.addAttribute("activeBlocksPathing", StringUtils::ToString(m_activeBlocksPathing).c_str());
	featureBlueprint.addAttribute("deactiveBlocksPathing", StringUtils::ToString(m_deactiveBlocksPathing).c_str());

	return featureBlueprint;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOADING
//---------------------------------------------------------------------------------------------------------------------------
void Feature::LoadFromXML(const XMLNode& node) {

	std::vector<XMLUtils::Attribute> featureAttribs;
	XMLUtils::GetAllAttributesOfNode(node, featureAttribs);

	for (unsigned int i = 0; i < featureAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = featureAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;

		if (currAttribName == "name") {
			m_name = currAttribVal;
		}
		else if (currAttribName == "position") {
			m_position = XMLUtils::ParseIntVector2FromXML(currAttribVal);
		}
		else if (currAttribName == "glyphActivated") {
			m_glyphActivated = XMLUtils::ParseCharFromXML(currAttribVal);
		}
		else if (currAttribName == "glyphDeactivated") {
			m_glyphDeactivated = XMLUtils::ParseCharFromXML(currAttribVal);
		}
		else if (currAttribName == "colorActivated") {
			m_colorActivated = XMLUtils::ParseFloatColorFromXML(currAttribVal);
		}
		else if (currAttribName == "colorDeactivated") {
			m_colorDeactivated = XMLUtils::ParseFloatColorFromXML(currAttribVal);
		}
		else if (currAttribName == "featureType") {
			m_featureType = (eFeatureType)XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "featureState") {
			m_featureState = (eFeatureState)XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "activeBlocksLOS") {
			m_activeBlocksLOS = XMLUtils::ParseIntFromXML(currAttribVal) != 0;
		}
		else if (currAttribName == "deactiveBlocksLOS") {
			m_deactiveBlocksLOS = XMLUtils::ParseIntFromXML(currAttribVal) != 0;
		}
		else if (currAttribName == "activeBlocksPathing") {
			m_activeBlocksPathing = XMLUtils::ParseIntFromXML(currAttribVal) != 0;
		}
		else if (currAttribName == "deactiveBlocksPathing") {
			m_deactiveBlocksPathing = XMLUtils::ParseIntFromXML(currAttribVal) != 0;
		}
	}

	m_id = s_entityIDs++;
	m_backgroundColor = RGBA(0.5f, 0.5f, 0.5f, 1.f);

	Tile* tileToPlaceOn = g_theWorld->m_activeMap->GetTileAtLocation(m_position);
	tileToPlaceOn->SetFeature(this);
	g_theWorld->m_entitiesOnMap.push_back(this);
}

VIRTUAL void Feature::ResolveEntityPointers() { }