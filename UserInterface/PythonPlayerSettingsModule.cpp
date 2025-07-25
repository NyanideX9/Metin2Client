#include "Stdafx.h"
#ifdef ENABLE_PERFORMANCE_IMPROVEMENTS_NEW

#include "PythonPlayerSettingsModule.h"
#include "PythonNetworkStream.h"
#include "PythonBackground.h"

#include "../EterLib/ResourceManager.h"
#include "../EterPack/EterPackManager.h"
#include "../GameLib/RaceManager.h"

#include "boost/algorithm/string.hpp"
CPythonPlayerSettingsModule::CPythonPlayerSettingsModule()
{
	isLoaded = false;
}

void CPythonPlayerSettingsModule::Load()
{
	if (isLoaded)
		return;
	
	__LoadGameInit();
	__LoadEmoticons();
	__RegisterEmotionIcons();

	for (const auto &[raceIndex, fileName] : vecRaceData)
	{
		CRaceManager::Instance().CreateRace(raceIndex);
		CRaceManager::Instance().SelectRace(raceIndex);
	
		switch (raceIndex)
		{
		case NPlayerData::MAIN_RACE_WARRIOR_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "warrior_m.msm");
			__LoadGameWarriorEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_WARRIOR_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "warrior_w.msm");
			__LoadGameWarriorEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;
			
		case NPlayerData::MAIN_RACE_ASSASSIN_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "assassin_m.msm");
			__LoadGameAssassinEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_ASSASSIN_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "assassin_w.msm");
			__LoadGameAssassinEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;
			
		case NPlayerData::MAIN_RACE_SURA_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "sura_m.msm");
			__LoadGameSuraEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_SURA_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "sura_w.msm");
			__LoadGameSuraEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;
			
		case NPlayerData::MAIN_RACE_SHAMAN_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "shaman_m.msm");
			__LoadGameShamanEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;

		case NPlayerData::MAIN_RACE_SHAMAN_W:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "shaman_w.msm");
			__LoadGameShamanEx(CRaceManager::Instance().GetSelectedRaceDataPointer(), fileName);
			break;
			
#ifdef ENABLE_WOLFMAN_CHARACTER
		case NPlayerData::MAIN_RACE_WOLFMAN_M:
			__LoadGameRace(CRaceManager::Instance().GetSelectedRaceDataPointer(), "wolfman_m.msm");
			__LoadGameWolfmanEx(CRaceManager::Instance().GetSelectedRaceDataPointer());
			break;
#endif
		}
	}
	
	__LoadGameEffects();
	__LoadGameNPC();
	__RegisterTitleColors();
	__RegisterNameColors();
	__RegisterTitleNames();
	__LoadGameSound();
	__RegisterDungeonMapNames();
	// __LoadRaceHeight();

	CPythonBackground::Instance().__CreateProperty();

	isLoaded = true;
	CPythonNetworkStream::Instance().SetClientLoaded();
}

void CPythonPlayerSettingsModule::__RegisterEmotionIcons()
{
	for (const auto& [index, imagePath] : emotionIcons)
	{
		CGraphicImage* pImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(imagePath);
		m_kMap_iEmotionIndex_pkIconImage.insert(std::make_pair(index, pImage));
	}
}

void CPythonPlayerSettingsModule::__RegisterTitleNames()
{
	for (size_t index = 0; index < titleNameList.size(); ++index)
	{
		std::string name = titleNameList[index];
		CInstanceBase::RegisterTitleName(index, name.c_str());
	}
}

void CPythonPlayerSettingsModule::__RegisterNameColors()
{
	for (const auto &[index, rgbTuple] : characterNameColors)
	{
		const auto &[r, g, b] = rgbTuple;
		CInstanceBase::RegisterNameColor(index, r, g, b);
	}
}

void CPythonPlayerSettingsModule::__RegisterTitleColors()
{
	for (const auto &[index, rgbTuple] : characterTitleColors)
	{
		const auto &[r, g, b] = rgbTuple;
		CInstanceBase::RegisterTitleColor(index, r, g, b);
	}
}

