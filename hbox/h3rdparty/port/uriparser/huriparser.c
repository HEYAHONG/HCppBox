/***************************************************************
 * Name:      huriparser.c
 * Purpose:   实现huriparser接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-10-20
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "huriparser.h"
#include H3RDPARTY_URIPARSER_URI_HEADER

bool huriparser_string_is_empty(huriparser_string_t uriparser_string)
{
    bool ret=false;
    if(uriparser_string.str==NULL || uriparser_string.str_len == 0)
    {
        ret=true;
    }
    return ret;
}

static huriparser_string_t huriparser_string_empty(void)
{
    huriparser_string_t ret;
    ret.str="";
    ret.str_len=0;
    return ret;
}

static huriparser_string_t huriparser_string_from_uritextrange(UriTextRangeA text_range)
{
    huriparser_string_t ret=huriparser_string_empty();
    ret.str=text_range.first;
    ret.str_len=0;
    if(text_range.first!=NULL && text_range.first != text_range.afterLast)
    {
        ret.str_len=(text_range.afterLast-text_range.first);
    }
    return ret;
}

huriparser_string_t huriparser_parse_scheme(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    UriUriA uri;
    const char * errorPos;
    if(uriParseSingleUriA(&uri, uri_cstring, &errorPos) == URI_SUCCESS)
    {
        ret=huriparser_string_from_uritextrange(uri.scheme);
        uriFreeUriMembersA(&uri);
    }

    return ret;
}

huriparser_string_t huriparser_parse_userinfo(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    UriUriA uri;
    const char * errorPos;
    if(uriParseSingleUriA(&uri, uri_cstring, &errorPos) == URI_SUCCESS)
    {
        ret=huriparser_string_from_uritextrange(uri.userInfo);
        uriFreeUriMembersA(&uri);
    }

    return ret;
}

huriparser_string_t huriparser_parse_host(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    UriUriA uri;
    const char * errorPos;
    if(uriParseSingleUriA(&uri, uri_cstring, &errorPos) == URI_SUCCESS)
    {
        ret=huriparser_string_from_uritextrange(uri.hostText);
        uriFreeUriMembersA(&uri);
    }

    return ret;
}

huriparser_string_t huriparser_parse_port(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    UriUriA uri;
    const char * errorPos;
    if(uriParseSingleUriA(&uri, uri_cstring, &errorPos) == URI_SUCCESS)
    {
        ret=huriparser_string_from_uritextrange(uri.portText);
        uriFreeUriMembersA(&uri);
    }

    return ret;
}

huriparser_string_t huriparser_parse_query(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    UriUriA uri;
    const char * errorPos;
    if(uriParseSingleUriA(&uri, uri_cstring, &errorPos) == URI_SUCCESS)
    {
        ret=huriparser_string_from_uritextrange(uri.query);
        uriFreeUriMembersA(&uri);
    }

    return ret;
}

huriparser_string_t huriparser_parse_fragment(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    UriUriA uri;
    const char * errorPos;
    if(uriParseSingleUriA(&uri, uri_cstring, &errorPos) == URI_SUCCESS)
    {
        ret=huriparser_string_from_uritextrange(uri.fragment);
        uriFreeUriMembersA(&uri);
    }

    return ret;
}

huriparser_string_t huriparser_parse_http_target(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    const char * afterLast=uri_cstring+strlen(uri_cstring);
    {
        huriparser_string_t fragment=huriparser_parse_fragment(uri_cstring);
        if(!huriparser_string_is_empty(fragment))
        {
            afterLast=fragment.str;
            afterLast--;
        }
    }

    const char *first=uri_cstring;
    {
        huriparser_string_t host=huriparser_parse_host(uri_cstring);
        if(!huriparser_string_is_empty(host))
        {
            first=&host.str[host.str_len];
        }
    }
    {
        huriparser_string_t port=huriparser_parse_port(uri_cstring);
        if(!huriparser_string_is_empty(port))
        {
            first=&port.str[port.str_len];
        }
    }
    UriTextRangeA text_range;
    text_range.first=first;
    text_range.afterLast=afterLast;
    ret=huriparser_string_from_uritextrange(text_range);
    return ret;
}

huriparser_string_t huriparser_parse_http_host(const char *uri_cstring)
{
    huriparser_string_t ret=huriparser_string_empty();
    if(uri_cstring==NULL)
    {
        return ret;
    }
    const char * afterLast=NULL;
    const char *first=NULL;
    {
        huriparser_string_t host=huriparser_parse_host(uri_cstring);
        if(!huriparser_string_is_empty(host))
        {
            first=&host.str[0];
            afterLast=&host.str[host.str_len];
        }
    }
    {
        huriparser_string_t port=huriparser_parse_port(uri_cstring);
        if(!huriparser_string_is_empty(port))
        {
            afterLast=&port.str[port.str_len];
        }
    }
    if(first!=NULL || afterLast !=NULL)
    {
        UriTextRangeA text_range;
        text_range.first=first;
        text_range.afterLast=afterLast;
        ret=huriparser_string_from_uritextrange(text_range);
    }
    return ret;
}

const char *huriparser_unescape_in_place(char *uri_string)
{
    return uriUnescapeInPlaceA(uri_string);
}

const char *huriparser_escape(const char *uri_in_string,char *uri_out_string)
{
    return uriEscapeA(uri_in_string,uri_out_string,URI_FALSE,URI_FALSE);
}



