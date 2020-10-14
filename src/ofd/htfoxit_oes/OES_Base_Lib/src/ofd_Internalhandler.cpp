#include "../include/stdafx.h"

std::string strPFX = std::string("kLbUICWkbJxTQNgJ+0qHIA7O11WeNuKUyUAyLk9L3+kwhuwhF4eC3KLbyzXin") + \
                    std::string("HHR7QU1mH0k9bmOoNLT3QRHVyJUbhg4fDPC96oAddVr63OcFGnYU0s+13eQsSu") + \
                    std::string("McPhyZdqovUDheVsoAWiXzgluzDHoLx1ywZEo1A0xK9CchVLUHL+1Z3hSKiEnM") + \
                    std::string("FCC1h31Vw7BEVrvNdG0Mo/WkctLKWY49OU9hlwUvPLko5pNcdrIxpjOFLqN6n9") + \
                    std::string("cuiWGPXy9SphyFnaVmdqe/cvTaWpLMs9kFjGXzQuIEi3QB8VZFlkmlU1exAwGm") + \
                    std::string("yulXoc5h9LDQN3A8ZgdaYROtCr/ngW3PbNJqm27dmE/qjKEvTdzJSUAk25xzpu") + \
                    std::string("EwCiX0mXIhbaGIWvKPlOg+Qhhd3xWP/tW3Ib8towfOP4yLh3UNMrI7l6IiYA2b") + \
                    std::string("sRplWi/A4fUN6b6pZO/u+iADt8YczYl5kBvxrKZV9Pg9ceqkWqBLg17zqih26g") + \
                    std::string("HT625BRzNc//rIQAKxKHPkfETI99GarCiBN8emRGvkoOFAiNxTRpbiDUYQ+/Vb") + \
                    std::string("qsPglFvXa+Hnn8t3fl/N3CgVde3XjfI7JYwwbdImkqksad4RUhqvhEKBhjLiR9") + \
                    std::string("ZQ3SV9qfnuu4AWIRs/xpRtoaZeGd9invPDVBN9oooNV/nJAdNAQt+h7rG1z4S4") + \
                    std::string("bq+sD6vb9xxOk14wAwnN2QP4ZQnz9xFa7irXw9FJHlb/vKTd9kl8L/SUJYWI6s") + \
                    std::string("94TUZnQEPOhZyRfy7UKZPHnCv398iMzz2Aj2f6zaJ49GhOrO7tVH9qIE2Q455K") + \
                    std::string("dkRoghGcPxzzI28aANNxA7lbiFEJ/pmL0ak94KVkefZPXzXGuT7JAHeIYFlgsj") + \
                    std::string("78HBbJp2GaS2bqNQHDCmNibQZH8/eQavUcsu4nmS5xyir4qdSNHr/N5SQWTcO+") + \
                    std::string("qAz7610tvaZTho3xeNsk3nh3YX3/WLybrR28rFTl7c1uNGR+Vy23Fsx/WO/e1c") + \
                    std::string("Lm0t/YiX9UGMWIzdQAoyPnAXbMIhwW3e4juOtOMexkvOll9J8JwOaTbmOOzjwY") + \
                    std::string("LdtvKICp3RL2rh2HV6vfwMwYZAzArbwmqUN9okw+kXBlQiwJR3Ff5G4FAqHuua") + \
                    std::string("3fXuOK9X4YCx4AuXEPprf7tejt1Iihe/qLzKlSQc0BMfqDIScgtMMGEGLAQD/K") + \
                    std::string("2CACNzKXtLDJzbH8rVceNwELlWYAlNILnI/9qquQcbe0VNkCTfB+Ec3hZgCulw") + \
                    std::string("V6Dd7nBCNpsgvP2hb9a7N5jjOca+vo60ppnVUMC+szXbFz9I2DecKb6rjS1QMU") + \
                    std::string("ExuFqpd4EMwYhjg7xLQzwOMO++quS+AnvAJobTmM52qJ90vY/Trh7uPvWIClPc") + \
                    std::string("b8bqcMqzAKVffbwHSO0QGGyd28pgKP8/GfZ8g9YRHy3s8aRBk0e/rZhUvoL/Mt") + \
                    std::string("7PGhZOjLZbzPLDRsv3kSzhpxJ1xAMOme2OpdTu7nQM19rRsf+lZYa6+Rq1w9PW") + \
                    std::string("kNANVREq0gJ64wjSx6SjCPld9sIX5FQYjbdiER+BCaMOy6i9eGOKF4NPuUmkM4") + \
                    std::string("ka8rlPmBbyy/N9ts6dUTGdr4LkOiyP/TzhRGXx0yY7A5tSvhOjwV2KbGX5FyIh") + \
                    std::string("lLgR4q0hfitZn1cVMAWQID2ylMbaiX5I54Vl7yf7bTjNJuEOenUtynt7vsL2Il") + \
                    std::string("0OWAJMl+tkRP6UjDbkLYsuxsPSVc/h4rO3/czq+bREL7k4Fy+6T9WB0vIFEeUf") + \
                    std::string("ih2TOAeGWuf605SUjnNulTEWWWC5JX0cNh/A8c6NiLa/XMX4Fb2vje+IDXihpY") + \
                    std::string("7o/sVEE5pvRUMjNdXqH/HhsHmlR2nTGIYlwmd1KPGZVD9vp30UWZBHJZTpRQSl") + \
                    std::string("sYwCp6PizhCic6kU2dwu7BtKYRQPhb5+SWknC7AtDuMdU3fqkUsnVpxShPwrqr") + \
                    std::string("OuOypQpgBLyrJiLLvhJAC7sHWj/vQkXGau1PXihkTHmSbBq1rSwIM0ubn8QnLG") + \
                    std::string("jtjRFMPZZOcVH/3zMVoWIuOoeQVSGTFdznJlBQXud0kp0+cF2j4dxmwn4TbGQt") + \
                    std::string("xoqN08PRWkxMtQ4kI8k7FOvIMUtnTSGqJzF30MCNx8wzcMmnTZW0gt00GxeXxs") + \
                    std::string("O7cKHPzYMf0mSujW2Sq1YDiUF/ybnWVT9he/Hiu42U7rmQDn3efFiz22+FvGyK") + \
                    std::string("qiFkV6dDwcCHPlsLQUbxHT2hEMM+uH2cT3DPDHxH7rrdBHzG91GkjyvzI+TUhI") + \
                    std::string("+xTZAc4zx0Tr2Git0zrY82vR9NUq4VUjtzHt+xZsE2+CMtOOuqqzJ0S9aQItGO") + \
                    std::string("pnewL5EYM457dKCcDQVSo+aiUV5k39YXCASiWZCmKbE7T01EZGAaYlre1haOj3") + \
                    std::string("GgTMjRCxT9A+f8ROxd78O60X4QgyfEuQCOOzu0r944CkVK/NypfHcUOadvtpDy") + \
                    std::string("bpxTZGa4CMeisAMIWeFH9mfu8fTRGM1w+0SnwXa7LCjeCD3kerJrlxAQ5nCMRG") + \
                    std::string("CeHD/4aC0tYn+UrvpE6PAfpZf4YOUSjpZrjFkmqpYQ74G9iJGbTpSRYBQrvM8y") + \
                    std::string("OjTiCJmKnrwRXrnJMXfzNJ+I27c9wkwgk0OyynG/+s+iiYiGpc8InTKfTVif66") + \
                    std::string("TKHmt/KyambbrqTqB/ccxSf08SrZE0XXyXv+3DeV/adZ9v8y05aeeBWaW2rFq6") + \
                    std::string("YylZwinMxfTp/5a4nrzMQPY0GAimbr53dAxpVWuvY8u6Q2CnpQtR3yfRMIcqiK") + \
                    std::string("WP3oUTz8L3Xj5RIh8FUGxUquQSaNS2p+tzPziBXchGNaF1PEGrimiPdCcBv4/G") + \
                    std::string("wk3nLmgviRKznGkgGy30tDsgHt5JDcFchaYAt9j7vnGfd9MWivQk17P1lntJ88") + \
                    std::string("pOc4/kPJg4Y0E+WYoL3BaoCvsn/vJ/VwA+eJTu3SP/6XH3VQ8Zfm/etG+9tGHM") + \
                    std::string("53sixH2811hZ/hqkqdIk79XFRDSVXoT4AME6IC7NI9a3N9+uO/MdVR8p2PFQCi") + \
                    std::string("i9b/k50aDAg+q4WDraahFrq725dP5WN4xMHhct9i2Ne97jruBfvE1d3AuTiuiJ") + \
                    std::string("3IHbFHhAPAVVbLzzyAZz15GHxU2GUmkrSKVTKAUvRniyZWceUsp3tbo9Eb0BsI") + \
                    std::string("T1UTh7w+cIX8hOnCliWW/iQpI7k2AAIGvJFSFe7y9SGw12fnqhpOPHSQ77qY8B") + \
                    std::string("2+mU5rebsiaY4ztBj28ZnFKLODXNBWmAaTRUs3KfoOUzP29H85j2J+U+uJhORx") + \
                    std::string("GVwBzDRglC7j8ZzS+6mKr9sl6Cktyj5697AYDc4ge114nWVH8k4olZgfxscbeh") + \
                    std::string("jushhI9ryOvUW0FrxREZyWzXNybG3JhEmM2P0SJhBbCdZSNvNheikD+glBmneM") + \
                    std::string("wPYdQtjx/msE4MpkYqNUk3Ml1ktNfxukN1ACzsI89xEEbdWd6XoIM0K4tOlWht") + \
                    std::string("I4irGvOAebXTFAIiljP");

