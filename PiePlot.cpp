/*!
 * @file PiePlot.cpp
 * @brief CPiePlot类实现文件
 * @author wrangler <onlymydreams@126.com>
 * @version 1.0
 */

#include "stdafx.h"
#include "PiePlot.h"
#include "Math.h"
#include "atlimage.h"

const float CPiePlot::PI = 3.1415926;

IMPLEMENT_DYNAMIC(CPiePlot, CWnd)

/*!
 * @brief 默认构造函数
 *        
 *        距离范围初始化为0.0~12000.0，Level范围初始化为0~3000，
 *        当前角度初始化为0.0。
 */
CPiePlot::CPiePlot()
{
	m_RangeMin = 0.0;
	m_RangeMax = 12000.0;
	m_Range = m_RangeMax - m_RangeMin;
	m_LevelBar.SetLevelRange(0,3000.0);
	m_curAngle = 30.0;
	m_ConstAngle = 30.0;
	m_PlotMode = 0;
	m_BackColor = Gdiplus::Color(255,0, 0, 64);
	m_GirdColor = Gdiplus::Color(255,0, 0, 64);
	m_ToolTipColor = Gdiplus::Color(255,0, 255, 255);
	m_Datas.clear();
	m_LevelBar.SetGirdColor(m_GirdColor);
}

/*!
 * @brief 析构函数  
 */
CPiePlot::~CPiePlot()
{
}

BEGIN_MESSAGE_MAP(CPiePlot, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/*!
 * @brief Create重写函数
 *        
 *        Create重写函数，调用此函数创建控件对象。
 */
BOOL CPiePlot::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, UINT nID) 
{
  BOOL result ;
  static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;

  result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
                          className, NULL, dwStyle, 
                          rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
                          pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
  if (result != 0)
    UpdatePlot() ;

  return result ;
}


void CPiePlot::SetMode(int mode,float fyAngle,float fwAngle)
{ 
	m_PlotMode = mode; 
	if(m_PlotMode == 0)
	{
		m_curAngle = fwAngle;
		m_ConstAngle = fyAngle;
	}
	else
	{
		m_curAngle = fyAngle;
		m_ConstAngle = fwAngle;
	}
	
	ClearScan();
}
/*!
 * @brief 设置距离范围
 *        
 * @param min 距离下限
 * @param max 距离上限
 */
void CPiePlot::SetRange(float min,float max)
{
	m_RangeMin = min;
	m_RangeMax = max;
	m_Range = m_RangeMax - m_RangeMin;

	UpdateRadScaler();
	UpdatePlot();
}

/*!
 * @brief 设置Level范围
 *        
 * @param min Level下限
 * @param max Level上限
 */
void CPiePlot::SetLevelRange(float min,float max)
{
	m_LevelBar.SetLevelRange(min,max);
	UpdatePlot();
}

/*!
 * @brief 更新控件进行重新绘图
 */
void CPiePlot::UpdatePlot()
{
  CClientDC dc(this);  

  if (m_dcGrid.GetSafeHdc() == NULL)
  {
    m_dcGrid.CreateCompatibleDC(&dc);
    m_bitmapGrid.CreateCompatibleBitmap(&dc, m_ClientWidth, m_ClientHeight) ;
    m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid) ;
  }

  Gdiplus::Graphics graphics(m_dcGrid.m_hDC);
  graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
  
  Gdiplus::Image bkImage(_T("F:/radar_wsp/Debug/map/1.png"));
  //image.FromFile(_T(""));
  graphics.DrawImage(&bkImage,0,0,bkImage.GetWidth(),bkImage.GetHeight());
  //Gdiplus::SolidBrush backBrush(Gdiplus::Color(255,0,0,64));
  //graphics.FillRectangle(&backBrush,m_ClientRect);


  Gdiplus::Pen gridPen(m_GirdColor,0);
  graphics.DrawEllipse(&gridPen,m_PieRect);

  m_LevelBar.DrawBar(graphics);

  Invalidate();
}

/*!
 * @brief WM_SIZE消息句柄函数
 *
 *        控件大小改变时调用此函数。       
 */
