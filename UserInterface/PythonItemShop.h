#pragma once

#include "Packet.h"

enum EEditorFlag
{
	EDITOR_FLAG_ADD,
	EDITOR_FLAG_EDIT,
	EDITOR_FLAG_DELETE
};

typedef struct STombolaSetting
{
	float fSpinSpeed;
	int iBlinkCount;
	int iMinSpin;
	int iMaxSpin;
	int iSlowMin;
	int iSlowMax;
} TTombolaSetting;

static const TTombolaSetting tombolaSetting = {
	0.1, 6, 1, 3, 4, 10
};

#ifdef ENABLE_EXTENDED_COIN
enum ECashType
{
	CASH_TYPE_COIN,
	CASH_TYPE_MARK,

	CASH_TYPE_MAX_NUM
};
#endif

enum EItemShopBonusType
{
	BONUS_NONE,
	BONUS_EXP,
	BONUS_PVP,
	BONUS_MAX_NUM,
};

class CItemShop : public CSingleton<CItemShop>
{
public:
	CItemShop();
	virtual						~CItemShop();
	void						SetWnd(PyObject* pyWnd) { m_pyItemShopWnd = pyWnd; }

	void						ClearItems();
	void						SetItemData(TItemShopItem item);
	void						DelItemData(DWORD dwSlotIndex, DWORD dwCategoryIndex);
	void						UpdateItem(TItemShopItem item);
	void						SetBuyStats(TItemShopBuyStats item);
	void						RefreshLimitedCountItem(TItemShopBuyStats item);
	void						SetDailyReward(BYTE byDay, TItemShopDailyReward reward);
	void						AppendCategory(BYTE byIndex, std::string stName);

#ifdef ENABLE_EXTENDED_COIN
	void						SetCoins(DWORD coins, BYTE type) { m_dwCoins[type] = coins; }
	DWORD						GetCoins(BYTE type) { return m_dwCoins[type]; }
#else
	void						SetCoins(DWORD coins) { m_dwCoins = coins; }
	DWORD						GetCoins() { return m_dwCoins; }
#endif

protected:
#ifdef ENABLE_EXTENDED_COIN
	DWORD						m_dwCoins[CASH_TYPE_MAX_NUM];
#else
	DWORD						m_dwCoins;
#endif
	PyObject*					m_pyItemShopWnd;
};