//////////////////////////////////////////////////////////////////////////
//class COFD_InternalHandler
COFD_InternalHandler::COFD_InternalHandler()
{
        m_pVerifyHandler = NULL;
        m_pSignHandler = NULL;
}

COFD_InternalHandler::~COFD_InternalHandler()
{
    m_pSignHandler->Release();
    m_pVerifyHandler->Release();
}

COFD_InternalHandler*	COFD_InternalHandler::Create()
{
	int cbData = 0;
    COFD_Utils::Base64_Decode((const unsigned char*)strPFX.data(), strPFX.length(), NULL, &cbData);

	char* pbData = new char[cbData];
    memset(pbData, 0, cbData);

    COFD_Utils::Base64_Decode((const unsigned char*)strPFX.data(), strPFX.length(), (unsigned char*)pbData, &cbData);

    std::string strKey = COFD_Utils::GetSeedSHA256Reverse();
    CRYPT_ArcFourCryptBlock((unsigned char*)pbData, cbData, (unsigned char*)strKey.data(), strKey.length());

    std::string strPassword("123456");
    COFD_InternalHandler* pHandler = new COFD_InternalHandler();
    if (!pHandler->Initialize(pbData, cbData, strPassword))
	{
        SAFE_DELETE(pHandler);
	}

    SAFE_DELETE_BLOCK(pbData);

	return pHandler;
}

