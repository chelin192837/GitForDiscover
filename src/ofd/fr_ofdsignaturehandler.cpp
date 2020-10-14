#include "fr_include.h"
#include "fr_ofdsignaturehandler.h"

// class FR_OFDSignatureHandler
FR_OFDSignatureHandler::FR_OFDSignatureHandler(FSSignatureHandler* handler)
{
    m_SigHandler = handler;
}

FR_OFDSignatureHandler::~FR_OFDSignatureHandler()
{
    
}

std::string FR_OFDSignatureHandler::getDigestMethod()
{
    switch (m_signClientData.m_algorithm) {
        case FSSignatureDefines::e_digestSHA1:
            return szOID_OIWSEC_sha1;
        case FSSignatureDefines::e_digestSHA256:
        default:
            return szOID_NIST_sha256;
    };
}

std::string FR_OFDSignatureHandler::getSignMethod()
{
    switch (m_signClientData.m_algorithm) {
        case FSSignatureDefines::e_digestSHA1:
            return szOID_RSA_SHA1RSA;
        case FSSignatureDefines::e_digestSHA256:
        default:
            return szOID_RSA_SHA256RSA;
    }
}

string      FR_OFDSignatureHandler::getCertData()
{
    if (!m_SigHandler) {
        return "";
    }
    
    if (!m_CertData.empty()) {
        return m_CertData;
    }
    
    FSString data = m_SigHandler->getCertData(m_signClientData.pfxpath.c_str(), m_signClientData.password.data(), m_signClientData.password.size(), m_signClientData.pClientData);
    if (data.isEmpty()) {
        return "";
    }
    
    string result;
    result.assign(data.getBuffer(), data.getBufferLen());
    m_CertData = result;
    return result;
}

std::string FR_OFDSignatureHandler::sign(std::string digest)
{
    if (!m_SigHandler) {
        return "";
    }
    if (!m_SignedData.empty() && digest == m_digest) {
        return m_SignedData;
    }
    FSString data = m_SigHandler->sign((void*)digest.data(), digest.size(), m_signClientData.pfxpath.c_str(), m_signClientData.password.data(), m_signClientData.password.size(), m_signClientData.m_algorithm, m_signClientData.pClientData);
    if (data.isEmpty()) {
        return "";
    }
    
    string result;
    result.assign(data.getBuffer(), data.getBufferLen());
    m_SignedData = result;
    m_digest = digest;
    return result;
}



