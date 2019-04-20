#include "openssl.h"

#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <memory>

#include <QtDebug>

QByteArray ZYB::Openssl::rsa_public(const QByteArray &data, const QByteArray &keyFile, Type type)
{
    FILE *file = fopen(keyFile.constData(), "rb");
    if(file == nullptr){
        qWarning() << "open key file error.";
        return "";
    }
    RSA *p_rsa = PEM_read_RSA_PUBKEY(file, nullptr, nullptr, nullptr);
    fclose(file);

    if(p_rsa == nullptr){
        qWarning() << "read key faild.";
        return "";
    }

    int rsa_len = RSA_size(p_rsa);

    //std::unique_ptr<int[]> p(new int[10]);//ok
    std::shared_ptr<uchar> out(new uchar[rsa_len], [](uchar *p){
        delete []p;
    });

    int ret;

    if(type == Decrypt) {
        ret = RSA_public_decrypt(rsa_len,
                                 reinterpret_cast<const uchar *>(data.constData()),
                                 out.get(),
                                 p_rsa,
                                 RSA_PKCS1_PADDING);
    } else {
        ret = RSA_public_encrypt(data.length(),
                                 reinterpret_cast<const uchar *>(data.constData()),
                                 out.get(),
                                 p_rsa,
                                 RSA_PKCS1_PADDING);
    }

    RSA_free(p_rsa);

    if(ret < 0){
        qWarning() << "encrypt faild.";
        return "";
    }

    return QByteArray(reinterpret_cast<char *>(out.get()), rsa_len);
}

QByteArray ZYB::Openssl::rsa_private(const QByteArray &data, const QByteArray &keyFile, Type type)
{
    FILE *file = fopen(keyFile.constData(), "rb");
    if(file == nullptr){
        qWarning() << "open key file error.";
        return "";
    }
    RSA *p_rsa = PEM_read_RSAPrivateKey(file, nullptr, nullptr, nullptr);
    fclose(file);

    if(p_rsa == nullptr){
        qWarning() << "read key faild.";
        return "";
    }

    int rsa_len = RSA_size(p_rsa);
    std::shared_ptr<uchar> out(new uchar[rsa_len], [](uchar *p){
        delete []p;
    });

    int ret;

    if(type == Decrypt) {
        ret = RSA_private_decrypt(rsa_len, reinterpret_cast<const uchar *>(data.constData()),
                                  out.get(),
                                  p_rsa,
                                  RSA_PKCS1_PADDING);
    } else {
        ret = RSA_private_encrypt(data.length(),
                                  reinterpret_cast<const uchar *>(data.constData()),
                                  out.get(),
                                  p_rsa,
                                  RSA_PKCS1_PADDING);
    }

    RSA_free(p_rsa);

    if(ret < 0){
        qWarning() << "encrypt faild.";
        return "";
    }

    return QByteArray(reinterpret_cast<char *>(out.get()), rsa_len);
}

QByteArray ZYB::Openssl::AES(const QByteArray &key, const QByteArray &data, Type type)
{
    AES_KEY AesKey;

    int ret = -1;
    if(type) {

        ret = AES_set_decrypt_key(reinterpret_cast<const uchar *>(key.constData()),
                                  128,
                                  &AesKey);
    } else {
        ret = AES_set_encrypt_key(reinterpret_cast<const uchar *>(key.constData()),
                                  128,
                                  &AesKey);
    }

    if(ret < 0) return "";

    // iv 偏移量
    uchar iv [AES_BLOCK_SIZE+1] = "L+\\~f4,Ir)b$=pkf";

    int len = data.length();
    if(len % AES_BLOCK_SIZE != 0)
        len = (len/AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;

    //补齐16的倍数
    QByteArray data2(data);
    data2.append(len - data2.length(), '\0');

    std::shared_ptr<uchar> out(new uchar[len+1], [](uchar *p){
        delete []p;
    });

    if(type == Decrypt) {
        AES_cbc_encrypt(reinterpret_cast<const uchar *>(data2.data()),
                        out.get(),
                        static_cast<size_t>(len),
                        &AesKey,
                        iv,
                        AES_DECRYPT);
    } else {
        AES_cbc_encrypt(reinterpret_cast<const uchar *>(data2.data()),
                        out.get(),
                        static_cast<size_t>(len),
                        &AesKey,
                        iv,
                        AES_ENCRYPT);
    }
    out.get()[len] = '\0';
    return QByteArray(reinterpret_cast<char *>(out.get()), len);
}

QByteArray ZYB::Openssl::decodeAes_Ctr(const QByteArray &iv, const QByteArray &key, const QByteArray &data)
{
    AES_KEY AesKey;

    //设置加密密钥
    int ret = AES_set_encrypt_key(reinterpret_cast<const uchar *>(key.constData()),
                                  128,
                                  &AesKey);
    if(ret < 0)
        return "";

    uint num = 0;
    uchar ecount[AES_BLOCK_SIZE] = {0};
    char out[AES_BLOCK_SIZE] = {0};

    AES_ctr128_encrypt(reinterpret_cast<const uchar *>(data.constData()),
                       reinterpret_cast<uchar *>(out),
                       static_cast<size_t>(data.length()),
                       &AesKey,
                       const_cast<uchar *>(reinterpret_cast<const uchar *>(iv.data())),
                       ecount,
                       &num);

    return QByteArray(out, data.length());
}

