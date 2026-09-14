/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_Text.c                                               **/
/** Author: XuYulin                                                     **/
/** Description: Text display interface                                 **/
/*************************************************************************/

//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Text.h"

//=======================================================================//
//= Public variable declaration.                                        =//
//=======================================================================//
SGUI_CSZSTR     SGUI_EMPTY_STRING = {""};

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:  SGUI_Text_GetTextExtent                             **/
/** Purpose:        Get the area size if show given text completely.    **/
/** Params:                                                             **/
/** @ szText[in]:   Text array pointer.                                 **/
/** @ pstFontRes[in]: Font resource, improve font size info.            **/
/** @ pstTextExtent[out]: Text extent size.                             **/
/** Return:         Next character X coordinate in current line.        **/
/** Limitation:     None.                                               **/
/*************************************************************************/
void SGUI_Text_GetTextExtent(SGUI_CSZSTR cszText, const SGUI_FONT_RES* pstFontRes, SGUI_AREA_SIZE* pstTextExtent)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    const SGUI_CHAR*            pcChar;
    SGUI_UINT32                 uiCharacterCode;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    pcChar =                    (SGUI_CSZSTR)ENCODE(cszText);

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pcChar) && (NULL != pstTextExtent))
    {
        pstTextExtent->iHeight = pstFontRes->iHeight;
        pstTextExtent->iWidth=0;
        while('\0' != *pcChar)
        {
            uiCharacterCode = 0;
            pcChar = pstFontRes->fnStepNext(pcChar, &uiCharacterCode);
            if('\0' !=uiCharacterCode)
            {
                pstTextExtent->iWidth+=(pstFontRes->fnIsFullWidth(uiCharacterCode)?pstFontRes->iFullWidth:pstFontRes->iHalfWidth);
            }
        }
    }
}

