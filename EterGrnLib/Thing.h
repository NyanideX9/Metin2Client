#pragma once

#include "Model.h"
#include "Motion.h"

#include <string_view> // ← to MUSI być obecne
#include <cstdint>     // ← jeśli używasz int32_t

class CGraphicThing : public CResource
{
	public:
		typedef CRef<CGraphicThing> TRef;

	public:
		static CGraphicThing::TType Type();

	public:
		CGraphicThing(const char * c_szFileName);
		virtual ~CGraphicThing();

		virtual bool			CreateDeviceObjects();
		virtual void			DestroyDeviceObjects();

		bool					CheckModelIndex(int iModel) const;
		CGrannyModel *			GetModelPointer(int iModel);
		int						GetModelCount() const;

		bool					CheckMotionIndex(int iMotion) const;
		CGrannyMotion *			GetMotionPointer(int iMotion);
		int						GetMotionCount() const;


		//@fix65
		int32_t GetTextureCount() const;
		std::string_view GetTexturePath(int32_t textureIndex) const;
		//@fix65

	protected:
		void					Initialize();

		bool					LoadModels();
		bool					LoadMotions();

	protected:
		bool					OnLoad(int iSize, const void* c_pvBuf);
		void					OnClear();
		bool					OnIsEmpty() const;
		bool					OnIsType(TType type);

	protected:
		granny_file *			m_pgrnFile;
		granny_file_info *		m_pgrnFileInfo;

		granny_animation *		m_pgrnAni;

		CGrannyModel *			m_models;
		CGrannyMotion *			m_motions;
};
//martysama0134
