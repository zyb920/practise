#ifndef RSA_OPENSSL_H
#define RSA_OPENSSL_H

#include <QByteArray>

namespace ZYB {

class Openssl
{
public:
    enum Type{Encrypt, Decrypt};

    /**
     * @param data     加解密的数据
     * @param keyFile  公有密钥文件
     * @param type     加密 or 解密
     * @return 解密后的数据
     */
    static QByteArray rsa_public (const QByteArray &data, const QByteArray &keyFile, Type type=Decrypt);
    static QByteArray rsa_private(const QByteArray &data, const QByteArray &keyFile, Type type=Decrypt);
    static QByteArray AES        (const QByteArray &key,  const QByteArray &data,    Type type=Decrypt);

    static QByteArray decodeAes_Ctr(const QByteArray &iv, const QByteArray &key, const QByteArray &data);

private:
    Openssl() = delete;
    Openssl(const Openssl&) = delete;
    Openssl & operator=(const Openssl&) = delete;
    ~Openssl() = delete;
};

}



#endif // RSA_OPENSSL_H
