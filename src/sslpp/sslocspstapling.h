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

#include <time.h>

#ifndef SSLOCSPSTAPLING_H
#define SSLOCSPSTAPLING_H
#include <socket/gsockaddr.h>
#include <util/autostr.h>

#include <openssl/x509.h>
#include <openssl/ocsp.h>

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

class HttpFetch;
class ConfigCtx;
class XmlNode;
class SslOcspStapling
{
public:
    SslOcspStapling();
    ~SslOcspStapling();
    int init(SSL_CTX* pSslCtx);
    int update();
    int getCertId(X509* pCert);
    int createRequest();
    int getResponder( X509* pCert );
    int callback(SSL *ssl);
    int processResponse(HttpFetch *pHttpFetch);
    int verifyRespFile( int iNeedVerify = 1 );
    int certVerify(OCSP_RESPONSE *pResponse, OCSP_BASICRESP *pBasicResp, X509_STORE *pXstore);
    void updateRespData(OCSP_RESPONSE *pResponse);
    int getRequestData(unsigned char *pReqData);
    void setCertFile( const char* Certfile );
    int config( const XmlNode *pNode, SSL_CTX *pSSL,  
                        const char *pCAFile, char *pachCert, ConfigCtx* pcurrentCtx );
    
    void setOcspResponder( const char* url )    {   m_sOcspResponder.setStr( url );     }
    void setCAFile( const char* CAfile )        {   m_sCAfile.setStr( CAfile );         }
    void setRespMaxAge( const int iMaxAge )     {   m_iocspRespMaxAge = iMaxAge;        }  
    void setRespfile( const char* Respfile )    {   m_sRespfile.setStr( Respfile );     }    
    
private:
    HttpFetch*      m_pHttpFetch;
    
    uint32_t    m_iDataLen;
    unsigned char*     m_pRespData;
    GSockAddr   m_addrResponder;
    
    AutoStr2    m_sCertfile;
    AutoStr2    m_sCAfile; 
    AutoStr2    m_sOcspResponder; 
    AutoStr2    m_sRespfile; 
    AutoStr2    m_sRespfileTmp; 
    int         m_iocspRespMaxAge;  
    SSL_CTX*    m_pCtx;  
    time_t      m_RespTime;
    OCSP_CERTID* m_pCertId;
    
};
const char* getStaplingErrMsg();
#endif // SSLOCSPSTAPLING_H