void CPythonPlayerSettingsModule::__LoadGameSound()
{
	for (const auto &[useSoundIndex, fileName] : vecUseSoundFileName)
		CPythonItem::Instance().SetUseSoundFileName(useSoundIndex, fileName);

	for (const auto &[dropSoundIndex, fileName] : vecDropSoundFileName)
		CPythonItem::Instance().SetDropSoundFileName(dropSoundIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadGameNPC()
{
	using namespace boost::algorithm;

	CMappedFile file;
	const VOID *pvData;
	if (!CEterPackManager::Instance().Get(file, NPC_LIST_FILE_NAME, &pvData))
	{
		TraceError("CPythonPlayerSettingsModule::LoadGameNPC(fileName=%s) - Load Error", NPC_LIST_FILE_NAME);
		return;
	}

	CMemoryTextFileLoader fileLoader;
	fileLoader.Bind(file.Size(), pvData);

	CTokenVector kTokenVector;

	for (size_t i = 0; i < fileLoader.GetLineCount(); ++i)
	{
		if (!fileLoader.SplitLineByTab(i, &kTokenVector))
			continue;

		const uint32_t& vnum = atoi(kTokenVector[0].c_str());
		std::string& name = kTokenVector[1];
		trim(name);

		if (vnum)
		{
			CRaceManager::Instance().RegisterRaceName(vnum, name.c_str());
		}
		else
		{
			std::string& src = kTokenVector[2];
			trim(src);
			CRaceManager::Instance().RegisterRaceSrcName(name.c_str(), src.c_str());
		}
	}
}

void CPythonPlayerSettingsModule::__RegisterDungeonMapNames()
{
	CPythonBackground::Instance().RegisterDungeonMapName(std::string("metin2_map_spiderdungeon").c_str());
	CPythonBackground::Instance().RegisterDungeonMapName(std::string("metin2_map_deviltower1").c_str());
}

void CPythonPlayerSettingsModule::__LoadEmoticons()
{
	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 0, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/sweat.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(sweat)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 1, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/money.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(money)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 2, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/happy.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(happy)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 3, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/love_s.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(love_s)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 4, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/love_l.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(love_l)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 5, std::string("").c_str(),std::string( "d:/ymir work/effect/etc/emoticon/angry.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(angry)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 6, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/aha.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(aha)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 7, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/gloom.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(gloom)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 8, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/sorry.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(sorry)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 9, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/!_mix_back.mse").c_str(), true); //dodałem
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(!)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 10, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/question.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(?)").c_str());

	CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_EMOTICON + 11, std::string("").c_str(), std::string("d:/ymir work/effect/etc/emoticon/fish.mse").c_str(), true);
	CPythonNetworkStream::Instance().RegisterEmoticonString(std::string("(fish)").c_str());
}

