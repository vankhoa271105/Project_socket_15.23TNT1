#include "func.h"
#include "C:\Program Files (x86)\EAGetMail\Include\tlh\EAGetMailObj.tlh"
using namespace EAGetMailObjLib;
#include "C:\Program Files (x86)\EAGetMail\Include\tlh\msxml3.tlh"
const int MailServerPop3 = 0;
const int MailServerImap4 = 1;
const int MailServerEWS = 2;
const int MailServerDAV = 3;
const int MailServerMsGraph = 4;

const int MailServerAuthLogin = 0;
const int MailServerAuthCRAM5 = 1;
const int MailServerAuthNTLM = 2;
const int MailServerAuthXOAUTH2 = 3;

const int GetMailInfos_All = 1;
const int GetMailInfos_NewOnly = 2;
const int GetMailInfos_ReadOnly = 4;
const int GetMailInfos_SeqRange = 8;
const int GetMailInfos_UIDRange = 16;
const int GetMailInfos_PR_ENTRYID = 32;
const int GetMailInfos_DateRange = 64;
const int GetMailInfos_OrderByDateTime = 128;

DWORD  _getCurrentPath(LPTSTR lpPath, DWORD nSize)
{
    DWORD dwSize = ::GetModuleFileName(NULL, lpPath, nSize);
    if (dwSize == 0 || dwSize == nSize)
    {
        return 0;
    }

    // Change file name to current full path
    LPCTSTR psz = _tcsrchr(lpPath, _T('\\'));
    if (psz != NULL)
    {
        lpPath[psz - lpPath] = _T('\0');
        return _tcslen(lpPath);
    }

    return 0;
}

const char* TCHARToChar(const TCHAR* tcharStr) {
#ifdef UNICODE
    // Conversion required: WideChar (TCHAR as wchar_t) to Multibyte (char)
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, NULL, 0, NULL, NULL);
    char* charStr = new char[size_needed];
    WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, charStr, size_needed, NULL, NULL);
    return charStr;
#else
    // No conversion needed (TCHAR is already char)
    return tcharStr;
#endif
}
pair<string, string> tokenize(string s, string del)
{
    vector<string> alo;
    int start, end = -1 * del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        alo.push_back(s.substr(start, end - start));
    } while (end != -1);
    return make_pair(alo[0], alo[1]);
}
bool RetrieveWithXOAUTH2(const char* lpszEmail, const char* lpszAccessToken, const char* password, string &revmail, pair<string, string>& request)
{
    try
    {
        TCHAR szPath[MAX_PATH + 1];
        _getCurrentPath(szPath, MAX_PATH);

        TCHAR szMailBox[MAX_PATH + 1];
        wsprintf(szMailBox, _T("%s\\inbox"), szPath);

        // Create a folder to store emails
        ::CreateDirectory(szMailBox, NULL);


        IMailServerPtr oServer = NULL;
        oServer.CreateInstance(__uuidof(EAGetMailObjLib::MailServer));

        // Gmail IMAP server address
        oServer->Server = _bstr_t("imap.gmail.com");
        oServer->User = _bstr_t(lpszEmail);

        // Use access token as password
        oServer->Password = _bstr_t(lpszAccessToken);
        // Use OAUTH 2.0
        oServer->AuthType = MailServerAuthXOAUTH2;
        // Use IMAP4 Protocol
        oServer->Protocol = MailServerImap4;

        // Enable SSL Connection
        oServer->SSLConnection = VARIANT_TRUE;
        // Set IMAP4 SSL Port
        oServer->Port = 993;

        IMailClientPtr oClient = NULL;
        oClient.CreateInstance(__uuidof(EAGetMailObjLib::MailClient));
        oClient->LicenseCode = _T("TryIt");

        _tprintf(_T("Connecting %s ...\r\n"), (const TCHAR*)oServer->Server);
        oClient->Connect(oServer);

        // Get new email only, if you want to get all emails, please remove this line
        oClient->GetMailInfosParam->GetMailInfosOptions = GetMailInfos_NewOnly;

        IMailInfoCollectionPtr infos = oClient->GetMailInfoList();
        _tprintf(_T("Total %d emails\r\n"), infos->Count);

        for (long i = 0; i < infos->Count; i++)
        {
            IMailInfoPtr pInfo = infos->GetItem(i);

            _tprintf(_T("Index: %d; Size: %d; UIDL: %s\r\n\r\n"),
                pInfo->Index, pInfo->Size, (const TCHAR*)pInfo->UIDL);

            TCHAR szFile[MAX_PATH + 1];
            // Generate a random file name by current local datetime,
            // You can use your method to generate the filename if you do not like it
            SYSTEMTIME curtm;
            ::GetLocalTime(&curtm);
            ::wsprintf(szFile, _T("%s\\%04d%02d%02d%02d%02d%02d%03d%d.eml"),
                szMailBox,
                curtm.wYear,
                curtm.wMonth,
                curtm.wDay,
                curtm.wHour,
                curtm.wMinute,
                curtm.wSecond,
                curtm.wMilliseconds,
                i);

            // Receive email from POP3 server
            IMailPtr oMail = oClient->GetMail(pInfo);
            const TCHAR* frommail = (const TCHAR*)oMail->From->Address;
            _tprintf(_T("From: %s\r\n"), frommail);
            const char* fromM = TCHARToChar(frommail);
            const TCHAR* Subject = (const TCHAR*)oMail->Subject;
            _tprintf(_T("Subject: %s\r\n"), Subject);
            const TCHAR* message = (const TCHAR*)oMail->TextBody;
            const char* zer = TCHARToChar(message);
            cout << zer << endl;
            const char* idx = strstr(zer, password);

            if (idx != NULL) {
                const char* subj = TCHARToChar(Subject);
                request = tokenize(string(subj));
                cout << "Request:" << request.first << " IP: " << request.second << endl;
                revmail = string(fromM);
                cout << "revmail:" << revmail << endl;
                //    // Calculate index by subtracting the base address 
                //    // of txt from result
                return true;
            }

            // Save email to local disk
            oMail->SaveAs(szFile, VARIANT_TRUE);

            // Mark email as read to prevent retrieving this email again.
            oClient->MarkAsRead(pInfo, VARIANT_TRUE);

            // If you want to delete current email, please use Delete method instead of MarkAsRead
            // oClient->Delete(pInfo);
        }

        // Delete method just mark the email as deleted,
        // Quit method expunge the emails from server exactly.
        oClient->Quit();
    }
    catch (_com_error& ep)
    {
        _tprintf(_T("Error: %s"), (const TCHAR*)ep.Description());
    }
    return false;
}

