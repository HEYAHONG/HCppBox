/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_Basic.c                                              **/
/** Author: XuYulin                                                     **/
/** Description: Simple GUI basic drawing operating interface.          **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Basic.h"

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawPoint                                **/
/** Purpose:        Set a pixel color or draw a point.                  **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iPosX[in]:    X coordinate of point by pixels.                    **/
/** @ iPosY[in]:    Y coordinate of point by pixels.                    **/
/** @ eColor[in]:       Point color, GUI_COLOR_BKGCLR means clear pix,  **/
/**                     GUI_COLOR_FRGCLR means set pix.                 **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawPoint(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iPosX, SGUI_INT iPosY, SGUI_COLOR eColor)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
	if(NULL == pstDeviceIF->fnSetPixel)
	{
		/* Action function is unspecified, no actions. */
	}
	else
    {
        if((iPosX >= pstDeviceIF->stMask.iStartX) && (iPosX <= pstDeviceIF->stMask.iEndX) && (iPosY >= pstDeviceIF->stMask.iStartY) && (iPosY <= pstDeviceIF->stMask.iEndY))
        {
            pstDeviceIF->fnSetPixel(iPosX, iPosY, eColor);
        }
    }
}

#ifdef SGUI_GET_POINT_FUNC_EN
/*************************************************************************/
/** Function Name:  SGUI_Basic_GetPoint                                 **/
/** Purpose:        Get a pixel color .                                 **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: SimpleGUI object pointer.                        **/
/** @ iPosX[in]:    X coordinate of point by pixels.                    **/
/** @ iPosY[in]:    Y coordinate of point by pixels.                    **/
/** Return:         SGUI_COLOR type enumerated for point color.         **/
/** Notice:         None.                                               **/
/*************************************************************************/
SGUI_COLOR SGUI_Basic_GetPoint(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iPosX, SGUI_INT iPosY)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_COLOR                  eColor;
    SGUI_UINT                   uiPixValue;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eColor =                    SGUI_COLOR_BKGCLR;
    uiPixValue =                0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pstDeviceIF) && (iPosX < SGUI_RECT_WIDTH(pstDeviceIF->stSize)) && (iPosY < SGUI_RECT_HEIGHT(pstDeviceIF->stSize)))
    {
        if(NULL == pstDeviceIF->fnSetPixel)
        {
            /* Action function is unspecified, no actions. */
        }
        else
        {
            uiPixValue = pstDeviceIF->fnGetPixel(iPosX, iPosY);
            if(0 == uiPixValue)
            {
                eColor = SGUI_COLOR_BKGCLR;
            }
            else
            {
                eColor = SGUI_COLOR_FRGCLR;
            }
        }
    }

    return eColor;
}
#endif // SGUI_GET_POINT_FUNC_EN