void CPiePlot::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rt;
	GetClientRect(&rt);
	m_ClientRect.X = rt.left;
	m_ClientRect.Y = rt.top;
	m_ClientRect.Width = rt.Width();
	m_ClientRect.Height = rt.Height();
	m_ClientWidth = m_ClientRect.Width;
	m_ClientHeight = m_ClientRect.Height;

	Gdiplus::RectF rectLevelBar;
	rectLevelBar.X = m_ClientRect.GetRight() - 80.0;
	rectLevelBar.Y = m_ClientRect.Y + (m_ClientHeight / 2.0);
	rectLevelBar.Width = 60.0;
	rectLevelBar.Height = m_ClientHeight / 2.0 - 20.0;
	m_LevelBar.SetClientRect(rectLevelBar);

	float width = (m_ClientWidth -100.0) / 2.0;
	float height = (m_ClientHeight -40.0) / 2.0;
	m_PieRadius = width > height ? height : width;
	if(m_PieRadius < 200.0)
		m_PieRadius = 200.0;
	m_PieRect.X = m_ClientRect.X + (m_ClientWidth -80.0) / 2.0 - m_PieRadius;
	m_PieRect.Y = m_ClientRect.Y + m_ClientHeight / 2.0 - m_PieRadius;
	m_PieRect.Width  = m_PieRadius * 2.0;
	m_PieRect.Height = m_PieRadius * 2.0;;
	m_PieCenter.X = m_PieRect.X + m_PieRadius;
	m_PieCenter.Y = m_PieRect.Y + m_PieRadius;
	
	UpdateRadScaler(); 
}

/*!
 * @brief WM_PAINT消息句柄函数
 *        
 *        控件需要重绘时调用此函数。
 */
void CPiePlot::OnPaint() 
{
  CPaintDC dc(this);  

  if(dc.GetSafeHdc() != NULL)
  {
    dc.BitBlt(0, 0, m_ClientWidth, m_ClientHeight, 
                 &m_dcGrid, 0, 0, SRCCOPY);
  }
}

/*!
 * @brief 更新饼图半径与距离比例因子
 *        
 */
void CPiePlot::UpdateRadScaler()
{
	m_RadScaleFactor = m_PieRadius / m_Range;
}

/*!
 * @brief 距离转换为半径
 *        
 * @param range 距离值
 * @return 半径
 */
float CPiePlot::Range2Rad(float range)
{
	return (range - m_RangeMin) * m_RadScaleFactor;
}

/*!
 * @brief 半径转换为距离
 *        
 * @param rad 半径
 * @return 距离
 */
float CPiePlot::Rad2Range(float rad)
{
	return  rad / m_RadScaleFactor + m_RangeMin;
}

/*!
 * @brief 给定距离和角度转换为圆周上的点
 *        
 * @param range 距离
 * @param fwAngle 角度
 * @param rad 圆周半径
 * @param pt 圆周上的点
 */
void CPiePlot::Range2Point(float range,float angle,float& rad,Gdiplus::PointF& pt)
{
	rad = Range2Rad(range);
	pt.X = m_PieCenter.X + rad * sin(angle / 180.0 * PI);
	pt.Y = m_PieCenter.Y - rad * cos(angle / 180.0 * PI);
}

/*!
 * @brief 给定圆周上的点转换为角度和距离
 *        
 * @param range 距离
 * @param fwAngle 角度
 * @param rad 圆周半径
 * @param pt 圆周上的点
 */
void CPiePlot::Point2Range(float& range,float &angle,float& rad,Gdiplus::PointF& pt)
{
	rad = sqrt(pow(pt.X - m_PieCenter.X, 2) + pow(pt.Y - m_PieCenter.Y,2));
	angle = asin((pt.X - m_PieCenter.X) / rad) * 180.0 / PI;
	range = Rad2Range(rad);
}

/*!
 * @brief 从当前角度绘制到目标角度
 *        
 * @param pRange 距离数据
 * @param pData 数据
 * @param size 数据量
 * @param angle 目标角度
 * @param bPaint 是否立即更新控件。设为TRUE时，将立即绘制到控件界面上。
 */
