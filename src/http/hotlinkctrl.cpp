/*****************************************************************************
*    Open LiteSpeed is an open source HTTP server.                           *
*    Copyright (C) 2013  LiteSpeed Technologies, Inc.                        *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program. If not, see http://www.gnu.org/licenses/.      *
*****************************************************************************/
#include "hotlinkctrl.h"

#include <util/pcregex.h>

#include "/app/vendor/pcre/include/pcreposix.h"
#include <string.h>


HotlinkCtrl::HotlinkCtrl()
    : m_iAllowDirectAcc( false )
    , m_iOnlySelf( true )
    , m_pRegex( NULL )
{
}
HotlinkCtrl::~HotlinkCtrl()
{
    if ( m_pRegex )
        delete m_pRegex;
}

int HotlinkCtrl::allowed( const char * pRef, int len ) const
{
    StringList::const_iterator iter;
    for( iter = m_listHosts.begin(); iter != m_listHosts.end(); ++iter )
    {
        if ( strncasecmp( pRef, (*iter)->c_str(), (*iter)->len() ) == 0 )
            return 1;
    }
    if ( m_pRegex )
    {
        int ovector[30];
        return m_pRegex->exec( pRef, len, 0, 0, ovector, 30 ) > 0;
    }
    return 0;
}

int HotlinkCtrl::setSuffixes( const char * suffix )
{
    if ( !suffix )
        return -1; 
    return m_listSuffix.split( suffix, suffix + strlen( suffix ), " ,|" );
}

int  HotlinkCtrl::setHosts( const char * pHosts )
{
    if ( !pHosts )
        return -1;
    return m_listHosts.split( pHosts, pHosts + strlen( pHosts ), " ,|" );
}

int  HotlinkCtrl::setRegex( const char * pRegex )
{
    if ( !pRegex )
        return -1;
    if ( !m_pRegex )
    {
        m_pRegex = new Pcregex();
        if ( !m_pRegex )
            return -1;
    }
    int ret = m_pRegex->compile( pRegex, REG_EXTENDED | REG_ICASE );
    if ( ret == -1 )
    {
        delete m_pRegex;
        m_pRegex = NULL;
    }
    return ret;

}


