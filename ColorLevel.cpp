/*!
 * @file ColorLevel.cpp
 * @brief CColorLevel类实现文件
 *
 * @author wrangler <onlymydreams@126.com>
 * @version 1.0
 */

#include "StdAfx.h"
#include "ColorLevel.h"


/*!
 * @brief 默认构造函数
 *        
 *        Level范围初始化为0~10，对应从蓝到红渐变的1024种颜色
 */
CColorLevel::CColorLevel(void)
{
	int clrLevel;
	for(int g =0;g<256;g++)
	{
		clrLevel = RGB(0,g,255);
		m_Colors.push_back(clrLevel);
	}

	for(int b = 255;b>=0;b--)
	{
		clrLevel = RGB(0,255,b);
		m_Colors.push_back(clrLevel);
	}

    for(int r =0;r<256;r++)
	{
		clrLevel = RGB(r,255,0);
		m_Colors.push_back(clrLevel);
	}

	for(int g = 255;g>=0;g--)
	{
		clrLevel = RGB(255,g,0);
		m_Colors.push_back(clrLevel);
	}

	m_MaxLevel = 10.0;
	m_MinLevel = 0.0;

	m_ScaleUnit = ((float)m_Colors.size()) / (m_MaxLevel - m_MinLevel);
}

/*!
 * @brief 构造函数
 *
 *        Level范围初始化为minLevel~maxLevel,对应从蓝到红渐变的1024种颜色
 *
 * @param maxLevel 初始化Level的上限
 * @param minLevel 初始化Level的下限   
 */
CColorLevel::CColorLevel(float maxLevel,float minLevel)
{
	CColorLevel();
	m_MaxLevel = maxLevel;
	m_MinLevel = minLevel;

	m_ScaleUnit = ((float)m_Colors.size()) / (m_MaxLevel - m_MinLevel);
}

/*!
 * @brief 析构函数  
 */
CColorLevel::~CColorLevel(void)
{
}

/*!
 * @brief 获取颜色总数
 *
 *        获取颜色向量m_Colors里存储的颜色总数
 *
 * @return 颜色总数
 */
int CColorLevel::GetLevelCount()
{
	return m_Colors.size();
}

/*!
 * @brief 设置Level的范围
 *
 * @param maxLevel Level的上限
 * @param minLevel Level的下限      
 */
void CColorLevel::SetLevelRange(float maxLevel,float minLevel)
{
	m_MaxLevel = maxLevel;
	m_MinLevel = minLevel;

	m_ScaleUnit = ((float)m_Colors.size()) / (m_MaxLevel - m_MinLevel);
}

/*!
 * @brief 获取Level的范围
 *
 * @param maxLevel Level的上限
 * @param minLevel Level的下限      
 */
void CColorLevel::GetLevelRange(float& maxLevel,float& minLevel)
{
	maxLevel = m_MaxLevel;
	minLevel = m_MinLevel;
}

/*!
 * @brief 根据level值返回对应的颜色
 *
 * @param level Level值
 * @return 对应level的颜色      
 */
COLORREF CColorLevel::GetColor(float level)
{
	float iIdx = (level-m_MinLevel) * m_ScaleUnit;
	if(iIdx < 0)
		return RGB(0,0,0);
	else if(iIdx >= m_Colors.size())
		return RGB(255,0,0);
	else return m_Colors[iIdx];
}

/*!
 * @brief 根据颜色索引返回对应的颜色
 *
 * @param levelIdx 颜色索引
 * @return 对应颜色索引的颜色      
 */
COLORREF CColorLevel::GetLevelColor(int levelIdx)
{
	return m_Colors[levelIdx];
}