/*************************************************************************/
/** Function Name:  SGUI_Basic_ResetMask                                **/
/** Purpose:        Reset paint mask to full screen.                    **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_ResetMask(SGUI_SCR_DEV* pstDeviceIF)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstDeviceIF->stMask.iStartX = 0;
    pstDeviceIF->stMask.iStartY = 0;
    pstDeviceIF->stMask.iEndX = pstDeviceIF->stSize.iWidth - 1;
    pstDeviceIF->stMask.iEndY = pstDeviceIF->stSize.iHeight - 1;
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_SetMask1                                 **/
/** Purpose:        Set paint mask area.                                **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iStartX[in]:      X coordinate of start point of rectangle.       **/
/** @ iStartY[in]:      Y coordinate of start point of rectangle.       **/
/** @ iEndX[in]:        X coordinate of end point of rectangle.         **/
/** @ iEndY[in]:        Y coordinate of end point of rectangle.         **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_SetMask1(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iStartY, SGUI_INT iEndX, SGUI_INT iEndY)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstDeviceIF->stMask.iStartX = (iStartX < 0) ? 0 : iStartX;
    pstDeviceIF->stMask.iStartY = (iStartY < 0) ? 0 : iStartY;
    pstDeviceIF->stMask.iEndX = (iEndX<pstDeviceIF->stSize.iWidth) ? iEndX : (pstDeviceIF->stSize.iWidth - 1);
    pstDeviceIF->stMask.iEndY = (iEndY<pstDeviceIF->stSize.iHeight) ? iEndY : (pstDeviceIF->stSize.iHeight - 1);

}

/*************************************************************************/
/** Function Name:  SGUI_Basic_SetMask2                                 **/
/** Purpose:        Set paint mask area.                                **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iX[in]:       X coordinate of start point of rectangle.           **/
/** @ iY[in]:       Y coordinate of start point of rectangle.           **/
/** @ iWidth[in]:   X coordinate of end point of rectangle.             **/
/** @ iHeight[in]:  Y coordinate of end point of rectangle.             **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_SetMask2(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iX, SGUI_INT iY, SGUI_INT iWidth, SGUI_INT iHeight)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstDeviceIF->stMask.iStartX = (iX < 0) ? 0 : iX;
    pstDeviceIF->stMask.iStartY = (iY < 0) ? 0 : iY;
    SGUI_INT iNewMaskEndX = iX + iWidth - 1;
    SGUI_INT iNewMaskEndY = iY + iHeight - 1;
    pstDeviceIF->stMask.iEndX = (iNewMaskEndX<pstDeviceIF->stSize.iWidth) ? iNewMaskEndX : (pstDeviceIF->stSize.iWidth - 1);
    pstDeviceIF->stMask.iEndY = (iNewMaskEndY<pstDeviceIF->stSize.iHeight) ? iNewMaskEndY : (pstDeviceIF->stSize.iHeight - 1);
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_SetMask3                                 **/
/** Purpose:        Set paint mask area.                                **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ pstMask[in]:  Mask range description.                             **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_SetMask3(SGUI_SCR_DEV* pstDeviceIF, const SGUI_PAINT_MASK* pstMask)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstDeviceIF->stMask.iStartX = (pstMask->iStartX < 0) ? 0 : pstMask->iStartX;
    pstDeviceIF->stMask.iStartY = (pstMask->iStartY < 0) ? 0 : pstMask->iStartY;
    pstDeviceIF->stMask.iEndX = (pstMask->iEndX < pstDeviceIF->stSize.iWidth) ? pstMask->iEndX : (pstDeviceIF->stSize.iWidth - 1);
    pstDeviceIF->stMask.iEndY = (pstMask->iEndY < pstDeviceIF->stSize.iHeight) ? pstMask->iEndY : (pstDeviceIF->stSize.iHeight - 1);
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_SetMask4                                 **/
/** Purpose:        Set paint mask area.                                **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ pstRect[in]:  Rectangle description.                              **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_SetMask4(SGUI_SCR_DEV* pstDeviceIF, const SGUI_RECT* pstRect)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstDeviceIF->stMask.iStartX = (pstRect->iX < 0) ? 0 : pstRect->iX;
    pstDeviceIF->stMask.iStartY = (pstRect->iY < 0) ? 0 : pstRect->iY;
    SGUI_INT iNewMaskEndX = pstRect->iX + pstRect->iWidth - 1;
    SGUI_INT iNewMaskEndY = pstRect->iY + pstRect->iHeight - 1;
    pstDeviceIF->stMask.iEndX = (iNewMaskEndX<pstDeviceIF->stSize.iWidth) ? iNewMaskEndX : (pstDeviceIF->stSize.iWidth - 1);
    pstDeviceIF->stMask.iEndY = (iNewMaskEndY<pstDeviceIF->stSize.iHeight) ? iNewMaskEndY : (pstDeviceIF->stSize.iHeight - 1);
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_ClearScreen                              **/
/** Purpose:        Clean LCD screen display.                           **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_ClearScreen(SGUI_SCR_DEV* pstDeviceIF)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstDeviceIF)
    {
        /* Clear screen. */
        if((NULL != pstDeviceIF->fnClear) && (NULL != pstDeviceIF->fnSyncBuffer))
        {
            pstDeviceIF->fnClear();
        }
        else
        {
            /* Draw a blank rectangle for clean screen when clean function is not supposed. */
            SGUI_Basic_DrawRectangle1(pstDeviceIF, 0, 0, SGUI_RECT_WIDTH(pstDeviceIF->stSize), SGUI_RECT_HEIGHT(pstDeviceIF->stSize), SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawLine                                 **/
/** Purpose:        Draw a line by the Bresenham algorithm.             **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iStartX[in]:      X coordinate of start point of line.            **/
/** @ iStartY[in]:      Y coordinate of start point of line.            **/
/** @ iEndX[in]:        X coordinate of end point of line.              **/
/** @ iEndY[in]:        Y coordinate of end point of line.              **/
/** @ eColor[in]:       Line color.                                     **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawLine(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iStartY, SGUI_INT iEndX, SGUI_INT iEndY, SGUI_COLOR eColor)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                    iDx, iDy;
    SGUI_INT                    iIncX, iIncY;
    SGUI_INT                    iErrX = 0, iErrY = 0;
    SGUI_INT                    i, iDs;
    SGUI_INT                    iCurrentPosX, iCurrentPosY;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    iErrX = 0;
    iErrY = 0;
    iDx = iEndX - iStartX;
    iDy = iEndY - iStartY;
    iCurrentPosX = iStartX;
    iCurrentPosY = iStartY;

    if(iDx > 0)
    {
        iIncX = 1;
    }
    else
    {
        if(iDx == 0)
        {
            iIncX = 0;
        }
        else
        {
            iIncX = -1;
            iDx = -iDx;
        }
    }

    if(iDy > 0)
    {
        iIncY = 1;
    }
    else
    {
        if(iDy == 0)
        {
            iIncY = 0;
        }
        else
        {
            iIncY = -1;
            iDy = -iDy;
        }
    }

    if(iDx > iDy)
    {
        iDs = iDx;
    }
    else
    {
        iDs = iDy;
    }

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    for(i = 0; i <= iDs+1; i++)
    {
        SGUI_Basic_DrawPoint(pstDeviceIF, iCurrentPosX, iCurrentPosY, eColor);
        iErrX += iDx;
        if(iErrX > iDs)
        {
            iErrX -= iDs;
            iCurrentPosX += iIncX;
        }
        iErrY += iDy;
        if(iErrY > iDs)
        {
            iErrY -= iDs;
            iCurrentPosY += iIncY;
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawHorizontalLine                       **/
/** Purpose:        Draw a horizontal line by the Bresenham algorithm.  **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:  SimpleGUI object pointer.                       **/
/** @ iStartX[in]:      X coordinate of start point of line.            **/
/** @ iEndX[in]:        X coordinate of end point of line.              **/
/** @ iY[in]:           Y coordinate of the line.                       **/
/** @ eColor[in]:       Line color.                                     **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawHorizontalLine(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iEndX, SGUI_INT iY, SGUI_COLOR eColor)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iPointX;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((iY >= pstDeviceIF->stMask.iStartY) && (iY <= pstDeviceIF->stMask.iEndY))
    {
        if(iStartX > iEndX)
        {
            SGUI_SWAP(iStartX, iEndX);
        }
        iStartX = (iStartX < pstDeviceIF->stMask.iStartX) ? (pstDeviceIF->stMask.iStartX) : (iStartX);
        iEndX = (iEndX > pstDeviceIF->stMask.iEndX) ? (pstDeviceIF->stMask.iEndX) : (iEndX);
        for(iPointX=iStartX; iPointX<=iEndX; iPointX++)
        {
            pstDeviceIF->fnSetPixel(iPointX, iY, eColor);
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawVerticalLine                         **/
/** Purpose:        Draw a vertical line by the Bresenham algorithm.    **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iX[in]:       X coordinate of the line.                           **/
/** @ iStartY[in]:  Y coordinate of start point of line.                **/
/** @ iEndY[in]:    Y coordinate of end point of line.                  **/
/** @ eColor[in]:   Line color.                                         **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawVerticalLine(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iX, SGUI_INT iStartY, SGUI_INT iEndY, SGUI_COLOR eColor)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iPointY;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((iX >= pstDeviceIF->stMask.iStartX) && (iX <= pstDeviceIF->stMask.iEndX))
    {
        if(iStartY > iEndY)
        {
            SGUI_SWAP(iStartY, iEndY);
        }
        for(iPointY=iStartY; iPointY<=iEndY; iPointY++)
        {
            SGUI_Basic_DrawPoint(pstDeviceIF, iX, iPointY, eColor);
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawRectangle1                           **/
/** Purpose:        Draw a rectangle on screen.                         **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iStartX[in]:      X coordinate of the upper-left corner.          **/
/** @ iStartY[in]:      Y coordinate of the upper-left corner.          **/
/** @ iWidth[in]: . Width of rectangle.                                 **/
/** @ iHeight[in]:      Height of rectangle.                            **/
/** @ eEdgeColor[in]:   Edge color.                                     **/
/** @ eFillColor[in]:   Fill color.                                     **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawRectangle1(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iStartY, SGUI_INT iWidth, SGUI_INT iHeight, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((iWidth > 0) && (iHeight > 0))
    {
        if((iWidth == 1) && (iHeight == 1))
        {
            SGUI_Basic_DrawPoint(pstDeviceIF, iStartX, iStartY, eEdgeColor);
        }
        else if(iWidth == 1)
        {
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, iStartX, iStartY, iStartY+iHeight-1, eEdgeColor);
        }
        else if(iHeight == 1)
        {
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX, iStartX+iWidth-1, iStartY, eEdgeColor);
        }
        else
        {
            // Draw edge.
            // Check and set changed page and column index is in edge display action.
            // Top edge
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX, iStartX+iWidth-1, iStartY, eEdgeColor);
            // Bottom edge
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX, iStartX+iWidth-1, iStartY+iHeight-1, eEdgeColor);
            // Left edge
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, iStartX, iStartY+1, iStartY+iHeight-2, eEdgeColor);
            // Right edge
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, iStartX+iWidth-1, iStartY+1, iStartY+iHeight-2, eEdgeColor);
            // Fill area.
            if((eFillColor != SGUI_COLOR_TRANS) && (iWidth > 2) && (iHeight > 2))
            {
                SGUI_Basic_FillRectangleArea(pstDeviceIF, iStartX+1, iStartY+1, iWidth-2, iHeight-2, eFillColor);
            }
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawRectangle2                           **/
/** Purpose:        Draw a rectangle on screen.                         **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ pstBase[in]:  Connrdinate of rectangle upper-left corner.         **/
/** @ pstSize[in]:  Rectangle size.                                     **/
/** @ eEdgeColor[in]:   Edge color.                                     **/
/** @ eFillColor[in]:   Fill color.                                     **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawRectangle2(SGUI_SCR_DEV* pstDeviceIF, const SGUI_POINT* pstBase, const SGUI_AREA_SIZE* pstSize, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((pstSize->iWidth > 0) && (pstSize->iHeight > 0))
    {
        if((pstSize->iWidth == 1) && (pstSize->iHeight == 1))
        {
            SGUI_Basic_DrawPoint(pstDeviceIF, pstBase->iX, pstBase->iY, eEdgeColor);
        }
        else if(pstSize->iWidth == 1)
        {
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, pstBase->iX, pstBase->iY, pstBase->iY+pstSize->iHeight-1, eEdgeColor);
        }
        else if(pstSize->iHeight == 1)
        {
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, pstBase->iX, pstBase->iX+pstSize->iWidth-1, pstBase->iY, eEdgeColor);
        }
        else
        {
            // Draw edge.
            // Check and set changed page and column index is in edge display action.
            // Top edge
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, pstBase->iX, pstBase->iX+pstSize->iWidth-1, pstBase->iY, eEdgeColor);
            // Bottom edge
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, pstBase->iX, pstBase->iX+pstSize->iWidth-1, pstBase->iY+pstSize->iHeight-1, eEdgeColor);
            // Left edge
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, pstBase->iX, pstBase->iY+1, pstBase->iY+pstSize->iHeight-2, eEdgeColor);
            // Right edge
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, pstBase->iX+pstSize->iWidth-1, pstBase->iY+1, pstBase->iY+pstSize->iHeight-2, eEdgeColor);
            // Fill area.
            if((eFillColor != SGUI_COLOR_TRANS) && (pstSize->iWidth > 2) && (pstSize->iHeight > 2))
            {
                SGUI_Basic_FillRectangleArea(pstDeviceIF, pstBase->iX+1, pstBase->iY+1, pstSize->iWidth-2, pstSize->iHeight-2, eFillColor);
            }
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawRectangle                            **/
/** Purpose:        Draw a rectangle on screen.                         **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ pstRect[in]:  Rectangle description.                              **/
/** @ eEdgeColor[in]:   Edge color.                                     **/
/** @ eFillColor[in]:   Fill color.                                     **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawRectangle3(SGUI_SCR_DEV* pstDeviceIF, const SGUI_RECT* pstRect, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((pstRect->iWidth > 0) && (pstRect->iHeight > 0))
    {
        if((pstRect->iWidth == 1) && (pstRect->iHeight == 1))
        {
            SGUI_Basic_DrawPoint(pstDeviceIF, pstRect->iX, pstRect->iY, eEdgeColor);
        }
        else if(pstRect->iWidth == 1)
        {
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, pstRect->iX, pstRect->iY, pstRect->iY+pstRect->iHeight-1, eEdgeColor);
        }
        else if(pstRect->iHeight == 1)
        {
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, pstRect->iX, pstRect->iX+pstRect->iWidth-1, pstRect->iY, eEdgeColor);
        }
        else
        {
            // Draw edge.
            // Check and set changed page and column index is in edge display action.
            // Top edge
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, pstRect->iX, pstRect->iX+pstRect->iWidth-1, pstRect->iY, eEdgeColor);
            // Bottom edge
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, pstRect->iX, pstRect->iX+pstRect->iWidth-1, pstRect->iY+pstRect->iHeight-1, eEdgeColor);
            // Left edge
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, pstRect->iX, pstRect->iY+1, pstRect->iY+pstRect->iHeight-2, eEdgeColor);
            // Right edge
            SGUI_Basic_DrawVerticalLine(pstDeviceIF, pstRect->iX+pstRect->iWidth-1, pstRect->iY+1, pstRect->iY+pstRect->iHeight-2, eEdgeColor);
            // Fill area.
            if((eFillColor != SGUI_COLOR_TRANS) && (pstRect->iWidth > 2) && (pstRect->iHeight > 2))
            {
                SGUI_Basic_FillRectangleArea(pstDeviceIF, pstRect->iX+1, pstRect->iY+1, pstRect->iWidth-2, pstRect->iHeight-2, eFillColor);
            }
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawCircle                               **/
/** Purpose:        Draw a circle by center coordinate and radius.      **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:  SimpleGUI object pointer.                       **/
/** @ iCx[in]:          Circle center X coordinate.                     **/
/** @ iCy[in]:          Circle center Y coordinate.                     **/
/** @ iRadius[in]:      Circle radius.                                  **/
/** @ eEdgeColor[in]:   Edge color.                                     **/
/** @ eFillColor[in]:   Fill color.                                     **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawCircle(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iCx, SGUI_INT iCy, SGUI_INT iRadius, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iPosXOffset = iRadius;
    SGUI_INT                iPosYOffset = 0;
    SGUI_INT                iXChange = 1 - (iRadius<<1); /* iRadius*2 */
    SGUI_INT                iYChange = 1;
    SGUI_INT                iRadiusError = 0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(iRadius > 0)
	{
		if(1 == iRadius)
		{
			SGUI_Basic_DrawPoint(pstDeviceIF, iCx, iCy, eEdgeColor);
		}
		else
		{
			while(iPosXOffset >= iPosYOffset)
			{
				// Fill the circle
				if(eFillColor != SGUI_COLOR_TRANS)
				{
                    SGUI_Basic_DrawVerticalLine(pstDeviceIF, iCx-iPosXOffset+1, iCy-iPosYOffset, iCy+iPosYOffset, eFillColor);
					SGUI_Basic_DrawVerticalLine(pstDeviceIF, iCx+iPosXOffset-1, iCy-iPosYOffset, iCy+iPosYOffset, eFillColor);
                    SGUI_Basic_DrawVerticalLine(pstDeviceIF, iCx-iPosYOffset, iCy-iPosXOffset, iCy+iPosXOffset, eFillColor);
					SGUI_Basic_DrawVerticalLine(pstDeviceIF, iCx+iPosYOffset, iCy-iPosXOffset, iCy+iPosXOffset, eFillColor);
				}

				// Draw edge.
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx+iPosXOffset, iCy-iPosYOffset, eEdgeColor); /*   0- 45 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx+iPosYOffset, iCy-iPosXOffset, eEdgeColor); /*  45- 90 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx-iPosYOffset, iCy-iPosXOffset, eEdgeColor); /*  90-135 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx-iPosXOffset, iCy-iPosYOffset, eEdgeColor); /* 135-180 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx-iPosXOffset, iCy+iPosYOffset, eEdgeColor); /* 180-225 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx-iPosYOffset, iCy+iPosXOffset, eEdgeColor); /* 225-270 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx+iPosYOffset, iCy+iPosXOffset, eEdgeColor); /* 270-315 */
				SGUI_Basic_DrawPoint(pstDeviceIF, iCx+iPosXOffset, iCy+iPosYOffset, eEdgeColor); /* 315-360 */

				iPosYOffset++;
				iRadiusError += iYChange;
				iYChange += 2;
				if ((2 * iRadiusError + iXChange) > 0)
				{
					iPosXOffset--;
					iRadiusError += iXChange;
					iXChange += 2;
				}
			}
		}
	}
}

