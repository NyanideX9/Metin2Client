#pragma once
#include <thread>
#include <array>

#include "../gamelib/MapManager.h"
#include "PythonItem.h"
#include "InstanceBase.h"
#include "PythonPlayer.h"
#include "locale_inc.h"
#include "PythonSkill.h" //dodane
#include "../GameLib/FlyingObjectManager.h" //doda³em
#include "GameType.h" //doda³em
#include "../GameLib/RaceData.h" //doda³em
#include "../GameLib/RaceMotionData.h" //doda³em
#include "../GameLib/RaceManager.h" //dodalem

static bool loaded = false;
static const char* NPC_LIST_FILE_NAME = "npclist.txt";

class CPythonPlayerSettingsModule : public CMapManager, public CSingleton<CPythonPlayerSettingsModule>
{
public:
	CPythonPlayerSettingsModule();
	virtual ~CPythonPlayerSettingsModule() = default;
	void Load();
	
public:
	void RegisterSkills();
	void RegisterSkills(uint32_t race, uint8_t skill_group, uint8_t empire);
	void __RegisterDungeonMapNames();
	void __RegisterTitleNames();
	void __RegisterNameColors();
	void __RegisterTitleColors();
	void __RegisterEmotionIcons();
	void __LoadGameInit();
	void __LoadGameSound();
	void __LoadGameEffects();
	void __LoadGameNPC();
	void __LoadEmoticons();
	// void __LoadRaceHeight();
	void __RegisterCacheMotionData(CRaceData* pRaceData, const uint16_t wMotionMode, const uint16_t wMotionIndex, const char* c_szFileName, const uint8_t byPercentage = 100);
	void __SetIntroMotions(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __SetGeneralMotions(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName);
	void __LoadGameRace(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadFishing(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadWedding(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadActions(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGuildSkill(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadWeaponMotion(CRaceData* pRaceData, WORD wMotionMode, const std::string& c_rstrFolderName);
	void __LoadComboRace(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName);
	void __LoadCombo(CRaceData* pRaceData, const uint16_t wMotionMode);
	void __LoadGameWarriorEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameAssassinEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameSuraEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
	void __LoadGameShamanEx(CRaceData* pRaceData, const std::string& c_rstrFolderName);
#ifdef ENABLE_WOLFMAN_CHARACTER
	void __LoadGameWolfmanEx(CRaceData* pRaceData);
#endif

public:
	bool RegisterGuildBuildingList(const char* filepath);
		int32_t GetCharacterTitleColor(uint8_t index); //dodane

protected:
	std::thread m_loadThread;
	bool isLoaded;
	
private:
	enum EGuildBuildingListToken
	{
		GUIILD_BUILDING_TOKEN_VNUM,
		GUIILD_BUILDING_TOKEN_TYPE,
		GUIILD_BUILDING_TOKEN_NAME,
		GUIILD_BUILDING_TOKEN_LOCAL_NAME,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_1,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_2,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_3,
		GUIILD_BUILDING_NO_USE_TOKEN_SIZE_4,
		GUIILD_BUILDING_TOKEN_X_ROT_LIMIT,
		GUIILD_BUILDING_TOKEN_Y_ROT_LIMIT,
		GUIILD_BUILDING_TOKEN_Z_ROT_LIMIT,
		GUIILD_BUILDING_TOKEN_PRICE,
		GUIILD_BUILDING_TOKEN_MATERIAL,
		GUIILD_BUILDING_TOKEN_NPC,
		GUIILD_BUILDING_TOKEN_GROUP,
		GUIILD_BUILDING_TOKEN_DEPEND_GROUP,
		GUIILD_BUILDING_TOKEN_ENABLE_FLAG,
		GUIILD_BUILDING_LIMIT_TOKEN_COUNT,
	} EGuildBuildingListToken;

	std::map<std::string_view, std::string> m_buildingTypeToFolder{
		{"HEADQUARTER", "headquarter"},
		{"FACILITY", "facility"},
		{"OBJECT", "object"},
		{"WALL", "fence"},
	};
	
public:
	void startLoadThread()
	{
		if (!isLoaded)
			m_loadThread = std::thread(&CPythonPlayerSettingsModule::Load, this);
	}
};
	
static const std::map<uint8_t, const char*> emotionIcons =
{
	{ CPythonPlayer::EMOTION_CLAP, "d:/ymir work/ui/game/windows/emotion_clap.sub" },
	{ CPythonPlayer::EMOTION_CHEERS_1, "d:/ymir work/ui/game/windows/emotion_cheers_1.sub" },
	{ CPythonPlayer::EMOTION_CHEERS_2, "d:/ymir work/ui/game/windows/emotion_cheers_2.sub" },
	{ CPythonPlayer::EMOTION_DANCE_1, "icon/action/dance1.tga" },
	{ CPythonPlayer::EMOTION_DANCE_2, "icon/action/dance2.tga" },
	{ CPythonPlayer::EMOTION_CONGRATULATION, "icon/action/congratulation.tga" },
	{ CPythonPlayer::EMOTION_FORGIVE, "icon/action/forgive.tga" },
	{ CPythonPlayer::EMOTION_ANGRY, "icon/action/angry.tga" },
	{ CPythonPlayer::EMOTION_ATTRACTIVE, "icon/action/attractive.tga" },
	{ CPythonPlayer::EMOTION_SAD, "icon/action/sad.tga" },
	{ CPythonPlayer::EMOTION_SHY, "icon/action/shy.tga" },
	{ CPythonPlayer::EMOTION_CHEERUP, "icon/action/cheerup.tga" },
	{ CPythonPlayer::EMOTION_BANTER, "icon/action/banter.tga" },
	{ CPythonPlayer::EMOTION_JOY, "icon/action/joy.tga" },
	{ CPythonPlayer::EMOTION_DANCE_1, "icon/action/dance1.tga" },
	{ CPythonPlayer::EMOTION_DANCE_2, "icon/action/dance2.tga" },
	{ CPythonPlayer::EMOTION_DANCE_3, "icon/action/dance3.tga" },
	{ CPythonPlayer::EMOTION_DANCE_4, "icon/action/dance4.tga" },
	{ CPythonPlayer::EMOTION_DANCE_5, "icon/action/dance5.tga" },
	{ CPythonPlayer::EMOTION_DANCE_6, "icon/action/dance6.tga" },
	{ CPythonPlayer::EMOTION_KISS, "d:/ymir work/ui/game/windows/emotion_kiss.sub" },
	{ CPythonPlayer::EMOTION_FRENCH_KISS, "d:/ymir work/ui/game/windows/emotion_french_kiss.sub" },
	{ CPythonPlayer::EMOTION_SLAP, "d:/ymir work/ui/game/windows/emotion_slap.sub" },
};

//static const std::array<std::string, 9> titleNameList { "Rycerski", "Szlachetny", "Dobry", "Przyjazny", "Neutralny", "Agresywny", "Nieuczciwy", "Z³oœliwy", "Okrutny" };
static const std::array<std::string, 14> titleNameList { "Legenda", "Pogromca", "Heroiczny", "Elitarny", "Waleczny", "Rycerski", "Szlachetny", "Dobry", "Przyjazny", "Neutralny", "Agresywny", "Nieuczciwy", "Z³oœliwy", "Okrutny" };


	
typedef std::array<uint8_t, 3> RGB;
typedef std::map<uint8_t, RGB> TCharacterColorMap;

static const TCharacterColorMap characterNameColors
{
	{ CInstanceBase::NAMECOLOR_NORMAL_PC, { 255, 215, 76 }},
	{ CInstanceBase::NAMECOLOR_NORMAL_NPC, { 122, 231, 93 }},
	{ CInstanceBase::NAMECOLOR_NORMAL_MOB, { 235, 22, 9 }},
	//{ CInstanceBase::NAMECOLOR_BOSS, { 255, 215, 0 }},
	{ CInstanceBase::NAMECOLOR_PVP, { 238, 54, 223 }},
	{ CInstanceBase::NAMECOLOR_PK, { 180, 100, 0 }},
	{ CInstanceBase::NAMECOLOR_PARTY, { 128, 192, 255 }},
	{ CInstanceBase::NAMECOLOR_WARP, { 136, 218, 241 }},
	{ CInstanceBase::NAMECOLOR_WAYPOINT, { 255, 255, 255 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_MOB, { 235, 22, 9 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_NPC, { 122, 231, 93 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_PC + 1, { 157, 0, 0 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_PC + 2, { 222, 160, 47 }},
	{ CInstanceBase::NAMECOLOR_EMPIRE_PC + 3, { 23, 30, 138 }},
	//#ifdef ENABLE_COMPANION_SYSTEM
	//{ CInstanceBase::NAMECOLOR_COMPANION, { 153, 255, 255 }},
	//#endif
};



static const TCharacterColorMap characterTitleColors
{
	{ 0, { 240, 231, 224 }},  // piec — jasny burgund
	{ 1, { 235, 173, 119 }},   // cztery — jasny bordowy / karmazyn
	{ 2, { 163, 194, 194 }},  // trzy — jaœniejszy fiolet (blue violet)
	{ 3, { 166, 230, 82 }},   // dwa — zieleñ liœciasta
	{ 4, { 230, 127, 82 }},  // jeden — royal blue
	{ 5, { 0, 204, 255 }}, //rycerski
	{ 6, { 0, 144, 255 }}, //szlachetny
	{ 7, { 92, 110, 255 }}, //dobry
	{ 8, { 155, 155, 255 }}, //przyjazny
	{ 9, { 255, 255, 255 }}, //neutral
	{ 10, { 207, 117, 0 }}, // agresywny
	{ 11, { 235, 83, 0 }}, //nieuczciwy
	{ 12, { 227, 0, 0 }}, //z³oœliwy
	{ 13, { 255, 0, 0 }}, //okrutny
};



typedef std::tuple<uint32_t, const char*, const char*, bool> TEffectList;
static const std::vector<TEffectList> vecEffectList =
{
	{ CInstanceBase::EFFECT_SPAWN_APPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_appear.mse", true},
	{ CInstanceBase::EFFECT_SPAWN_DISAPPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_die.mse", true},
	{ CInstanceBase::EFFECT_FLAME_ATTACK, "equip_right_hand", "d:/ymir work/effect/hit/blow_flame/flame_3_weapon.mse", true},
	{ CInstanceBase::EFFECT_FLAME_HIT, "", "d:/ymir work/effect/hit/blow_flame/flame_3_blow.mse", true},
	{ CInstanceBase::EFFECT_FLAME_ATTACH, "", "d:/ymir work/effect/hit/blow_flame/flame_3_body.mse", true},
	{ CInstanceBase::EFFECT_ELECTRIC_ATTACK, "equip_right", "d:/ymir work/effect/hit/blow_electric/light_1_weapon.mse", true},
	{ CInstanceBase::EFFECT_ELECTRIC_HIT, "", "d:/ymir work/effect/hit/blow_electric/light_1_blow.mse", true},
	{ CInstanceBase::EFFECT_ELECTRIC_ATTACH, "", "d:/ymir work/effect/hit/blow_electric/light_1_body.mse", true},
	{ CInstanceBase::EFFECT_LEVELUP, "", "d:/ymir work/effect/etc/levelup_1/level_up.mse", true},
	{ CInstanceBase::EFFECT_SKILLUP, "", "d:/ymir work/effect/etc/skillup/skillup_1.mse", true},
	{ CInstanceBase::EFFECT_EMPIRE + 1, "Bip01", "d:/ymir work/effect/etc/empire/empire_A.mse", true},
	{ CInstanceBase::EFFECT_EMPIRE + 2, "Bip01", "d:/ymir work/effect/etc/empire/empire_B.mse", true},
	{ CInstanceBase::EFFECT_EMPIRE + 3, "Bip01", "d:/ymir work/effect/etc/empire/empire_C.mse", true},
	{ CInstanceBase::EFFECT_WEAPON + 1, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_sword_loop.mse", true},
	{ CInstanceBase::EFFECT_WEAPON + 2, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_spear_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 0, "Bip01", "locale/pl/effect/gm.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 3, "Bip01", "d:/ymir work/effect/hit/blow_poison/poison_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 4, "", "d:/ymir work/effect/affect/slow.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 5, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 6, "", "d:/ymir work/effect/etc/ready/ready.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 16, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 17, "", "d:/ymir work/pc/assassin/effect/gyeonggong_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 19, "Bip01 R Finger2", "d:/ymir work/pc/sura/effect/gwigeom_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 20, "", "d:/ymir work/pc/sura/effect/fear_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 21, "", "d:/ymir work/pc/sura/effect/jumagap_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 22, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 23, "", "d:/ymir work/pc/shaman/effect/boho_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 24, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 25, "", "d:/ymir work/pc/sura/effect/heuksin_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 26, "", "d:/ymir work/pc/sura/effect/muyeong_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 28, "Bip01", "d:/ymir work/effect/hit/blow_flame/flame_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 29, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 30, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 32, "Bip01 Head", "d:/ymir work/pc/sura/effect/pabeop_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 33, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 35, "", "d:/ymir work/effect/etc/guild_war_flag/flag_red.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 36, "", "d:/ymir work/effect/etc/guild_war_flag/flag_blue.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 37, "", "d:/ymir work/effect/etc/guild_war_flag/flag_yellow.mse", true},
#ifdef ENABLE_WOLFMAN_CHARACTER
	{ CInstanceBase::EFFECT_AFFECT + 42, "Bip01", "d:/ymir work/effect/hit/blow_poison/bleeding_loop.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 43, "Bip01", "d:/ymir work/effect/hit/blow_flame/flame_loop_w.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 44, "", "d:/ymir work/pc3/common/effect/gyeokgongjang_loop_w.mse", true},
#endif
	{ CInstanceBase::EFFECT_AFFECT + 45, "Bip01", "d:/ymir work/effect/etc/beer/beer.mse", true},
	{ CInstanceBase::EFFECT_AFFECT + 46, "Bip01", "d:/ymir work/effect/etc/summer2022/summer2022.mse", true},

	{ CInstanceBase::EFFECT_REFINED + 1, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 2, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 3, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 4, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 5, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_8_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 6, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_9_b.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 7, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 8, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 9, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9_f.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 10, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 11, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 12, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 13, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 14, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_8_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 15, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_9_s.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 16, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_7.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 17, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_8.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 18, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_9.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 19, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-4-2-1.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 20, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-4-2-2.mse", true},
	{ CInstanceBase::EFFECT_REFINED + 21, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-5-1.mse", true}, // EFFECT_BODYARMOR_FIRE
	{ CInstanceBase::EFFECT_REFINED + 22, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-5_jin.mse", true}, // EFFECT_BODYARMOR_FIRE2
	{ CInstanceBase::EFFECT_DUST, "", "d:/ymir work/effect/etc/dust/dust.mse", true},
	{ CInstanceBase::EFFECT_HORSE_DUST, "", "d:/ymir work/effect/etc/dust/running_dust.mse", true},
	{ CInstanceBase::EFFECT_HIT, "", "d:/ymir work/effect/hit/blow_1/blow_1_low.mse", true},
	{ CInstanceBase::EFFECT_HPUP_RED, "", "d:/ymir work/effect/etc/recuperation/drugup_red.mse", true},
	{ CInstanceBase::EFFECT_SPUP_BLUE, "", "d:/ymir work/effect/etc/recuperation/drugup_blue.mse", true},
	{ CInstanceBase::EFFECT_SPEEDUP_GREEN, "", "d:/ymir work/effect/etc/recuperation/drugup_green.mse", true},
	{ CInstanceBase::EFFECT_DXUP_PURPLE, "", "d:/ymir work/effect/etc/recuperation/drugup_purple.mse", true},
	{ CInstanceBase::EFFECT_AUTO_HPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_red.mse", true},
	{ CInstanceBase::EFFECT_AUTO_SPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_blue.mse", true},
	{ CInstanceBase::EFFECT_RAMADAN_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item1.mse", true},
	{ CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item2.mse", true},
	{ CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item3.mse", true},
	{ CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item4.mse", true},
	{ CInstanceBase::EFFECT_FIRECRACKER, "", "d:/ymir work/effect/etc/firecracker/newyear_firecracker.mse", true},
	{ CInstanceBase::EFFECT_SPIN_TOP, "", "d:/ymir work/effect/etc/firecracker/paing_i.mse", true},
	{ CInstanceBase::EFFECT_SELECT, "", "d:/ymir work/effect/etc/click/click_select.mse", true},
	{ CInstanceBase::EFFECT_TARGET, "", "d:/ymir work/effect/etc/click/click_glow_select.mse", true},
	{ CInstanceBase::EFFECT_STUN, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun.mse", true},
	{ CInstanceBase::EFFECT_CRITICAL, "Bip01 R Hand", "d:/ymir work/effect/hit/critical.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_TARGET, "", "d:/ymir work/effect/affect/damagevalue/target.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_NOT_TARGET, "", "d:/ymir work/effect/affect/damagevalue/nontarget.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE, "", "d:/ymir work/effect/affect/damagevalue/damage.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE2, "", "d:/ymir work/effect/affect/damagevalue/damage_1.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_POISON, "", "d:/ymir work/effect/affect/damagevalue/poison.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_MISS, "", "d:/ymir work/effect/affect/damagevalue/miss.mse", true},
	{ CInstanceBase::EFFECT_DAMAGE_TARGETMISS, "", "d:/ymir work/effect/affect/damagevalue/target_miss.mse", true},
	{ CInstanceBase::EFFECT_LEVELUP_ON_14_FOR_GERMANY, "", "season1/effect/paymessage_warning.mse", true},
	{ CInstanceBase::EFFECT_LEVELUP_UNDER_15_FOR_GERMANY, "", "season1/effect/paymessage_decide.mse", true},
	{ CInstanceBase::EFFECT_PERCENT_DAMAGE1, "", "d:/ymir work/effect/hit/percent_damage1.mse", true},
	{ CInstanceBase::EFFECT_PERCENT_DAMAGE2, "", "d:/ymir work/effect/hit/percent_damage2.mse", true},
	{ CInstanceBase::EFFECT_PERCENT_DAMAGE3, "", "d:/ymir work/effect/hit/percent_damage3.mse", true},





	//Dodane na tmp4 pliki
	//{ CInstanceBase::EFFECT_BEER, "", "d:/ymir work/effect/etc/beer/beer.mse", true},


};

typedef std::pair<DWORD, std::string> TSoundFileName;
static const std::array<TSoundFileName, CPythonItem::USESOUND_NUM - 1> vecUseSoundFileName =
{ {
	{CPythonItem::USESOUND_DEFAULT, "sound/ui/drop.wav"},
	{CPythonItem::USESOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav"},
	{CPythonItem::USESOUND_ARMOR, "sound/ui/equip_metal_armor.wav"},
	{CPythonItem::USESOUND_BOW, "sound/ui/equip_bow.wav"},
	{CPythonItem::USESOUND_WEAPON, "sound/ui/equip_metal_weapon.wav"},
	{CPythonItem::USESOUND_POTION, "sound/ui/eat_potion.wav"},
	{CPythonItem::USESOUND_PORTAL, "sound/ui/potal_scroll.wav"},
} };

static const std::array<TSoundFileName, CPythonItem::DROPSOUND_NUM> vecDropSoundFileName =
{ {
		{CPythonItem::DROPSOUND_DEFAULT, "sound/ui/drop.wav"},
		{CPythonItem::DROPSOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav"},
		{CPythonItem::DROPSOUND_ARMOR, "sound/ui/equip_metal_armor.wav"},
		{CPythonItem::DROPSOUND_BOW, "sound/ui/equip_bow.wav"},
		{CPythonItem::DROPSOUND_WEAPON, "sound/ui/equip_metal_weapon.wav"},
} };

typedef std::pair<DWORD, const std::string> TRaceData;
static const std::array<TRaceData, NPlayerData::MAIN_RACE_MAX_NUM> vecRaceData =
{ {
		{NPlayerData::MAIN_RACE_WARRIOR_M, "d:/ymir work/pc/warrior/"},
		{NPlayerData::MAIN_RACE_WARRIOR_W, "d:/ymir work/pc2/warrior/"},
		{NPlayerData::MAIN_RACE_ASSASSIN_W, "d:/ymir work/pc/assassin/"},
		{NPlayerData::MAIN_RACE_ASSASSIN_M, "d:/ymir work/pc2/assassin/"},
		{NPlayerData::MAIN_RACE_SURA_M, "d:/ymir work/pc/sura/"},
		{NPlayerData::MAIN_RACE_SURA_W, "d:/ymir work/pc2/sura/"},
		{NPlayerData::MAIN_RACE_SHAMAN_W, "d:/ymir work/pc/shaman/"},
		{NPlayerData::MAIN_RACE_SHAMAN_M, "d:/ymir work/pc2/shaman/"},
#ifdef ENABLE_WOLFMAN_CHARACTER
		{NPlayerData::MAIN_RACE_WOLFMAN_M, "d:/ymir work/pc3/wolfman/"},
#endif
} };

const constexpr uint8_t GENERAL_MOTION_NUM = 13;
typedef std::tuple<DWORD, const std::string, const uint8_t> TGMotionList;
static const std::array<TGMotionList, GENERAL_MOTION_NUM> vecGeneralMotions =
{ {
		{CRaceMotionData::NAME_WAIT, "wait.msa", 100},
		{CRaceMotionData::NAME_WALK, "walk.msa", 100},
		{CRaceMotionData::NAME_RUN, "run.msa", 100},
		{CRaceMotionData::NAME_DAMAGE, "damage.msa", 50},
		{CRaceMotionData::NAME_DAMAGE, "damage_1.msa", 50},
		{CRaceMotionData::NAME_DAMAGE_BACK, "damage_2.msa", 50},
		{CRaceMotionData::NAME_DAMAGE_BACK, "damage_3.msa", 50},
		{CRaceMotionData::NAME_DAMAGE_FLYING, "damage_flying.msa", 100},
		{CRaceMotionData::NAME_STAND_UP, "falling_stand.msa", 100},
		{CRaceMotionData::NAME_DAMAGE_FLYING_BACK, "back_damage_flying.msa", 100},
		{CRaceMotionData::NAME_STAND_UP_BACK, "back_falling_stand.msa", 100},
		{CRaceMotionData::NAME_DEAD, "dead.msa", 100},
		{CRaceMotionData::NAME_DIG, "dig.msa", 100},
} };

enum ComboType
{
	COMBO_TYPE_1,
	COMBO_TYPE_2,
	COMBO_TYPE_3,
};

enum ComboIndex
{
	COMBO_INDEX_1,
	COMBO_INDEX_2,
	COMBO_INDEX_3,
	COMBO_INDEX_4,
	COMBO_INDEX_5,
	COMBO_INDEX_6,
};


const constexpr uint8_t FISHING_MOTION_NUM = 8;
typedef std::pair<DWORD, const char*> TFishing;
constexpr std::array<TFishing, FISHING_MOTION_NUM> vecFishing =
{ {
		{CRaceMotionData::NAME_WAIT, "wait.msa"},
		{CRaceMotionData::NAME_WALK, "walk.msa"},
		{CRaceMotionData::NAME_FISHING_THROW, "throw.msa"},
		{CRaceMotionData::NAME_FISHING_WAIT, "fishing_wait.msa"},
		{CRaceMotionData::NAME_FISHING_STOP, "fishing_cancel.msa"},
		{CRaceMotionData::NAME_FISHING_REACT, "fishing_react.msa"},
		{CRaceMotionData::NAME_FISHING_CATCH, "fishing_catch.msa"},
		{CRaceMotionData::NAME_FISHING_FAIL, "fishing_fail.msa"},
} };

const constexpr uint8_t GUILD_SKILL_MOTION_NUM = 6;
typedef std::pair<DWORD, const char*> TGuildSkill;
constexpr std::array<TGuildSkill, GUILD_SKILL_MOTION_NUM> vecGuildSkill =
{ {
		{CRaceMotionData::NAME_SKILL + 107, "guild_yongsinuipi.msa"},
		{CRaceMotionData::NAME_SKILL + 108, "guild_yongsinuichukbok.msa"},
		{CRaceMotionData::NAME_SKILL + 109, "guild_seonghwigap.msa"},
		{CRaceMotionData::NAME_SKILL + 110, "guild_gasokhwa.msa"},
		{CRaceMotionData::NAME_SKILL + 111, "guild_yongsinuibunno.msa"},
		{CRaceMotionData::NAME_SKILL + 112, "guild_jumunsul.msa"},
} };

const constexpr uint8_t WEDDING_MOTION_NUM = 3;
typedef std::pair<DWORD, const char*> TWeddingList;
constexpr std::array<TWeddingList, WEDDING_MOTION_NUM> vecWeddingList =
{ {
		{CRaceMotionData::NAME_WAIT, "wait.msa"},
		{CRaceMotionData::NAME_WALK, "walk.msa"},
		{CRaceMotionData::NAME_RUN, "walk.msa"},
} };

#ifdef ENABLE_WOLFMAN_CHARACTER
const constexpr uint8_t ACTION_MOTION_NUM = 38;
#else
const constexpr uint8_t ACTION_MOTION_NUM = 38 - 4;
#endif
typedef std::pair<DWORD, const char*> TActionsList;
constexpr std::array<TActionsList, ACTION_MOTION_NUM> vecActionsList =
{ {
		{CRaceMotionData::NAME_CLAP, "clap.msa"},			// 1
		{CRaceMotionData::NAME_CHEERS_1, "cheers_1.msa"},	// 2
		{CRaceMotionData::NAME_CHEERS_2, "cheers_2.msa"},	// 3
		{CRaceMotionData::NAME_DANCE_1, "dance_1.msa"},		// 4
		{CRaceMotionData::NAME_DANCE_2, "dance_2.msa"},		// 5
		{CRaceMotionData::NAME_DANCE_3, "dance_3.msa"},		// 6
		{CRaceMotionData::NAME_DANCE_4, "dance_4.msa"},		// 7
		{CRaceMotionData::NAME_DANCE_5, "dance_5.msa"},		// 8
		{CRaceMotionData::NAME_DANCE_6, "dance_6.msa"},		// 9
		{CRaceMotionData::NAME_CONGRATULATION, "congratulation.msa"},	// 10
		{CRaceMotionData::NAME_FORGIVE, "forgive.msa"},					// 11
		{CRaceMotionData::NAME_ANGRY, "angry.msa"},						// 12
		{CRaceMotionData::NAME_ATTRACTIVE, "attractive.msa"},			// 13
		{CRaceMotionData::NAME_SAD, "sad.msa"},							// 14
		{CRaceMotionData::NAME_SHY, "shy.msa"},							// 15
		{CRaceMotionData::NAME_CHEERUP, "cheerup.msa"},					// 16
		{CRaceMotionData::NAME_BANTER, "banter.msa"},					// 17
		{CRaceMotionData::NAME_JOY, "joy.msa"},							// 18
		{CRaceMotionData::NAME_FRENCH_KISS_WITH_WARRIOR, "french_kiss_with_warrior.msa"},	// 19
		{CRaceMotionData::NAME_FRENCH_KISS_WITH_ASSASSIN, "french_kiss_with_assassin.msa"},	// 20
		{CRaceMotionData::NAME_FRENCH_KISS_WITH_SURA, "french_kiss_with_sura.msa"},			// 21
		{CRaceMotionData::NAME_FRENCH_KISS_WITH_SHAMAN, "french_kiss_with_shaman.msa"},		// 22
#ifdef ENABLE_WOLFMAN_CHARACTER
		{CRaceMotionData::NAME_FRENCH_KISS_WITH_WOLFMAN, "french_kiss_with_wolfman.msa"},	// 23
#endif
		{CRaceMotionData::NAME_KISS_WITH_WARRIOR, "kiss_with_warrior.msa"},					// 24
		{CRaceMotionData::NAME_KISS_WITH_ASSASSIN, "kiss_with_assassin.msa"},				// 25
		{CRaceMotionData::NAME_KISS_WITH_SURA, "kiss_with_sura.msa"},						// 26
		{CRaceMotionData::NAME_KISS_WITH_SHAMAN, "kiss_with_shaman.msa"},					// 27
#ifdef ENABLE_WOLFMAN_CHARACTER
		{CRaceMotionData::NAME_KISS_WITH_WOLFMAN, "kiss_with_wolfman.msa"},					// 28
#endif
		{CRaceMotionData::NAME_SLAP_HIT_WITH_WARRIOR, "slap_hit.msa"},						// 29
		{CRaceMotionData::NAME_SLAP_HIT_WITH_ASSASSIN, "slap_hit.msa"},						// 30
		{CRaceMotionData::NAME_SLAP_HIT_WITH_SURA, "slap_hit.msa"},							// 31
		{CRaceMotionData::NAME_SLAP_HIT_WITH_SHAMAN, "slap_hit.msa"},						// 32
#ifdef ENABLE_WOLFMAN_CHARACTER
		{CRaceMotionData::NAME_SLAP_HIT_WITH_WOLFMAN, "slap_hit.msa"},						// 33
#endif
		{CRaceMotionData::NAME_SLAP_HURT_WITH_WARRIOR, "slap_hurt.msa"},					// 34
		{CRaceMotionData::NAME_SLAP_HURT_WITH_ASSASSIN, "slap_hurt.msa"},					// 35
		{CRaceMotionData::NAME_SLAP_HURT_WITH_SURA, "slap_hurt.msa"},						// 36
		{CRaceMotionData::NAME_SLAP_HURT_WITH_SHAMAN, "slap_hurt.msa"},						// 37
#ifdef ENABLE_WOLFMAN_CHARACTER
		{CRaceMotionData::NAME_SLAP_HURT_WITH_WOLFMAN, "slap_hurt.msa"},					// 38
#endif
} };

const constexpr uint8_t INTRO_MOTION_NUM = 3;
typedef std::pair<DWORD, const char *> TIMotionList;
constexpr std::array<TIMotionList, INTRO_MOTION_NUM> vecIntroMotions =
{{
		{CRaceMotionData::NAME_INTRO_WAIT, "wait.msa"},
		{CRaceMotionData::NAME_INTRO_SELECTED, "selected.msa"},
		{CRaceMotionData::NAME_INTRO_NOT_SELECTED, "not_selected.msa"},
}};

constexpr uint8_t GUILD_MATERIAL_NUM = 3;

enum EGuildMaterialItems
{
	MATERIAL_STONE_ID = 90010,
	MATERIAL_LOG_ID,
	MATERIAL_PLYWOOD_ID,
};

static const uint8_t getGuildMaterialIndex(const int32_t& vnum)
{
	switch (vnum)
	{
	case MATERIAL_STONE_ID:
	case MATERIAL_LOG_ID:
	case MATERIAL_PLYWOOD_ID:
		return vnum - 90010;
	}

	return -1;
}
	


static const std::map<uint8_t, std::map<uint8_t, std::vector<uint8_t>>> skillIndexes{
	{ NRaceData::JOB_WARRIOR, {
		{1, {1, 2, 3, 4, 5, 6, 0, 0, 137, 0, 138, 0, 139, 0}},
		{2, {16, 17, 18, 19, 20, 21, 0, 0, 137, 0, 138, 0, 139, 0}},
	}},
	{ NRaceData::JOB_ASSASSIN, {
		{1, {31, 32, 33, 34, 35, 36, 0, 0, 137, 0, 138, 0, 139, 0, 140}},
		{2, {46, 47, 48, 49, 50, 51, 0, 0, 137, 0, 138, 0, 139, 0, 140}},
	}},
	{ NRaceData::JOB_SURA, {
		{1, {61, 62, 63, 64, 65, 66, 0, 0, 137, 0, 138, 0, 139, 0}},
		{2, {76, 77, 78, 79, 80, 81, 0, 0, 137, 0, 138, 0, 139, 0}},
	}},
	{ NRaceData::JOB_SHAMAN, {
		{1, {91, 92, 93, 94, 95, 96, 0, 0, 137, 0, 138, 0, 139, 0}},
		{2, {106, 107, 108, 109, 110, 111, 0, 0, 137, 0, 138, 0, 139, 0}},
	}},
#ifdef ENABLE_WOLFMAN_CHARACTER
	{ NRaceData::JOB_WOLFMAN, {
		{1, {170, 171, 172, 173, 174, 175, 0, 0, 137, 0, 0, 0, 0, 0}},
		{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	}},
#endif
};


constexpr std::array<uint8_t, 12> supportSkillIndexes =
{
	{122, 123, 121, 124, 125, 129, 131, 130, 0, 0, 0, 0},
};


static const std::map<std::string, std::vector<uint8_t>> guildSkills =
{
	{ "PASSIVE", { 151 }},
	{ "ACTIVE", { 152, 153, 154, 155, 156, 157 }},
};

// static const std::map<DWORD, float> raceHeight =
// {
	// {20246, 160.00f},
	// {20247, 160.00f},
	// {2493, 500.00f},
	// {3999, 500.00f},
	// {20030, 220.00f},
	// {20101, 220.00f},
	// {20102, 220.00f},
	// {20103, 220.00f},
	// {20104, 220.00f},
	// {20105, 220.00f},
	// {20106, 220.00f},
	// {20107, 220.00f},
	// {20108, 220.00f},
	// {20109, 220.00f},
	// {0, 180.00f},
	// {1, 180.00f},
	// {2, 190.00f},
	// {3, 180.00f},
	// {4, 180.00f},
	// {5, 180.00f},
	// {6, 180.00f},
	// {7, 180.00f},
// #ifdef ENABLE_WOLFMAN_CHARACTER
	// {8, 230.00f},
// #endif
//};
