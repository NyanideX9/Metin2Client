/****************************************
* title_name		: Projekt Natanka [Martysama] == SOURCE ==
* author			: Astetik
****************************************/

#pragma once

/*****************-- RZECZY Z MARTYSAMY --******************/

//#define ENABLE_ULTIMATE_REGEN
//#define RENEWAL_MISSION_BOOKS
//#define NEW_ALIGNMENT
//#define ENABLE_NPC_TARGET_EFFECT_FIX							//Fix Efekt strzały NPC w zadaniu

#define LOCALE_SERVICE_EUROPE
#define ENABLE_COSTUME_SYSTEM
#define ENABLE_NEW_EQUIPMENT_SYSTEM
#define ENABLE_MOUNT_COSTUME_SYSTEM
#define ENABLE_WEAPON_COSTUME_SYSTEM
#define ENABLE_MOUSEWHEEL_EVENT
#define ENABLE_PACK_GET_CHECK
#define ENABLE_CANSEEHIDDENTHING_FOR_GM
#define ENABLE_PROTOSTRUCT_AUTODETECT
#define ENABLE_PLAYER_PER_ACCOUNT5
#define ENABLE_LEVEL_IN_TRADE
#define ENABLE_DICE_SYSTEM
#define ENABLE_EXTEND_INVEN_SYSTEM
#define ENABLE_SLOT_WINDOW_EX
#define ENABLE_TEXT_LEVEL_REFRESH
#define ENABLE_USE_COSTUME_ATTR
#define ENABLE_DISCORD_RPC
#define ENABLE_PET_SYSTEM_EX
#define ENABLE_HIGHLIGHT_NEW_ITEM
#define ENABLE_EMOJI_SYSTEM
#define __ENABLE_STEALTH_FIX__
#define ENABLE_MINIMAP_WHITEMARK_CIRCLE
#define WJ_SHOW_MOB_INFO
#ifdef WJ_SHOW_MOB_INFO
#define ENABLE_SHOW_MOBAIFLAG
#define ENABLE_SHOW_MOBLEVEL
#endif
/*****************-- RZECZY Z MARTYSAMY --******************/


#define ENABLE_FOX_FS							// Pack Kilitleme Sistemi. 


/*****************-- SYSTEMY --******************/
#define ENABLE_PERFORMANCE_IMPROVEMENTS_NEW						//Optymalka
//#define __CHANGE_CHANNEL_SYSTEM__								//Zmiana Kanałów
#define ENABLE_GLOBAL_CHAT										//Flaga przy nicku
#define ENABLE_LOCALIZATION_SYSTEM								//Zapis Lokalizcji
#define WEEKLY_RANK_BYLUZER										//Ranking tygodniowy
#define ENABLE_SWITCHBOT										//Switchbot
#define ENABLE_TRASH_SYSTEM										//Kosz Koygera
#define ENABLE_ANTI_EXP											//Anty-Exp
#define ENABLE_DROP_INFO										//Wikipedia Dropu
#define ENABLE_FOV_OPTION										//Nowa Kamera
#define LOTERIA_MOJA											//Losowanie Skrzynek
#define ENABLE_DYNAMIC_FONTS									//Zmiana Czcionki
#define ENABLE_678TH_SKILL										//6/7skill Wojownik & Ninja
#define FULL_YANG												//Yang 2kkk+
#define ENABLE_ALIGN_RENEWAL									//System nowych rang 60k

#define __BL_MOVE_CHANNEL__


/*****************-- SYSTEMY CZ2 --******************/


	////////////// Sklepy offline //////////////
#define ENABLE_IKASHOP_RENEWAL									//S
#define ENABLE_IKASHOP_ENTITIES									//K
#define EXTEND_IKASHOP_PRO										//L
#define EXTEND_IKASHOP_ULTIMATE									//E
#define ENABLE_LARGE_DYNAMIC_PACKETS							//P
	////////////// Sklepy offline //////////////


#define ENABLE_BONUS_SWITCHER
#ifdef ENABLE_BONUS_SWITCHER
#define ENABLE_SWITCH_ITEM_SELECT
#endif


	////////////// ItemShop //////////////
#define ENABLE_ITEMSHOP											//I
#ifdef ENABLE_ITEMSHOP											//T
#define ENABLE_EXTENDED_COIN									//E
#ifdef ENABLE_EXTENDED_COIN										//M
#define ENABLE_EXTENDED_COIN_SPLIT_CATEGORY						
#endif															
#endif															
#define ENABLE_TOMBOLA											//S
#ifdef ENABLE_TOMBOLA											//H
#define ENABLE_TOMBOLA_PERCENTAGE								//O
#endif															//P
	////////////// ItemShop //////////////








