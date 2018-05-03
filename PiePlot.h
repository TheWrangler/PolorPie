/*!
 * @file PiePlot.h
 * @brief CPiePlot类定义文件
 * @author wrangler <onlymydreams@126.com>
 * @version 1.0
 */

#pragma once
#include "LevelBar.h"
#include <vector>
using namespace std;

/*!
 * @class CPiePlot
 * @brief PiePlot控件类
 *
 *        PiePlot控件类，继承自CWnd，是极坐标下的伪彩图或瀑布图控件，
 *        对一组数据按照角度进行绘制，绘制采用的
 *        颜色是将数据的值作为level值通过CLevelBar类
 *        获取对应的颜色。
 */
class AFX_EXT_CLASS CPiePlot : public CWnd
{
	DECLARE_DYNAMIC(CPiePlot)

public:
	CPiePlot();
	virtual ~CPiePlot();

    /*! 
     * @var PI
     * @brief 圆周率
     */
	const static float PI;

	/*!
	 * @typedef PieData
	 * @brief 定义tagPieData为PieData类型
	 */
	/*!
	 * @typedef PTR_PieData
	 * @brief 定义tagPieData*为PTR_PieData类型
	 */
	/*!
	 * @struct tagPieData
	 * @brief PiePlot控件绘制采用的数据结构
	 */
	typedef struct tagPieData
	{
		double angle;/*!< 角度 */
		double* pData;/*!< 数据指针 */
		double* pRange;/*!< 距离数据指针 */
		double size;/*!< 数据量 */
	}PieData,*PTR_PieData;

protected:
	/*!
	 * @brief 控件设备上下文对象指针 
	 */
	CDC     m_dcGrid;
	/*!
	 * @brief 控件设备上下文对象内存指针 
	 */
	CBitmap *m_pbitmapOldGrid;
	/*!
	 * @brief 控件设备上下文对象旧内存指针 
	 */
	CBitmap m_bitmapGrid;

	/*!
	 * @brief 控件客户区
	 */
	Gdiplus::RectF m_ClientRect;
	/*!
	 * @brief 控件宽度
	 */	
	float m_ClientWidth;
	/*!
	 * @brief 控件高度
	 */	
	float m_ClientHeight;

	/*!
	 * @brief 饼图区域
	 */	
	Gdiplus::RectF m_PieRect;
	/*!
	 * @brief 饼图半径
	 */	
	float m_PieRadius;
	/*!
	 * @brief 饼图圆心点
	 */	
	Gdiplus::PointF m_PieCenter;


	/*!
	 * @brief 控件背景颜色
	 */	
	Gdiplus::Color m_BackColor;
	/*!
	 * @brief 控件刻度线颜色
	 */	
	Gdiplus::Color m_GirdColor;
	/*!
	 * @brief Tooltip工具颜色
	 */	
	Gdiplus::Color m_ToolTipColor;

	/*!
	 * @brief 距离最小值
	 */	
	float m_RangeMin;
	/*!
	 * @brief 距离最大值
	 */	
	float m_RangeMax;
	/*!
	 * @brief 距离
	 */	
	float m_Range;

	/*!
	 * @brief 当前角度
	 */	
	float m_curAngle;

	/*!
	 * @brief 饼图半径与距离比例因子
	 */	
	float m_RadScaleFactor;

	/*!
	 * @brief 饼图数据向量
	 */	
	vector<PTR_PieData> m_Datas;

	/*!
	 * @brief 色条控件
	 */	
	CLevelBar m_LevelBar;

	int m_PlotMode;
	double m_ConstAngle;

public:
	void SetMode(int mode,float fyAngle,float fwAngle);
	void SetLevelRange(float min,float max);
	void SetRange(float min,float max);
	void ScanToAngle(double* pRange,double* pData,int size,float angle,BOOL bPaint);
	void ClearScan();
	//TODO:zoom
	void Zoom2Range(float zoomMin,float zoomMax);
	void ZoomOff();

	static float angle2rad(float angle){ return angle/180.0 * PI;}
	static float rad2angle(float rad){ return rad * 180 / PI; }

protected:
	void UpdateRadScaler();
	void UpdatePlot();
	float Range2Rad(float range);
	float Rad2Range(float rad);
	void Range2Point(float range,float angle,float& rad,Gdiplus::PointF& pt);
	void Point2Range(float& range,float &angle,float& rad,Gdiplus::PointF& pt);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); 
};