/*************************************************************************/
/** Function Name:  SGUI_Text_DrawASCIICharacter                        **/
/** Purpose:        Write a single line text in a fixed area.           **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: SimpleGUI object pointer.                        **/
/** @ cChar[in]:    Character, only allow ASCII characters.             **/
/** @ pstFontRes[in]: Font resource object.                             **/
/** @ iX[in]:       Paint text x-coordinate.                            **/
/** @ iX[in]:       Paint text y-coordinate.                            **/
/** @ eFontMode[in] Character display mode(normal or reverse color).    **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Text_DrawASCIICharacter(SGUI_SCR_DEV* pstDeviceIF, SGUI_CHAR cChar, const SGUI_FONT_RES* pstFontRes, SGUI_INT iX, SGUI_INT iY, SGUI_DRAW_MODE eFontMode)
{
	/*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_BMP_RES                stCharBitmap;

	/*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
	stCharBitmap.iHeight = pstFontRes->iHeight;
	stCharBitmap.iWidth = pstFontRes->iHalfWidth;
	stCharBitmap.pData = pstDeviceIF->stBuffer.pBuffer;

	/*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((cChar < 0x7F))
	{
		SGUI_Text_GetCharacterData(pstFontRes, cChar, pstDeviceIF->stBuffer.pBuffer, pstDeviceIF->stBuffer.sSize);
		SGUI_Basic_DrawBitMap(pstDeviceIF, iX, iY, &stCharBitmap, eFontMode);
	}
}

/*************************************************************************/
/** Function Name:  SGUI_Text_DrawSingleLineText                        **/
/** Purpose:        Write a single line text in a fixed area.           **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:  SimpleGUI object pointer.                       **/
/** @ cszText[in]:  Text array pointer.                                 **/
/** @ pstFontRes[in]: Font resource object.                             **/
/** @ iX[in]:       Paint bitmap x-coordinate.                          **/
/** @ iY[in]:       Paint bitmap y-coordinate.                          **/
/** @ eFontMode[in] Character display mode(normal or reverse color).    **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_Text_DrawText(SGUI_SCR_DEV* pstDeviceIF, SGUI_CSZSTR cszText, const SGUI_FONT_RES* pstFontRes, SGUI_INT iX, SGUI_INT iY, SGUI_DRAW_MODE eFontMode)
{

    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    const SGUI_CHAR*            pcChar = (SGUI_CSZSTR)ENCODE(cszText);// Text character pointer.
    SGUI_UINT32                 uiCharacterCode;                // Character byte, might be tow bytes.
    SGUI_BMP_RES                stCharBitmap;
    SGUI_INT                    iPaintX = iX;
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pcChar)
    {
        // Initialize drawing area data.
        stCharBitmap.iHeight = pstFontRes->iHeight;
        stCharBitmap.pData = pstDeviceIF->stBuffer.pBuffer;

        // Loop for Each char.
        while(((NULL != pcChar) && ('\0' != *pcChar)) && (iPaintX <= pstDeviceIF->stMask.iEndX))
        {
            uiCharacterCode = 0;
            pcChar = pstFontRes->fnStepNext(pcChar, &uiCharacterCode);
            //if(SGUI_IS_VISIBLE_CHAR(uiCharacterCode))
            {
                SGUI_RECT_WIDTH(stCharBitmap) = pstFontRes->fnIsFullWidth(uiCharacterCode)?pstFontRes->iFullWidth:pstFontRes->iHalfWidth;
                if((iPaintX+stCharBitmap.iWidth-1) >= 0)
                {
                    SGUI_Text_GetCharacterData(pstFontRes, uiCharacterCode, pstDeviceIF->stBuffer.pBuffer, pstDeviceIF->stBuffer.sSize);
                    SGUI_Basic_DrawBitMap(pstDeviceIF, iPaintX, iY, &stCharBitmap, eFontMode);
                }
                iPaintX += SGUI_RECT_WIDTH(stCharBitmap);
            }
        }
    }
}

/*************************************************************************/
/** Function Name:  GUI_DrawMultipleLinesText                           **/
/** Purpose:        Write a mulitiplt line text in a rectangular area.  **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]:  SimpleGUI object pointer.                       **/
/** @ cszText[in]:  Text array pointer.                                 **/
/** @ pstFontRes[in]: Font resource object.                             **/
/** @ iX[in]:       Paint bitmap x-coordinate.                          **/
/** @ iY[in]:       Paint bitmap y-coordinate.                          **/
/** @ iWidth[in]:   Text line max width.                                **/
/** @ eFontMode[in]: Character display mode(normal or reverse color).   **/
/** Return:         Used line count.                                    **/
/*************************************************************************/
SGUI_SIZE SGUI_Text_DrawMultipleLinesText(SGUI_SCR_DEV* pstDeviceIF, SGUI_CSZSTR cszText, const SGUI_FONT_RES* pstFontRes, SGUI_INT iX, SGUI_INT iY, SGUI_INT iWidth, SGUI_DRAW_MODE eFontMode)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    const SGUI_CHAR*            pcChar = (SGUI_CSZSTR)ENCODE(cszText);
    SGUI_UINT32                 uiCharacterCode;
    SGUI_SIZE                   uiLines = 0;
    SGUI_BMP_RES                stCharBitmap;
    SGUI_INT                    iPaintX = iX;
    SGUI_INT                    iPaintY = iY;
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(cszText != NULL)
    {
        // Initialize line number.
        uiLines = 1;
        // Initialize character bitmap structure.
        stCharBitmap.iHeight = pstFontRes->iHeight;
        stCharBitmap.pData = pstDeviceIF->stBuffer.pBuffer;
        // Loop for each word in display area.
        while(((NULL != pcChar) && ('\0' != *pcChar)))
        {
            uiCharacterCode = 0;
            pcChar = pstFontRes->fnStepNext(pcChar, &uiCharacterCode);

            // Judge change line symbol.
            if(uiCharacterCode == '\n')
            {
                // Change lines.
                iPaintX = iX;
                iPaintY += pstFontRes->iHeight;
                uiLines ++;
                continue;
            }
            // Get character width;
            stCharBitmap.iWidth = pstFontRes->fnIsFullWidth(uiCharacterCode)?pstFontRes->iFullWidth:pstFontRes->iHalfWidth;

            // Judge change line
            if((iPaintX+stCharBitmap.iWidth-1) >= (pstDeviceIF->stMask.iStartX + iWidth))
            {
                 // Change lines.
                iPaintX = iX;
                iPaintY += pstFontRes->iHeight;
                uiLines ++;
            }
            // Draw characters.
            if(((iPaintX+stCharBitmap.iWidth-1) >= pstDeviceIF->stMask.iStartX) && (iPaintX <= pstDeviceIF->stMask.iEndX)
               && ((iPaintY+stCharBitmap.iHeight-1) >= pstDeviceIF->stMask.iStartY) && (iPaintY <= pstDeviceIF->stMask.iEndY))
            {
                // Draw character.
                SGUI_Text_GetCharacterData(pstFontRes, uiCharacterCode, pstDeviceIF->stBuffer.pBuffer, pstDeviceIF->stBuffer.sSize);
                SGUI_Basic_DrawBitMap(pstDeviceIF, iPaintX, iPaintY, &stCharBitmap, eFontMode);
            }
            else
            {
                // character is not in visible area, ignore.
            }
            iPaintX += SGUI_RECT_WIDTH(stCharBitmap);
        }
    }
    return uiLines;
}

