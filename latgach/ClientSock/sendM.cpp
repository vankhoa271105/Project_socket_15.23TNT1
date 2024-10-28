#include "func.h"
#include "C:\Program Files (x86)\EASendMail\Include\tlh\EASendMailObj.tlh"
#include "C:\Program Files (x86)\EASendMail\Include\tlh\msxml3.tlh"
////using namespace MSXML2;
const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;

const int AuthAuto = -1;
const int AuthLogin = 0;
const int AuthNtlm = 1;
const int AuthCramMd5 = 2;
const int AuthPlain = 3;
const int AuthMsn = 4;
const int AuthXoauth2 = 5;
void SendMailWithXOAUTH2(const char* lpszEmail, const char* lpszAccessToken, const char* endmail, int lenh)
{
    EASendMailObjLib::IMailPtr oSmtp = NULL;
    oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
    oSmtp->LicenseCode = _bstr_t("TryIt");

    // Gmail SMTP server address
    oSmtp->ServerAddr = _bstr_t("smtp.gmail.com");
    // Using 587 port, you can also use 465 or 25 port
    oSmtp->ServerPort = 587;

    // Enable SSL/TLS connection
    oSmtp->ConnectType = ConnectSSLAuto;

    //  XOAUTH 2.0 authentication
    oSmtp->AuthType = AuthXoauth2;
    // set user authentication
    oSmtp->UserName = lpszEmail;
    // use access token as password
    oSmtp->Password = lpszAccessToken;

    // Your email address
    oSmtp->FromAddr = lpszEmail;
    // Please change recipient address to yours for test
    oSmtp->AddRecipient(_bstr_t("Support Team"),
        _bstr_t(endmail), 0);

    oSmtp->Subject = _bstr_t("test email from gmail account using OAUTH 2 C++");
    oSmtp->BodyText = _bstr_t("Hello, this is a test email from SendGmailAPI C++ ....");
    //dieu chinh file gui di
    string filename;
    if (lenh == 1)
        filename = "result.txt";
    else if (lenh == 5)
        filename = "chaymanhinh2.png";
    else if (lenh == 7)
        filename == "shutdown.txt";
    int i = oSmtp->AddAttachment(_bstr_t(filename.c_str()));

    printf("start to send email using OAUTH 2.0 ...\n");

    if (oSmtp->SendMail() == 0)
        printf("The email has been submitted to server successfully!\n");
    else
        printf("%s\n", (const char*)oSmtp->GetLastErrDescription());
}

// client configuration
// You should create your client id and client secret,
// do not use the following client id in production environment, it is used for test purpose only.
const char* clientID_send = "43792816665-33v59p4o27qpj3k4uuf27a3thgmf6c9p.apps.googleusercontent.com";
const char* clientSecret_send = "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";
const char* scope_send = "openid%20profile%20email%20https://mail.google.com";
const char* authUri_send = "https://accounts.google.com/o/oauth2/auth";
const char* tokenUr_send = "https://oauth2.googleapis.com/token";

_bstr_t RequestAccessToken_send(const char* code, const char* redirectUri)
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
        tokenRequestBody += clientID_send;
        tokenRequestBody += "&client_secret=";
        tokenRequestBody += clientSecret_send;
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
        httpRequest->open(L"POST", _bstr_t(tokenUr_send), async, vtMissing, vtMissing);
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

void DoOauthAndSendEmail(const char* endmail, int lenh)
{
        EASendMailObjLib::IHttpListenerPtr httpListener = NULL;
        httpListener.CreateInstance(__uuidof(EASendMailObjLib::HttpListener));

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
        _bstr_t authorizationRequest = authUri_send;
        authorizationRequest += "?response_type=code&scope=";
        authorizationRequest += scope_send;
        authorizationRequest += "&redirect_uri=";
        authorizationRequest += szUri;
        authorizationRequest += "&client_id=";
        authorizationRequest += clientID_send;

        printf("open %s ...\n", (const char*)authorizationRequest);

        // Opens request in the browser.
        EASendMailObjLib::IBrowserUiPtr browserUi = NULL;
        browserUi.CreateInstance(__uuidof(EASendMailObjLib::BrowserUi));
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

        _bstr_t responseText = RequestAccessToken_send((const char*)code, szUri);
        printf("%s\n", (const char*)responseText);

        EASendMailObjLib::IOAuthResponseParserPtr parser = NULL;
        parser.CreateInstance(__uuidof(EASendMailObjLib::OAuthResponseParser));
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
    SendMailWithXOAUTH2((const char*)user, (const char*)accessToken, endmail, lenh);
}