#ifdef SGUI_REVERSE_FUNC_EN
/*************************************************************************/
/** Function Name:  SGUI_Basic_ReverseBlockColor                        **/
/** Purpose:        Reverse all pixel color in a rectangle area.        **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:vSimpleGUI object pointer.                        **/
/** @ iStartX[in]:  X coordinate of the upper-left corner.              **/
/** @ iStartY[in]:  Y coordinate of the upper-left corner.              **/
/** @ iWidth[in]:   Width of rectangle.                                 **/
/** @ iHeight[in]:  Height of rectangle.                                **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_ReverseBlockColor(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iStartY, SGUI_INT iWidth, SGUI_INT iHeight)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                    iIdxW, iIdxH;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    for(iIdxW=0; iIdxW<iWidth; iIdxW++)
    {
        for(iIdxH=0; iIdxH<iHeight; iIdxH++)
        {
            if(SGUI_Basic_GetPoint(pstDeviceIF, iStartX+iIdxW, iStartY+iIdxH) == SGUI_COLOR_FRGCLR)
            {
                SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iIdxW, iStartY+iIdxH, SGUI_COLOR_BKGCLR);
            }
            else
            {
                SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iIdxW, iStartY+iIdxH, SGUI_COLOR_FRGCLR);
            }
        }
    }
}
#endif // SGUI_REVERSE_FUNC_EN

/*************************************************************************/
/** Function Name:  SGUI_Basic_FillRectangleArea                        **/
/** Purpose:        Draw a rectangular area bit map on LCD screen.      **/
/** Params:                                                             **/
/** @ iStartX[in]:  X coordinate of the rectangle area upper-left.      **/
/** @ iStartY[in]:  Y coordinate of the rectangle area upper-left.      **/
/** @ iWidth[in]:   Width of rectangle area.                            **/
/** @ iHeight[in]:  Height of rectangle area.                           **/
/** @ eFillColor[in]: Edge color.                                       **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_FillRectangleArea(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iStartY, SGUI_INT iWidth, SGUI_INT iHeight, SGUI_COLOR eFillColor)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iFillPos;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // Fill center.
    if(eFillColor != SGUI_COLOR_TRANS)
    {
        if(iWidth > iHeight)
        {
            /* Filled with horizontal line when width is greater then height. */
            for(iFillPos=iStartY; iFillPos<iStartY+iHeight; iFillPos++)
            {
                SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX, iStartX+iWidth-1, iFillPos, eFillColor);
            }

        }
        else
        {
            /* Filled with vertical line when height greater then width. */
            for(iFillPos=iStartX; iFillPos<iStartX+iWidth; iFillPos++)
            {
                SGUI_Basic_DrawVerticalLine(pstDeviceIF, iFillPos, iStartY, iStartY+iHeight-1, eFillColor);
            }
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawBitMap                               **/
/** Purpose:        Draw a rectangular area bit map on LCD screen.      **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:  SimpleGUI object pointer.                       **/
/** @ iX[in]:       Paint bitmap x-coordinate.                          **/
/** @ iY[in]:       Paint bitmap y-coordinate.                          **/
/** @ pstBitmapData[in]: Bitmap object, include size and data.          **/
/** @ eDrawMode[in]     Bit map display mode(normal or reverse color).  **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_Basic_DrawBitMap(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iX, SGUI_INT iY, const SGUI_BMP_RES* pstBitmapData, SGUI_DRAW_MODE eDrawMode)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                    iPaintStartX, iPaintEndX, iPaintStartY, iPaintEndY;
    SGUI_INT                    iPaintX, iPaintY;
    SGUI_INT                    iBmpPixX, iBmpPixY;
    SGUI_INT                    iDrawnWidthIndex = 0;
    SGUI_INT                    iDrawnHeightIndex = 0;
    SGUI_INT                    iPixIndex;
    const SGUI_BYTE*            pData;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/

    iPaintStartX = iX;
    iPaintStartY = iY;
    iPaintEndX = iX + pstBitmapData->iWidth - 1;
    iPaintEndY = iY + pstBitmapData->iHeight - 1;

    // Only paint in visible area.
    if((iPaintStartX <= pstDeviceIF->stMask.iEndX) && (iPaintEndX >= pstDeviceIF->stMask.iStartX)
       && (iPaintStartY <= pstDeviceIF->stMask.iEndY) && (iPaintEndY >= pstDeviceIF->stMask.iStartY))
    {
        // Set loop start parameter of x coordinate
        iPaintStartX = (iPaintStartX > pstDeviceIF->stMask.iStartX) ? iPaintStartX : pstDeviceIF->stMask.iStartX;
        iPaintStartY = (iPaintStartY > pstDeviceIF->stMask.iStartY) ? iPaintStartY : pstDeviceIF->stMask.iStartY;
        iPaintEndX = (iPaintEndX > pstDeviceIF->stMask.iEndX) ? pstDeviceIF->stMask.iEndX : iPaintEndX;
        iPaintEndY = (iPaintEndY > pstDeviceIF->stMask.iStartY) ? pstDeviceIF->stMask.iEndY : iPaintEndY;
        //
        iBmpPixX = (iPaintStartX - iX);
        iPaintX = iPaintStartX;
        iDrawnWidthIndex = iBmpPixX;
        // Loop for x coordinate;
        while((iDrawnWidthIndex<SGUI_RECT_WIDTH(*pstBitmapData)) && (iPaintX<SGUI_RECT_WIDTH(pstDeviceIF->stSize)))
        {
            // Redirect to data array for column.
            pData = pstBitmapData->pData+iBmpPixX;
            // Set loop start parameter of y coordinate
            iPaintY = iPaintStartY;
            iBmpPixY = (iPaintStartY - iY);
            iDrawnHeightIndex = iBmpPixY;
            iPixIndex = iBmpPixY % 8;
            pData += (iBmpPixY / 8) * SGUI_RECT_WIDTH(*pstBitmapData);
            // Loop for y coordinate;
            while((iDrawnHeightIndex<SGUI_RECT_HEIGHT(*pstBitmapData)) && (iPaintY<SGUI_RECT_HEIGHT(pstDeviceIF->stSize)))
            {
                if(iPixIndex == 8)
                {
                    iPixIndex = 0;
                    pData += SGUI_RECT_WIDTH(*pstBitmapData);
                }
                if(SGUI_GET_PAGE_BIT(*pData, iPixIndex) != eDrawMode)
                {
                    SGUI_Basic_DrawPoint(pstDeviceIF, iPaintX, iPaintY, SGUI_COLOR_FRGCLR);
                }
                else
                {
                    SGUI_Basic_DrawPoint(pstDeviceIF, iPaintX, iPaintY, SGUI_COLOR_BKGCLR);
                }
                iDrawnHeightIndex ++;
                iPixIndex ++;
                iPaintY ++;
                iBmpPixY ++;
            }
            iDrawnWidthIndex ++;
            iPaintX ++;
            iBmpPixX ++;
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_PointIsInArea                            **/
/** Purpose:        Judge point is in the specified rectangular area.   **/
/** Params:                                                             **/
/** @ pstArea[in]:  Specified rectangular area.                         **/
/** @ iPosX[in]:    Point coordinate.                                   **/
/** @ iPosY[in]:    Point coordinate.                                   **/
/** Return:         SGUI_TRUE for the point is in the rectangular area. **/
/**                 SGUI_FALSE for the point is out of range.           **/
/** Notice:         None.                                               **/
/*************************************************************************/
SGUI_BOOL SGUI_Basic_PointIsInArea(const SGUI_RECT* pstArea, SGUI_INT iPosX, SGUI_INT iPosY)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_BOOL                   bReturn;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((iPosX < SGUI_RECT_X_START(*pstArea)) ||
       (iPosX > SGUI_RECT_X_END(*pstArea)) ||
	   (iPosY < SGUI_RECT_Y_START(*pstArea)) ||
       (iPosY > SGUI_RECT_Y_END(*pstArea)))
    {
        bReturn = SGUI_FALSE;
    }
    else
    {
        bReturn = SGUI_TRUE;
    }

    return bReturn;
}

