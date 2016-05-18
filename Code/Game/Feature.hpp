#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/General/Utils/GeneralUtils/BitUtils.hpp"
#include "Engine/General/Utils/StringUtils.hpp"
#include "Game/Entity.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//ENUMS
//---------------------------------------------------------------------------------------------------------------------------
enum eFeatureType {
	FEATURE_TYPE_DOOR = 0,
	FEATURE_TYPE_PLANT,
	FEATURE_TYPE_BONFIRE,
	FEATURE_TYPE_NUM_TYPES
};
enum eFeatureState {
	FEATURE_STATE_ACTIVATED = 0,
	FEATURE_STATE_DEACTIVATED,
	FEATURE_STATE_OPEN,
	FEATURE_STATE_CLOSED,
	FEATURE_STATE_TRIGGERED,
	FEATURE_STATE_NUM_STATES
};

enum eActivatedProperties {
	ACTIVATED_BLOCKS_LOS = 1 << 0,
	ACTIVATED_BLOCKS_PATHING = 2 << 0
};
enum eDeactivatedProperties {
	DEACTIVATED_BLOCKS_LOS = 1 << 0,
	DEACTIVATED_BLOCKS_PATHING = 2 << 0
};

//---------------------------------------------------------------------------------------------------------------------------
//FEATURE CLASS
//---------------------------------------------------------------------------------------------------------------------------
class Feature : public Entity {
public:
	//STRUCTORS
	Feature() { }
	Feature(const XMLNode& node);
	Feature(Feature* templateFeature);

	void PopulateFromXMLNode(const XMLNode& node);

	//UPDATE RENDER
	void Run();
	void Update(float deltaSeconds);
	void Render() const;

	//USING
	void Toggle();

	//LOGGING
	void LogActivation();
	void LogDeactivation();
	String GetInfoText();

	//GETTERS SETTERS
	eFeatureType GetFeatureType() const { return m_featureType; }
	void SetFeature(eFeatureType type) { m_featureType = type; }
	eFeatureState GetFeatureState() const { return m_featureState; }
	void SetFeatureState(eFeatureState state) { m_featureState = state; }

	char GetGlyphActivated() const { return m_glyphActivated; }
	void SetGlyphActivated(char glyphActivated) { m_glyphActivated = glyphActivated; }
	char GetGlyphDeactivated() const { return m_glyphDeactivated; }
	void SetGlyphDeactivated(char glyphDeactivated) { m_glyphDeactivated = glyphDeactivated; }

	RGBA GetColorActivated() const { return m_colorActivated; }
	void SetColorActivated(const RGBA& col) { m_colorActivated = col; }
	RGBA GetColorDeactivated() const { return m_colorDeactivated; }
	void SetColorDeactivated(const RGBA& col) { m_colorDeactivated = col; }

	//CHECKS AGAINST FEATURE
	bool IsCurrentlyBlockingPathing() const;
	bool IsCurrentlyBlockingLOS() const;


	//SAVE LOADING
	VIRTUAL XMLNode SaveToXML() const;
	void LoadFromXML(const XMLNode& node);

	VIRTUAL void ResolveEntityPointers();



	//Public since feature specific
	bool m_activeBlocksLOS;
	bool m_deactiveBlocksLOS;
	bool m_activeBlocksPathing;
	bool m_deactiveBlocksPathing;

private:
	eFeatureType m_featureType;
	eFeatureState m_featureState;

	char m_glyphActivated;
	char m_glyphDeactivated;
	RGBA m_colorActivated;
	RGBA m_colorDeactivated;

	//INHERITED
	//IntVector2 m_position;
	//char m_glyph;
	//RGBA m_color;
	//RGBA m_backgroundColor;
	//String m_name;
	//uint m_id;
	//Map* m_belongingMap;
	//BitmapFont* m_squirrelFont;
};
