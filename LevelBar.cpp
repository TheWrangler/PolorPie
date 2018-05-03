/*!
 * @file LevelBar.cpp
 * @brief CLevelBar类实现文件
 *
 * @author wrangler <onlymydreams@126.com>
 * @version 1.0
 */

#include "StdAfx.h"
#include "LevelBar.h"

/*!
 * @brief 默认构造函数
 *        
 *        初始化控件客户区矩形为(0,0,50,200),
 *        初始化绘制文本使用的颜色为白色，
 *        初始化绘制刻度线使用的颜色为白色。
 */
CLevelBar::CLevelBar(void)
{
	m_ClientRect = Gdiplus::RectF(0,0,50,200);
	m_TextureColor = Gdiplus::Color(255,0,0,64);
	m_GirdColor = Gdiplus::Color(255,255,255,255);
}

/*!
 * @brief 析构函数  
 */
CLevelBar::~CLevelBar(void)
{
}

/*!
 * @brief 设置色条控件的客户区
 *
 * @param rt 客户区区域
 */
void CLevelBar::SetClientRect(Gdiplus::RectF &rt)
{
	m_ClientRect.X = rt.X;
	m_ClientRect.Y = rt.Y;
	m_ClientRect.Width= rt.Width;
	m_ClientRect.Height = rt.Height;

	m_BarRect.X = m_ClientRect.X;
	m_BarRect.Y = m_ClientRect.Y;
	m_BarRect.Width = m_ClientRect.Width/3;
	m_BarRect.Height = m_ClientRect.Height;
}

/*!
 * @brief 设置色条Level的范围
 *
 * @param min Level下限
 * @param max Level上限
 */
void CLevelBar::SetLevelRange(float min,float max)
{
	m_ColorLevel.SetLevelRange(max,min);
}

/*!
 * @brief 获取对应level值的颜色
 *
 * @param level level值
 * @param color 颜色
 */
void CLevelBar::GetColor(float level,Gdiplus::Color& color)
{
	COLORREF levelColor = m_ColorLevel.GetColor(level);
	color.SetFromCOLORREF(levelColor);
}

/*!
 * @brief 绘制色条控件
 *
 * @param graphics 设备上下文对象，GDI+对象
 */
void CLevelBar::DrawBar(Gdiplus::Graphics& graphics)
{
	float nLevels = m_ColorLevel.GetLevelCount();
	float nHeight = m_BarRect.Height;
	float width = m_BarRect.Width;
	float bottom = m_BarRect.Y + m_BarRect.Height;
	Gdiplus::Color color;
	COLORREF levelColor;
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255,0,0,0));
	for(int i=0;i<= nHeight;i++)
	{
		levelColor = m_ColorLevel.GetLevelColor(((float)i)/(nHeight+1) * nLevels);
		color.SetFromCOLORREF(levelColor);
		brush.SetColor(color);
		graphics.FillRectangle(&brush,m_BarRect.X,bottom - i,m_BarRect.Width,1.0);
	}

	Gdiplus::Pen gridPen(m_GirdColor,0);
	graphics.DrawLine(&gridPen,m_BarRect.GetRight(),m_BarRect.Y,
							   m_BarRect.GetRight(),m_BarRect.GetBottom());

	Gdiplus::FontFamily fontFamily(_T("Arial"));
	Gdiplus::Font font(&fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	CString strVal;
	float minLevel,maxLevel;
	m_ColorLevel.GetLevelRange(maxLevel,minLevel);
	float rangeUnit = (maxLevel - minLevel) / 5.0;
	float heightUnit = nHeight / 5.0;
	int level;
	brush.SetColor(m_TextureColor);
	Gdiplus::PointF pointF;
	for(int i=0;i<6;i++)
	{	
		graphics.DrawLine(&gridPen,m_BarRect.GetRight(),m_BarRect.GetBottom() - i*heightUnit,
								   m_BarRect.GetRight()+5.0,m_BarRect.GetBottom() - i*heightUnit);
		level = minLevel + rangeUnit * i;
		strVal.Format(_T("%d"),level);
		pointF.X = m_BarRect.GetRight()+7.0;
		pointF.Y = bottom-i*heightUnit - 7.0;
		graphics.DrawString(strVal,-1,&font,pointF,&brush);
	}
}
