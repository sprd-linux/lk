#include <config_gcm_sw.h>
#include <gcm.h>
#include <authentication.h>
#include <crypto/driver/sprd_rng.h>
#include <sprd_rsa.h>
#include <sprd_crypto_sw.h>
#include <sprdsha.h>

#define ENABLE_SELFTEST 0

#if ENABLE_SELFTEST
extern void crypto_hexdump(const char *title, uint8_t * data, int len);
#endif

struct {
    unsigned int data_len;
    unsigned char e[4];
    unsigned char n[RSA_2048_BUF_LEN];
    int n_bitlen;
    unsigned char R1[RSA_2048_BUF_LEN];
    unsigned char version[VERSION_LEN];
} rsa_authentic_pubkey = {
    RSA_2048_BUF_LEN,
    /* e */
    { 0x00, 0x01, 0x00, 0x01, },
    /* n */
    { 0xd9, 0xde, 0x1e, 0x16, 0xe4, 0xe2, 0xbf, 0x98, 0xb2, 0x5a, 0xf1, 0x1a, 0xe5, 0x54, 0x1e, 0xf0,
      0x43, 0x16, 0xbe, 0x6d, 0x01, 0x31, 0xa1, 0xc8, 0x6d, 0xc7, 0x60, 0x63, 0x40, 0xe2, 0x5e, 0xd9,
      0x39, 0xf3, 0x2a, 0x12, 0x95, 0x23, 0xb8, 0x21, 0x49, 0x8e, 0x8c, 0x46, 0x1c, 0xee, 0x01, 0x81,
      0x70, 0xfb, 0xb4, 0x41, 0x4f, 0x89, 0x70, 0xc4, 0x83, 0x3c, 0xd2, 0x03, 0xb5, 0xcc, 0x5c, 0x09,
      0x9e, 0x48, 0xc2, 0xc1, 0x0e, 0xc3, 0xd3, 0x25, 0x57, 0xbe, 0xc5, 0x5f, 0xbb, 0x94, 0xab, 0xcc,
      0xd3, 0x6e, 0x5b, 0x2e, 0xf1, 0x1a, 0x0d, 0xd5, 0xde, 0x77, 0xd3, 0x96, 0x30, 0xf4, 0xbf, 0x60,
      0xfd, 0x95, 0xbb, 0xd6, 0x1a, 0xb9, 0x18, 0x9b, 0x85, 0xe3, 0x39, 0xf5, 0x30, 0x47, 0x80, 0x2c,
      0x5a, 0x98, 0x7d, 0x1f, 0x9d, 0x48, 0x3e, 0x82, 0x15, 0x78, 0x3a, 0x76, 0x59, 0xe7, 0x28, 0x44,
      0x6f, 0x4e, 0x6f, 0xff, 0x9e, 0x85, 0x2d, 0xd3, 0xf1, 0x1a, 0xc7, 0x8e, 0xca, 0xd2, 0xe1, 0x15,
      0x4c, 0x77, 0x2e, 0xf9, 0x70, 0x55, 0x68, 0xf6, 0x45, 0xe7, 0xd5, 0xb0, 0xcf, 0x78, 0x9d, 0xb4,
      0x43, 0x04, 0xb9, 0x22, 0x34, 0x21, 0x85, 0xb8, 0xdb, 0x78, 0xd3, 0x32, 0x76, 0x0b, 0x52, 0xc3,
      0x16, 0xc5, 0x6e, 0x32, 0xba, 0xc8, 0x0d, 0x80, 0x35, 0x60, 0x67, 0xcc, 0x94, 0x07, 0xff, 0x6d,
      0x2b, 0x48, 0x81, 0x7d, 0x66, 0xa3, 0x12, 0x9e, 0x27, 0x98, 0x00, 0x4c, 0xf4, 0x7d, 0x14, 0xff,
      0x9b, 0x04, 0x79, 0x62, 0xf6, 0x95, 0x42, 0x56, 0x0d, 0xef, 0xd8, 0xdf, 0x2e, 0xce, 0x0a, 0x77,
      0xdf, 0xee, 0xc5, 0x54, 0xf9, 0x5e, 0xa9, 0x86, 0x87, 0x4c, 0xe0, 0xae, 0x6b, 0x67, 0xe6, 0x82,
      0x35, 0xee, 0xbf, 0x38, 0xf5, 0xa0, 0x12, 0xa7, 0x90, 0xfd, 0x49, 0x42, 0x80, 0xdb, 0x14, 0xa7, },
    /* n_bitlen */
    RSA_2048_BUF_LEN * 8,
    /* R1 */
    { 0 },
    /* verision */
    { 0 },
};

