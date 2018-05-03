/*!
 * @file ColorLevel.h
 * @brief CColorLevel类定义文件
 * @author wrangler <onlymydreams@126.com>
 * @version 1.0
 */

#pragma once
#include <vector>
using namespace std;

/*!
 * @class CColorLevel
 * @brief 色条颜色管理类
 *
 *        对色条所使用的颜色进行管理，提供Level值与颜色的转换；
 */

class AFX_EXT_CLASS CColorLevel
{
public:
	CColorLevel(void);
	CColorLevel(float maxLevel,float minLevel);
	~CColorLevel(void);

protected:
	/*!
	 * @brief 颜色向量 
	 */
	vector<COLORREF> m_Colors;
	/*!
	 * @brief Level的上限
	 */
	float m_MaxLevel;
	/*!
	 * @brief Level的下线
	 */
	float m_MinLevel;
	/*!
	 * @brief Level与颜色索引值的转换单位
	 */
	float m_ScaleUnit;

public:
	void GetLevelRange(float& maxLevel,float& minLevel);
	void SetLevelRange(float maxLevel,float minLevel);
	int GetLevelCount();
	COLORREF GetLevelColor(int levelIdx);
	COLORREF GetColor(float level);
};