void CPiePlot::ScanToAngle(double* pRange,double* pData,int size,float angle,BOOL bPaint)
{
	PTR_PieData pPieData = new PieData();
	pPieData->angle = angle;
	pPieData->size = size;
	pPieData->pRange = new double[size];
	memcpy(pPieData->pRange,pRange,sizeof(double)*size);
	pPieData->pData = new double[size];
	memcpy(pPieData->pData,pData,sizeof(double)*size);

	Gdiplus::Graphics graphics(m_dcGrid.m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	Gdiplus::PointF oldpt,newpt;
	Gdiplus::RectF rt;
	float rad;
	Gdiplus::Color color;
	Gdiplus::SolidBrush brush(Gdiplus::Color(255,0,0,64));
	for(int i=size-1;i>=0;i--)
	{
		if(m_PlotMode == 1)
			rad = Range2Rad(pRange[i]/sin(angle2rad(angle)));
		else rad = Range2Rad(pRange[i]/sin(angle2rad(m_ConstAngle)));
		//rad = Range2Rad(pRange[i]);
		rt.X = m_PieCenter.X - rad;
		rt.Y = m_PieCenter.Y - rad;
		rt.Width = rad * 2.0;
		rt.Height = rad * 2.0;

		m_LevelBar.GetColor(pData[i],color);
		brush.SetColor(color);
		graphics.FillPie(&brush,rt,m_curAngle,angle - m_curAngle);
	}
	m_curAngle = angle;

	if(bPaint)
		Invalidate();
}

/*!
 * @brief 清除控件已绘制部分
 */
void CPiePlot::ClearScan()
{
	vector<PTR_PieData>::iterator it = m_Datas.begin();
	while(it != m_Datas.end())
	{
		delete[] (*it)->pData;
		delete[] (*it)->pRange;
		delete (*it);
		it++;
	}
	m_Datas.clear();

	UpdatePlot();
}

/*!
 * @brief WM_MOUSEMOVE消息句柄函数
 *
 *        此函数根据当前数据位置，计算所处的角度、距离和数值并显示。
 */
void CPiePlot::OnMouseMove(UINT nFlags, CPoint point)
{
	Gdiplus::Graphics graphics(m_dcGrid.m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gdiplus::SolidBrush brush(m_BackColor);
	graphics.FillRectangle(&brush,m_ClientRect.X,m_ClientRect.Y,100.0f,70.0f);

	if(point.x < m_PieRect.X ||
		point.x > m_PieRect.GetRight() ||
		point.y < m_PieRect.Y ||
		point.y > m_PieRect.GetBottom())
	{
		Invalidate();
		return;
	}
		
	float range,angle,rad;
	Gdiplus::PointF pt;
	pt.X = point.x;
	pt.Y = point.y;
	Point2Range(range,angle,rad,pt);

	Gdiplus::FontFamily fontFamily(_T("Arial"));
	Gdiplus::Font font(&fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	brush.SetColor(m_ToolTipColor);
	CString strTemp;

	if(m_PlotMode == 0)
	{
		pt.X = m_ClientRect.X + 5.0;
		pt.Y = m_ClientRect.Y + 5.0;
		strTemp.Format(_T("方位角:%.2f"),angle);
		graphics.DrawString(strTemp,-1,&font,pt,&brush);

		pt.X = m_ClientRect.X + 5.0;
		pt.Y = m_ClientRect.Y + 21.0;
		strTemp.Format(_T("俯仰角:%.2f"),m_ConstAngle);
		graphics.DrawString(strTemp,-1,&font,pt,&brush);

		pt.X = m_ClientRect.X + 5.0;
		pt.Y = m_ClientRect.Y + 37.0;
		strTemp.Format(_T("高度:%.2f"),range * sin(angle2rad(m_ConstAngle)));
		graphics.DrawString(strTemp,-1,&font,pt,&brush);
	}
	else
	{
		pt.X = m_ClientRect.X + 5.0;
		pt.Y = m_ClientRect.Y + 5.0;
		strTemp.Format(_T("方位角:%.2f"),m_ConstAngle);
		graphics.DrawString(strTemp,-1,&font,pt,&brush);

		pt.X = m_ClientRect.X + 5.0;
		pt.Y = m_ClientRect.Y + 21.0;
		strTemp.Format(_T("俯仰角:%.2f"),angle);
		graphics.DrawString(strTemp,-1,&font,pt,&brush);

		pt.X = m_ClientRect.X + 5.0;
		pt.Y = m_ClientRect.Y + 37.0;
		strTemp.Format(_T("高度:%.2f"),range * sin(angle2rad(angle)));
		graphics.DrawString(strTemp,-1,&font,pt,&brush);
	}
	
	pt.X = m_ClientRect.X + 5.0;
	pt.Y = m_ClientRect.Y + 53.0;
	strTemp.Format(_T("强度:%.0f"),786.0);
	graphics.DrawString(strTemp,-1,&font,pt,&brush);

	Invalidate();
}