void CPythonPlayerSettingsModule::__LoadGameEffects()
{
	for (const auto& [effectName, effectType, effectFolder, effectCache] : vecEffectList)
		CInstanceBase::RegisterEffect(effectName, effectType, effectFolder, effectCache);

	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_EXP, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_yellow_small2.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_HP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_red_small.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_HP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_red_big.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_blue_warrior_small.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_blue_small.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_blue_big.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK1, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_1.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK2, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_2.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK3, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_3.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK4, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_4.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK5, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_5.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK6, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_6.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_FIREWORK_XMAS, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, std::string("d:/ymir work/effect/etc/firecracker/firecracker_xmas.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_CHAIN_LIGHTNING, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/pc/shaman/effect/pokroe.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_HP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/effect/etc/gathering/ga_piece_red_smallest.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_SKILL_MUYEONG, CFlyingManager::INDEX_FLY_TYPE_AUTO_FIRE, std::string("d:/ymir work/pc/sura/effect/muyeong_fly.msf").c_str());
	CFlyingManager::Instance().RegisterIndexedFlyData(CFlyingManager::FLY_QUIVER_ATTACK_NORMAL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, std::string("d:/ymir work/pc/assassin/effect/arrow_02.msf").c_str());
}

void CPythonPlayerSettingsModule::__LoadGameInit()
{
	CInstanceBase::SetDustGap(250);
	CInstanceBase::SetHorseDustGap(500);
	CPythonPlayer::Instance().RegisterEffect(CPythonPlayer::EFFECT_PICK, std::string("d:/ymir work/effect/etc/click/click.mse").c_str(), true);
}

void CPythonPlayerSettingsModule::__RegisterCacheMotionData(CRaceData *pRaceData, const uint16_t wMotionMode, const uint16_t wMotionIndex, const char *c_szFileName, const uint8_t byPercentage)
{
	const char *c_szFullFileName = CRaceManager::Instance().GetFullPathFileName(c_szFileName);
	const CGraphicThing *pkMotionThing = pRaceData->RegisterMotionData(wMotionMode, wMotionIndex, c_szFullFileName, byPercentage);

	if (pkMotionThing)
		CResourceManager::Instance().LoadStaticCache(pkMotionThing->GetFileName());
}

#ifdef ENABLE_WOLFMAN_CHARACTER
void CPythonPlayerSettingsModule::__LoadGameWolfmanEx(CRaceData* pRaceData)
{
	__SetIntroMotions(pRaceData, "d:/ymir work/pc3/wolfman/");

	//general
	CRaceManager::Instance().SetPathName(std::string("d:/ymir work/pc3/wolfman/general/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DAMAGE, std::string("front_damage.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DAMAGE, std::string("front_damage1.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DAMAGE_BACK, std::string("back_damage.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DAMAGE_BACK, std::string("back_damage1.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DAMAGE_FLYING, std::string("front_damage_flying.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_STAND_UP, std::string("front_falling_standup.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DAMAGE_FLYING_BACK, std::string("back_damage_flying.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_STAND_UP_BACK, std::string("back_falling_standup.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str(), 100);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_DIG, std::string("dig.msa").c_str(), 100);

	pRaceData->SetMotionRandomWeight(CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, 0, 70);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 30);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack1.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack2.msa").c_str(), 50);
	
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 6);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_5, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_6, CRaceMotionData::NAME_COMBO_ATTACK_1);
	
	//skill
	CRaceManager::Instance().SetPathName(std::string("d:/ymir work/pc3/wolfman/skill/").c_str());
	
	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("split_slash" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("wind_death" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("reef_attack" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("wreckage" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("red_possession" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("blue_possession" + END_STRING + ".msa").c_str());
	
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}
	
	__LoadFishing(pRaceData, "d:/ymir work/pc3/wolfman/");
	__LoadGuildSkill(pRaceData, "d:/ymir work/pc3/wolfman/");
	
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	
	CRaceManager::Instance().SetPathName(std::string("d:/ymir work/pc3/wolfman/horse/").c_str());
	
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait1.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait2.msa").c_str(), 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, std::string("front_damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, std::string("front_damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, std::string("skill_charge.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());
	
	CRaceManager::Instance().SetPathName(std::string("d:/ymir work/pc3/wolfman/horse_claw/").c_str());
	
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_CLAW);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_CLAW, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_CLAW, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_CLAW, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_CLAW, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_CLAW, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_CLAW, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_CLAW, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_CLAW, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());
	
	__LoadWedding(pRaceData, "d:/ymir work/pc3/wolfman/");
	__LoadActions(pRaceData, "d:/ymir work/pc3/wolfman/");
		
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_CLAW, "d:/ymir work/pc3/wolfman/claw/");
	
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, std::string("equip_right_weapon").c_str());
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON_LEFT, std::string("equip_left_weapon").c_str());
	
#ifdef ENABLE_SASH_SYSTEM
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_SASH, std::string("Bip01 Spine2").c_str());
#endif
}
#endif

void CPythonPlayerSettingsModule::__LoadComboRace(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName)
{
    const BYTE RaceIndex = RaceToJob(pRaceData->GetRace());

    CRaceManager::Instance().SetPathName(c_rstrFolderName.c_str());

    pRaceData->RegisterMotionMode(wMotionMode);

    if (RaceIndex == NRaceData::JOB_WARRIOR)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 50);
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 50);
    }
    else if (RaceIndex == NRaceData::JOB_ASSASSIN)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 70);
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 30);
    }
    else if (RaceIndex == NRaceData::JOB_SURA)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str());
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str());
    }
    else if (RaceIndex == NRaceData::JOB_SHAMAN)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str());
    }
