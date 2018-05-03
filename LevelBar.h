/*!
 * @file LevelBar.h
 * @brief CLevelBar类定义文件
 * @author wrangler <onlymydreams@126.com>
 * @version 1.0
 */

#pragma once
#include "ColorLevel.h"

/*!
 * @class CLevelBar
 * @brief 色条控件信息类，管理色条的绘制信息
 *
 *        色条相关绘制信息包括所在区域、色条文本颜色、
 *        刻度颜色、色条颜色。
 */
class AFX_EXT_CLASS CLevelBar
{
public:
	CLevelBar(void);
	~CLevelBar(void);

protected:
	/*!
	 * @brief 色条颜色管理 
	 */
	CColorLevel m_ColorLevel;
	/*!
	 * @brief 色条与刻度构成的控件客户区
	 */
	Gdiplus::RectF m_ClientRect;
	/*!
	 * @brief 色条区
	 */
	Gdiplus::RectF m_BarRect;
	/*!
	 * @brief 绘制色条控件文本所使用的颜色
	 */
	Gdiplus::Color m_TextureColor;
	/*!
	 * @brief 绘制色条刻度线所使用的颜色
	 */
	Gdiplus::Color m_GirdColor;

public:
	/*!
 	 * @brief 设置绘制色条刻度线使用的颜色
 	 *
 	 * @param gridColor 颜色
 	 */
	void SetGirdColor(Gdiplus::Color gridColor) { m_GirdColor = gridColor; }
	/*!
 	 * @brief 设置绘制色条控件文本使用的颜色
 	 *
 	 * @param textureColor 颜色
 	 */
	void SetTextureColor(Gdiplus::Color textureColor) { m_TextureColor = textureColor; }
	void SetLevelRange(float min,float max);
	void SetClientRect(Gdiplus::RectF &rt);
	void DrawBar(Gdiplus::Graphics& graphics);
	void GetColor(float level,Gdiplus::Color& color);
};

