/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_ProcessBar.c                                         **/
/** Author: XuYulin                                                     **/
/** Description: Process bar display interface.                         **/
/*************************************************************************/

//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_ProcessBar.h"

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:  SGUI_RefreshProcessBar                              **/
/** Purpose:        Display or update a process bar.                    **/
/** Resources:      Process bar data structure.                         **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:  SimpleGUI object pointer.                       **/
/** @ pstObj[in]:   Process bar object.                                 **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_ProcessBar_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_PROCBAR_STRUCT *pstObj)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_UINT16                 uiProcessBlockStartX, uiProcessBlockStartY;
    SGUI_UINT16                 uiProcessBlockWidth, uiProcessBlockHeight;
    SGUI_COLOR                  eBackColor, eFillColor;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstObj)
    {
        if((pstObj->stParameter.stLayout.iWidth > 3) && (pstObj->stParameter.stLayout.iHeight > 3))
        {
            // Check value must be less then PROCBAR_VALUE_MAX.
            if(pstObj->stData.iValue > pstObj->stParameter.iMaxValue)
            {
                pstObj->stData.iValue = pstObj->stParameter.iMaxValue;
            }
            // Update process bar data.
            switch(pstObj->stParameter.eDirection)
            {
                case SGUI_PROCBAR_DOWN: // Process from up to down.
                {
                    uiProcessBlockStartX    = pstObj->stParameter.stLayout.iX + 1;
                    uiProcessBlockStartY    = pstObj->stParameter.stLayout.iY + 1;
                    uiProcessBlockWidth     = pstObj->stParameter.stLayout.iWidth - 2;
                    uiProcessBlockHeight    = (pstObj->stParameter.stLayout.iHeight - 2) * (pstObj->stData.iValue) / pstObj->stParameter.iMaxValue;
                    eBackColor = SGUI_COLOR_BKGCLR;
                    eFillColor = SGUI_COLOR_FRGCLR;
                    break;
                }
                case SGUI_PROCBAR_LEFT: // Process from right to left.
                {
                    uiProcessBlockStartX    = pstObj->stParameter.stLayout.iX + 1;
                    uiProcessBlockStartY    = pstObj->stParameter.stLayout.iY + 1;
                    uiProcessBlockWidth     = (pstObj->stParameter.stLayout.iWidth - 2) * (pstObj->stParameter.iMaxValue - pstObj->stData.iValue) / pstObj->stParameter.iMaxValue;
                    uiProcessBlockHeight    = pstObj->stParameter.stLayout.iHeight - 2;
                    eBackColor = SGUI_COLOR_FRGCLR;
                    eFillColor = SGUI_COLOR_BKGCLR;
                    break;
                }
                case SGUI_PROCBAR_RIGHT:    // Process from left to right.
                {
                    uiProcessBlockStartX    = pstObj->stParameter.stLayout.iX + 1;
                    uiProcessBlockStartY    = pstObj->stParameter.stLayout.iY + 1;
                    uiProcessBlockWidth     = (pstObj->stParameter.stLayout.iWidth - 2) * (pstObj->stData.iValue) / pstObj->stParameter.iMaxValue;
                    uiProcessBlockHeight    = pstObj->stParameter.stLayout.iHeight - 2;
                    eBackColor = SGUI_COLOR_BKGCLR;
                    eFillColor = SGUI_COLOR_FRGCLR;
                    break;
                }
                case SGUI_PROCBAR_UP:   // Process from down to up.
                default:
                {
                    uiProcessBlockStartX    = pstObj->stParameter.stLayout.iX + 1;
                    uiProcessBlockStartY    = pstObj->stParameter.stLayout.iY + 1;
                    uiProcessBlockWidth     = pstObj->stParameter.stLayout.iWidth - 2;
                    uiProcessBlockHeight    = (pstObj->stParameter.stLayout.iHeight - 2) * (pstObj->stParameter.iMaxValue - pstObj->stData.iValue) / pstObj->stParameter.iMaxValue;
                    eBackColor = SGUI_COLOR_FRGCLR;
                    eFillColor = SGUI_COLOR_BKGCLR;
                }
            }
            // Redraw edge and clean up area.
            SGUI_Basic_DrawRectangle3(pstDeviceIF, &(pstObj->stParameter.stLayout), SGUI_COLOR_FRGCLR, eBackColor);
            // Draw process block.
            SGUI_Basic_DrawRectangle1(pstDeviceIF, uiProcessBlockStartX, uiProcessBlockStartY, uiProcessBlockWidth, uiProcessBlockHeight, eFillColor, eFillColor);
        }
    }
}