#ifdef ENABLE_WOLFMAN_CHARACTER
	else if (RaceIndex == NRaceData::JOB_WOLFMAN)
    {
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WAIT, std::string("wait1.msa").c_str(), 50);
    }
#endif

	__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str());
    __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());

#ifdef ENABLE_WOLFMAN_CHARACTER
	if (wMotionMode == CRaceMotionData::MODE_CLAW)
	{
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, std::string("front_damage.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, std::string("back_damage.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE_BACK, std::string("back_damage.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE_BACK, std::string("back_damage.msa").c_str(), 50);
		
	}
	else
#endif
	{
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE, std::string("damage_1.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE_BACK, std::string("damage_2.msa").c_str(), 50);
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_DAMAGE_BACK, std::string("damage_3.msa").c_str(), 50);
	}

    if (wMotionMode == CRaceMotionData::MODE_BOW)
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack.msa").c_str());
	else
	{
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_4, std::string("combo_04.msa").c_str());
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_5, std::string("combo_05.msa").c_str());
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_6, std::string("combo_06.msa").c_str());
		__RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_7, std::string("combo_07.msa").c_str());
	}
      if (wMotionMode == CRaceMotionData::MODE_DUALHAND_SWORD)
        __RegisterCacheMotionData(pRaceData, wMotionMode, CRaceMotionData::NAME_COMBO_ATTACK_8, std::string("combo_08.msa").c_str());

    __LoadCombo(pRaceData, wMotionMode);
}

void CPythonPlayerSettingsModule::__LoadCombo(CRaceData *pRaceData, const uint16_t wMotionMode)
{

        if (wMotionMode == CRaceMotionData::MODE_BOW)
        {
            pRaceData->ReserveComboAttack(CRaceMotionData::MODE_BOW, COMBO_TYPE_1, 1);
            pRaceData->RegisterComboAttack(CRaceMotionData::MODE_BOW, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
            return;
        }

        pRaceData->ReserveComboAttack(wMotionMode, COMBO_TYPE_1, 4);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_1, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_4);


        pRaceData->ReserveComboAttack(wMotionMode, COMBO_TYPE_2, 5);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_5);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_2, COMBO_INDEX_5, CRaceMotionData::NAME_COMBO_ATTACK_7);

        pRaceData->ReserveComboAttack(wMotionMode, COMBO_TYPE_3, 6);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_4, CRaceMotionData::NAME_COMBO_ATTACK_5);
        pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_5, CRaceMotionData::NAME_COMBO_ATTACK_6);
		if (wMotionMode == CRaceMotionData::MODE_DUALHAND_SWORD)
			pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_6, CRaceMotionData::NAME_COMBO_ATTACK_8);
		else
			pRaceData->RegisterComboAttack(wMotionMode, COMBO_TYPE_3, COMBO_INDEX_6, CRaceMotionData::NAME_COMBO_ATTACK_4);
}