/*************************************************************************/
/** Function Name:  SGUI_Text_GetMultiLineTextLines                     **/
/** Purpose:        Get a string's lines in a fixed width area.         **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ cszText[in]:  Text array pointer.                                 **/
/** @ pstFontRes[in]: Font resource object.                             **/
/** @ uiDisplayAreaWidth[in]: Display area width.                       **/
/** Return:         String lines.                                       **/
/*************************************************************************/
SGUI_SIZE SGUI_Text_GetMultiLineTextLines(SGUI_CSZSTR cszText, const SGUI_FONT_RES* pstFontRes, SGUI_SIZE uiDisplayAreaWidth)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_SIZE                   uiLineNumber, uiLineLength;
    SGUI_UINT32                 uiCharacterCode;
    SGUI_UINT16                 uiCharWidth;
    SGUI_CSZSTR                 pcChar;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    uiLineLength =              0;
    uiLineNumber =              1;
    pcChar =                    (SGUI_CSZSTR)ENCODE(cszText);

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    while('\0' != *pcChar)
    {
        uiCharacterCode = 0;
        pcChar = pstFontRes->fnStepNext(pcChar, &uiCharacterCode);
        if('\n' == uiCharacterCode)
        {
            uiLineNumber++;
            uiLineLength = 0;
        }
        else
        {
            uiCharWidth = pstFontRes->fnIsFullWidth(uiCharacterCode)?pstFontRes->iFullWidth:pstFontRes->iHalfWidth;
            if((uiLineLength+uiCharWidth)>uiDisplayAreaWidth)
            {
                uiLineNumber++;
                uiLineLength = uiCharWidth;
            }
            else
            {
                uiLineLength+=uiCharWidth;
            }
        }
    }
    return uiLineNumber;
}

/*****************************************************************************/
/** Function Name:  SGUI_Text_GetCharacterData                              **/
/** Purpose:        Get character data form font resource by char code.     **/
/** Params:                                                                 **/
/** @ pstFontRes[in]:   Font resource structure pointer.                    **/
/** @ uiCode[in]:       Character code.                                     **/
/** @ pDataBuffer[out]: Buffer for output char data.                        **/
/** @ sBufferSize[in]:  Output buffer size.                                 **/
/** Return:             Number of read data, return 0 when error occurred.  **/
/*****************************************************************************/
SGUI_SIZE SGUI_Text_GetCharacterData(const SGUI_FONT_RES* pstFontRes, SGUI_UINT32 uiCode, SGUI_BYTE* pDataBuffer, SGUI_SIZE sBufferSize)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_SIZE                   sGetDataSize;
    SGUI_SIZE                   sReadDataSize;
    SGUI_SIZE                   sDataBlockSize;
    SGUI_INT                    iCharIndex;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    sGetDataSize =              0;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pstFontRes) && (NULL != pDataBuffer) && (0 != sBufferSize))
    {
        iCharIndex = pstFontRes->fnGetIndex(uiCode);
        if(SGUI_INVALID_INDEX != iCharIndex)
        {
            sDataBlockSize = SGUI_USED_BYTE(pstFontRes->iHeight)*(pstFontRes->iHalfWidth);
            sReadDataSize = pstFontRes->fnIsFullWidth(uiCode)?(sDataBlockSize*2):sDataBlockSize;
            sGetDataSize = pstFontRes->fnGetData(iCharIndex*sDataBlockSize, pDataBuffer, sReadDataSize>sBufferSize?sBufferSize:sReadDataSize);
        }
    }

    return sGetDataSize;
}