/*****************-- MODYFIKACJE --******************/
#define ENABLE_AFFECT_FIX										//Obramowanie affect
#define FIX_MOB_COLLISION_REWORKED								//Przenikanie przez moby, sklepy, ogniska
#define ENABLE_AFFECT_POLYMORPH_REMOVE							//Usunięcie polimorfii
#define __BL_HYPERLINK_ITEM_ICON__								//ikonka przy itemie
#define ENABLE_DOG_MODE											//Zamiana na dzikie psy
#define ENABLE_ENTITY_PRELOADING								//Ulepszone ładowanie jednostek by amun
#define ENABLE_TEXT_SHADOW										//Wyłączenie obrysu na przycisk
#define ENABLE_SHOW_NIGHT_SYSTEM								//Modyfikacja - zmiana tekstury nieba podczas gry
#define MAVIRUH_WATER_SYSTEM									//Modyfikacja - zmiana tekstury wody
#define ENABLE_HORSE_ATTACK_MIN_LEVEL							//Atakowanie z konia od 1 levela
#define STONE_AND_MOB_OPTIMISATION								//Mała optymalizacja fps (zbicie metka, brak efektów)
#define ENABLE_TWEENY_LIBRARY									//Animowane okienka
#define ENABLE_TEXTURE_CHANGER 									//Zmiana podłogi
#define EXTREME_FPS_BOOST										//Zanikanie obiektów pod przycisk (Entity DracaryS)
#define GM_WALLHACK												//Jako GM moge przenikać przez obiekty
#define ENABLE_OFFICIAL_REVERSED_BUTTON							//Do ui.py
#define ENABLE_NO_MOUNT_CHECK									//Do wierzchowców
#define __BL_MINIMAP_SMOOTH_ZOOM__								//Płynne powiększanie minimapy
#define ENABLE_SLOT_TIME_INFO									//Wyświetlanie czasu po użyciu skilli cz.1
#define __BL_SLOT_TIME_INFO__									//Wyświetlanie czasu po użyciu skilli cz.2
#define __BL_PARTY_POSITION__									//ludzie w pt lokalizacja
#define ENABLE_MINIMAP_WHITEMARK_CIRCLE							//Kółka na minimapie

//#define ENABLE_LZ4_PACK_COMPRESSION

/*****************-- MODYFIKACJE CZ2--******************/
#define __BL_ENVIRONMENT_EFFECT_OPTION__						//Pogoda.1
#if defined(__BL_ENVIRONMENT_EFFECT_OPTION__)					//Pogoda.1
	#define __BL_EFFECT_DAY_TYPE__								//Pogoda.1
#endif															//Pogoda.1
#if defined(ENABLE_FIX_MOBS_LAG)								//The define ENABLE_FIX_MOBS_LAG have problems in device reseting.
#define FIX_MOBS_LAG_FIX										//With this new define all this problems are fixed.
#endif															///////////////////////////////////////////////////






/*****************-- FIXY --******************/
#define __FONT_TEXTURE_FIX__									//Owsap fix font 
#define LALAKER_FIX_MEMORYLEAK									//Naprawa wycieku pamięci lalaker1
#define ENABLE_ITEM_MOVE_FIX									//Naprawiono miejsca w ekwipunku podczas próby przeniesienia przedmiotów
#define ENABLE_MOVEMENT_FIX										//Fix z ruchem?
#define FIX_PLAYERS_LOAD										//Ładowanie graczy
#define FIX_CONNECT_KEN											//Naprawa połączenia by Ken
#define FULL_YANG_FIX											//Dodatki naprawcze po odblokowaniu limitu yang
#define FIXME440												//Szybsze pojawiania się modeli/mobów ---- IAbstractApplication::GetSingleton().SkipRenderBuffering(300);
#define MEMORY_LEAK_FIX											//Naprawa wycieków pamięci któraś część z kolei...
#define UPDATE_TELEPORT_CRASH									//Fix na teleport crash z visuala
#define COMBO_FIX												//Combo tylko działa kiedy nie atakujesz bronią (jaki bug był)
#define FOG_FIX													//[REVERSED] Fog Fix				* Author: Mali		*





/*****************-- W Y Ł Ą C Z O N E --******************/




//#define ENABLE_ENERGY_SYSTEM
//#define ENABLE_DRAGON_SOUL_SYSTEM
//#define ENABLE_LVL115_ARMOR_EFFECT
//#define ENABLE_MAGIC_REDUCTION_SYSTEM
//#define ENABLE_WOLFMAN_CHARACTER
//#define ENABLE_ACCE_COSTUME_SYSTEM
//#define ENABLE_OBJ_SCALLING
// #define USE_ACCE_ABSORB_WITH_NO_NEGATIVE_BONUS
// for debug: print received packets
// #define ENABLE_PRINT_RECV_PACKET_DEBUG
// if is define ENABLE_ACCE_COSTUME_SYSTEM the players can use shoulder sash
// if you want to use object scaling function you must defined ENABLE_OBJ_SCALLING

/*****************-- W Y Ł Ą C Z O N E --******************/