/*
 * flg = 1 : respond to authentication requests
 * Msg : [out] M1, the encrypted R1(version||rand||socid) with rsa_pubkey
 * flg = 0 for return authentication result
 * Msg : [in] M2, the data to verify with rsa_pubkey
 */

int authentication (unsigned char *msg, int flg)
{
    int res = BAD_INPUT;
    int temp_len = 0;
    unsigned char rng[CRYPTO_RNG_MAX_SIZE] = {0};
    unsigned char uid[HASH_SHA256_BUF_LEN] = {0};
    unsigned char hash_uid[HASH_SHA256_BUF_LEN + 1] = {0};
    unsigned char buf[RSA_2048_BUF_LEN] = {0};

    if (msg == NULL) {
        printf("authentication: bad input!\n");
        return BAD_INPUT;
    }

    switch (flg) {
    case VERIFICATION:
        res = RSA_Verify(rsa_authentic_pubkey.e, rsa_authentic_pubkey.n, rsa_authentic_pubkey.n_bitlen, msg, buf);

        if (res != HASH_SHA256_BUF_LEN || memcmp(rsa_authentic_pubkey.R1, buf, RSA_2048_VERIFY_BUF_LEN)){
            crypto_hexdump("R1:", rsa_authentic_pubkey.R1, RSA_2048_VERIFY_BUF_LEN);
            crypto_hexdump("buf:", buf, RSA_2048_VERIFY_BUF_LEN);
            return AUTH_FAILED;
        } else {
            memset(msg, 0, rsa_authentic_pubkey.n_bitlen);
            return AUTH_SUCCESS;
        }
        break;

    case ENCRYPTION:
        /* 1.version */
        memcpy(rsa_authentic_pubkey.R1, rsa_authentic_pubkey.version, VERSION_LEN);
        temp_len += VERSION_LEN;

        /* 2.random R0 */
        if (sprd_rng_gen(rng, DEFINE_RNG_LEN)) {
            printf("GET_RNG_FAILED\n");
            return GET_RNG_FAILED;
        }
        crypto_hexdump("rng:", rng, DEFINE_RNG_LEN);
        memcpy(rsa_authentic_pubkey.R1+temp_len, rng, DEFINE_RNG_LEN);
        memset(rng, 0, CRYPTO_RNG_MAX_SIZE);
        temp_len += HASH_SHA256_BUF_LEN;

        /* 3.hashed uid instead socid */
        if (sprd_get_chip_hex_uid (uid)) {
            return GET_UID_FAILED;
        }
        sha256_csum_wd_sw(uid, HASH_SHA256_BUF_LEN, hash_uid, NULL);
        memset(uid, 0, HASH_SHA256_BUF_LEN);
        memcpy(rsa_authentic_pubkey.R1+temp_len, hash_uid, HASH_SHA256_BUF_LEN);
        memset(hash_uid, 0, HASH_SHA256_BUF_LEN+1);
        temp_len += HASH_SHA256_BUF_LEN;
        crypto_hexdump("R1:", rsa_authentic_pubkey.R1, temp_len);

        /* 4. rsa encrypt with pkcs1.5 padding */
        res = RSA_PubEnc(rsa_authentic_pubkey.e, rsa_authentic_pubkey.n,
                rsa_authentic_pubkey.n_bitlen, rsa_authentic_pubkey.R1, temp_len, msg);

        if (0 != res) {
            return ENCRYPT_FAILED;
        } else {
            /* save the hashed R1 */
            sha256_csum_wd_sw(rsa_authentic_pubkey.R1, HASH_SHA256_BUF_LEN, buf, NULL);
            memset(rsa_authentic_pubkey.R1, 0, temp_len);
            memcpy(rsa_authentic_pubkey.R1, buf, HASH_SHA256_BUF_LEN);
            return ENCRYPT_SUCCESSED;
        }
        break;

    default:
        printf("unsupported flg!\n");
    }

    return res;
}
/* do not use this */
#if ENABLE_SELFTEST
void authentication_test()
{
    unsigned char msg[ENCRYPT_SUCCESSED] = {0};
    int ret = -1;
    ret = authentication (msg, 1);
    printf("ret = %d\n", ret);
    ret = authentication (msg, 1);
    printf("ret = %d\n", ret);
    ret = authentication (msg, 0);
    printf("ret = %d\n", ret);
    ret = authentication (msg, 2);
    printf("ret = %d\n", ret);
    return 0;
 }
#endif

