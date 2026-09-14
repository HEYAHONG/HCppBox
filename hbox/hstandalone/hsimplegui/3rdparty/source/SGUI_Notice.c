/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_Notice.c                                             **/
/** Author: XuYulin                                                     **/
/** Description: Notice box display interface.                          **/
/*************************************************************************/

//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Notice.h"

//=======================================================================//
//= Macro definition.                                                   =//
//=======================================================================//
#define SGUI_NOTICE_MARGIN                              (5)

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:  SGUI_Notice_RefreshNotice                           **/
/** Purpose:        Show a notice box.                                  **/
/** Params:                                                             **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:      SimpleGUI object pointer.                   **/
/** @ pstObject[in]:    Object structure pointer.                       **/
/** @ pstFontRes[in]:   Text font resource.                             **/
/** @ uiTextOffset[in]: Text top offset.                                **/
/** Return:         Remaining text height display.                      **/
/** Notice:         None.                                               **/
/*************************************************************************/
SGUI_SIZE SGUI_Notice_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_NOTICT_BOX* pstObject, const SGUI_FONT_RES* pstFontRes, SGUI_INT uiTextOffset)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_SIZE               uiTextLines;
    SGUI_INT                iIconPosX, iIconPosY;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pstObject) && (NULL != pstObject->cszNoticeText))
    {
        SGUI_Basic_ResetMask(pstDeviceIF);
        // Draw edge
        SGUI_Basic_DrawRectangle3(pstDeviceIF, &(pstObject->stLayout), SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);

        SGUI_INT iNoticeTextY = pstObject->stLayout.iY+2;
        SGUI_INT iNiticeTextHeight = pstObject->stLayout.iHeight-4;
        SGUI_INT iNoticeTextX;
        SGUI_INT iNoticeTextWidth;

        if(NULL == pstObject->pstIcon)
        {
            iNoticeTextX = pstObject->stLayout.iX+2;
            iNoticeTextWidth = pstObject->stLayout.iWidth-4;
        }
        else
        {
            iNoticeTextX = pstObject->stLayout.iX+pstObject->pstIcon->iWidth+4;
            iNoticeTextWidth = pstObject->stLayout.iWidth-pstObject->pstIcon->iWidth-6;

            iIconPosX = pstObject->stLayout.iX+2;
            iIconPosY = pstObject->stLayout.iY+2;
            // Paint icon.
            SGUI_Basic_SetMask2(pstDeviceIF, iIconPosX, iIconPosY, pstObject->pstIcon->iWidth, pstObject->pstIcon->iHeight);
            SGUI_Basic_DrawBitMap(pstDeviceIF, iIconPosX, iIconPosY, pstObject->pstIcon, SGUI_DRAW_NORMAL);
        }
        // Draw text
        SGUI_Basic_SetMask2(pstDeviceIF, iNoticeTextX, iNoticeTextY, iNoticeTextWidth, iNiticeTextHeight);
        uiTextLines = SGUI_Text_DrawMultipleLinesText(pstDeviceIF, pstObject->cszNoticeText, pstFontRes, iNoticeTextX, iNoticeTextY + uiTextOffset, iNoticeTextWidth, SGUI_DRAW_NORMAL);
    }
    return uiTextLines;
}

void SGUI_Notice_FitArea(SGUI_SCR_DEV* pstDeviceIF, SGUI_RECT* pstFitArea)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pstDeviceIF) && (NULL != pstFitArea))
    {
        pstFitArea->iX = SGUI_NOTICE_MARGIN;
        pstFitArea->iY = SGUI_NOTICE_MARGIN;
        pstFitArea->iWidth = pstDeviceIF->stSize.iWidth-(SGUI_NOTICE_MARGIN<<1);
        pstFitArea->iHeight = pstDeviceIF->stSize.iHeight-(SGUI_NOTICE_MARGIN<<1);
    }
}
