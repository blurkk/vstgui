//-----------------------------------------------------------------------------
// VST Plug-Ins SDK
// VSTGUI: Graphical User Interface Framework for VST plugins
//
// Version 4.3
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// (c) 2015, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __gdiplusdrawcontext__
#define __gdiplusdrawcontext__

#include "../../coffscreencontext.h"

#if WINDOWS

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

namespace VSTGUI {
class GdiplusBitmap;

//-----------------------------------------------------------------------------
class GdiplusDrawContext : public COffscreenContext
{
public:
	GdiplusDrawContext (HWND window, const CRect& drawSurface);
	GdiplusDrawContext (GdiplusBitmap* bitmap);
	~GdiplusDrawContext ();

	Gdiplus::Graphics* getGraphics () const { return pGraphics; }
	Gdiplus::Pen* getPen () const { return pPen; }
	Gdiplus::SolidBrush* getBrush () const { return pBrush; }
	Gdiplus::SolidBrush* getFontBrush () const { return pFontBrush; }

	// CDrawContext
	void drawLine (const LinePair& line) override;
	void drawLines (const LineList& lines) override;
	void drawPolygon (const PointList& polygonPointList, const CDrawStyle drawStyle = kDrawStroked) override;
	void drawRect (const CRect &rect, const CDrawStyle drawStyle = kDrawStroked) override;
	void drawArc (const CRect &rect, const float startAngle1, const float endAngle2, const CDrawStyle drawStyle = kDrawStroked) override;
	void drawEllipse (const CRect &rect, const CDrawStyle drawStyle = kDrawStroked) override;
	void drawPoint (const CPoint &point, const CColor& color) override;
	void drawBitmap (CBitmap* bitmap, const CRect& dest, const CPoint& offset = CPoint (0, 0), float alpha = 1.f) override;
	void clearRect (const CRect& rect) override;
	void setLineStyle (const CLineStyle& style) override;
	void setLineWidth (CCoord width) override;
	void setDrawMode (CDrawMode mode) override;
	void setClipRect (const CRect &clip) override;
	void resetClipRect () override;
	void setFillColor  (const CColor& color) override;
	void setFrameColor (const CColor& color) override;
	void setFontColor (const CColor& color) override;
	void setGlobalAlpha (float newAlpha) override;
	void saveGlobalState () override;
	void restoreGlobalState () override;
	CGraphicsPath* createGraphicsPath () override;
	CGraphicsPath* createTextPath (const CFontRef font, UTF8StringPtr text) override;
	void drawGraphicsPath (CGraphicsPath* path, PathDrawMode mode = kPathFilled, CGraphicsTransform* transformation = 0) override;
	void fillLinearGradient (CGraphicsPath* path, const CGradient& gradient, const CPoint& startPoint, const CPoint& endPoint, bool evenOdd = false, CGraphicsTransform* transformation = 0) override;
	void fillRadialGradient (CGraphicsPath* path, const CGradient& gradient, const CPoint& center, CCoord radius, const CPoint& originOffset = CPoint (0, 0), bool evenOdd = false, CGraphicsTransform* transformation = 0) override;

	void endDraw () override;
//-----------------------------------------------------------------------------
protected:
	void init () override;

	void setFillColorInternal (const CColor& color);
	void setFrameColorInternal (const CColor& color);
	void setFontColorInternal (const CColor& color);
	void setLineStyleInternal (const CLineStyle& style);
	void setLineWidthInternal (CCoord width);
	void setDrawModeInternal (CDrawMode mode);

	HWND window;
	Gdiplus::Graphics	*pGraphics;
	Gdiplus::Pen		*pPen;
	Gdiplus::SolidBrush	*pBrush;
	Gdiplus::SolidBrush	*pFontBrush;
};

//-----------------------------------------------------------------------------
struct GdiplusDrawScope
{
	GdiplusDrawScope (Gdiplus::Graphics* graphics, const CRect& clipRect, const CGraphicsTransform& transform)
	: graphics (graphics)
	{
		graphics->SetClip (Gdiplus::RectF ((Gdiplus::REAL)clipRect.left, (Gdiplus::REAL)clipRect.top, (Gdiplus::REAL)clipRect.getWidth (), (Gdiplus::REAL)clipRect.getHeight ()), Gdiplus::CombineModeReplace);
		if (transform.isInvariant () == false)
		{
			Gdiplus::Matrix matrix ((Gdiplus::REAL)transform.m11, (Gdiplus::REAL)transform.m12, (Gdiplus::REAL)transform.m21, (Gdiplus::REAL)transform.m22, (Gdiplus::REAL)transform.dx, (Gdiplus::REAL)transform.dy);
			graphics->SetTransform (&matrix);
		}
	}

	~GdiplusDrawScope ()
	{
		Gdiplus::Matrix matrix;
		graphics->SetTransform (&matrix);
	}

	Gdiplus::Graphics* graphics;
};

//-----------------------------------------------------------------------------
inline void convert (Gdiplus::Matrix& matrix, const CGraphicsTransform& t)
{
	matrix.SetElements ((Gdiplus::REAL)t.m11, (Gdiplus::REAL)t.m21, (Gdiplus::REAL)t.m12, (Gdiplus::REAL)t.m22, (Gdiplus::REAL)t.dx, (Gdiplus::REAL)t.dy);
}


} // namespace

#endif // WINDOWS

#endif // __gdiplusdrawcontext__