void	COFD_InternalHandler::Release()
{
	delete this;
}

bool  COFD_InternalHandler::CreateAlgorithmHandler(char* pbData, FX_DWORD cbData,
                                                   std::string pstrPassword)
{
    std::string strDigestMethod("sha256");
    m_pSignHandler = COFD_OESSignHandler::Create(pbData, cbData, pstrPassword, strDigestMethod);
    if (!m_pSignHandler) return false;

    m_pVerifyHandler = COFD_OESVerifyHandler::Create(pbData, cbData, pstrPassword, strDigestMethod);
    if (!m_pVerifyHandler) return false;

    return true;
}

bool	COFD_InternalHandler::Initialize(char* pbData, FX_DWORD cbData, std::string PFX_PWD)
{
    COFD_Utils::InitOpenSSL();

    BIO* bo = BIO_new_mem_buf(pbData, cbData);
    if (!bo) return false;

    //Extracting client certificate & private key from .pfx or .p12 file
    PKCS12 *p12 = d2i_PKCS12_bio(bo, NULL);
    BIO_free(bo);

    if (!p12)
    {
        printf("Extracting client certificate & private key failed.");
        return false;
    }

    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;

    STACK_OF(X509) *ca = NULL;
    int result = PKCS12_parse(p12, PFX_PWD.data(), &pkey, &x509, &ca);
    if (!result)
    {
        ERR_print_errors_fp(stderr);
        printf("Parsing PKCS#12 failed.\n");
        return false;
    }
    X509_free(x509);
    PKCS12_free(p12);
    sk_X509_free(ca);

    EVP_PKEY_free(pkey);
    EVP_cleanup();

    if (!CreateAlgorithmHandler(pbData, cbData, PFX_PWD))
    {
        return false;
    }

    return true;
}