void CPythonPlayerSettingsModule::__LoadGameWarriorEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	pRaceData->SetMotionRandomWeight(CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, 0, 70);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack_1.msa").c_str(), 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("samyeon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("palbang" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("jeongwi" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("geomgyeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("tanhwan" + END_STRING + ".msa").c_str());
#ifdef ENABLE_678TH_SKILL
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("gihyeol" + END_STRING + ".msa").c_str());
#endif
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("gigongcham" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("gyeoksan" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("daejin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("cheongeun" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("geompung" + END_STRING + ".msa").c_str());
#ifdef ENABLE_678TH_SKILL
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("noegeom" + END_STRING + ".msa").c_str());
#endif

		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}

	__LoadFishing(pRaceData, c_rstrFolderName);
	__LoadGuildSkill(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_2.msa").c_str(), 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, std::string("skill_charge.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_onehand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_twohand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str(), 1);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());

	__LoadWedding(pRaceData, c_rstrFolderName);
	__LoadActions(pRaceData, c_rstrFolderName);
    __LoadComboRace(pRaceData, CRaceMotionData::MODE_ONEHAND_SWORD, c_rstrFolderName + "onehand_sword/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_TWOHAND_SWORD, c_rstrFolderName + "twohand_sword/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, std::string("equip_right_hand").c_str());
	
//#ifdef ENABLE_SASH_SYSTEM
//	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_SASH, std::string("Bip01 Spine2").c_str());
//#endif
}

void CPythonPlayerSettingsModule::__LoadGameAssassinEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	pRaceData->SetMotionRandomWeight(CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_WAIT, 0, 70);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack_1.msa").c_str(), 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("amseup" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("gungsin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("charyun" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("eunhyeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("sangong" + END_STRING + ".msa").c_str());
#ifdef ENABLE_678TH_SKILL
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("seomjeon" + END_STRING + ".msa").c_str());
#endif
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("yeonsa" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("gwangyeok" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("hwajo" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("gyeonggong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("dokgigung" + END_STRING + ".msa").c_str());
#ifdef ENABLE_678TH_SKILL
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("seomgwang" + END_STRING + ".msa").c_str());

#endif
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}

	__LoadFishing(pRaceData, c_rstrFolderName);
	__LoadGuildSkill(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_2.msa").c_str(), 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, std::string("skill_charge.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_onehand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_dualhand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_bow/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_BOW);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_WAIT, std::string("wait_2.msa").c_str(), 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BOW, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_BOW, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BOW, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	__LoadWedding(pRaceData, c_rstrFolderName);
	__LoadActions(pRaceData, c_rstrFolderName);
    __LoadComboRace(pRaceData, CRaceMotionData::MODE_ONEHAND_SWORD, c_rstrFolderName + "onehand_sword/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_DUALHAND_SWORD, c_rstrFolderName + "dualhand_sword/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_BOW, c_rstrFolderName + "bow/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, std::string("equip_right").c_str());
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON_LEFT, std::string("equip_left").c_str());
//#ifdef ENABLE_SASH_SYSTEM
//	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_SASH, std::string("Bip01 Spine2").c_str());
//#endif
}

void CPythonPlayerSettingsModule::__LoadGameSuraEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack_1.msa").c_str(), 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);


	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	for (int i = 0; i < CPythonSkill::SKILL_EFFECT_COUNT; ++i)
	{
		std::string END_STRING = "";
		if (i != 0)
			END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("swaeryeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("yonggwon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("gwigeom" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("gongpo" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("jumagap" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("pabeop" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("maryeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("hwayeom" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("muyeong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("heuksin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("tusok" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("mahwan" + END_STRING + ".msa").c_str());
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}
	__LoadGuildSkill(pRaceData, c_rstrFolderName);
	__LoadFishing(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_2.msa").c_str(), 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, std::string("skill_charge.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_onehand_sword/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());

	__LoadWedding(pRaceData, c_rstrFolderName);

	__LoadActions(pRaceData, c_rstrFolderName);
        __LoadComboRace(pRaceData, CRaceMotionData::MODE_ONEHAND_SWORD, c_rstrFolderName + "onehand_sword/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, std::string("equip_right").c_str());
//#ifdef ENABLE_SASH_SYSTEM
//	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_SASH, std::string("Bip01 Spine2").c_str());
//#endif
}

void CPythonPlayerSettingsModule::__LoadGameShamanEx(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	__SetIntroMotions(pRaceData, c_rstrFolderName);
	__SetGeneralMotions(pRaceData, CRaceMotionData::MODE_GENERAL, c_rstrFolderName + "general/");
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack.msa").c_str(), 50);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("attack_1.msa").c_str(), 50);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());

	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 1, std::string("bipabu.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 2, std::string("yongpa.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 3, std::string("paeryong.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 4, std::string("hosin_target.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 5, std::string("boho_target.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 6, std::string("gicheon_target.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 16, std::string("noejeon.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 17, std::string("byeorak.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 18, std::string("pokroe.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 19, std::string("jeongeop_target.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 20, std::string("kwaesok_target.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + 21, std::string("jeungryeok_target.msa").c_str());

	std::vector<int> skillList{ 1, 2, 3 };

	for (const auto& i : skillList)
	{
		std::string END_STRING = "_" + std::to_string(i + 1);

		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 1, std::string("bipabu" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 2, std::string("yongpa" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 3, std::string("paeryong" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 4, std::string("hosin" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 5, std::string("boho" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 6, std::string("gicheon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 16, std::string("noejeon" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 17, std::string("byeorak" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 18, std::string("pokroe" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 19, std::string("jeongeop" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 20, std::string("kwaesok" + END_STRING + ".msa").c_str());
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_SKILL + (i * CPythonSkill::SKILL_GRADEGAP) + 21, std::string("jeungryeok" + END_STRING + ".msa").c_str());
		pRaceData->ReserveComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, 1);
		pRaceData->RegisterComboAttack(CRaceMotionData::MODE_GENERAL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	}

	__LoadGuildSkill(pRaceData, c_rstrFolderName);
	__LoadFishing(pRaceData, c_rstrFolderName);

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_1.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WAIT, std::string("wait_2.msa").c_str(), 1);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_WALK, std::string("walk.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_RUN, std::string("run.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DAMAGE_BACK, std::string("damage.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_DEAD, std::string("dead.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 122, std::string("skill_charge.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE, CRaceMotionData::NAME_SKILL + 123, std::string("skill_splash.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_fan/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_FAN);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str());
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_FAN, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());

	CRaceManager::Instance().SetPathName((c_rstrFolderName + "horse_bell/").c_str());

	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_HORSE_BELL);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_COMBO_ATTACK_1, std::string("combo_01.msa").c_str(), 90);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_COMBO_ATTACK_2, std::string("combo_02.msa").c_str(), 9);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_COMBO_ATTACK_3, std::string("combo_03.msa").c_str(), 1);
	pRaceData->ReserveComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, 3);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, COMBO_INDEX_1, CRaceMotionData::NAME_COMBO_ATTACK_1);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, COMBO_INDEX_2, CRaceMotionData::NAME_COMBO_ATTACK_2);
	pRaceData->RegisterComboAttack(CRaceMotionData::MODE_HORSE_BELL, COMBO_TYPE_1, COMBO_INDEX_3, CRaceMotionData::NAME_COMBO_ATTACK_3);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_HORSE_BELL, CRaceMotionData::NAME_SKILL + 121, std::string("skill_wildattack.msa").c_str());

	__LoadWedding(pRaceData, c_rstrFolderName);
	__LoadActions(pRaceData, c_rstrFolderName);
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_FAN, c_rstrFolderName + "fan/");
	__LoadComboRace(pRaceData, CRaceMotionData::MODE_BELL, c_rstrFolderName + "Bell/");

	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON, std::string("equip_right").c_str());
	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_WEAPON_LEFT, std::string("equip_left").c_str());
//#ifdef ENABLE_SASH_SYSTEM
//	pRaceData->RegisterAttachingBoneName(CRaceData::EParts::PART_SASH, std::string("Bip01 Spine2").c_str());
//#endif
}

void CPythonPlayerSettingsModule::__SetIntroMotions(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "intro/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_INTRO_WAIT, std::string("wait.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_INTRO_SELECTED, std::string("selected.msa").c_str());
	__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, CRaceMotionData::NAME_INTRO_NOT_SELECTED, std::string("not_selected.msa").c_str());
}

void CPythonPlayerSettingsModule::__SetGeneralMotions(CRaceData *pRaceData, const uint16_t wMotionMode, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName(c_rstrFolderName.c_str());
	pRaceData->RegisterMotionMode(wMotionMode);

	for (const auto& [motionIndex, fileName, percentage] : vecGeneralMotions)
	{
		std::string afileName = fileName;
		__RegisterCacheMotionData(pRaceData, wMotionMode, motionIndex, afileName.c_str(), percentage);
	}
}

void CPythonPlayerSettingsModule::__LoadGameRace(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	if (!pRaceData->LoadRaceData(c_rstrFolderName.c_str()))
	{
		TraceError("Failed to load race data : %s\n", c_rstrFolderName.c_str());
	}
}

void CPythonPlayerSettingsModule::__LoadWedding(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "wedding/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_WEDDING_DRESS);

	for (const auto &[motionIndex, fileName] : vecWeddingList)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_WEDDING_DRESS, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadGuildSkill(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "skill/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);

	for (const auto &[motionIndex, fileName] : vecGuildSkill)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadActions(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "action/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_GENERAL);

	for (const auto &[motionIndex, fileName] : vecActionsList)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_GENERAL, motionIndex, fileName);
}

void CPythonPlayerSettingsModule::__LoadFishing(CRaceData *pRaceData, const std::string &c_rstrFolderName)
{
	CRaceManager::Instance().SetPathName((c_rstrFolderName + "fishing/").c_str());
	pRaceData->RegisterMotionMode(CRaceMotionData::MODE_FISHING);

	for (const auto &[motionIndex, fileName] : vecFishing)
		__RegisterCacheMotionData(pRaceData, CRaceMotionData::MODE_FISHING, motionIndex, fileName);
}

bool CPythonPlayerSettingsModule::RegisterGuildBuildingList(const char* filepath)
{
	CMappedFile File;
	LPCVOID pData;

	if (!CEterPackManager::Instance().Get(File, filepath, &pData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(File.Size(), pData);

	PyObject* pyGuildData = PyList_New(0);

	CTokenVector TokenVector;
	for (size_t i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		if (!textFileLoader.SplitLineByTab(i, &TokenVector))
			continue;

		std::string& strVnum = TokenVector[GUIILD_BUILDING_TOKEN_VNUM];

		if (strVnum.find_first_not_of("0123456789") != std::string::npos)
			continue;

		if (TokenVector.size() < GUIILD_BUILDING_LIMIT_TOKEN_COUNT)
		{
			TraceError("RegisterGuildBuildingList(%s) - StrangeLine in %d TokenVector size too long: %d", filepath, i, TokenVector.size());
			continue;
		}

		const int32_t& id = atoi(strVnum.c_str());
		const std::string_view type = TokenVector[GUIILD_BUILDING_TOKEN_TYPE];
		const char* name = TokenVector[GUIILD_BUILDING_TOKEN_NAME].c_str();
		const char* localName = TokenVector[GUIILD_BUILDING_TOKEN_LOCAL_NAME].c_str();
		const int32_t& xRotLimit = atoi(TokenVector[GUIILD_BUILDING_TOKEN_X_ROT_LIMIT].c_str());
		const int32_t& yRotLimit = atoi(TokenVector[GUIILD_BUILDING_TOKEN_Y_ROT_LIMIT].c_str());
		const int32_t& zRotLimit = atoi(TokenVector[GUIILD_BUILDING_TOKEN_Z_ROT_LIMIT].c_str());
		const int32_t& price = atoi(TokenVector[GUIILD_BUILDING_TOKEN_PRICE].c_str());
		const std::string_view material = TokenVector[GUIILD_BUILDING_TOKEN_MATERIAL];
		const int32_t& enableType = atoi(TokenVector[GUIILD_BUILDING_TOKEN_ENABLE_FLAG].c_str());

		if (enableType == 0)
			continue;

		// Register material
		const std::string& folderName = m_buildingTypeToFolder[type];

		CRaceManager::Instance().RegisterRaceSrcName(name, folderName);
		CRaceManager::Instance().RegisterRaceName(id, name);

		// Create building material List
		std::vector<std::string> materialVector;
		boost::split(materialVector, material, boost::is_any_of("/"));

		PyObject* pyMaterialList = PyList_New(3);

		for (size_t j = 0; j < GUILD_MATERIAL_NUM; ++j)
		{
			PyList_SetItem(pyMaterialList, j, Py_BuildValue("s", "0"));
		}

		for (const std::string& itemPair : materialVector)
		{
			std::vector<std::string> itemVector;
			boost::split(itemVector, itemPair, boost::is_any_of(","));

			if (itemVector.size() != 2)
				continue;

			const int32_t& vnum = atoi(itemVector[0].c_str());
			const std::string& count = itemVector[1];

			const uint8_t index = getGuildMaterialIndex(vnum);

			if (index == -1)
			{
				TraceError("Found strange guild material: %d", vnum);
				continue;
			}

			PyList_SetItem(pyMaterialList, index, Py_BuildValue("s", count.c_str()));
		}

		// Create guild data dict and append to list
		PyObject* pyGuildDataItem = PyDict_New();
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "VNUM"), Py_BuildValue("i", id));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "TYPE"), Py_BuildValue("s", type.data()));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "NAME"), Py_BuildValue("s", name));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "LOCAL_NAME"), Py_BuildValue("s", localName));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "X_ROT_LIMIT"), Py_BuildValue("i", xRotLimit));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "Y_ROT_LIMIT"), Py_BuildValue("i", yRotLimit));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "Z_ROT_LIMIT"), Py_BuildValue("i", zRotLimit));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "PRICE"), Py_BuildValue("i", price));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "MATERIAL"), Py_BuildValue("O", pyMaterialList));
		PyDict_SetItem(pyGuildDataItem, Py_BuildValue("s", "SHOW"), Py_BuildValue("i", enableType != 2));
		PyList_Append(pyGuildData, pyGuildDataItem);
	}

	CPythonNetworkStream::Instance().SendPythonData(pyGuildData, "BINARY_SetGuildBuildingList");
	return true;
}