// client configuration
// You should create your client id and client secret,
// do not use the following client id in production environment, it is used for test purpose only.
const char* clientID = "82309243693-9bjn2e4ucfn4b17dae6d3j3ro2hjb74t.apps.googleusercontent.com";
const char* clientSecret = "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";
const char* scope = "openid%20profile%20email%20https://mail.google.com";
const char* authUri = "https://accounts.google.com/o/oauth2/v2/auth";
const char* tokenUri = "https://www.googleapis.com/oauth2/v4/token";

_bstr_t ParseParameter(const char* uri, const char* key)
{
    _bstr_t value = "";
    if (uri == NULL || key == NULL)
        return value;

    const char* p = strchr(uri, '?');
    p = (p != NULL) ? p + 1 : uri;

    do
    {
        if (_strnicmp(p, key, strlen(key)) == 0)
        {
            p += strlen(key);
            const char* pend = strpbrk(p, "&#");
            if (pend != NULL)
            {
                char* buf = new char[pend - p + 1];
                memcpy(buf, p, pend - p);
                buf[pend - p] = '\0';
                value = buf;

                delete[] buf;
            }
            else
            {
                value = p;
            }

            return value;
        }

        p = strchr(p, '&');
        if (p) p++;

    } while (p);

    return value;
}

_bstr_t RequestAccessToken(const char* code, const char* redirectUri)
{
    printf("Exchanging code for tokens...\n");

    _bstr_t responseText = "";
    try
    {
        MSXML2::IServerXMLHTTPRequestPtr httpRequest = NULL;
        httpRequest.CreateInstance(__uuidof(MSXML2::ServerXMLHTTP));
        if (httpRequest == NULL)
        {
            printf("Failed to create XML HTTP Object, please make sure you install MSXML 3.0 on your machine.\n");
            return responseText;
        }

        _bstr_t tokenRequestBody = "code=";
        tokenRequestBody += code;
        tokenRequestBody += "&redirect_uri=";
        tokenRequestBody += redirectUri;
        tokenRequestBody += "&client_id=";
        tokenRequestBody += clientID;
        tokenRequestBody += "&client_secret=";
        tokenRequestBody += clientSecret;
        tokenRequestBody += "&grant_type=authorization_code";

        const char* postData = (const char*)tokenRequestBody;
        long cdata = (long)strlen(postData);
        LPSAFEARRAY psaHunk = ::SafeArrayCreateVectorEx(VT_UI1, 0, cdata, NULL);
        for (long k = 0; k < cdata; k++)
        {
            BYTE ch = (BYTE)postData[k];
            ::SafeArrayPutElement(psaHunk, &k, &ch);
        }

        _variant_t requestBuffer;
        requestBuffer.vt = (VT_ARRAY | VT_UI1);
        requestBuffer.parray = psaHunk;

        _variant_t async(true);

        httpRequest->setOption((MSXML2::SERVERXMLHTTP_OPTION)2, 13056);
        httpRequest->open(L"POST", _bstr_t(tokenUri), async, vtMissing, vtMissing);
        httpRequest->setRequestHeader(L"Content-Type", L"application/x-www-form-urlencoded");
        httpRequest->send(requestBuffer);

        while (httpRequest->readyState != 4) {
            httpRequest->waitForResponse(1);
        }

        long status = httpRequest->status;
        responseText = httpRequest->responseText;

        if (status < 200 || status >= 300)
        {
            printf("Failed to get access token from server: %d %s\n", status, (const char*)responseText);
        }

        return responseText;
    }
    catch (_com_error& ep)
    {
        printf("Failed to get access token: %s\n", (const char*)ep.Description());
    }

    return responseText;
}

