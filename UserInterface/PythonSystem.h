#pragma once

class CPythonSystem : public CSingleton<CPythonSystem>
{
	public:
		enum EWindow
		{
			WINDOW_STATUS,
			WINDOW_INVENTORY,
			WINDOW_ABILITY,
			WINDOW_SOCIETY,
			WINDOW_JOURNAL,
			WINDOW_COMMAND,

			WINDOW_QUICK,
			WINDOW_GAUGE,
			WINDOW_MINIMAP,
			WINDOW_CHAT,

			WINDOW_MAX_NUM,
		};

		enum
		{
			FREQUENCY_MAX_NUM  = 30,
			RESOLUTION_MAX_NUM = 100
		};

		typedef struct SResolution
		{
			DWORD	width;
			DWORD	height;
			DWORD	bpp;		// bits per pixel (high-color = 16bpp, true-color = 32bpp)

			DWORD	frequency[20];
			BYTE	frequency_count;
		} TResolution;

		typedef struct SWindowStatus
		{
			int		isVisible;
			int		isMinimized;

			int		ixPosition;
			int		iyPosition;
			int		iHeight;
		} TWindowStatus;

		typedef struct SConfig
		{
			DWORD			width;
			DWORD			height;
			DWORD			bpp;
			DWORD			frequency;

			bool			is_software_cursor;
			bool			is_object_culling;
			int				iDistance;
			int				iShadowLevel;

			FLOAT			music_volume;
			BYTE			voice_volume;

			int				gamma;

			int				isSaveID;
			char			SaveID[20];

			bool			bWindowed;
			bool			bDecompressDDS;
			bool			bNoSoundCard;
			bool			bUseDefaultIME;
			BYTE			bSoftwareTiling;
			bool			bViewChat;
			bool			bAlwaysShowName;
			bool			bShowDamage;
			bool			bShowSalesText;
			uint8_t			bCameraMode; //@natanek97

#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_PRO
			float			shopRangeView = 5000.f;
#endif
#endif

			#if defined(FOG_FIX)
			bool			bIsFogModeEnabled;
			#endif

#ifdef ENABLE_TEXTURE_CHANGER
			bool			bSnowTexturesMode;
			bool			bDesertTexturesMode;
			bool			bGreenTexturesMode;
#endif

#ifdef ENABLE_SHOW_NIGHT_SYSTEM
			int				bNightlyMode;
#endif

#ifdef MAVIRUH_WATER_SYSTEM
			int				bMaviRuhSuDegistir;
#endif

#if defined(__BL_ENVIRONMENT_EFFECT_OPTION__)
			bool			bNightMode;
			bool			bSnowMode;
			bool			bSnowTextureMode;
#endif

#ifdef ENABLE_TEXT_SHADOW
			bool			bTextShadowMode;
#endif

#ifdef EXTREME_FPS_BOOST
		bool				bExtremeFpsBoost;
#endif

#ifdef ENABLE_DOG_MODE
			bool			bDogMode;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
			bool			bShowMobAIFlag;
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
			bool			bShowMobLevel;
#endif
#if defined(ENABLE_FOV_OPTION)
		float fFOV;
#endif
		} TConfig;

	public:
		CPythonSystem();
		virtual ~CPythonSystem();

		void Clear();
		void SetInterfaceHandler(PyObject * poHandler);
		void DestroyInterfaceHandler();

		// Config
		void							SetDefaultConfig();
		bool							LoadConfig();
		bool							SaveConfig();
		void							ApplyConfig();
		void							SetConfig(TConfig * set_config);
		TConfig *						GetConfig();
		void							ChangeSystem();

		// Interface
		bool							LoadInterfaceStatus();
		void							SaveInterfaceStatus();
		bool							isInterfaceConfig();
		const TWindowStatus &			GetWindowStatusReference(int iIndex);