void CPythonPlayerSettingsModule::RegisterSkills()
{
	CPythonNetworkStream& net = CPythonNetworkStream::Instance();
	RegisterSkills(net.GetMainActorRace(), net.GetMainActorSkillGroup(), net.GetMainActorEmpire());
}

void CPythonPlayerSettingsModule::RegisterSkills(uint32_t race, uint8_t skill_group, uint8_t empire)
{
	const uint32_t playerRace = race;
	const uint32_t playerSkillGroup = skill_group;
	const uint8_t playerJob = RaceToJob(playerRace);
	const uint32_t playerEmpire = empire;

	// Player Skills
	if (skillIndexes.find(playerJob) != skillIndexes.end() &&
		skillIndexes.at(playerJob).find(skill_group) != skillIndexes.at(playerJob).end())
	{
		std::vector<uint8_t> playerSkills = skillIndexes.at(playerJob).at(playerSkillGroup);

		for (size_t slot = 0; slot < playerSkills.size(); ++slot)
			CPythonPlayer::Instance().SetSkill(slot + 1, playerSkills[slot]);
	}

	// Support Skills
	const auto supportSkills = supportSkillIndexes;
	for (size_t slot = 0; slot < supportSkills.size(); ++slot)
		CPythonPlayer::Instance().SetSkill(slot + 100 + 1, supportSkills[slot]);


/*/
	 //Language Skills
	if (playerEmpire)
	{
		for (size_t i = 0; i < 3; ++i)
		{
			if ((i+1) != playerEmpire)
				CPythonPlayer::Instance().SetSkill(109+i, c_iSkillIndex_Language1+i);
		}
	}
/*/


	// Passive Guild Skills
	auto passiveSkills = guildSkills.at("PASSIVE");
	for (size_t j = 0; j < passiveSkills.size(); ++j)
		CPythonPlayer::Instance().SetSkill(200 + j, passiveSkills[j]);

	// Active Guild Skills
	auto activeSkills = guildSkills.at("ACTIVE");
	for (size_t k = 0; k < activeSkills.size(); ++k)
		CPythonPlayer::Instance().SetSkill(210 + k, activeSkills[k]);
}

//dodane cz2.
int32_t CPythonPlayerSettingsModule::GetCharacterTitleColor(uint8_t index)
{
	const auto &[r, g, b] = characterTitleColors.at(index);
	return CPythonGraphic::Instance().GenerateColor(r, g, b, 1.0);
}
//dodane cz2.

// void CPythonPlayerSettingsModule::__LoadRaceHeight()
// {
	// for (const auto & height : raceHeight)
		// CRaceManager::Instance().SetRaceHeight(height.first, height.second);
// }
#endif