char* userConst;
char* accessTokenConst;

void DoOauthAndRetrieveEmail(const char* password, string &revmail, bool first, pair<string, string>& request)
{
    if (first) {
        IHttpListenerPtr httpListener = NULL;
        httpListener.CreateInstance(__uuidof(EAGetMailObjLib::HttpListener));

        // Creates a redirect URI using an available port on the loopback address.
        if (httpListener->Create("127.0.0.1", 0) == VARIANT_FALSE)
        {
            printf("Failed to listen on %s\n", (const char*)httpListener->GetLastError());
            return;
        }

        char szUri[MAX_PATH + 1];
        sprintf_s(szUri, MAX_PATH, "http://127.0.0.1:%d", httpListener->ListenPort);
        printf("listen on %s ...\n", szUri);

        // Creates the OAuth 2.0 authorization request.
        _bstr_t authorizationRequest = authUri;
        authorizationRequest += "?response_type=code&scope=";
        authorizationRequest += scope;
        authorizationRequest += "&redirect_uri=";
        authorizationRequest += szUri;
        authorizationRequest += "&client_id=";
        authorizationRequest += clientID;

        printf("open %s ...\n", (const char*)authorizationRequest);

        // Opens request in the browser.
        IBrowserUiPtr browserUi = NULL;
        browserUi.CreateInstance(__uuidof(EAGetMailObjLib::BrowserUi));
        browserUi->OpenUrl(authorizationRequest);

        // Waits for the OAuth authorization response.
        if (httpListener->GetRequestUrl(-1) == VARIANT_FALSE)
        {
            printf("Failed to get authorization response %s\n", (const char*)httpListener->GetLastError());
            return;
        }

        // Brings the Console to Focus.
        SetForegroundWindow(GetConsoleWindow());

        // Send response and stop http listener.
        httpListener->SendResponse(_bstr_t("200"),
            _bstr_t("text/html; charset=utf-8"),
            "<html><head></head><body>Please return to the app and close current window.</body></html>");
        httpListener->Close();

        _bstr_t requestUri = httpListener->RequestUrl;
        printf("RequestUri: %s\n", (const char*)requestUri);

        // Checks for errors.
        _bstr_t error = ParseParameter((const char*)requestUri, "error=");
        if (error.length())
        {
            printf("OAuth authorization error: %s\n", (const char*)error);
            return;
        }

        // Check authorization code
        _bstr_t code = ParseParameter((const char*)requestUri, "code=");
        if (code.length() == 0)
        {
            printf("Malformed authorization response: %s\n", (const char*)requestUri);
            return;
        }

        printf("\nAuthorization code: %s\n", (const char*)code);

        _bstr_t responseText = RequestAccessToken((const char*)code, szUri);
        printf("%s\n", (const char*)responseText);

        IOAuthResponseParserPtr parser = NULL;
        parser.CreateInstance(__uuidof(EAGetMailObjLib::OAuthResponseParser));
        parser->Load(responseText);

        _bstr_t user = parser->EmailInIdToken;
        _bstr_t accessToken = parser->AccessToken;

        if (accessToken.length() == 0)
        {
            printf("Failed to request access token, return!\n");
            return;
        }

        printf("User: %s\n", (const char*)user);
        printf("AccessToken: %s\n", (const char*)accessToken);
        userConst = _strdup((const char*)user);
        accessTokenConst = _strdup((const char*)accessToken);
    }
    while (!_kbhit()) {
        bool z = RetrieveWithXOAUTH2((const char*)userConst, (const char*)accessTokenConst, password, revmail, request);
        if (z) {
            cout << "right pass" << endl;
            return;
        }
        Sleep(2000);
    }
}