		DWORD							GetWidth();
		DWORD							GetHeight();
		DWORD							GetBPP();
		DWORD							GetFrequency();
		bool							IsSoftwareCursor();
		bool							IsWindowed();
		bool							IsViewChat();
		bool							IsAlwaysShowName();
		bool							IsShowDamage();
		bool							IsShowSalesText();
		bool							IsUseDefaultIME();
		bool							IsNoSoundCard();
		bool							IsAutoTiling();
		bool							IsSoftwareTiling();
		void							SetSoftwareTiling(bool isEnable);
		void							SetViewChatFlag(int iFlag);
		void							SetAlwaysShowNameFlag(int iFlag);
		void							SetShowDamageFlag(int iFlag);
		void							SetShowSalesTextFlag(int iFlag);

#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_PRO
		void							SetShopRangeView(float distance) { m_Config.shopRangeView = MINMAX(600.0f, distance, 5000.f); }
		float							GetShopRangeView() const {return m_Config.shopRangeView;}
#endif
#endif

			void						SetCameraMode(uint8_t bMode); //@natanek97
			uint8_t						GetCameraMode() const; //@natanek97

#ifdef MAVIRUH_WATER_SYSTEM
		void							SetMaviRuhSuDegistir(unsigned int level);
		int								GetMaviRuhSuDegistir();
#endif

#ifdef ENABLE_SHOW_NIGHT_SYSTEM
		void							SetNightMode(unsigned int level);
		int								GetNightMode();
#endif

		void							SetDogMode(int iFlag);
		bool							GetDogMode();
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
		bool							IsShowMobAIFlag();
		void							SetShowMobAIFlagFlag(int iFlag);
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
		bool							IsShowMobLevel();
		void							SetShowMobLevelFlag(int iFlag);
#endif

#ifdef ENABLE_TEXT_SHADOW
		void							SetTextShadowMode(int iFlag);
		bool							GetTextShadowMode();
#endif

#ifdef ENABLE_TEXTURE_CHANGER
		void							SetSnowTexturesMode(int iFlag);
		bool							IsSnowTexturesMode();
		void							SetDesertTexturesMode(int iFlag);
		bool							IsDesertTexturesMode();
		void							SetGreenTexturesMode(int iFlag);
		bool							IsGreenTexturesMode();
#endif

		// Window
		void							SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight);

		// SaveID
		int								IsSaveID();
		const char *					GetSaveID();
		void							SetSaveID(int iValue, const char * c_szSaveID);

		/// Display
		void							GetDisplaySettings();

		int								GetResolutionCount();
		int								GetFrequencyCount(int index);
		bool							GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp);
		bool							GetFrequency(int index, int freq_index, OUT DWORD *frequncy);
		int								GetResolutionIndex(DWORD width, DWORD height, DWORD bpp);
		int								GetFrequencyIndex(int res_index, DWORD frequency);
		bool							isViewCulling();

		// Sound
		float							GetMusicVolume();
		int								GetSoundVolume();
		void							SetMusicVolume(float fVolume);
		void							SetSoundVolumef(float fVolume);

		int								GetDistance();

#if defined(__BL_ENVIRONMENT_EFFECT_OPTION__)
		void							SetNightModeOption(bool bEnable);
		bool							GetNightModeOption() const;
		
		void							SetSnowModeOption(bool bEnable);
		bool							GetSnowModeOption() const;

		void							SetSnowTextureModeOption(bool bEnable);
		bool							GetSnowTextureModeOption() const;
#endif

#if defined(ENABLE_FOV_OPTION)
	// FoV
	float GetFOV();
	void SetFOV(float c_fValue);
#endif

#ifdef EXTREME_FPS_BOOST
	void							SetExtremeFpsBoost(bool bVal);
	bool							GetExtremeFpsBoost() { return m_Config.bExtremeFpsBoost; }
#endif

		int								GetShadowLevel();
		void							SetShadowLevel(unsigned int level);

		#if defined(FOG_FIX)
		void							SetFogMode (bool bEnable);
		bool							GetFogMode() const;
		#endif

	protected:
		TResolution						m_ResolutionList[RESOLUTION_MAX_NUM];
		int								m_ResolutionCount;

		TConfig							m_Config;
		TConfig							m_OldConfig;

		bool							m_isInterfaceConfig;
		PyObject *						m_poInterfaceHandler;
		TWindowStatus					m_WindowStatus[WINDOW_MAX_NUM];
};
//martysama0134