/*************************************************************************/
/** Function Name:  SGUI_Basic_DrawRoundedRectangle                     **/
/** Purpose:        Draw a rounded rectangle.                           **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iStartX[in]:  X coordinate of the upper-left corner.              **/
/** @ iStartY[in]:  Y coordinate of the upper-left corner.              **/
/** @ iWidth[in]:   Width of rectangle.                                 **/
/** @ iHeight[in]:  Height of rectangle.                                **/
/** @ iFillet[in]:  Fillet radius.                                      **/
/** @ eEdgeColor[in]: Edge color.                                       **/
/** @ eFillColor[in]: Fill color.                                       **/
/** Return:         None.                                               **/
/** Notice:         The width and height of the rectangle must greater  **/
/**                 then double fillet radius.                          **/
/*************************************************************************/
void SGUI_Basic_DrawRoundedRectangle(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iStartX, SGUI_INT iStartY, SGUI_INT iWidth, SGUI_INT iHeight, SGUI_INT iFillet, SGUI_COLOR eEdgeColor, SGUI_COLOR eFillColor)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT                iPosXOffset = iFillet;
    SGUI_INT                iPosYOffset = 0;
    SGUI_INT                iXOffset_Old = -1;
    SGUI_INT                iXChange = 1 - (iFillet << 1); /* iFillet*2 */
    SGUI_INT                iYChange = 1;
    SGUI_INT                iRadiusError = 0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/

    if((iWidth > (iFillet<<1)) && (iHeight > (iFillet<<1)))
    {
        // Paint first horizontal line for edge.
        SGUI_Basic_DrawLine(pstDeviceIF, iStartX+iFillet, iStartY, iStartX+iWidth-1-iFillet, iStartY, eEdgeColor);
        // Paint last horizontal line for edge.
        SGUI_Basic_DrawLine(pstDeviceIF, iStartX+iFillet, iStartY+iHeight-1, iStartX+iWidth-1-iFillet, iStartY+iHeight-1, eEdgeColor);
        // Paint Arc
        while(iPosXOffset >= iPosYOffset)
        {
			// Fill
			if(eFillColor != SGUI_COLOR_TRANS)
			{
				if(iPosXOffset != iPosYOffset)
				{
					SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX+iFillet-iPosXOffset+1, iStartX+iWidth-iFillet+iPosXOffset-2, iStartY+iFillet-iPosYOffset, eFillColor);
				}
				if((iPosXOffset != iFillet) && (iXOffset_Old != iPosXOffset))
				{
					SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX+iFillet-iPosYOffset+1, iStartX+iWidth-iFillet+iPosYOffset-2, iStartY+iFillet-iPosXOffset, eFillColor);
				}
			}
			// Draw arc edge for 2nd quadrant(Left top arc).
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iFillet-iPosXOffset, iStartY+iFillet-iPosYOffset, eEdgeColor);
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iFillet-iPosYOffset, iStartY+iFillet-iPosXOffset, eEdgeColor);
			// Draw arc edge for 1st quadrant(Right top arc).
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iWidth-iFillet+iPosXOffset-1, iStartY+iFillet-iPosYOffset, eEdgeColor);
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iWidth-iFillet+iPosYOffset-1, iStartY+iFillet-iPosXOffset, eEdgeColor);
			// Fill
			if(eFillColor != SGUI_COLOR_TRANS)
			{
				if(iPosXOffset != iPosYOffset)
				{
					SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX+iFillet-iPosXOffset+1, iStartX+iWidth-iFillet+iPosXOffset-2, iStartY+iHeight-iFillet+iPosYOffset-1, eFillColor);
				}
				if((iPosXOffset != iFillet) && (iXOffset_Old != iPosXOffset))
				{
					SGUI_Basic_DrawHorizontalLine(pstDeviceIF, iStartX+iFillet-iPosYOffset+1, iStartX+iWidth-iFillet+iPosYOffset-2, iStartY+iHeight-iFillet+iPosXOffset-1, eFillColor);
				}
			}
			// Draw arc edge for 3rd quadrant(Left bottom arc).
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iFillet-iPosXOffset, iStartY+iHeight-iFillet+iPosYOffset-1, eEdgeColor);
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iFillet-iPosYOffset, iStartY+iHeight-iFillet+iPosXOffset-1, eEdgeColor);
			// Draw arc edge for 4th quadrant(Right bottom arc).
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iWidth-iFillet+iPosXOffset-1, iStartY+iHeight-iFillet+iPosYOffset-1, eEdgeColor);
			SGUI_Basic_DrawPoint(pstDeviceIF, iStartX+iWidth-iFillet+iPosYOffset-1, iStartY+iHeight-iFillet+iPosXOffset-1, eEdgeColor);

			iXOffset_Old = iPosXOffset;

            iPosYOffset++;
            iRadiusError += iYChange;
            iYChange += 2;
            if ((2 * iRadiusError + iXChange) > 0)
            {
                iPosXOffset--;
                iRadiusError += iXChange;
                iXChange += 2;
            }
        }

        // Draw left vertical edge
        SGUI_Basic_DrawLine(pstDeviceIF, iStartX, iStartY+iFillet, iStartX, iStartY+iHeight-iFillet-1, eEdgeColor);
        // Draw right vertical edge
        SGUI_Basic_DrawLine(pstDeviceIF, iStartX+iWidth-1, iStartY+iFillet, iStartX+iWidth-1, iStartY+iHeight-iFillet-1, eEdgeColor);
        // Fill center.
        if(eFillColor != SGUI_COLOR_TRANS)
        {
            SGUI_Basic_FillRectangleArea(pstDeviceIF, iStartX+1, iStartY+iFillet+1, iWidth-2, iHeight-(iFillet<<1)-2, eFillColor);
        }
    }
}
