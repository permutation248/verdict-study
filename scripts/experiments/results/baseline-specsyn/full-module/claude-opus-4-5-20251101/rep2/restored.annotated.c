#include <stddef.h>

#include <stdint.h>

#include <string.h>

#include <strings.h>

#include <unistd.h>

enum __anonenum_x509_parser_errors_1 {
	X509_PARSER_ERROR_VERSION_ABSENT = -1,
	X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH = -2,
	X509_PARSER_ERROR_VERSION_NOT_3 = -3
};

typedef uint8_t u8;

typedef uint16_t u16;

typedef uint32_t u32;

typedef uint64_t u64;

enum __anonenum_tag_class_2 {
	CLASS_UNIVERSAL = 0x00,
	CLASS_APPLICATION = 0x01,
	CLASS_CONTEXT_SPECIFIC = 0x02,
	CLASS_PRIVATE = 0x03
};

typedef enum __anonenum_tag_class_2 tag_class;

enum __anonenum_asn1_type_3 {
	ASN1_TYPE_BOOLEAN = 0x01,
	ASN1_TYPE_INTEGER = 0x02,
	ASN1_TYPE_BIT_STRING = 0x03,
	ASN1_TYPE_OCTET_STRING = 0x04,
	ASN1_TYPE_NULL = 0x05,
	ASN1_TYPE_OID = 0x06,
	ASN1_TYPE_ENUMERATED = 0x0a,
	ASN1_TYPE_SEQUENCE = 0x10,
	ASN1_TYPE_SET = 0x11,
	ASN1_TYPE_PrintableString = 0x13,
	ASN1_TYPE_T61String = 0x14,
	ASN1_TYPE_IA5String = 0x16,
	ASN1_TYPE_UTCTime = 0x17,
	ASN1_TYPE_GeneralizedTime = 0x18
};

enum __anonenum_hash_alg_id_4 {
	HASH_ALG_UNKNOWN = 0,
	HASH_ALG_MD2 = 1,
	HASH_ALG_MD4 = 2,
	HASH_ALG_MD5 = 3,
	HASH_ALG_MDC2 = 4,
	HASH_ALG_SHA1 = 5,
	HASH_ALG_WHIRLPOOL = 6,
	HASH_ALG_RIPEMD160 = 7,
	HASH_ALG_RIPEMD128 = 8,
	HASH_ALG_RIPEMD256 = 9,
	HASH_ALG_SHA224 = 10,
	HASH_ALG_SHA256 = 11,
	HASH_ALG_SHA384 = 12,
	HASH_ALG_SHA512 = 13,
	HASH_ALG_SHA512_224 = 14,
	HASH_ALG_SHA512_256 = 15,
	HASH_ALG_SHA3_224 = 16,
	HASH_ALG_SHA3_256 = 17,
	HASH_ALG_SHA3_384 = 18,
	HASH_ALG_SHA3_512 = 19,
	HASH_ALG_SHAKE128 = 20,
	HASH_ALG_SHAKE256 = 21,
	HASH_ALG_SM3 = 22,
	HASH_ALG_GOSTR3411_94 = 23,
	HASH_ALG_STREEBOG256 = 24,
	HASH_ALG_STREEBOG512 = 25,
	HASH_ALG_HBELT = 26,
	HASH_ALG_BASH256 = 27,
	HASH_ALG_BASH384 = 28,
	HASH_ALG_BASH512 = 29
};

typedef enum __anonenum_hash_alg_id_4 hash_alg_id;

enum __anonenum_sig_alg_id_5 {
	SIG_ALG_UNKNOWN = 0,
	SIG_ALG_DSA = 1,
	SIG_ALG_RSA_SSA_PSS = 2,
	SIG_ALG_RSA_PKCS1_V1_5 = 3,
	SIG_ALG_ED25519 = 4,
	SIG_ALG_ED448 = 5,
	SIG_ALG_SM2 = 6,
	SIG_ALG_GOSTR3410_2012_256 = 7,
	SIG_ALG_GOSTR3410_2012_512 = 8,
	SIG_ALG_GOSTR3410_2001 = 9,
	SIG_ALG_GOSTR3410_94 = 10,
	SIG_ALG_BIGN = 11,
	SIG_ALG_ECDSA = 12,
	SIG_ALG_RSA_9796_2_PAD = 13,
	SIG_ALG_MONKEYSPHERE = 14,
	SIG_ALG_BELGIAN_RSA = 15
};

typedef enum __anonenum_sig_alg_id_5 sig_alg_id;

enum __anonenum_mgf_alg_id_6 {
	MGF_ALG_UNKNOWN = 0,
	MGF_ALG_MGF1 = 1
};

typedef enum __anonenum_mgf_alg_id_6 mgf_alg_id;

enum __anonenum_spki_alg_id_7 {
	SPKI_ALG_UNKNOWN = 0,
	SPKI_ALG_ECPUBKEY = 1,
	SPKI_ALG_ED25519 = 2,
	SPKI_ALG_ED448 = 3,
	SPKI_ALG_X25519 = 4,
	SPKI_ALG_X448 = 5,
	SPKI_ALG_RSA = 6,
	SPKI_ALG_DSA = 7,
	SPKI_ALG_GOSTR3410_2012_256 = 8,
	SPKI_ALG_GOSTR3410_2012_512 = 9,
	SPKI_ALG_GOSTR3410_2001 = 10,
	SPKI_ALG_GOSTR3410_94 = 11,
	SPKI_ALG_BIGN_PUBKEY = 12
};

typedef enum __anonenum_spki_alg_id_7 spki_alg_id;

enum __anonenum_curve_id_8 {
	CURVE_UNKNOWN = 0,
	CURVE_BIGN256v1 = 1,
	CURVE_BIGN384v1 = 2,
	CURVE_BIGN512v1 = 3,
	CURVE_C2PNB163V1 = 4,
	CURVE_SECT571K1 = 5,
	CURVE_SECT163K1 = 6,
	CURVE_SECP192K1 = 7,
	CURVE_SECP224K1 = 8,
	CURVE_SECP256K1 = 9,
	CURVE_SECP192R1 = 10,
	CURVE_SECP224R1 = 11,
	CURVE_SECP256R1 = 12,
	CURVE_SECP384R1 = 13,
	CURVE_SECP521R1 = 14,
	CURVE_BRAINPOOLP192R1 = 15,
	CURVE_BRAINPOOLP224R1 = 16,
	CURVE_BRAINPOOLP256R1 = 17,
	CURVE_BRAINPOOLP384R1 = 18,
	CURVE_BRAINPOOLP512R1 = 19,
	CURVE_BRAINPOOLP192T1 = 20,
	CURVE_BRAINPOOLP224T1 = 21,
	CURVE_BRAINPOOLP256T1 = 22,
	CURVE_BRAINPOOLP320R1 = 23,
	CURVE_BRAINPOOLP320T1 = 24,
	CURVE_BRAINPOOLP384T1 = 25,
	CURVE_BRAINPOOLP512T1 = 26,
	CURVE_SM2P256TEST = 27,
	CURVE_SM2P256V1 = 28,
	CURVE_FRP256V1 = 29,
	CURVE_WEI25519 = 30,
	CURVE_WEI448 = 31,
	CURVE_GOST256 = 32,
	CURVE_GOST512 = 33,
	CURVE_GOST_R3410_2012_256_PARAMSETA = 34,
	CURVE_GOST_R3410_2001_TESTPARAMSET = 35,
	CURVE_GOST_R3410_2001_CRYPTOPRO_A_PARAMSET = 36,
	CURVE_GOST_R3410_2001_CRYPTOPRO_B_PARAMSET = 37,
	CURVE_GOST_R3410_2001_CRYPTOPRO_C_PARAMSET = 38,
	CURVE_GOST_R3410_2001_CRYPTOPRO_XCHA_PARAMSET = 39,
	CURVE_GOST_R3410_2001_CRYPTOPRO_XCHB_PARAMSET = 40,
	CURVE_GOST_R3410_2012_256_PARAMSETB = 41,
	CURVE_GOST_R3410_2012_256_PARAMSETC = 42,
	CURVE_GOST_R3410_2012_256_PARAMSETD = 43,
	CURVE_GOST_R3410_2012_512_PARAMSETTEST = 44,
	CURVE_GOST_R3410_2012_512_PARAMSETA = 45,
	CURVE_GOST_R3410_2012_512_PARAMSETB = 46,
	CURVE_GOST_R3410_2012_512_PARAMSETC = 47
};

typedef enum __anonenum_curve_id_8 curve_id;

enum __anonenum__gost94_pub_params_id_9 {
	GOST94_PARAMS_UNKNOWN = 0,
	GOST94_PARAMS_TEST = 1,
	GOST94_PARAMS_CRYPTOPRO_A = 2,
	GOST94_PARAMS_CRYPTOPRO_B = 3,
	GOST94_PARAMS_CRYPTOPRO_C = 4,
	GOST94_PARAMS_CRYPTOPRO_D = 5,
	GOST94_PARAMS_CRYPTOPRO_XCHA = 6,
	GOST94_PARAMS_CRYPTOPRO_XCHB = 7,
	GOST94_PARAMS_CRYPTOPRO_XCHC = 8
};

typedef enum __anonenum__gost94_pub_params_id_9 _gost94_pub_params_id;

struct __anonstruct__hash_alg_10 {
	u8 const *alg_name ;
	u8 const *alg_printable_oid ;
	u8 const *alg_der_oid ;
	u32 const alg_der_oid_len ;
	hash_alg_id hash_id ;
};

typedef struct __anonstruct__hash_alg_10 _hash_alg;

struct __anonstruct__mgf_11 {
	u8 const *alg_name ;
	u8 const *alg_printable_oid ;
	u8 const *alg_der_oid ;
	u32 const alg_der_oid_len ;
	mgf_alg_id const mgf_id ;
};

typedef struct __anonstruct__mgf_11 _mgf;

struct __anonstruct__curve_13 {
	u8 const *crv_name ;
	u8 const *crv_printable_oid ;
	u8 const *crv_der_oid ;
	u32 const crv_der_oid_len ;
	u32 const crv_order_bit_len ;
	curve_id crv_id ;
};

typedef struct __anonstruct__curve_13 _curve;

struct __anonstruct_spki_ecpubkey_params_14 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	int compression ;
	u32 ecc_raw_x_off ;
	u32 ecc_raw_x_len ;
	u32 ecc_raw_y_off ;
	u32 ecc_raw_y_len ;
};

typedef struct __anonstruct_spki_ecpubkey_params_14 spki_ecpubkey_params;

struct __anonstruct_spki_ed25519_params_15 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 ed25519_raw_pub_off ;
	u32 ed25519_raw_pub_len ;
};

typedef struct __anonstruct_spki_ed25519_params_15 spki_ed25519_params;

struct __anonstruct_spki_ed448_params_16 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 ed448_raw_pub_off ;
	u32 ed448_raw_pub_len ;
};

typedef struct __anonstruct_spki_ed448_params_16 spki_ed448_params;

struct __anonstruct_spki_x25519_params_17 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 x25519_raw_pub_off ;
	u32 x25519_raw_pub_len ;
};

typedef struct __anonstruct_spki_x25519_params_17 spki_x25519_params;

struct __anonstruct_spki_x448_params_18 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 x448_raw_pub_off ;
	u32 x448_raw_pub_len ;
};

typedef struct __anonstruct_spki_x448_params_18 spki_x448_params;

struct __anonstruct_spki_rsa_params_19 {
	u32 rsa_advertised_bit_len ;
	u32 rsa_raw_modulus_off ;
	u32 rsa_raw_modulus_len ;
	u32 rsa_raw_pub_exp_off ;
	u32 rsa_raw_pub_exp_len ;
};

typedef struct __anonstruct_spki_rsa_params_19 spki_rsa_params;

struct __anonstruct_spki_dsa_params_20 {
	u32 dsa_raw_pub_off ;
	u32 dsa_raw_pub_len ;
	u32 dsa_raw_p_off ;
	u32 dsa_raw_p_len ;
	u32 dsa_raw_q_off ;
	u32 dsa_raw_q_len ;
	u32 dsa_raw_g_off ;
	u32 dsa_raw_g_len ;
};

typedef struct __anonstruct_spki_dsa_params_20 spki_dsa_params;

struct __anonstruct_spki_gost94_params_21 {
	u32 gost94_raw_pub_off ;
	u32 gost94_raw_pub_len ;
	_gost94_pub_params_id gost94_params_id ;
};

typedef struct __anonstruct_spki_gost94_params_21 spki_gost94_params;

struct __anonstruct_spki_gost2001_params_22 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 gost2001_raw_x_pub_off ;
	u32 gost2001_raw_x_pub_len ;
	u32 gost2001_raw_y_pub_off ;
	u32 gost2001_raw_y_pub_len ;
};

typedef struct __anonstruct_spki_gost2001_params_22 spki_gost2001_params;

struct __anonstruct_spki_gost2012_256_params_23 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 gost2012_256_raw_x_pub_off ;
	u32 gost2012_256_raw_x_pub_len ;
	u32 gost2012_256_raw_y_pub_off ;
	u32 gost2012_256_raw_y_pub_len ;
};

typedef struct __anonstruct_spki_gost2012_256_params_23 spki_gost2012_256_params;

struct __anonstruct_spki_gost2012_512_params_24 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 gost2012_512_raw_x_pub_off ;
	u32 gost2012_512_raw_x_pub_len ;
	u32 gost2012_512_raw_y_pub_off ;
	u32 gost2012_512_raw_y_pub_len ;
};

typedef struct __anonstruct_spki_gost2012_512_params_24 spki_gost2012_512_params;

typedef spki_rsa_params spki_ea_rsa_params;

struct __anonstruct_spki_bign_params_25 {
	curve_id curve ;
	u32 curve_order_bit_len ;
	u32 bign_raw_x_pub_off ;
	u32 bign_raw_x_pub_len ;
	u32 bign_raw_y_pub_off ;
	u32 bign_raw_y_pub_len ;
};

typedef struct __anonstruct_spki_bign_params_25 spki_bign_params;

struct __anonstruct_spki_avest_params_26 {
	u32 avest_raw_pub_off ;
	u32 avest_raw_pub_len ;
};

typedef struct __anonstruct_spki_avest_params_26 spki_avest_params;

union __anonunion_spki_params_27 {
	spki_ecpubkey_params ecpubkey ;
	spki_ed25519_params ed25519 ;
	spki_ed448_params ed448 ;
	spki_x25519_params x25519 ;
	spki_x448_params x448 ;
	spki_rsa_params rsa ;
	spki_ea_rsa_params ea_rsa ;
	spki_dsa_params dsa ;
	spki_gost94_params gost94 ;
	spki_gost2001_params gost2001 ;
	spki_gost2012_256_params gost2012_256 ;
	spki_gost2012_512_params gost2012_512 ;
	spki_bign_params bign ;
	spki_avest_params avest ;
};

typedef union __anonunion_spki_params_27 spki_params;

struct __anonstruct_sig_dsa_params_28 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_dsa_params_28 sig_dsa_params;

struct __anonstruct_sig_rsa_ssa_pss_params_29 {
	u32 sig_raw_off ;
	u32 sig_raw_len ;
	mgf_alg_id mgf_alg ;
	hash_alg_id mgf_hash_alg ;
	u8 salt_len ;
	u8 trailer_field ;
};

typedef struct __anonstruct_sig_rsa_ssa_pss_params_29 sig_rsa_ssa_pss_params;

struct __anonstruct_sig_rsa_pkcs1_v1_5_params_30 {
	u32 sig_raw_off ;
	u32 sig_raw_len ;
};

typedef struct __anonstruct_sig_rsa_pkcs1_v1_5_params_30 sig_rsa_pkcs1_v1_5_params;

typedef sig_rsa_pkcs1_v1_5_params sig_rsa_9796_2_pad_params;

typedef sig_rsa_pkcs1_v1_5_params sig_belgian_rsa_params;

struct __anonstruct_sig_ed25519_params_31 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_ed25519_params_31 sig_ed25519_params;

struct __anonstruct_sig_ed448_params_32 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_ed448_params_32 sig_ed448_params;

struct __anonstruct_sig_sm2_params_33 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_sm2_params_33 sig_sm2_params;

struct __anonstruct_sig_gost_r3410_2012_256_params_34 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_gost_r3410_2012_256_params_34 sig_gost_r3410_2012_256_params;

struct __anonstruct_sig_gost_r3410_2012_512_params_35 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_gost_r3410_2012_512_params_35 sig_gost_r3410_2012_512_params;

struct __anonstruct_sig_gost_r3410_2001_params_36 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_gost_r3410_2001_params_36 sig_gost_r3410_2001_params;

struct __anonstruct_sig_gost_r3410_94_params_37 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_gost_r3410_94_params_37 sig_gost_r3410_94_params;

struct __anonstruct_sig_bign_params_38 {
	u32 sig_raw_off ;
	u32 sig_raw_len ;
};

typedef struct __anonstruct_sig_bign_params_38 sig_bign_params;

struct __anonstruct_sig_ecdsa_params_39 {
	u32 r_raw_off ;
	u32 r_raw_len ;
	u32 s_raw_off ;
	u32 s_raw_len ;
};

typedef struct __anonstruct_sig_ecdsa_params_39 sig_ecdsa_params;

struct __anonstruct_sig_monkeysphere_params_40 {
	u32 sig_raw_off ;
	u32 sig_raw_len ;
};

typedef struct __anonstruct_sig_monkeysphere_params_40 sig_monkeysphere_params;

union __anonunion_sig_params_41 {
	sig_dsa_params dsa ;
	sig_rsa_ssa_pss_params rsa_ssa_pss ;
	sig_rsa_pkcs1_v1_5_params rsa_pkcs1_v1_5 ;
	sig_ed25519_params ed25519 ;
	sig_ed448_params ed448 ;
	sig_sm2_params sm2 ;
	sig_gost_r3410_2012_256_params gost_r3410_2012_256 ;
	sig_gost_r3410_2012_512_params gost_r3410_2012_512 ;
	sig_gost_r3410_2001_params gost_r3410_2001 ;
	sig_gost_r3410_94_params gost_r3410_94 ;
	sig_bign_params bign ;
	sig_ecdsa_params ecdsa ;
	sig_rsa_9796_2_pad_params rsa_9796_2_pad ;
	sig_monkeysphere_params monkeysphere ;
	sig_belgian_rsa_params belgian_rsa ;
};

typedef union __anonunion_sig_params_41 sig_params;

struct __anonstruct__sig_alg_42 {
	u8 const *alg_name ;
	u8 const *alg_printable_oid ;
	u8 const *alg_der_oid ;
	u32 const alg_der_oid_len ;
	sig_alg_id sig_id ;
	hash_alg_id hash_id ;
	int (*parse_algoid_sig_params)(sig_params *params, hash_alg_id *hash_alg,
	u8 const *cert, u32 off, u32 len) ;
	int (*parse_sig)(sig_params *params, u8 const *cert, u32 off, u32 len,
	u32 *eaten) ;
};

typedef struct __anonstruct__sig_alg_42 _sig_alg;

struct __anonstruct__name_oid_43 {
	u8 const *oid ;
	u8 oid_len ;
	int (*parse_rdn_val)(u8 const *buf, u32 len) ;
};

typedef struct __anonstruct__name_oid_43 _name_oid;

struct __anonstruct_crl_parsing_ctx_43 {
	u32 tbs_start ;
	u32 tbs_len ;
	u8 version ;
	u32 tbs_sig_alg_start ;
	u32 tbs_sig_alg_len ;
	u32 tbs_sig_alg_oid_start ;
	u32 tbs_sig_alg_oid_len ;
	u32 tbs_sig_alg_oid_params_start ;
	u32 tbs_sig_alg_oid_params_len ;
	u32 issuer_start ;
	u32 issuer_len ;
	u64 this_update ;
	u64 next_update ;
	int has_aki ;
	int aki_has_keyIdentifier ;
	u32 aki_keyIdentifier_start ;
	u32 aki_keyIdentifier_len ;
	int aki_has_generalNames_and_serial ;
	u32 aki_generalNames_start ;
	u32 aki_generalNames_len ;
	u32 aki_serial_start ;
	u32 aki_serial_len ;
	int has_crldp ;
	int one_crldp_has_all_reasons ;
	int has_crlnumber ;
	u32 crlnumber_start ;
	u32 crlnumber_len ;
	int has_revoked_certs ;
	u32 sig_alg_start ;
	u32 sig_alg_len ;
	sig_alg_id sig_alg ;
	hash_alg_id hash_alg ;
	sig_params sig_alg_params ;
	u32 sig_start ;
	u32 sig_len ;
};

typedef struct __anonstruct_crl_parsing_ctx_43 crl_parsing_ctx;

enum __anonenum_crl_reason_44 {
	crl_reason_unspecified = 0x00,
	crl_reason_keyCompromise = 0x01,
	crl_reason_cACompromise = 0x02,
	crl_reason_affiliationChanged = 0x03,
	crl_reason_superseded = 0x04,
	crl_reason_cessationOfOperation = 0x05,
	crl_reason_certificateHold = 0x06,
	crl_reason_removeFromCRL = 0x08,
	crl_reason_privilegeWithdrawn = 0X09,
	crl_reason_aACompromise = 0x0a
};

typedef enum __anonenum_crl_reason_44 crl_reason;

struct __anonstruct__crl_entry_ext_oid_45 {
	u8 const *oid ;
	u8 oid_len ;
	int (*parse_crl_entry_ext_params)(crl_parsing_ctx *ctx, u8 const *crl,
	u32 off, u32 len, int critical) ;
};

typedef struct __anonstruct__crl_entry_ext_oid_45 _crl_entry_ext_oid;

struct __anonstruct__crl_ext_oid_46 {
	u8 const *oid ;
	u8 oid_len ;
	int (*parse_crl_ext_params)(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
	u32 len, int critical) ;
};

typedef struct __anonstruct__crl_ext_oid_46 _crl_ext_oid;

struct __anonstruct_cert_parsing_ctx_43 {
	u32 tbs_start ;
	u32 tbs_len ;
	u8 version ;
	u32 serial_start ;
	u32 serial_len ;
	u32 tbs_sig_alg_start ;
	u32 tbs_sig_alg_len ;
	u32 tbs_sig_alg_oid_start ;
	u32 tbs_sig_alg_oid_len ;
	u32 tbs_sig_alg_oid_params_start ;
	u32 tbs_sig_alg_oid_params_len ;
	u32 issuer_start ;
	u32 issuer_len ;
	u64 not_before ;
	u64 not_after ;
	u32 subject_start ;
	u32 subject_len ;
	int empty_subject ;
	int subject_issuer_identical ;
	u32 spki_start ;
	u32 spki_len ;
	u32 spki_alg_oid_start ;
	u32 spki_alg_oid_len ;
	u32 spki_alg_oid_params_start ;
	u32 spki_alg_oid_params_len ;
	u32 spki_pub_key_start ;
	u32 spki_pub_key_len ;
	spki_alg_id spki_alg ;
	spki_params spki_alg_params ;
	int has_ski ;
	u32 ski_start ;
	u32 ski_len ;
	int has_aki ;
	int aki_has_keyIdentifier ;
	u32 aki_keyIdentifier_start ;
	u32 aki_keyIdentifier_len ;
	int aki_has_generalNames_and_serial ;
	u32 aki_generalNames_start ;
	u32 aki_generalNames_len ;
	u32 aki_serial_start ;
	u32 aki_serial_len ;
	int has_san ;
	int san_critical ;
	int bc_critical ;
	int ca_true ;
	int pathLenConstraint_set ;
	int has_keyUsage ;
	int keyCertSign_set ;
	int cRLSign_set ;
	int has_eku ;
	int has_crldp ;
	int one_crldp_has_all_reasons ;
	int has_name_constraints ;
	u32 sig_alg_start ;
	u32 sig_alg_len ;
	sig_alg_id sig_alg ;
	hash_alg_id hash_alg ;
	sig_params sig_alg_params ;
	u32 sig_start ;
	u32 sig_len ;
};

typedef struct __anonstruct_cert_parsing_ctx_43 cert_parsing_ctx;

struct __anonstruct__pubkey_alg_44 {
	u8 const *alg_name ;
	u8 const *alg_printable_oid ;
	u8 const *alg_der_oid ;
	u32 const alg_der_oid_len ;
	spki_alg_id pubkey_id ;
	int (*parse_algoid_pubkey_params)(cert_parsing_ctx *ctx, u8 const *cert,
	u32 off, u32 len) ;
	int (*parse_pubkey)(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
	u32 len) ;
};

typedef struct __anonstruct__pubkey_alg_44 _pubkey_alg;

struct __anonstruct__gost94_pub_params_45 {
	u8 const *params_name ;
	u8 const *params_printable_oid ;
	u8 const *params_der_oid ;
	u32 const params_der_oid_len ;
	_gost94_pub_params_id params_id ;
};

typedef struct __anonstruct__gost94_pub_params_45 _gost94_pub_params;

struct __anonstruct__kp_oid_46 {
	u8 const *oid ;
	u8 oid_len ;
};

typedef struct __anonstruct__kp_oid_46 _kp_oid;

struct __anonstruct__ext_oid_47 {
	u8 const *oid ;
	u32 oid_len ;
	int (*parse_ext_params)(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
	u32 len, int critical) ;
};

typedef struct __anonstruct__ext_oid_47 _ext_oid;

int bufs_differ(u8 const *b1, u8 const *b2, u32 n);

/*@
	requires \valid_read(b1 + (0 .. n-1)) || n == 0;
	requires \valid_read(b2 + (0 .. n-1)) || n == 0;
	assigns \nothing;
	ensures \result == 0 || \result == 1;
	ensures \result == 0 ==>(\forall integer k; 0 <= k < n ==> b1[k] == b2[k]);
	ensures \result == 1 ==>(\exists integer k; 0 <= k < n && b1[k] != b2[k]);
*/
int bufs_differ(u8 const *b1, u8 const *b2, u32 n)
{
	int ret = 0;
	u32 i = (u32)0;
	i = (u32)0;
	/*@
		loop invariant 0 <= i <= n;
		loop invariant ret == 0 || ret == 1;
		loop invariant ret == 0 ==> (\forall integer k; 0 <= k < i ==> b1[k] == b2[k]);
		loop invariant ret == 1 ==> (\exists integer k; 0 <= k < i && b1[k] != b2[k]);
		loop assigns i, ret;
		loop variant n - i;
	*/
	while (i < n) {
	/*@ assert rte_4: mem_access: \valid_read(b1 + i); */
	/*@ assert rte_5: mem_access: \valid_read(b2 + i); */
		if ((int)*(b1 + i) != (int)*(b2 + i)) {
			ret = 1;
			break;
		}
		i ++;
	}
	return ret;
}

int parse_sig_ed448(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_ed25519(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_ecdsa(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_sm2(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_dsa(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_rsa_pkcs1_v15(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten);

int parse_sig_rsa_ssa_pss(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten);

int parse_sig_rsa_9796_2_pad(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten);

int parse_sig_rsa_belgian(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten);

int parse_sig_gost94(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_gost2001(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_gost2012_512(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten);

int parse_sig_gost2012_256(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten);

int parse_sig_bign(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_sig_monkey(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten);

int parse_algoid_sig_params_ecdsa_with(sig_params *params,
hash_alg_id *hash_alg, u8 const *cert,
u32 off, u32 len);

int parse_algoid_sig_params_ecdsa_with_specified(sig_params *params,
hash_alg_id *hash_alg,
u8 const *cert, u32 off,
u32 len);

int parse_algoid_sig_params_sm2(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len);

int parse_algoid_sig_params_eddsa(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len);

int parse_algoid_params_rsa(u8 const *cert, u32 off, u32 len);

int parse_algoid_sig_params_rsa(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len);

int parse_algoid_sig_params_rsassa_pss(sig_params *params,
hash_alg_id *hash_alg, u8 const *cert,
u32 off, u32 len);

int parse_algoid_sig_params_none(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len);

int parse_algoid_sig_params_bign_with_hspec(sig_params *params,
hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len);

int parse_algoid_params_none(u8 const *cert, u32 off, u32 len);

_sig_alg const *known_sig_algs[59];

u16 const num_known_sig_algs;

_curve const *known_curves[44];

_sig_alg const *find_sig_alg_by_oid(u8 const *buf, u32 len);

_hash_alg const *find_hash_by_oid(u8 const *buf, u32 len);

_curve const *find_curve_by_oid(u8 const *buf, u32 len);

int get_length(u8 const *buf, u32 len, u32 *adv_len, u32 *eaten);

int parse_id_len(u8 const *buf, u32 len, tag_class exp_class, u32 exp_type,
u32 *parsed, u32 *content_len);

int parse_explicit_id_len(u8 const *buf, u32 len, u32 exp_ext_type,
tag_class exp_int_class, u32 exp_int_type,
u32 *parsed, u32 *data_len);

int parse_null(u8 const *buf, u32 len, u32 *parsed);

int parse_OID(u8 const *buf, u32 len, u32 *parsed);

int parse_integer(u8 const *buf, u32 len, tag_class exp_class, u32 exp_type,
u32 *hdr_len, u32 *data_len);

int parse_non_negative_integer(u8 const *buf, u32 len, tag_class exp_class,
u32 exp_type, u32 *hdr_len, u32 *data_len);

int parse_boolean(u8 const *buf, u32 len, u32 *eaten);

int parse_generalizedTime(u8 const *buf, u32 len, u32 *eaten, u16 *year,
u8 *month, u8 *day, u8 *hour, u8 *min, u8 *sec);

int parse_GeneralName(u8 const *buf, u32 len, u32 *eaten, int *empty);

int parse_SerialNumber(u8 const *cert, u32 off, u32 len, tag_class exp_class,
u32 exp_type, u32 *eaten);

int verify_correct_time_use(u8 time_type, u16 yyyy);

int parse_Time(u8 const *buf, u32 len, u8 *t_type, u32 *eaten, u16 *year,
u8 *month, u8 *day, u8 *hour, u8 *min, u8 *sec);

int parse_AKICertSerialNumber(u8 const *cert, u32 off, u32 len,
tag_class exp_class, u32 exp_type, u32 *eaten);

int parse_crldp_reasons(u8 const *buf, u32 len, u32 exp_type, u32 *eaten);

int parse_DistributionPoint(u8 const *buf, u32 len,
int *crldp_has_all_reasons, u32 *eaten);

int parse_AIA(u8 const *cert, u32 off, u32 len, int critical);

int parse_ia5_string(u8 const *buf, u32 len, u32 lb, u32 ub);

int parse_x509_Name(u8 const *buf, u32 len, u32 *eaten, int *empty);

int parse_DisplayText(u8 const *buf, u32 len, u32 *eaten);

int parse_nine_bit_named_bit_list(u8 const *buf, u32 len, u16 *val);

int parse_GeneralNames(u8 const *buf, u32 len, tag_class exp_class,
u32 exp_type, u32 *eaten);

u64 time_components_to_comparable_u64(u16 na_year, u8 na_month, u8 na_day,
u8 na_hour, u8 na_min, u8 na_sec);

static u8 const _md2_hash_name[4] = {(u8)'M', (u8)'D', (u8)'2', (u8)'\000'};

static u8 const _md2_hash_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _md2_hash_der_oid[10] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x02,
(u8)0x02};

static _hash_alg const _md2_hash_alg =
{.alg_name = _md2_hash_name,
	.alg_printable_oid = _md2_hash_printable_oid,
	.alg_der_oid = _md2_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_md2_hash_der_oid),
.hash_id = HASH_ALG_MD2};

static u8 const _md4_hash_name[4] = {(u8)'M', (u8)'D', (u8)'4', (u8)'\000'};

static u8 const _md4_hash_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _md4_hash_der_oid[10] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x02,
(u8)0x04};

static _hash_alg const _md4_hash_alg =
{.alg_name = _md4_hash_name,
	.alg_printable_oid = _md4_hash_printable_oid,
	.alg_der_oid = _md4_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_md4_hash_der_oid),
.hash_id = HASH_ALG_MD4};

static u8 const _md5_hash_name[4] = {(u8)'M', (u8)'D', (u8)'5', (u8)'\000'};

static u8 const _md5_hash_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _md5_hash_der_oid[10] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x02,
(u8)0x05};

static _hash_alg const _md5_hash_alg =
{.alg_name = _md5_hash_name,
	.alg_printable_oid = _md5_hash_printable_oid,
	.alg_der_oid = _md5_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_md5_hash_der_oid),
.hash_id = HASH_ALG_MD5};

static u8 const _mdc2_hash_name[5] =
{(u8)'M', (u8)'D', (u8)'C', (u8)'2', (u8)'\000'};

static u8 const _mdc2_hash_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'9',
(u8)'\000'};

static u8 const _mdc2_hash_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x13};

static _hash_alg const _mdc2_hash_alg =
{.alg_name = _mdc2_hash_name,
	.alg_printable_oid = _mdc2_hash_printable_oid,
	.alg_der_oid = _mdc2_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_mdc2_hash_der_oid),
.hash_id = HASH_ALG_MDC2};

static u8 const _sha1_hash_name[5] =
{(u8)'S', (u8)'H', (u8)'A', (u8)'1', (u8)'\000'};

static u8 const _sha1_hash_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'6',
(u8)'\000'};

static u8 const _sha1_hash_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x1a};

static _hash_alg const _sha1_hash_alg =
{.alg_name = _sha1_hash_name,
	.alg_printable_oid = _sha1_hash_printable_oid,
	.alg_der_oid = _sha1_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha1_hash_der_oid),
.hash_id = HASH_ALG_SHA1};

static u8 const _ripemd160_hash_name[10] =
{(u8)'R',
	(u8)'I',
	(u8)'P',
	(u8)'E',
	(u8)'M',
	(u8)'D',
	(u8)'1',
	(u8)'6',
	(u8)'0',
(u8)'\000'};

static u8 const _ripemd160_hash_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _ripemd160_hash_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x24, (u8)0x03, (u8)0x02, (u8)0x01};

static _hash_alg const _ripemd160_hash_alg =
{.alg_name = _ripemd160_hash_name,
	.alg_printable_oid = _ripemd160_hash_printable_oid,
	.alg_der_oid = _ripemd160_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ripemd160_hash_der_oid),
.hash_id = HASH_ALG_RIPEMD160};

static u8 const _ripemd160_iso_hash_name[10] =
{(u8)'R',
	(u8)'I',
	(u8)'P',
	(u8)'E',
	(u8)'M',
	(u8)'D',
	(u8)'1',
	(u8)'6',
	(u8)'0',
(u8)'\000'};

static u8 const _ripemd160_iso_hash_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'9',
(u8)'\000'};

static u8 const _ripemd160_iso_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x28,
	(u8)0xCF,
	(u8)0x06,
	(u8)0x03,
	(u8)0x00,
(u8)0x31};

static _hash_alg const _ripemd160_iso_hash_alg =
{.alg_name = _ripemd160_iso_hash_name,
	.alg_printable_oid = _ripemd160_iso_hash_printable_oid,
	.alg_der_oid = _ripemd160_iso_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ripemd160_iso_hash_der_oid),
.hash_id = HASH_ALG_RIPEMD160};

static u8 const _ripemd128_hash_name[10] =
{(u8)'R',
	(u8)'I',
	(u8)'P',
	(u8)'E',
	(u8)'M',
	(u8)'D',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _ripemd128_hash_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _ripemd128_hash_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x24, (u8)0x03, (u8)0x02, (u8)0x02};

static _hash_alg const _ripemd128_hash_alg =
{.alg_name = _ripemd128_hash_name,
	.alg_printable_oid = _ripemd128_hash_printable_oid,
	.alg_der_oid = _ripemd128_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ripemd128_hash_der_oid),
.hash_id = HASH_ALG_RIPEMD128};

static u8 const _ripemd128_iso_hash_name[10] =
{(u8)'R',
	(u8)'I',
	(u8)'P',
	(u8)'E',
	(u8)'M',
	(u8)'D',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _ripemd128_iso_hash_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'5',
	(u8)'0',
(u8)'\000'};

static u8 const _ripemd128_iso_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x28,
	(u8)0xCF,
	(u8)0x06,
	(u8)0x03,
	(u8)0x00,
(u8)0x32};

static _hash_alg const _ripemd128_iso_hash_alg =
{.alg_name = _ripemd128_iso_hash_name,
	.alg_printable_oid = _ripemd128_iso_hash_printable_oid,
	.alg_der_oid = _ripemd128_iso_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ripemd128_iso_hash_der_oid),
.hash_id = HASH_ALG_RIPEMD128};

static u8 const _ripemd256_hash_name[10] =
{(u8)'R',
	(u8)'I',
	(u8)'P',
	(u8)'E',
	(u8)'M',
	(u8)'D',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _ripemd256_hash_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _ripemd256_hash_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x24, (u8)0x03, (u8)0x02, (u8)0x03};

static _hash_alg const _ripemd256_hash_alg =
{.alg_name = _ripemd256_hash_name,
	.alg_printable_oid = _ripemd256_hash_printable_oid,
	.alg_der_oid = _ripemd256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ripemd256_hash_der_oid),
.hash_id = HASH_ALG_RIPEMD256};

static u8 const _sha224_hash_name[7] =
{(u8)'S', (u8)'H', (u8)'A', (u8)'2', (u8)'2', (u8)'4', (u8)'\000'};

static u8 const _sha224_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _sha224_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x04};

static _hash_alg const _sha224_hash_alg =
{.alg_name = _sha224_hash_name,
	.alg_printable_oid = _sha224_hash_printable_oid,
	.alg_der_oid = _sha224_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha224_hash_der_oid),
.hash_id = HASH_ALG_SHA224};

static u8 const _sha256_hash_name[7] =
{(u8)'S', (u8)'H', (u8)'A', (u8)'2', (u8)'5', (u8)'6', (u8)'\000'};

static u8 const _sha256_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _sha256_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x01};

static _hash_alg const _sha256_hash_alg =
{.alg_name = _sha256_hash_name,
	.alg_printable_oid = _sha256_hash_printable_oid,
	.alg_der_oid = _sha256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha256_hash_der_oid),
.hash_id = HASH_ALG_SHA256};

static u8 const _sha384_hash_name[7] =
{(u8)'S', (u8)'H', (u8)'A', (u8)'3', (u8)'8', (u8)'4', (u8)'\000'};

static u8 const _sha384_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _sha384_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x02};

static _hash_alg const _sha384_hash_alg =
{.alg_name = _sha384_hash_name,
	.alg_printable_oid = _sha384_hash_printable_oid,
	.alg_der_oid = _sha384_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha384_hash_der_oid),
.hash_id = HASH_ALG_SHA384};

static u8 const _sha512_hash_name[7] =
{(u8)'S', (u8)'H', (u8)'A', (u8)'5', (u8)'1', (u8)'2', (u8)'\000'};

static u8 const _sha512_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _sha512_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x03};

static _hash_alg const _sha512_hash_alg =
{.alg_name = _sha512_hash_name,
	.alg_printable_oid = _sha512_hash_printable_oid,
	.alg_der_oid = _sha512_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha512_hash_der_oid),
.hash_id = HASH_ALG_SHA512};

static u8 const _sha512_224_hash_name[11] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'2',
	(u8)'2',
	(u8)'4',
(u8)'\000'};

static u8 const _sha512_224_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _sha512_224_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x05};

static _hash_alg const _sha512_224_hash_alg =
{.alg_name = _sha512_224_hash_name,
	.alg_printable_oid = _sha512_224_hash_printable_oid,
	.alg_der_oid = _sha512_224_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha512_224_hash_der_oid),
.hash_id = HASH_ALG_SHA512_224};

static u8 const _sha512_256_hash_name[11] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _sha512_256_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
(u8)'\000'};

static u8 const _sha512_256_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x06};

static _hash_alg const _sha512_256_hash_alg =
{.alg_name = _sha512_256_hash_name,
	.alg_printable_oid = _sha512_256_hash_printable_oid,
	.alg_der_oid = _sha512_256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha512_256_hash_der_oid),
.hash_id = HASH_ALG_SHA512_256};

static u8 const _sha3_224_hash_name[9] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'3',
	(u8)'_',
	(u8)'2',
	(u8)'2',
	(u8)'4',
(u8)'\000'};

static u8 const _sha3_224_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'7',
(u8)'\000'};

static u8 const _sha3_224_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x07};

static _hash_alg const _sha3_224_hash_alg =
{.alg_name = _sha3_224_hash_name,
	.alg_printable_oid = _sha3_224_hash_printable_oid,
	.alg_der_oid = _sha3_224_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha3_224_hash_der_oid),
.hash_id = HASH_ALG_SHA3_224};

static u8 const _sha3_256_hash_name[9] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'3',
	(u8)'_',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _sha3_256_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
(u8)'\000'};

static u8 const _sha3_256_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x08};

static _hash_alg const _sha3_256_hash_alg =
{.alg_name = _sha3_256_hash_name,
	.alg_printable_oid = _sha3_256_hash_printable_oid,
	.alg_der_oid = _sha3_256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha3_256_hash_der_oid),
.hash_id = HASH_ALG_SHA3_256};

static u8 const _sha3_384_hash_name[9] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'3',
	(u8)'_',
	(u8)'3',
	(u8)'8',
	(u8)'4',
(u8)'\000'};

static u8 const _sha3_384_hash_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'9',
(u8)'\000'};

static u8 const _sha3_384_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x09};

static _hash_alg const _sha3_384_hash_alg =
{.alg_name = _sha3_384_hash_name,
	.alg_printable_oid = _sha3_384_hash_printable_oid,
	.alg_der_oid = _sha3_384_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha3_384_hash_der_oid),
.hash_id = HASH_ALG_SHA3_384};

static u8 const _sha3_512_hash_name[9] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'3',
	(u8)'_',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _sha3_512_hash_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'0',
(u8)'\000'};

static u8 const _sha3_512_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x0a};

static _hash_alg const _sha3_512_hash_alg =
{.alg_name = _sha3_512_hash_name,
	.alg_printable_oid = _sha3_512_hash_printable_oid,
	.alg_der_oid = _sha3_512_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha3_512_hash_der_oid),
.hash_id = HASH_ALG_SHA3_512};

static u8 const _shake128_hash_name[9] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'K',
	(u8)'E',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _shake128_hash_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _shake128_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x0b};

static _hash_alg const _shake128_hash_alg =
{.alg_name = _shake128_hash_name,
	.alg_printable_oid = _shake128_hash_printable_oid,
	.alg_der_oid = _shake128_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_shake128_hash_der_oid),
.hash_id = HASH_ALG_SHAKE128};

static u8 const _shake256_hash_name[9] =
{(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'K',
	(u8)'E',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _shake256_hash_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _shake256_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x02,
(u8)0x0c};

static _hash_alg const _shake256_hash_alg =
{.alg_name = _shake256_hash_name,
	.alg_printable_oid = _shake256_hash_printable_oid,
	.alg_der_oid = _shake256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_shake256_hash_der_oid),
.hash_id = HASH_ALG_SHAKE256};

static u8 const _hbelt_hash_name[6] =
{(u8)'H', (u8)'B', (u8)'E', (u8)'L', (u8)'T', (u8)'\000'};

static u8 const _hbelt_hash_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'1',
	(u8)'.',
	(u8)'8',
	(u8)'1',
(u8)'\000'};

static u8 const _hbelt_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x1F,
(u8)0x51};

static _hash_alg const _hbelt_hash_alg =
{.alg_name = _hbelt_hash_name,
	.alg_printable_oid = _hbelt_hash_printable_oid,
	.alg_der_oid = _hbelt_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_hbelt_hash_der_oid),
.hash_id = HASH_ALG_HBELT};

static u8 const _bash256_hash_name[8] =
{(u8)'B', (u8)'A', (u8)'S', (u8)'H', (u8)'2', (u8)'5', (u8)'6', (u8)'\000'};

static u8 const _bash256_hash_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'7',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _bash256_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x4D,
(u8)0x0B};

static _hash_alg const _bash256_hash_alg =
{.alg_name = _bash256_hash_name,
	.alg_printable_oid = _bash256_hash_printable_oid,
	.alg_der_oid = _bash256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bash256_hash_der_oid),
.hash_id = HASH_ALG_BASH256};

static u8 const _bash384_hash_name[8] =
{(u8)'B', (u8)'A', (u8)'S', (u8)'H', (u8)'3', (u8)'8', (u8)'4', (u8)'\000'};

static u8 const _bash384_hash_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'7',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _bash384_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x4D,
(u8)0x0C};

static _hash_alg const _bash384_hash_alg =
{.alg_name = _bash384_hash_name,
	.alg_printable_oid = _bash384_hash_printable_oid,
	.alg_der_oid = _bash384_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bash384_hash_der_oid),
.hash_id = HASH_ALG_BASH384};

static u8 const _bash512_hash_name[8] =
{(u8)'B', (u8)'A', (u8)'S', (u8)'H', (u8)'5', (u8)'1', (u8)'2', (u8)'\000'};

static u8 const _bash512_hash_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'7',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _bash512_hash_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x4D,
(u8)0x0D};

static _hash_alg const _bash512_hash_alg =
{.alg_name = _bash512_hash_name,
	.alg_printable_oid = _bash512_hash_printable_oid,
	.alg_der_oid = _bash512_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bash512_hash_der_oid),
.hash_id = HASH_ALG_BASH512};

static u8 const _whirlpool_hash_name[10] =
{(u8)'W',
	(u8)'H',
	(u8)'I',
	(u8)'R',
	(u8)'L',
	(u8)'P',
	(u8)'O',
	(u8)'O',
	(u8)'L',
(u8)'\000'};

static u8 const _whirlpool_hash_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'5',
	(u8)'5',
(u8)'\000'};

static u8 const _whirlpool_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x28,
	(u8)0xCF,
	(u8)0x06,
	(u8)0x03,
	(u8)0x00,
(u8)0x37};

static _hash_alg const _whirlpool_hash_alg =
{.alg_name = _whirlpool_hash_name,
	.alg_printable_oid = _whirlpool_hash_printable_oid,
	.alg_der_oid = _whirlpool_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_whirlpool_hash_der_oid),
.hash_id = HASH_ALG_WHIRLPOOL};

static u8 const _streebog256_hash_name[12] =
{(u8)'S',
	(u8)'T',
	(u8)'R',
	(u8)'E',
	(u8)'E',
	(u8)'B',
	(u8)'O',
	(u8)'G',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _streebog256_hash_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _streebog256_hash_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x01,
	(u8)0x02,
(u8)0x02};

static _hash_alg const _streebog256_hash_alg =
{.alg_name = _streebog256_hash_name,
	.alg_printable_oid = _streebog256_hash_printable_oid,
	.alg_der_oid = _streebog256_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_streebog256_hash_der_oid),
.hash_id = HASH_ALG_STREEBOG256};

static u8 const _streebog256_bis_hash_name[12] =
{(u8)'S',
	(u8)'T',
	(u8)'R',
	(u8)'E',
	(u8)'E',
	(u8)'B',
	(u8)'O',
	(u8)'G',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _streebog256_bis_hash_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'0',
(u8)'\000'};

static u8 const _streebog256_bis_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x28,
	(u8)0xCF,
	(u8)0x06,
	(u8)0x03,
	(u8)0x00,
(u8)0x3c};

static _hash_alg const _streebog256_bis_hash_alg =
{.alg_name = _streebog256_bis_hash_name,
	.alg_printable_oid = _streebog256_bis_hash_printable_oid,
	.alg_der_oid = _streebog256_bis_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_streebog256_bis_hash_der_oid),
.hash_id = HASH_ALG_STREEBOG256};

static u8 const _streebog512_hash_name[12] =
{(u8)'S',
	(u8)'T',
	(u8)'R',
	(u8)'E',
	(u8)'E',
	(u8)'B',
	(u8)'O',
	(u8)'G',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _streebog512_hash_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _streebog512_hash_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x01,
	(u8)0x02,
(u8)0x03};

static _hash_alg const _streebog512_hash_alg =
{.alg_name = _streebog512_hash_name,
	.alg_printable_oid = _streebog512_hash_printable_oid,
	.alg_der_oid = _streebog512_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_streebog512_hash_der_oid),
.hash_id = HASH_ALG_STREEBOG512};

static u8 const _streebog512_bis_hash_name[12] =
{(u8)'S',
	(u8)'T',
	(u8)'R',
	(u8)'E',
	(u8)'E',
	(u8)'B',
	(u8)'O',
	(u8)'G',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _streebog512_bis_hash_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'5',
	(u8)'9',
(u8)'\000'};

static u8 const _streebog512_bis_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x28,
	(u8)0xCF,
	(u8)0x06,
	(u8)0x03,
	(u8)0x00,
(u8)0x3b};

static _hash_alg const _streebog512_bis_hash_alg =
{.alg_name = _streebog512_bis_hash_name,
	.alg_printable_oid = _streebog512_bis_hash_printable_oid,
	.alg_der_oid = _streebog512_bis_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_streebog512_bis_hash_der_oid),
.hash_id = HASH_ALG_STREEBOG512};

static u8 const _sm3_hash_name[4] = {(u8)'S', (u8)'M', (u8)'3', (u8)'\000'};

static u8 const _sm3_hash_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'5',
(u8)'\000'};

static u8 const _sm3_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x28,
	(u8)0xCF,
	(u8)0x06,
	(u8)0x03,
	(u8)0x00,
(u8)0x41};

static _hash_alg const _sm3_hash_alg =
{.alg_name = _sm3_hash_name,
	.alg_printable_oid = _sm3_hash_printable_oid,
	.alg_der_oid = _sm3_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sm3_hash_der_oid),
.hash_id = HASH_ALG_SM3};

static u8 const _gostR3411_94_hash_name[16] =
{(u8)'G',
	(u8)'O',
	(u8)'S',
	(u8)'T',
	(u8)' ',
	(u8)'R',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'-',
	(u8)'9',
	(u8)'4',
(u8)'\000'};

static u8 const _gostR3411_94_hash_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'9',
(u8)'\000'};

static u8 const _gostR3411_94_hash_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
(u8)0x09};

static _hash_alg const _gostR3411_94_hash_alg =
{.alg_name = _gostR3411_94_hash_name,
	.alg_printable_oid = _gostR3411_94_hash_printable_oid,
	.alg_der_oid = _gostR3411_94_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gostR3411_94_hash_der_oid),
.hash_id = HASH_ALG_GOSTR3411_94};

static u8 const _gostR3411_94_bis_hash_name[16] =
{(u8)'G',
	(u8)'O',
	(u8)'S',
	(u8)'T',
	(u8)' ',
	(u8)'R',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'-',
	(u8)'9',
	(u8)'4',
(u8)'\000'};

static u8 const _gostR3411_94_bis_hash_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'0',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _gostR3411_94_bis_hash_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x1e,
(u8)0x01};

static _hash_alg const _gostR3411_94_bis_hash_alg =
{.alg_name = _gostR3411_94_bis_hash_name,
	.alg_printable_oid = _gostR3411_94_bis_hash_printable_oid,
	.alg_der_oid = _gostR3411_94_bis_hash_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gostR3411_94_bis_hash_der_oid),
.hash_id = HASH_ALG_GOSTR3411_94};

_hash_alg const *known_hashes[34] =
{& _md2_hash_alg,
	& _md4_hash_alg,
	& _md5_hash_alg,
	& _mdc2_hash_alg,
	& _sha1_hash_alg,
	& _ripemd160_hash_alg,
	& _ripemd160_iso_hash_alg,
	& _ripemd128_hash_alg,
	& _ripemd128_iso_hash_alg,
	& _ripemd256_hash_alg,
	& _sha224_hash_alg,
	& _sha256_hash_alg,
	& _sha384_hash_alg,
	& _sha512_hash_alg,
	& _sha512_224_hash_alg,
	& _sha512_256_hash_alg,
	& _sha3_224_hash_alg,
	& _sha3_256_hash_alg,
	& _sha3_384_hash_alg,
	& _sha3_512_hash_alg,
	& _shake128_hash_alg,
	& _shake256_hash_alg,
	& _hbelt_hash_alg,
	& _bash256_hash_alg,
	& _bash384_hash_alg,
	& _bash512_hash_alg,
	& _whirlpool_hash_alg,
	& _gostR3411_94_hash_alg,
	& _gostR3411_94_bis_hash_alg,
	& _streebog256_hash_alg,
	& _streebog256_bis_hash_alg,
	& _streebog512_hash_alg,
	& _streebog512_bis_hash_alg,
& _sm3_hash_alg};

static u8 const _mgf_alg_mgf1_name[5] =
{(u8)'M', (u8)'G', (u8)'F', (u8)'1', (u8)'\000'};

static u8 const _mgf_alg_mgf1_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'8',
(u8)'\000'};

static u8 const _mgf_alg_mgf1_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x08};

static _mgf const _mgf1_alg =
{.alg_name = _mgf_alg_mgf1_name,
	.alg_printable_oid = _mgf_alg_mgf1_printable_oid,
	.alg_der_oid = _mgf_alg_mgf1_der_oid,
	.alg_der_oid_len = (u32)sizeof(_mgf_alg_mgf1_der_oid),
.mgf_id = MGF_ALG_MGF1};

static u8 const _Curve25519_curve_name[11] =
{(u8)'C',
	(u8)'u',
	(u8)'r',
	(u8)'v',
	(u8)'e',
	(u8)'2',
	(u8)'5',
	(u8)'5',
	(u8)'1',
	(u8)'9',
(u8)'\000'};

static u8 const _Curve25519_curve_printable_oid[23] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'5',
	(u8)'9',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'5',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _Curve25519_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2B,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0xDA,
	(u8)0x47,
	(u8)0x0F,
(u8)0x01};

static _curve const _curve_Curve25519 =
{.crv_name = _Curve25519_curve_name,
	.crv_printable_oid = _Curve25519_curve_printable_oid,
	.crv_der_oid = _Curve25519_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_Curve25519_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_WEI25519};

static u8 const _Curve448_curve_name[9] =
{(u8)'C',
	(u8)'u',
	(u8)'r',
	(u8)'v',
	(u8)'e',
	(u8)'4',
	(u8)'4',
	(u8)'8',
(u8)'\000'};

static u8 const _Curve448_curve_printable_oid[23] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'5',
	(u8)'9',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'5',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _Curve448_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2B,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0xDA,
	(u8)0x47,
	(u8)0x0F,
(u8)0x02};

static _curve const _curve_Curve448 =
{.crv_name = _Curve448_curve_name,
	.crv_printable_oid = _Curve448_curve_printable_oid,
	.crv_der_oid = _Curve448_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_Curve448_curve_der_oid),
	.crv_order_bit_len = (u32)448,
.crv_id = CURVE_WEI448};

static u8 const _bign_curve256v1_curve_name[16] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'-',
	(u8)'c',
	(u8)'u',
	(u8)'r',
	(u8)'v',
	(u8)'e',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _bign_curve256v1_curve_printable_oid[28] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _bign_curve256v1_curve_der_oid[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
	(u8)0x03,
(u8)0x01};

static _curve const _curve_bign_curve256v1 =
{.crv_name = _bign_curve256v1_curve_name,
	.crv_printable_oid = _bign_curve256v1_curve_printable_oid,
	.crv_der_oid = _bign_curve256v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_bign_curve256v1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_BIGN256v1};

static u8 const _bign_curve384v1_curve_name[16] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'-',
	(u8)'c',
	(u8)'u',
	(u8)'r',
	(u8)'v',
	(u8)'e',
	(u8)'3',
	(u8)'8',
	(u8)'4',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _bign_curve384v1_curve_printable_oid[28] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _bign_curve384v1_curve_der_oid[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
	(u8)0x03,
(u8)0x02};

static _curve const _curve_bign_curve384v1 =
{.crv_name = _bign_curve384v1_curve_name,
	.crv_printable_oid = _bign_curve384v1_curve_printable_oid,
	.crv_der_oid = _bign_curve384v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_bign_curve384v1_curve_der_oid),
	.crv_order_bit_len = (u32)384,
.crv_id = CURVE_BIGN384v1};

static u8 const _bign_curve512v1_curve_name[16] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'-',
	(u8)'c',
	(u8)'u',
	(u8)'r',
	(u8)'v',
	(u8)'e',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _bign_curve512v1_curve_printable_oid[28] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _bign_curve512v1_curve_der_oid[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
	(u8)0x03,
(u8)0x03};

static _curve const _curve_bign_curve512v1 =
{.crv_name = _bign_curve512v1_curve_name,
	.crv_printable_oid = _bign_curve512v1_curve_printable_oid,
	.crv_der_oid = _bign_curve512v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_bign_curve512v1_curve_der_oid),
	.crv_order_bit_len = (u32)512,
.crv_id = CURVE_BIGN512v1};

static u8 const _prime192v1_curve_name[11] =
{(u8)'p',
	(u8)'r',
	(u8)'i',
	(u8)'m',
	(u8)'e',
	(u8)'1',
	(u8)'9',
	(u8)'2',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _prime192v1_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _prime192v1_curve_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x03,
	(u8)0x01,
(u8)0x01};

static _curve const _curve_prime192v1 =
{.crv_name = _prime192v1_curve_name,
	.crv_printable_oid = _prime192v1_curve_printable_oid,
	.crv_der_oid = _prime192v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_prime192v1_curve_der_oid),
	.crv_order_bit_len = (u32)192,
.crv_id = CURVE_SECP192R1};

static u8 const _c2pnb163v1_curve_name[11] =
{(u8)'c',
	(u8)'2',
	(u8)'p',
	(u8)'n',
	(u8)'b',
	(u8)'1',
	(u8)'6',
	(u8)'3',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _c2pnb163v1_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _c2pnb163v1_curve_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x03,
	(u8)0x00,
(u8)0x01};

static _curve const _curve_c2pnb163v1 =
{.crv_name = _c2pnb163v1_curve_name,
	.crv_printable_oid = _c2pnb163v1_curve_printable_oid,
	.crv_der_oid = _c2pnb163v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_c2pnb163v1_curve_der_oid),
	.crv_order_bit_len = (u32)163,
.crv_id = CURVE_C2PNB163V1};

static u8 const _sect571k1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'t',
	(u8)'5',
	(u8)'7',
	(u8)'1',
	(u8)'k',
	(u8)'1',
(u8)'\000'};

static u8 const _sect571k1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'8',
(u8)'\000'};

static u8 const _sect571k1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x26};

static _curve const _curve_sect571k1 =
{.crv_name = _sect571k1_curve_name,
	.crv_printable_oid = _sect571k1_curve_printable_oid,
	.crv_der_oid = _sect571k1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_sect571k1_curve_der_oid),
	.crv_order_bit_len = (u32)571,
.crv_id = CURVE_SECT571K1};

static u8 const _sect163k1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'t',
	(u8)'1',
	(u8)'6',
	(u8)'3',
	(u8)'k',
	(u8)'1',
(u8)'\000'};

static u8 const _sect163k1_curve_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _sect163k1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x01};

static _curve const _curve_sect163k1 =
{.crv_name = _sect163k1_curve_name,
	.crv_printable_oid = _sect163k1_curve_printable_oid,
	.crv_der_oid = _sect163k1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_sect163k1_curve_der_oid),
	.crv_order_bit_len = (u32)163,
.crv_id = CURVE_SECT163K1};

static u8 const _secp192k1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'1',
	(u8)'9',
	(u8)'2',
	(u8)'k',
	(u8)'1',
(u8)'\000'};

static u8 const _secp192k1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'1',
(u8)'\000'};

static u8 const _secp192k1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x1f};

static _curve const _curve_secp192k1 =
{.crv_name = _secp192k1_curve_name,
	.crv_printable_oid = _secp192k1_curve_printable_oid,
	.crv_der_oid = _secp192k1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp192k1_curve_der_oid),
	.crv_order_bit_len = (u32)192,
.crv_id = CURVE_SECP192K1};

static u8 const _secp224k1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'2',
	(u8)'2',
	(u8)'4',
	(u8)'k',
	(u8)'1',
(u8)'\000'};

static u8 const _secp224k1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'2',
(u8)'\000'};

static u8 const _secp224k1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x20};

static _curve const _curve_secp224k1 =
{.crv_name = _secp224k1_curve_name,
	.crv_printable_oid = _secp224k1_curve_printable_oid,
	.crv_der_oid = _secp224k1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp224k1_curve_der_oid),
	.crv_order_bit_len = (u32)224,
.crv_id = CURVE_SECP224K1};

static u8 const _secp256k1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'k',
	(u8)'1',
(u8)'\000'};

static u8 const _secp256k1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
(u8)'\000'};

static u8 const _secp256k1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x0a};

static _curve const _curve_secp256k1 =
{.crv_name = _secp256k1_curve_name,
	.crv_printable_oid = _secp256k1_curve_printable_oid,
	.crv_der_oid = _secp256k1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp256k1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_SECP256K1};

static u8 const _secp224r1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'2',
	(u8)'2',
	(u8)'4',
	(u8)'r',
	(u8)'1',
(u8)'\000'};

static u8 const _secp224r1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'3',
(u8)'\000'};

static u8 const _secp224r1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x21};

static _curve const _curve_secp224r1 =
{.crv_name = _secp224r1_curve_name,
	.crv_printable_oid = _secp224r1_curve_printable_oid,
	.crv_der_oid = _secp224r1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp224r1_curve_der_oid),
	.crv_order_bit_len = (u32)224,
.crv_id = CURVE_SECP224R1};

static u8 const _secp256r1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'r',
	(u8)'1',
(u8)'\000'};

static u8 const _secp256r1_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'7',
(u8)'\000'};

static u8 const _secp256r1_curve_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x03,
	(u8)0x01,
(u8)0x07};

static _curve const _curve_secp256r1 =
{.crv_name = _secp256r1_curve_name,
	.crv_printable_oid = _secp256r1_curve_printable_oid,
	.crv_der_oid = _secp256r1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp256r1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_SECP256R1};

static u8 const _secp384r1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'3',
	(u8)'8',
	(u8)'4',
	(u8)'r',
	(u8)'1',
(u8)'\000'};

static u8 const _secp384r1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
(u8)'\000'};

static u8 const _secp384r1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x22};

static _curve const _curve_secp384r1 =
{.crv_name = _secp384r1_curve_name,
	.crv_printable_oid = _secp384r1_curve_printable_oid,
	.crv_der_oid = _secp384r1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp384r1_curve_der_oid),
	.crv_order_bit_len = (u32)384,
.crv_id = CURVE_SECP384R1};

static u8 const _secp521r1_curve_name[10] =
{(u8)'s',
	(u8)'e',
	(u8)'c',
	(u8)'p',
	(u8)'5',
	(u8)'2',
	(u8)'1',
	(u8)'r',
	(u8)'1',
(u8)'\000'};

static u8 const _secp521r1_curve_printable_oid[13] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'5',
(u8)'\000'};

static u8 const _secp521r1_curve_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x81, (u8)0x04, (u8)0x00, (u8)0x23};

static _curve const _curve_secp521r1 =
{.crv_name = _secp521r1_curve_name,
	.crv_printable_oid = _secp521r1_curve_printable_oid,
	.crv_der_oid = _secp521r1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_secp521r1_curve_der_oid),
	.crv_order_bit_len = (u32)521,
.crv_id = CURVE_SECP521R1};

static u8 const _brainpoolP192R1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'1',
	(u8)'9',
	(u8)'2',
	(u8)'R',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP192R1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _brainpoolP192R1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x03};

static _curve const _curve_brainpoolP192R1 =
{.crv_name = _brainpoolP192R1_curve_name,
	.crv_printable_oid = _brainpoolP192R1_curve_printable_oid,
	.crv_der_oid = _brainpoolP192R1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP192R1_curve_der_oid),
	.crv_order_bit_len = (u32)192,
.crv_id = CURVE_BRAINPOOLP192R1};

static u8 const _brainpoolP224R1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'2',
	(u8)'2',
	(u8)'4',
	(u8)'R',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP224R1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _brainpoolP224R1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x05};

static _curve const _curve_brainpoolP224R1 =
{.crv_name = _brainpoolP224R1_curve_name,
	.crv_printable_oid = _brainpoolP224R1_curve_printable_oid,
	.crv_der_oid = _brainpoolP224R1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP224R1_curve_der_oid),
	.crv_order_bit_len = (u32)224,
.crv_id = CURVE_BRAINPOOLP224R1};

static u8 const _brainpoolP256R1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'R',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP256R1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'7',
(u8)'\000'};

static u8 const _brainpoolP256R1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x07};

static _curve const _curve_brainpoolP256R1 =
{.crv_name = _brainpoolP256R1_curve_name,
	.crv_printable_oid = _brainpoolP256R1_curve_printable_oid,
	.crv_der_oid = _brainpoolP256R1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP256R1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_BRAINPOOLP256R1};

static u8 const _brainpoolP320R1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'3',
	(u8)'2',
	(u8)'0',
	(u8)'R',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP320R1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'9',
(u8)'\000'};

static u8 const _brainpoolP320R1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x09};

static _curve const _curve_brainpoolP320R1 =
{.crv_name = _brainpoolP320R1_curve_name,
	.crv_printable_oid = _brainpoolP320R1_curve_printable_oid,
	.crv_der_oid = _brainpoolP320R1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP320R1_curve_der_oid),
	.crv_order_bit_len = (u32)320,
.crv_id = CURVE_BRAINPOOLP320R1};

static u8 const _brainpoolP384R1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'3',
	(u8)'8',
	(u8)'4',
	(u8)'R',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP384R1_curve_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP384R1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x0b};

static _curve const _curve_brainpoolP384R1 =
{.crv_name = _brainpoolP384R1_curve_name,
	.crv_printable_oid = _brainpoolP384R1_curve_printable_oid,
	.crv_der_oid = _brainpoolP384R1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP384R1_curve_der_oid),
	.crv_order_bit_len = (u32)384,
.crv_id = CURVE_BRAINPOOLP384R1};

static u8 const _brainpoolP512R1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'R',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP512R1_curve_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _brainpoolP512R1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x0d};

static _curve const _curve_brainpoolP512R1 =
{.crv_name = _brainpoolP512R1_curve_name,
	.crv_printable_oid = _brainpoolP512R1_curve_printable_oid,
	.crv_der_oid = _brainpoolP512R1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP512R1_curve_der_oid),
	.crv_order_bit_len = (u32)512,
.crv_id = CURVE_BRAINPOOLP512R1};

static u8 const _brainpoolP192T1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'1',
	(u8)'9',
	(u8)'2',
	(u8)'T',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP192T1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _brainpoolP192T1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x04};

static _curve const _curve_brainpoolP192T1 =
{.crv_name = _brainpoolP192T1_curve_name,
	.crv_printable_oid = _brainpoolP192T1_curve_printable_oid,
	.crv_der_oid = _brainpoolP192T1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP192T1_curve_der_oid),
	.crv_order_bit_len = (u32)192,
.crv_id = CURVE_BRAINPOOLP192T1};

static u8 const _brainpoolP224T1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'2',
	(u8)'2',
	(u8)'4',
	(u8)'T',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP224T1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'6',
(u8)'\000'};

static u8 const _brainpoolP224T1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x06};

static _curve const _curve_brainpoolP224T1 =
{.crv_name = _brainpoolP224T1_curve_name,
	.crv_printable_oid = _brainpoolP224T1_curve_printable_oid,
	.crv_der_oid = _brainpoolP224T1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP224T1_curve_der_oid),
	.crv_order_bit_len = (u32)224,
.crv_id = CURVE_BRAINPOOLP224T1};

static u8 const _brainpoolP256T1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'T',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP256T1_curve_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'8',
(u8)'\000'};

static u8 const _brainpoolP256T1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x08};

static _curve const _curve_brainpoolP256T1 =
{.crv_name = _brainpoolP256T1_curve_name,
	.crv_printable_oid = _brainpoolP256T1_curve_printable_oid,
	.crv_der_oid = _brainpoolP256T1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP256T1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_BRAINPOOLP256T1};

static u8 const _brainpoolP320T1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'3',
	(u8)'2',
	(u8)'0',
	(u8)'T',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP320T1_curve_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
(u8)'\000'};

static u8 const _brainpoolP320T1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x0a};

static _curve const _curve_brainpoolP320T1 =
{.crv_name = _brainpoolP320T1_curve_name,
	.crv_printable_oid = _brainpoolP320T1_curve_printable_oid,
	.crv_der_oid = _brainpoolP320T1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP320T1_curve_der_oid),
	.crv_order_bit_len = (u32)320,
.crv_id = CURVE_BRAINPOOLP320T1};

static u8 const _brainpoolP384T1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'3',
	(u8)'8',
	(u8)'4',
	(u8)'T',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP384T1_curve_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _brainpoolP384T1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x0c};

static _curve const _curve_brainpoolP384T1 =
{.crv_name = _brainpoolP384T1_curve_name,
	.crv_printable_oid = _brainpoolP384T1_curve_printable_oid,
	.crv_der_oid = _brainpoolP384T1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP384T1_curve_der_oid),
	.crv_order_bit_len = (u32)384,
.crv_id = CURVE_BRAINPOOLP384T1};

static u8 const _brainpoolP512T1_curve_name[16] =
{(u8)'b',
	(u8)'r',
	(u8)'a',
	(u8)'i',
	(u8)'n',
	(u8)'p',
	(u8)'o',
	(u8)'o',
	(u8)'l',
	(u8)'P',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'T',
	(u8)'1',
(u8)'\000'};

static u8 const _brainpoolP512T1_curve_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'4',
(u8)'\000'};

static u8 const _brainpoolP512T1_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x02,
	(u8)0x08,
	(u8)0x01,
	(u8)0x01,
(u8)0x0e};

static _curve const _curve_brainpoolP512T1 =
{.crv_name = _brainpoolP512T1_curve_name,
	.crv_printable_oid = _brainpoolP512T1_curve_printable_oid,
	.crv_der_oid = _brainpoolP512T1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_brainpoolP512T1_curve_der_oid),
	.crv_order_bit_len = (u32)512,
.crv_id = CURVE_BRAINPOOLP512T1};

static u8 const _sm2p256v1_curve_name[10] =
{(u8)'s',
	(u8)'m',
	(u8)'2',
	(u8)'p',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _sm2p256v1_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'5',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'9',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'0',
	(u8)'1',
(u8)'\000'};

static u8 const _sm2p256v1_curve_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x81,
	(u8)0x1c,
	(u8)0xcf,
	(u8)0x55,
	(u8)0x01,
	(u8)0x82,
(u8)0x2d};

static _curve const _curve_sm2p256v1 =
{.crv_name = _sm2p256v1_curve_name,
	.crv_printable_oid = _sm2p256v1_curve_printable_oid,
	.crv_der_oid = _sm2p256v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_sm2p256v1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_SM2P256V1};

static u8 const _frp256v1_curve_name[9] =
{(u8)'f',
	(u8)'r',
	(u8)'p',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'v',
	(u8)'1',
(u8)'\000'};

static u8 const _frp256v1_curve_printable_oid[24] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'5',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'2',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _frp256v1_curve_der_oid[12] =
{(u8)0x06,
	(u8)0x0A,
	(u8)0x2A,
	(u8)0x81,
	(u8)0x7A,
	(u8)0x01,
	(u8)0x81,
	(u8)0x5F,
	(u8)0x65,
	(u8)0x82,
	(u8)0x00,
(u8)0x01};

static _curve const _curve_frp256v1 =
{.crv_name = _frp256v1_curve_name,
	.crv_printable_oid = _frp256v1_curve_printable_oid,
	.crv_der_oid = _frp256v1_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_frp256v1_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_FRP256V1};

static u8 const _gost_R3410_2001_CryptoPro_A_ParamSet_curve_name[37] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'A',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_A_ParamSet_curve_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'5',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_A_ParamSet_curve_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x23,
(u8)0x01};

static _curve const _curve_gost_R3410_2001_CryptoPro_A_ParamSet =
{.crv_name = _gost_R3410_2001_CryptoPro_A_ParamSet_curve_name,
	.crv_printable_oid = _gost_R3410_2001_CryptoPro_A_ParamSet_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2001_CryptoPro_A_ParamSet_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2001_CryptoPro_A_ParamSet_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2001_CRYPTOPRO_A_PARAMSET};

static u8 const _gost_R3410_2001_CryptoPro_B_ParamSet_curve_name[37] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'B',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_B_ParamSet_curve_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'5',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_B_ParamSet_curve_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x23,
(u8)0x02};

static _curve const _curve_gost_R3410_2001_CryptoPro_B_ParamSet =
{.crv_name = _gost_R3410_2001_CryptoPro_B_ParamSet_curve_name,
	.crv_printable_oid = _gost_R3410_2001_CryptoPro_B_ParamSet_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2001_CryptoPro_B_ParamSet_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2001_CryptoPro_B_ParamSet_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2001_CRYPTOPRO_B_PARAMSET};

static u8 const _gost_R3410_2001_CryptoPro_C_ParamSet_curve_name[37] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'C',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_C_ParamSet_curve_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'5',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_C_ParamSet_curve_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x23,
(u8)0x03};

static _curve const _curve_gost_R3410_2001_CryptoPro_C_ParamSet =
{.crv_name = _gost_R3410_2001_CryptoPro_C_ParamSet_curve_name,
	.crv_printable_oid = _gost_R3410_2001_CryptoPro_C_ParamSet_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2001_CryptoPro_C_ParamSet_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2001_CryptoPro_C_ParamSet_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2001_CRYPTOPRO_C_PARAMSET};

static u8 const _gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_name[40] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'X',
	(u8)'c',
	(u8)'h',
	(u8)'A',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'0',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x24,
(u8)0x00};

static _curve const _curve_gost_R3410_2001_CryptoPro_XchA_ParamSet =
{.crv_name = _gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_name,
	.crv_printable_oid = _gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2001_CryptoPro_XchA_ParamSet_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2001_CRYPTOPRO_XCHA_PARAMSET};

static u8 const _gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_name[40] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'X',
	(u8)'c',
	(u8)'h',
	(u8)'B',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x24,
(u8)0x01};

static _curve const _curve_gost_R3410_2001_CryptoPro_XchB_ParamSet =
{.crv_name = _gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_name,
	.crv_printable_oid = _gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2001_CryptoPro_XchB_ParamSet_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2001_CRYPTOPRO_XCHB_PARAMSET};

static u8 const _gost_R3410_2001_TestParamSet_curve_name[29] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)'_',
	(u8)'T',
	(u8)'e',
	(u8)'s',
	(u8)'t',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2001_TestParamSet_curve_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'5',
	(u8)'.',
	(u8)'0',
(u8)'\000'};

static u8 const _gost_R3410_2001_TestParamSet_curve_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x23,
(u8)0x00};

static _curve const _curve_gost_R3410_2001_TestParamSet =
{.crv_name = _gost_R3410_2001_TestParamSet_curve_name,
	.crv_printable_oid = _gost_R3410_2001_TestParamSet_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2001_TestParamSet_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2001_TestParamSet_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2001_TESTPARAMSET};

static u8 const _gost_R3410_2012_256_paramSetA_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'A',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetA_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetA_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x01,
(u8)0x01};

static _curve const _curve_gost_R3410_2012_256_paramSetA =
{.crv_name = _gost_R3410_2012_256_paramSetA_curve_name,
	.crv_printable_oid = _gost_R3410_2012_256_paramSetA_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_256_paramSetA_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_256_paramSetA_curve_der_oid),
	.crv_order_bit_len = (u32)257,
.crv_id = CURVE_GOST_R3410_2012_256_PARAMSETA};

static u8 const _gost_R3410_2012_256_paramSetB_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'B',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetB_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetB_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x01,
(u8)0x02};

static _curve const _curve_gost_R3410_2012_256_paramSetB =
{.crv_name = _gost_R3410_2012_256_paramSetB_curve_name,
	.crv_printable_oid = _gost_R3410_2012_256_paramSetB_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_256_paramSetB_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_256_paramSetB_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2012_256_PARAMSETB};

static u8 const _gost_R3410_2012_256_paramSetC_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'C',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetC_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetC_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x01,
(u8)0x02};

static _curve const _curve_gost_R3410_2012_256_paramSetC =
{.crv_name = _gost_R3410_2012_256_paramSetC_curve_name,
	.crv_printable_oid = _gost_R3410_2012_256_paramSetC_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_256_paramSetC_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_256_paramSetC_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2012_256_PARAMSETC};

static u8 const _gost_R3410_2012_256_paramSetD_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'D',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetD_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_paramSetD_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x01,
(u8)0x04};

static _curve const _curve_gost_R3410_2012_256_paramSetD =
{.crv_name = _gost_R3410_2012_256_paramSetD_curve_name,
	.crv_printable_oid = _gost_R3410_2012_256_paramSetD_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_256_paramSetD_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_256_paramSetD_curve_der_oid),
	.crv_order_bit_len = (u32)256,
.crv_id = CURVE_GOST_R3410_2012_256_PARAMSETD};

static u8 const _gost_R3410_2012_512_paramSetA_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'A',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetA_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetA_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x02,
(u8)0x01};

static _curve const _curve_gost_R3410_2012_512_paramSetA =
{.crv_name = _gost_R3410_2012_512_paramSetA_curve_name,
	.crv_printable_oid = _gost_R3410_2012_512_paramSetA_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_512_paramSetA_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_512_paramSetA_curve_der_oid),
	.crv_order_bit_len = (u32)512,
.crv_id = CURVE_GOST_R3410_2012_512_PARAMSETA};

static u8 const _gost_R3410_2012_512_paramSetB_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'B',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetB_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetB_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x02,
(u8)0x02};

static _curve const _curve_gost_R3410_2012_512_paramSetB =
{.crv_name = _gost_R3410_2012_512_paramSetB_curve_name,
	.crv_printable_oid = _gost_R3410_2012_512_paramSetB_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_512_paramSetB_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_512_paramSetB_curve_der_oid),
	.crv_order_bit_len = (u32)512,
.crv_id = CURVE_GOST_R3410_2012_512_PARAMSETB};

static u8 const _gost_R3410_2012_512_paramSetC_curve_name[30] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'C',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetC_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetC_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x02,
(u8)0x03};

static _curve const _curve_gost_R3410_2012_512_paramSetC =
{.crv_name = _gost_R3410_2012_512_paramSetC_curve_name,
	.crv_printable_oid = _gost_R3410_2012_512_paramSetC_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_512_paramSetC_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_512_paramSetC_curve_der_oid),
	.crv_order_bit_len = (u32)512,
.crv_id = CURVE_GOST_R3410_2012_512_PARAMSETC};

static u8 const _gost_R3410_2012_512_paramSetTest_curve_name[33] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'_',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'_',
	(u8)'p',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
	(u8)'T',
	(u8)'e',
	(u8)'s',
	(u8)'t',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetTest_curve_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_paramSetTest_curve_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x02,
	(u8)0x01,
	(u8)0x02,
(u8)0x00};

static _curve const _curve_gost_R3410_2012_512_paramSetTest =
{.crv_name = _gost_R3410_2012_512_paramSetTest_curve_name,
	.crv_printable_oid = _gost_R3410_2012_512_paramSetTest_curve_printable_oid,
	.crv_der_oid = _gost_R3410_2012_512_paramSetTest_curve_der_oid,
	.crv_der_oid_len = (u32)sizeof(_gost_R3410_2012_512_paramSetTest_curve_der_oid),
	.crv_order_bit_len = (u32)511,
.crv_id = CURVE_GOST_R3410_2012_512_PARAMSETTEST};

_curve const *known_curves[44] =
{& _curve_secp224r1,
	& _curve_secp256r1,
	& _curve_secp384r1,
	& _curve_secp521r1,
	& _curve_prime192v1,
	& _curve_c2pnb163v1,
	& _curve_sect571k1,
	& _curve_sect163k1,
	& _curve_secp192k1,
	& _curve_secp224k1,
	& _curve_secp256k1,
	& _curve_brainpoolP192R1,
	& _curve_brainpoolP224R1,
	& _curve_brainpoolP256R1,
	& _curve_brainpoolP320R1,
	& _curve_brainpoolP384R1,
	& _curve_brainpoolP512R1,
	& _curve_brainpoolP192T1,
	& _curve_brainpoolP224T1,
	& _curve_brainpoolP256T1,
	& _curve_brainpoolP320T1,
	& _curve_brainpoolP384T1,
	& _curve_brainpoolP512T1,
	& _curve_sm2p256v1,
	& _curve_bign_curve256v1,
	& _curve_bign_curve384v1,
	& _curve_bign_curve512v1,
	& _curve_frp256v1,
	& _curve_gost_R3410_2001_CryptoPro_A_ParamSet,
	& _curve_gost_R3410_2001_CryptoPro_B_ParamSet,
	& _curve_gost_R3410_2001_CryptoPro_C_ParamSet,
	& _curve_gost_R3410_2001_CryptoPro_XchA_ParamSet,
	& _curve_gost_R3410_2001_CryptoPro_XchB_ParamSet,
	& _curve_gost_R3410_2001_TestParamSet,
	& _curve_gost_R3410_2012_256_paramSetA,
	& _curve_gost_R3410_2012_256_paramSetB,
	& _curve_gost_R3410_2012_256_paramSetC,
	& _curve_gost_R3410_2012_256_paramSetD,
	& _curve_gost_R3410_2012_512_paramSetA,
	& _curve_gost_R3410_2012_512_paramSetB,
	& _curve_gost_R3410_2012_512_paramSetC,
	& _curve_gost_R3410_2012_512_paramSetTest,
	& _curve_Curve25519,
& _curve_Curve448};

static u8 const _ecdsa_sha1_sig_name[16] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'1',
(u8)'\000'};

static u8 const _ecdsa_sha1_sig_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _ecdsa_sha1_sig_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x04,
(u8)0x01};

static _sig_alg const _ecdsa_sha1_sig_alg =
{.alg_name = _ecdsa_sha1_sig_name,
	.alg_printable_oid = _ecdsa_sha1_sig_printable_oid,
	.alg_der_oid = _ecdsa_sha1_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_sha1_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_sha224_sig_name[18] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'2',
	(u8)'2',
	(u8)'4',
(u8)'\000'};

static u8 const _ecdsa_sha224_sig_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _ecdsa_sha224_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x04,
	(u8)0x03,
(u8)0x01};

static _sig_alg const _ecdsa_sha224_sig_alg =
{.alg_name = _ecdsa_sha224_sig_name,
	.alg_printable_oid = _ecdsa_sha224_sig_printable_oid,
	.alg_der_oid = _ecdsa_sha224_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_sha224_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHA224,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_sha256_sig_name[18] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _ecdsa_sha256_sig_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _ecdsa_sha256_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x04,
	(u8)0x03,
(u8)0x02};

static _sig_alg const _ecdsa_sha256_sig_alg =
{.alg_name = _ecdsa_sha256_sig_name,
	.alg_printable_oid = _ecdsa_sha256_sig_printable_oid,
	.alg_der_oid = _ecdsa_sha256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_sha256_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHA256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_sha384_sig_name[18] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'3',
	(u8)'8',
	(u8)'4',
(u8)'\000'};

static u8 const _ecdsa_sha384_sig_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _ecdsa_sha384_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x04,
	(u8)0x03,
(u8)0x03};

static _sig_alg const _ecdsa_sha384_sig_alg =
{.alg_name = _ecdsa_sha384_sig_name,
	.alg_printable_oid = _ecdsa_sha384_sig_printable_oid,
	.alg_der_oid = _ecdsa_sha384_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_sha384_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHA384,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_sha512_sig_name[18] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _ecdsa_sha512_sig_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _ecdsa_sha512_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x04,
	(u8)0x03,
(u8)0x04};

static _sig_alg const _ecdsa_sha512_sig_alg =
{.alg_name = _ecdsa_sha512_sig_name,
	.alg_printable_oid = _ecdsa_sha512_sig_printable_oid,
	.alg_der_oid = _ecdsa_sha512_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_sha512_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHA512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_with_sha3_256_sig_name[23] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'-',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _ecdsa_with_sha3_256_sig_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
(u8)'\000'};

static u8 const _ecdsa_with_sha3_256_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x0a};

static _sig_alg const _ecdsa_with_sha3_256_sig_alg =
{.alg_name = _ecdsa_with_sha3_256_sig_name,
	.alg_printable_oid = _ecdsa_with_sha3_256_sig_printable_oid,
	.alg_der_oid = _ecdsa_with_sha3_256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_with_sha3_256_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHA3_256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_with_shake128_sig_name[20] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'k',
	(u8)'e',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _ecdsa_with_shake128_sig_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'2',
(u8)'\000'};

static u8 const _ecdsa_with_shake128_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x06,
(u8)0x20};

static _sig_alg const _ecdsa_with_shake128_sig_alg =
{.alg_name = _ecdsa_with_shake128_sig_name,
	.alg_printable_oid = _ecdsa_with_shake128_sig_printable_oid,
	.alg_der_oid = _ecdsa_with_shake128_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_with_shake128_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHAKE128,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_with_shake256_sig_name[20] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'k',
	(u8)'e',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _ecdsa_with_shake256_sig_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'2',
(u8)'\000'};

static u8 const _ecdsa_with_shake256_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x06,
(u8)0x20};

static _sig_alg const _ecdsa_with_shake256_sig_alg =
{.alg_name = _ecdsa_with_shake256_sig_name,
	.alg_printable_oid = _ecdsa_with_shake256_sig_printable_oid,
	.alg_der_oid = _ecdsa_with_shake256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_with_shake256_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_SHAKE256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ecdsa_with_specified_sig_name[21] =
{(u8)'e',
	(u8)'c',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'p',
	(u8)'e',
	(u8)'c',
	(u8)'i',
	(u8)'f',
	(u8)'i',
	(u8)'e',
	(u8)'d',
(u8)'\000'};

static u8 const _ecdsa_with_specified_sig_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _ecdsa_with_specified_sig_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x04,
(u8)0x03};

static _sig_alg const _ecdsa_with_specified_sig_alg =
{.alg_name = _ecdsa_with_specified_sig_name,
	.alg_printable_oid = _ecdsa_with_specified_sig_printable_oid,
	.alg_der_oid = _ecdsa_with_specified_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecdsa_with_specified_sig_der_oid),
	.sig_id = SIG_ALG_ECDSA,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_ecdsa_with_specified,
.parse_sig = & parse_sig_ecdsa};

static u8 const _ed25519_sig_name[8] =
{(u8)'E', (u8)'d', (u8)'2', (u8)'5', (u8)'5', (u8)'1', (u8)'9', (u8)'\000'};

static u8 const _ed25519_sig_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _ed25519_sig_der_oid[5] =
{(u8)0x06, (u8)0x03, (u8)0x2b, (u8)0x65, (u8)0x70};

static _sig_alg const _ed25519_sig_alg =
{.alg_name = _ed25519_sig_name,
	.alg_printable_oid = _ed25519_sig_printable_oid,
	.alg_der_oid = _ed25519_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ed25519_sig_der_oid),
	.sig_id = SIG_ALG_ED25519,
	.hash_id = HASH_ALG_SHA512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_eddsa,
.parse_sig = & parse_sig_ed25519};

static u8 const _ed448_sig_name[6] =
{(u8)'E', (u8)'d', (u8)'4', (u8)'4', (u8)'8', (u8)'\000'};

static u8 const _ed448_sig_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _ed448_sig_der_oid[5] =
{(u8)0x06, (u8)0x03, (u8)0x2b, (u8)0x65, (u8)0x71};

static _sig_alg const _ed448_sig_alg =
{.alg_name = _ed448_sig_name,
	.alg_printable_oid = _ed448_sig_printable_oid,
	.alg_der_oid = _ed448_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ed448_sig_der_oid),
	.sig_id = SIG_ALG_ED448,
	.hash_id = HASH_ALG_SHAKE256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_eddsa,
.parse_sig = & parse_sig_ed448};

static u8 const _sm2_sm3_sig_name[11] =
{(u8)'S',
	(u8)'M',
	(u8)'2',
	(u8)' ',
	(u8)'w',
	(u8)'/',
	(u8)' ',
	(u8)'S',
	(u8)'M',
	(u8)'3',
(u8)'\000'};

static u8 const _sm2_sm3_sig_printable_oid[20] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'5',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'9',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'0',
	(u8)'1',
(u8)'\000'};

static u8 const _sm2_sm3_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x81,
	(u8)0x1c,
	(u8)0xcf,
	(u8)0x55,
	(u8)0x01,
	(u8)0x83,
(u8)0x75};

static _sig_alg const _sm2_sm3_sig_alg =
{.alg_name = _sm2_sm3_sig_name,
	.alg_printable_oid = _sm2_sm3_sig_printable_oid,
	.alg_der_oid = _sm2_sm3_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sm2_sm3_sig_der_oid),
	.sig_id = SIG_ALG_SM2,
	.hash_id = HASH_ALG_SM3,
	.parse_algoid_sig_params = & parse_algoid_sig_params_sm2,
.parse_sig = & parse_sig_sm2};

static u8 const _rsa_mdc2_sig_name[12] =
{(u8)'m',
	(u8)'d',
	(u8)'c',
	(u8)'2',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
(u8)'\000'};

static u8 const _rsa_mdc2_sig_printable_oid[12] =
{(u8)'2',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
(u8)'\000'};

static u8 const _rsa_mdc2_sig_der_oid[6] =
{(u8)0x06, (u8)0x04, (u8)0x55, (u8)0x08, (u8)0x03, (u8)0x64};

static _sig_alg const _rsa_mdc2_sig_alg =
{.alg_name = _rsa_mdc2_sig_name,
	.alg_printable_oid = _rsa_mdc2_sig_printable_oid,
	.alg_der_oid = _rsa_mdc2_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_mdc2_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_MDC2,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_md2_sig_name[21] =
{(u8)'m',
	(u8)'d',
	(u8)'2',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_md2_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _rsa_md2_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x02};

static _sig_alg const _rsa_md2_sig_alg =
{.alg_name = _rsa_md2_sig_name,
	.alg_printable_oid = _rsa_md2_sig_printable_oid,
	.alg_der_oid = _rsa_md2_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_md2_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_MD2,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_md4_sig_name[21] =
{(u8)'m',
	(u8)'d',
	(u8)'4',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_md4_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _rsa_md4_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x03};

static _sig_alg const _rsa_md4_sig_alg =
{.alg_name = _rsa_md4_sig_name,
	.alg_printable_oid = _rsa_md4_sig_printable_oid,
	.alg_der_oid = _rsa_md4_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_md4_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_MD4,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_md5_sig_name[21] =
{(u8)'m',
	(u8)'d',
	(u8)'5',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_md5_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _rsa_md5_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x04};

static _sig_alg const _rsa_md5_sig_alg =
{.alg_name = _rsa_md5_sig_name,
	.alg_printable_oid = _rsa_md5_sig_printable_oid,
	.alg_der_oid = _rsa_md5_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_md5_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_MD5,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _sha1WithRSAEnc_sig_name[22] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x05};

static _sig_alg const _sha1WithRSAEnc_sig_alg =
{.alg_name = _sha1WithRSAEnc_sig_name,
	.alg_printable_oid = _sha1WithRSAEnc_sig_printable_oid,
	.alg_der_oid = _sha1WithRSAEnc_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha1WithRSAEnc_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _sha1WithRSAEnc_bis_sig_name[28] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'b',
	(u8)'i',
	(u8)'s',
	(u8)')',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_bis_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'9',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_bis_sig_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x1d};

static _sig_alg const _sha1WithRSAEnc_bis_sig_alg =
{.alg_name = _sha1WithRSAEnc_bis_sig_name,
	.alg_printable_oid = _sha1WithRSAEnc_bis_sig_printable_oid,
	.alg_der_oid = _sha1WithRSAEnc_bis_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha1WithRSAEnc_bis_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _sha1WithRSAEnc_alt_sig_name[26] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
	(u8)'_',
	(u8)'a',
	(u8)'l',
	(u8)'t',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_alt_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_alt_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x44,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x05};

static _sig_alg const _sha1WithRSAEnc_alt_sig_alg =
{.alg_name = _sha1WithRSAEnc_alt_sig_name,
	.alg_printable_oid = _sha1WithRSAEnc_alt_sig_printable_oid,
	.alg_der_oid = _sha1WithRSAEnc_alt_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha1WithRSAEnc_alt_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _sha1WithRSAEnc_alt2_sig_name[27] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
	(u8)'_',
	(u8)'a',
	(u8)'l',
	(u8)'t',
	(u8)'2',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_alt2_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'4',
	(u8)'9',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_alt2_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0xa6,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x05};

static _sig_alg const _sha1WithRSAEnc_alt2_sig_alg =
{.alg_name = _sha1WithRSAEnc_alt2_sig_name,
	.alg_printable_oid = _sha1WithRSAEnc_alt2_sig_printable_oid,
	.alg_der_oid = _sha1WithRSAEnc_alt2_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha1WithRSAEnc_alt2_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _sha1WithRSAEnc_ter_sig_name[28] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'t',
	(u8)'e',
	(u8)'r',
	(u8)')',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_ter_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _sha1WithRSAEnc_ter_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x00,
	(u8)0x01,
(u8)0x05};

static _sig_alg const _sha1WithRSAEnc_ter_sig_alg =
{.alg_name = _sha1WithRSAEnc_ter_sig_name,
	.alg_printable_oid = _sha1WithRSAEnc_ter_sig_printable_oid,
	.alg_der_oid = _sha1WithRSAEnc_ter_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_sha1WithRSAEnc_ter_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_sha1_crap_sig_name[33] =
{(u8)'A',
	(u8)'n',
	(u8)'o',
	(u8)'t',
	(u8)'h',
	(u8)'e',
	(u8)'r',
	(u8)' ',
	(u8)'u',
	(u8)'n',
	(u8)'s',
	(u8)'p',
	(u8)'e',
	(u8)'c',
	(u8)'i',
	(u8)'f',
	(u8)'i',
	(u8)'e',
	(u8)'d',
	(u8)' ',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'1',
	(u8)' ',
	(u8)'o',
	(u8)'i',
	(u8)'d',
(u8)'\000'};

static u8 const _rsa_sha1_crap_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'5',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _rsa_sha1_crap_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x58,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x05};

static _sig_alg const _rsa_sha1_crap_sig_alg =
{.alg_name = _rsa_sha1_crap_sig_name,
	.alg_printable_oid = _rsa_sha1_crap_sig_printable_oid,
	.alg_der_oid = _rsa_sha1_crap_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_sha1_crap_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_sha1_crap_bis_sig_name[31] =
{(u8)'c',
	(u8)'r',
	(u8)'a',
	(u8)'p',
	(u8)'p',
	(u8)'y',
	(u8)' ',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'s',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'a',
	(u8)'t',
	(u8)'u',
	(u8)'r',
	(u8)'e',
(u8)'\000'};

static u8 const _rsa_sha1_crap_bis_sig_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'7',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _rsa_sha1_crap_bis_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x68,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x05};

static _sig_alg const _rsa_sha1_crap_bis_sig_alg =
{.alg_name = _rsa_sha1_crap_bis_sig_name,
	.alg_printable_oid = _rsa_sha1_crap_bis_sig_printable_oid,
	.alg_der_oid = _rsa_sha1_crap_bis_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_sha1_crap_bis_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _shaWithRSASig_9796_2_sig_name[27] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'S',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'a',
	(u8)'t',
	(u8)'u',
	(u8)'r',
	(u8)'e',
	(u8)'-',
	(u8)'9',
	(u8)'7',
	(u8)'9',
	(u8)'6',
	(u8)'-',
	(u8)'2',
(u8)'\000'};

static u8 const _shaWithRSASig_9796_2_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'5',
(u8)'\000'};

static u8 const _shaWithRSASig_9796_2_sig_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x0f};

static _sig_alg const _shaWithRSASig_9796_2_sig_alg =
{.alg_name = _shaWithRSASig_9796_2_sig_name,
	.alg_printable_oid = _shaWithRSASig_9796_2_sig_printable_oid,
	.alg_der_oid = _shaWithRSASig_9796_2_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_shaWithRSASig_9796_2_sig_der_oid),
	.sig_id = SIG_ALG_RSA_9796_2_PAD,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_9796_2_pad};

static u8 const _rsa_sha224_sig_name[24] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'2',
	(u8)'2',
	(u8)'4',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_sha224_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'4',
(u8)'\000'};

static u8 const _rsa_sha224_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x0e};

static _sig_alg const _rsa_sha224_sig_alg =
{.alg_name = _rsa_sha224_sig_name,
	.alg_printable_oid = _rsa_sha224_sig_printable_oid,
	.alg_der_oid = _rsa_sha224_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_sha224_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA224,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_sha256_sig_name[24] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_sha256_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _rsa_sha256_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x0b};

static _sig_alg const _rsa_sha256_sig_alg =
{.alg_name = _rsa_sha256_sig_name,
	.alg_printable_oid = _rsa_sha256_sig_printable_oid,
	.alg_der_oid = _rsa_sha256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_sha256_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_sha384_sig_name[24] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'8',
	(u8)'4',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_sha384_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _rsa_sha384_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x0c};

static _sig_alg const _rsa_sha384_sig_alg =
{.alg_name = _rsa_sha384_sig_name,
	.alg_printable_oid = _rsa_sha384_sig_printable_oid,
	.alg_der_oid = _rsa_sha384_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_sha384_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA384,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_sha512_sig_name[24] =
{(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _rsa_sha512_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _rsa_sha512_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x0d};

static _sig_alg const _rsa_sha512_sig_alg =
{.alg_name = _rsa_sha512_sig_name,
	.alg_printable_oid = _rsa_sha512_sig_printable_oid,
	.alg_der_oid = _rsa_sha512_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_sha512_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_ripemd160_sig_name[26] =
{(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'S',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'a',
	(u8)'t',
	(u8)'u',
	(u8)'r',
	(u8)'e',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'r',
	(u8)'i',
	(u8)'p',
	(u8)'e',
	(u8)'m',
	(u8)'d',
	(u8)'1',
	(u8)'6',
	(u8)'0',
(u8)'\000'};

static u8 const _rsa_ripemd160_sig_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _rsa_ripemd160_sig_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x01,
(u8)0x02};

static _sig_alg const _rsa_ripemd160_sig_alg =
{.alg_name = _rsa_ripemd160_sig_name,
	.alg_printable_oid = _rsa_ripemd160_sig_printable_oid,
	.alg_der_oid = _rsa_ripemd160_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_ripemd160_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_RIPEMD160,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_ripemd128_sig_name[26] =
{(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'S',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'a',
	(u8)'t',
	(u8)'u',
	(u8)'r',
	(u8)'e',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'r',
	(u8)'i',
	(u8)'p',
	(u8)'e',
	(u8)'m',
	(u8)'d',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _rsa_ripemd128_sig_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _rsa_ripemd128_sig_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x01,
(u8)0x03};

static _sig_alg const _rsa_ripemd128_sig_alg =
{.alg_name = _rsa_ripemd128_sig_name,
	.alg_printable_oid = _rsa_ripemd128_sig_printable_oid,
	.alg_der_oid = _rsa_ripemd128_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_ripemd128_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_RIPEMD128,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsa_ripemd256_sig_name[26] =
{(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'S',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'a',
	(u8)'t',
	(u8)'u',
	(u8)'r',
	(u8)'e',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'r',
	(u8)'i',
	(u8)'p',
	(u8)'e',
	(u8)'m',
	(u8)'d',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _rsa_ripemd256_sig_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _rsa_ripemd256_sig_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2b,
	(u8)0x24,
	(u8)0x03,
	(u8)0x03,
	(u8)0x01,
(u8)0x03};

static _sig_alg const _rsa_ripemd256_sig_alg =
{.alg_name = _rsa_ripemd256_sig_name,
	.alg_printable_oid = _rsa_ripemd256_sig_printable_oid,
	.alg_der_oid = _rsa_ripemd256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_ripemd256_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_RIPEMD256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _pkcs1_v15_w_sha3_224_sig_name[35] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'s',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'p',
	(u8)'k',
	(u8)'c',
	(u8)'s',
	(u8)'1',
	(u8)'-',
	(u8)'v',
	(u8)'1',
	(u8)'-',
	(u8)'5',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'-',
	(u8)'2',
	(u8)'2',
	(u8)'4',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_224_sig_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_224_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x0d};

static _sig_alg const _pkcs1_v15_w_sha3_224_sig_alg =
{.alg_name = _pkcs1_v15_w_sha3_224_sig_name,
	.alg_printable_oid = _pkcs1_v15_w_sha3_224_sig_printable_oid,
	.alg_der_oid = _pkcs1_v15_w_sha3_224_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_pkcs1_v15_w_sha3_224_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA3_224,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _pkcs1_v15_w_sha3_256_sig_name[35] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'s',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'p',
	(u8)'k',
	(u8)'c',
	(u8)'s',
	(u8)'1',
	(u8)'-',
	(u8)'v',
	(u8)'1',
	(u8)'-',
	(u8)'5',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'-',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_256_sig_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_256_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x0e};

static _sig_alg const _pkcs1_v15_w_sha3_256_sig_alg =
{.alg_name = _pkcs1_v15_w_sha3_256_sig_name,
	.alg_printable_oid = _pkcs1_v15_w_sha3_256_sig_printable_oid,
	.alg_der_oid = _pkcs1_v15_w_sha3_256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_pkcs1_v15_w_sha3_256_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA3_256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _pkcs1_v15_w_sha3_384_sig_name[35] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'s',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'p',
	(u8)'k',
	(u8)'c',
	(u8)'s',
	(u8)'1',
	(u8)'-',
	(u8)'v',
	(u8)'1',
	(u8)'-',
	(u8)'5',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'-',
	(u8)'3',
	(u8)'8',
	(u8)'4',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_384_sig_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'5',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_384_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x0f};

static _sig_alg const _pkcs1_v15_w_sha3_384_sig_alg =
{.alg_name = _pkcs1_v15_w_sha3_384_sig_name,
	.alg_printable_oid = _pkcs1_v15_w_sha3_384_sig_printable_oid,
	.alg_der_oid = _pkcs1_v15_w_sha3_384_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_pkcs1_v15_w_sha3_384_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA3_384,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _pkcs1_v15_w_sha3_512_sig_name[35] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'s',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'p',
	(u8)'k',
	(u8)'c',
	(u8)'s',
	(u8)'1',
	(u8)'-',
	(u8)'v',
	(u8)'1',
	(u8)'-',
	(u8)'5',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'-',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_512_sig_printable_oid[24] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'6',
(u8)'\000'};

static u8 const _pkcs1_v15_w_sha3_512_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x10};

static _sig_alg const _pkcs1_v15_w_sha3_512_sig_alg =
{.alg_name = _pkcs1_v15_w_sha3_512_sig_name,
	.alg_printable_oid = _pkcs1_v15_w_sha3_512_sig_printable_oid,
	.alg_der_oid = _pkcs1_v15_w_sha3_512_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_pkcs1_v15_w_sha3_512_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_SHA3_512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsassa_pss_sig_name[11] =
{(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'S',
	(u8)'S',
	(u8)'A',
	(u8)'-',
	(u8)'P',
	(u8)'S',
	(u8)'S',
(u8)'\000'};

static u8 const _rsassa_pss_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
(u8)'\000'};

static u8 const _rsassa_pss_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x0a};

static _sig_alg const _rsassa_pss_sig_alg =
{.alg_name = _rsassa_pss_sig_name,
	.alg_printable_oid = _rsassa_pss_sig_printable_oid,
	.alg_der_oid = _rsassa_pss_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsassa_pss_sig_der_oid),
	.sig_id = SIG_ALG_RSA_SSA_PSS,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsassa_pss,
.parse_sig = & parse_sig_rsa_ssa_pss};

static u8 const _rsassa_pss_shake128_sig_name[20] =
{(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'S',
	(u8)'S',
	(u8)'A',
	(u8)'-',
	(u8)'P',
	(u8)'S',
	(u8)'S',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'K',
	(u8)'E',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _rsassa_pss_shake128_sig_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'0',
(u8)'\000'};

static u8 const _rsassa_pss_shake128_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x06,
(u8)0x1e};

static _sig_alg const _rsassa_pss_shake128_sig_alg =
{.alg_name = _rsassa_pss_shake128_sig_name,
	.alg_printable_oid = _rsassa_pss_shake128_sig_printable_oid,
	.alg_der_oid = _rsassa_pss_shake128_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsassa_pss_shake128_sig_der_oid),
	.sig_id = SIG_ALG_RSA_SSA_PSS,
	.hash_id = HASH_ALG_SHAKE128,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_rsa_ssa_pss};

static u8 const _rsassa_pss_shake256_sig_name[20] =
{(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'S',
	(u8)'S',
	(u8)'A',
	(u8)'-',
	(u8)'P',
	(u8)'S',
	(u8)'S',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'K',
	(u8)'E',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _rsassa_pss_shake256_sig_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'1',
(u8)'\000'};

static u8 const _rsassa_pss_shake256_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x06,
(u8)0x1f};

static _sig_alg const _rsassa_pss_shake256_sig_alg =
{.alg_name = _rsassa_pss_shake256_sig_name,
	.alg_printable_oid = _rsassa_pss_shake256_sig_printable_oid,
	.alg_der_oid = _rsassa_pss_shake256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsassa_pss_shake256_sig_der_oid),
	.sig_id = SIG_ALG_RSA_SSA_PSS,
	.hash_id = HASH_ALG_SHAKE256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_rsa_ssa_pss};

static u8 const _belgian_rsa_sig_name[27] =
{(u8)'U',
	(u8)'n',
	(u8)'d',
	(u8)'o',
	(u8)'c',
	(u8)'.',
	(u8)' ',
	(u8)'B',
	(u8)'e',
	(u8)'l',
	(u8)'g',
	(u8)'i',
	(u8)'a',
	(u8)'n',
	(u8)' ',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)' ',
	(u8)'s',
	(u8)'i',
	(u8)'g',
	(u8)' ',
	(u8)'o',
	(u8)'i',
	(u8)'d',
(u8)'\000'};

static u8 const _belgian_rsa_sig_printable_oid[25] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'5',
	(u8)'6',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'8',
	(u8)'8',
	(u8)'0',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _belgian_rsa_sig_der_oid[13] =
{(u8)0x06,
	(u8)0x0b,
	(u8)0x60,
	(u8)0x38,
	(u8)0x02,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x01,
	(u8)0x82,
	(u8)0xaf,
	(u8)0x16,
(u8)0x01};

static _sig_alg const _belgian_rsa_sig_alg =
{.alg_name = _belgian_rsa_sig_name,
	.alg_printable_oid = _belgian_rsa_sig_printable_oid,
	.alg_der_oid = _belgian_rsa_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_belgian_rsa_sig_der_oid),
	.sig_id = SIG_ALG_BELGIAN_RSA,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_belgian};

static u8 const _rsalabs1_sig_name[21] =
{(u8)'U',
	(u8)'n',
	(u8)'k',
	(u8)'n',
	(u8)'o',
	(u8)'w',
	(u8)'n',
	(u8)' ',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)' ',
	(u8)'L',
	(u8)'a',
	(u8)'b',
	(u8)'s',
	(u8)' ',
	(u8)'O',
	(u8)'I',
	(u8)'D',
(u8)'\000'};

static u8 const _rsalabs1_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'9',
	(u8)'9',
(u8)'\000'};

static u8 const _rsalabs1_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x63};

static _sig_alg const _rsalabs1_sig_alg =
{.alg_name = _rsalabs1_sig_name,
	.alg_printable_oid = _rsalabs1_sig_printable_oid,
	.alg_der_oid = _rsalabs1_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsalabs1_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _rsalabs2_sig_name[20] =
{(u8)'U',
	(u8)'n',
	(u8)'s',
	(u8)'p',
	(u8)'e',
	(u8)'c',
	(u8)'i',
	(u8)'f',
	(u8)'i',
	(u8)'e',
	(u8)'d',
	(u8)' ',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)' ',
	(u8)'o',
	(u8)'i',
	(u8)'d',
(u8)'\000'};

static u8 const _rsalabs2_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'6',
	(u8)'0',
	(u8)'5',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _rsalabs2_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x45,
	(u8)0x01,
	(u8)0x01,
(u8)0x0b};

static _sig_alg const _rsalabs2_sig_alg =
{.alg_name = _rsalabs2_sig_name,
	.alg_printable_oid = _rsalabs2_sig_printable_oid,
	.alg_der_oid = _rsalabs2_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsalabs2_sig_der_oid),
	.sig_id = SIG_ALG_RSA_PKCS1_V1_5,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_rsa,
.parse_sig = & parse_sig_rsa_pkcs1_v15};

static u8 const _dsa_sha1_sig_name[12] =
{(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'1',
(u8)'\000'};

static u8 const _dsa_sha1_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'7',
(u8)'\000'};

static u8 const _dsa_sha1_sig_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x1b};

static _sig_alg const _dsa_sha1_sig_alg =
{.alg_name = _dsa_sha1_sig_name,
	.alg_printable_oid = _dsa_sha1_sig_printable_oid,
	.alg_der_oid = _dsa_sha1_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_sha1_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_sha1_old_sig_name[16] =
{(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'1',
	(u8)'-',
	(u8)'o',
	(u8)'l',
	(u8)'d',
(u8)'\000'};

static u8 const _dsa_sha1_old_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _dsa_sha1_old_sig_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x0c};

static _sig_alg const _dsa_sha1_old_sig_alg =
{.alg_name = _dsa_sha1_old_sig_name,
	.alg_printable_oid = _dsa_sha1_old_sig_printable_oid,
	.alg_der_oid = _dsa_sha1_old_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_sha1_old_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_sha1_jdk_sig_name[16] =
{(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'W',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'1',
	(u8)'-',
	(u8)'j',
	(u8)'d',
	(u8)'k',
(u8)'\000'};

static u8 const _dsa_sha1_jdk_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _dsa_sha1_jdk_sig_der_oid[7] =
{(u8)0x06, (u8)0x05, (u8)0x2b, (u8)0x0e, (u8)0x03, (u8)0x02, (u8)0x0d};

static _sig_alg const _dsa_sha1_jdk_sig_alg =
{.alg_name = _dsa_sha1_jdk_sig_name,
	.alg_printable_oid = _dsa_sha1_jdk_sig_printable_oid,
	.alg_der_oid = _dsa_sha1_jdk_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_sha1_jdk_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_sha1_bis_sig_name[14] =
{(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'1',
(u8)'\000'};

static u8 const _dsa_sha1_bis_sig_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _dsa_sha1_bis_sig_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x38,
	(u8)0x04,
(u8)0x03};

static _sig_alg const _dsa_sha1_bis_sig_alg =
{.alg_name = _dsa_sha1_bis_sig_name,
	.alg_printable_oid = _dsa_sha1_bis_sig_printable_oid,
	.alg_der_oid = _dsa_sha1_bis_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_sha1_bis_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA1,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_with_sha224_sig_name[19] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'2',
	(u8)'2',
	(u8)'4',
(u8)'\000'};

static u8 const _dsa_with_sha224_sig_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _dsa_with_sha224_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x01};

static _sig_alg const _dsa_with_sha224_sig_alg =
{.alg_name = _dsa_with_sha224_sig_name,
	.alg_printable_oid = _dsa_with_sha224_sig_printable_oid,
	.alg_der_oid = _dsa_with_sha224_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_with_sha224_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA224,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_with_sha256_sig_name[19] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _dsa_with_sha256_sig_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _dsa_with_sha256_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x02};

static _sig_alg const _dsa_with_sha256_sig_alg =
{.alg_name = _dsa_with_sha256_sig_name,
	.alg_printable_oid = _dsa_with_sha256_sig_printable_oid,
	.alg_der_oid = _dsa_with_sha256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_with_sha256_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_with_sha384_sig_name[19] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'3',
	(u8)'8',
	(u8)'4',
(u8)'\000'};

static u8 const _dsa_with_sha384_sig_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _dsa_with_sha384_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x03};

static _sig_alg const _dsa_with_sha384_sig_alg =
{.alg_name = _dsa_with_sha384_sig_name,
	.alg_printable_oid = _dsa_with_sha384_sig_printable_oid,
	.alg_der_oid = _dsa_with_sha384_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_with_sha384_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA384,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _dsa_with_sha512_sig_name[19] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'d',
	(u8)'s',
	(u8)'a',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'s',
	(u8)'h',
	(u8)'a',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _dsa_with_sha512_sig_printable_oid[23] =
{(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'6',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _dsa_with_sha512_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x65,
	(u8)0x03,
	(u8)0x04,
	(u8)0x03,
(u8)0x04};

static _sig_alg const _dsa_with_sha512_sig_alg =
{.alg_name = _dsa_with_sha512_sig_name,
	.alg_printable_oid = _dsa_with_sha512_sig_printable_oid,
	.alg_der_oid = _dsa_with_sha512_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_with_sha512_sig_der_oid),
	.sig_id = SIG_ALG_DSA,
	.hash_id = HASH_ALG_SHA512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_dsa};

static u8 const _gost_R3411_94_R3410_2001_sig_name[37] =
{(u8)'s',
	(u8)'i',
	(u8)'g',
	(u8)'_',
	(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'1',
	(u8)'-',
	(u8)'9',
	(u8)'4',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
(u8)'\000'};

static u8 const _gost_R3411_94_R3410_2001_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _gost_R3411_94_R3410_2001_sig_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
(u8)0x03};

static _sig_alg const _gost_R3411_94_R3410_2001_sig_alg =
{.alg_name = _gost_R3411_94_R3410_2001_sig_name,
	.alg_printable_oid = _gost_R3411_94_R3410_2001_sig_printable_oid,
	.alg_der_oid = _gost_R3411_94_R3410_2001_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3411_94_R3410_2001_sig_der_oid),
	.sig_id = SIG_ALG_GOSTR3410_2001,
	.hash_id = HASH_ALG_GOSTR3411_94,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_gost2001};

static u8 const _gost_R3411_94_R3410_94_sig_name[35] =
{(u8)'s',
	(u8)'i',
	(u8)'g',
	(u8)'_',
	(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'1',
	(u8)'-',
	(u8)'9',
	(u8)'4',
	(u8)'-',
	(u8)'w',
	(u8)'i',
	(u8)'t',
	(u8)'h',
	(u8)'-',
	(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'9',
	(u8)'4',
(u8)'\000'};

static u8 const _gost_R3411_94_R3410_94_sig_printable_oid[14] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _gost_R3411_94_R3410_94_sig_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
(u8)0x04};

static _sig_alg const _gost_R3411_94_R3410_94_sig_alg =
{.alg_name = _gost_R3411_94_R3410_94_sig_name,
	.alg_printable_oid = _gost_R3411_94_R3410_94_sig_printable_oid,
	.alg_der_oid = _gost_R3411_94_R3410_94_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3411_94_R3410_94_sig_der_oid),
	.sig_id = SIG_ALG_GOSTR3410_94,
	.hash_id = HASH_ALG_GOSTR3411_94,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_gost94};

static u8 const _gost_R3410_2012_256_sig_name[22] =
{(u8)'s',
	(u8)'i',
	(u8)'g',
	(u8)'_',
	(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'-',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_sig_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x01,
	(u8)0x03,
(u8)0x02};

static _sig_alg const _gost_R3410_2012_256_sig_alg =
{.alg_name = _gost_R3410_2012_256_sig_name,
	.alg_printable_oid = _gost_R3410_2012_256_sig_printable_oid,
	.alg_der_oid = _gost_R3410_2012_256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3410_2012_256_sig_der_oid),
	.sig_id = SIG_ALG_GOSTR3410_2012_256,
	.hash_id = HASH_ALG_STREEBOG256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_gost2012_256};

static u8 const _gost_R3410_2012_512_sig_name[22] =
{(u8)'s',
	(u8)'i',
	(u8)'g',
	(u8)'_',
	(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'-',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_sig_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_sig_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x01,
	(u8)0x03,
(u8)0x03};

static _sig_alg const _gost_R3410_2012_512_sig_alg =
{.alg_name = _gost_R3410_2012_512_sig_name,
	.alg_printable_oid = _gost_R3410_2012_512_sig_printable_oid,
	.alg_der_oid = _gost_R3410_2012_512_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3410_2012_512_sig_der_oid),
	.sig_id = SIG_ALG_GOSTR3410_2012_512,
	.hash_id = HASH_ALG_STREEBOG512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_gost2012_512};

static u8 const _bign_with_hbelt_sig_name[43] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'S',
	(u8)'T',
	(u8)'B',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'3',
	(u8)')',
	(u8)' ',
	(u8)'u',
	(u8)'s',
	(u8)'i',
	(u8)'n',
	(u8)'g',
	(u8)' ',
	(u8)'h',
	(u8)'b',
	(u8)'e',
	(u8)'l',
	(u8)'t',
	(u8)' ',
	(u8)'h',
	(u8)'a',
	(u8)'s',
	(u8)'h',
(u8)'\000'};

static u8 const _bign_with_hbelt_sig_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _bign_with_hbelt_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
(u8)0x0c};

static _sig_alg const _bign_with_hbelt_sig_alg =
{.alg_name = _bign_with_hbelt_sig_name,
	.alg_printable_oid = _bign_with_hbelt_sig_printable_oid,
	.alg_der_oid = _bign_with_hbelt_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bign_with_hbelt_sig_der_oid),
	.sig_id = SIG_ALG_BIGN,
	.hash_id = HASH_ALG_HBELT,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_bign};

static u8 const _bign_with_bash256_sig_name[40] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'S',
	(u8)'T',
	(u8)'B',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'3',
	(u8)')',
	(u8)' ',
	(u8)'u',
	(u8)'s',
	(u8)'i',
	(u8)'n',
	(u8)'g',
	(u8)' ',
	(u8)'B',
	(u8)'A',
	(u8)'S',
	(u8)'H',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _bign_with_bash256_sig_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _bign_with_bash256_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
(u8)0x0d};

static _sig_alg const _bign_with_bash256_sig_alg =
{.alg_name = _bign_with_bash256_sig_name,
	.alg_printable_oid = _bign_with_bash256_sig_printable_oid,
	.alg_der_oid = _bign_with_bash256_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bign_with_bash256_sig_der_oid),
	.sig_id = SIG_ALG_BIGN,
	.hash_id = HASH_ALG_BASH256,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_bign};

static u8 const _bign_with_bash384_sig_name[40] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'S',
	(u8)'T',
	(u8)'B',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'3',
	(u8)')',
	(u8)' ',
	(u8)'u',
	(u8)'s',
	(u8)'i',
	(u8)'n',
	(u8)'g',
	(u8)' ',
	(u8)'B',
	(u8)'A',
	(u8)'S',
	(u8)'H',
	(u8)'3',
	(u8)'8',
	(u8)'4',
(u8)'\000'};

static u8 const _bign_with_bash384_sig_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'1',
	(u8)'4',
(u8)'\000'};

static u8 const _bign_with_bash384_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
(u8)0x0e};

static _sig_alg const _bign_with_bash384_sig_alg =
{.alg_name = _bign_with_bash384_sig_name,
	.alg_printable_oid = _bign_with_bash384_sig_printable_oid,
	.alg_der_oid = _bign_with_bash384_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bign_with_bash384_sig_der_oid),
	.sig_id = SIG_ALG_BIGN,
	.hash_id = HASH_ALG_BASH384,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_bign};

static u8 const _bign_with_bash512_sig_name[40] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'S',
	(u8)'T',
	(u8)'B',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'3',
	(u8)')',
	(u8)' ',
	(u8)'u',
	(u8)'s',
	(u8)'i',
	(u8)'n',
	(u8)'g',
	(u8)' ',
	(u8)'B',
	(u8)'A',
	(u8)'S',
	(u8)'H',
	(u8)'5',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _bign_with_bash512_sig_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'1',
	(u8)'5',
(u8)'\000'};

static u8 const _bign_with_bash512_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
(u8)0x0f};

static _sig_alg const _bign_with_bash512_sig_alg =
{.alg_name = _bign_with_bash512_sig_name,
	.alg_printable_oid = _bign_with_bash512_sig_printable_oid,
	.alg_der_oid = _bign_with_bash512_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bign_with_bash512_sig_der_oid),
	.sig_id = SIG_ALG_BIGN,
	.hash_id = HASH_ALG_BASH512,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_bign};

static u8 const _bign_with_hspec_sig_name[45] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)' ',
	(u8)'(',
	(u8)'S',
	(u8)'T',
	(u8)'B',
	(u8)' ',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'3',
	(u8)')',
	(u8)' ',
	(u8)'w',
	(u8)'/',
	(u8)' ',
	(u8)'g',
	(u8)'i',
	(u8)'v',
	(u8)'e',
	(u8)'n',
	(u8)' ',
	(u8)'h',
	(u8)'a',
	(u8)'s',
	(u8)'h',
	(u8)' ',
	(u8)'f',
	(u8)'u',
	(u8)'n',
	(u8)'c',
(u8)'\000'};

static u8 const _bign_with_hspec_sig_printable_oid[27] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _bign_with_hspec_sig_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
(u8)0x0b};

static _sig_alg const _bign_with_hspec_sig_alg =
{.alg_name = _bign_with_hspec_sig_name,
	.alg_printable_oid = _bign_with_hspec_sig_printable_oid,
	.alg_der_oid = _bign_with_hspec_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bign_with_hspec_sig_der_oid),
	.sig_id = SIG_ALG_BIGN,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_bign_with_hspec,
.parse_sig = & parse_sig_bign};

static u8 const _monkeysphere_sig_name[42] =
{(u8)'u',
	(u8)'n',
	(u8)'k',
	(u8)'n',
	(u8)'o',
	(u8)'w',
	(u8)'n',
	(u8)' ',
	(u8)'O',
	(u8)'I',
	(u8)'D',
	(u8)' ',
	(u8)'f',
	(u8)'r',
	(u8)'o',
	(u8)'m',
	(u8)' ',
	(u8)'T',
	(u8)'h',
	(u8)'e',
	(u8)' ',
	(u8)'M',
	(u8)'o',
	(u8)'n',
	(u8)'k',
	(u8)'e',
	(u8)'y',
	(u8)'s',
	(u8)'p',
	(u8)'h',
	(u8)'e',
	(u8)'r',
	(u8)'e',
	(u8)' ',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'j',
	(u8)'e',
	(u8)'c',
	(u8)'t',
(u8)'\000'};

static u8 const _monkeysphere_sig_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'7',
	(u8)'2',
	(u8)'1',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _monkeysphere_sig_der_oid[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0xa2,
	(u8)0x5a,
	(u8)0x01,
(u8)0x01};

static _sig_alg const _monkeysphere_sig_alg =
{.alg_name = _monkeysphere_sig_name,
	.alg_printable_oid = _monkeysphere_sig_printable_oid,
	.alg_der_oid = _monkeysphere_sig_der_oid,
	.alg_der_oid_len = (u32)sizeof(_monkeysphere_sig_der_oid),
	.sig_id = SIG_ALG_MONKEYSPHERE,
	.hash_id = HASH_ALG_UNKNOWN,
	.parse_algoid_sig_params = & parse_algoid_sig_params_none,
.parse_sig = & parse_sig_monkey};

_sig_alg const *known_sig_algs[59] =
{& _ecdsa_sha1_sig_alg,
	& _ecdsa_sha224_sig_alg,
	& _ecdsa_sha256_sig_alg,
	& _ecdsa_sha384_sig_alg,
	& _ecdsa_sha512_sig_alg,
	& _ecdsa_with_sha3_256_sig_alg,
	& _ecdsa_with_shake128_sig_alg,
	& _ecdsa_with_shake256_sig_alg,
	& _rsassa_pss_sig_alg,
	& _rsassa_pss_shake128_sig_alg,
	& _rsassa_pss_shake256_sig_alg,
	& _rsa_mdc2_sig_alg,
	& _rsa_md2_sig_alg,
	& _rsa_md4_sig_alg,
	& _rsa_md5_sig_alg,
	& _rsa_sha224_sig_alg,
	& _rsa_sha256_sig_alg,
	& _rsa_sha384_sig_alg,
	& _rsa_sha512_sig_alg,
	& _sha1WithRSAEnc_sig_alg,
	& _sha1WithRSAEnc_bis_sig_alg,
	& _sha1WithRSAEnc_alt_sig_alg,
	& _sha1WithRSAEnc_alt2_sig_alg,
	& _sha1WithRSAEnc_ter_sig_alg,
	& _shaWithRSASig_9796_2_sig_alg,
	& _rsa_ripemd160_sig_alg,
	& _rsa_ripemd128_sig_alg,
	& _rsa_ripemd256_sig_alg,
	& _pkcs1_v15_w_sha3_224_sig_alg,
	& _pkcs1_v15_w_sha3_256_sig_alg,
	& _pkcs1_v15_w_sha3_384_sig_alg,
	& _pkcs1_v15_w_sha3_512_sig_alg,
	& _dsa_sha1_sig_alg,
	& _dsa_sha1_old_sig_alg,
	& _dsa_sha1_jdk_sig_alg,
	& _dsa_with_sha224_sig_alg,
	& _dsa_with_sha256_sig_alg,
	& _dsa_with_sha384_sig_alg,
	& _dsa_with_sha512_sig_alg,
	& _ed25519_sig_alg,
	& _ed448_sig_alg,
	& _sm2_sm3_sig_alg,
	& _gost_R3410_2012_256_sig_alg,
	& _gost_R3410_2012_512_sig_alg,
	& _gost_R3411_94_R3410_2001_sig_alg,
	& _gost_R3411_94_R3410_94_sig_alg,
	& _bign_with_hbelt_sig_alg,
	& _bign_with_bash256_sig_alg,
	& _bign_with_bash384_sig_alg,
	& _bign_with_bash512_sig_alg,
	& _bign_with_hspec_sig_alg,
	& _monkeysphere_sig_alg,
	& _belgian_rsa_sig_alg,
	& _rsalabs1_sig_alg,
	& _rsalabs2_sig_alg,
	& _dsa_sha1_bis_sig_alg,
	& _ecdsa_with_specified_sig_alg,
	& _rsa_sha1_crap_sig_alg,
& _rsa_sha1_crap_bis_sig_alg};

u16 const num_known_sig_algs =
(u16)(sizeof(known_sig_algs) / sizeof(known_sig_algs[0]));

/*@
	requires tag_num_valid: \valid(tag_num);
	requires eaten_valid: eaten != \null ==> \valid(eaten);
	requires buf_valid: (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
	requires separation: \separated(tag_num, eaten);
	assigns *tag_num, *eaten;
	ensures \result == 0 || \result == -(3*100000 + 468) ||@         \result == -(3*100000 + 506) || \result == -(3*100000 + 512);
*/
static int _extract_complex_tag(u8 const *buf, u32 len, u32 *tag_num,
u32 *eaten)
{
	u32 rbytes;
	int ret;
	u32 t = (u32)0;
	if (len == (u32)0) goto _LOR;
	else
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 468);
			goto out;
		}
	}
	if (len > (u32)4) len = (u32)4;
	rbytes = (u32)0;
	/*@
		loop invariant bounds: 0 <= rbytes <= len;
		loop invariant len_bound: 1 <= len <= 4;
		loop invariant buf_valid: \valid_read(buf + (0 .. len - 1));
		loop invariant t_bound: t <= 0x0FFFFFFF;
		loop assigns rbytes, t, tmp1, tmp2;
		loop variant len - rbytes;
	*/
	while (rbytes < len) {
		{
			u32 tmp1;
			u32 tmp2;
			tmp1 = t << (u32)7;
		/*@ assert rte_36: mem_access: \valid_read(buf + rbytes); */
			tmp2 = (u32)*(buf + rbytes) & (u32)0x7f;
			t = tmp1 + tmp2;
		/*@ assert rte_37: mem_access: \valid_read(buf + rbytes); */
			if (((int)*(buf + rbytes) & 0x80) == 0) break;
		}
		rbytes ++;
	}
	if (rbytes == len) {
		ret = - (3 * 100000 + 506);
		goto out;
	}
	if (t < (u32)0x1f) {
		ret = - (3 * 100000 + 512);
		goto out;
	}
/*@ assert rte_38: mem_access: \valid(tag_num); */
	*tag_num = t;
/*@ assert rte_39: mem_access: \valid(eaten); */
	*eaten = rbytes + (u32)1;
	ret = 0;
	out: ;
	return ret;
}

static int get_identifier(u8 const *buf, u32 len, tag_class *cls, u8 *prim,
u32 *tag_num, u32 *eaten)
{
	int ret;
	u32 t;
	u8 p;
	tag_class c;
	u32 rbytes = (u32)0;
	if (buf == (u8 const *)0) {
		ret = - (3 * 100000 + 568);
		goto out;
	}
	if (len == (u32)0) {
		ret = - (3 * 100000 + 579);
		goto out;
	}
/*@ assert rte_40: mem_access: \valid_read(buf + 0); */
	c = (tag_class)(((int)*(buf + 0) >> 6) & 0x03);
/*@ assert rte_41: mem_access: \valid_read(buf + 0); */
	p = (u8)(((int)*(buf + 0) >> 5) & 0x01);
/*@ assert rte_42: mem_access: \valid_read(buf + 0); */
	t = (u32)((int)*(buf + 0) & 0x1f);
	rbytes = (u32)1;
	switch (c) {
		case CLASS_UNIVERSAL: case CLASS_APPLICATION:
		case CLASS_CONTEXT_SPECIFIC: case CLASS_PRIVATE:
		break;
		default: ret = - (3 * 100000 + 602);
		goto out;
		break;
	}
	if (t == (u32)0x1f) {
		u32 tag_len = (u32)0;
		ret = _extract_complex_tag(buf + rbytes,len - rbytes,& t,& tag_len);
		if (ret) goto out;
		rbytes += tag_len;
	}
/*@ assert rte_43: mem_access: \valid(cls); */
	*cls = c;
/*@ assert rte_44: mem_access: \valid(prim); */
	*prim = p;
/*@ assert rte_45: mem_access: \valid(tag_num); */
	*tag_num = t;
/*@ assert rte_46: mem_access: \valid(eaten); */
	*eaten = rbytes;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(adv_len) || adv_len == \null;
	requires \valid(eaten) || eaten == \null;
	requires buf == \null || \valid_read(buf + (0 .. len - 1));
	assigns *adv_len, *eaten;
	ensures result_range: \result <= 0;
	ensures null_buf: buf == \null ==> \result == -(3 * 100000 + 674);
	ensures empty_buf: (buf != \null && len == 0) ==> \result == -(3 * 100000 + 680);
*/
int get_length(u8 const *buf, u32 len, u32 *adv_len, u32 *eaten)
{
	u32 l;
	u32 len_len;
	u32 b0;
	int ret;
	u32 rbytes = (u32)0;
	if (buf == (u8 const *)0) {
		ret = - (3 * 100000 + 674);
		goto out;
	}
	if (len == (u32)0) {
		ret = - (3 * 100000 + 680);
		goto out;
	}
/*@ assert rte_21: mem_access: \valid_read(buf + 0); */
	b0 = (u32)*(buf + 0);
	if ((b0 & (unsigned int)0x80) == (unsigned int)0) {
		l = b0 & (unsigned int)0x7f;
		if (l + (u32)1 > len) {
			ret = - (3 * 100000 + 700);
			goto out;
		}
	/*@ assert rte_22: mem_access: \valid(eaten); */
		*eaten = (u32)1;
	/*@ assert rte_23: mem_access: \valid(adv_len); */
		*adv_len = l;
		ret = 0;
		goto out;
	}
	if (b0 == (u32)0x80) {
		ret = - (3 * 100000 + 719);
		goto out;
	}
	len_len = b0 & (unsigned int)0x7f;
	rbytes += (u32)1;
	if (len_len + (u32)1 > len) {
		ret = - (3 * 100000 + 740);
		goto out;
	}
	switch (len_len) {
		case (u32)0: ret = - (3 * 100000 + 749);
		goto out;
		break;
		case (u32)1:
	/*@ assert rte_24: mem_access: \valid_read(buf + 1); */
		l = (u32)*(buf + 1);
		if (l <= (u32)127) {
			ret = - (3 * 100000 + 762);
			goto out;
		}
		rbytes += (u32)1;
		break;
		case (u32)2:
	/*@ assert rte_25: mem_access: \valid_read(buf + 1); */
	/*@ assert rte_26: mem_access: \valid_read(buf + 2); */
		l = ((u32)*(buf + 1) << 8) + (u32)*(buf + 2);
		if (l <= (u32)0xff) {
			ret = - (3 * 100000 + 775);
			goto out;
		}
		rbytes += (u32)2;
		break;
		case (u32)3:
	/*@ assert rte_27: mem_access: \valid_read(buf + 1); */
	/*@ assert rte_28: mem_access: \valid_read(buf + 2); */
	/*@ assert rte_29: mem_access: \valid_read(buf + 3); */
		l = (((u32)*(buf + 1) << 16) + ((u32)*(buf + 2) << 8)) + (u32)*(buf + 3);
		if (l <= (u32)0xffff) {
			ret = - (3 * 100000 + 788);
			goto out;
		}
		rbytes += (u32)3;
		break;
		case (u32)4:
	/*@ assert rte_30: mem_access: \valid_read(buf + 1); */
	/*@ assert rte_31: mem_access: \valid_read(buf + 2); */
	/*@ assert rte_32: mem_access: \valid_read(buf + 3); */
	/*@ assert rte_33: mem_access: \valid_read(buf + 4); */
		l = ((((u32)*(buf + 1) << 24) + ((u32)*(buf + 2) << 16)) + ((u32)*(
		buf + 3) << 8)) + (u32)*(
		buf + 4);
		if (l <= (u32)0xffffff) {
			ret = - (3 * 100000 + 801);
			goto out;
		}
		rbytes += (u32)4;
		break;
		default: ret = - (3 * 100000 + 816);
		goto out;
		break;
	}
	if (len - rbytes < l) {
		ret = - (3 * 100000 + 825);
		goto out;
	}
/*@ assert rte_34: mem_access: \valid(eaten); */
	*eaten = rbytes;
/*@ assert rte_35: mem_access: \valid(adv_len); */
	*adv_len = l;
	ret = 0;
	out: ;
	return ret;
}

int parse_id_len(u8 const *buf, u32 len, tag_class exp_class, u32 exp_type,
u32 *parsed, u32 *content_len)
{
	u8 p;
	u32 grabbed;
	int ret;
	tag_class c = (tag_class)0;
	u32 t = (u32)0;
	u32 cur_parsed = (u32)0;
	u32 adv_len = (u32)0;
	if (buf == (u8 const *)0) {
		ret = - (3 * 100000 + 883);
		goto out;
	}
	ret = get_identifier(buf,len,& c,& p,& t,& cur_parsed);
	if (ret) goto out;
	if (t != exp_type) {
		ret = - (3 * 100000 + 901);
		goto out;
	}
	if (c != exp_class) {
		ret = - (3 * 100000 + 908);
		goto out;
	}
	grabbed = cur_parsed;
	len -= cur_parsed;
	buf += cur_parsed;
	ret = get_length(buf,len,& adv_len,& cur_parsed);
	if (ret) goto out;
	grabbed += cur_parsed;
	len -= cur_parsed;
	buf += cur_parsed;
	if (adv_len > len) {
		ret = - (3 * 100000 + 933);
		goto out;
	}
/*@ assert rte_47: mem_access: \valid(parsed); */
	*parsed = grabbed;
/*@ assert rte_48: mem_access: \valid(content_len); */
	*content_len = adv_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_explicit_id_len(u8 const *buf, u32 len, u32 exp_ext_type,
tag_class exp_int_class, u32 exp_int_type,
u32 *parsed, u32 *data_len)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 val_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 982);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_CONTEXT_SPECIFIC,exp_ext_type,& hdr_len,
	& val_len);
	if (ret) goto out;
	buf += hdr_len;
	len -= hdr_len;
/*@ assert rte_49: mem_access: \valid(parsed); */
	*parsed = hdr_len;
	ret = parse_id_len(buf,len,exp_int_class,exp_int_type,& hdr_len,& val_len);
	if (ret) goto out;
	len -= hdr_len;
/*@ assert rte_50: mem_access: \valid(parsed); */
/*@ assert rte_51: mem_access: \valid_read(parsed); */
	*parsed += hdr_len;
	if (len < val_len) {
		ret = - (3 * 100000 + 1010);
		goto out;
	}
/*@ assert rte_52: mem_access: \valid(data_len); */
	*data_len = val_len;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(arc_val);
	requires \valid(eaten);
	requires (len == 0) || (buf == \null) || \valid_read(buf + (0 .. (len <= 4 ? len : 4) - 1));
	assigns *arc_val, *eaten;
*/
static int _parse_arc(u8 const *buf, u32 len, u32 *arc_val, u32 *eaten)
{
	u32 rbytes;
	int ret;
	u32 av = (u32)0;
	if (len == (u32)0) goto _LOR;
	else
	if (buf == (u8 const *)0) {
		_LOR: {
			ret = - (3 * 100000 + 1048);
			goto out;
		}
	}
	if (len > (u32)4) len = (u32)4;
	rbytes = (u32)0;
	/*@
		loop invariant 0 <= rbytes <= len;
		loop invariant len <= 4;
		loop invariant \forall integer i; 0 <= i < rbytes ==> (buf[i] & 0x80) != 0;
		loop invariant rbytes <= 3 ==> av <= (1 << (7 * (rbytes + 1))) - 1;
		loop invariant rbytes >= 1 ==> \valid_read(buf + (0 .. rbytes - 1));
		loop assigns rbytes, av, tmp1, tmp2;
		loop variant len - rbytes;
	*/
	while (rbytes < len) {
		{
			u32 tmp1;
			u32 tmp2;
			tmp1 = av << (u32)7;
		/*@ assert rte_54: mem_access: \valid_read(buf + rbytes); */
			tmp2 = (u32)*(buf + rbytes) & (u32)0x7f;
			av = tmp1 + tmp2;
		/*@ assert rte_55: mem_access: \valid_read(buf + rbytes); */
			if (((int)*(buf + rbytes) & 0x80) == 0) break;
		}
		rbytes ++;
	}
	if (rbytes >= len) {
		ret = - (3 * 100000 + 1095);
		goto out;
	}
/*@ assert rte_56: mem_access: \valid(arc_val); */
	*arc_val = av;
/*@ assert rte_57: mem_access: \valid(eaten); */
	*eaten = rbytes + (u32)1;
	ret = 0;
	out: ;
	return ret;
}

static u8 const null_encoded_val[2] = {(u8)0x05, (u8)0x00};

/*@
	requires (len == 0) || \valid_read(buf + (0 .. len-1));
	requires (parsed == \null) || \valid(parsed);
	assigns *parsed;
	ensures (len == 0 || buf == \null || parsed == \null) ==>\result == -(3 * 100000 + 1133);
	ensures (len != 0 && buf != \null && parsed != \null &&(unsigned long)len != sizeof(null_encoded_val)) ==>\result == -(3 * 100000 + 1139);
	ensures (len != 0 && buf != \null && parsed != \null &&(unsigned long)len == sizeof(null_encoded_val) &&(\exists integer k; 0 <= k < 2 && buf[k] != null_encoded_val[k])) ==>\result == -(3 * 100000 + 1146);
	ensures (len != 0 && buf != \null && parsed != \null &&(unsigned long)len == sizeof(null_encoded_val) &&(\forall integer k; 0 <= k < 2 ==> buf[k] == null_encoded_val[k])) ==>(\result == 0 && *parsed == (u32)sizeof(null_encoded_val));
	ensures \result == 0 ||\result == -(3 * 100000 + 1133) ||\result == -(3 * 100000 + 1139) ||\result == -(3 * 100000 + 1146);
*/
int parse_null(u8 const *buf, u32 len, u32 *parsed)
{
	int ret;
	if (len == (u32)0) goto _LOR;
	else
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (parsed == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 1133);
			goto out;
		}
	}
	if ((unsigned long)len != sizeof(null_encoded_val)) {
		ret = - (3 * 100000 + 1139);
		goto out;
	}
	ret = bufs_differ(buf,null_encoded_val,(u32)sizeof(null_encoded_val));
	if (ret) {
		ret = - (3 * 100000 + 1146);
		goto out;
	}
	ret = 0;
/*@ assert rte_53: mem_access: \valid(parsed); */
	*parsed = (u32)sizeof(null_encoded_val);
	out: ;
	return ret;
}

int parse_OID(u8 const *buf, u32 len, u32 *parsed)
{
	u32 num;
	int ret;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 1185);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_OID,& hdr_len,
	& data_len);
	if (ret) goto out;
	len -= hdr_len;
	buf += hdr_len;
	if (data_len < (u32)1) {
		ret = - (3 * 100000 + 1200);
		goto out;
	}
	remain = data_len;
	num = (u32)0;
	while (remain) {
		u32 arc_val = (u32)0;
		u32 rbytes = (u32)0;
		if (num > (u32)20) {
			ret = - (3 * 100000 + 1223);
			goto out;
		}
		ret = _parse_arc(buf,remain,& arc_val,& rbytes);
		if (ret) goto out;
		num += (u32)1;
		buf += rbytes;
		remain -= rbytes;
	}
	if (num < (u32)1) {
		ret = - (3 * 100000 + 1248);
		goto out;
	}
/*@ assert rte_58: mem_access: \valid(parsed); */
	*parsed = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int _parse_integer(u8 const *buf, u32 len, tag_class exp_class,
u32 exp_type, u32 *hdr_len, u32 *data_len,
int pos_or_zero)
{
	int ret;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (hdr_len == (u32 *)0) goto _LOR;
	else
	if (data_len == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 1292);
			goto out;
		}
	}
/*@ assert rte_59: mem_access: \valid(hdr_len); */
	*hdr_len = (u32)0;
/*@ assert rte_60: mem_access: \valid(data_len); */
	*data_len = (u32)0;
	ret = parse_id_len(buf,len,exp_class,exp_type,hdr_len,data_len);
	if (ret) goto out;
/*@ assert rte_61: mem_access: \valid_read(hdr_len); */
	buf += *hdr_len;
/*@ assert rte_62: mem_access: \valid_read(data_len); */
	if (*data_len == (u32)0) {
		ret = - (3 * 100000 + 1312);
		goto out;
	}
/*@ assert rte_63: mem_access: \valid_read(data_len); */
	if (*data_len > (u32)1) {
	/*@ assert rte_64: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) == 0)
	/*@ assert rte_65: mem_access: \valid_read(buf + 1); */
		if (((int)*(buf + 1) & 0x80) == 0) {
			ret = - (3 * 100000 + 1327);
			goto out;
		}
	/*@ assert rte_66: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) == 0xff)
	/*@ assert rte_67: mem_access: \valid_read(buf + 1); */
		if ((int)*(buf + 1) & 0x80) {
			ret = - (3 * 100000 + 1333);
			goto out;
		}
	}
	if (pos_or_zero)
/*@ assert rte_68: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) & 0x80) {
		ret = - (3 * 100000 + 1340);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

int parse_integer(u8 const *buf, u32 len, tag_class exp_class, u32 exp_type,
u32 *hdr_len, u32 *data_len)
{
	int tmp;
	tmp = _parse_integer(buf,len,exp_class,exp_type,hdr_len,data_len,0);
	return tmp;
}

int parse_non_negative_integer(u8 const *buf, u32 len, tag_class exp_class,
u32 exp_type, u32 *hdr_len, u32 *data_len)
{
	int tmp;
	tmp = _parse_integer(buf,len,exp_class,exp_type,hdr_len,data_len,1);
	return tmp;
}

/*@
	requires \valid(eaten);
	requires (buf == \null) || \valid_read(buf + (0 .. len - 1));
	assigns *eaten;
	ensures (buf == \null || len == (u32)0) ==> \result == -(3 * 100000 + 1428);
	ensures (buf != \null && len != (u32)0 && len < (u32)3) ==> \result == -(3 * 100000 + 1434);
	ensures (buf != \null && len >= (u32)3 && ((int)buf[0] != ASN1_TYPE_BOOLEAN || (int)buf[1] != 0x01)) ==> \result == -(3 * 100000 + 1440);
	ensures (buf != \null && len >= (u32)3 && (int)buf[0] == ASN1_TYPE_BOOLEAN && (int)buf[1] == 0x01 && (int)buf[2] != 0x00 && (int)buf[2] != 0xff) ==> \result == -(3 * 100000 + 1450);
	ensures (buf != \null && len >= (u32)3 && (int)buf[0] == ASN1_TYPE_BOOLEAN && (int)buf[1] == 0x01 && ((int)buf[2] == 0x00 || (int)buf[2] == 0xff)) ==> (\result == 0 && *eaten == (u32)3);
	ensures \result <= 0;
*/
int parse_boolean(u8 const *buf, u32 len, u32 *eaten)
{
	int ret;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 1428);
			goto out;
		}
	}
	if (len < (u32)3) {
		ret = - (3 * 100000 + 1434);
		goto out;
	}
/*@ assert rte_69: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != ASN1_TYPE_BOOLEAN) goto _LOR_0;
	else
/*@ assert rte_70: mem_access: \valid_read(buf + 1); */
	if ((int)*(buf + 1) != 0x01) {
		_LOR_0: {
			ret = - (3 * 100000 + 1440);
			goto out;
		}
	}
/*@ assert rte_71: mem_access: \valid_read(buf + 2); */
	switch ((int)*(buf + 2)) {
		case 0x00: case 0xff: break;
		default: ret = - (3 * 100000 + 1450);
		goto out;
		break;
	}
/*@ assert rte_72: mem_access: \valid(eaten); */
	*eaten = (u32)3;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires 0x30 <= d <= 0x39;
	requires 0x30 <= u <= 0x39;
	assigns \nothing;
	ensures \result == (u8)(((int)d - 0x30) * 10 + ((int)u - 0x30));
	ensures 0 <= \result <= 99;
*/
static u8 compute_decimal(u8 d, u8 u)
{
	u8 __retres;
/*@ assert rte_89: signed_overflow: -2147483648 <= (int)d - 0x30; */
/*@ assert rte_90: signed_overflow: -2147483648 <= (int)((int)d - 0x30) * 10; */
/*@ assert rte_91: signed_overflow: (int)((int)d - 0x30) * 10 <= 2147483647; */
/*@ assert rte_92: signed_overflow: -2147483648 <= (int)u - 0x30; */
/*@ assert rte_93: signed_overflow: -2147483648 <= (int)((int)((int)d - 0x30) * 10) + (int)((int)u - 0x30); */
/*@ assert rte_94: signed_overflow: (int)((int)((int)d - 0x30) * 10) + (int)((int)u - 0x30) <= 2147483647; */
	__retres = (u8)(((int)d - 0x30) * 10 + ((int)u - 0x30));
	return __retres;
}

/*@
	requires \valid(eaten);
	requires \valid(year);
	requires \valid(month);
	requires \valid(day);
	requires \valid(hour);
	requires \valid(min);
	requires \valid(sec);
	requires buf == \null || \valid_read(buf + (0 .. len - 1));
	requires \separated(eaten, year, month, day, hour, min, sec, buf + (..));
	assigns *eaten, *year, *month, *day, *hour, *min, *sec;
	ensures buf == \null ==> \result == -(3 * 100000 + 1516);
	ensures buf != \null && len < 15 ==> \result == -(3 * 100000 + 1529);
	ensures \result == 0 ==> *eaten == 15;
	ensures \result == 0 ==> 1950 <= *year <= 2049;
	ensures \result == 0 ==> 0 <= *month <= 12;
	ensures \result == 0 ==> 0 <= *day <= 31;
	ensures \result == 0 ==> 0 <= *hour <= 23;
	ensures \result == 0 ==> 0 <= *min <= 59;
	ensures \result == 0 ==> 0 <= *sec <= 59;
	ensures \result != 0 ==> *eaten == \old(*eaten);
	ensures \result != 0 ==> *year == \old(*year);
	ensures \result != 0 ==> *month == \old(*month);
	ensures \result != 0 ==> *day == \old(*day);
	ensures \result != 0 ==> *hour == \old(*hour);
	ensures \result != 0 ==> *min == \old(*min);
	ensures \result != 0 ==> *sec == \old(*sec);
	ensures \result == 0 || \result == -(3 * 100000 + 1512) ||\result == -(3 * 100000 + 1516) || \result == -(3 * 100000 + 1529) ||\result == -(3 * 100000 + 1536) || \result == -(3 * 100000 + 1543) ||\result == -(3 * 100000 + 1562) || \result == -(3 * 100000 + 1567) ||\result == -(3 * 100000 + 1574) || \result == -(3 * 100000 + 1613);
*/
static int parse_UTCTime(u8 const *buf, u32 len, u32 *eaten, u16 *year,
u8 *month, u8 *day, u8 *hour, u8 *min, u8 *sec)
{
	u16 yyyy;
	u8 mo;
	u8 dd;
	u8 hh;
	u8 mm;
	u8 ss;
	u8 time_type;
	u8 time_len;
	u8 i;
	u8 tmp;
	u8 const c_zero = (u8)'0';
	int ret = - (3 * 100000 + 1512);
	if (buf == (u8 const *)0) {
		ret = - (3 * 100000 + 1516);
		goto out;
	}
	if (len < (u32)15) {
		ret = - (3 * 100000 + 1529);
		goto out;
	}
/*@ assert rte_173: mem_access: \valid_read(buf + 0); */
	time_type = *(buf + 0);
	if ((int)time_type != ASN1_TYPE_UTCTime) {
		ret = - (3 * 100000 + 1536);
		goto out;
	}
/*@ assert rte_174: mem_access: \valid_read(buf + 1); */
	time_len = *(buf + 1);
	if ((int)time_len != 13) {
		ret = - (3 * 100000 + 1543);
		goto out;
	}
	buf += 2;
	i = (u8)0;
	/*@
		loop invariant 0 <= (int)i <= 12;
		loop invariant \valid_read(buf + (0 .. 12));
		loop assigns i;
		loop variant 12 - (int)i;
	*/
	while ((int)i < 12) {
	/*@ assert rte_175: mem_access: \valid_read(buf + (int)i); */
		if ((int)c_zero > (int)*(buf + (int)i)) {
			ret = - (3 * 100000 + 1562);
			goto out;
		}
	/*@ assert rte_176: mem_access: \valid_read(buf + (int)i); */
	/*@ assert rte_177: signed_overflow: -2147483648 <= (int)*(buf + (int)i) - (int)c_zero; */
	/*@ assert rte_178: signed_overflow: (int)*(buf + (int)i) - (int)c_zero <= 2147483647; */
		if ((int)*(buf + (int)i) - (int)c_zero > 9) {
			ret = - (3 * 100000 + 1567);
			goto out;
		}
	/*@ assert rte_179: signed_overflow: (int)i + 1 <= 2147483647; */
		i = (u8)((int)i + 1);
	}
/*@ assert rte_180: mem_access: \valid_read(buf + 12); */
	if ((int)*(buf + 12) != 'Z') {
		ret = - (3 * 100000 + 1574);
		goto out;
	}
/*@ assert rte_181: mem_access: \valid_read(buf + 0); */
/*@ assert rte_182: mem_access: \valid_read(buf + 1); */
	tmp = compute_decimal(*(buf + 0),*(buf + 1));
	yyyy = (u16)tmp;
	if ((int)yyyy >= 50)
/*@ assert rte_183: signed_overflow: (int)yyyy + 1900 <= 2147483647; */
	yyyy = (u16)((int)yyyy + 1900);
	else
/*@ assert rte_184: signed_overflow: (int)yyyy + 2000 <= 2147483647; */
	yyyy = (u16)((int)yyyy + 2000);
/*@ assert rte_185: mem_access: \valid_read(buf + 2); */
/*@ assert rte_186: mem_access: \valid_read(buf + 3); */
	mo = compute_decimal(*(buf + 2),*(buf + 3));
/*@ assert rte_187: mem_access: \valid_read(buf + 4); */
/*@ assert rte_188: mem_access: \valid_read(buf + 5); */
	dd = compute_decimal(*(buf + 4),*(buf + 5));
/*@ assert rte_189: mem_access: \valid_read(buf + 6); */
/*@ assert rte_190: mem_access: \valid_read(buf + 7); */
	hh = compute_decimal(*(buf + 6),*(buf + 7));
/*@ assert rte_191: mem_access: \valid_read(buf + 8); */
/*@ assert rte_192: mem_access: \valid_read(buf + 9); */
	mm = compute_decimal(*(buf + 8),*(buf + 9));
/*@ assert rte_193: mem_access: \valid_read(buf + 10); */
/*@ assert rte_194: mem_access: \valid_read(buf + 11); */
	ss = compute_decimal(*(buf + 10),*(buf + 11));
	if ((int)mo > 12) goto _LOR;
	else
	if ((int)dd > 31) goto _LOR;
	else
	if ((int)hh > 23) goto _LOR;
	else
	if ((int)mm > 59) goto _LOR;
	else
	if ((int)ss > 59) {
		_LOR: {
			ret = - (3 * 100000 + 1613);
			goto out;
		}
	}
/*@ assert rte_195: mem_access: \valid(year); */
	*year = yyyy;
/*@ assert rte_196: mem_access: \valid(month); */
	*month = mo;
/*@ assert rte_197: mem_access: \valid(day); */
	*day = dd;
/*@ assert rte_198: mem_access: \valid(hour); */
	*hour = hh;
/*@ assert rte_199: mem_access: \valid(min); */
	*min = mm;
/*@ assert rte_200: mem_access: \valid(sec); */
	*sec = ss;
	ret = 0;
	out:
	if (! ret)
/*@ assert rte_201: mem_access: \valid(eaten); */
	*eaten = (u32)15;
	return ret;
}

/*@
	requires valid_d1: 0x30 <= d1 <= 0x39;
	requires valid_d2: 0x30 <= d2 <= 0x39;
	requires valid_d3: 0x30 <= d3 <= 0x39;
	requires valid_d4: 0x30 <= d4 <= 0x39;
	assigns \nothing;
	ensures result_value: \result == (u16)(((d1 - 0x30) * 1000) +((d2 - 0x30) * 100) +((d3 - 0x30) * 10) +(d4 - 0x30));
	ensures result_bound: 0 <= \result <= 9999;
*/
static u16 compute_year(u8 d1, u8 d2, u8 d3, u8 d4)
{
	u16 __retres;
/*@ assert rte_73: signed_overflow: -2147483648 <= (int)((u16)d1) - 0x30; */
/*@ assert rte_74: signed_overflow: -2147483648 <= (int)((int)((u16)d1) - 0x30) * 1000; */
/*@ assert rte_75: signed_overflow: (int)((int)((u16)d1) - 0x30) * 1000 <= 2147483647; */
/*@ assert rte_76: signed_overflow: -2147483648 <= (int)((u16)d2) - 0x30; */
/*@ assert rte_77: signed_overflow: -2147483648 <= (int)((int)((u16)d2) - 0x30) * 100; */
/*@ assert rte_78: signed_overflow: (int)((int)((u16)d2) - 0x30) * 100 <= 2147483647; */
/*@ assert rte_79: signed_overflow: -2147483648 <= (int)((int)((int)((u16)d1) - 0x30) * 1000) + (int)((int)((int)((u16)d2) - 0x30) * 100); */
/*@ assert rte_80: signed_overflow: (int)((int)((int)((u16)d1) - 0x30) * 1000) + (int)((int)((int)((u16)d2) - 0x30) * 100) <= 2147483647; */
/*@ assert rte_81: signed_overflow: -2147483648 <= (int)((u16)d3) - 0x30; */
/*@ assert rte_82: signed_overflow: -2147483648 <= (int)((int)((u16)d3) - 0x30) * 10; */
/*@ assert rte_83: signed_overflow: (int)((int)((u16)d3) - 0x30) * 10 <= 2147483647; */
/*@ assert rte_84: signed_overflow: -2147483648 <= (int)((int)((int)((int)((u16)d1) - 0x30) * 1000) + (int)((int)((int)((u16)d2) - 0x30) * 100)) + (int)((int)((int)((u16)d3) - 0x30) * 10); */
/*@ assert rte_85: signed_overflow: (int)((int)((int)((int)((u16)d1) - 0x30) * 1000) + (int)((int)((int)((u16)d2) - 0x30) * 100)) + (int)((int)((int)((u16)d3) - 0x30) * 10) <= 2147483647; */
/*@ assert rte_86: signed_overflow: -2147483648 <= (int)((u16)d4) - 0x30; */
/*@ assert rte_87: signed_overflow: -2147483648 <= (int)((int)((int)((int)((int)((u16)d1) - 0x30) * 1000) + (int)((int)((int)((u16)d2) - 0x30) * 100)) + (int)((int)((int)((u16)d3) - 0x30) * 10)) + (int)((int)((u16)d4) - 0x30); */
/*@ assert rte_88: signed_overflow: (int)((int)((int)((int)((int)((u16)d1) - 0x30) * 1000) + (int)((int)((int)((u16)d2) - 0x30) * 100)) + (int)((int)((int)((u16)d3) - 0x30) * 10)) + (int)((int)((u16)d4) - 0x30) <= 2147483647; */
	__retres = (u16)(((((int)((u16)d1) - (int)((u16)0x30)) * 1000 + ((int)((u16)d2) - (int)((u16)0x30)) * 100) +
	((int)((u16)d3) - (int)((u16)0x30)) * 10) + ((int)((u16)d4) - (int)((u16)0x30)));
	return __retres;
}

/*@
	requires \separated(eaten, year, month, day, hour, min, sec, buf + (0 .. 16));
	requires \valid(sec);
	requires \valid(min);
	requires \valid(hour);
	requires \valid(day);
	requires \valid(month);
	requires \valid(year);
	requires \valid(eaten);
	requires \valid_read(buf + (0 .. 16)) || buf == \null;
	assigns *eaten, *year, *month, *day, *hour, *min, *sec;
	ensures (buf == \null) ==> (\result == -(3 * 100000 + 1698));
	ensures (buf != \null && len < 17) ==> (\result == -(3 * 100000 + 1711));
	ensures (buf != \null && len >= 17) ==> (\result == 0);
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (\result == 0);
	ensures (buf[1] == 15) ==> (\result == 0);
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (\result == 0);
	ensures (buf[16] == 'Z') ==> (\result == 0);
	ensures (buf != \null && len >= 17) ==> (*eaten == 17);
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*eaten == 17);
	ensures (buf[1] == 15) ==> (*eaten == 17);
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*eaten == 17);
	ensures (buf[16] == 'Z') ==> (*eaten == 17);
	ensures (buf != \null && len >= 17) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures (buf[1] == 15) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures (buf[16] == 'Z') ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures (buf != \null && len >= 17) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures (buf[1] == 15) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures (buf[16] == 'Z') ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures (buf != \null && len >= 17) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures (buf[1] == 15) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures (buf[16] == 'Z') ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures (buf != \null && len >= 17) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures (buf[1] == 15) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures (buf[16] == 'Z') ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures (buf != \null && len >= 17) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures (buf[1] == 15) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures (buf[16] == 'Z') ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures (buf != \null && len >= 17) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures (buf[0] == ASN1_TYPE_GeneralizedTime) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures (buf[1] == 15) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures (\forall integer k; 0 <= k < 14 ==> 0x30 <= buf[2 + k] <= 0x39) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures (buf[16] == 'Z') ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures (buf != \null && len >= 17) ==> (\result < 0);
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (\result == 0);
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (\result == 0);
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (\result == 0);
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (\result == 0);
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (\result == 0);
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*eaten == 17);
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*eaten == 17);
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*eaten == 17);
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*eaten == 17);
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*eaten == 17);
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*year == (u16)(((buf[2] - 0x30) * 1000) + ((buf[3] - 0x30) * 100) + ((buf[4] - 0x30) * 10) + (buf[5] - 0x30)));
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*month == (u8)(((buf[6] - 0x30) * 10) + (buf[7] - 0x30)));
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*day == (u8)(((buf[8] - 0x30) * 10) + (buf[9] - 0x30)));
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*hour == (u8)(((buf[10] - 0x30) * 10) + (buf[11] - 0x30)));
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*min == (u8)(((buf[12] - 0x30) * 10) + (buf[13] - 0x30)));
	ensures ((buf[6] - 0x30) * 10 + (buf[7] - 0x30) <= 12) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures ((buf[8] - 0x30) * 10 + (buf[9] - 0x30) <= 31) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures ((buf[10] - 0x30) * 10 + (buf[11] - 0x30) <= 23) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures ((buf[12] - 0x30) * 10 + (buf[13] - 0x30) <= 59) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures ((buf[14] - 0x30) * 10 + (buf[15] - 0x30) <= 59) ==> (*sec == (u8)(((buf[14] - 0x30) * 10) + (buf[15] - 0x30)));
	ensures (buf[0] != ASN1_TYPE_GeneralizedTime ||buf[1] != 15 ||(\exists integer k; 0 <= k < 14 && !(0x30 <= buf[2 + k] <= 0x39)) ||buf[16] != 'Z' ||(buf[6] - 0x30) * 10 + (buf[7] - 0x30) > 12 ||(buf[8] - 0x30) * 10 + (buf[9] - 0x30) > 31 ||(buf[10] - 0x30) * 10 + (buf[11] - 0x30) > 23 ||(buf[12] - 0x30) * 10 + (buf[13] - 0x30) > 59 ||(buf[14] - 0x30) * 10 + (buf[15] - 0x30) > 59) ==> (\result < 0);
*/
int parse_generalizedTime(u8 const *buf, u32 len, u32 *eaten, u16 *year,
u8 *month, u8 *day, u8 *hour, u8 *min, u8 *sec)
{
	u16 yyyy;
	u8 mo;
	u8 dd;
	u8 hh;
	u8 mm;
	u8 ss;
	u8 time_type;
	u8 time_len;
	u8 i;
	u8 const c_zero = (u8)'0';
	int ret = - (3 * 100000 + 1694);
	if (buf == (u8 const *)0) {
		ret = - (3 * 100000 + 1698);
		goto out;
	}
	if (len < (u32)17) {
		ret = - (3 * 100000 + 1711);
		goto out;
	}
/*@ assert rte_95: mem_access: \valid_read(buf + 0); */
	time_type = *(buf + 0);
	if ((int)time_type != ASN1_TYPE_GeneralizedTime) {
		ret = - (3 * 100000 + 1718);
		goto out;
	}
/*@ assert rte_96: mem_access: \valid_read(buf + 1); */
	time_len = *(buf + 1);
	if ((int)time_len != 15) {
		ret = - (3 * 100000 + 1725);
		goto out;
	}
	buf += 2;
	i = (u8)0;
	/*@
		loop invariant i_bounds: 0 <= (int)i <= 14;
		loop invariant digit_check: \forall integer k; 0 <= k < (int)i ==>(0x30 <= buf[k] <= 0x39);
		loop invariant ret_unchanged: ret == -(3 * 100000 + 1694);
		loop invariant buf_valid: \valid_read(buf + (0 .. 14));
		loop assigns i;
		loop variant 14 - (int)i;
	*/
	while ((int)i < 14) {
	/*@ assert rte_97: mem_access: \valid_read(buf + (int)i); */
		if ((int)c_zero > (int)*(buf + (int)i)) {
			ret = - (3 * 100000 + 1744);
			goto out;
		}
	/*@ assert rte_98: mem_access: \valid_read(buf + (int)i); */
	/*@ assert rte_99: signed_overflow: -2147483648 <= (int)*(buf + (int)i) - (int)c_zero; */
	/*@ assert rte_100: signed_overflow: (int)*(buf + (int)i) - (int)c_zero <= 2147483647; */
		if ((int)*(buf + (int)i) - (int)c_zero > 9) {
			ret = - (3 * 100000 + 1749);
			goto out;
		}
	/*@ assert rte_101: signed_overflow: (int)i + 1 <= 2147483647; */
		i = (u8)((int)i + 1);
	}
/*@ assert rte_102: mem_access: \valid_read(buf + 14); */
	if ((int)*(buf + 14) != 'Z') {
		ret = - (3 * 100000 + 1755);
		goto out;
	}
/*@ assert rte_103: mem_access: \valid_read(buf + 0); */
/*@ assert rte_104: mem_access: \valid_read(buf + 1); */
/*@ assert rte_105: mem_access: \valid_read(buf + 2); */
/*@ assert rte_106: mem_access: \valid_read(buf + 3); */
	yyyy = compute_year(*(buf + 0),*(buf + 1),*(buf + 2),*(buf + 3));
/*@ assert rte_107: mem_access: \valid_read(buf + 4); */
/*@ assert rte_108: mem_access: \valid_read(buf + 5); */
	mo = compute_decimal(*(buf + 4),*(buf + 5));
/*@ assert rte_109: mem_access: \valid_read(buf + 6); */
/*@ assert rte_110: mem_access: \valid_read(buf + 7); */
	dd = compute_decimal(*(buf + 6),*(buf + 7));
/*@ assert rte_111: mem_access: \valid_read(buf + 8); */
/*@ assert rte_112: mem_access: \valid_read(buf + 9); */
	hh = compute_decimal(*(buf + 8),*(buf + 9));
/*@ assert rte_113: mem_access: \valid_read(buf + 10); */
/*@ assert rte_114: mem_access: \valid_read(buf + 11); */
	mm = compute_decimal(*(buf + 10),*(buf + 11));
/*@ assert rte_115: mem_access: \valid_read(buf + 12); */
/*@ assert rte_116: mem_access: \valid_read(buf + 13); */
	ss = compute_decimal(*(buf + 12),*(buf + 13));
	if ((int)mo > 12) goto _LOR;
	else
	if ((int)dd > 31) goto _LOR;
	else
	if ((int)hh > 23) goto _LOR;
	else
	if ((int)mm > 59) goto _LOR;
	else
	if ((int)ss > 59) {
		_LOR: {
			ret = - (3 * 100000 + 1784);
			goto out;
		}
	}
/*@ assert rte_117: mem_access: \valid(year); */
	*year = yyyy;
/*@ assert rte_118: mem_access: \valid(month); */
	*month = mo;
/*@ assert rte_119: mem_access: \valid(day); */
	*day = dd;
/*@ assert rte_120: mem_access: \valid(hour); */
	*hour = hh;
/*@ assert rte_121: mem_access: \valid(min); */
	*min = mm;
/*@ assert rte_122: mem_access: \valid(sec); */
	*sec = ss;
	ret = 0;
	out:
	if (! ret)
/*@ assert rte_123: mem_access: \valid(eaten); */
	*eaten = (u32)17;
	return ret;
}

/*@
	requires \valid_read(buf + (0 .. len - 1)) || buf == \null || len == 0;
	requires len <= 0xFFFFFFFF;
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 1833);
	ensures \result == 0 || \result < 0;
	ensures \result == 0 ||@         \result == -(3 * 100000 + 1833) ||@         \result == -(3 * 100000 + 1864) ||@         \result == -(3 * 100000 + 1871) ||@         \result == -(3 * 100000 + 1888) ||@         \result == -(3 * 100000 + 1896) ||@         \result == -(3 * 100000 + 1907) ||@         \result == -(3 * 100000 + 1912) ||@         \result == -(3 * 100000 + 1929) ||@         \result == -(3 * 100000 + 1941) ||@         \result == -(3 * 100000 + 1946) ||@         \result == -(3 * 100000 + 1954) ||@         \result == -(3 * 100000 + 1964);
*/
static int check_utf8_string(u8 const *buf, u32 len)
{
	int ret;
	u8 b0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 1833);
			goto out;
		}
	}
	/*@
		loop invariant ptr_relation: buf + len == \at(buf, Pre) + \at(len, Pre);
		loop invariant len_bound: len <= \at(len, Pre);
		loop invariant valid_buf: buf != \null ==> \valid_read(buf + (0 .. len - 1));
		loop assigns buf, len;
		loop variant len;
	*/
	while (len) {
	/*@ assert rte_130: mem_access: \valid_read(buf + 0); */
		b0 = *(buf + 0);
		if ((int)b0 <= 0x7f) {
			len -= (u32)1;
			buf ++;
			continue;
		}
		if ((int)b0 >= 0xc2)
		if ((int)b0 <= 0xdf) {
			if (len < (u32)2) {
				ret = - (3 * 100000 + 1864);
				goto out;
			}
		/*@ assert rte_131: mem_access: \valid_read(buf + 1); */
			if (((int)*(buf + 1) & 0xc0) != 0x80) {
				ret = - (3 * 100000 + 1871);
				goto out;
			}
			len -= (u32)2;
			buf += 2;
			continue;
		}
		if ((int)b0 >= 0xe0)
		if ((int)b0 <= 0xef) {
			if (len < (u32)3) {
				ret = - (3 * 100000 + 1888);
				goto out;
			}
		/*@ assert rte_132: mem_access: \valid_read(buf + 1); */
			if (((int)*(buf + 1) & 0xc0) != 0x80) goto _LOR_0;
			else
		/*@ assert rte_133: mem_access: \valid_read(buf + 2); */
			if (((int)*(buf + 2) & 0xc0) != 0x80) {
				_LOR_0: {
					ret = - (3 * 100000 + 1896);
					goto out;
				}
			}
			if ((int)b0 == 0xe0) {
			/*@ assert rte_134: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) < 0xa0) goto _LOR_2;
				else
			/*@ assert rte_135: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) > 0xbf) {
					_LOR_2: {
						ret = - (3 * 100000 + 1907);
						goto out;
					}
				}
				else goto _LAND;
			}
			else {
				_LAND: ;
				if ((int)b0 == 0xed)
			/*@ assert rte_136: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) < 0x80) goto _LOR_1;
				else
			/*@ assert rte_137: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) > 0x9f) {
					_LOR_1: {
						ret = - (3 * 100000 + 1912);
						goto out;
					}
				}
			}
			len -= (u32)3;
			buf += 3;
			continue;
		}
		if ((int)b0 >= 0xf0)
		if ((int)b0 <= 0xf4) {
			if (len < (u32)4) {
				ret = - (3 * 100000 + 1929);
				goto out;
			}
			if ((int)b0 == 0xf0) {
			/*@ assert rte_138: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) < 0x90) goto _LOR_4;
				else
			/*@ assert rte_139: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) > 0xbf) {
					_LOR_4: {
						ret = - (3 * 100000 + 1941);
						goto out;
					}
				}
				else goto _LAND_0;
			}
			else {
				_LAND_0: ;
				if ((int)b0 == 0xf4)
			/*@ assert rte_140: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) < 0x80) goto _LOR_3;
				else
			/*@ assert rte_141: mem_access: \valid_read(buf + 1); */
				if ((int)*(buf + 1) > 0x8f) {
					_LOR_3: {
						ret = - (3 * 100000 + 1946);
						goto out;
					}
				}
			}
		/*@ assert rte_142: mem_access: \valid_read(buf + 1); */
			if (((int)*(buf + 1) & 0xc0) != 0x80) goto _LOR_5;
			else
		/*@ assert rte_143: mem_access: \valid_read(buf + 2); */
			if (((int)*(buf + 2) & 0xc0) != 0x80) goto _LOR_5;
			else
		/*@ assert rte_144: mem_access: \valid_read(buf + 3); */
			if (((int)*(buf + 3) & 0xc0) != 0x80) {
				_LOR_5: {
					ret = - (3 * 100000 + 1954);
					goto out;
				}
			}
			len -= (u32)4;
			buf += 4;
			continue;
		}
		ret = - (3 * 100000 + 1964);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires (buf != \null && len > 0) ==> \valid_read(buf + (0 .. len - 1));
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 2001);
	ensures (buf != \null && len != 0) ==>@         (\result == 0 || \result == -(3 * 100000 + 2038));
	ensures (buf != \null && len != 0 &&@          (\forall u32 i; 0 <= i < len ==> is_printable_char(buf[i]))) ==>@         \result == 0;
	ensures (buf != \null && len != 0 &&@          (\exists u32 i; 0 <= i < len && !is_printable_char(buf[i]))) ==>@         \result == -(3 * 100000 + 2038);
*/
static int check_printable_string(u8 const *buf, u32 len)
{
	int ret;
	u32 rbytes;
	u8 c;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2001);
			goto out;
		}
	}
	rbytes = (u32)0;
	/*@
		loop invariant 0 <= rbytes <= len;
		loop invariant \valid_read(buf + (0 .. len - 1));
		loop assigns rbytes, c;
		loop variant len - rbytes;
	*/
	while (rbytes < len) {
	/*@ assert rte_129: mem_access: \valid_read(buf + rbytes); */
		c = *(buf + rbytes);
		if ((int)c >= 'a') {
			if ((int)c <= 'z') goto _LOR_0; else goto _LAND_0;
		}
		else {
			_LAND_0: ;
			if ((int)c >= 'A') {
				if ((int)c <= 'Z') goto _LOR_0; else goto _LAND;
			}
			else {
				_LAND: ;
				if ((int)c >= '0')
				if ((int)c <= '9') _LOR_0: goto __Cont;
			}
		}
		switch ((int)c) {
			case 39: case '=': case '(': case ')': case '+': case ',': case '-':
			case '.': case '/': case ':': case '?': case ' ':
			goto __Cont;
			default: break;
		}
		ret = - (3 * 100000 + 2038);
		goto out;
		__Cont: rbytes ++;
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires buf == \null || len == 0 || \valid_read(buf + (0 .. len - 1));
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == -(3*100000 + 2069);
	ensures (buf != \null && len != 0 && all_numeric(buf, len)) ==> \result == 0;
	ensures (buf != \null && len != 0 && !all_numeric(buf, len)) ==>@         \result == -(3*100000 + 2083);
	ensures \result == 0 || \result == -(3*100000 + 2069) || \result == -(3*100000 + 2083);
*/
static int check_numeric_string(u8 const *buf, u32 len)
{
	int ret;
	u32 rbytes;
	u8 c;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2069);
			goto out;
		}
	}
	rbytes = (u32)0;
	/*@
		loop invariant 0 <= rbytes <= len;
		loop invariant \valid_read(buf + (0 .. len - 1));
		loop invariant \forall integer i; 0 <= i < rbytes ==> '0' <= buf[i] <= '9';
		loop assigns rbytes, c;
		loop variant len - rbytes;
	*/
	while (rbytes < len) {
	/*@ assert rte_146: mem_access: \valid_read(buf + rbytes); */
		c = *(buf + rbytes);
		if ((int)c < '0') goto _LOR_0;
		else
		if ((int)c > '9') {
			_LOR_0: {
				ret = - (3 * 100000 + 2083);
				goto out;
			}
		}
		rbytes ++;
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires valid_buf_or_null: buf == \null || len == 0 || \valid_read(buf + (0 .. len - 1));
	assigns \nothing;
	ensures (buf == \null) ==> (\result == -(3 * 100000 + 2114));
	ensures (buf != \null && len == 0) ==> (\result == -(3 * 100000 + 2114));
	ensures (buf != \null && len > 0) ==> (\result == 0 || \result == -(3 * 100000 + 2172));
	ensures result_range: \result == 0 || \result == -(3 * 100000 + 2114) || \result == -(3 * 100000 + 2172);
*/
static int check_visible_string(u8 const *buf, u32 len)
{
	int ret;
	u8 c;
	u32 rbytes = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2114);
			goto out;
		}
	}
	/*@
		loop invariant bounds: 0 <= rbytes <= len;
		loop invariant valid_access: \valid_read(buf + (0 .. len - 1));
		loop assigns rbytes, c;
		loop variant len - rbytes;
	*/
	while (rbytes < len) {
	/*@ assert rte_145: mem_access: \valid_read(buf + rbytes); */
		c = *(buf + rbytes);
		if ((int)c >= 'a') {
			if ((int)c <= 'z') goto _LOR_0; else goto _LAND_0;
		}
		else {
			_LAND_0: ;
			if ((int)c >= 'A') {
				if ((int)c <= 'Z') goto _LOR_0; else goto _LAND;
			}
			else {
				_LAND: ;
				if ((int)c >= '0')
				if ((int)c <= '9') {
					_LOR_0: {
						rbytes += (u32)1;
						continue;
					}
				}
			}
		}
		switch ((int)c) {
			case 39: case '=': case '(': case ')': case '+': case ',': case '-':
			case '.': case '/': case ':': case '?': case ' ': case '!': case '\"':
			case '#': case '$': case '%': case '&': case '*': case ';': case '<':
			case '>': case '[': case '\\': case ']': case '^': case '_': case '`':
		case '{': case '|': case '}': case '~':
			rbytes += (u32)1;
			continue;
			default: break;
		}
		ret = - (3 * 100000 + 2172);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires valid_buf_or_null: buf == \null || len == 0 || \valid_read(buf + (0 .. len - 1));
	assigns \nothing;
	ensures null_buffer: (buf == \null) ==> (\result == -(3 * 100000 + 2114));
	ensures zero_length: (buf != \null && len == 0) ==> (\result == -(3 * 100000 + 2114));
	ensures valid_input: (buf != \null && len > 0) ==> (\result == 0 || \result == -(3 * 100000 + 2172));
	ensures result_range: \result == 0 || \result == -(3 * 100000 + 2114) || \result == -(3 * 100000 + 2172);
*/
static int check_teletex_string(u8 const *buf, u32 len)
{
	int tmp;
	tmp = check_visible_string(buf,len);
	return tmp;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result == -302215;
*/
static int check_universal_string(u8 const *buf, u32 len)
{
	int __retres;
	__retres = - (3 * 100000 + 2215);
	return __retres;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result == -302232;
*/
static int check_bmp_string(u8 const *buf, u32 len)
{
	int __retres;
	__retres = - (3 * 100000 + 2232);
	return __retres;
}

/*@
	requires (buf == \null) || (len == 0) || \valid_read(buf + (0..len-1));
	assigns \nothing;
	ensures (buf == \null || len == 0) ==>\result == -(3 * 100000 + 2250);
	ensures (buf != \null && len != 0 &&(\exists u32 k; 0 <= k < len && (int)buf[k] > 0x7f)) ==>\result == -(3 * 100000 + 2263);
	ensures (buf != \null && len != 0 &&(\forall u32 k; 0 <= k < len ==> (int)buf[k] <= 0x7f)) ==>\result == 0;
	ensures \result == 0 ||\result == -(3 * 100000 + 2250) ||\result == -(3 * 100000 + 2263);
*/
static int check_ia5_string(u8 const *buf, u32 len)
{
	int ret;
	u32 i;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2250);
			goto out;
		}
	}
	i = (u32)0;
	/*@
		loop invariant 0 <= i <= len;
		loop invariant \forall integer k; 0 <= k < i ==> (int)buf[k] <= 0x7f;
		loop assigns i;
		loop variant len - i;
	*/
	while (i < len) {
	/*@ assert rte_124: mem_access: \valid_read(buf + i); */
		if ((int)*(buf + i) > 0x7f) {
			ret = - (3 * 100000 + 2263);
			goto out;
		}
		i ++;
	}
	ret = 0;
	out: ;
	return ret;
}

int parse_GeneralName(u8 const *buf, u32 len, u32 *eaten, int *empty)
{
	u8 name_type;
	int ret;
	u32 remain = (u32)0;
	u32 name_len = (u32)0;
	u32 name_hdr_len = (u32)0;
	u32 grabbed = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2321);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 2327);
		goto out;
	}
	remain = len;
/*@ assert rte_164: mem_access: \valid_read(buf + 0); */
	name_type = *(buf + 0);
	if (! ((int)name_type & 0x80)) {
		ret = - (3 * 100000 + 2340);
		goto out;
	}
	switch ((int)name_type) {
		case 0x81: case 0x82: case 0x86: buf ++;
		remain -= (u32)1;
		ret = get_length(buf,remain,& name_len,& grabbed);
		if (ret) goto out;
		buf += grabbed;
		remain -= grabbed;
		if (name_len > remain) {
			ret = - (3 * 100000 + 2361);
			goto out;
		}
		ret = check_ia5_string(buf,name_len);
		if (ret) goto out;
		switch ((int)name_type) {
			case 0x81: break;
			case 0x82: break;
			case 0x86: break;
			default: break;
		}
		remain -= name_len;
		buf += name_len;
	/*@ assert rte_165: mem_access: \valid(eaten); */
		*eaten = (name_len + grabbed) + (u32)1;
	/*@ assert rte_166: mem_access: \valid(empty); */
		*empty = ! name_len;
		break;
		case 0x87: buf ++;
		remain -= (u32)1;
		ret = get_length(buf,remain,& name_len,& grabbed);
		if (ret) goto out;
		buf += grabbed;
		remain -= grabbed;
		if (name_len > remain) {
			ret = - (3 * 100000 + 2418);
			goto out;
		}
		remain -= name_len;
		buf += name_len;
	/*@ assert rte_167: mem_access: \valid(eaten); */
		*eaten = (name_len + grabbed) + (u32)1;
	/*@ assert rte_168: mem_access: \valid(empty); */
		*empty = ! name_len;
		break;
		case 0xa0: ret = - (3 * 100000 + 2434);
		goto out;
		break;
		case 0xa3: ret = - (3 * 100000 + 2441);
		goto out;
		break;
		case 0xa4:
		ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)4,
		& name_hdr_len,& name_len);
		if (ret) goto out;
		buf += name_hdr_len;
		remain = name_len;
		ret = parse_x509_Name(buf,remain,& grabbed,empty);
		if (ret) goto out;
		buf += grabbed;
		remain -= grabbed;
		if (remain) {
			ret = - (3 * 100000 + 2467);
			goto out;
		}
	/*@ assert rte_169: mem_access: \valid(eaten); */
		*eaten = name_hdr_len + name_len;
		break;
		case 0xa5: ret = - (3 * 100000 + 2478);
		goto out;
		break;
		case 0x88: ret = - (3 * 100000 + 2485);
		goto out;
		break;
		default: ret = - (3 * 100000 + 2491);
		goto out;
		break;
	}
	ret = 0;
	out: ;
	return ret;
}

int parse_GeneralNames(u8 const *buf, u32 len, tag_class exp_class,
u32 exp_type, u32 *eaten)
{
	u32 remain;
	int ret;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	int unused = 0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2525);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,exp_class,exp_type,& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	while (remain) {
		ret = parse_GeneralName(buf,remain,& parsed,& unused);
		if (ret) goto out;
		remain -= parsed;
		buf += parsed;
	}
/*@ assert rte_230: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_SerialNumber(u8 const *cert, u32 off, u32 len, tag_class exp_class,
u32 exp_type, u32 *eaten)
{
	int ret;
	u8 const *buf = cert + off;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 2597);
			goto out;
		}
	}
	ret = parse_integer(buf,len,exp_class,exp_type,& hdr_len,& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
/*@ assert rte_170: mem_access: \valid(eaten); */
	*eaten = parsed;
	if (data_len == (u32)1)
/*@ assert rte_171: mem_access: \valid_read(buf + 2); */
	if ((int)*(buf + 2) == 0) {
		ret = 0;
		goto out;
	}
	if (parsed > (u32)22) ret = 0;
/*@ assert rte_172: mem_access: \valid_read(buf + 2); */
	if ((int)*(buf + 2) & 0x80) ret = 0;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \true;
	assigns \nothing;
*/
int verify_correct_time_use(u8 time_type, u16 yyyy)
{
	int ret;
	switch ((int)time_type) {
		case ASN1_TYPE_UTCTime: ;
		if ((int)yyyy <= 2049) ret = 0; else ret = - (3 * 100000 + 2695);
		break;
		case ASN1_TYPE_GeneralizedTime: ;
		if ((int)yyyy >= 2050) ret = 0; else ret = - (3 * 100000 + 2698);
		break;
		default: ret = -1;
		break;
	}
	return ret;
}

/*@
	requires \valid(t_type);
	requires \valid(eaten);
	requires \valid(year);
	requires \valid(month);
	requires \valid(day);
	requires \valid(hour);
	requires \valid(min);
	requires \valid(sec);
	requires buf == \null || \valid_read(buf + (0 .. len - 1));
	requires \separated(t_type, eaten, year, month, day, hour, min, sec, buf + (..));
	assigns *t_type, *eaten, *year, *month, *day, *hour, *min, *sec;
	ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 2748);
	ensures (buf != \null && len != 0 && len < 2) ==> \result == -(3 * 100000 + 2754);
	ensures (buf != \null && len >= 2 && buf[0] != ASN1_TYPE_UTCTime && buf[0] != ASN1_TYPE_GeneralizedTime) ==> \result == -(3 * 100000 + 2776);
	ensures \result != 0 ==> *eaten == 0;
	ensures \result == 0 ==> (*t_type == ASN1_TYPE_UTCTime || *t_type == ASN1_TYPE_GeneralizedTime);
	ensures (buf != \null && len >= 2 && \result == 0) ==> *t_type == buf[0];
	ensures (buf != \null && len >= 2 && buf[0] == ASN1_TYPE_UTCTime && \result == 0) ==> *eaten == 15;
	ensures (buf != \null && len >= 2 && buf[0] == ASN1_TYPE_GeneralizedTime && \result == 0) ==> *eaten == 17;
	ensures \result == 0 ==> (1 <= *month <= 12);
	ensures \result == 0 ==> (1 <= *day <= 31);
	ensures \result == 0 ==> (0 <= *hour <= 23);
	ensures \result == 0 ==> (0 <= *min <= 59);
	ensures \result == 0 ==> (0 <= *sec <= 59);
	ensures (buf != \null && len >= 2 && buf[0] == ASN1_TYPE_UTCTime && \result == 0) ==> (1950 <= *year <= 2049);
	ensures (buf != \null && len >= 2 && buf[0] == ASN1_TYPE_GeneralizedTime && \result == 0) ==> (0 <= *year <= 9999);
	ensures \result == 0 || \result < 0;
*/
int parse_Time(u8 const *buf, u32 len, u8 *t_type, u32 *eaten, u16 *year,
u8 *month, u8 *day, u8 *hour, u8 *min, u8 *sec)
{
	u8 time_type;
	int ret = - (3 * 100000 + 2745);
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 2748);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 2754);
		goto out;
	}
/*@ assert rte_202: mem_access: \valid_read(buf + 0); */
	time_type = *(buf + 0);
	switch ((int)time_type) {
		case ASN1_TYPE_UTCTime:
		ret = parse_UTCTime(buf,len,eaten,year,month,day,hour,min,sec);
		int tmp = ret;
		break;
		case ASN1_TYPE_GeneralizedTime:
		ret = parse_generalizedTime(buf,len,eaten,year,month,day,hour,min,sec);
		int tmp_3 = ret;
		break;
		default: ret = - (3 * 100000 + 2776);
		break;
	}
/*@ assert rte_203: mem_access: \valid(t_type); */
	*t_type = time_type;
	out: ;
	if (ret)
/*@ assert rte_204: mem_access: \valid(eaten); */
	*eaten = (u32)0;
	return ret;
}

int parse_AKICertSerialNumber(u8 const *cert, u32 off, u32 len,
tag_class exp_class, u32 exp_type, u32 *eaten)
{
	int ret;
	ret = parse_SerialNumber(cert,off,len,exp_class,exp_type,eaten);
	if (ret) goto out;
	out: ;
	return ret;
}

/*@
	requires \valid(val);
	requires len <= 3;
	requires len >= 1 ==> \valid_read(buf + (0 .. len - 1));
	assigns \result, *val;
	ensures (buf == \null || len == 0 || val == \null|| ((int)buf[0] & 0xf8) != 0|| len == 1|| (len == 2 && (int)buf[1] == 0)|| (len == 3 && ((int)buf[0] != 7 || (int)buf[2] != 0x80))|| len > 3) ==> (\result < 0);
	ensures (buf != \null && val != \null && len >= 2 && len <= 3) ==> (\result == 0 || \result < 0);
	ensures (((int)buf[0] & 0xf8) == 0) ==> (\result == 0 || \result < 0);
	ensures ((len == 2) ==> (int)buf[1] != 0) ==> (\result == 0 || \result < 0);
	ensures ((len == 3) ==> ((int)buf[0] == 7 && (int)buf[2] == 0x80)) ==> (\result == 0 || \result < 0);
	ensures \result == 0 || \result < 0;
	ensures (buf == \null || len == 0 || val == \null) ==> \result == -(3 * 100000 + 2857);
	ensures (buf != \null && len > 0 && val != \null && ((int)buf[0] & 0xf8) != 0)==> \result == -(3 * 100000 + 2864);
	ensures (buf != \null && len == 1 && val != \null && ((int)buf[0] & 0xf8) == 0 && (int)buf[0] != 0)==> \result == -(3 * 100000 + 2882);
	ensures (buf != \null && len == 1 && val != \null && ((int)buf[0] & 0xf8) == 0 && (int)buf[0] == 0)==> \result == -(3 * 100000 + 2894);
	ensures (buf != \null && len == 2 && val != \null && ((int)buf[0] & 0xf8) == 0 && (int)buf[1] == 0)==> \result == -(3 * 100000 + 2910);
	ensures (buf != \null && len == 3 && val != \null && ((int)buf[0] & 0xf8) == 0&& ((int)buf[0] != 7 || (int)buf[2] != 0x80))==> \result == -(3 * 100000 + 2977);
	ensures (buf != \null && len > 3 && val != \null && ((int)buf[0] & 0xf8) == 0)==> \result == -(3 * 100000 + 3003);
	ensures (\result == 0) ==> (len == 2 || len == 3);
	ensures (\result == 0) ==> \valid(val);
	ensures (\result == 0 && len == 2) ==> 0 <= *val <= 0xFF;
	ensures (\result == 0 && len == 3) ==> 0 <= *val <= 0x1FF;
*/
int parse_nine_bit_named_bit_list(u8 const *buf, u32 len, u16 *val)
{
	u8 k;
	u8 non_signif;
	u16 tmp;
	int ret;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (val == (u16 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 2857);
			goto out;
		}
	}
/*@ assert rte_205: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) & 0xf8) {
		ret = - (3 * 100000 + 2864);
		goto out;
	}
	switch (len) {
		int tmp_2;
		case (u32)1: ;
	/*@ assert rte_206: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) != 0) {
			ret = - (3 * 100000 + 2882);
			goto out;
		}
		else {
			ret = - (3 * 100000 + 2894);
			goto out;
		}
		break;
		case (u32)2: ;
	/*@ assert rte_207: mem_access: \valid_read(buf + 1); */
		if ((int)*(buf + 1) == 0) {
			ret = - (3 * 100000 + 2910);
			goto out;
		}
		non_signif = (u8)0;
		k = (u8)0;
		/*@
			loop invariant 0 <= k <= 8;
			loop invariant \valid_read(buf + 1);
			loop assigns k, non_signif;
			loop variant 8 - k;
		*/
		while ((int)k < 8) {
		/*@ assert rte_208: mem_access: \valid_read(buf + 1); */
		/*@ assert rte_209: shift: 0 <= (int)k < 32; */
			if (((int)*(buf + 1) >> (int)k) & 0x1) {
				non_signif = k;
				break;
			}
		/*@ assert rte_210: signed_overflow: (int)k + 1 <= 2147483647; */
			k = (u8)((int)k + 1);
		}
	/*@ assert rte_211: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) != (int)non_signif) {
			ret = - (3 * 100000 + 2946);
			goto out;
		}
		tmp = (u16)0;
		k = (u8)0;
		/*@
			loop invariant 0 <= k <= 8;
			loop invariant 0 <= tmp <= 0xFF;
			loop assigns k, tmp;
			loop variant 8 - k;
		*/
		while ((int)k < 8) {
			{
				int tmp_0;
				u8 const mask[8] =
			{(u8)1, (u8)2, (u8)4, (u8)8, (u8)16, (u8)32, (u8)64, (u8)128};
			/*@ assert rte_212: mem_access: \valid_read(buf + 1); */
			/*@ assert rte_213: index_bound: k < 8; */
				if ((int)*(buf + 1) & (int)mask[k])
			/*@ assert rte_214: signed_overflow: 7 - (int)k <= 2147483647; */
			/*@ assert rte_215: index_bound: 0 <= (int)(7 - (int)k); */
			/*@ assert rte_216: index_bound: (int)(7 - (int)k) < 8; */
				tmp_0 = (int)mask[7 - (int)k];
				else tmp_0 = 0;
				tmp = (u16)((int)tmp | tmp_0);
			}
		/*@ assert rte_217: signed_overflow: (int)k + 1 <= 2147483647; */
			k = (u8)((int)k + 1);
		}
	/*@ assert rte_218: mem_access: \valid(val); */
		*val = tmp;
		break;
		case (u32)3: ;
	/*@ assert rte_219: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) != 7) goto _LOR_0;
		else
	/*@ assert rte_220: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) != 0x80) {
			_LOR_0: {
				ret = - (3 * 100000 + 2977);
				goto out;
			}
		}
		tmp = (u16)0;
		k = (u8)0;
		/*@
			loop invariant 0 <= k <= 8;
			loop invariant 0 <= tmp <= 0xFF;
			loop assigns k, tmp;
			loop variant 8 - k;
		*/
		while ((int)k < 8) {
			{
				int tmp_1;
				u8 const mask_0[8] =
			{(u8)1, (u8)2, (u8)4, (u8)8, (u8)16, (u8)32, (u8)64, (u8)128};
			/*@ assert rte_221: mem_access: \valid_read(buf + 1); */
			/*@ assert rte_222: index_bound: k < 8; */
				if ((int)*(buf + 1) & (int)mask_0[k])
			/*@ assert rte_223: signed_overflow: 7 - (int)k <= 2147483647; */
			/*@ assert rte_224: index_bound: 0 <= (int)(7 - (int)k); */
			/*@ assert rte_225: index_bound: (int)(7 - (int)k) < 8; */
				tmp_1 = (int)mask_0[7 - (int)k];
				else tmp_1 = 0;
				tmp = (u16)((int)tmp | tmp_1);
			}
		/*@ assert rte_226: signed_overflow: (int)k + 1 <= 2147483647; */
			k = (u8)((int)k + 1);
		}
	/*@ assert rte_227: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) & 0x80) tmp_2 = 0x0100; else tmp_2 = 0x0000;
		tmp = (u16)((int)tmp | tmp_2);
	/*@ assert rte_228: mem_access: \valid(val); */
		*val = tmp;
		break;
		default: ret = - (3 * 100000 + 3003);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

int parse_crldp_reasons(u8 const *buf, u32 len, u32 exp_type, u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u16 val = (u16)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 3051);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_CONTEXT_SPECIFIC,exp_type,& hdr_len,
	& data_len);
	if (ret) goto out;
	buf += hdr_len;
	len -= hdr_len;
	ret = parse_nine_bit_named_bit_list(buf,data_len,& val);
	if (ret) goto out;
/*@ assert rte_229: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_DistributionPoint(u8 const *buf, u32 len,
int *crldp_has_all_reasons, u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 remain = (u32)0;
	u32 total_len = (u32)0;
	int dp_or_issuer_present = 0;
	u32 parsed = (u32)0;
	int has_all_reasons = 0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 3126);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) {
		ret = - (3 * 100000 + 3136);
		goto out;
	}
	total_len = hdr_len + data_len;
	remain = data_len;
	buf += hdr_len;
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& hdr_len,
	& data_len);
	if (! ret) {
		u8 dpn_type;
		u32 dpn_remain = (u32)0;
		u32 dpn_eaten = (u32)0;
		buf += hdr_len;
		remain -= hdr_len;
		dpn_remain = data_len;
		if (data_len == (u32)0) {
			ret = - (3 * 100000 + 3160);
			goto out;
		}
	/*@ assert rte_231: mem_access: \valid_read(buf + 0); */
		dpn_type = *(buf + 0);
		switch ((int)dpn_type) {
			case 0xa0:
			ret = parse_GeneralNames(buf,dpn_remain,CLASS_CONTEXT_SPECIFIC,(u32)0,
			& dpn_eaten);
			if (ret) goto out;
			dpn_remain -= dpn_eaten;
			buf += dpn_eaten;
			break;
			case 0xa1: ret = - (3 * 100000 + 3193);
			goto out;
			break;
			default: ret = - (3 * 100000 + 3198);
			goto out;
			break;
		}
		if (dpn_remain) {
			ret = - (3 * 100000 + 3204);
			goto out;
		}
		dp_or_issuer_present |= 1;
		remain -= data_len;
	}
	ret = parse_crldp_reasons(buf,remain,(u32)0x01,& parsed);
	if (! ret) {
		buf += parsed;
		remain -= parsed;
	}
	else has_all_reasons = 1;
	ret = parse_GeneralNames(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)2,& parsed);
	if (! ret) {
		dp_or_issuer_present |= 1;
		buf += parsed;
		remain -= parsed;
	}
	if (remain) {
		ret = - (3 * 100000 + 3240);
		goto out;
	}
	if (! dp_or_issuer_present) {
		ret = - (3 * 100000 + 3251);
		goto out;
	}
/*@ assert rte_232: mem_access: \valid(eaten); */
	*eaten = total_len;
/*@ assert rte_233: mem_access: \valid(crldp_has_all_reasons); */
	*crldp_has_all_reasons = has_all_reasons;
	ret = 0;
	out: ;
	return ret;
}

static int parse_AccessDescription(u8 const *buf, u32 len, u32 *eaten)
{
	u32 remain;
	int ret;
	int found;
	int unused;
	u8 const id_ad_caIssuers_oid[10] =
	{(u8)0x06,
		(u8)0x08,
		(u8)0x2b,
		(u8)0x06,
		(u8)0x01,
		(u8)0x05,
		(u8)0x05,
		(u8)0x07,
		(u8)0x30,
	(u8)0x01};
	u8 const id_ad_ocsp_oid[10] =
	{(u8)0x06,
		(u8)0x08,
		(u8)0x2b,
		(u8)0x06,
		(u8)0x01,
		(u8)0x05,
		(u8)0x05,
		(u8)0x07,
		(u8)0x30,
	(u8)0x02};
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 al_len = (u32)0;
	u32 saved_ad_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 3290);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	saved_ad_len = hdr_len + data_len;
	remain -= hdr_len;
	buf += hdr_len;
	ret = parse_OID(buf,data_len,& oid_len);
	if (ret) goto out;
	found = 0;
	if ((unsigned long)oid_len == sizeof(id_ad_caIssuers_oid)) {
		int tmp_0;
		int tmp;
		tmp = bufs_differ(buf,id_ad_caIssuers_oid,oid_len);
		if (tmp) tmp_0 = 0; else tmp_0 = 1;
		found = tmp_0;
	}
	if (! found)
	if ((unsigned long)oid_len == sizeof(id_ad_ocsp_oid)) {
		int tmp_2;
		int tmp_1;
		tmp_1 = bufs_differ(buf,id_ad_ocsp_oid,oid_len);
		if (tmp_1) tmp_2 = 0; else tmp_2 = 1;
		found = tmp_2;
	}
	if (! found) {
		ret = - (3 * 100000 + 3333);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	data_len -= oid_len;
	ret = parse_GeneralName(buf,data_len,& al_len,& unused);
	if (ret) goto out;
	buf += al_len;
	remain -= al_len;
	data_len -= al_len;
	if (data_len) {
		ret = - (3 * 100000 + 3362);
		goto out;
	}
/*@ assert rte_234: mem_access: \valid(eaten); */
	*eaten = saved_ad_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_AIA(u8 const *cert, u32 off, u32 len, int critical)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 3423);
			goto out;
		}
	}
	if (critical) {
		ret = - (3 * 100000 + 3435);
		goto out;
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain -= hdr_len;
	if (remain != data_len) {
		ret = - (3 * 100000 + 3455);
		goto out;
	}
	if (! remain) {
		ret = - (3 * 100000 + 3465);
		goto out;
	}
	while (remain) {
		u32 parsed = (u32)0;
		ret = parse_AccessDescription(buf,remain,& parsed);
		if (ret) {
			ret = - (3 * 100000 + 3485);
			goto out;
		}
		remain -= parsed;
		buf += parsed;
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires buf == \null || \valid_read(buf + (0 .. len-1)) || len == 0;
	requires num_known_sig_algs == 59;
	requires \valid_read(known_sig_algs + (0 .. 58));
	requires \forall integer i; 0 <= i < 59 ==>known_sig_algs[i] != \null &&\valid_read(known_sig_algs[i]) &&\valid_read(&known_sig_algs[i]->alg_der_oid_len) &&\valid_read(&known_sig_algs[i]->alg_der_oid) &&(known_sig_algs[i]->alg_der_oid_len == 0 ||\valid_read(known_sig_algs[i]->alg_der_oid + (0 .. known_sig_algs[i]->alg_der_oid_len - 1)));
	assigns \nothing;
	ensures buf == \null ==> \result == \null;
	ensures len == 0 ==> \result == \null;
	ensures \result == \null ||(\exists integer k; 0 <= k < 59 && \result == known_sig_algs[k]);
	ensures \result != \null ==>\result->alg_der_oid_len == len &&(\forall integer i; 0 <= i < len ==> \result->alg_der_oid[i] == buf[i]);
	ensures (buf != \null && len != 0 && \result == \null) ==>(\forall integer k; 0 <= k < 59 ==>(known_sig_algs[k]->alg_der_oid_len != len ||(\exists integer i; 0 <= i < len && known_sig_algs[k]->alg_der_oid[i] != buf[i])));
*/
_sig_alg const *find_sig_alg_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_sig_alg const *found = (_sig_alg const *)0;
	_sig_alg const *cur = (_sig_alg const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= num_known_sig_algs;
		loop invariant num_known_sig_algs == 59;
		loop invariant found == (_sig_alg const *)0 ||(\exists integer j; 0 <= j < k && found == known_sig_algs[j]);
		loop invariant found == (_sig_alg const *)0 ==>(\forall integer j; 0 <= j < k ==>known_sig_algs[j]->alg_der_oid_len != len ||(\exists integer i; 0 <= i < len &&known_sig_algs[j]->alg_der_oid[i] != buf[i]));
		loop assigns k, cur, found;
		loop variant (int)num_known_sig_algs - (int)k;
	*/
	while ((int)k < (int)num_known_sig_algs) {
		{
			int ret;
			int tmp_0;
			int tmp;
		/*@ assert rte_6: index_bound: k < 59; */
			cur = known_sig_algs[k];
		/*@ assert rte_7: mem_access: \valid_read(&cur->alg_der_oid_len); */
			if (cur->alg_der_oid_len != len) goto __Cont;
		/*@ assert rte_8: mem_access: \valid_read(&cur->alg_der_oid); */
		/*@ assert rte_9: mem_access: \valid_read(&cur->alg_der_oid_len); */
			tmp = bufs_differ(cur->alg_der_oid,buf,cur->alg_der_oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_10: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

/*@
	requires buf == \null || \valid_read(buf + (0 .. len-1));
	requires \forall integer i; 0 <= i < 34 ==> known_hashes[i] != \null;
	requires \forall integer i; 0 <= i < 34 ==>\valid_read(known_hashes[i]) &&\valid_read(&known_hashes[i]->alg_der_oid_len) &&\valid_read(&known_hashes[i]->alg_der_oid) &&(known_hashes[i]->alg_der_oid_len == 0 ||\valid_read(known_hashes[i]->alg_der_oid + (0 .. known_hashes[i]->alg_der_oid_len - 1)));
	assigns \nothing;
	ensures buf == \null ==> \result == \null;
	ensures len == 0 ==> \result == \null;
	ensures \result == \null ||(\exists integer i; 0 <= i < 34 && \result == known_hashes[i]);
	ensures \result != \null ==>(\exists integer i; 0 <= i < 34 &&\result == known_hashes[i] &&\result->alg_der_oid_len == len &&(\forall integer k; 0 <= k < len ==> \result->alg_der_oid[k] == buf[k]));
	ensures (buf != \null && len != 0 && \result == \null) ==>(\forall integer i; 0 <= i < 34 ==>(known_hashes[i]->alg_der_oid_len != len ||(\exists integer k; 0 <= k < len && known_hashes[i]->alg_der_oid[k] != buf[k])));
*/
_hash_alg const *find_hash_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_hash_alg const *found = (_hash_alg const *)0;
	_hash_alg const *cur = (_hash_alg const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= 34;
		loop invariant found == \null ||(\exists integer j; 0 <= j < k && found == known_hashes[j]);
		loop invariant found == \null ==>(\forall integer j; 0 <= j < k ==>known_hashes[j]->alg_der_oid_len != len ||bufs_differ(known_hashes[j]->alg_der_oid, buf, known_hashes[j]->alg_der_oid_len) != 0);
		loop assigns k, cur, found;
		loop variant 34 - k;
	*/
	while ((unsigned long)k < sizeof(known_hashes) / sizeof(known_hashes[0])) {
		{
			int ret;
			int tmp_0;
			int tmp;
		/*@ assert rte_11: index_bound: k < 34; */
			cur = known_hashes[k];
		/*@ assert rte_12: mem_access: \valid_read(&cur->alg_der_oid_len); */
			if (cur->alg_der_oid_len != len) goto __Cont;
		/*@ assert rte_13: mem_access: \valid_read(&cur->alg_der_oid); */
		/*@ assert rte_14: mem_access: \valid_read(&cur->alg_der_oid_len); */
			tmp = bufs_differ(cur->alg_der_oid,buf,cur->alg_der_oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_15: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

/*@
	requires (buf == \null) || (len == 0) || \valid_read(buf + (0 .. len-1));
	requires \valid_read(known_curves + (0 .. 43));
	requires \forall integer i; 0 <= i < 44 ==>(known_curves[i] != \null &&\valid_read(known_curves[i]) &&\valid_read(&known_curves[i]->crv_der_oid_len) &&\valid_read(&known_curves[i]->crv_der_oid) &&(known_curves[i]->crv_der_oid_len == 0 ||\valid_read(known_curves[i]->crv_der_oid + (0 .. known_curves[i]->crv_der_oid_len - 1))));
	assigns \nothing;
	ensures \result == \null ||(\exists integer i; 0 <= i < 44 && \result == known_curves[i]);
	ensures (buf == \null || len == 0) ==> \result == \null;
	ensures \result != \null ==>(buf != \null && len > 0 &&\result->crv_der_oid_len == len &&(\forall integer j; 0 <= j < len ==> \result->crv_der_oid[j] == buf[j]));
	ensures (buf != \null && len > 0 && \result == \null) ==>(\forall integer i; 0 <= i < 44 ==>(known_curves[i]->crv_der_oid_len != len ||(\exists integer j; 0 <= j < len &&known_curves[i]->crv_der_oid[j] != buf[j])));
*/
_curve const *find_curve_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_curve const *found = (_curve const *)0;
	_curve const *cur = (_curve const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= 44;
		loop invariant found == \null ||(\exists integer i; 0 <= i < k && found == known_curves[i]);
		loop invariant found == \null ==>(\forall integer i; 0 <= i < k ==>known_curves[i] != \null ==>(known_curves[i]->crv_der_oid_len != len ||bufs_differ(known_curves[i]->crv_der_oid, buf, known_curves[i]->crv_der_oid_len) != 0));
		loop assigns k, cur, found;
		loop variant 44 - k;
	*/
	while ((unsigned long)k < sizeof(known_curves) / sizeof(known_curves[0])) {
		{
			int ret;
			int tmp_0;
			int tmp;
		/*@ assert rte_16: index_bound: k < 44; */
			cur = known_curves[k];
		/*@ assert rte_17: mem_access: \valid_read(&cur->crv_der_oid_len); */
			if (cur->crv_der_oid_len != len) goto __Cont;
		/*@ assert rte_18: mem_access: \valid_read(&cur->crv_der_oid); */
		/*@ assert rte_19: mem_access: \valid_read(&cur->crv_der_oid_len); */
			tmp = bufs_differ(cur->crv_der_oid,buf,cur->crv_der_oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_20: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

static u8 const _dn_oid_cn[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x03};

static u8 const _dn_oid_surname[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x04};

static u8 const _dn_oid_serial[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x05};

static u8 const _dn_oid_country[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x06};

static u8 const _dn_oid_locality[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x07};

static u8 const _dn_oid_state[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x08};

static u8 const _dn_oid_org[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x0a};

static u8 const _dn_oid_org_unit[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x0b};

static u8 const _dn_oid_title[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x0c};

static u8 const _dn_oid_name[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x29};

static u8 const _dn_oid_emailaddress[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x09,
(u8)0x01};

static u8 const _dn_oid_given_name[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2a};

static u8 const _dn_oid_initials[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2b};

static u8 const _dn_oid_gen_qual[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2c};

static u8 const _dn_oid_dn_qual[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x2e};

static u8 const _dn_oid_pseudo[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x41};

static u8 const _dn_oid_dc[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x09,
	(u8)0x92,
	(u8)0x26,
	(u8)0x89,
	(u8)0x93,
	(u8)0xf2,
	(u8)0x2c,
	(u8)0x64,
	(u8)0x01,
(u8)0x19};

static u8 const _dn_oid_ogrn[7] =
{(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x01};

static u8 const _dn_oid_snils[7] =
{(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x03};

static u8 const _dn_oid_ogrnip[7] =
{(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x05};

static u8 const _dn_oid_inn[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x03,
	(u8)0x81,
	(u8)0x03,
	(u8)0x01,
(u8)0x01};

static u8 const _dn_oid_street_address[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x04, (u8)0x09};

static int parse_directory_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
	u8 str_type;
	int ret = - (3 * 100000 + 3735);
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 3739);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 3745);
		goto out;
	}
/*@ assert rte_147: mem_access: \valid_read(buf + 0); */
	str_type = *(buf + 0);
	len -= (u32)2;
/*@ assert rte_148: mem_access: \valid_read(buf + 1); */
	if ((u32)*(buf + 1) != len) {
		ret = - (3 * 100000 + 3754);
		goto out;
	}
	buf += 2;
	if (len < lb) goto _LOR_0;
	else
	if (len > ub) {
		_LOR_0: {
			ret = - (3 * 100000 + 3761);
			goto out;
		}
	}
	switch ((int)str_type) {
		case 19: ret = check_printable_string(buf,len);
		int tmp = ret;
		break;
		case 12: ret = check_utf8_string(buf,len);
		int tmp_3 = ret;
		break;
		case 20: ret = check_teletex_string(buf,len);
		int tmp_5 = ret;
		break;
		case 28: ret = check_universal_string(buf,len);
		int tmp_7 = ret;
		break;
		case 30: ret = check_bmp_string(buf,len);
		int tmp_9 = ret;
		break;
		case 22: ret = check_ia5_string(buf,len);
		int tmp_11 = ret;
		break;
		case 18: ret = check_numeric_string(buf,len);
		int tmp_13 = ret;
		break;
		default: ret = - (3 * 100000 + 3817);
		break;
	}
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

static int parse_printable_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
	u8 str_type;
	int ret = - (3 * 100000 + 3849);
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 3853);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 3859);
		goto out;
	}
/*@ assert rte_149: mem_access: \valid_read(buf + 0); */
	str_type = *(buf + 0);
	if ((int)str_type != 19) {
		ret = - (3 * 100000 + 3866);
		goto out;
	}
	len -= (u32)2;
/*@ assert rte_150: mem_access: \valid_read(buf + 1); */
	if ((u32)*(buf + 1) != len) {
		ret = - (3 * 100000 + 3873);
		goto out;
	}
	buf += 2;
	if (len < lb) goto _LOR_0;
	else
	if (len > ub) {
		_LOR_0: {
			ret = - (3 * 100000 + 3880);
			goto out;
		}
	}
	ret = check_printable_string(buf,len);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

static int parse_numeric_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
	u8 str_type;
	int ret = - (3 * 100000 + 3913);
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 3917);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 3923);
		goto out;
	}
/*@ assert rte_158: mem_access: \valid_read(buf + 0); */
	str_type = *(buf + 0);
	if ((int)str_type != 18) {
		ret = - (3 * 100000 + 3930);
		goto out;
	}
	len -= (u32)2;
/*@ assert rte_159: mem_access: \valid_read(buf + 1); */
	if ((u32)*(buf + 1) != len) {
		ret = - (3 * 100000 + 3937);
		goto out;
	}
	buf += 2;
	if (len < lb) goto _LOR_0;
	else
	if (len > ub) {
		_LOR_0: {
			ret = - (3 * 100000 + 3944);
			goto out;
		}
	}
	ret = check_numeric_string(buf,len);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires (buf == \null) || \valid_read(buf + (0..len-1));
	requires lb <= ub;
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 3981);
	ensures (buf != \null && len != 0 && len < 2) ==> \result == -(3 * 100000 + 3987);
	ensures (buf != \null && len >= 2 && (int)buf[0] != 22) ==> \result == -(3 * 100000 + 3994);
	ensures (buf != \null && len >= 2 && (int)buf[0] == 22 && (u32)buf[1] != len - 2)==> \result == -(3 * 100000 + 4001);
	ensures (buf != \null && len >= 2 && (int)buf[0] == 22 && (u32)buf[1] == len - 2&& (len - 2 < lb || len - 2 > ub))==> \result == -(3 * 100000 + 4008);
	ensures (buf != \null && len >= 2 && (int)buf[0] == 22 && (u32)buf[1] == len - 2&& len - 2 >= lb && len - 2 <= ub)==> (\result == 0 || \result == -(3 * 100000 + 2263));
	ensures \result == 0 || \result == -(3 * 100000 + 3981) || \result == -(3 * 100000 + 3987)|| \result == -(3 * 100000 + 3994) || \result == -(3 * 100000 + 4001)|| \result == -(3 * 100000 + 4008) || \result == -(3 * 100000 + 2263);
*/
int parse_ia5_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
	u8 str_type;
	int ret = - (3 * 100000 + 3977);
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 3981);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 3987);
		goto out;
	}
/*@ assert rte_151: mem_access: \valid_read(buf + 0); */
	str_type = *(buf + 0);
	if ((int)str_type != 22) {
		ret = - (3 * 100000 + 3994);
		goto out;
	}
	len -= (u32)2;
/*@ assert rte_152: mem_access: \valid_read(buf + 1); */
	if ((u32)*(buf + 1) != len) {
		ret = - (3 * 100000 + 4001);
		goto out;
	}
	buf += 2;
	if (len < lb) goto _LOR_0;
	else
	if (len > ub) {
		_LOR_0: {
			ret = - (3 * 100000 + 4008);
			goto out;
		}
	}
	ret = check_ia5_string(buf,len);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires valid_buf: buf == \null || len == 0 || \valid_read(buf + (0 .. len - 1));
	requires bound_order: lb <= ub;
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 4042);
	ensures (buf != \null && len != 0 && len < 2) ==>\result == -(3 * 100000 + 4048);
	ensures (buf != \null && len >= 2 && *buf != 22) ==>\result == -(3 * 100000 + 4055);
	ensures (buf != \null && len >= 2 && *buf == 22 &&(u32)*(buf + 1) != len - 2) ==>\result == -(3 * 100000 + 4062);
	ensures (buf != \null && len >= 2 && *buf == 22 &&(u32)*(buf + 1) == len - 2 &&(len - 2 < lb || len - 2 > ub)) ==>\result == -(3 * 100000 + 4069);
	ensures \result <= 0;
	ensures \result == 0 ||\result == -(3 * 100000 + 4042) ||\result == -(3 * 100000 + 4048) ||\result == -(3 * 100000 + 4055) ||\result == -(3 * 100000 + 4062) ||\result == -(3 * 100000 + 4069) ||\result == -(3 * 100000 + 1833) ||\result == -(3 * 100000 + 1864) ||\result == -(3 * 100000 + 1871) ||\result == -(3 * 100000 + 1888) ||\result == -(3 * 100000 + 1896) ||\result == -(3 * 100000 + 1907) ||\result == -(3 * 100000 + 1912) ||\result == -(3 * 100000 + 1929) ||\result == -(3 * 100000 + 1941) ||\result == -(3 * 100000 + 1946) ||\result == -(3 * 100000 + 1954) ||\result == -(3 * 100000 + 1964);
*/
static int parse_utf8_string(u8 const *buf, u32 len, u32 lb, u32 ub)
{
	u8 str_type;
	int ret = - (3 * 100000 + 4038);
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 4042);
			goto out;
		}
	}
	if (len < (u32)2) {
		ret = - (3 * 100000 + 4048);
		goto out;
	}
/*@ assert rte_153: mem_access: \valid_read(buf + 0); */
	str_type = *(buf + 0);
	if ((int)str_type != 22) {
		ret = - (3 * 100000 + 4055);
		goto out;
	}
	len -= (u32)2;
/*@ assert rte_154: mem_access: \valid_read(buf + 1); */
	if ((u32)*(buf + 1) != len) {
		ret = - (3 * 100000 + 4062);
		goto out;
	}
	buf += 2;
	if (len < lb) goto _LOR_0;
	else
	if (len > ub) {
		_LOR_0: {
			ret = - (3 * 100000 + 4069);
			goto out;
		}
	}
	ret = check_utf8_string(buf,len);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

static int parse_rdn_val_cn(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)192);
	return tmp;
}

static int parse_rdn_val_x520name(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)32768);
	return tmp;
}

/*@
	requires buf == \null || \valid_read(buf + (0 .. len - 1));
	requires 1 <= 255;
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> (\result == -(3 * 100000 + 4042));
	ensures (buf != \null && len != 0 && \valid_read(buf + (0 .. len - 1))) ==> (\result <= 0);
	ensures \result <= 0;
	ensures \result == 0 ||\result == -(3 * 100000 + 3981) ||\result == -(3 * 100000 + 3987) ||\result == -(3 * 100000 + 3994) ||\result == -(3 * 100000 + 4001) ||\result == -(3 * 100000 + 4008) ||\result == -(3 * 100000 + 2263) ||\result == -(3 * 100000 + 4042) ||\result == -(3 * 100000 + 4048) ||\result == -(3 * 100000 + 4055) ||\result == -(3 * 100000 + 4062) ||\result == -(3 * 100000 + 4069) ||\result == -(3 * 100000 + 1833) ||\result == -(3 * 100000 + 1864) ||\result == -(3 * 100000 + 1871) ||\result == -(3 * 100000 + 1888) ||\result == -(3 * 100000 + 1896) ||\result == -(3 * 100000 + 1907) ||\result == -(3 * 100000 + 1912) ||\result == -(3 * 100000 + 1929) ||\result == -(3 * 100000 + 1941) ||\result == -(3 * 100000 + 1946) ||\result == -(3 * 100000 + 1954) ||\result == -(3 * 100000 + 1964);
*/
static int parse_rdn_val_emailaddress(u8 const *buf, u32 len)
{
	int ret;
	ret = parse_ia5_string(buf,len,(u32)1,(u32)255);
	if (ret) ret = parse_utf8_string(buf,len,(u32)1,(u32)255);
	return ret;
}

static int parse_rdn_val_serial(u8 const *buf, u32 len)
{
	int ret;
	ret = parse_printable_string(buf,len,(u32)1,(u32)64);
	if (ret) ret = parse_ia5_string(buf,len,(u32)1,(u32)64);
	return ret;
}

static int parse_rdn_val_country(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)2,(u32)2);
	return tmp;
}

static int parse_rdn_val_locality(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
	return tmp;
}

static int parse_rdn_val_state(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
	return tmp;
}

static int parse_rdn_val_org(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)64);
	return tmp;
}

static int parse_rdn_val_org_unit(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
	return tmp;
}

static int parse_rdn_val_title(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)64);
	return tmp;
}

static int parse_rdn_val_dn_qual(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_printable_string(buf,len,(u32)1,(u32)0xffffffffUL);
	return tmp;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result == 0 || \result == 1;
	ensures ('0' <= c && c <= '9') ==> \result == 1;
	ensures (c < '0' || c > '9') ==> \result == 0;
*/
inline static int _is_digit(u8 c)
{
	int tmp;
	if ((int)c >= '0')
	if ((int)c <= '9') tmp = 1; else tmp = 0;
	else tmp = 0;
	return tmp;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result == 0 || \result == 1;
	ensures ('a' <= c <= 'z') ==> \result == 1;
	ensures ('A' <= c <= 'Z') ==> \result == 1;
	ensures !('a' <= c <= 'z') && !('A' <= c <= 'Z') ==> \result == 0;
*/
inline static int _is_alpha(u8 c)
{
	int tmp;
	if ((int)c >= 'a') {
		if ((int)c <= 'z') tmp = 1; else goto _LAND;
	}
	else {
		_LAND: ;
		if ((int)c >= 'A')
		if ((int)c <= 'Z') tmp = 1; else tmp = 0;
		else tmp = 0;
	}
	return tmp;
}

/*@
	requires buf == \null || len == 0 || \valid_read(buf + (0..len-1));
	requires len <= 0xFFFFFFFF;
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == -(3 * 100000 + 4376);
	ensures (buf != \null && len != 0 && len < 2) ==>\result == -(3 * 100000 + 3987);
	ensures (buf != \null && len >= 2 && (int)buf[0] != 22) ==>\result == -(3 * 100000 + 3994);
	ensures (buf != \null && len >= 2 && (int)buf[0] == 22 && (u32)buf[1] != len - 2) ==>\result == -(3 * 100000 + 4001);
	ensures (buf != \null && len >= 2 && (int)buf[0] == 22 && (u32)buf[1] == len - 2 &&(len - 2 < 1 || len - 2 > 63)) ==>\result == -(3 * 100000 + 4008);
	ensures (buf != \null && len >= 2 && (int)buf[0] == 22 && (u32)buf[1] == len - 2 &&len - 2 >= 1 && len - 2 <= 63 &&(\exists u32 k; 0 <= k < len - 2 && (int)buf[2 + k] > 0x7f)) ==>\result == -(3 * 100000 + 2263);
	ensures \result == 0 ||\result == -(3 * 100000 + 4376) ||\result == -(3 * 100000 + 3987) ||\result == -(3 * 100000 + 3994) ||\result == -(3 * 100000 + 4001) ||\result == -(3 * 100000 + 4008) ||\result == -(3 * 100000 + 2263) ||\result == -(3 * 100000 + 4395) ||\result == -(3 * 100000 + 4412) ||\result == -(3 * 100000 + 4430);
	ensures \result == 0 ==> buf != \null && len >= 3;
*/
static int parse_rdn_val_dc(u8 const *buf, u32 len)
{
	int ret;
	u32 i;
	u8 c;
	int tmp_1;
	int tmp;
	int tmp_4;
	int tmp_2;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 4376);
			goto out;
		}
	}
	ret = parse_ia5_string(buf,len,(u32)1,(u32)63);
	if (ret) goto out;
	buf += 2;
	len -= (u32)2;
/*@ assert rte_155: mem_access: \valid_read(buf + 0); */
	c = *(buf + 0);
	tmp = _is_alpha(c);
	if (tmp) tmp_1 = 1;
	else {
		int tmp_0;
		tmp_0 = _is_digit(c);
		if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
	}
	ret = tmp_1;
	if (! ret) {
		ret = - (3 * 100000 + 4395);
		goto out;
	}
	buf ++;
	len -= (u32)1;
	if (! len) {
		ret = 0;
		goto out;
	}
/*@ assert rte_156: mem_access: \valid_read(buf + (u32)(len - 1)); */
	c = *(buf + (len - (u32)1));
	tmp_2 = _is_alpha(c);
	if (tmp_2) tmp_4 = 1;
	else {
		int tmp_3;
		tmp_3 = _is_digit(c);
		if (tmp_3) tmp_4 = 1; else tmp_4 = 0;
	}
	ret = tmp_4;
	if (! ret) {
		ret = - (3 * 100000 + 4412);
		goto out;
	}
	buf ++;
	len -= (u32)1;
	i = (u32)0;
	/*@
		loop invariant 0 <= i <= len;
		loop invariant \valid_read(buf + (0 .. len - 1));
		loop invariant \forall integer k; 0 <= k < i ==>(_is_digit(buf[k]) == 1 || _is_alpha(buf[k]) == 1 || (int)buf[k] == '-');
		loop assigns i, c, tmp_5, tmp_6, tmp_7, ret;
		loop variant len - i;
	*/
	while (i < len) {
		{
			int tmp_7;
			int tmp_5;
		/*@ assert rte_157: mem_access: \valid_read(buf + i); */
			c = *(buf + i);
			tmp_5 = _is_digit(c);
			if (tmp_5) tmp_7 = 1;
			else {
				int tmp_6;
				tmp_6 = _is_alpha(c);
				if (tmp_6) tmp_7 = 1;
				else
				if ((int)c == '-') tmp_7 = 1; else tmp_7 = 0;
			}
			ret = tmp_7;
			if (! ret) {
				ret = - (3 * 100000 + 4430);
				goto out;
			}
		}
		i ++;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_rdn_val_pseudo(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)128);
	return tmp;
}

static int parse_rdn_val_ogrn(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_numeric_string(buf,len,(u32)1,(u32)13);
	return tmp;
}

static int parse_rdn_val_snils(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_numeric_string(buf,len,(u32)1,(u32)11);
	return tmp;
}

static int parse_rdn_val_ogrnip(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_numeric_string(buf,len,(u32)1,(u32)15);
	return tmp;
}

static int parse_rdn_val_inn(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_numeric_string(buf,len,(u32)1,(u32)12);
	return tmp;
}

static int parse_rdn_val_street_address(u8 const *buf, u32 len)
{
	int tmp;
	tmp = parse_directory_string(buf,len,(u32)1,(u32)64);
	return tmp;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result == 0;
*/
static int parse_rdn_val_bad_oid(u8 const *buf, u32 len)
{
	int __retres;
	__retres = 0;
	return __retres;
}

static _name_oid const generic_unsupported_rdn_oid =
{.oid = (u8 const *)0,
	.oid_len = (u8)0,
.parse_rdn_val = & parse_rdn_val_bad_oid};

static _name_oid const known_dn_oids[22] =
{{.oid = _dn_oid_cn,
		.oid_len = (u8)sizeof(_dn_oid_cn),
	.parse_rdn_val = & parse_rdn_val_cn},
	{.oid = _dn_oid_surname,
		.oid_len = (u8)sizeof(_dn_oid_surname),
	.parse_rdn_val = & parse_rdn_val_x520name},
	{.oid = _dn_oid_serial,
		.oid_len = (u8)sizeof(_dn_oid_serial),
	.parse_rdn_val = & parse_rdn_val_serial},
	{.oid = _dn_oid_country,
		.oid_len = (u8)sizeof(_dn_oid_country),
	.parse_rdn_val = & parse_rdn_val_country},
	{.oid = _dn_oid_locality,
		.oid_len = (u8)sizeof(_dn_oid_locality),
	.parse_rdn_val = & parse_rdn_val_locality},
	{.oid = _dn_oid_state,
		.oid_len = (u8)sizeof(_dn_oid_state),
	.parse_rdn_val = & parse_rdn_val_state},
	{.oid = _dn_oid_org,
		.oid_len = (u8)sizeof(_dn_oid_org),
	.parse_rdn_val = & parse_rdn_val_org},
	{.oid = _dn_oid_org_unit,
		.oid_len = (u8)sizeof(_dn_oid_org_unit),
	.parse_rdn_val = & parse_rdn_val_org_unit},
	{.oid = _dn_oid_title,
		.oid_len = (u8)sizeof(_dn_oid_title),
	.parse_rdn_val = & parse_rdn_val_title},
	{.oid = _dn_oid_name,
		.oid_len = (u8)sizeof(_dn_oid_name),
	.parse_rdn_val = & parse_rdn_val_x520name},
	{.oid = _dn_oid_emailaddress,
		.oid_len = (u8)sizeof(_dn_oid_emailaddress),
	.parse_rdn_val = & parse_rdn_val_emailaddress},
	{.oid = _dn_oid_given_name,
		.oid_len = (u8)sizeof(_dn_oid_given_name),
	.parse_rdn_val = & parse_rdn_val_x520name},
	{.oid = _dn_oid_initials,
		.oid_len = (u8)sizeof(_dn_oid_initials),
	.parse_rdn_val = & parse_rdn_val_x520name},
	{.oid = _dn_oid_gen_qual,
		.oid_len = (u8)sizeof(_dn_oid_gen_qual),
	.parse_rdn_val = & parse_rdn_val_x520name},
	{.oid = _dn_oid_dn_qual,
		.oid_len = (u8)sizeof(_dn_oid_dn_qual),
	.parse_rdn_val = & parse_rdn_val_dn_qual},
	{.oid = _dn_oid_pseudo,
		.oid_len = (u8)sizeof(_dn_oid_pseudo),
	.parse_rdn_val = & parse_rdn_val_pseudo},
	{.oid = _dn_oid_dc,
		.oid_len = (u8)sizeof(_dn_oid_dc),
	.parse_rdn_val = & parse_rdn_val_dc},
	{.oid = _dn_oid_ogrn,
		.oid_len = (u8)sizeof(_dn_oid_ogrn),
	.parse_rdn_val = & parse_rdn_val_ogrn},
	{.oid = _dn_oid_snils,
		.oid_len = (u8)sizeof(_dn_oid_snils),
	.parse_rdn_val = & parse_rdn_val_snils},
	{.oid = _dn_oid_ogrnip,
		.oid_len = (u8)sizeof(_dn_oid_ogrnip),
	.parse_rdn_val = & parse_rdn_val_ogrnip},
	{.oid = _dn_oid_inn,
		.oid_len = (u8)sizeof(_dn_oid_inn),
	.parse_rdn_val = & parse_rdn_val_inn},
	{.oid = _dn_oid_street_address,
		.oid_len = (u8)sizeof(_dn_oid_street_address),
.parse_rdn_val = & parse_rdn_val_street_address}};

/*@
	requires buf == \null || len == 0 || \valid_read(buf + (0 .. len-1));
	requires \valid_read(&known_dn_oids[0 .. 21]);
	requires \forall integer k; 0 <= k < 22 ==>(known_dn_oids[k].oid != \null ==>\valid_read(known_dn_oids[k].oid + (0 .. known_dn_oids[k].oid_len - 1)));
	assigns \nothing;
	ensures \result == \null ||(\exists integer k; 0 <= k < 22 && \result == &known_dn_oids[k]);
	ensures (buf == \null || len == 0) ==> \result == \null;
	ensures \result != \null ==>((u32)\result->oid_len == len &&\result->oid != \null &&(\forall integer i; 0 <= i < len ==> \result->oid[i] == buf[i]));
	ensures (buf != \null && len != 0 && \result == \null) ==>(\forall integer k; 0 <= k < 22 ==>((u32)known_dn_oids[k].oid_len != len ||(\exists integer i; 0 <= i < (u32)known_dn_oids[k].oid_len &&known_dn_oids[k].oid[i] != buf[i])));
*/
static _name_oid const *find_dn_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_name_oid const *found = (_name_oid const *)0;
	_name_oid const *cur = (_name_oid const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= 22;
		loop invariant found == \null || (\exists integer j; 0 <= j < k && found == &known_dn_oids[j]);
		loop invariant found == \null ==>(\forall integer j; 0 <= j < k ==>((u32)known_dn_oids[j].oid_len != len ||bufs_differ(known_dn_oids[j].oid, buf, (u32)known_dn_oids[j].oid_len) != 0));
		loop assigns k, cur, found;
		loop variant 22 - k;
	*/
	while ((unsigned long)k < sizeof(known_dn_oids) / sizeof(known_dn_oids[0])) {
		{
			int ret;
			int tmp_0;
			int tmp;
			cur = & known_dn_oids[k];
		/*@ assert rte_125: mem_access: \valid_read(&cur->oid_len); */
			if ((u32)cur->oid_len != len) goto __Cont;
		/*@ assert rte_126: mem_access: \valid_read(&cur->oid); */
		/*@ assert rte_127: mem_access: \valid_read(&cur->oid_len); */
			tmp = bufs_differ(cur->oid,buf,(u32)cur->oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_128: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

static int parse_AttributeTypeAndValue(u8 const *buf, u32 len, u32 *eaten)
{
	u32 parsed;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	_name_oid const *cur = (_name_oid const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 4747);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
	buf += hdr_len;
	len -= hdr_len;
	ret = parse_OID(buf,data_len,& oid_len);
	if (ret) goto out;
	cur = find_dn_by_oid(buf,oid_len);
	if (cur == (_name_oid const *)0) cur = & generic_unsupported_rdn_oid;
	data_len -= oid_len;
	buf += oid_len;
	if (cur == known_dn_oids) ret = parse_rdn_val_cn(buf,data_len);
	else
	if (cur == & known_dn_oids[1]) ret = parse_rdn_val_x520name(buf,data_len);
	else
	if (cur == & known_dn_oids[2]) ret = parse_rdn_val_serial(buf,data_len);
	else
	if (cur == & known_dn_oids[3]) ret = parse_rdn_val_country(buf,
	data_len);
	else
	if (cur == & known_dn_oids[4]) ret = parse_rdn_val_locality
	(buf,data_len);
	else
	if (cur == & known_dn_oids[5]) ret = parse_rdn_val_state(buf,
	data_len);
	else
	if (cur == & known_dn_oids[6]) ret = parse_rdn_val_org(buf,
	data_len);
	else
	if (cur == & known_dn_oids[7]) ret = parse_rdn_val_org_unit
	(buf,data_len);
	else
	if (cur == & known_dn_oids[8]) ret = parse_rdn_val_title
	(buf,data_len);
	else
	if (cur == & known_dn_oids[9]) ret = parse_rdn_val_x520name
	(buf,data_len);
	else
	if (cur == & known_dn_oids[10]) ret = parse_rdn_val_emailaddress
	(buf,data_len);
	else
	if (cur == & known_dn_oids[11]) ret = parse_rdn_val_x520name
	(buf,data_len);
	else
	if (cur == & known_dn_oids[12]) ret = parse_rdn_val_x520name
	(buf,data_len);
	else
	if (cur == & known_dn_oids[13]) ret = parse_rdn_val_x520name
	(buf,data_len);
	else
	if (cur == & known_dn_oids[14]) ret = parse_rdn_val_dn_qual
	(buf,data_len);
	else
	if (cur == & known_dn_oids[15]) ret = parse_rdn_val_pseudo
	(buf,
	data_len);
	else
	if (cur == & known_dn_oids[16]) ret = parse_rdn_val_dc
	(buf,
	data_len);
	else
	if (cur == & known_dn_oids[17]) ret = parse_rdn_val_ogrn
	(buf,
	data_len);
	else
	if (cur == & known_dn_oids[18])
	ret = parse_rdn_val_snils(buf,
	data_len);
	else
	if (cur == & known_dn_oids[19])
	ret = parse_rdn_val_ogrnip(buf,
	data_len);
	else
	if (cur == & known_dn_oids[20])
	ret = parse_rdn_val_inn(buf,
	data_len);
	else
	if (cur == & known_dn_oids[21])
	ret = parse_rdn_val_street_address
	(buf,data_len);
	else
	if (cur == & generic_unsupported_rdn_oid)
	ret = parse_rdn_val_bad_oid
	(buf,data_len);
	if (ret) goto out;
/*@ assert rte_160: mem_access: \valid(eaten); */
	*eaten = parsed;
	ret = 0;
	out: ;
	return ret;
}

static int parse_RelativeDistinguishedName(u8 const *buf, u32 len, u32 *eaten)
{
	u32 rdn_remain;
	u32 saved_rdn_len;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 4865);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SET,& hdr_len,
	& data_len);
	if (ret) goto out;
	saved_rdn_len = hdr_len + data_len;
	buf += hdr_len;
	rdn_remain = data_len;
	while (rdn_remain) {
		u32 parsed = (u32)0;
		ret = parse_AttributeTypeAndValue(buf,rdn_remain,& parsed);
		if (ret) goto out;
		rdn_remain -= parsed;
		buf += parsed;
	}
/*@ assert rte_161: mem_access: \valid(eaten); */
	*eaten = saved_rdn_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_x509_Name(u8 const *buf, u32 len, u32 *eaten, int *empty)
{
	int ret;
	u32 name_hdr_len = (u32)0;
	u32 name_data_len = (u32)0;
	u32 remain = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 5000);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& name_hdr_len,& name_data_len);
	if (ret) goto out;
	buf += name_hdr_len;
	remain = name_data_len;
	while (remain) {
		u32 parsed = (u32)0;
		ret = parse_RelativeDistinguishedName(buf,remain,& parsed);
		if (ret) goto out;
		buf += parsed;
		remain -= parsed;
	}
/*@ assert rte_162: mem_access: \valid(eaten); */
	*eaten = name_hdr_len + name_data_len;
/*@ assert rte_163: mem_access: \valid(empty); */
	*empty = ! name_data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_DisplayText(u8 const *buf, u32 len, u32 *eaten)
{
	u8 str_type;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	int ret = -1;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 5071);
			goto out;
		}
	}
/*@ assert rte_235: mem_access: \valid_read(buf + 0); */
	str_type = *(buf + 0);
	switch ((int)str_type) {
		case 12: case 22: case 26: case 30:
		ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)str_type,& hdr_len,
		& data_len);
		if (ret) goto out;
		buf += hdr_len;
		switch ((int)str_type) {
			case 12: ret = check_utf8_string(buf,data_len);
			if (ret) goto out;
			break;
			case 22: ret = check_ia5_string(buf,data_len);
			if (ret) goto out;
			break;
			case 26: ret = check_visible_string(buf,data_len);
			if (ret) goto out;
			break;
			case 30: ret = check_bmp_string(buf,data_len);
			if (ret) goto out;
			break;
			default: ret = - (3 * 100000 + 5122);
			goto out;
			break;
		}
	/*@ assert rte_236: mem_access: \valid(eaten); */
		*eaten = hdr_len + data_len;
		break;
		default: ret = - (3 * 100000 + 5132);
		goto out;
		break;
	}
	out: ;
	return ret;
}

int parse_sig_monkey(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 5168);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (3 * 100000 + 5188);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
/*@ assert rte_610: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (3 * 100000 + 5202);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
/*@ assert rte_611: mem_access: \valid(&params->monkeysphere.sig_raw_off); */
	params->monkeysphere.sig_raw_off = off;
/*@ assert rte_612: mem_access: \valid(&params->monkeysphere.sig_raw_len); */
	params->monkeysphere.sig_raw_len = remain;
/*@ assert rte_613: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int sig_gost_extract_r_s(u8 const *buf, u32 len, u32 *r_start_off,
u32 *r_len, u32 *s_start_off, u32 *s_len,
u32 *eaten)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 off = (u32)0;
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (3 * 100000 + 5271);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
/*@ assert rte_582: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (3 * 100000 + 5285);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
/*@ assert rte_583: mem_access: \valid(s_start_off); */
	*s_start_off = off;
/*@ assert rte_584: mem_access: \valid(s_len); */
	*s_len = remain / (u32)2;
/*@ assert rte_585: mem_access: \valid(r_start_off); */
/*@ assert rte_586: mem_access: \valid_read(s_len); */
	*r_start_off = off + *s_len;
/*@ assert rte_587: mem_access: \valid(r_len); */
/*@ assert rte_588: mem_access: \valid_read(s_len); */
	*r_len = *s_len;
/*@ assert rte_589: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_gost94(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5337);
			goto out;
		}
	}
	ret = sig_gost_extract_r_s(buf,len,& r_start_off,& r_len,& s_start_off,
	& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_590: mem_access: \valid(&params->gost_r3410_94.r_raw_off); */
	params->gost_r3410_94.r_raw_off = off + r_start_off;
/*@ assert rte_591: mem_access: \valid(&params->gost_r3410_94.r_raw_len); */
	params->gost_r3410_94.r_raw_len = r_len;
/*@ assert rte_592: mem_access: \valid(&params->gost_r3410_94.s_raw_off); */
	params->gost_r3410_94.s_raw_off = off + s_start_off;
/*@ assert rte_593: mem_access: \valid(&params->gost_r3410_94.s_raw_len); */
	params->gost_r3410_94.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_gost2001(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5391);
			goto out;
		}
	}
	ret = sig_gost_extract_r_s(buf,len,& r_start_off,& r_len,& s_start_off,
	& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_594: mem_access: \valid(&params->gost_r3410_2001.r_raw_off); */
	params->gost_r3410_2001.r_raw_off = off + r_start_off;
/*@ assert rte_595: mem_access: \valid(&params->gost_r3410_2001.r_raw_len); */
	params->gost_r3410_2001.r_raw_len = r_len;
/*@ assert rte_596: mem_access: \valid(&params->gost_r3410_2001.s_raw_off); */
	params->gost_r3410_2001.s_raw_off = off + s_start_off;
/*@ assert rte_597: mem_access: \valid(&params->gost_r3410_2001.s_raw_len); */
	params->gost_r3410_2001.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_bign(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5443);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (3 * 100000 + 5463);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
/*@ assert rte_606: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (3 * 100000 + 5477);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
/*@ assert rte_607: mem_access: \valid(&params->bign.sig_raw_off); */
	params->bign.sig_raw_off = off;
/*@ assert rte_608: mem_access: \valid(&params->bign.sig_raw_len); */
	params->bign.sig_raw_len = remain;
/*@ assert rte_609: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_gost2012_256(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5522);
			goto out;
		}
	}
	ret = sig_gost_extract_r_s(buf,len,& r_start_off,& r_len,& s_start_off,
	& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_602: mem_access: \valid(&params->gost_r3410_2012_256.r_raw_off); */
	params->gost_r3410_2012_256.r_raw_off = off + r_start_off;
/*@ assert rte_603: mem_access: \valid(&params->gost_r3410_2012_256.r_raw_len); */
	params->gost_r3410_2012_256.r_raw_len = r_len;
/*@ assert rte_604: mem_access: \valid(&params->gost_r3410_2012_256.s_raw_off); */
	params->gost_r3410_2012_256.s_raw_off = off + s_start_off;
/*@ assert rte_605: mem_access: \valid(&params->gost_r3410_2012_256.s_raw_len); */
	params->gost_r3410_2012_256.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_gost2012_512(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5575);
			goto out;
		}
	}
	ret = sig_gost_extract_r_s(buf,len,& r_start_off,& r_len,& s_start_off,
	& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_598: mem_access: \valid(&params->gost_r3410_2012_512.r_raw_off); */
	params->gost_r3410_2012_512.r_raw_off = off + r_start_off;
/*@ assert rte_599: mem_access: \valid(&params->gost_r3410_2012_512.r_raw_len); */
	params->gost_r3410_2012_512.r_raw_len = r_len;
/*@ assert rte_600: mem_access: \valid(&params->gost_r3410_2012_512.s_raw_off); */
	params->gost_r3410_2012_512.s_raw_off = off + s_start_off;
/*@ assert rte_601: mem_access: \valid(&params->gost_r3410_2012_512.s_raw_len); */
	params->gost_r3410_2012_512.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_rsa_helper(u8 const *buf, u32 len, u32 *bs_data_start_off,
u32 *bs_data_len, u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) goto _LOR;
	else
	if (bs_data_start_off == (u32 *)0) goto _LOR;
	else
	if (bs_data_len == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5627);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	if (len != hdr_len + data_len) {
		ret = - (3 * 100000 + 5642);
		goto out;
	}
	if (data_len == (u32)0) {
		ret = - (3 * 100000 + 5654);
		goto out;
	}
/*@ assert rte_570: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (3 * 100000 + 5665);
		goto out;
	}
	buf ++;
/*@ assert rte_571: mem_access: \valid(bs_data_start_off); */
	*bs_data_start_off = hdr_len + (u32)1;
/*@ assert rte_572: mem_access: \valid(bs_data_len); */
	*bs_data_len = data_len - (u32)1;
/*@ assert rte_573: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_eddsa(u8 const *cert, u32 off, u32 len, u32 exp_sig_len,
u32 *r_start_off, u32 *r_len, u32 *s_start_off,
u32 *s_len, u32 *eaten)
{
	u32 comp_len;
	int ret;
	u32 sig_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 remain = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) goto _LOR;
	else
	if (r_start_off == (u32 *)0) goto _LOR;
	else
	if (r_len == (u32 *)0) goto _LOR;
	else
	if (s_start_off == (u32 *)0) goto _LOR;
	else
	if (s_len == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5721);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	if (len != hdr_len + data_len) {
		ret = - (3 * 100000 + 5736);
		goto out;
	}
	if (data_len == (u32)0) {
		ret = - (3 * 100000 + 5748);
		goto out;
	}
/*@ assert rte_546: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (3 * 100000 + 5759);
		goto out;
	}
	buf ++;
	sig_len = data_len - (u32)1;
	if (sig_len != exp_sig_len) {
		ret = - (3 * 100000 + 5766);
		goto out;
	}
	comp_len = sig_len / (u32)2;
	if (sig_len != comp_len * (u32)2) {
		ret = - (3 * 100000 + 5774);
		goto out;
	}
/*@ assert rte_547: mem_access: \valid(r_start_off); */
	*r_start_off = (off + hdr_len) + (u32)1;
/*@ assert rte_548: mem_access: \valid(r_len); */
	*r_len = comp_len;
/*@ assert rte_549: mem_access: \valid(s_start_off); */
	*s_start_off = ((off + hdr_len) + (u32)1) + comp_len;
/*@ assert rte_550: mem_access: \valid(s_len); */
	*s_len = comp_len;
	if (remain != (u32)0) {
		ret = - (3 * 100000 + 5795);
		goto out;
	}
/*@ assert rte_551: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_ed448(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5834);
			goto out;
		}
	}
	ret = parse_sig_eddsa(cert,off,len,(u32)114,& params->ed448.r_raw_off,
	& params->ed448.r_raw_len,& params->ed448.s_raw_off,
	& params->ed448.s_raw_len,eaten);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_ed25519(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5881);
			goto out;
		}
	}
	ret = parse_sig_eddsa(cert,off,len,(u32)64,& params->ed25519.r_raw_off,
	& params->ed25519.r_raw_len,
	& params->ed25519.s_raw_off,
	& params->ed25519.s_raw_len,eaten);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

int sig_dsa_based_extract_r_s(u8 const *buf, u32 len, u32 *r_start_off,
u32 *r_len, u32 *s_start_off, u32 *s_len,
u32 *eaten)
{
	u32 off;
	int ret;
	u32 bs_hdr_len = (u32)0;
	u32 bs_data_len = (u32)0;
	u32 sig_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 remain = (u32)0;
	u32 saved_sig_len = (u32)0;
	u32 integer_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) goto _LOR;
	else
	if (r_start_off == (u32 *)0) goto _LOR;
	else
	if (r_len == (u32 *)0) goto _LOR;
	else
	if (s_start_off == (u32 *)0) goto _LOR;
	else
	if (s_len == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 5947);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& bs_hdr_len,& bs_data_len);
	if (ret) goto out;
	saved_sig_len = bs_hdr_len + bs_data_len;
	buf += bs_hdr_len;
	off = bs_hdr_len;
	if (bs_data_len == (u32)0) {
		ret = - (3 * 100000 + 5971);
		goto out;
	}
/*@ assert rte_552: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (3 * 100000 + 6000);
		goto out;
	}
	buf ++;
	off += (u32)1;
	sig_len = bs_data_len - (u32)1;
	ret = parse_id_len(buf,sig_len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (sig_len != hdr_len + data_len) {
		ret = - (3 * 100000 + 6024);
		goto out;
	}
	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	integer_len = hdr_len + data_len;
	remain -= integer_len;
	buf += integer_len;
/*@ assert rte_553: mem_access: \valid(r_start_off); */
	*r_start_off = off + hdr_len;
/*@ assert rte_554: mem_access: \valid(r_len); */
	*r_len = data_len;
	off += hdr_len + data_len;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	integer_len = hdr_len + data_len;
	remain -= integer_len;
	buf += hdr_len + data_len;
/*@ assert rte_555: mem_access: \valid(s_start_off); */
	*s_start_off = off + hdr_len;
/*@ assert rte_556: mem_access: \valid(s_len); */
	*s_len = data_len;
	off += integer_len;
	if (remain != (u32)0) {
		ret = - (3 * 100000 + 6090);
		goto out;
	}
/*@ assert rte_557: mem_access: \valid(eaten); */
	*eaten = saved_sig_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_ecdsa(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6133);
			goto out;
		}
	}
	ret = sig_dsa_based_extract_r_s(buf,len,& r_start_off,& r_len,
	& s_start_off,& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_558: mem_access: \valid(&params->ecdsa.r_raw_off); */
	params->ecdsa.r_raw_off = off + r_start_off;
/*@ assert rte_559: mem_access: \valid(&params->ecdsa.r_raw_len); */
	params->ecdsa.r_raw_len = r_len;
/*@ assert rte_560: mem_access: \valid(&params->ecdsa.s_raw_off); */
	params->ecdsa.s_raw_off = off + s_start_off;
/*@ assert rte_561: mem_access: \valid(&params->ecdsa.s_raw_len); */
	params->ecdsa.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_sm2(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6190);
			goto out;
		}
	}
	ret = sig_dsa_based_extract_r_s(buf,len,& r_start_off,& r_len,
	& s_start_off,& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_562: mem_access: \valid(&params->sm2.r_raw_off); */
	params->sm2.r_raw_off = off + r_start_off;
/*@ assert rte_563: mem_access: \valid(&params->sm2.r_raw_len); */
	params->sm2.r_raw_len = r_len;
/*@ assert rte_564: mem_access: \valid(&params->sm2.s_raw_off); */
	params->sm2.s_raw_off = off + s_start_off;
/*@ assert rte_565: mem_access: \valid(&params->sm2.s_raw_len); */
	params->sm2.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_sig_rsa_pkcs1_v15(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 bs_data_start_off = (u32)0;
	u32 bs_data_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6246);
			goto out;
		}
	}
	ret = parse_sig_rsa_helper(buf,len,& bs_data_start_off,& bs_data_len,eaten);
	if (ret) goto out;
/*@ assert rte_574: mem_access: \valid(&params->rsa_pkcs1_v1_5.sig_raw_off); */
	params->rsa_pkcs1_v1_5.sig_raw_off = off + bs_data_start_off;
/*@ assert rte_575: mem_access: \valid(&params->rsa_pkcs1_v1_5.sig_raw_len); */
	params->rsa_pkcs1_v1_5.sig_raw_len = bs_data_len;
	out: ;
	return ret;
}

int parse_sig_rsa_ssa_pss(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 bs_data_start_off = (u32)0;
	u32 bs_data_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6304);
			goto out;
		}
	}
	ret = parse_sig_rsa_helper(buf,len,& bs_data_start_off,& bs_data_len,eaten);
	if (ret) goto out;
/*@ assert rte_576: mem_access: \valid(&params->rsa_ssa_pss.sig_raw_off); */
	params->rsa_ssa_pss.sig_raw_off = off + bs_data_start_off;
/*@ assert rte_577: mem_access: \valid(&params->rsa_ssa_pss.sig_raw_len); */
	params->rsa_ssa_pss.sig_raw_len = bs_data_len;
	out: ;
	return ret;
}

int parse_sig_rsa_9796_2_pad(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 bs_data_start_off = (u32)0;
	u32 bs_data_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6362);
			goto out;
		}
	}
	ret = parse_sig_rsa_helper(buf,len,& bs_data_start_off,& bs_data_len,eaten);
	if (ret) goto out;
/*@ assert rte_578: mem_access: \valid(&params->rsa_9796_2_pad.sig_raw_off); */
	params->rsa_9796_2_pad.sig_raw_off = off + bs_data_start_off;
/*@ assert rte_579: mem_access: \valid(&params->rsa_9796_2_pad.sig_raw_len); */
	params->rsa_9796_2_pad.sig_raw_len = bs_data_len;
	out: ;
	return ret;
}

int parse_sig_rsa_belgian(sig_params *params, u8 const *cert, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 bs_data_start_off = (u32)0;
	u32 bs_data_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6420);
			goto out;
		}
	}
	ret = parse_sig_rsa_helper(buf,len,& bs_data_start_off,& bs_data_len,eaten);
	if (ret) goto out;
/*@ assert rte_580: mem_access: \valid(&params->belgian_rsa.sig_raw_off); */
	params->belgian_rsa.sig_raw_off = off + bs_data_start_off;
/*@ assert rte_581: mem_access: \valid(&params->belgian_rsa.sig_raw_len); */
	params->belgian_rsa.sig_raw_len = bs_data_len;
	out: ;
	return ret;
}

int parse_sig_dsa(sig_params *params, u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u32 r_start_off = (u32)0;
	u32 r_len = (u32)0;
	u32 s_start_off = (u32)0;
	u32 s_len = (u32)0;
	u8 const *buf = cert + off;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6478);
			goto out;
		}
	}
	ret = sig_dsa_based_extract_r_s(buf,len,& r_start_off,& r_len,
	& s_start_off,& s_len,eaten);
	if (ret) goto out;
/*@ assert rte_566: mem_access: \valid(&params->dsa.r_raw_off); */
	params->dsa.r_raw_off = off + r_start_off;
/*@ assert rte_567: mem_access: \valid(&params->dsa.r_raw_len); */
	params->dsa.r_raw_len = r_len;
/*@ assert rte_568: mem_access: \valid(&params->dsa.s_raw_off); */
	params->dsa.s_raw_off = off + s_start_off;
/*@ assert rte_569: mem_access: \valid(&params->dsa.s_raw_len); */
	params->dsa.s_raw_len = s_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_HashAlgorithm(u8 const *buf, u32 len,
_hash_alg const **hash_alg, u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 remain = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (hash_alg == (_hash_alg const **)0) goto _LOR;
	else
	if (eaten == (u32 *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 6536);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
/*@ assert rte_243: mem_access: \valid(hash_alg); */
	*hash_alg = find_hash_by_oid(buf,oid_len);
/*@ assert rte_244: mem_access: \valid_read(hash_alg); */
	if (*hash_alg == (_hash_alg const *)0) {
		ret = - (3 * 100000 + 6561);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	if (remain != (u32)2) goto _LOR_0;
	else
/*@ assert rte_245: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0x05) goto _LOR_0;
	else
/*@ assert rte_246: mem_access: \valid_read(buf + 1); */
	if ((int)*(buf + 1) != 0x00) {
		_LOR_0: {
			ret = - (3 * 100000 + 6573);
			goto out;
		}
	}
/*@ assert rte_247: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

int parse_algoid_sig_params_rsassa_pss(sig_params *params,
hash_alg_id *hash_alg, u8 const *cert,
u32 off, u32 len)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 int_hdr_len = (u32)0;
	u32 int_data_len = (u32)0;
	u32 attr_hdr_len = (u32)0;
	u32 attr_data_len = (u32)0;
	u32 eaten = (u32)0;
	u8 salt_len = (u8)0;
	u8 const *buf = cert + off;
	_hash_alg const *hash = (_hash_alg const *)0;
	_mgf const *mgf = (_mgf const *)0;
	_hash_alg const *mgf_hash = (_hash_alg const *)0;
	u8 trailer_field = (u8)0;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 6638);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& attr_hdr_len,
	& attr_data_len);
	if (ret) hash = & _sha1_hash_alg;
	else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;
		ret = parse_HashAlgorithm(buf,attr_data_len,& hash,& eaten);
		if (ret) goto out;
		if (eaten != attr_data_len) {
			ret = - (3 * 100000 + 6681);
			goto out;
		}
		buf += eaten;
		remain -= eaten;
	}
/*@ assert rte_250: mem_access: \valid(hash_alg); */
/*@ assert rte_251: mem_access: \valid_read(&hash->hash_id); */
	*hash_alg = hash->hash_id;
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)1,& attr_hdr_len,
	& attr_data_len);
	if (ret) {
		mgf = & _mgf1_alg;
		mgf_hash = & _sha1_hash_alg;
	}
	else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;
		ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
		& hdr_len,& data_len);
		if (ret) goto out;
		if (attr_data_len != hdr_len + data_len) {
			ret = - (3 * 100000 + 6724);
			goto out;
		}
		buf += hdr_len;
		remain -= hdr_len;
		ret = parse_OID(buf,data_len,& oid_len);
		if (ret) goto out;
		if (oid_len != _mgf1_alg.alg_der_oid_len) goto _LOR_0;
		else {
			int tmp;
			tmp = bufs_differ(buf,_mgf1_alg.alg_der_oid,oid_len);
			if (tmp) {
				_LOR_0: {
					ret = - (3 * 100000 + 6742);
					goto out;
				}
			}
		}
		mgf = & _mgf1_alg;
		buf += oid_len;
		remain -= oid_len;
		data_len -= oid_len;
		ret = parse_HashAlgorithm(buf,data_len,& mgf_hash,& eaten);
		if (ret) goto out;
		buf += eaten;
		remain -= eaten;
		data_len -= eaten;
		if (data_len != (u32)0) {
			ret = - (3 * 100000 + 6770);
			goto out;
		}
	}
/*@ assert rte_252: mem_access: \valid(&params->rsa_ssa_pss.mgf_alg); */
/*@ assert rte_253: mem_access: \valid_read(&mgf->mgf_id); */
	params->rsa_ssa_pss.mgf_alg = mgf->mgf_id;
/*@ assert rte_254: mem_access: \valid(&params->rsa_ssa_pss.mgf_hash_alg); */
/*@ assert rte_255: mem_access: \valid_read(&mgf_hash->hash_id); */
	params->rsa_ssa_pss.mgf_hash_alg = mgf_hash->hash_id;
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)2,& attr_hdr_len,
	& attr_data_len);
	if (ret) salt_len = (u8)20;
	else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;
		ret = parse_non_negative_integer(buf,attr_data_len,CLASS_UNIVERSAL,
		(u32)ASN1_TYPE_INTEGER,& int_hdr_len,
		& int_data_len);
		if (ret) goto out;
		eaten = int_hdr_len + int_data_len;
		if (eaten != attr_data_len) {
			ret = - (3 * 100000 + 6809);
			goto out;
		}
		if (int_data_len != (u32)1) {
			ret = - (3 * 100000 + 6821);
			goto out;
		}
	/*@ assert rte_256: mem_access: \valid_read(buf + 2); */
		salt_len = *(buf + 2);
		buf += eaten;
		remain -= eaten;
	}
/*@ assert rte_257: mem_access: \valid(&params->rsa_ssa_pss.salt_len); */
	params->rsa_ssa_pss.salt_len = salt_len;
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)3,& attr_hdr_len,
	& attr_data_len);
	if (ret) trailer_field = (u8)1;
	else {
		buf += attr_hdr_len;
		remain -= attr_hdr_len;
		ret = parse_integer(buf,attr_data_len,CLASS_UNIVERSAL,
		(u32)ASN1_TYPE_INTEGER,& int_hdr_len,& int_data_len);
		if (ret) goto out;
		eaten = int_hdr_len + int_data_len;
		if (eaten != attr_data_len) {
			ret = - (3 * 100000 + 6869);
			goto out;
		}
		if (eaten != (u32)3) {
			ret = - (3 * 100000 + 6875);
			goto out;
		}
	/*@ assert rte_258: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) == 1) {
			ret = - (3 * 100000 + 6885);
			goto out;
		}
		ret = - (3 * 100000 + 6894);
		goto out;
	}
/*@ assert rte_259: mem_access: \valid(&params->rsa_ssa_pss.trailer_field); */
	params->rsa_ssa_pss.trailer_field = trailer_field;
	if (remain) {
		ret = - (3 * 100000 + 6904);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires params == \null || \valid(params);
	requires hash_alg == \null || \valid(hash_alg);
	requires (cert == \null) || (len == 0) || \valid_read(cert + (0 .. len - 1));
	assigns \nothing;
*/
int parse_algoid_sig_params_ecdsa_with(sig_params *params,
hash_alg_id *hash_alg, u8 const *cert,
u32 off, u32 len)
{
	int ret;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (3 * 100000 + 6962);
			goto out;
		}
	}
	if (len != (u32)0) ret = - (3 * 100000 + 6968); else ret = 0;
	out: ;
	return ret;
}

int parse_algoid_sig_params_ecdsa_with_specified(sig_params *params,
hash_alg_id *hash_alg,
u8 const *cert, u32 off,
u32 len)
{
	int ret;
	u8 const *buf = cert + off;
	_hash_alg const *hash = (_hash_alg const *)0;
	u32 parsed = (u32)0;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 7007);
			goto out;
		}
	}
	ret = parse_HashAlgorithm(buf,len,& hash,& parsed);
	if (ret) goto out;
	if (parsed != len) {
		ret = - (3 * 100000 + 7023);
		goto out;
	}
/*@ assert rte_248: mem_access: \valid(hash_alg); */
/*@ assert rte_249: mem_access: \valid_read(&hash->hash_id); */
	*hash_alg = hash->hash_id;
	out: ;
	return ret;
}

int parse_algoid_sig_params_bign_with_hspec(sig_params *params,
hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len)
{
	_hash_alg const *hash;
	u32 remain;
	int ret;
	u8 const *buf = cert + off;
	u32 oid_len = (u32)0;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 7066);
			goto out;
		}
	}
	remain = len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	hash = find_hash_by_oid(buf,oid_len);
	if (hash == (_hash_alg const *)0) {
		ret = - (3 * 100000 + 7083);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	if (remain) {
		ret = - (3 * 100000 + 7092);
		goto out;
	}
/*@ assert rte_260: mem_access: \valid(hash_alg); */
/*@ assert rte_261: mem_access: \valid_read(&hash->hash_id); */
	*hash_alg = hash->hash_id;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(params) || params == \null;
	requires \valid(hash_alg) || hash_alg == \null;
	requires cert == \null || len == 0 || \valid_read(cert + off + (0 .. len-1));
	assigns \nothing;
	ensures (params == \null) ==> (\result == -(3 * 100000 + 7132));
	ensures (params != \null && hash_alg == \null) ==> (\result == -(3 * 100000 + 7132));
	ensures (params != \null && hash_alg != \null && cert == \null) ==> (\result == -(3 * 100000 + 7132));
	ensures (params != \null && hash_alg != \null && cert != \null) ==> (\result == 0);
	ensures (len == 0) ==> (\result == 0);
	ensures (len == 2) ==> (\result == 0);
	ensures ((cert + off)[0] == 0x05 && (cert + off)[1] == 0x00) ==> (\result == 0);
	ensures (params != \null && hash_alg != \null && cert != \null) ==> (\result == -(3 * 100000 + 1146));
	ensures (len == 2) ==> (\result == -(3 * 100000 + 1146));
	ensures (!((cert + off)[0] == 0x05 && (cert + off)[1] == 0x00)) ==> (\result == -(3 * 100000 + 1146));
	ensures (params != \null && hash_alg != \null && cert != \null) ==> (\result == -(3 * 100000 + 7150));
	ensures (len != 0 && len != 2) ==> (\result == -(3 * 100000 + 7150));
*/
int parse_algoid_sig_params_sm2(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	u8 const *buf = cert + off;
	u32 parsed = (u32)0;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (3 * 100000 + 7132);
			goto out;
		}
	}
	switch (len) {
		case (u32)0: ret = 0;
		break;
		case (u32)2: ret = parse_null(buf,len,& parsed);
		int tmp = ret;
		break;
		default: ret = - (3 * 100000 + 7150);
		break;
	}
	out: ;
	return ret;
}

/*@
	requires \valid(params) || params == \null;
	requires \valid(hash_alg) || hash_alg == \null;
	requires (cert == \null) || (len == 0) || \valid_read(cert + (0 .. len - 1));
	assigns \nothing;
	ensures (params == \null) ==> (\result == -(3 * 100000 + 7185));
	ensures (params != \null && hash_alg == \null) ==> (\result == -(3 * 100000 + 7185));
	ensures (params != \null && hash_alg != \null && cert == \null) ==> (\result == -(3 * 100000 + 7185));
	ensures (params != \null && hash_alg != \null && cert != \null && len != 0) ==> (\result == -(3 * 100000 + 7185));
	ensures (params != \null && hash_alg != \null && cert != \null && len == 0) ==> (\result == 0);
	ensures \result == 0 || \result == -(3 * 100000 + 7185);
*/
int parse_algoid_sig_params_eddsa(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len != (u32)0) {
		_LOR: {
			ret = - (3 * 100000 + 7185);
			goto out;
		}
	}
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires params == \null || \valid(params);
	requires hash_alg == \null || \valid(hash_alg);
	requires cert == \null || len == 0 || \valid_read((cert + off) + (0 .. len - 1));
	requires off <= UINT32_MAX - len;
*/
int parse_algoid_sig_params_none(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) {
		_LOR: {
			ret = - (3 * 100000 + 7216);
			goto out;
		}
	}
	ret = parse_algoid_params_none(cert,off,len);
	if (ret) goto out;
	out: ;
	return ret;
}

/*@
	requires cert == \null || len == 0 || \valid_read(cert + off + (0 .. len - 1));
	requires off <= UINT32_MAX - len;
	assigns \nothing;
	ensures cert == \null ==> \result == -(3 * 100000 + 7253);
	ensures (cert != \null && len == 0) ==> \result == 0;
	ensures (cert != \null && len == 2 &&(cert + off)[0] == 0x05 && (cert + off)[1] == 0x00) ==>\result == 0;
	ensures (cert != \null && len == 2 &&!((cert + off)[0] == 0x05 && (cert + off)[1] == 0x00)) ==>\result == -(3 * 100000 + 1146);
	ensures (cert != \null && len != 0 && len != 2) ==> \result == -1;
	ensures \result == 0 ||\result == -1 ||\result == -(3 * 100000 + 7253) ||\result == -(3 * 100000 + 1146);
*/
int parse_algoid_params_none(u8 const *cert, u32 off, u32 len)
{
	int ret;
	u8 const *buf = cert + off;
	u32 parsed = (u32)0;
	if (cert == (u8 const *)0) {
		ret = - (3 * 100000 + 7253);
		goto out;
	}
	switch (len) {
		case (u32)0: ret = 0;
		break;
		case (u32)2: ret = parse_null(buf,len,& parsed);
		int tmp = ret;
		break;
		default: ret = -1;
		break;
	}
	out: ;
	return ret;
}

/*@
	requires cert == \null || \valid_read(cert + (0 .. off + len - 1));
	requires off <= UINT32_MAX - len;
	assigns \nothing;
	ensures cert == \null ==> \result == -(3 * 100000 + 7304);
	ensures (cert != \null && len == 0) ==> \result == 0;
	ensures (cert != \null && len != 0 && len != 2) ==> \result == -(3 * 100000 + 1139);
	ensures (cert != \null && len == 2 &&(cert[off] != 0x05 || cert[off + 1] != 0x00)) ==>\result == -(3 * 100000 + 1146);
	ensures (cert != \null && len == 2 &&cert[off] == 0x05 && cert[off + 1] == 0x00) ==>\result == 0;
	ensures \result == 0 ||\result == -(3 * 100000 + 7304) ||\result == -(3 * 100000 + 1139) ||\result == -(3 * 100000 + 1146);
*/
int parse_algoid_params_rsa(u8 const *cert, u32 off, u32 len)
{
	int ret;
	u8 const *buf = cert + off;
	u32 parsed = (u32)0;
	if (cert == (u8 const *)0) {
		ret = - (3 * 100000 + 7304);
		goto out;
	}
	if (len == (u32)0) {
		ret = 0;
		goto out;
	}
	ret = parse_null(buf,len,& parsed);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires cert == \null || \valid_read(cert + (0 .. off + len - 1));
	requires off <= UINT32_MAX - len;
	requires params == \null || \valid(params);
	requires hash_alg == \null || \valid(hash_alg);
	assigns \nothing;
	ensures (cert == \null || params == \null || hash_alg == \null)==> \result == -(3 * 100000 + 7361);
	ensures (cert != \null && params != \null && hash_alg != \null && len == 0)==> \result == 0;
	ensures (cert != \null && params != \null && hash_alg != \null && len != 0 && len != 2)==> \result == -(3 * 100000 + 1139);
	ensures (cert != \null && params != \null && hash_alg != \null && len == 2 &&(cert[off] != 0x05 || cert[off + 1] != 0x00))==> \result == -(3 * 100000 + 1146);
	ensures (cert != \null && params != \null && hash_alg != \null && len == 2 &&cert[off] == 0x05 && cert[off + 1] == 0x00)==> \result == 0;
	ensures \result == 0 ||\result == -(3 * 100000 + 7361) ||\result == -(3 * 100000 + 1139) ||\result == -(3 * 100000 + 1146);
*/
int parse_algoid_sig_params_rsa(sig_params *params, hash_alg_id *hash_alg,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (params == (sig_params *)0) goto _LOR;
	else
	if (hash_alg == (hash_alg_id *)0) {
		_LOR: {
			ret = - (3 * 100000 + 7361);
			goto out;
		}
	}
	ret = parse_algoid_params_rsa(cert,off,len);
	if (ret) goto out;
	out: ;
	return ret;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result == (u64)na_sec+ ((u64)na_min << 8)+ ((u64)na_hour << 16)+ ((u64)na_day << 24)+ ((u64)na_month << 32)+ ((u64)na_year << 40);
*/
u64 time_components_to_comparable_u64(u16 na_year, u8 na_month, u8 na_day,
u8 na_hour, u8 na_min, u8 na_sec)
{
	u64 res;
	u64 tmp;
	res = (u64)na_sec;
	tmp = (u64)((unsigned long long)((u64)na_min) * (1ULL << 8));
	res += tmp;
	tmp = (u64)((unsigned long long)((u64)na_hour) * (1ULL << 16));
	res += tmp;
	tmp = (u64)((unsigned long long)((u64)na_day) * (1ULL << 24));
	res += tmp;
	tmp = (u64)((unsigned long long)((u64)na_month) * (1ULL << 32));
	res += tmp;
	tmp = (u64)((unsigned long long)((u64)na_year) * (1ULL << 40));
	res += tmp;
	return res;
}

int parse_x509_crl(crl_parsing_ctx *ctx, u8 const *crl, u32 len);

static u8 const _crl_entry_ext_oid_ReasonCode[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x15};

static u8 const _crl_entry_ext_oid_InvalidityDate[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x18};

static u8 const _crl_entry_ext_oid_CertIssuer[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x1d};

/*@
	requires valid_ctx: ctx == \null || \valid(ctx);
	requires valid_crl: crl == \null || (off <= UINT32_MAX - len && \valid_read(crl + (off .. off + len - 1)));
	requires no_overflow: crl != \null ==> off + len <= UINT32_MAX;
	assigns \nothing;
	ensures (ctx == \null) ==> (\result == -(5 * 100000 + 114));
	ensures (ctx != \null && crl == \null) ==> (\result == -(5 * 100000 + 114));
	ensures (ctx != \null && crl != \null && critical != 0) ==> (\result == -(5 * 100000 + 124));
	ensures (ctx != \null && crl != \null && critical == 0) ==> (\result == -(5 * 100000 + 135));
	ensures (len != 3 || crl[off] != ASN1_TYPE_ENUMERATED || crl[off + 1] != 0x01) ==> (\result == -(5 * 100000 + 135));
	ensures (ctx != \null && crl != \null && critical == 0) ==> (\result == -(5 * 100000 + 156));
	ensures (len == 3 && crl[off] == ASN1_TYPE_ENUMERATED && crl[off + 1] == 0x01) ==> (\result == -(5 * 100000 + 156));
	ensures (crl[off + 2] != crl_reason_unspecified &&crl[off + 2] != crl_reason_keyCompromise &&crl[off + 2] != crl_reason_cACompromise &&crl[off + 2] != crl_reason_affiliationChanged &&crl[off + 2] != crl_reason_superseded &&crl[off + 2] != crl_reason_cessationOfOperation &&crl[off + 2] != crl_reason_certificateHold &&crl[off + 2] != crl_reason_removeFromCRL &&crl[off + 2] != crl_reason_privilegeWithdrawn &&crl[off + 2] != crl_reason_aACompromise) ==> (\result == -(5 * 100000 + 156));
	ensures (ctx != \null && crl != \null && critical == 0) ==> (\result == 0);
	ensures (len == 3 && crl[off] == ASN1_TYPE_ENUMERATED && crl[off + 1] == 0x01) ==> (\result == 0);
	ensures (crl[off + 2] == crl_reason_unspecified ||crl[off + 2] == crl_reason_keyCompromise ||crl[off + 2] == crl_reason_cACompromise ||crl[off + 2] == crl_reason_affiliationChanged ||crl[off + 2] == crl_reason_superseded ||crl[off + 2] == crl_reason_cessationOfOperation ||crl[off + 2] == crl_reason_certificateHold ||crl[off + 2] == crl_reason_removeFromCRL ||crl[off + 2] == crl_reason_privilegeWithdrawn ||crl[off + 2] == crl_reason_aACompromise) ==> (\result == 0);
	ensures \result == 0 ||\result == -(5 * 100000 + 114) ||\result == -(5 * 100000 + 124) ||\result == -(5 * 100000 + 135) ||\result == -(5 * 100000 + 156);
*/
static int parse_crl_entry_ext_ReasonCode(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
int critical)
{
	crl_reason reasonCode;
	u8 const *buf = crl + off;
	int ret = -1;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) {
		_LOR: {
			ret = - (5 * 100000 + 114);
			goto out;
		}
	}
	if (critical) {
		ret = - (5 * 100000 + 124);
		goto out;
	}
	if (len != (u32)3) goto _LOR_0;
	else
/*@ assert rte_665: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != ASN1_TYPE_ENUMERATED) goto _LOR_0;
	else
/*@ assert rte_666: mem_access: \valid_read(buf + 1); */
	if ((int)*(buf + 1) != 0x01) {
		_LOR_0: {
			ret = - (5 * 100000 + 135);
			goto out;
		}
	}
/*@ assert rte_667: mem_access: \valid_read(buf + 2); */
	reasonCode = (crl_reason)*(buf + 2);
	switch (reasonCode) {
		case crl_reason_unspecified: case crl_reason_keyCompromise:
		case crl_reason_cACompromise: case crl_reason_affiliationChanged:
		case crl_reason_superseded: case crl_reason_cessationOfOperation:
		case crl_reason_certificateHold: case crl_reason_removeFromCRL:
		case crl_reason_privilegeWithdrawn: case crl_reason_aACompromise:
		ret = 0;
		break;
		default: ret = - (5 * 100000 + 156);
		goto out;
	}
	out: ;
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires \valid_read(crl + (0 .. len - 1)) || crl == \null;
	requires off <= 0xFFFFFFFF - len || len == 0;
	requires (crl != \null && ctx != \null) ==> \valid_read(crl + off + (0 .. len - 1));
	requires \separated(ctx, crl + (0 .. off + len - 1));
	assigns \nothing;
	ensures (ctx == \null) ==> (\result == -(5 * 100000 + 194));
	ensures (ctx != \null && crl == \null) ==> (\result == -(5 * 100000 + 194));
	ensures (ctx != \null && crl != \null && critical != 0) ==> (\result == -(5 * 100000 + 204));
	ensures (ctx != \null && crl != \null && critical == 0) ==> (\result <= 0);
	ensures (ctx != \null && crl != \null && critical == 0) ==> (\result == 0 || \result < 0);
*/
static int parse_crl_entry_ext_InvalidityDate(crl_parsing_ctx *ctx,
u8 const *crl, u32 off,
u32 len, int critical)
{
	u32 eaten;
	u16 year;
	u8 month = (u8)0;
	u8 day = (u8)0;
	u8 hour = (u8)0;
	u8 min = (u8)0;
	u8 sec = (u8)0;
	u8 const *buf = crl + off;
	int ret = -1;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) {
		_LOR: {
			ret = - (5 * 100000 + 194);
			goto out;
		}
	}
	if (critical) {
		ret = - (5 * 100000 + 204);
		goto out;
	}
	ret = parse_generalizedTime(buf,len,& eaten,& year,& month,& day,& hour,
	& min,& sec);
	if (ret) goto out;
	out: ;
	return ret;
}

static int parse_crl_entry_ext_CertIssuer(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
int critical)
{
	u32 eaten;
	int ret;
	u8 const *buf = crl + off;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) {
		_LOR: {
			ret = - (5 * 100000 + 248);
			goto out;
		}
	}
	if (! critical) {
		ret = - (5 * 100000 + 261);
		goto out;
	}
	ret = parse_GeneralNames(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& eaten);
	if (ret) goto out;
	out: ;
	return ret;
}

static _crl_entry_ext_oid const known_crl_entry_ext_oids[3] =
{{.oid = _crl_entry_ext_oid_ReasonCode,
		.oid_len = (u8)sizeof(_crl_entry_ext_oid_ReasonCode),
	.parse_crl_entry_ext_params = & parse_crl_entry_ext_ReasonCode},
	{.oid = _crl_entry_ext_oid_InvalidityDate,
		.oid_len = (u8)sizeof(_crl_entry_ext_oid_InvalidityDate),
	.parse_crl_entry_ext_params = & parse_crl_entry_ext_InvalidityDate},
	{.oid = _crl_entry_ext_oid_CertIssuer,
		.oid_len = (u8)sizeof(_crl_entry_ext_oid_CertIssuer),
.parse_crl_entry_ext_params = & parse_crl_entry_ext_CertIssuer}};

/*@
	requires buf == \null || \valid_read(buf + (0 .. len - 1)) || len == 0;
	assigns \nothing;
	ensures \result == \null ||(\exists integer i; 0 <= i < 3 && \result == &known_crl_entry_ext_oids[i]);
	ensures (buf == \null || len == 0) ==> \result == \null;
	ensures \result != \null ==>(u32)\result->oid_len == len;
	ensures \result != \null ==>\valid_read(\result->oid + (0 .. (u32)\result->oid_len - 1));
	ensures \result != \null && buf != \null && len != 0 ==>(\forall integer j; 0 <= j < len ==> \result->oid[j] == buf[j]);
	ensures \result == \null && buf != \null && len != 0 ==>(\forall integer i; 0 <= i < 3 ==>(u32)known_crl_entry_ext_oids[i].oid_len != len ||(\exists integer j; 0 <= j < len &&known_crl_entry_ext_oids[i].oid[j] != buf[j]));
*/
static _crl_entry_ext_oid const *find_crl_entry_ext_by_oid(u8 const *buf,
u32 len)
{
	u16 k;
	_crl_entry_ext_oid const *found = (_crl_entry_ext_oid const *)0;
	_crl_entry_ext_oid const *cur = (_crl_entry_ext_oid const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u16)0;
	/*@
		loop invariant 0 <= k <= 3;
		loop invariant found == \null ||(\exists integer i; 0 <= i < k && found == &known_crl_entry_ext_oids[i]);
		loop invariant found != \null ==>((u32)found->oid_len == len &&bufs_differ(found->oid, buf, len) == 0);
		loop invariant found == \null ==>(\forall integer i; 0 <= i < k ==>(u32)known_crl_entry_ext_oids[i].oid_len != len ||bufs_differ(known_crl_entry_ext_oids[i].oid, buf, (u32)known_crl_entry_ext_oids[i].oid_len) != 0);
		loop assigns k, cur, found;
		loop variant 3 - k;
	*/
	while ((unsigned long)k < sizeof(known_crl_entry_ext_oids) / sizeof(known_crl_entry_ext_oids[0])) {
		{
			int ret;
			int tmp_0;
			int tmp;
			cur = & known_crl_entry_ext_oids[k];
		/*@ assert rte_657: mem_access: \valid_read(&cur->oid_len); */
			if ((u32)cur->oid_len != len) goto __Cont;
		/*@ assert rte_658: mem_access: \valid_read(&cur->oid); */
		/*@ assert rte_659: mem_access: \valid_read(&cur->oid_len); */
			tmp = bufs_differ(cur->oid,buf,(u32)cur->oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_660: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u16)((int)k + 1);
	}
	out: ;
	return found;
}

/*@
	requires \valid_read(ext);
	requires \valid(parsed_oid_list + (0 .. 2));
	requires \separated(ext, parsed_oid_list + (0 .. 2));
	assigns *(parsed_oid_list + (0 .. 2));
	ensures \result == 0 || \result == -500383 || \result == -500395;
	ensures \result == 0 ==>@   (\exists integer i; 0 <= i < 3 &&@     \old(*(parsed_oid_list + i)) == (_crl_entry_ext_oid const *)0 &&@     *(parsed_oid_list + i) == ext &&@     (\forall integer j; 0 <= j < i ==> \old(*(parsed_oid_list + j)) != ext));
	ensures \result == -500383 ==>@   (\exists integer i; 0 <= i < 3 &&@     \old(*(parsed_oid_list + i)) == ext &&@     (\forall integer j; 0 <= j < i ==>@       \old(*(parsed_oid_list + j)) != (_crl_entry_ext_oid const *)0 &&@       \old(*(parsed_oid_list + j)) != ext));
	ensures \result == -500395 ==>@   (\forall integer i; 0 <= i < 3 ==>@     \old(*(parsed_oid_list + i)) != (_crl_entry_ext_oid const *)0 &&@     \old(*(parsed_oid_list + i)) != ext);
	ensures \result != 0 ==>@   (\forall integer i; 0 <= i < 3 ==> *(parsed_oid_list + i) == \old(*(parsed_oid_list + i)));
*/
static int check_record_crl_entry_ext_unknown(_crl_entry_ext_oid const *ext,
_crl_entry_ext_oid const **parsed_oid_list)
{
	int ret;
	u16 pos = (u16)0;
	/*@
		loop invariant 0 <= pos <= 3;
		loop invariant \forall integer j; 0 <= j < pos ==>@   *(parsed_oid_list + j) != (_crl_entry_ext_oid const *)0 &&@   *(parsed_oid_list + j) != ext;
		loop assigns pos;
		loop variant 3 - pos;
	*/
	while ((unsigned long)pos < sizeof(known_crl_entry_ext_oids) / sizeof(known_crl_entry_ext_oids[0])) {
	/*@ assert rte_661: mem_access: \valid_read(parsed_oid_list + (int)pos); */
		if (*(parsed_oid_list + (int)pos) == (_crl_entry_ext_oid const *)0) {
		/*@ assert rte_662: mem_access: \valid(parsed_oid_list + (int)pos); */
			*(parsed_oid_list + (int)pos) = ext;
			break;
		}
	/*@ assert rte_663: mem_access: \valid_read(parsed_oid_list + (int)pos); */
		if (ext == *(parsed_oid_list + (int)pos)) {
			ret = - (5 * 100000 + 383);
			goto out;
		}
	/*@ assert rte_664: signed_overflow: (int)pos + 1 <= 2147483647; */
		pos = (u16)((int)pos + 1);
	}
	if ((unsigned long)pos >= sizeof(known_crl_entry_ext_oids) / sizeof(known_crl_entry_ext_oids[0])) {
		ret = - (5 * 100000 + 395);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_entry_Extension(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
_crl_entry_ext_oid const **parsed_oid_list,
u32 *eaten)
{
	u32 remain;
	int ret;
	u32 ext_hdr_len = (u32)0;
	u32 ext_data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 saved_ext_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 parsed = (u32)0;
	u8 const *buf = crl + off;
	_crl_entry_ext_oid const *ext = (_crl_entry_ext_oid const *)0;
	int critical = 0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 440);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& ext_hdr_len,& ext_data_len);
	if (ret) goto out;
	buf += ext_hdr_len;
	off += ext_hdr_len;
	remain -= ext_hdr_len;
	saved_ext_len = ext_hdr_len + ext_data_len;
	ret = parse_OID(buf,ext_data_len,& oid_len);
	if (ret) goto out;
	ext = find_crl_entry_ext_by_oid(buf,oid_len);
	if (ext == (_crl_entry_ext_oid const *)0) {
		ret = - (5 * 100000 + 479);
		goto out;
	}
	ret = check_record_crl_entry_ext_unknown(ext,parsed_oid_list);
	if (ret) goto out;
	buf += oid_len;
	off += oid_len;
	ext_data_len -= oid_len;
	ret = parse_boolean(buf,ext_data_len,& parsed);
	if (ret) {
		if (ext_data_len)
	/*@ assert rte_668: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) == ASN1_TYPE_BOOLEAN) {
			ret = - (5 * 100000 + 533);
			goto out;
		}
	}
	else {
		if (parsed != (u32)3) {
			ret = - (5 * 100000 + 545);
			goto out;
		}
		critical = 1;
		buf += parsed;
		off += parsed;
		ext_data_len -= parsed;
	}
	ret = parse_id_len(buf,ext_data_len,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_OCTET_STRING,& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	ext_data_len -= hdr_len;
	if (data_len != ext_data_len) {
		ret = - (5 * 100000 + 587);
		goto out;
	}
	if (ext == known_crl_entry_ext_oids) ret = parse_crl_entry_ext_ReasonCode
	(ctx,crl,off,ext_data_len,critical);
	else
	if (ext == & known_crl_entry_ext_oids[1]) ret = parse_crl_entry_ext_InvalidityDate
	(ctx,crl,off,ext_data_len,
	critical);
	else
	if (ext == & known_crl_entry_ext_oids[2]) ret = parse_crl_entry_ext_CertIssuer
	(ctx,crl,off,ext_data_len,
	critical);
	if (ret) goto out;
/*@ assert rte_669: mem_access: \valid(eaten); */
	*eaten = saved_ext_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_entry_Extensions(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
u32 *eaten)
{
	_crl_entry_ext_oid const *parsed_crl_entry_ext_oid_list[sizeof(known_crl_entry_ext_oids) / sizeof(known_crl_entry_ext_oids[0])];
	int ret;
	u16 i;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u8 const *buf = crl + off;
	u32 saved_len = (u32)0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 642);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	saved_len = hdr_len + data_len;
	i = (u16)0;
	while ((unsigned long)i < sizeof(known_crl_entry_ext_oids) / sizeof(known_crl_entry_ext_oids[0])) {
	/*@ assert rte_670: index_bound: i < (unsigned long)(sizeof(_crl_entry_ext_oid const [3]) / sizeof(_crl_entry_ext_oid)); */
		parsed_crl_entry_ext_oid_list[i] = (_crl_entry_ext_oid const *)0;
	/*@ assert rte_671: signed_overflow: (int)i + 1 <= 2147483647; */
		i = (u16)((int)i + 1);
	}
	while (remain) {
		u32 ext_len = (u32)0;
		ret = parse_x509_crl_entry_Extension(ctx,crl,off,remain,
		parsed_crl_entry_ext_oid_list,
		& ext_len);
		if (ret) goto out;
		remain -= ext_len;
		buf += ext_len;
		off += ext_len;
	}
/*@ assert rte_672: mem_access: \valid(eaten); */
	*eaten = saved_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_revokedCertificate(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u8 const *buf = crl + off;
	u32 rev_len = (u32)0;
	u16 rev_year = (u16)0;
	u8 rev_month = (u8)0;
	u8 rev_day = (u8)0;
	u8 rev_hour = (u8)0;
	u8 rev_min = (u8)0;
	u8 rev_sec = (u8)0;
	u8 t_type = (u8)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 parsed = (u32)0;
	u32 remain = len;
	u32 saved_rev_len = (u32)0;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 769);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	off += hdr_len;
	saved_rev_len = hdr_len + data_len;
	ret = parse_SerialNumber(crl,off,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& parsed);
	if (ret) goto out;
	buf += parsed;
	remain -= parsed;
	off += parsed;
	ret = parse_Time(buf,remain,& t_type,& rev_len,& rev_year,& rev_month,
	& rev_day,& rev_hour,& rev_min,& rev_sec);
	if (ret) goto out;
	ret = verify_correct_time_use(t_type,rev_year);
	if (ret) goto out;
	buf += rev_len;
	remain -= rev_len;
	off += rev_len;
/*@ assert rte_673: mem_access: \valid_read(&ctx->version); */
	if ((int)ctx->version != 0x01)
	if (remain) {
		ret = - (5 * 100000 + 823);
		goto out;
	}
	if (remain) {
		ret = parse_x509_crl_entry_Extensions(ctx,crl,off,remain,& parsed);
		if (ret) goto out;
		remain -= parsed;
		off += parsed;
		buf += parsed;
	}
/*@ assert rte_674: mem_access: \valid(eaten); */
	*eaten = saved_rev_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_crl_ext_AKI(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
u32 len, int critical)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = crl + off;
	u32 key_id_hdr_len = (u32)0;
	u32 key_id_data_len = (u32)0;
	u32 key_id_data_off = (u32)0;
	u32 gen_names_off = (u32)0;
	u32 gen_names_len = (u32)0;
	u32 cert_serial_off = (u32)0;
	u32 cert_serial_len = (u32)0;
	u32 parsed = (u32)0;
	int has_keyIdentifier = 0;
	int has_gen_names_and_serial = 0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) {
		_LOR: {
			ret = - (5 * 100000 + 909);
			goto out;
		}
	}
	if (critical) {
		ret = - (5 * 100000 + 921);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (5 * 100000 + 939);
		goto out;
	}
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,
	& key_id_hdr_len,& key_id_data_len);
	if (! ret) {
		if (! key_id_data_len) {
			ret = - (5 * 100000 + 952);
			goto out;
		}
		key_id_data_off = off + key_id_hdr_len;
		buf += key_id_hdr_len + key_id_data_len;
		off += key_id_hdr_len + key_id_data_len;
		remain -= key_id_hdr_len + key_id_data_len;
		has_keyIdentifier = 1;
	}
	ret = parse_GeneralNames(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)1,& parsed);
	if (! ret) {
		gen_names_off = off;
		gen_names_len = parsed;
		buf += parsed;
		off += parsed;
		remain -= parsed;
		ret = parse_AKICertSerialNumber(crl,off,remain,CLASS_CONTEXT_SPECIFIC,
		(u32)2,& cert_serial_len);
		if (ret) goto out;
		has_gen_names_and_serial = 1;
		cert_serial_off = off;
		buf += cert_serial_len;
		off += cert_serial_len;
		remain -= cert_serial_len;
	}
	if (remain) {
		ret = - (5 * 100000 + 999);
		goto out;
	}
/*@ assert rte_687: mem_access: \valid(&ctx->aki_has_keyIdentifier); */
	ctx->aki_has_keyIdentifier = has_keyIdentifier;
/*@ assert rte_688: mem_access: \valid_read(&ctx->aki_has_keyIdentifier); */
	if (ctx->aki_has_keyIdentifier) {
	/*@ assert rte_689: mem_access: \valid(&ctx->aki_keyIdentifier_start); */
		ctx->aki_keyIdentifier_start = key_id_data_off;
	/*@ assert rte_690: mem_access: \valid(&ctx->aki_keyIdentifier_len); */
		ctx->aki_keyIdentifier_len = key_id_data_len;
	}
/*@ assert rte_691: mem_access: \valid(&ctx->aki_has_generalNames_and_serial); */
	ctx->aki_has_generalNames_and_serial = has_gen_names_and_serial;
/*@ assert rte_692: mem_access: \valid_read(&ctx->aki_has_generalNames_and_serial); */
	if (ctx->aki_has_generalNames_and_serial) {
	/*@ assert rte_693: mem_access: \valid(&ctx->aki_generalNames_start); */
		ctx->aki_generalNames_start = gen_names_off;
	/*@ assert rte_694: mem_access: \valid(&ctx->aki_generalNames_len); */
		ctx->aki_generalNames_len = gen_names_len;
	/*@ assert rte_695: mem_access: \valid(&ctx->aki_serial_start); */
		ctx->aki_serial_start = cert_serial_off + (u32)2;
	/*@ assert rte_696: mem_access: \valid(&ctx->aki_serial_len); */
		ctx->aki_serial_len = cert_serial_len - (u32)2;
	}
/*@ assert rte_697: mem_access: \valid(&ctx->has_aki); */
	ctx->has_aki = 1;
	ret = 0;
	out: ;
	return ret;
}

static int parse_crl_ext_IAN(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
u32 len, int critical)
{
	int ret;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 eaten = (u32)0;
	u8 const *buf = crl + off;
	int unused = 0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) {
		_LOR: {
			ret = - (5 * 100000 + 1062);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (! data_len) {
		ret = - (5 * 100000 + 1097);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (5 * 100000 + 1107);
		goto out;
	}
	while (remain) {
		ret = parse_GeneralName(buf,remain,& eaten,& unused);
		if (ret) goto out;
		remain -= eaten;
		off += eaten;
		buf += eaten;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_crl_ext_CRLnum(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
u32 len, int critical)
{
	int ret;
	u8 const *buf = crl + off;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (5 * 100000 + 1166);
			goto out;
		}
	}
	if (critical) {
		ret = - (5 * 100000 + 1173);
		goto out;
	}
	ret = parse_integer(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_INTEGER,
	& hdr_len,& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
	if (parsed > (u32)22) {
		ret = - (5 * 100000 + 1202);
		goto out;
	}
/*@ assert rte_698: mem_access: \valid_read(buf + 2); */
	if ((int)*(buf + 2) & 0x80) {
		ret = - (5 * 100000 + 1209);
		goto out;
	}
/*@ assert rte_699: mem_access: \valid(&ctx->has_crlnumber); */
	ctx->has_crlnumber = 1;
/*@ assert rte_700: mem_access: \valid(&ctx->crlnumber_start); */
	ctx->crlnumber_start = off + hdr_len;
/*@ assert rte_701: mem_access: \valid(&ctx->crlnumber_len); */
	ctx->crlnumber_len = data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_crl_ext_DeltaCRL_indicator(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
int critical)
{
	int ret;
	u8 const *buf = crl + off;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (5 * 100000 + 1252);
			goto out;
		}
	}
	if (! critical) {
		ret = - (5 * 100000 + 1262);
		goto out;
	}
	ret = parse_integer(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_INTEGER,
	& hdr_len,& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
	if (parsed > (u32)22) {
		ret = - (5 * 100000 + 1289);
		goto out;
	}
/*@ assert rte_702: mem_access: \valid_read(buf + 2); */
	if ((int)*(buf + 2) & 0x80) {
		ret = - (5 * 100000 + 1296);
		goto out;
	}
	out: ;
	return ret;
}

/*@
	requires eaten_valid: \valid(eaten);
	requires buf_valid: buf == \null || \valid_read(buf + (0 .. len - 1));
	requires tag_bound: 0 <= tag <= 0x1f;
	assigns *eaten;
	ensures (buf == \null || len == 0) ==> (\result == -(5 * 100000 + 1341));
	ensures (buf != \null && len != 0 && len < 3) ==> (\result == -(5 * 100000 + 1347));
	ensures (buf != \null && len >= 3) ==> (\result == -(5 * 100000 + 1357));
	ensures ((((int)buf[0] >> 6) & 0x03) != CLASS_CONTEXT_SPECIFIC ||(((int)buf[0] >> 5) & 0x01) != 0 ||((int)buf[0] & 0x1f) != (int)tag) ==> (\result == -(5 * 100000 + 1357));
	ensures (buf != \null && len >= 3) ==> (\result == -(5 * 100000 + 1363));
	ensures ((((int)buf[0] >> 6) & 0x03) == CLASS_CONTEXT_SPECIFIC) ==> (\result == -(5 * 100000 + 1363));
	ensures ((((int)buf[0] >> 5) & 0x01) == 0) ==> (\result == -(5 * 100000 + 1363));
	ensures (((int)buf[0] & 0x1f) == (int)tag) ==> (\result == -(5 * 100000 + 1363));
	ensures ((int)buf[1] != 0x01) ==> (\result == -(5 * 100000 + 1363));
	ensures (buf != \null && len >= 3) ==> (\result == -(5 * 100000 + 1373));
	ensures ((((int)buf[0] >> 6) & 0x03) == CLASS_CONTEXT_SPECIFIC) ==> (\result == -(5 * 100000 + 1373));
	ensures ((((int)buf[0] >> 5) & 0x01) == 0) ==> (\result == -(5 * 100000 + 1373));
	ensures (((int)buf[0] & 0x1f) == (int)tag) ==> (\result == -(5 * 100000 + 1373));
	ensures ((int)buf[1] == 0x01) ==> (\result == -(5 * 100000 + 1373));
	ensures ((int)buf[2] != 0x00 && (int)buf[2] != 0xff) ==> (\result == -(5 * 100000 + 1373));
	ensures (buf != \null && len >= 3) ==> (\result == 0);
	ensures ((((int)buf[0] >> 6) & 0x03) == CLASS_CONTEXT_SPECIFIC) ==> (\result == 0);
	ensures ((((int)buf[0] >> 5) & 0x01) == 0) ==> (\result == 0);
	ensures (((int)buf[0] & 0x1f) == (int)tag) ==> (\result == 0);
	ensures ((int)buf[1] == 0x01) ==> (\result == 0);
	ensures ((int)buf[2] == 0x00 || (int)buf[2] == 0xff) ==> (\result == 0);
	ensures (buf != \null && len >= 3) ==> (*eaten == 3);
	ensures ((((int)buf[0] >> 6) & 0x03) == CLASS_CONTEXT_SPECIFIC) ==> (*eaten == 3);
	ensures ((((int)buf[0] >> 5) & 0x01) == 0) ==> (*eaten == 3);
	ensures (((int)buf[0] & 0x1f) == (int)tag) ==> (*eaten == 3);
	ensures ((int)buf[1] == 0x01) ==> (*eaten == 3);
	ensures ((int)buf[2] == 0x00 || (int)buf[2] == 0xff) ==> (*eaten == 3);
*/
static int parse_context_specific_boolean(u8 const *buf, u32 len, u8 tag,
u32 *eaten)
{
	u8 c;
	u8 p;
	u8 t;
	int ret;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (5 * 100000 + 1341);
			goto out;
		}
	}
	if (len < (u32)3) {
		ret = - (5 * 100000 + 1347);
		goto out;
	}
/*@ assert rte_703: mem_access: \valid_read(buf + 0); */
	c = (u8)(((int)*(buf + 0) >> 6) & 0x03);
/*@ assert rte_704: mem_access: \valid_read(buf + 0); */
	p = (u8)(((int)*(buf + 0) >> 5) & 0x01);
/*@ assert rte_705: mem_access: \valid_read(buf + 0); */
	t = (u8)((int)*(buf + 0) & 0x1f);
	if ((int)c != CLASS_CONTEXT_SPECIFIC) goto _LOR_0;
	else
	if ((int)p != 0) goto _LOR_0;
	else
	if ((int)t != (int)tag) {
		_LOR_0: {
			ret = - (5 * 100000 + 1357);
			goto out;
		}
	}
/*@ assert rte_706: mem_access: \valid_read(buf + 1); */
	if ((int)*(buf + 1) != 0x01) {
		ret = - (5 * 100000 + 1363);
		goto out;
	}
/*@ assert rte_707: mem_access: \valid_read(buf + 2); */
	switch ((int)*(buf + 2)) {
		case 0x00: case 0xff: break;
		default: ret = - (5 * 100000 + 1373);
		goto out;
		break;
	}
/*@ assert rte_708: mem_access: \valid(eaten); */
	*eaten = (u32)3;
	ret = 0;
	out: ;
	return ret;
}

static int parse_crl_ext_IDP(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
u32 len, int critical)
{
	u32 remain;
	int onlyContainsUserCerts;
	int onlyContainsCACerts;
	int indirectCRL;
	int onlyContainsAttributeCerts;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 eaten = (u32)0;
	int has_dp = 0;
	u8 const *buf = crl + off;
	u8 dpn_type = (u8)0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) {
		_LOR: {
			ret = - (5 * 100000 + 1428);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	remain -= hdr_len;
	buf += hdr_len;
	if (data_len == (u32)0) {
		ret = - (5 * 100000 + 1469);
		goto out;
	}
	if (remain != data_len) {
		ret = - (5 * 100000 + 1476);
		goto out;
	}
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& hdr_len,
	& data_len);
	if (! ret) {
		u32 dpn_remain = (u32)0;
		u32 dpn_eaten = (u32)0;
		buf += hdr_len;
		remain -= hdr_len;
		dpn_remain = data_len;
		if (data_len == (u32)0) {
			ret = - (5 * 100000 + 1495);
			goto out;
		}
	/*@ assert rte_709: mem_access: \valid_read(buf + 0); */
		dpn_type = *(buf + 0);
		switch ((int)dpn_type) {
			case 0xa0:
			ret = parse_GeneralNames(buf,dpn_remain,CLASS_CONTEXT_SPECIFIC,(u32)0,
			& dpn_eaten);
			if (ret) goto out;
			dpn_remain -= dpn_eaten;
			buf += dpn_eaten;
			break;
			case 0xa1: ret = - (5 * 100000 + 1528);
			goto out;
			break;
			default: ret = - (5 * 100000 + 1533);
			goto out;
			break;
		}
		if (dpn_remain) {
			ret = - (5 * 100000 + 1539);
			goto out;
		}
		has_dp = 1;
		remain -= data_len;
	}
	onlyContainsUserCerts = 0;
	onlyContainsCACerts = 0;
	indirectCRL = 0;
	onlyContainsAttributeCerts = 0;
	ret = parse_context_specific_boolean(buf,remain,(u8)0x01,& eaten);
	if (! ret) {
	/*@ assert rte_710: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) == 0x00) {
			ret = - (5 * 100000 + 1568);
			goto out;
		}
		onlyContainsUserCerts = 1;
		remain -= eaten;
		buf += eaten;
	}
	ret = parse_context_specific_boolean(buf,remain,(u8)0x02,& eaten);
	if (! ret) {
	/*@ assert rte_711: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) == 0x00) {
			ret = - (5 * 100000 + 1591);
			goto out;
		}
		onlyContainsCACerts = 1;
		remain -= eaten;
		buf += eaten;
	}
	ret = parse_crldp_reasons(buf,remain,(u32)0x03,& eaten);
	if (! ret) {
		buf += eaten;
		remain -= eaten;
	}
	ret = parse_context_specific_boolean(buf,remain,(u8)0x04,& eaten);
	if (! ret) {
	/*@ assert rte_712: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) == 0x00) {
			ret = - (5 * 100000 + 1624);
			goto out;
		}
		indirectCRL = 1;
		remain -= eaten;
		buf += eaten;
	}
	ret = parse_context_specific_boolean(buf,remain,(u8)0x05,& eaten);
	if (! ret) {
	/*@ assert rte_713: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) == 0x00) {
			ret = - (5 * 100000 + 1647);
			goto out;
		}
		onlyContainsAttributeCerts = 1;
		remain -= eaten;
		buf += eaten;
	}
/*@ assert rte_714: signed_overflow: -2147483648 <= onlyContainsUserCerts + onlyContainsCACerts; */
/*@ assert rte_715: signed_overflow: onlyContainsUserCerts + onlyContainsCACerts <= 2147483647; */
/*@ assert rte_716: signed_overflow: -2147483648 <= (int)(onlyContainsUserCerts + onlyContainsCACerts) + onlyContainsAttributeCerts; */
/*@ assert rte_717: signed_overflow: (int)(onlyContainsUserCerts + onlyContainsCACerts) + onlyContainsAttributeCerts <= 2147483647; */
	if ((onlyContainsUserCerts + onlyContainsCACerts) + onlyContainsAttributeCerts > 1) {
		ret = - (5 * 100000 + 1664);
		goto out;
	}
	if (remain) {
		ret = - (5 * 100000 + 1670);
		goto out;
	}
	ret = 0;
	u8 tmp = dpn_type;
	int tmp_3 = has_dp;
	int tmp_5 = indirectCRL;
	out: ;
	return ret;
}

/*@
	requires \valid(ctx);
	requires \valid_read(crl + (0 .. off + len - 1));
	requires off <= UINT32_MAX - len;
	assigns \nothing;
	ensures \result == -501712;
	ensures \result < 0;
*/
static int parse_crl_ext_CRLDP(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
u32 len, int critical)
{
	int ret;
	ret = - (5 * 100000 + 1712);
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires \valid_read(crl + (0 .. off + len - 1)) || crl == \null;
	requires off + len <= UINT32_MAX;
	assigns \nothing;
	ensures (crl == \null) ==> (\result == -(5 * 100000 + 1748));
	ensures (crl != \null && ctx == \null) ==> (\result == -(5 * 100000 + 1748));
	ensures (crl != \null && ctx != \null && critical != 0) ==> (\result == -(5 * 100000 + 1754));
	ensures (crl != \null && ctx != \null && critical == 0) ==> (\result <= 0);
	ensures (crl != \null && ctx != \null && critical == 0) ==> (len < 17 ==> \result == -(3 * 100000 + 1711));
	ensures (crl != \null && ctx != \null && critical == 0) ==> (\result == 0 ==> len >= 17);
*/
static int parse_crl_ext_expCertsOnCRL(crl_parsing_ctx *ctx, u8 const *crl,
u32 off, u32 len, int critical)
{
	u32 eaten;
	u16 year;
	u8 month = (u8)0;
	u8 day = (u8)0;
	u8 hour = (u8)0;
	u8 min = (u8)0;
	u8 sec = (u8)0;
	u8 const *buf = crl + off;
	int ret = -1;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) {
		_LOR: {
			ret = - (5 * 100000 + 1748);
			goto out;
		}
	}
	if (critical) {
		ret = - (5 * 100000 + 1754);
		goto out;
	}
	ret = parse_generalizedTime(buf,len,& eaten,& year,& month,& day,& hour,
	& min,& sec);
	if (ret) goto out;
	out: ;
	return ret;
}

static int parse_crl_ext_FreshestCRL(crl_parsing_ctx *ctx, u8 const *crl,
u32 off, u32 len, int critical)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = crl + off;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) {
		_LOR: {
			ret = - (5 * 100000 + 1806);
			goto out;
		}
	}
	if (critical) {
		ret = - (5 * 100000 + 1816);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (5 * 100000 + 1834);
		goto out;
	}
/*@ assert rte_718: mem_access: \valid(&ctx->has_crldp); */
	ctx->has_crldp = 1;
/*@ assert rte_719: mem_access: \valid(&ctx->one_crldp_has_all_reasons); */
	ctx->one_crldp_has_all_reasons = 0;
	while (remain) {
		int crldp_has_all_reasons = 0;
		u32 eaten = (u32)0;
		ret = parse_DistributionPoint(buf,remain,& crldp_has_all_reasons,& eaten);
		if (ret) goto out;
		if (crldp_has_all_reasons)
	/*@ assert rte_720: mem_access: \valid(&ctx->one_crldp_has_all_reasons); */
		ctx->one_crldp_has_all_reasons = 1;
		remain -= eaten;
		buf += eaten;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_crl_ext_AIA(crl_parsing_ctx *ctx, u8 const *crl, u32 off,
u32 len, int critical)
{
	int ret;
	if (ctx == (crl_parsing_ctx *)0) {
		ret = - (5 * 100000 + 1894);
		goto out;
	}
	ret = parse_AIA(crl,off,len,critical);
	if (ret) {
		ret = - (5 * 100000 + 1901);
		goto out;
	}
	out: ;
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires crl == \null || \valid_read(crl + (0 .. off + len - 1));
	requires len == 0 || crl == \null || ctx == \null || \valid_read(crl + off + (0 .. len - 1));
	requires \separated(ctx, crl + (..));
	assigns \nothing;
	ensures (ctx == \null || crl == \null || len == 0) ==>\result == -(5 * 100000 + 1936);
	ensures (ctx != \null && crl != \null && len != 0) ==>(\result == 0 || \result < 0);
	ensures \result == 0 ==>(ctx != \null && crl != \null && len != 0);
	ensures (ctx != \null && crl != \null && len != 0 && \result != 0 &&\result != -(5 * 100000 + 1936)) ==>(\result == -(5 * 100000 + 1949) || \result < 0);
*/
static int parse_crl_ext_szOID_CRL_NEXT_PUBLISH(crl_parsing_ctx *ctx,
u8 const *crl, u32 off,
u32 len, int critical)
{
	u8 t_month = (u8)0;
	u8 t_day = (u8)0;
	u8 t_hour = (u8)0;
	u8 t_min = (u8)0;
	u8 t_sec = (u8)0;
	u8 const *buf = crl + off;
	u32 t_len = (u32)0;
	u16 t_year = (u16)0;
	u8 t_type = (u8)0;
	int ret = -1;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (5 * 100000 + 1936);
			goto out;
		}
	}
	ret = parse_Time(buf,len,& t_type,& t_len,& t_year,& t_month,& t_day,
	& t_hour,& t_min,& t_sec);
	if (ret) goto out;
	if (t_len != len) {
		ret = - (5 * 100000 + 1949);
		goto out;
	}
	out: ;
	return ret;
}

static int parse_crl_ext_szOID_CERTSRV_CA_VERSION(crl_parsing_ctx *ctx,
u8 const *crl, u32 off,
u32 len, int critical)
{
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = crl + off;
	int ret = -1;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (5 * 100000 + 1985);
			goto out;
		}
	}
	ret = parse_non_negative_integer(buf,len,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	if (data_len > (u32)4) {
		ret = - (5 * 100000 + 2000);
		goto out;
	}
	parsed = hdr_len + data_len;
	if (parsed != len) {
		ret = - (5 * 100000 + 2007);
		goto out;
	}
	out: ;
	return ret;
}

/*@
	requires \valid(ctx);
	requires (len > 0) ==> \valid_read(crl + (0 .. off + len - 1));
	requires off <= 0xFFFFFFFF - len;
	assigns \nothing;
	ensures \result == 0;
*/
static int parse_crl_ext_szOID_CRL_SELF_CDP(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
int critical)
{
	int __retres;
	__retres = 0;
	return __retres;
}

static u8 const _crl_ext_oid_AKI[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x23};

static u8 const _crl_ext_oid_IAN[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x12};

static u8 const _crl_ext_oid_CRLnum[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x14};

static u8 const _crl_ext_oid_DeltaCRL[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x1b};

static u8 const _crl_ext_oid_IDP[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x1c};

static u8 const _crl_ext_oid_CRLDP[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x1f};

static u8 const _crl_ext_oid_FreshestCRL[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x2e};

static u8 const _crl_ext_oid_AIA[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x01,
(u8)0x01};

static u8 const _crl_ext_oid_expCertsOnCRL[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x3c};

static u8 const _crl_ext_oid_szOID_CRL_NEXT_PUBLISH[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x04};

static u8 const _crl_ext_oid_szOID_CERTSRV_CA_VERSION[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x01};

static u8 const _crl_ext_oid_szOID_CRL_SELF_CDP[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x0e};

static _crl_ext_oid const known_crl_ext_oids[12] =
{{.oid = _crl_ext_oid_AKI,
		.oid_len = (u8)sizeof(_crl_ext_oid_AKI),
	.parse_crl_ext_params = & parse_crl_ext_AKI},
	{.oid = _crl_ext_oid_IAN,
		.oid_len = (u8)sizeof(_crl_ext_oid_IAN),
	.parse_crl_ext_params = & parse_crl_ext_IAN},
	{.oid = _crl_ext_oid_CRLnum,
		.oid_len = (u8)sizeof(_crl_ext_oid_CRLnum),
	.parse_crl_ext_params = & parse_crl_ext_CRLnum},
	{.oid = _crl_ext_oid_DeltaCRL,
		.oid_len = (u8)sizeof(_crl_ext_oid_DeltaCRL),
	.parse_crl_ext_params = & parse_crl_ext_DeltaCRL_indicator},
	{.oid = _crl_ext_oid_IDP,
		.oid_len = (u8)sizeof(_crl_ext_oid_IDP),
	.parse_crl_ext_params = & parse_crl_ext_IDP},
	{.oid = _crl_ext_oid_CRLDP,
		.oid_len = (u8)sizeof(_crl_ext_oid_CRLDP),
	.parse_crl_ext_params = & parse_crl_ext_CRLDP},
	{.oid = _crl_ext_oid_FreshestCRL,
		.oid_len = (u8)sizeof(_crl_ext_oid_FreshestCRL),
	.parse_crl_ext_params = & parse_crl_ext_FreshestCRL},
	{.oid = _crl_ext_oid_AIA,
		.oid_len = (u8)sizeof(_crl_ext_oid_AIA),
	.parse_crl_ext_params = & parse_crl_ext_AIA},
	{.oid = _crl_ext_oid_expCertsOnCRL,
		.oid_len = (u8)sizeof(_crl_ext_oid_expCertsOnCRL),
	.parse_crl_ext_params = & parse_crl_ext_expCertsOnCRL},
	{.oid = _crl_ext_oid_szOID_CRL_NEXT_PUBLISH,
		.oid_len = (u8)sizeof(_crl_ext_oid_szOID_CRL_NEXT_PUBLISH),
	.parse_crl_ext_params = & parse_crl_ext_szOID_CRL_NEXT_PUBLISH},
	{.oid = _crl_ext_oid_szOID_CERTSRV_CA_VERSION,
		.oid_len = (u8)sizeof(_crl_ext_oid_szOID_CERTSRV_CA_VERSION),
	.parse_crl_ext_params = & parse_crl_ext_szOID_CERTSRV_CA_VERSION},
	{.oid = _crl_ext_oid_szOID_CRL_SELF_CDP,
		.oid_len = (u8)sizeof(_crl_ext_oid_szOID_CRL_SELF_CDP),
.parse_crl_ext_params = & parse_crl_ext_szOID_CRL_SELF_CDP}};

/*@
	requires buf == \null || \valid_read(buf + (0 .. len - 1));
	assigns \nothing;
	ensures buf == \null ==> \result == \null;
	ensures len == 0 ==> \result == \null;
	ensures buf != \null && len != 0 ==>(\result == \null ||(\exists integer i; 0 <= i < 12 && \result == &known_crl_ext_oids[i]));
	ensures \result != \null ==>((u32)\result->oid_len == len &&bufs_differ(\result->oid, buf, len) == 0);
	ensures \result == \null && buf != \null && len != 0 ==>(\forall integer i; 0 <= i < 12 ==>((u32)known_crl_ext_oids[i].oid_len != len ||bufs_differ(known_crl_ext_oids[i].oid, buf, (u32)known_crl_ext_oids[i].oid_len) != 0));
*/
static _crl_ext_oid const *find_crl_ext_by_oid(u8 const *buf, u32 len)
{
	u16 k;
	_crl_ext_oid const *found = (_crl_ext_oid const *)0;
	_crl_ext_oid const *cur = (_crl_ext_oid const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u16)0;
	/*@
		loop invariant 0 <= k <= sizeof(known_crl_ext_oids)/sizeof(known_crl_ext_oids[0]);
		loop invariant found == (_crl_ext_oid const *)0;
		loop invariant \forall integer i; 0 <= i < k ==>((u32)known_crl_ext_oids[i].oid_len != len ||bufs_differ(known_crl_ext_oids[i].oid, buf, (u32)known_crl_ext_oids[i].oid_len) != 0);
		loop assigns k, cur;
		loop variant sizeof(known_crl_ext_oids)/sizeof(known_crl_ext_oids[0]) - k;
	*/
	while ((unsigned long)k < sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) {
		{
			int ret;
			int tmp_0;
			int tmp;
			cur = & known_crl_ext_oids[k];
		/*@ assert rte_679: mem_access: \valid_read(&cur->oid_len); */
			if ((u32)cur->oid_len != len) goto __Cont;
		/*@ assert rte_680: mem_access: \valid_read(&cur->oid); */
		/*@ assert rte_681: mem_access: \valid_read(&cur->oid_len); */
			tmp = bufs_differ(cur->oid,buf,(u32)cur->oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_682: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u16)((int)k + 1);
	}
	out: ;
	return found;
}

/*@
	requires \valid_read(ext);
	requires \valid(parsed_oid_list + (0 .. (sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) - 1));
	assigns parsed_oid_list[0 .. 11];
	ensures \result == 0 || \result == -502225 || \result == -502237;
	ensures \result == 0 ==>(\exists integer i; 0 <= i < 12 &&parsed_oid_list[i] == ext &&(\forall integer j; 0 <= j < i ==>\old(parsed_oid_list[j]) != (_crl_ext_oid const *)0 &&\old(parsed_oid_list[j]) != ext));
	ensures \result == -502225 ==>(\exists integer i; 0 <= i < 12 &&\old(parsed_oid_list[i]) == ext &&(\forall integer j; 0 <= j < i ==>\old(parsed_oid_list[j]) != (_crl_ext_oid const *)0));
	ensures \result == -502237 ==>(\forall integer i; 0 <= i < 12 ==>\old(parsed_oid_list[i]) != (_crl_ext_oid const *)0 &&\old(parsed_oid_list[i]) != ext);
	ensures \result == 0 ==>(\forall integer i; 0 <= i < 12 ==>(parsed_oid_list[i] == ext || parsed_oid_list[i] == \old(parsed_oid_list[i])));
	ensures \result != 0 ==>(\forall integer i; 0 <= i < 12 ==> parsed_oid_list[i] == \old(parsed_oid_list[i]));
*/
static int check_record_crl_ext_unknown(_crl_ext_oid const *ext,
_crl_ext_oid const **parsed_oid_list)
{
	int ret;
	u16 pos = (u16)0;
	/*@
		loop invariant 0 <= pos <= sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0]);
		loop invariant \forall integer j; 0 <= j < pos ==>parsed_oid_list[j] != (_crl_ext_oid const *)0 &&parsed_oid_list[j] != ext;
		loop assigns pos, parsed_oid_list[0 .. (sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) - 1];
		loop variant (sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) - pos;
	*/
	while ((unsigned long)pos < sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) {
	/*@ assert rte_683: mem_access: \valid_read(parsed_oid_list + (int)pos); */
		if (*(parsed_oid_list + (int)pos) == (_crl_ext_oid const *)0) {
		/*@ assert rte_684: mem_access: \valid(parsed_oid_list + (int)pos); */
			*(parsed_oid_list + (int)pos) = ext;
			break;
		}
	/*@ assert rte_685: mem_access: \valid_read(parsed_oid_list + (int)pos); */
		if (ext == *(parsed_oid_list + (int)pos)) {
			ret = - (5 * 100000 + 2225);
			goto out;
		}
	/*@ assert rte_686: signed_overflow: (int)pos + 1 <= 2147483647; */
		pos = (u16)((int)pos + 1);
	}
	if ((unsigned long)pos >= sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) {
		ret = - (5 * 100000 + 2237);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_Extension(crl_parsing_ctx *ctx, u8 const *crl,
u32 off, u32 len,
_crl_ext_oid const **parsed_oid_list,
u32 *eaten)
{
	u32 remain;
	int ret;
	u32 ext_hdr_len = (u32)0;
	u32 ext_data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 saved_ext_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 parsed = (u32)0;
	u8 const *buf = crl + off;
	_crl_ext_oid const *ext = (_crl_ext_oid const *)0;
	int critical = 0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) goto _LOR;
	else
	if (parsed_oid_list == (_crl_ext_oid const **)0) {
		_LOR: {
			ret = - (5 * 100000 + 2282);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& ext_hdr_len,& ext_data_len);
	if (ret) goto out;
	buf += ext_hdr_len;
	off += ext_hdr_len;
	remain -= ext_hdr_len;
	saved_ext_len = ext_hdr_len + ext_data_len;
	ret = parse_OID(buf,ext_data_len,& oid_len);
	if (ret) goto out;
	ext = find_crl_ext_by_oid(buf,oid_len);
	if (ext == (_crl_ext_oid const *)0) {
		ret = - (5 * 100000 + 2321);
		goto out;
	}
	ret = check_record_crl_ext_unknown(ext,parsed_oid_list);
	if (ret) goto out;
	buf += oid_len;
	off += oid_len;
	ext_data_len -= oid_len;
	ret = parse_boolean(buf,ext_data_len,& parsed);
	if (ret) {
		if (ext_data_len)
	/*@ assert rte_721: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) == ASN1_TYPE_BOOLEAN) {
			ret = - (5 * 100000 + 2375);
			goto out;
		}
	}
	else {
		if (parsed != (u32)3) {
			ret = - (5 * 100000 + 2387);
			goto out;
		}
	/*@ assert rte_722: mem_access: \valid_read(buf + 2); */
		if ((int)*(buf + 2) == 0) critical = 0; else critical = 1;
		buf += parsed;
		off += parsed;
		ext_data_len -= parsed;
	}
	ret = parse_id_len(buf,ext_data_len,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_OCTET_STRING,& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	ext_data_len -= hdr_len;
	if (data_len != ext_data_len) {
		ret = - (5 * 100000 + 2429);
		goto out;
	}
	if (ext == known_crl_ext_oids) ret = parse_crl_ext_AKI(ctx,crl,off,
	ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[1]) ret = parse_crl_ext_IAN(ctx,crl,off,
	ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[2]) ret = parse_crl_ext_CRLnum(ctx,crl,
	off,
	ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[3]) ret = parse_crl_ext_DeltaCRL_indicator
	(ctx,crl,off,ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[4]) ret = parse_crl_ext_IDP
	(ctx,crl,off,ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[5]) ret = parse_crl_ext_CRLDP
	(ctx,crl,off,ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[6]) ret = parse_crl_ext_FreshestCRL
	(ctx,crl,off,ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[7]) ret = parse_crl_ext_AIA
	(ctx,crl,off,
	ext_data_len,critical);
	else
	if (ext == & known_crl_ext_oids[8]) ret = parse_crl_ext_expCertsOnCRL
	(ctx,crl,off,
	ext_data_len,critical);
	else
	if (ext == & known_crl_ext_oids[9]) ret = parse_crl_ext_szOID_CRL_NEXT_PUBLISH
	(ctx,crl,off,
	ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[10]) ret = parse_crl_ext_szOID_CERTSRV_CA_VERSION
	(ctx,crl,off,
	ext_data_len,
	critical);
	else
	if (ext == & known_crl_ext_oids[11]) ret = parse_crl_ext_szOID_CRL_SELF_CDP
	(ctx,crl,off,
	ext_data_len,
	critical);
	if (ret) goto out;
/*@ assert rte_723: mem_access: \valid(eaten); */
	*eaten = saved_ext_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_Extensions(crl_parsing_ctx *ctx, u8 const *crl,
u32 off, u32 len, u32 *eaten)
{
	_crl_ext_oid const *parsed_crl_ext_oid_list[sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])];
	int ret;
	u16 i;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u8 const *buf = crl + off;
	u32 saved_len = (u32)0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 2505);
			goto out;
		}
	}
	ret = parse_explicit_id_len(buf,len,(u32)0,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_SEQUENCE,& hdr_len,& data_len);
	if (ret) goto out;
	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	saved_len = hdr_len + data_len;
	if (data_len == (u32)0) {
		ret = - (5 * 100000 + 2538);
		goto out;
	}
	i = (u16)0;
	while ((unsigned long)i < sizeof(known_crl_ext_oids) / sizeof(known_crl_ext_oids[0])) {
	/*@ assert rte_724: index_bound: i < (unsigned long)(sizeof(_crl_ext_oid const [12]) / sizeof(_crl_ext_oid)); */
		parsed_crl_ext_oid_list[i] = (_crl_ext_oid const *)0;
	/*@ assert rte_725: signed_overflow: (int)i + 1 <= 2147483647; */
		i = (u16)((int)i + 1);
	}
	while (remain) {
		u32 ext_len = (u32)0;
		ret = parse_x509_crl_Extension(ctx,crl,off,remain,
		parsed_crl_ext_oid_list,& ext_len);
		if (ret) goto out;
		remain -= ext_len;
		buf += ext_len;
		off += ext_len;
	}
/*@ assert rte_726: mem_access: \valid(eaten); */
	*eaten = saved_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_Version(u8 const *cert, u32 off, u32 len,
u8 *version, u32 *eaten)
{
	int ret;
	u8 const *buf = cert + off;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (version == (u8 *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 2638);
			goto out;
		}
	}
	ret = parse_integer(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_INTEGER,
	& hdr_len,& data_len);
	if (ret) {
		ret = X509_PARSER_ERROR_VERSION_ABSENT;
		goto out;
	}
	buf += hdr_len;
	if (data_len != (u32)1) {
		ret = X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH;
		goto out;
	}
/*@ assert rte_636: mem_access: \valid(version); */
/*@ assert rte_637: mem_access: \valid_read(buf + 0); */
	*version = *(buf + 0);
/*@ assert rte_638: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_tbsCertList_sig_AlgorithmIdentifier(crl_parsing_ctx *ctx,
u8 const *crl,
u32 off, u32 len,
_sig_alg const **alg,
u32 *eaten)
{
	u32 param_len;
	int ret;
	_sig_alg const *talg = (_sig_alg const *)0;
	u8 const *buf = crl + off;
	u32 saved_off = off;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (5 * 100000 + 2710);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
	buf += hdr_len;
	off += hdr_len;
	ret = parse_OID(buf,data_len,& oid_len);
	if (ret) goto out;
	talg = find_sig_alg_by_oid(buf,oid_len);
	if (talg == (_sig_alg const *)0) {
		ret = - (5 * 100000 + 2738);
		goto out;
	}
/*@ assert rte_639: mem_access: \valid(&ctx->tbs_sig_alg_oid_start); */
	ctx->tbs_sig_alg_oid_start = off;
/*@ assert rte_640: mem_access: \valid(&ctx->tbs_sig_alg_oid_len); */
	ctx->tbs_sig_alg_oid_len = oid_len;
/*@ assert rte_641: mem_access: \valid(&ctx->sig_alg); */
/*@ assert rte_642: mem_access: \valid_read(&talg->sig_id); */
	ctx->sig_alg = talg->sig_id;
/*@ assert rte_643: mem_access: \valid(&ctx->hash_alg); */
/*@ assert rte_644: mem_access: \valid_read(&talg->hash_id); */
	ctx->hash_alg = talg->hash_id;
	buf += oid_len;
	off += oid_len;
	param_len = data_len - oid_len;
/*@ assert rte_645: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_ecdsa_with)
	ret = parse_algoid_sig_params_ecdsa_with(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,off,
	param_len);
	else
/*@ assert rte_646: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_ecdsa_with_specified)
	ret = parse_algoid_sig_params_ecdsa_with_specified(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,
	off,param_len);
	else
/*@ assert rte_647: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_sm2)
	ret = parse_algoid_sig_params_sm2(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,off,param_len);
	else
/*@ assert rte_648: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_eddsa)
	ret = parse_algoid_sig_params_eddsa(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,off,
	param_len);
	else
/*@ assert rte_649: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_rsa)
	ret = parse_algoid_sig_params_rsa(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,off,
	param_len);
	else
/*@ assert rte_650: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_rsassa_pss)
	ret = parse_algoid_sig_params_rsassa_pss(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,
	off,param_len);
	else
/*@ assert rte_651: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_none)
	ret = parse_algoid_sig_params_none(& ctx->sig_alg_params,
	& ctx->hash_alg,crl,off,
	param_len);
	else
/*@ assert rte_652: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_bign_with_hspec)
	ret = parse_algoid_sig_params_bign_with_hspec(& ctx->sig_alg_params,
	& ctx->hash_alg,
	crl,off,
	param_len);
	if (ret) goto out;
/*@ assert rte_653: mem_access: \valid(alg); */
	*alg = talg;
/*@ assert rte_654: mem_access: \valid(eaten); */
	*eaten = parsed;
/*@ assert rte_655: mem_access: \valid(&ctx->tbs_sig_alg_start); */
	ctx->tbs_sig_alg_start = saved_off;
/*@ assert rte_656: mem_access: \valid(&ctx->tbs_sig_alg_len); */
	ctx->tbs_sig_alg_len = parsed;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_revokedCertificates(crl_parsing_ctx *ctx,
u8 const *crl, u32 off,
u32 len, u32 *eaten)
{
	int ret;
	u32 remain = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 parsed = (u32)0;
	u32 cur_off = (u32)0;
	u8 const *buf = crl + off;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 2831);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) {
	/*@ assert rte_675: mem_access: \valid(&ctx->has_revoked_certs); */
		ctx->has_revoked_certs = 0;
	/*@ assert rte_676: mem_access: \valid(eaten); */
		*eaten = (u32)0;
		ret = 0;
		goto out;
	}
	remain = data_len;
	cur_off = off + hdr_len;
	while (remain) {
		ret = parse_x509_crl_revokedCertificate(ctx,crl,cur_off,remain,& parsed);
		if (ret) goto out;
		remain -= parsed;
		cur_off += parsed;
	}
/*@ assert rte_677: mem_access: \valid(&ctx->has_revoked_certs); */
	ctx->has_revoked_certs = data_len != (u32)0;
/*@ assert rte_678: mem_access: \valid(eaten); */
	*eaten = data_len + hdr_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_TBSCertList(crl_parsing_ctx *ctx, u8 const *crl,
u32 off, u32 len, _sig_alg const **sig_alg,
u32 *eaten)
{
	int ret;
	u64 thisUpdate;
	u64 nextUpdate;
	u32 tbs_data_len = (u32)0;
	u32 tbs_hdr_len = (u32)0;
	u32 tbs_crl_len = (u32)0;
	u32 remain = (u32)0;
	u32 parsed = (u32)0;
	u32 cur_off = off;
	u8 const *buf = crl + cur_off;
	_sig_alg const *alg = (_sig_alg const *)0;
	int empty_issuer = 1;
	u32 tu_len = (u32)0;
	u32 nu_len = (u32)0;
	u16 nu_year = (u16)0;
	u16 tu_year = (u16)0;
	u8 nu_month = (u8)0;
	u8 nu_day = (u8)0;
	u8 nu_hour = (u8)0;
	u8 nu_min = (u8)0;
	u8 nu_sec = (u8)0;
	u8 tu_month = (u8)0;
	u8 tu_day = (u8)0;
	u8 tu_hour = (u8)0;
	u8 tu_min = (u8)0;
	u8 tu_sec = (u8)0;
	u8 t_type = (u8)0;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 2971);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& tbs_hdr_len,& tbs_data_len);
	if (ret) goto out;
	tbs_crl_len = tbs_hdr_len + tbs_data_len;
	buf += tbs_hdr_len;
	cur_off += tbs_hdr_len;
	remain = tbs_data_len;
	ret = parse_x509_crl_Version(crl,cur_off,remain,& ctx->version,& parsed);
	if (ret)
/*@ assert rte_727: mem_access: \valid(&ctx->version); */
	ctx->version = (u8)0x00;
	else {
	/*@ assert rte_728: mem_access: \valid_read(&ctx->version); */
		if ((int)ctx->version != 0x01) {
			ret = - (5 * 100000 + 3015);
			goto out;
		}
		buf += parsed;
		cur_off += parsed;
		remain -= parsed;
	}
	ret = parse_x509_tbsCertList_sig_AlgorithmIdentifier(ctx,crl,cur_off,
	remain,& alg,& parsed);
	if (ret) goto out;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_x509_Name(buf,remain,& parsed,& empty_issuer);
	if (ret) goto out;
/*@ assert rte_729: mem_access: \valid(&ctx->issuer_start); */
	ctx->issuer_start = cur_off;
/*@ assert rte_730: mem_access: \valid(&ctx->issuer_len); */
	ctx->issuer_len = parsed;
	if (empty_issuer) {
		ret = - (5 * 100000 + 3058);
		goto out;
	}
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_Time(buf,remain,& t_type,& tu_len,& tu_year,& tu_month,
	& tu_day,& tu_hour,& tu_min,& tu_sec);
	if (ret) goto out;
	ret = verify_correct_time_use(t_type,tu_year);
	if (ret) goto out;
	remain -= tu_len;
	cur_off += tu_len;
	buf += tu_len;
	thisUpdate = time_components_to_comparable_u64(tu_year,tu_month,tu_day,
	tu_hour,tu_min,tu_sec);
	ret = parse_Time(buf,remain,& t_type,& nu_len,& nu_year,& nu_month,
	& nu_day,& nu_hour,& nu_min,& nu_sec);
	if (! ret) {
		ret = verify_correct_time_use(t_type,nu_year);
		if (ret) goto out;
		remain -= nu_len;
		cur_off += nu_len;
		buf += nu_len;
	}
	else {
		nu_year = (u16)9999;
		nu_month = (u8)12;
		nu_day = (u8)31;
		nu_hour = (u8)23;
		nu_min = (u8)59;
		nu_sec = (u8)59;
	}
	nextUpdate = time_components_to_comparable_u64(nu_year,nu_month,nu_day,
	nu_hour,nu_min,nu_sec);
	if (thisUpdate >= nextUpdate) {
		ret = - (5 * 100000 + 3151);
		goto out;
	}
	if (remain) {
		ret = parse_x509_crl_revokedCertificates(ctx,crl,cur_off,remain,& parsed);
		if (ret) goto out;
		cur_off += parsed;
		remain -= parsed;
	}
/*@ assert rte_731: mem_access: \valid_read(&ctx->version); */
	if ((int)ctx->version != 0x01)
	if (remain) {
		ret = - (5 * 100000 + 3178);
		goto out;
	}
	if (remain) {
		ret = parse_x509_crl_Extensions(ctx,crl,cur_off,remain,& parsed);
		if (ret) goto out;
		buf += parsed;
		cur_off += parsed;
		remain -= parsed;
	}
	if (remain != (u32)0) {
		ret = - (5 * 100000 + 3203);
		goto out;
	}
/*@ assert rte_732: mem_access: \valid(eaten); */
	*eaten = tbs_crl_len;
/*@ assert rte_733: mem_access: \valid(sig_alg); */
	*sig_alg = alg;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(eaten) || eaten == \null;
	requires \valid(ctx) || ctx == \null;
	requires (crl == \null) || \valid_read(crl + (0 .. len-1));
	requires ctx != \null ==> \valid_read(&ctx->tbs_sig_alg_len);
	requires ctx != \null ==> \valid_read(&ctx->tbs_sig_alg_start);
	requires ctx != \null ==> \valid(&ctx->sig_alg_start);
	requires ctx != \null ==> \valid(&ctx->sig_alg_len);
	requires (ctx != \null && crl != \null && len > 0) ==>(ctx->tbs_sig_alg_start + ctx->tbs_sig_alg_len <= len);
	requires (ctx != \null && crl != \null && len > 0) ==>(off + ctx->tbs_sig_alg_len <= len || ctx->tbs_sig_alg_len > len);
	assigns ctx->sig_alg_start, ctx->sig_alg_len, *eaten;
	ensures \result == 0 ||\result == -(5 * 100000 + 3254) ||\result == -(5 * 100000 + 3261) ||\result == -(5 * 100000 + 3274);
	ensures (crl == \null || len == 0 || ctx == \null || eaten == \null) ==>\result == -(5 * 100000 + 3254);
	ensures (crl != \null && len != 0 && ctx != \null && eaten != \null &&ctx->tbs_sig_alg_len > len) ==>\result == -(5 * 100000 + 3261);
	ensures (crl != \null && len != 0 && ctx != \null && eaten != \null &&ctx->tbs_sig_alg_len <= len &&(\exists integer k; 0 <= k < ctx->tbs_sig_alg_len &&crl[ctx->tbs_sig_alg_start + k] != crl[off + k])) ==>\result == -(5 * 100000 + 3274);
	ensures (crl != \null && len != 0 && ctx != \null && eaten != \null &&ctx->tbs_sig_alg_len <= len &&(\forall integer k; 0 <= k < ctx->tbs_sig_alg_len ==>crl[ctx->tbs_sig_alg_start + k] == crl[off + k])) ==>(\result == 0 &&ctx->sig_alg_start == off &&ctx->sig_alg_len == \old(ctx->tbs_sig_alg_len) &&eaten == \old(ctx->tbs_sig_alg_len));
*/
static int parse_x509_crl_signatureAlgorithm(crl_parsing_ctx *ctx,
u8 const *crl, u32 off, u32 len,
u32 *eaten)
{
	u32 prev_len;
	int ret;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 3254);
			goto out;
		}
	}
/*@ assert rte_734: mem_access: \valid_read(&ctx->tbs_sig_alg_len); */
	prev_len = ctx->tbs_sig_alg_len;
	if (prev_len > len) {
		ret = - (5 * 100000 + 3261);
		goto out;
	}
/*@ assert rte_735: mem_access: \valid_read(&ctx->tbs_sig_alg_start); */
	ret = bufs_differ(crl + ctx->tbs_sig_alg_start,crl + off,prev_len);
	if (ret) {
		ret = - (5 * 100000 + 3274);
		goto out;
	}
/*@ assert rte_736: mem_access: \valid(&ctx->sig_alg_start); */
	ctx->sig_alg_start = off;
/*@ assert rte_737: mem_access: \valid(&ctx->sig_alg_len); */
	ctx->sig_alg_len = prev_len;
/*@ assert rte_738: mem_access: \valid(eaten); */
	*eaten = prev_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_crl_signatureValue(crl_parsing_ctx *ctx, u8 const *crl,
u32 off, u32 len,
_sig_alg const *sig_alg, u32 *eaten)
{
	int ret;
	u32 saved_off = off;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (sig_alg == (_sig_alg const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (5 * 100000 + 3322);
			goto out;
		}
	}
/*@ assert rte_739: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == (int (*)(sig_params *params, u8 const *cert,
	u32 off, u32 len, u32 *eaten))0) {
		ret = - (5 * 100000 + 3328);
		goto out;
	}
/*@ assert rte_740: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_ed448) ret = parse_sig_ed448(& ctx->sig_alg_params,
	crl,off,
	len,
	eaten);
	else
/*@ assert rte_741: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_ed25519) ret = parse_sig_ed25519
	(& ctx->sig_alg_params,
	crl,off,len,eaten);
	else
/*@ assert rte_742: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_ecdsa) ret = parse_sig_ecdsa
	(& ctx->sig_alg_params,
	crl,off,len,eaten);
	else
/*@ assert rte_743: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_sm2) ret = parse_sig_sm2
	(& ctx->sig_alg_params,
	crl,off,len,eaten);
	else
/*@ assert rte_744: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_dsa) ret = parse_sig_dsa
	(& ctx->sig_alg_params,
	crl,off,len,eaten);
	else
/*@ assert rte_745: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_pkcs1_v15) ret = parse_sig_rsa_pkcs1_v15
	(& ctx->sig_alg_params,
	crl,off,
	len,eaten);
	else
/*@ assert rte_746: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_ssa_pss) ret = parse_sig_rsa_ssa_pss
	(& ctx->sig_alg_params,
	crl,off,
	len,eaten);
	else
/*@ assert rte_747: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_9796_2_pad)
	ret = parse_sig_rsa_9796_2_pad(& ctx->sig_alg_params,crl,
	off,len,eaten);
	else
/*@ assert rte_748: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_belgian)
	ret = parse_sig_rsa_belgian(& ctx->sig_alg_params,crl,
	off,len,eaten);
	else
/*@ assert rte_749: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost94) ret = parse_sig_gost94
	(& ctx->sig_alg_params,
	crl,off,
	len,eaten);
	else
/*@ assert rte_750: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost2001)
	ret = parse_sig_gost2001(& ctx->sig_alg_params,crl,
	off,len,eaten);
	else
/*@ assert rte_751: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost2012_512)
	ret = parse_sig_gost2012_512(& ctx->sig_alg_params,
	crl,off,len,eaten);
	else
/*@ assert rte_752: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost2012_256)
	ret = parse_sig_gost2012_256(& ctx->sig_alg_params,
	crl,off,len,eaten);
	else
/*@ assert rte_753: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_bign)
	ret = parse_sig_bign(& ctx->sig_alg_params,crl,
	off,len,eaten);
	else
/*@ assert rte_754: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_monkey)
	ret = parse_sig_monkey(& ctx->sig_alg_params,
	crl,off,len,eaten);
	if (ret) goto out;
/*@ assert rte_755: mem_access: \valid(&ctx->sig_start); */
	ctx->sig_start = saved_off;
/*@ assert rte_756: mem_access: \valid(&ctx->sig_len); */
/*@ assert rte_757: mem_access: \valid_read(eaten); */
	ctx->sig_len = *eaten;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result.tbs_start == 0;
	ensures \result.tbs_len == 0;
	ensures \result.version == 0;
	ensures \result.tbs_sig_alg_start == 0;
	ensures \result.tbs_sig_alg_len == 0;
	ensures \result.tbs_sig_alg_oid_start == 0;
	ensures \result.tbs_sig_alg_oid_len == 0;
	ensures \result.tbs_sig_alg_oid_params_start == 0;
	ensures \result.tbs_sig_alg_oid_params_len == 0;
	ensures \result.issuer_start == 0;
	ensures \result.issuer_len == 0;
	ensures \result.this_update == 0;
	ensures \result.next_update == 0;
	ensures \result.has_aki == 0;
	ensures \result.aki_has_keyIdentifier == 0;
	ensures \result.aki_keyIdentifier_start == 0;
	ensures \result.aki_keyIdentifier_len == 0;
	ensures \result.aki_has_generalNames_and_serial == 0;
	ensures \result.aki_generalNames_start == 0;
	ensures \result.aki_generalNames_len == 0;
	ensures \result.aki_serial_start == 0;
	ensures \result.aki_serial_len == 0;
	ensures \result.has_crldp == 0;
	ensures \result.one_crldp_has_all_reasons == 0;
	ensures \result.has_crlnumber == 0;
	ensures \result.crlnumber_start == 0;
	ensures \result.crlnumber_len == 0;
	ensures \result.has_revoked_certs == 0;
	ensures \result.sig_alg_start == 0;
	ensures \result.sig_alg_len == 0;
	ensures \result.sig_alg == (sig_alg_id)0;
	ensures \result.hash_alg == (hash_alg_id)0;
	ensures \result.sig_alg_params.dsa.r_raw_off == 0;
	ensures \result.sig_alg_params.dsa.r_raw_len == 0;
	ensures \result.sig_alg_params.dsa.s_raw_off == 0;
	ensures \result.sig_alg_params.dsa.s_raw_len == 0;
	ensures \result.sig_start == 0;
	ensures \result.sig_len == 0;
*/
static crl_parsing_ctx get_zeroized_crl_ctx_val(void)
{
	crl_parsing_ctx zeroized_ctx =
	{.tbs_start = (u32)0,
		.tbs_len = 0U,
		.version = (unsigned char)0,
		.tbs_sig_alg_start = 0U,
		.tbs_sig_alg_len = 0U,
		.tbs_sig_alg_oid_start = 0U,
		.tbs_sig_alg_oid_len = 0U,
		.tbs_sig_alg_oid_params_start = 0U,
		.tbs_sig_alg_oid_params_len = 0U,
		.issuer_start = 0U,
		.issuer_len = 0U,
		.this_update = 0UL,
		.next_update = 0UL,
		.has_aki = 0,
		.aki_has_keyIdentifier = 0,
		.aki_keyIdentifier_start = 0U,
		.aki_keyIdentifier_len = 0U,
		.aki_has_generalNames_and_serial = 0,
		.aki_generalNames_start = 0U,
		.aki_generalNames_len = 0U,
		.aki_serial_start = 0U,
		.aki_serial_len = 0U,
		.has_crldp = 0,
		.one_crldp_has_all_reasons = 0,
		.has_crlnumber = 0,
		.crlnumber_start = 0U,
		.crlnumber_len = 0U,
		.has_revoked_certs = 0,
		.sig_alg_start = 0U,
		.sig_alg_len = 0U,
		.sig_alg = (sig_alg_id)0,
		.hash_alg = (hash_alg_id)0,
		.sig_alg_params = {.dsa = {.r_raw_off = 0U,
				.r_raw_len = 0U,
				.s_raw_off = 0U,
		.s_raw_len = 0U}},
		.sig_start = 0U,
	.sig_len = 0U};
	return zeroized_ctx;
}

int parse_x509_crl(crl_parsing_ctx *ctx, u8 const *crl, u32 len)
{
	int ret;
	u32 seq_data_len = (u32)0;
	u32 eaten = (u32)0;
	u32 off = (u32)0;
	_sig_alg const *sig_alg = (_sig_alg const *)0;
	if (crl == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (crl_parsing_ctx *)0) {
		_LOR: {
			ret = - (5 * 100000 + 3424);
			goto out;
		}
	}
/*@ assert rte_758: mem_access: \valid(ctx); */
	*ctx = get_zeroized_crl_ctx_val();
	ret = parse_id_len(crl,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,& eaten,
	& seq_data_len);
	if (ret) goto out;
	len -= eaten;
	off += eaten;
	if (seq_data_len != len) {
		ret = - (5 * 100000 + 3451);
		goto out;
	}
	ret = parse_x509_TBSCertList(ctx,crl,off,len,& sig_alg,& eaten);
	if (ret) goto out;
/*@ assert rte_759: mem_access: \valid(&ctx->tbs_start); */
	ctx->tbs_start = off;
/*@ assert rte_760: mem_access: \valid(&ctx->tbs_len); */
	ctx->tbs_len = eaten;
	len -= eaten;
	off += eaten;
	ret = parse_x509_crl_signatureAlgorithm(ctx,crl,off,len,& eaten);
	if (ret) goto out;
	len -= eaten;
	off += eaten;
	ret = parse_x509_crl_signatureValue(ctx,crl,off,len,sig_alg,& eaten);
	if (ret) goto out;
	if (len != eaten) {
		ret = - (5 * 100000 + 3502);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

int parse_x509_cert(cert_parsing_ctx *ctx, u8 const *cert, u32 len);

static int parse_pubkey_ed448(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len);

static int parse_pubkey_x448(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len);

static int parse_pubkey_ed25519(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len);

static int parse_pubkey_x25519(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len);

static int parse_pubkey_ec(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len);

static int parse_pubkey_rsa(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len);

static int parse_pubkey_gostr3410_94(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len);

static int parse_pubkey_gostr3410_2001(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len);

static int parse_pubkey_gostr3410_2012_256(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_pubkey_gostr3410_2012_512(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_pubkey_dsa(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len);

static int parse_pubkey_bign(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len);

static int parse_algoid_pubkey_params_ecPublicKey(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len);

static int parse_algoid_pubkey_params_ed25519(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len);

static int parse_algoid_pubkey_params_ed448(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_x25519(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_x448(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_rsa(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_gost_r3410_2012_256(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len);

static int parse_algoid_pubkey_params_gost_r3410_2012_512(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len);

static int parse_algoid_pubkey_params_gost_r3410_2001(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len);

static int parse_algoid_pubkey_params_gost_r3410_94(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len);

static int parse_algoid_pubkey_params_dsa(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_ea_rsa(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_none(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static int parse_algoid_pubkey_params_bign(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len);

static u8 const _pkcs1_rsaEncryption_pubkey_name[21] =
{(u8)'P',
	(u8)'K',
	(u8)'C',
	(u8)'S',
	(u8)'-',
	(u8)'1',
	(u8)' ',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)'E',
	(u8)'n',
	(u8)'c',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'i',
	(u8)'o',
	(u8)'n',
(u8)'\000'};

static u8 const _pkcs1_rsaEncryption_pubkey_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _pkcs1_rsaEncryption_pubkey_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x01};

static _pubkey_alg const _pkcs1_rsaEncryption_pubkey_alg =
{.alg_name = _pkcs1_rsaEncryption_pubkey_name,
	.alg_printable_oid = _pkcs1_rsaEncryption_pubkey_printable_oid,
	.alg_der_oid = _pkcs1_rsaEncryption_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_pkcs1_rsaEncryption_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_rsa,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _weird_rsa_pub_1_pubkey_name[29] =
{(u8)'U',
	(u8)'n',
	(u8)'d',
	(u8)'o',
	(u8)'c',
	(u8)'u',
	(u8)'m',
	(u8)'e',
	(u8)'n',
	(u8)'t',
	(u8)'e',
	(u8)'d',
	(u8)' ',
	(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)' ',
	(u8)'k',
	(u8)'e',
	(u8)'y',
	(u8)' ',
	(u8)'o',
	(u8)'i',
	(u8)'d',
(u8)'\000'};

static u8 const _weird_rsa_pub_1_pubkey_printable_oid[21] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'8',
	(u8)'8',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _weird_rsa_pub_1_pubkey_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0x77,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x01};

static _pubkey_alg const _weird_rsa_pub_1_pubkey_alg =
{.alg_name = _weird_rsa_pub_1_pubkey_name,
	.alg_printable_oid = _weird_rsa_pub_1_pubkey_printable_oid,
	.alg_der_oid = _weird_rsa_pub_1_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_weird_rsa_pub_1_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_rsa,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _weird_rsa_pub_2_pubkey_name[23] =
{(u8)'a',
	(u8)'n',
	(u8)'o',
	(u8)'t',
	(u8)'h',
	(u8)'e',
	(u8)'r',
	(u8)' ',
	(u8)'r',
	(u8)'s',
	(u8)'a',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'k',
	(u8)'e',
	(u8)'y',
	(u8)' ',
	(u8)'o',
	(u8)'i',
	(u8)'d',
(u8)'\000'};

static u8 const _weird_rsa_pub_2_pubkey_printable_oid[22] =
{(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'8',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
	(u8)'5',
	(u8)'4',
	(u8)'9',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _weird_rsa_pub_2_pubkey_der_oid[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x3a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x01,
(u8)0x01};

static _pubkey_alg const _weird_rsa_pub_2_pubkey_alg =
{.alg_name = _weird_rsa_pub_2_pubkey_name,
	.alg_printable_oid = _weird_rsa_pub_2_pubkey_printable_oid,
	.alg_der_oid = _weird_rsa_pub_2_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_weird_rsa_pub_2_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_rsa,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _rsa_gip_cps_pubkey_name[8] =
{(u8)'G', (u8)'I', (u8)'P', (u8)'-', (u8)'C', (u8)'P', (u8)'S', (u8)'\000'};

static u8 const _rsa_gip_cps_pubkey_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'5',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'7',
	(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _rsa_gip_cps_pubkey_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x81,
	(u8)0x7a,
	(u8)0x01,
	(u8)0x47,
	(u8)0x02,
	(u8)0x06,
(u8)0x01};

static _pubkey_alg const _rsa_gip_cps_pubkey_alg =
{.alg_name = _rsa_gip_cps_pubkey_name,
	.alg_printable_oid = _rsa_gip_cps_pubkey_printable_oid,
	.alg_der_oid = _rsa_gip_cps_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsa_gip_cps_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_rsa,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _rsassa_pss_shake256_pubkey_name[20] =
{(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'S',
	(u8)'S',
	(u8)'A',
	(u8)'-',
	(u8)'P',
	(u8)'S',
	(u8)'S',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'K',
	(u8)'E',
	(u8)'2',
	(u8)'5',
	(u8)'6',
(u8)'\000'};

static u8 const _rsassa_pss_shake256_pubkey_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'1',
(u8)'\000'};

static u8 const _rsassa_pss_shake256_pubkey_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x06,
(u8)0x1f};

static _pubkey_alg const _rsassa_pss_shake256_pubkey_alg =
{.alg_name = _rsassa_pss_shake256_pubkey_name,
	.alg_printable_oid = _rsassa_pss_shake256_pubkey_printable_oid,
	.alg_der_oid = _rsassa_pss_shake256_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsassa_pss_shake256_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_none,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _rsassa_pss_shake128_pubkey_name[20] =
{(u8)'R',
	(u8)'S',
	(u8)'A',
	(u8)'S',
	(u8)'S',
	(u8)'A',
	(u8)'-',
	(u8)'P',
	(u8)'S',
	(u8)'S',
	(u8)'-',
	(u8)'S',
	(u8)'H',
	(u8)'A',
	(u8)'K',
	(u8)'E',
	(u8)'1',
	(u8)'2',
	(u8)'8',
(u8)'\000'};

static u8 const _rsassa_pss_shake128_pubkey_printable_oid[19] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'6',
	(u8)'.',
	(u8)'3',
	(u8)'0',
(u8)'\000'};

static u8 const _rsassa_pss_shake128_pubkey_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x06,
(u8)0x1e};

static _pubkey_alg const _rsassa_pss_shake128_pubkey_alg =
{.alg_name = _rsassa_pss_shake128_pubkey_name,
	.alg_printable_oid = _rsassa_pss_shake128_pubkey_printable_oid,
	.alg_der_oid = _rsassa_pss_shake128_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_rsassa_pss_shake128_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_none,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _ea_rsa_pubkey_name[10] =
{(u8)'i',
	(u8)'d',
	(u8)'-',
	(u8)'e',
	(u8)'a',
	(u8)'-',
	(u8)'r',
	(u8)'s',
	(u8)'a',
(u8)'\000'};

static u8 const _ea_rsa_pubkey_printable_oid[10] =
{(u8)'2',
	(u8)'.',
	(u8)'5',
	(u8)'.',
	(u8)'8',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _ea_rsa_pubkey_der_oid[6] =
{(u8)0x06, (u8)0x04, (u8)0x55, (u8)0x08, (u8)0x01, (u8)0x01};

static _pubkey_alg const _ea_rsa_pubkey_alg =
{.alg_name = _ea_rsa_pubkey_name,
	.alg_printable_oid = _ea_rsa_pubkey_printable_oid,
	.alg_der_oid = _ea_rsa_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ea_rsa_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_RSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_ea_rsa,
.parse_pubkey = & parse_pubkey_rsa};

static u8 const _ecpublickey_pubkey_name[12] =
{(u8)'e',
	(u8)'c',
	(u8)'P',
	(u8)'u',
	(u8)'b',
	(u8)'l',
	(u8)'i',
	(u8)'c',
	(u8)'K',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _ecpublickey_pubkey_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _ecpublickey_pubkey_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x3d,
	(u8)0x02,
(u8)0x01};

static _pubkey_alg const _ecpublickey_pubkey_alg =
{.alg_name = _ecpublickey_pubkey_name,
	.alg_printable_oid = _ecpublickey_pubkey_printable_oid,
	.alg_der_oid = _ecpublickey_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ecpublickey_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_ECPUBKEY,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_ecPublicKey,
.parse_pubkey = & parse_pubkey_ec};

static u8 const _dsa_pubkey_pubkey_name[23] =
{(u8)'D',
	(u8)'S',
	(u8)'A',
	(u8)' ',
	(u8)'s',
	(u8)'u',
	(u8)'b',
	(u8)'j',
	(u8)'e',
	(u8)'c',
	(u8)'t',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'l',
	(u8)'i',
	(u8)'c',
	(u8)' ',
	(u8)'k',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _dsa_pubkey_pubkey_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'8',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'0',
	(u8)'4',
	(u8)'0',
	(u8)'.',
	(u8)'4',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _dsa_pubkey_pubkey_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0xce,
	(u8)0x38,
	(u8)0x04,
(u8)0x01};

static _pubkey_alg const _dsa_pubkey_pubkey_alg =
{.alg_name = _dsa_pubkey_pubkey_name,
	.alg_printable_oid = _dsa_pubkey_pubkey_printable_oid,
	.alg_der_oid = _dsa_pubkey_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_dsa_pubkey_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_DSA,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_dsa,
.parse_pubkey = & parse_pubkey_dsa};

static u8 const _x448_pubkey_name[5] =
{(u8)'X', (u8)'4', (u8)'4', (u8)'8', (u8)'\000'};

static u8 const _x448_pubkey_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'1',
(u8)'\000'};

static u8 const _x448_pubkey_der_oid[5] =
{(u8)0x06, (u8)0x03, (u8)0x2b, (u8)0x65, (u8)0x6f};

static _pubkey_alg const _x448_pubkey_alg =
{.alg_name = _x448_pubkey_name,
	.alg_printable_oid = _x448_pubkey_printable_oid,
	.alg_der_oid = _x448_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_x448_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_X448,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_x448,
.parse_pubkey = & parse_pubkey_x448};

static u8 const _ed448_pubkey_name[6] =
{(u8)'E', (u8)'d', (u8)'4', (u8)'4', (u8)'8', (u8)'\000'};

static u8 const _ed448_pubkey_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'3',
(u8)'\000'};

static u8 const _ed448_pubkey_der_oid[5] =
{(u8)0x06, (u8)0x03, (u8)0x2b, (u8)0x65, (u8)0x71};

static _pubkey_alg const _ed448_pubkey_alg =
{.alg_name = _ed448_pubkey_name,
	.alg_printable_oid = _ed448_pubkey_printable_oid,
	.alg_der_oid = _ed448_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ed448_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_ED448,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_ed448,
.parse_pubkey = & parse_pubkey_ed448};

static u8 const _x25519_pubkey_name[7] =
{(u8)'X', (u8)'2', (u8)'5', (u8)'5', (u8)'1', (u8)'9', (u8)'\000'};

static u8 const _x25519_pubkey_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'0',
(u8)'\000'};

static u8 const _x25519_pubkey_der_oid[5] =
{(u8)0x06, (u8)0x03, (u8)0x2b, (u8)0x65, (u8)0x6e};

static _pubkey_alg const _x25519_pubkey_alg =
{.alg_name = _x25519_pubkey_name,
	.alg_printable_oid = _x25519_pubkey_printable_oid,
	.alg_der_oid = _x25519_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_x25519_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_X25519,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_x25519,
.parse_pubkey = & parse_pubkey_x25519};

static u8 const _ed25519_pubkey_name[8] =
{(u8)'E', (u8)'d', (u8)'2', (u8)'5', (u8)'5', (u8)'1', (u8)'9', (u8)'\000'};

static u8 const _ed25519_pubkey_printable_oid[12] =
{(u8)'1',
	(u8)'.',
	(u8)'3',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
(u8)'\000'};

static u8 const _ed25519_pubkey_der_oid[5] =
{(u8)0x06, (u8)0x03, (u8)0x2b, (u8)0x65, (u8)0x70};

static _pubkey_alg const _ed25519_pubkey_alg =
{.alg_name = _ed25519_pubkey_name,
	.alg_printable_oid = _ed25519_pubkey_printable_oid,
	.alg_der_oid = _ed25519_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_ed25519_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_ED25519,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_ed25519,
.parse_pubkey = & parse_pubkey_ed25519};

static u8 const _bign_pubkey_name[12] =
{(u8)'b',
	(u8)'i',
	(u8)'g',
	(u8)'n',
	(u8)'-',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'k',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _bign_pubkey_printable_oid[28] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'1',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'0',
	(u8)'.',
	(u8)'3',
	(u8)'4',
	(u8)'.',
	(u8)'1',
	(u8)'0',
	(u8)'1',
	(u8)'.',
	(u8)'4',
	(u8)'5',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _bign_pubkey_der_oid[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2a,
	(u8)0x70,
	(u8)0x00,
	(u8)0x02,
	(u8)0x00,
	(u8)0x22,
	(u8)0x65,
	(u8)0x2d,
	(u8)0x02,
(u8)0x01};

static _pubkey_alg const _bign_pubkey_alg =
{.alg_name = _bign_pubkey_name,
	.alg_printable_oid = _bign_pubkey_printable_oid,
	.alg_der_oid = _bign_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_bign_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_BIGN_PUBKEY,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_bign,
.parse_pubkey = & parse_pubkey_bign};

static u8 const _gost_R3410_94_pubkey_name[24] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'9',
	(u8)'4',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'l',
	(u8)'i',
	(u8)'c',
	(u8)' ',
	(u8)'k',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _gost_R3410_94_pubkey_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'0',
(u8)'\000'};

static u8 const _gost_R3410_94_pubkey_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
(u8)0x14};

static _pubkey_alg const _gost_R3410_94_pubkey_alg =
{.alg_name = _gost_R3410_94_pubkey_name,
	.alg_printable_oid = _gost_R3410_94_pubkey_printable_oid,
	.alg_der_oid = _gost_R3410_94_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3410_94_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_GOSTR3410_94,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_gost_r3410_94,
.parse_pubkey = & parse_pubkey_gostr3410_94};

static u8 const _gost_R3410_2001_pubkey_name[26] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'0',
	(u8)'1',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'l',
	(u8)'i',
	(u8)'c',
	(u8)' ',
	(u8)'k',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _gost_R3410_2001_pubkey_printable_oid[15] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'1',
	(u8)'9',
(u8)'\000'};

static u8 const _gost_R3410_2001_pubkey_der_oid[8] =
{(u8)0x06,
	(u8)0x06,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
(u8)0x13};

static _pubkey_alg const _gost_R3410_2001_pubkey_alg =
{.alg_name = _gost_R3410_2001_pubkey_name,
	.alg_printable_oid = _gost_R3410_2001_pubkey_printable_oid,
	.alg_der_oid = _gost_R3410_2001_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3410_2001_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_GOSTR3410_2001,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_gost_r3410_2001,
.parse_pubkey = & parse_pubkey_gostr3410_2001};

static u8 const _gost_R3410_2012_512_pubkey_name[29] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'-',
	(u8)'5',
	(u8)'1',
	(u8)'2',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'l',
	(u8)'i',
	(u8)'c',
	(u8)' ',
	(u8)'k',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_pubkey_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _gost_R3410_2012_512_pubkey_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x01,
	(u8)0x01,
(u8)0x02};

static _pubkey_alg const _gost_R3410_2012_512_pubkey_alg =
{.alg_name = _gost_R3410_2012_512_pubkey_name,
	.alg_printable_oid = _gost_R3410_2012_512_pubkey_printable_oid,
	.alg_der_oid = _gost_R3410_2012_512_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3410_2012_512_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_GOSTR3410_2012_512,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_gost_r3410_2012_512,
.parse_pubkey = & parse_pubkey_gostr3410_2012_512};

static u8 const _gost_R3410_2012_256_pubkey_name[29] =
{(u8)'g',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'-',
	(u8)'2',
	(u8)'0',
	(u8)'1',
	(u8)'2',
	(u8)'-',
	(u8)'2',
	(u8)'5',
	(u8)'6',
	(u8)' ',
	(u8)'p',
	(u8)'u',
	(u8)'b',
	(u8)'l',
	(u8)'i',
	(u8)'c',
	(u8)' ',
	(u8)'k',
	(u8)'e',
	(u8)'y',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_pubkey_printable_oid[18] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'7',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _gost_R3410_2012_256_pubkey_der_oid[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2a,
	(u8)0x85,
	(u8)0x03,
	(u8)0x07,
	(u8)0x01,
	(u8)0x01,
	(u8)0x01,
(u8)0x01};

static _pubkey_alg const _gost_R3410_2012_256_pubkey_alg =
{.alg_name = _gost_R3410_2012_256_pubkey_name,
	.alg_printable_oid = _gost_R3410_2012_256_pubkey_printable_oid,
	.alg_der_oid = _gost_R3410_2012_256_pubkey_der_oid,
	.alg_der_oid_len = (u32)sizeof(_gost_R3410_2012_256_pubkey_der_oid),
	.pubkey_id = SPKI_ALG_GOSTR3410_2012_256,
	.parse_algoid_pubkey_params = & parse_algoid_pubkey_params_gost_r3410_2012_256,
.parse_pubkey = & parse_pubkey_gostr3410_2012_256};

static _pubkey_alg const *known_pubkey_algs[18] =
{& _ecpublickey_pubkey_alg,
	& _pkcs1_rsaEncryption_pubkey_alg,
	& _rsa_gip_cps_pubkey_alg,
	& _rsassa_pss_shake256_pubkey_alg,
	& _rsassa_pss_shake128_pubkey_alg,
	& _ed448_pubkey_alg,
	& _ed25519_pubkey_alg,
	& _x448_pubkey_alg,
	& _x25519_pubkey_alg,
	& _gost_R3410_94_pubkey_alg,
	& _gost_R3410_2001_pubkey_alg,
	& _gost_R3410_2012_512_pubkey_alg,
	& _gost_R3410_2012_256_pubkey_alg,
	& _bign_pubkey_alg,
	& _weird_rsa_pub_1_pubkey_alg,
	& _weird_rsa_pub_2_pubkey_alg,
	& _dsa_pubkey_pubkey_alg,
& _ea_rsa_pubkey_alg};

u16 const num_known_pubkey_algs =
(u16)(sizeof(known_pubkey_algs) / sizeof(known_pubkey_algs[0]));

static u8 const _GostR3410_94_Test_gost_94_params_name[26] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'T',
	(u8)'e',
	(u8)'s',
	(u8)'t',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_Test_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'0',
(u8)'\000'};

static u8 const _GostR3410_94_Test_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x20,
(u8)0x00};

static _gost94_pub_params const _GostR3410_94_Test_ParamSet =
{.params_name = _GostR3410_94_Test_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_Test_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_Test_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_Test_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_TEST};

static u8 const _GostR3410_94_CryptoPro_A_gost_94_params_name[34] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'A',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_A_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_A_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x20,
(u8)0x02};

static _gost94_pub_params const _GostR3410_94_CryptoPro_A_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_A_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_A_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_A_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_A_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_A};

static u8 const _GostR3410_94_CryptoPro_B_gost_94_params_name[34] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'B',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_B_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_B_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x20,
(u8)0x03};

static _gost94_pub_params const _GostR3410_94_CryptoPro_B_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_B_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_B_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_B_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_B_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_B};

static u8 const _GostR3410_94_CryptoPro_C_gost_94_params_name[34] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'C',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_C_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'4',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_C_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x20,
(u8)0x04};

static _gost94_pub_params const _GostR3410_94_CryptoPro_C_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_C_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_C_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_C_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_C_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_C};

static u8 const _GostR3410_94_CryptoPro_D_gost_94_params_name[34] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'D',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_D_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'2',
	(u8)'.',
	(u8)'5',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_D_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x20,
(u8)0x05};

static _gost94_pub_params const _GostR3410_94_CryptoPro_D_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_D_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_D_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_D_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_D_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_D};

static u8 const _GostR3410_94_CryptoPro_XchA_gost_94_params_name[37] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'X',
	(u8)'c',
	(u8)'h',
	(u8)'A',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_XchA_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'3',
	(u8)'.',
	(u8)'1',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_XchA_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x21,
(u8)0x01};

static _gost94_pub_params const _GostR3410_94_CryptoPro_XchA_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_XchA_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_XchA_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_XchA_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_XchA_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_XCHA};

static u8 const _GostR3410_94_CryptoPro_XchB_gost_94_params_name[37] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'X',
	(u8)'c',
	(u8)'h',
	(u8)'B',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_XchB_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'3',
	(u8)'.',
	(u8)'2',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_XchB_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x21,
(u8)0x02};

static _gost94_pub_params const _GostR3410_94_CryptoPro_XchB_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_XchB_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_XchB_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_XchB_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_XchB_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_XCHB};

static u8 const _GostR3410_94_CryptoPro_XchC_gost_94_params_name[37] =
{(u8)'G',
	(u8)'o',
	(u8)'s',
	(u8)'t',
	(u8)'R',
	(u8)'3',
	(u8)'4',
	(u8)'1',
	(u8)'0',
	(u8)'_',
	(u8)'9',
	(u8)'4',
	(u8)'_',
	(u8)'C',
	(u8)'r',
	(u8)'y',
	(u8)'p',
	(u8)'t',
	(u8)'o',
	(u8)'P',
	(u8)'r',
	(u8)'o',
	(u8)'_',
	(u8)'X',
	(u8)'c',
	(u8)'h',
	(u8)'C',
	(u8)'_',
	(u8)'P',
	(u8)'a',
	(u8)'r',
	(u8)'a',
	(u8)'m',
	(u8)'S',
	(u8)'e',
	(u8)'t',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_XchC_gost_94_params_printable_oid[17] =
{(u8)'1',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'6',
	(u8)'4',
	(u8)'3',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'2',
	(u8)'.',
	(u8)'3',
	(u8)'3',
	(u8)'.',
	(u8)'3',
(u8)'\000'};

static u8 const _GostR3410_94_CryptoPro_XchC_gost_94_params_der_oid[9] =
{(u8)0x06,
	(u8)0x07,
	(u8)0x2A,
	(u8)0x85,
	(u8)0x03,
	(u8)0x02,
	(u8)0x02,
	(u8)0x21,
(u8)0x03};

static _gost94_pub_params const _GostR3410_94_CryptoPro_XchC_ParamSet =
{.params_name = _GostR3410_94_CryptoPro_XchC_gost_94_params_name,
	.params_printable_oid = _GostR3410_94_CryptoPro_XchC_gost_94_params_printable_oid,
	.params_der_oid = _GostR3410_94_CryptoPro_XchC_gost_94_params_der_oid,
	.params_der_oid_len = (u32)sizeof(_GostR3410_94_CryptoPro_XchC_gost_94_params_der_oid),
.params_id = GOST94_PARAMS_CRYPTOPRO_XCHC};

static _gost94_pub_params const *known_gost_94_params[8] =
{& _GostR3410_94_Test_ParamSet,
	& _GostR3410_94_CryptoPro_A_ParamSet,
	& _GostR3410_94_CryptoPro_B_ParamSet,
	& _GostR3410_94_CryptoPro_C_ParamSet,
	& _GostR3410_94_CryptoPro_D_ParamSet,
	& _GostR3410_94_CryptoPro_XchA_ParamSet,
	& _GostR3410_94_CryptoPro_XchB_ParamSet,
& _GostR3410_94_CryptoPro_XchC_ParamSet};

u16 const num_known_gost94_params =
(u16)(sizeof(known_gost_94_params) / sizeof(known_gost_94_params[0]));

/*@
	requires buf == \null || \valid_read(buf + (0 .. len - 1));
	requires num_known_gost94_params <= 8;
	requires \forall integer i; 0 <= i < num_known_gost94_params ==>known_gost_94_params[i] != \null &&\valid_read(known_gost_94_params[i]) &&\valid_read(&known_gost_94_params[i]->params_der_oid_len) &&\valid_read(&known_gost_94_params[i]->params_der_oid) &&(known_gost_94_params[i]->params_der_oid_len == 0 ||\valid_read(known_gost_94_params[i]->params_der_oid +(0 .. known_gost_94_params[i]->params_der_oid_len - 1)));
	assigns \nothing;
	ensures (buf == \null || len == 0) ==> \result == \null;
	ensures \result == \null ||(\exists integer i; 0 <= i < num_known_gost94_params &&\result == known_gost_94_params[i]);
	ensures \result != \null ==>\result->params_der_oid_len == len &&(\forall integer j; 0 <= j < len ==>\result->params_der_oid[j] == buf[j]);
*/
_gost94_pub_params const *find_gost94_params_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_gost94_pub_params const *found = (_gost94_pub_params const *)0;
	_gost94_pub_params const *cur = (_gost94_pub_params const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= num_known_gost94_params;
		loop invariant found == \null ==>(\forall integer i; 0 <= i < k ==>known_gost_94_params[i]->params_der_oid_len != len ||bufs_differ(known_gost_94_params[i]->params_der_oid, buf,known_gost_94_params[i]->params_der_oid_len) != 0);
		loop invariant found != \null ==>(\exists integer i; 0 <= i < k &&found == known_gost_94_params[i] &&found->params_der_oid_len == len &&bufs_differ(found->params_der_oid, buf, found->params_der_oid_len) == 0);
		loop assigns k, found, cur;
		loop variant num_known_gost94_params - k;
	*/
	while ((int)k < (int)num_known_gost94_params) {
		{
			int ret;
			int tmp_0;
			int tmp;
		/*@ assert rte_321: index_bound: k < 8; */
			cur = known_gost_94_params[k];
		/*@ assert rte_322: mem_access: \valid_read(&cur->params_der_oid_len); */
			if (cur->params_der_oid_len != len) goto __Cont;
		/*@ assert rte_323: mem_access: \valid_read(&cur->params_der_oid); */
		/*@ assert rte_324: mem_access: \valid_read(&cur->params_der_oid_len); */
			tmp = bufs_differ(cur->params_der_oid,buf,cur->params_der_oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_325: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

/*@
	requires buf == \null || len == 0 || \valid_read(buf + (0 .. len - 1));
	requires num_known_pubkey_algs == 18;
	requires \valid_read(known_pubkey_algs + (0 .. 17));
	requires \forall integer i; 0 <= i < 18 ==>known_pubkey_algs[i] != \null &&\valid_read(known_pubkey_algs[i]) &&\valid_read(&(known_pubkey_algs[i]->alg_der_oid)) &&\valid_read(&(known_pubkey_algs[i]->alg_der_oid_len)) &&(known_pubkey_algs[i]->alg_der_oid_len == 0 ||\valid_read(known_pubkey_algs[i]->alg_der_oid + (0 .. known_pubkey_algs[i]->alg_der_oid_len - 1)));
	assigns \nothing;
	ensures buf == \null ==> \result == \null;
	ensures len == 0 ==> \result == \null;
	ensures \result == \null ||(\exists integer i; 0 <= i < 18 && \result == known_pubkey_algs[i]);
	ensures \result != \null ==>\result->alg_der_oid_len == len &&(\forall integer j; 0 <= j < len ==>\result->alg_der_oid[j] == buf[j]);
	ensures (\result == \null && buf != \null && len != 0) ==>(\forall integer i; 0 <= i < 18 ==>known_pubkey_algs[i]->alg_der_oid_len != len ||(\exists integer j; 0 <= j < len &&known_pubkey_algs[i]->alg_der_oid[j] != buf[j]));
*/
_pubkey_alg const *find_pubkey_alg_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_pubkey_alg const *found = (_pubkey_alg const *)0;
	_pubkey_alg const *cur = (_pubkey_alg const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= num_known_pubkey_algs;
		loop invariant found == \null ||(\exists integer i; 0 <= i < k && found == known_pubkey_algs[i]);
		loop invariant found == \null ==>(\forall integer i; 0 <= i < k ==>known_pubkey_algs[i]->alg_der_oid_len != len ||(\exists integer j; 0 <= j < len &&known_pubkey_algs[i]->alg_der_oid[j] != buf[j]));
		loop assigns k, cur;
		loop variant num_known_pubkey_algs - k;
	*/
	while ((int)k < (int)num_known_pubkey_algs) {
		{
			int ret;
			int tmp_0;
			int tmp;
		/*@ assert rte_285: index_bound: k < 18; */
			cur = known_pubkey_algs[k];
		/*@ assert rte_286: mem_access: \valid_read(&cur->alg_der_oid_len); */
			if (cur->alg_der_oid_len != len) goto __Cont;
		/*@ assert rte_287: mem_access: \valid_read(&cur->alg_der_oid); */
		/*@ assert rte_288: mem_access: \valid_read(&cur->alg_der_oid_len); */
			tmp = bufs_differ(cur->alg_der_oid,buf,cur->alg_der_oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_289: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

static int parse_x509_cert_Version(u8 const *cert, u32 off, u32 len,
u8 *version, u32 *eaten)
{
	int ret;
	u8 const *buf = cert + off;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (version == (u8 *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 315);
			goto out;
		}
	}
	ret = parse_explicit_id_len(buf,len,(u32)0,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,& data_len);
	if (ret) {
		ret = X509_PARSER_ERROR_VERSION_ABSENT;
		goto out;
	}
	buf += hdr_len;
	if (data_len != (u32)1) {
		ret = X509_PARSER_ERROR_VERSION_UNEXPECTED_LENGTH;
		goto out;
	}
/*@ assert rte_237: mem_access: \valid(version); */
/*@ assert rte_238: mem_access: \valid_read(buf + 0); */
	*version = *(buf + 0);
/*@ assert rte_239: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_CertSerialNumber(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, tag_class exp_class,
u32 exp_type, u32 *eaten)
{
	int ret;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 378);
			goto out;
		}
	}
	ret = parse_SerialNumber(cert,off,len,exp_class,exp_type,eaten);
	if (ret) goto out;
/*@ assert rte_240: mem_access: \valid(&ctx->serial_start); */
	ctx->serial_start = off + (u32)2;
/*@ assert rte_241: mem_access: \valid(&ctx->serial_len); */
/*@ assert rte_242: mem_access: \valid_read(eaten); */
	ctx->serial_len = *eaten - (u32)2;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_ecPublicKey(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len)
{
	int ret;
	u8 const *buf = cert + off;
	_curve const *curve = (_curve const *)0;
	u32 oid_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 424);
			goto out;
		}
	}
/*@ assert rte_290: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) == 0x30) {
		ret = -1;
		goto out;
	}
	ret = parse_OID(buf,len,& oid_len);
	if (ret) goto out;
	if (oid_len != len) {
		ret = - (2 * 100000 + 478);
		goto out;
	}
	curve = find_curve_by_oid(buf,oid_len);
	if (curve == (_curve const *)0) {
		ret = - (2 * 100000 + 486);
		goto out;
	}
/*@ assert rte_291: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.curve_order_bit_len); */
/*@ assert rte_292: mem_access: \valid_read(&curve->crv_order_bit_len); */
	ctx->spki_alg_params.ecpubkey.curve_order_bit_len = curve->crv_order_bit_len;
/*@ assert rte_293: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.curve); */
/*@ assert rte_294: mem_access: \valid_read(&curve->crv_id); */
	ctx->spki_alg_params.ecpubkey.curve = curve->crv_id;
	ret = 0;
	out: ;
	return ret;
}

static int parse_pubkey_eddsa(u8 const *cert, u32 off, u32 len,
u32 exp_pub_len, u32 *raw_pub_off,
u32 *raw_pub_len)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (raw_pub_off == (u32 *)0) goto _LOR;
	else
	if (raw_pub_len == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 545);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 566);
		goto out;
	}
	buf += hdr_len;
/*@ assert rte_377: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 579);
		goto out;
	}
	buf ++;
	remain = data_len - (u32)1;
	if (remain != exp_pub_len) {
		ret = - (2 * 100000 + 587);
		goto out;
	}
	ret = 0;
/*@ assert rte_378: mem_access: \valid(raw_pub_off); */
	*raw_pub_off = (off + hdr_len) + (u32)1;
/*@ assert rte_379: mem_access: \valid(raw_pub_len); */
	*raw_pub_len = exp_pub_len;
	out: ;
	return ret;
}

static int parse_pubkey_ed25519(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len)
{
	int tmp;
	tmp = parse_pubkey_eddsa(cert,off,len,(u32)32,
	& ctx->spki_alg_params.ed25519.ed25519_raw_pub_off,
	& ctx->spki_alg_params.ed25519.ed25519_raw_pub_len);
	return tmp;
}

static int parse_pubkey_x25519(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len)
{
	int tmp;
	tmp = parse_pubkey_eddsa(cert,off,len,(u32)32,
	& ctx->spki_alg_params.x25519.x25519_raw_pub_off,
	& ctx->spki_alg_params.x25519.x25519_raw_pub_len);
	return tmp;
}

static int parse_pubkey_ed448(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len)
{
	int tmp;
	tmp = parse_pubkey_eddsa(cert,off,len,(u32)57,
	& ctx->spki_alg_params.ed448.ed448_raw_pub_off,
	& ctx->spki_alg_params.ed448.ed448_raw_pub_len);
	return tmp;
}

static int parse_pubkey_x448(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len)
{
	int tmp;
	tmp = parse_pubkey_eddsa(cert,off,len,(u32)57,
	& ctx->spki_alg_params.x448.x448_raw_pub_off,
	& ctx->spki_alg_params.x448.x448_raw_pub_len);
	return tmp;
}

static int parse_pubkey_ec(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len)
{
	u32 remain;
	u32 order_ceil_len;
	int ret;
	u8 pc;
	u8 const *buf = cert + off;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 742);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 763);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
/*@ assert rte_380: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 777);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
	if (remain == (u32)0) {
		ret = - (2 * 100000 + 795);
		goto out;
	}
/*@ assert rte_381: mem_access: \valid_read(buf + 0); */
	pc = *(buf + 0);
	remain -= (u32)1;
	buf ++;
	off += (u32)1;
/*@ assert rte_382: mem_access: \valid_read(&ctx->spki_alg_params.ecpubkey.curve_order_bit_len); */
	order_ceil_len = (ctx->spki_alg_params.ecpubkey.curve_order_bit_len + (u32)7) / (u32)8;
	switch ((int)pc) {
		case 0x04: ;
		if (remain != order_ceil_len * (u32)2) {
			ret = - (2 * 100000 + 814);
			goto out;
		}
	/*@ assert rte_383: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.compression); */
		ctx->spki_alg_params.ecpubkey.compression = (int)pc;
	/*@ assert rte_384: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_x_off); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_off = off;
	/*@ assert rte_385: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_x_len); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_len = order_ceil_len;
	/*@ assert rte_386: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_y_off); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_off = off + order_ceil_len;
	/*@ assert rte_387: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_y_len); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_len = order_ceil_len;
		break;
		case 0x02: case 0x03: ;
		if (remain != order_ceil_len) {
			ret = - (2 * 100000 + 828);
			goto out;
		}
	/*@ assert rte_388: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.compression); */
		ctx->spki_alg_params.ecpubkey.compression = (int)pc;
	/*@ assert rte_389: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_x_off); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_off = off;
	/*@ assert rte_390: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_x_len); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_x_len = order_ceil_len;
	/*@ assert rte_391: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_y_off); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_off = (u32)0;
	/*@ assert rte_392: mem_access: \valid(&ctx->spki_alg_params.ecpubkey.ecc_raw_y_len); */
		ctx->spki_alg_params.ecpubkey.ecc_raw_y_len = (u32)0;
		break;
		default: ret = - (2 * 100000 + 840);
		goto out;
		break;
	}
	ret = 0;
	out: ;
	return ret;
}

static int _parse_pubkey_gost_on_curves(u8 const *cert, u32 off, u32 len,
u32 exp_pub_len, u32 *raw_x_off,
u32 *raw_x_len, u32 *raw_y_off,
u32 *raw_y_len)
{
	u32 remain;
	int ret;
	u8 const *buf = cert + off;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (raw_x_off == (u32 *)0) goto _LOR;
	else
	if (raw_x_len == (u32 *)0) goto _LOR;
	else
	if (raw_y_off == (u32 *)0) goto _LOR;
	else
	if (raw_y_len == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 906);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 926);
		goto out;
	}
	off += hdr_len;
	buf += hdr_len;
/*@ assert rte_410: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 940);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_OCTET_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len != exp_pub_len) {
		ret = -1;
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
	remain -= hdr_len;
/*@ assert rte_411: mem_access: \valid(raw_x_off); */
	*raw_x_off = off;
/*@ assert rte_412: mem_access: \valid(raw_x_len); */
	*raw_x_len = exp_pub_len / (u32)2;
/*@ assert rte_413: mem_access: \valid(raw_y_off); */
/*@ assert rte_414: mem_access: \valid_read(raw_x_len); */
	*raw_y_off = off + *raw_x_len;
/*@ assert rte_415: mem_access: \valid(raw_y_len); */
	*raw_y_len = exp_pub_len / (u32)2;
	if (remain != data_len) {
		ret = - (2 * 100000 + 976);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_pubkey_gostr3410_94(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len)
{
	u32 remain;
	int ret;
	u8 const *buf = cert + off;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1026);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 1046);
		goto out;
	}
	off += hdr_len;
	buf += hdr_len;
/*@ assert rte_407: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 1060);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_OCTET_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len != (u32)128) {
		ret = -1;
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
	remain -= hdr_len;
	if (remain != data_len) {
		ret = - (2 * 100000 + 1091);
		goto out;
	}
/*@ assert rte_408: mem_access: \valid(&ctx->spki_alg_params.gost94.gost94_raw_pub_off); */
	ctx->spki_alg_params.gost94.gost94_raw_pub_off = off;
/*@ assert rte_409: mem_access: \valid(&ctx->spki_alg_params.gost94.gost94_raw_pub_len); */
	ctx->spki_alg_params.gost94.gost94_raw_pub_len = (u32)128;
	ret = 0;
	out: ;
	return ret;
}

static int parse_pubkey_gostr3410_2001(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len)
{
	int tmp;
	tmp = _parse_pubkey_gost_on_curves(cert,off,len,(u32)64,
	& ctx->spki_alg_params.gost2001.gost2001_raw_x_pub_off,
	& ctx->spki_alg_params.gost2001.gost2001_raw_x_pub_len,
	& ctx->spki_alg_params.gost2001.gost2001_raw_y_pub_off,
	& ctx->spki_alg_params.gost2001.gost2001_raw_y_pub_len);
	return tmp;
}

static int parse_pubkey_gostr3410_2012_256(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int tmp;
	tmp = _parse_pubkey_gost_on_curves(cert,off,len,(u32)64,
	& ctx->spki_alg_params.gost2012_256.gost2012_256_raw_x_pub_off,
	& ctx->spki_alg_params.gost2012_256.gost2012_256_raw_x_pub_len,
	& ctx->spki_alg_params.gost2012_256.gost2012_256_raw_y_pub_off,
	& ctx->spki_alg_params.gost2012_256.gost2012_256_raw_y_pub_len);
	return tmp;
}

static int parse_pubkey_gostr3410_2012_512(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int tmp;
	tmp = _parse_pubkey_gost_on_curves(cert,off,len,(u32)128,
	& ctx->spki_alg_params.gost2012_512.gost2012_512_raw_x_pub_off,
	& ctx->spki_alg_params.gost2012_512.gost2012_512_raw_x_pub_len,
	& ctx->spki_alg_params.gost2012_512.gost2012_512_raw_y_pub_off,
	& ctx->spki_alg_params.gost2012_512.gost2012_512_raw_y_pub_len);
	return tmp;
}

static int parse_pubkey_bign(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len)
{
	u32 order_ceil_len;
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 1203);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 1224);
		goto out;
	}
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 1235);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
/*@ assert rte_421: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 1249);
		goto out;
	}
	buf ++;
	off += (u32)1;
	remain = data_len - (u32)1;
/*@ assert rte_422: mem_access: \valid_read(&ctx->spki_alg_params.bign.curve_order_bit_len); */
	order_ceil_len = (ctx->spki_alg_params.bign.curve_order_bit_len + (u32)7) / (u32)8;
	if (remain != (u32)2 * order_ceil_len) {
		ret = - (2 * 100000 + 1263);
		goto out;
	}
/*@ assert rte_423: mem_access: \valid(&ctx->spki_alg_params.bign.bign_raw_x_pub_off); */
	ctx->spki_alg_params.bign.bign_raw_x_pub_off = off;
/*@ assert rte_424: mem_access: \valid(&ctx->spki_alg_params.bign.bign_raw_x_pub_len); */
	ctx->spki_alg_params.bign.bign_raw_x_pub_len = order_ceil_len;
/*@ assert rte_425: mem_access: \valid(&ctx->spki_alg_params.bign.bign_raw_y_pub_off); */
	ctx->spki_alg_params.bign.bign_raw_y_pub_off = off + order_ceil_len;
/*@ assert rte_426: mem_access: \valid(&ctx->spki_alg_params.bign.bign_raw_y_pub_len); */
	ctx->spki_alg_params.bign.bign_raw_y_pub_len = order_ceil_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_bign(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	_curve const *curve;
	u32 remain;
	int ret;
	u8 const *buf = cert + off;
	u32 oid_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1310);
			goto out;
		}
	}
	remain = len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	curve = find_curve_by_oid(buf,oid_len);
	if (curve == (_curve const *)0) {
		ret = - (2 * 100000 + 1326);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	if (remain) {
		ret = - (2 * 100000 + 1336);
		goto out;
	}
/*@ assert rte_349: mem_access: \valid(&ctx->spki_alg_params.bign.curve_order_bit_len); */
/*@ assert rte_350: mem_access: \valid_read(&curve->crv_order_bit_len); */
	ctx->spki_alg_params.bign.curve_order_bit_len = curve->crv_order_bit_len;
/*@ assert rte_351: mem_access: \valid(&ctx->spki_alg_params.bign.curve); */
/*@ assert rte_352: mem_access: \valid_read(&curve->crv_id); */
	ctx->spki_alg_params.bign.curve = curve->crv_id;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires \valid_read(cert) || cert == \null;
	assigns ctx->spki_alg_params.ed448.curve,ctx->spki_alg_params.ed448.curve_order_bit_len;
	ensures (ctx == \null || cert == \null || len != 0) ==> (\result == -201372);
	ensures (ctx != \null && cert != \null && len == 0) ==> (ctx->spki_alg_params.ed448.curve == CURVE_WEI448);
	ensures (ctx != \null && cert != \null && len == 0) ==> (ctx->spki_alg_params.ed448.curve_order_bit_len == 448);
	ensures (ctx != \null && cert != \null && len == 0) ==> (\result == 0);
*/
static int parse_algoid_pubkey_params_ed448(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len != (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1372);
			goto out;
		}
	}
/*@ assert rte_297: mem_access: \valid(&ctx->spki_alg_params.ed448.curve); */
	ctx->spki_alg_params.ed448.curve = CURVE_WEI448;
/*@ assert rte_298: mem_access: \valid(&ctx->spki_alg_params.ed448.curve_order_bit_len); */
	ctx->spki_alg_params.ed448.curve_order_bit_len = (u32)448;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires \valid(cert) || cert == \null;
	assigns \nothing;
	ensures (ctx == \null) ==> (\result == -(2 * 100000 + 1405));
	ensures (ctx != \null && cert == \null) ==> (\result == -(2 * 100000 + 1405));
	ensures (ctx != \null && cert != \null && len != (u32)0) ==> (\result == -(2 * 100000 + 1405));
	ensures (ctx != \null && cert != \null && len == (u32)0) ==> (ctx->spki_alg_params.x448.curve == CURVE_WEI448);
	ensures (ctx != \null && cert != \null && len == (u32)0) ==> (ctx->spki_alg_params.x448.curve_order_bit_len == (u32)448);
	ensures (ctx != \null && cert != \null && len == (u32)0) ==> (\result == 0);
*/
static int parse_algoid_pubkey_params_x448(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len != (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1405);
			goto out;
		}
	}
/*@ assert rte_301: mem_access: \valid(&ctx->spki_alg_params.x448.curve); */
	ctx->spki_alg_params.x448.curve = CURVE_WEI448;
/*@ assert rte_302: mem_access: \valid(&ctx->spki_alg_params.x448.curve_order_bit_len); */
	ctx->spki_alg_params.x448.curve_order_bit_len = (u32)448;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires cert == \null || \valid_read(cert);
	assigns ctx->spki_alg_params.ed25519.curve,@         ctx->spki_alg_params.ed25519.curve_order_bit_len;
	ensures (ctx == \null) ==> \result == -201438;
	ensures (ctx != \null && cert == \null) ==> \result == -201438;
	ensures (ctx != \null && cert != \null && len != 0) ==> \result == -201438;
	ensures (ctx != \null && cert != \null && len == 0) ==>@         (\result == 0 &&@          ctx->spki_alg_params.ed25519.curve == CURVE_WEI25519 &&@          ctx->spki_alg_params.ed25519.curve_order_bit_len == 256);
	ensures \result == 0 || \result == -201438;
*/
static int parse_algoid_pubkey_params_ed25519(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len != (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1438);
			goto out;
		}
	}
/*@ assert rte_295: mem_access: \valid(&ctx->spki_alg_params.ed25519.curve); */
	ctx->spki_alg_params.ed25519.curve = CURVE_WEI25519;
/*@ assert rte_296: mem_access: \valid(&ctx->spki_alg_params.ed25519.curve_order_bit_len); */
	ctx->spki_alg_params.ed25519.curve_order_bit_len = (u32)256;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires ctx == \null || \valid(ctx);
	requires cert == \null || \valid_read(cert + (0 .. len - 1)) || len == 0;
	assigns ctx->spki_alg_params.x25519.curve,@         ctx->spki_alg_params.x25519.curve_order_bit_len;
	ensures (ctx == \null || cert == \null || len != 0) ==> \result == -201471;
	ensures (ctx != \null && cert != \null && len == 0) ==> \result == 0;
	ensures (ctx != \null && cert != \null && len == 0) ==>@         ctx->spki_alg_params.x25519.curve == CURVE_WEI25519;
	ensures (ctx != \null && cert != \null && len == 0) ==>@         ctx->spki_alg_params.x25519.curve_order_bit_len == (u32)256;
*/
static int parse_algoid_pubkey_params_x25519(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len != (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1471);
			goto out;
		}
	}
/*@ assert rte_299: mem_access: \valid(&ctx->spki_alg_params.x25519.curve); */
	ctx->spki_alg_params.x25519.curve = CURVE_WEI25519;
/*@ assert rte_300: mem_access: \valid(&ctx->spki_alg_params.x25519.curve_order_bit_len); */
	ctx->spki_alg_params.x25519.curve_order_bit_len = (u32)256;
	ret = 0;
	out: ;
	return ret;
}

static int parse_algoid_params_gost2012PublicKey(u8 const *cert, u32 off,
u32 len,
_curve const **curve,
_hash_alg const **hash)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	u32 oid_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (curve == (_curve const **)0) goto _LOR;
	else
	if (hash == (_hash_alg const **)0) {
		_LOR: {
			ret = - (2 * 100000 + 1542);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
/*@ assert rte_303: mem_access: \valid(curve); */
	*curve = find_curve_by_oid(buf,oid_len);
/*@ assert rte_304: mem_access: \valid_read(curve); */
	if (*curve == (_curve const *)0) {
		ret = - (2 * 100000 + 1582);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	if (remain) {
		ret = parse_OID(buf,remain,& oid_len);
		if (ret) goto out;
	/*@ assert rte_305: mem_access: \valid(hash); */
		*hash = find_hash_by_oid(buf,oid_len);
	/*@ assert rte_306: mem_access: \valid_read(hash); */
		if (*hash == (_hash_alg const *)0) {
			ret = - (2 * 100000 + 1633);
			goto out;
		}
		buf += oid_len;
		remain -= oid_len;
		if (remain) {
			ret = - (2 * 100000 + 1642);
			goto out;
		}
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_ea_rsa(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	u32 bit_len = (u32)0;
	u32 parsed = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (2 * 100000 + 1696);
			goto out;
		}
	}
	switch (len) {
		case (u32)0: ret = 0;
		break;
		case (u32)2: ret = parse_null(buf,len,& parsed);
		if (ret) goto out;
		break;
		case (u32)4:
		ret = parse_non_negative_integer(buf,len,CLASS_UNIVERSAL,
		(u32)ASN1_TYPE_INTEGER,& hdr_len,
		& data_len);
		if (ret) goto out;
	/*@ assert rte_346: mem_access: \valid_read(buf + 2); */
	/*@ assert rte_347: mem_access: \valid_read(buf + 3); */
		bit_len = (u32)*(buf + 2) * (u32)(1 << 8) + (u32)*(buf + 3);
		break;
		default: ret = - (2 * 100000 + 1741);
		break;
	}
/*@ assert rte_348: mem_access: \valid(&ctx->spki_alg_params.rsa.rsa_advertised_bit_len); */
	ctx->spki_alg_params.rsa.rsa_advertised_bit_len = bit_len;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_gost_r3410_94(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len)
{
	u32 remain;
	_gost94_pub_params const *params;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	u8 const *buf = cert + off;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 1801);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	params = find_gost94_params_by_oid(buf,oid_len);
	if (params == (_gost94_pub_params const *)0) {
		ret = - (2 * 100000 + 1828);
		goto out;
	}
/*@ assert rte_326: mem_access: \valid(&ctx->spki_alg_params.gost94.gost94_params_id); */
/*@ assert rte_327: mem_access: \valid_read(&params->params_id); */
	ctx->spki_alg_params.gost94.gost94_params_id = params->params_id;
	buf += oid_len;
	remain -= oid_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	buf += oid_len;
	remain -= oid_len;
	if (remain) {
		ret = parse_OID(buf,remain,& oid_len);
		if (ret) goto out;
		remain -= oid_len;
		if (remain) {
			ret = - (2 * 100000 + 1864);
			goto out;
		}
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_gost_r3410_2001(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len)
{
	u32 remain;
	_curve const *curve;
	_hash_alg const *h;
	int ret;
	u8 const *buf = cert + off;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 parsed = (u32)0;
	u32 oid_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (2 * 100000 + 1921);
			goto out;
		}
	}
/*@ assert rte_315: mem_access: \valid(&ctx->spki_alg_params.gost2001.curve_order_bit_len); */
	ctx->spki_alg_params.gost2001.curve_order_bit_len = (u32)0;
/*@ assert rte_316: mem_access: \valid(&ctx->spki_alg_params.gost2001.curve); */
	ctx->spki_alg_params.gost2001.curve = CURVE_UNKNOWN;
	if (len == (u32)0) {
		ret = 0;
		goto out;
	}
	if (len == (u32)2) {
		int tmp;
		tmp = parse_null(buf,len,& parsed);
		if (! tmp) {
			ret = 0;
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	curve = find_curve_by_oid(buf,oid_len);
	if (curve == (_curve const *)0) {
		ret = - (2 * 100000 + 1967);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	h = find_hash_by_oid(buf,oid_len);
	if (h == (_hash_alg const *)0) {
		ret = - (2 * 100000 + 1986);
		goto out;
	}
	buf += oid_len;
	remain -= oid_len;
	if (remain) {
		ret = parse_OID(buf,remain,& oid_len);
		if (ret) goto out;
		buf += oid_len;
		remain -= oid_len;
		if (remain) {
			ret = - (2 * 100000 + 2007);
			goto out;
		}
	}
/*@ assert rte_317: mem_access: \valid(&ctx->spki_alg_params.gost2001.curve_order_bit_len); */
/*@ assert rte_318: mem_access: \valid_read(&curve->crv_order_bit_len); */
	ctx->spki_alg_params.gost2001.curve_order_bit_len = curve->crv_order_bit_len;
/*@ assert rte_319: mem_access: \valid(&ctx->spki_alg_params.gost2001.curve); */
/*@ assert rte_320: mem_access: \valid_read(&curve->crv_id); */
	ctx->spki_alg_params.gost2001.curve = curve->crv_id;
	ret = 0;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_gost_r3410_2012_256(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len)
{
	int ret;
	_hash_alg const *hash = (_hash_alg const *)0;
	_curve const *curve = (_curve const *)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (2 * 100000 + 2050);
			goto out;
		}
	}
	ret = parse_algoid_params_gost2012PublicKey(cert,off,len,& curve,& hash);
	if (ret) goto out;
	if (curve == (_curve const *)0) {
		ret = - (2 * 100000 + 2062);
		goto out;
	}
/*@ assert rte_307: mem_access: \valid(&ctx->spki_alg_params.gost2012_256.curve_order_bit_len); */
/*@ assert rte_308: mem_access: \valid_read(&curve->crv_order_bit_len); */
	ctx->spki_alg_params.gost2012_256.curve_order_bit_len = curve->crv_order_bit_len;
/*@ assert rte_309: mem_access: \valid(&ctx->spki_alg_params.gost2012_256.curve); */
/*@ assert rte_310: mem_access: \valid_read(&curve->crv_id); */
	ctx->spki_alg_params.gost2012_256.curve = curve->crv_id;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_gost_r3410_2012_512(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len)
{
	int ret;
	_hash_alg const *hash = (_hash_alg const *)0;
	_curve const *curve = (_curve const *)0;
	if (ctx == (cert_parsing_ctx *)0) {
		ret = - (2 * 100000 + 2101);
		goto out;
	}
	ret = parse_algoid_params_gost2012PublicKey(cert,off,len,& curve,& hash);
	if (ret) goto out;
	if (curve == (_curve const *)0) {
		ret = - (2 * 100000 + 2113);
		goto out;
	}
/*@ assert rte_311: mem_access: \valid(&ctx->spki_alg_params.gost2012_512.curve_order_bit_len); */
/*@ assert rte_312: mem_access: \valid_read(&curve->crv_order_bit_len); */
	ctx->spki_alg_params.gost2012_512.curve_order_bit_len = curve->crv_order_bit_len;
/*@ assert rte_313: mem_access: \valid(&ctx->spki_alg_params.gost2012_256.curve); */
/*@ assert rte_314: mem_access: \valid_read(&curve->crv_id); */
	ctx->spki_alg_params.gost2012_256.curve = curve->crv_id;
	out: ;
	return ret;
}

/*@
	requires ctx == \null || \valid(ctx);
	requires cert == \null || len == 0 || \valid_read(cert + off + (0 .. len - 1));
	requires off <= UINT32_MAX - len;
	assigns \nothing;
	ensures ctx == \null ==> \result == -(2 * 100000 + 2145);
	ensures (ctx != \null && cert == \null) ==> \result == -(3 * 100000 + 7253);
	ensures (ctx != \null && cert != \null && len == 0) ==> \result == 0;
	ensures (ctx != \null && cert != \null && len == 2 &&(cert + off)[0] == 0x05 && (cert + off)[1] == 0x00) ==> \result == 0;
	ensures (ctx != \null && cert != \null && len == 2 &&!((cert + off)[0] == 0x05 && (cert + off)[1] == 0x00)) ==>\result == -(3 * 100000 + 1146);
	ensures (ctx != \null && cert != \null && len != 0 && len != 2) ==> \result == -1;
	ensures \result == 0 ||\result == -1 ||\result == -(2 * 100000 + 2145) ||\result == -(3 * 100000 + 7253) ||\result == -(3 * 100000 + 1146);
*/
static int parse_algoid_pubkey_params_none(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) {
		ret = - (2 * 100000 + 2145);
		goto out;
	}
	ret = parse_algoid_params_none(cert,off,len);
	if (ret) goto out;
	out: ;
	return ret;
}

/*@
	requires ctx == \null || \valid(ctx);
	requires cert == \null || \valid_read(cert + (0 .. off + len - 1));
	requires off <= UINT32_MAX - len;
	assigns \nothing;
	ensures (ctx == \null || cert == \null) ==> \result == -(2 * 100000 + 2179);
	ensures (ctx != \null && cert != \null && len == 0) ==> \result == 0;
	ensures (ctx != \null && cert != \null && len != 0 && len != 2) ==> \result == -(3 * 100000 + 1139);
	ensures (ctx != \null && cert != \null && len == 2 && (cert[off] != 0x05 || cert[off + 1] != 0x00)) ==> \result == -(3 * 100000 + 1146);
	ensures (ctx != \null && cert != \null && len == 2 && cert[off] == 0x05 && cert[off + 1] == 0x00) ==> \result == 0;
	ensures \result == 0 ||\result == -(2 * 100000 + 2179) ||\result == -(3 * 100000 + 1139) ||\result == -(3 * 100000 + 1146);
*/
static int parse_algoid_pubkey_params_rsa(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (2 * 100000 + 2179);
			goto out;
		}
	}
	ret = parse_algoid_params_rsa(cert,off,len);
	if (ret) goto out;
	out: ;
	return ret;
}

static int spki_rsa_export_n_e(u8 const *buf, u32 len, u32 *n_start_off,
u32 *n_len, u32 *e_start_off, u32 *e_len)
{
	u32 remain;
	u32 off;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 parsed = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (n_start_off == (u32 *)0) goto _LOR;
	else
	if (n_len == (u32 *)0) goto _LOR;
	else
	if (e_start_off == (u32 *)0) goto _LOR;
	else
	if (e_len == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 2239);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 2258);
		goto out;
	}
	buf += hdr_len;
	off = hdr_len;
/*@ assert rte_393: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 2272);
		goto out;
	}
	buf ++;
	remain = data_len - (u32)1;
	off += (u32)1;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	off += hdr_len;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
/*@ assert rte_394: mem_access: \valid(n_start_off); */
	*n_start_off = off + hdr_len;
/*@ assert rte_395: mem_access: \valid(n_len); */
	*n_len = data_len;
	if (data_len != (u32)0)
/*@ assert rte_396: mem_access: \valid_read(buf + hdr_len); */
	if ((int)*(buf + hdr_len) == 0) {
	/*@ assert rte_397: mem_access: \valid(n_start_off); */
	/*@ assert rte_398: mem_access: \valid_read(n_start_off); */
		*n_start_off += (u32)1;
	/*@ assert rte_399: mem_access: \valid(n_len); */
	/*@ assert rte_400: mem_access: \valid_read(n_len); */
		*n_len -= (u32)1;
	}
	buf += parsed;
	off += parsed;
	remain -= parsed;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
/*@ assert rte_401: mem_access: \valid(e_start_off); */
	*e_start_off = off + hdr_len;
/*@ assert rte_402: mem_access: \valid(e_len); */
	*e_len = data_len;
	buf += parsed;
	off += parsed;
	remain -= parsed;
	if (remain != (u32)0) {
		ret = - (2 * 100000 + 2346);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_pubkey_rsa(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len)
{
	int ret;
	u32 n_start_off = (u32)0;
	u32 n_len = (u32)0;
	u32 e_start_off = (u32)0;
	u32 e_len = (u32)0;
	u8 const *buf = cert + off;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 2380);
			goto out;
		}
	}
	ret = spki_rsa_export_n_e(buf,len,& n_start_off,& n_len,& e_start_off,
	& e_len);
	if (ret) goto out;
/*@ assert rte_403: mem_access: \valid(&ctx->spki_alg_params.rsa.rsa_raw_modulus_off); */
	ctx->spki_alg_params.rsa.rsa_raw_modulus_off = off + n_start_off;
/*@ assert rte_404: mem_access: \valid(&ctx->spki_alg_params.rsa.rsa_raw_modulus_len); */
	ctx->spki_alg_params.rsa.rsa_raw_modulus_len = n_len;
/*@ assert rte_405: mem_access: \valid(&ctx->spki_alg_params.rsa.rsa_raw_pub_exp_off); */
	ctx->spki_alg_params.rsa.rsa_raw_pub_exp_off = off + e_start_off;
/*@ assert rte_406: mem_access: \valid(&ctx->spki_alg_params.rsa.rsa_raw_pub_exp_len); */
	ctx->spki_alg_params.rsa.rsa_raw_pub_exp_len = e_len;
	out: ;
	return ret;
}

int parse_algoid_dsa_export_params(u8 const *buf, u32 len, u32 *p_start_off,
u32 *p_len, u32 *q_start_off, u32 *q_len,
u32 *g_start_off, u32 *g_len)
{
	int ret;
	u32 remain = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 eaten = (u32)0;
	u32 parsed = (u32)0;
	u32 off = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (p_start_off == (u32 *)0) goto _LOR;
	else
	if (p_len == (u32 *)0) goto _LOR;
	else
	if (q_start_off == (u32 *)0) goto _LOR;
	else
	if (q_len == (u32 *)0) goto _LOR;
	else
	if (g_start_off == (u32 *)0) goto _LOR;
	else
	if (g_len == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 2472);
			goto out;
		}
	}
/*@ assert rte_328: mem_access: \valid(p_start_off); */
	*p_start_off = (u32)0;
/*@ assert rte_329: mem_access: \valid(p_len); */
	*p_len = (u32)0;
/*@ assert rte_330: mem_access: \valid(q_start_off); */
	*q_start_off = (u32)0;
/*@ assert rte_331: mem_access: \valid(q_len); */
	*q_len = (u32)0;
/*@ assert rte_332: mem_access: \valid(g_start_off); */
	*g_start_off = (u32)0;
/*@ assert rte_333: mem_access: \valid(g_len); */
	*g_len = (u32)0;
	if (len == (u32)0) {
		ret = 0;
		goto out;
	}
	ret = parse_null(buf,len,& parsed);
	if (! ret) {
		ret = 0;
		goto out;
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	eaten = hdr_len + data_len;
/*@ assert rte_334: mem_access: \valid(p_start_off); */
	*p_start_off = off + hdr_len;
/*@ assert rte_335: mem_access: \valid(p_len); */
	*p_len = data_len;
	remain -= eaten;
	buf += eaten;
	off += eaten;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	eaten = hdr_len + data_len;
/*@ assert rte_336: mem_access: \valid(q_start_off); */
	*q_start_off = off + hdr_len;
/*@ assert rte_337: mem_access: \valid(q_len); */
	*q_len = data_len;
	remain -= eaten;
	buf += eaten;
	off += eaten;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	eaten = hdr_len + data_len;
/*@ assert rte_338: mem_access: \valid(g_start_off); */
	*g_start_off = off + hdr_len;
/*@ assert rte_339: mem_access: \valid(g_len); */
	*g_len = data_len;
	remain -= eaten;
	if (remain) {
		ret = - (2 * 100000 + 2570);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_algoid_pubkey_params_dsa(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len)
{
	u32 p_start_off;
	u32 p_len;
	u32 q_start_off;
	u32 q_len;
	u32 g_start_off;
	u32 g_len;
	int ret;
	u8 const *buf = cert + off;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 2611);
			goto out;
		}
	}
	ret = parse_algoid_dsa_export_params(buf,len,& p_start_off,& p_len,
	& q_start_off,& q_len,& g_start_off,
	& g_len);
	if (ret) goto out;
/*@ assert rte_340: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_p_off); */
	ctx->spki_alg_params.dsa.dsa_raw_p_off = off + p_start_off;
/*@ assert rte_341: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_p_len); */
	ctx->spki_alg_params.dsa.dsa_raw_p_len = p_len;
/*@ assert rte_342: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_q_off); */
	ctx->spki_alg_params.dsa.dsa_raw_q_off = off + q_start_off;
/*@ assert rte_343: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_q_len); */
	ctx->spki_alg_params.dsa.dsa_raw_q_len = q_len;
/*@ assert rte_344: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_g_off); */
	ctx->spki_alg_params.dsa.dsa_raw_g_off = off + g_start_off;
/*@ assert rte_345: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_g_len); */
	ctx->spki_alg_params.dsa.dsa_raw_g_len = g_len;
	out: ;
	return ret;
}

int parse_pubkey_dsa_export_pub(u8 const *buf, u32 len, u32 *pub_start_off,
u32 *pub_len)
{
	u32 remain;
	u32 off;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (pub_start_off == (u32 *)0) goto _LOR;
	else
	if (pub_len == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 2663);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 2682);
		goto out;
	}
	buf += hdr_len;
	off = hdr_len;
/*@ assert rte_416: mem_access: \valid_read(buf + 0); */
	if ((int)*(buf + 0) != 0) {
		ret = - (2 * 100000 + 2696);
		goto out;
	}
	buf ++;
	remain = data_len - (u32)1;
	off += (u32)1;
	ret = parse_non_negative_integer(buf,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& hdr_len,
	& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	off += hdr_len;
/*@ assert rte_417: mem_access: \valid(pub_start_off); */
	*pub_start_off = off;
/*@ assert rte_418: mem_access: \valid(pub_len); */
	*pub_len = data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_pubkey_dsa(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len)
{
	int ret;
	u8 const *buf = cert + off;
	u32 pub_start_off = (u32)0;
	u32 pub_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) {
		_LOR: {
			ret = - (2 * 100000 + 2748);
			goto out;
		}
	}
	ret = parse_pubkey_dsa_export_pub(buf,len,& pub_start_off,& pub_len);
	if (ret) goto out;
/*@ assert rte_419: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_pub_off); */
	ctx->spki_alg_params.dsa.dsa_raw_pub_off = off + pub_start_off;
/*@ assert rte_420: mem_access: \valid(&ctx->spki_alg_params.dsa.dsa_raw_pub_len); */
	ctx->spki_alg_params.dsa.dsa_raw_pub_len = pub_len;
	out: ;
	return ret;
}

static int parse_x509_tbsCert_sig_AlgorithmIdentifier(cert_parsing_ctx *ctx,
u8 const *cert,
u32 off, u32 len,
_sig_alg const **alg,
u32 *eaten)
{
	u32 param_len;
	int ret;
	_sig_alg const *talg = (_sig_alg const *)0;
	u8 const *buf = cert + off;
	u32 saved_off = off;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 2840);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	parsed = hdr_len + data_len;
	buf += hdr_len;
	off += hdr_len;
	ret = parse_OID(buf,data_len,& oid_len);
	if (ret) goto out;
	talg = find_sig_alg_by_oid(buf,oid_len);
	if (talg == (_sig_alg const *)0) {
		ret = - (2 * 100000 + 2870);
		goto out;
	}
/*@ assert rte_262: mem_access: \valid(&ctx->tbs_sig_alg_oid_start); */
	ctx->tbs_sig_alg_oid_start = off;
/*@ assert rte_263: mem_access: \valid(&ctx->tbs_sig_alg_oid_len); */
	ctx->tbs_sig_alg_oid_len = oid_len;
/*@ assert rte_264: mem_access: \valid(&ctx->sig_alg); */
/*@ assert rte_265: mem_access: \valid_read(&talg->sig_id); */
	ctx->sig_alg = talg->sig_id;
/*@ assert rte_266: mem_access: \valid(&ctx->hash_alg); */
/*@ assert rte_267: mem_access: \valid_read(&talg->hash_id); */
	ctx->hash_alg = talg->hash_id;
	buf += oid_len;
	off += oid_len;
	param_len = data_len - oid_len;
/*@ assert rte_268: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_ecdsa_with)
	ret = parse_algoid_sig_params_ecdsa_with(& ctx->sig_alg_params,
	& ctx->hash_alg,cert,off,
	param_len);
	else
/*@ assert rte_269: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_ecdsa_with_specified)
	ret = parse_algoid_sig_params_ecdsa_with_specified(& ctx->sig_alg_params,
	& ctx->hash_alg,
	cert,off,param_len);
	else
/*@ assert rte_270: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_sm2)
	ret = parse_algoid_sig_params_sm2(& ctx->sig_alg_params,
	& ctx->hash_alg,cert,off,param_len);
	else
/*@ assert rte_271: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_eddsa)
	ret = parse_algoid_sig_params_eddsa(& ctx->sig_alg_params,
	& ctx->hash_alg,cert,off,
	param_len);
	else
/*@ assert rte_272: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_rsa)
	ret = parse_algoid_sig_params_rsa(& ctx->sig_alg_params,
	& ctx->hash_alg,cert,off,
	param_len);
	else
/*@ assert rte_273: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_rsassa_pss)
	ret = parse_algoid_sig_params_rsassa_pss(& ctx->sig_alg_params,
	& ctx->hash_alg,cert,
	off,param_len);
	else
/*@ assert rte_274: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_none)
	ret = parse_algoid_sig_params_none(& ctx->sig_alg_params,
	& ctx->hash_alg,cert,off,
	param_len);
	else
/*@ assert rte_275: mem_access: \valid_read(&talg->parse_algoid_sig_params); */
	if (talg->parse_algoid_sig_params == & parse_algoid_sig_params_bign_with_hspec)
	ret = parse_algoid_sig_params_bign_with_hspec(& ctx->sig_alg_params,
	& ctx->hash_alg,
	cert,off,
	param_len);
	if (ret) goto out;
/*@ assert rte_276: mem_access: \valid(&ctx->tbs_sig_alg_oid_params_start); */
	ctx->tbs_sig_alg_oid_params_start = off;
/*@ assert rte_277: mem_access: \valid(&ctx->tbs_sig_alg_oid_params_len); */
	ctx->tbs_sig_alg_oid_params_len = param_len;
/*@ assert rte_278: mem_access: \valid(alg); */
	*alg = talg;
/*@ assert rte_279: mem_access: \valid(eaten); */
	*eaten = parsed;
/*@ assert rte_280: mem_access: \valid(&ctx->tbs_sig_alg_start); */
	ctx->tbs_sig_alg_start = saved_off;
/*@ assert rte_281: mem_access: \valid(&ctx->tbs_sig_alg_len); */
	ctx->tbs_sig_alg_len = parsed;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_pubkey_AlgorithmIdentifier(cert_parsing_ctx *ctx,
u8 const *cert, u32 off,
u32 len,
_pubkey_alg const **alg,
u32 *eaten)
{
	u32 param_len;
	int ret;
	_pubkey_alg const *talg = (_pubkey_alg const *)0;
	u8 const *buf = cert + off;
	u32 saved_off = off;
	u32 parsed = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 2979);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	ret = parse_OID(buf,data_len,& oid_len);
	if (ret) goto out;
/*@ assert rte_353: mem_access: \valid(&ctx->spki_alg_oid_start); */
	ctx->spki_alg_oid_start = off;
/*@ assert rte_354: mem_access: \valid(&ctx->spki_alg_oid_len); */
	ctx->spki_alg_oid_len = oid_len;
	talg = find_pubkey_alg_by_oid(buf,oid_len);
	if (talg == (_pubkey_alg const *)0) {
		ret = - (2 * 100000 + 3012);
		goto out;
	}
	buf += oid_len;
	off += oid_len;
	param_len = data_len - oid_len;
/*@ assert rte_355: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_ecPublicKey)
	ret = parse_algoid_pubkey_params_ecPublicKey(ctx,cert,off,param_len);
	else
/*@ assert rte_356: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_ed25519)
	ret = parse_algoid_pubkey_params_ed25519(ctx,cert,off,param_len);
	else
/*@ assert rte_357: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_ed448)
	ret = parse_algoid_pubkey_params_ed448(ctx,cert,off,param_len);
	else
/*@ assert rte_358: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_x25519)
	ret = parse_algoid_pubkey_params_x25519(ctx,cert,off,param_len);
	else
/*@ assert rte_359: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_x448)
	ret = parse_algoid_pubkey_params_x448(ctx,cert,off,param_len);
	else
/*@ assert rte_360: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_rsa)
	ret = parse_algoid_pubkey_params_rsa(ctx,cert,off,param_len);
	else
/*@ assert rte_361: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_gost_r3410_2012_256)
	ret = parse_algoid_pubkey_params_gost_r3410_2012_256(ctx,
	cert,off,
	param_len);
	else
/*@ assert rte_362: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_gost_r3410_2012_512)
	ret = parse_algoid_pubkey_params_gost_r3410_2012_512
	(ctx,cert,off,param_len);
	else
/*@ assert rte_363: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_gost_r3410_2001)
	ret = parse_algoid_pubkey_params_gost_r3410_2001(ctx,
	cert,off,
	param_len);
	else
/*@ assert rte_364: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_gost_r3410_94)
	ret = parse_algoid_pubkey_params_gost_r3410_94(ctx,
	cert,off,
	param_len);
	else
/*@ assert rte_365: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_dsa)
	ret = parse_algoid_pubkey_params_dsa(ctx,cert,off,
	param_len);
	else
/*@ assert rte_366: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_ea_rsa)
	ret = parse_algoid_pubkey_params_ea_rsa(ctx,cert,
	off,
	param_len);
	else
/*@ assert rte_367: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_none)
	ret = parse_algoid_pubkey_params_none(ctx,cert,
	off,
	param_len);
	else
/*@ assert rte_368: mem_access: \valid_read(&talg->parse_algoid_pubkey_params); */
	if (talg->parse_algoid_pubkey_params == & parse_algoid_pubkey_params_bign)
	ret = parse_algoid_pubkey_params_bign(ctx,cert,
	off,
	param_len);
	if (ret) goto out;
/*@ assert rte_369: mem_access: \valid(&ctx->spki_alg_oid_params_start); */
	ctx->spki_alg_oid_params_start = off;
/*@ assert rte_370: mem_access: \valid(&ctx->spki_alg_oid_params_len); */
	ctx->spki_alg_oid_params_len = param_len;
	parsed = hdr_len + data_len;
/*@ assert rte_371: mem_access: \valid(alg); */
	*alg = talg;
/*@ assert rte_372: mem_access: \valid(eaten); */
	*eaten = parsed;
/*@ assert rte_373: mem_access: \valid(&ctx->spki_alg); */
/*@ assert rte_374: mem_access: \valid_read(&talg->pubkey_id); */
	ctx->spki_alg = talg->pubkey_id;
/*@ assert rte_375: mem_access: \valid(&ctx->spki_alg_oid_start); */
	ctx->spki_alg_oid_start = saved_off;
/*@ assert rte_376: mem_access: \valid(&ctx->spki_alg_oid_len); */
	ctx->spki_alg_oid_len = parsed;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_Validity(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, u32 *eaten)
{
	int ret;
	u64 not_after;
	u64 not_before;
	u8 const *buf = cert + off;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 data_len = (u32)0;
	u32 nb_len = (u32)0;
	u32 na_len = (u32)0;
	u16 na_year = (u16)0;
	u16 nb_year = (u16)0;
	u8 na_month = (u8)0;
	u8 na_day = (u8)0;
	u8 na_hour = (u8)0;
	u8 na_min = (u8)0;
	u8 na_sec = (u8)0;
	u8 nb_month = (u8)0;
	u8 nb_day = (u8)0;
	u8 nb_hour = (u8)0;
	u8 nb_min = (u8)0;
	u8 nb_sec = (u8)0;
	u8 t_type = (u8)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 3138);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_Time(buf,remain,& t_type,& nb_len,& nb_year,& nb_month,
	& nb_day,& nb_hour,& nb_min,& nb_sec);
	if (ret) goto out;
	ret = verify_correct_time_use(t_type,nb_year);
	if (ret) goto out;
	remain -= nb_len;
	buf += nb_len;
	ret = parse_Time(buf,remain,& t_type,& na_len,& na_year,& na_month,
	& na_day,& na_hour,& na_min,& na_sec);
	if (ret) goto out;
	ret = verify_correct_time_use(t_type,na_year);
	if (ret) goto out;
	remain -= na_len;
	if (remain) {
		ret = - (2 * 100000 + 3191);
		goto out;
	}
	not_after = time_components_to_comparable_u64(na_year,na_month,na_day,
	na_hour,na_min,na_sec);
	not_before = time_components_to_comparable_u64(nb_year,nb_month,nb_day,
	nb_hour,nb_min,nb_sec);
	if (not_before >= not_after) {
		ret = - (2 * 100000 + 3222);
		goto out;
	}
/*@ assert rte_282: mem_access: \valid(&ctx->not_before); */
	ctx->not_before = not_before;
/*@ assert rte_283: mem_access: \valid(&ctx->not_after); */
	ctx->not_after = not_after;
/*@ assert rte_284: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_subjectPublicKeyInfo(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 parsed = (u32)0;
	u32 remain = (u32)0;
	u32 saved_off = off;
	u8 const *buf = cert + off;
	_pubkey_alg const *alg = (_pubkey_alg const *)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 3270);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	off += hdr_len;
	ret = parse_x509_pubkey_AlgorithmIdentifier(ctx,cert,off,remain,& alg,
	& parsed);
	if (ret) goto out;
	buf += parsed;
	remain -= parsed;
	off += parsed;
/*@ assert rte_427: mem_access: \valid_read(&alg->parse_pubkey); */
	if (! alg->parse_pubkey) {
		ret = - (2 * 100000 + 3310);
		goto out;
	}
/*@ assert rte_428: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_ed448) ret = parse_pubkey_ed448
	(ctx,cert,off,remain);
	else
/*@ assert rte_429: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_x448) ret = parse_pubkey_x448
	(ctx,cert,off,remain);
	else
/*@ assert rte_430: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_ed25519) ret = parse_pubkey_ed25519
	(ctx,cert,off,remain);
	else
/*@ assert rte_431: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_x25519) ret = parse_pubkey_x25519
	(ctx,cert,off,remain);
	else
/*@ assert rte_432: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_ec) ret = parse_pubkey_ec
	(ctx,cert,off,remain);
	else
/*@ assert rte_433: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_rsa) ret = parse_pubkey_rsa
	(ctx,cert,off,
	remain);
	else
/*@ assert rte_434: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_gostr3410_94) ret = parse_pubkey_gostr3410_94
	(ctx,
	cert,off,
	remain);
	else
/*@ assert rte_435: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_gostr3410_2001)
	ret = parse_pubkey_gostr3410_2001(ctx,cert,off,remain);
	else
/*@ assert rte_436: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_gostr3410_2012_256)
	ret = parse_pubkey_gostr3410_2012_256(ctx,cert,off,
	remain);
	else
/*@ assert rte_437: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_gostr3410_2012_512)
	ret = parse_pubkey_gostr3410_2012_512(ctx,cert,off,
	remain);
	else
/*@ assert rte_438: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_dsa) ret = parse_pubkey_dsa
	(ctx,cert,
	off,
	remain);
	else
/*@ assert rte_439: mem_access: \valid_read(&alg->parse_pubkey); */
	if (alg->parse_pubkey == & parse_pubkey_bign)
	ret = parse_pubkey_bign(ctx,cert,off,remain);
	if (ret) goto out;
/*@ assert rte_440: mem_access: \valid(&ctx->spki_pub_key_start); */
	ctx->spki_pub_key_start = off;
/*@ assert rte_441: mem_access: \valid(&ctx->spki_pub_key_len); */
	ctx->spki_pub_key_len = remain;
/*@ assert rte_442: mem_access: \valid(&ctx->spki_start); */
	ctx->spki_start = saved_off;
/*@ assert rte_443: mem_access: \valid(&ctx->spki_len); */
	ctx->spki_len = hdr_len + data_len;
/*@ assert rte_444: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_AIA(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	int ret;
	if (ctx == (cert_parsing_ctx *)0) {
		ret = - (2 * 100000 + 3467);
		goto out;
	}
	ret = parse_AIA(cert,off,len,critical);
	if (ret) {
		ret = - (2 * 100000 + 3474);
		goto out;
	}
	out: ;
	return ret;
}

static int parse_ext_AKI(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	u32 key_id_hdr_len = (u32)0;
	u32 key_id_data_len = (u32)0;
	u32 key_id_data_off = (u32)0;
	u32 gen_names_off = (u32)0;
	u32 gen_names_len = (u32)0;
	u32 cert_serial_off = (u32)0;
	u32 cert_serial_len = (u32)0;
	u32 parsed = (u32)0;
	int has_keyIdentifier = 0;
	int has_gen_names_and_serial = 0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 3536);
			goto out;
		}
	}
	if (critical) {
		ret = - (2 * 100000 + 3548);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 3566);
		goto out;
	}
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,
	& key_id_hdr_len,& key_id_data_len);
	if (! ret) {
		if (! key_id_data_len) {
			ret = - (2 * 100000 + 3584);
			goto out;
		}
		key_id_data_off = off + key_id_hdr_len;
		buf += key_id_hdr_len + key_id_data_len;
		off += key_id_hdr_len + key_id_data_len;
		remain -= key_id_hdr_len + key_id_data_len;
		has_keyIdentifier = 1;
	}
	ret = parse_GeneralNames(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)1,& parsed);
	if (! ret) {
		gen_names_off = off;
		gen_names_len = parsed;
		buf += parsed;
		off += parsed;
		remain -= parsed;
		ret = parse_AKICertSerialNumber(cert,off,remain,CLASS_CONTEXT_SPECIFIC,
		(u32)2,& cert_serial_len);
		if (ret) goto out;
		has_gen_names_and_serial = 1;
		cert_serial_off = off;
		buf += cert_serial_len;
		off += cert_serial_len;
		remain -= cert_serial_len;
	}
	if (remain) {
		ret = - (2 * 100000 + 3632);
		goto out;
	}
/*@ assert rte_453: mem_access: \valid(&ctx->aki_has_keyIdentifier); */
	ctx->aki_has_keyIdentifier = has_keyIdentifier;
/*@ assert rte_454: mem_access: \valid_read(&ctx->aki_has_keyIdentifier); */
	if (ctx->aki_has_keyIdentifier) {
	/*@ assert rte_455: mem_access: \valid(&ctx->aki_keyIdentifier_start); */
		ctx->aki_keyIdentifier_start = key_id_data_off;
	/*@ assert rte_456: mem_access: \valid(&ctx->aki_keyIdentifier_len); */
		ctx->aki_keyIdentifier_len = key_id_data_len;
	}
/*@ assert rte_457: mem_access: \valid(&ctx->aki_has_generalNames_and_serial); */
	ctx->aki_has_generalNames_and_serial = has_gen_names_and_serial;
/*@ assert rte_458: mem_access: \valid_read(&ctx->aki_has_generalNames_and_serial); */
	if (ctx->aki_has_generalNames_and_serial) {
	/*@ assert rte_459: mem_access: \valid(&ctx->aki_generalNames_start); */
		ctx->aki_generalNames_start = gen_names_off;
	/*@ assert rte_460: mem_access: \valid(&ctx->aki_generalNames_len); */
		ctx->aki_generalNames_len = gen_names_len;
	/*@ assert rte_461: mem_access: \valid(&ctx->aki_serial_start); */
		ctx->aki_serial_start = cert_serial_off + (u32)2;
	/*@ assert rte_462: mem_access: \valid(&ctx->aki_serial_len); */
		ctx->aki_serial_len = cert_serial_len - (u32)2;
	}
/*@ assert rte_463: mem_access: \valid(&ctx->has_aki); */
	ctx->has_aki = 1;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_SKI(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	u32 remain;
	int ret;
	u32 key_id_hdr_len = (u32)0;
	u32 key_id_data_len = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 3696);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_OCTET_STRING,
	& key_id_hdr_len,& key_id_data_len);
	if (ret) goto out;
	if (len != key_id_hdr_len + key_id_data_len) {
		ret = - (2 * 100000 + 3727);
		goto out;
	}
	if (! key_id_data_len) {
		ret = - (2 * 100000 + 3734);
		goto out;
	}
	remain -= key_id_hdr_len + key_id_data_len;
	if (remain) {
		ret = - (2 * 100000 + 3741);
		goto out;
	}
/*@ assert rte_464: mem_access: \valid(&ctx->has_ski); */
	ctx->has_ski = 1;
/*@ assert rte_465: mem_access: \valid(&ctx->ski_start); */
	ctx->ski_start = off + key_id_hdr_len;
/*@ assert rte_466: mem_access: \valid(&ctx->ski_len); */
	ctx->ski_len = key_id_data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_keyUsage(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u16 val = (u16)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 3815);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_BIT_STRING,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 3836);
		goto out;
	}
	buf += hdr_len;
	len -= hdr_len;
	ret = parse_nine_bit_named_bit_list(buf,data_len,& val);
	if (ret) goto out;
	if ((int)val & 0x0100)
	if (! ((int)val & 0x0010)) {
		ret = - (2 * 100000 + 3861);
		goto out;
	}
	if ((int)val & 0x0080)
	if (! ((int)val & 0x0010)) {
		ret = - (2 * 100000 + 3872);
		goto out;
	}
/*@ assert rte_467: mem_access: \valid(&ctx->has_keyUsage); */
	ctx->has_keyUsage = 1;
/*@ assert rte_468: mem_access: \valid(&ctx->keyCertSign_set); */
	ctx->keyCertSign_set = ! (! ((int)val & 0x0020));
/*@ assert rte_469: mem_access: \valid(&ctx->cRLSign_set); */
	ctx->cRLSign_set = ! (! ((int)val & 0x0040));
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires (buf == \null) || \valid_read(buf + (0..len-1));
	requires (buf != \null && len != 0) ==> \valid(eaten);
	assigns *eaten;
	ensures (buf == \null || len == 0) ==> \result == -(2 * 100000 + 3905);
	ensures (buf != \null && len != 0 && \result == 0) ==> *eaten == len;
	ensures \result == 0 ||\result == -(2 * 100000 + 3905) ||\result == -(3 * 100000 + 3981) ||\result == -(3 * 100000 + 3987) ||\result == -(3 * 100000 + 3994) ||\result == -(3 * 100000 + 4001) ||\result == -(3 * 100000 + 4008) ||\result == -(3 * 100000 + 2263);
*/
static int parse_CPSuri(u8 const *buf, u32 len, u32 *eaten)
{
	int ret;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 3905);
			goto out;
		}
	}
	ret = parse_ia5_string(buf,len,(u32)1,(u32)65534);
	if (ret) goto out;
/*@ assert rte_470: mem_access: \valid(eaten); */
	*eaten = len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_NoticeReference(u8 const *buf, u32 len, u32 *eaten)
{
	u32 remain;
	int ret;
	u32 parsed = (u32)0;
	u32 saved_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 3955);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	saved_len = hdr_len + data_len;
	remain = data_len;
	buf += hdr_len;
	ret = parse_DisplayText(buf,remain,& parsed);
	if (ret) goto out;
	remain -= parsed;
	buf += parsed;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	remain -= hdr_len;
	buf += hdr_len;
	if (remain != data_len) {
		ret = - (2 * 100000 + 4003);
		goto out;
	}
	while (remain) {
		ret = parse_integer(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_INTEGER,
		& hdr_len,& data_len);
		if (ret) goto out;
		parsed = hdr_len + data_len;
		remain -= parsed;
		buf += parsed;
	}
/*@ assert rte_471: mem_access: \valid(eaten); */
	*eaten = saved_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_UserNotice(u8 const *buf, u32 len, u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 remain = (u32)0;
	u32 parsed = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 4071);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	remain -= hdr_len;
	buf += hdr_len;
	if (! data_len) {
		ret = - (2 * 100000 + 4091);
		goto out;
	}
	ret = parse_NoticeReference(buf,remain,& parsed);
	if (! ret) {
		remain -= parsed;
		buf += parsed;
	}
	ret = parse_DisplayText(buf,remain,& parsed);
	if (! ret) {
		remain -= parsed;
		buf += parsed;
	}
	if (remain) {
		ret = - (2 * 100000 + 4111);
		goto out;
	}
/*@ assert rte_472: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_policyQualifierInfo(u8 const *buf, u32 len, u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 remain = (u32)0;
	u8 id_qt_cps_oid[10] =
	{(u8)0x06,
		(u8)0x08,
		(u8)0x2b,
		(u8)0x06,
		(u8)0x01,
		(u8)0x05,
		(u8)0x05,
		(u8)0x07,
		(u8)0x02,
	(u8)0x01};
	u8 id_qt_unotice_oid[10] =
	{(u8)0x06,
		(u8)0x08,
		(u8)0x2b,
		(u8)0x06,
		(u8)0x01,
		(u8)0x05,
		(u8)0x05,
		(u8)0x07,
		(u8)0x02,
	(u8)0x02};
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 4159);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	remain = data_len;
	buf += hdr_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	if ((unsigned long)oid_len == sizeof(id_qt_cps_oid)) {
		int tmp_0;
		tmp_0 = bufs_differ(buf,(u8 const *)(id_qt_cps_oid),oid_len);
		if (tmp_0) goto _LAND_0;
		else {
			u32 cpsuri_len = (u32)0;
			buf += oid_len;
			remain -= oid_len;
			ret = parse_CPSuri(buf,remain,& cpsuri_len);
			if (ret) goto out;
			remain -= cpsuri_len;
			buf += cpsuri_len;
		}
	}
	else {
		_LAND_0: ;
		if ((unsigned long)oid_len == sizeof(id_qt_unotice_oid)) {
			int tmp;
			tmp = bufs_differ(buf,(u8 const *)(id_qt_unotice_oid),oid_len);
			if (tmp) goto _LAND;
			else {
				u32 cpsunotice_len = (u32)0;
				buf += oid_len;
				remain -= oid_len;
				ret = parse_UserNotice(buf,remain,& cpsunotice_len);
				if (ret) goto out;
				remain -= cpsunotice_len;
				buf += cpsunotice_len;
			}
		}
		else {
			_LAND: {
				ret = - (2 * 100000 + 4218);
				goto out;
			}
		}
	}
/*@ assert rte_473: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_PolicyInformation(u8 const *buf, u32 len, u32 *eaten)
{
	u32 saved_pi_len;
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 4255);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	saved_pi_len = hdr_len + data_len;
	remain = data_len;
	buf += hdr_len;
	ret = parse_OID(buf,remain,& oid_len);
	if (ret) goto out;
	remain -= oid_len;
	buf += oid_len;
	if (remain) {
		ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
		& hdr_len,& data_len);
		if (ret) goto out;
		remain -= hdr_len;
		buf += hdr_len;
		if (remain != data_len) {
			ret = - (2 * 100000 + 4298);
			goto out;
		}
		while (remain) {
			u32 pqi_len = (u32)0;
			ret = parse_policyQualifierInfo(buf,remain,& pqi_len);
			if (ret) goto out;
			remain -= pqi_len;
			buf += pqi_len;
		}
	}
/*@ assert rte_474: mem_access: \valid(eaten); */
	*eaten = saved_pi_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_certPolicies(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u32 remain = (u32)0;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 eaten = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 4411);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 4446);
		goto out;
	}
	while (remain) {
		ret = parse_PolicyInformation(buf,remain,& eaten);
		if (ret) goto out;
		remain -= eaten;
		off += eaten;
		buf += eaten;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_policyMapping(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u32 remain = (u32)0;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 eaten = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 4510);
			goto out;
		}
	}
	if (! critical) {
		ret = - (2 * 100000 + 4522);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 4540);
		goto out;
	}
	while (remain) {
		ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
		& hdr_len,& data_len);
		if (ret) goto out;
		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;
		ret = parse_OID(buf,data_len,& eaten);
		if (ret) goto out;
		buf += eaten;
		off += eaten;
		remain -= eaten;
		data_len -= eaten;
		ret = parse_OID(buf,data_len,& eaten);
		if (ret) goto out;
		data_len -= eaten;
		if (data_len) {
			ret = - (2 * 100000 + 4590);
			goto out;
		}
		buf += eaten;
		off += eaten;
		remain -= eaten;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_SAN(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	int ret;
	int empty_gen_name;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 eaten = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 4658);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (! data_len) {
		ret = - (2 * 100000 + 4678);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 4688);
		goto out;
	}
	while (remain) {
		empty_gen_name = 0;
		ret = parse_GeneralName(buf,remain,& eaten,& empty_gen_name);
		if (ret) goto out;
		if (empty_gen_name) {
			ret = - (2 * 100000 + 4712);
			goto out;
		}
	/*@ assert rte_475: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) == 0x87)
		switch (eaten) {
			case (u32)6: break;
			case (u32)18: break;
			default: ret = - (2 * 100000 + 4735);
			goto out;
			break;
		}
		remain -= eaten;
		buf += eaten;
	}
/*@ assert rte_476: mem_access: \valid(&ctx->has_san); */
	ctx->has_san = 1;
/*@ assert rte_477: mem_access: \valid(&ctx->san_critical); */
	ctx->san_critical = critical;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_IAN(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	int ret;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 eaten = (u32)0;
	u8 const *buf = cert + off;
	int unused = 0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 4782);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	if (! data_len) {
		ret = - (2 * 100000 + 4817);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 4827);
		goto out;
	}
	while (remain) {
		ret = parse_GeneralName(buf,remain,& eaten,& unused);
		if (ret) goto out;
		remain -= eaten;
		off += eaten;
		buf += eaten;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_subjectDirAttr(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 remain = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 4896);
			goto out;
		}
	}
	if (critical) {
		ret = - (2 * 100000 + 4906);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 4924);
		goto out;
	}
	while (remain) {
		ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
		& hdr_len,& data_len);
		if (ret) goto out;
		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;
		ret = parse_OID(buf,data_len,& oid_len);
		if (ret) goto out;
		remain -= data_len;
		off += data_len;
		buf += data_len;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_basicConstraints(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
u8 const ca_true_wo_plc[3] = {(u8)0x01, (u8)0x01, (u8)0xff};
	u8 const ca_true_w_plc[5] =
{(u8)0x01, (u8)0x01, (u8)0xff, (u8)0x02, (u8)0x01};
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5004);
			goto out;
		}
	}
/*@ assert rte_478: mem_access: \valid(&ctx->bc_critical); */
	ctx->bc_critical = critical;
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 5026);
		goto out;
	}
	switch (data_len) {
		case (u32)0: ret = 0;
		break;
		case (u32)3: ret = bufs_differ(buf,ca_true_wo_plc,(u32)3);
		if (! ret) {
		/*@ assert rte_479: mem_access: \valid(&ctx->ca_true); */
			ctx->ca_true = 1;
			break;
		}
		{
		u8 const ca_false_explicit_wo_plc[3] = {(u8)0x01, (u8)0x01, (u8)0x00};
			ret = bufs_differ(buf,ca_false_explicit_wo_plc,(u32)3);
			if (! ret) break;
		}
		ret = - (2 * 100000 + 5081);
		goto out;
		break;
		case (u32)6: ret = bufs_differ(buf,ca_true_w_plc,(u32)5);
		if (ret) {
			ret = - (2 * 100000 + 5088);
			goto out;
		}
	/*@ assert rte_480: mem_access: \valid_read(buf + 5); */
		if ((int)*(buf + 5) & 0x80) {
			ret = - (2 * 100000 + 5100);
			goto out;
		}
	/*@ assert rte_481: mem_access: \valid(&ctx->ca_true); */
		ctx->ca_true = 1;
	/*@ assert rte_482: mem_access: \valid(&ctx->pathLenConstraint_set); */
		ctx->pathLenConstraint_set = 1;
		break;
		default: ret = - (2 * 100000 + 5108);
		break;
	}
	out: ;
	return ret;
}

static int parse_GeneralSubtrees(u8 const *buf, u32 len)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 grabbed = (u32)0;
	u32 data_len = (u32)0;
	int unused = 0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) {
		_LOR: {
			ret = - (2 * 100000 + 5145);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	ret = parse_GeneralName(buf,remain,& grabbed,& unused);
	if (ret) goto out;
	buf += grabbed;
	remain -= grabbed;
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& hdr_len,
	& data_len);
	if (! ret) {
		ret = - (2 * 100000 + 5184);
		goto out;
	}
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)1,& hdr_len,
	& data_len);
	if (! ret) {
		ret = - (2 * 100000 + 5193);
		goto out;
	}
	if (remain) {
		ret = - (2 * 100000 + 5200);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_nameConstraints(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u32 remain = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5233);
			goto out;
		}
	}
	if (! critical) {
		ret = - (2 * 100000 + 5245);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& remain);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	if (! remain) {
		ret = - (2 * 100000 + 5266);
		goto out;
	}
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& hdr_len,
	& data_len);
	if (! ret) {
		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;
		ret = parse_GeneralSubtrees(buf,data_len);
		if (ret) goto out;
		buf += data_len;
		off += data_len;
		remain -= data_len;
	}
	ret = parse_id_len(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)1,& hdr_len,
	& data_len);
	if (! ret) {
		buf += hdr_len;
		off += hdr_len;
		remain -= hdr_len;
		ret = parse_GeneralSubtrees(buf,data_len);
		if (ret) goto out;
		buf += data_len;
		off += data_len;
		remain -= data_len;
	}
	if (remain) {
		ret = - (2 * 100000 + 5310);
		goto out;
	}
/*@ assert rte_483: mem_access: \valid(&ctx->has_name_constraints); */
	ctx->has_name_constraints = 1;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_policyConstraints(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 parsed = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5357);
			goto out;
		}
	}
	if (! critical) {
		ret = - (2 * 100000 + 5369);
		goto out;
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) {
		ret = - (2 * 100000 + 5378);
		goto out;
	}
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 5388);
		goto out;
	}
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	ret = parse_integer(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)0,& hdr_len,
	& data_len);
	if (! ret) {
		parsed = hdr_len + data_len;
		if (parsed != (u32)3) {
			ret = - (2 * 100000 + 5408);
			goto out;
		}
		buf += parsed;
		off += parsed;
		remain -= parsed;
	}
	ret = parse_integer(buf,remain,CLASS_CONTEXT_SPECIFIC,(u32)1,& hdr_len,
	& data_len);
	if (! ret) {
		parsed = hdr_len + data_len;
		if (parsed != (u32)3) {
			ret = - (2 * 100000 + 5429);
			goto out;
		}
		buf += parsed;
		off += parsed;
		remain -= parsed;
	}
	if (remain) {
		ret = - (2 * 100000 + 5440);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static u8 const _id_kp_anyEKU[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x00};

static u8 const _id_kp_serverAuth[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x01};

static u8 const _id_kp_clientAuth[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x02};

static u8 const _id_kp_codeSigning[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x03};

static u8 const _id_kp_emailProt[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x04};

static u8 const _id_kp_timeStamping[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x08};

static u8 const _id_kp_OCSPSigning[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x03,
(u8)0x09};

static u8 const _id_kp_ns_SGC[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x86,
	(u8)0xF8,
	(u8)0x42,
	(u8)0x04,
(u8)0x01};

static u8 const _id_kp_ms_SGC[12] =
{(u8)0x06,
	(u8)0x0A,
	(u8)0x2B,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x0A,
	(u8)0x03,
(u8)0x03};

static _kp_oid const known_kp_oids[9] =
{{.oid = _id_kp_anyEKU, .oid_len = (u8)sizeof(_id_kp_anyEKU)},
{.oid = _id_kp_serverAuth, .oid_len = (u8)sizeof(_id_kp_serverAuth)},
{.oid = _id_kp_clientAuth, .oid_len = (u8)sizeof(_id_kp_clientAuth)},
{.oid = _id_kp_codeSigning, .oid_len = (u8)sizeof(_id_kp_codeSigning)},
{.oid = _id_kp_emailProt, .oid_len = (u8)sizeof(_id_kp_emailProt)},
{.oid = _id_kp_timeStamping, .oid_len = (u8)sizeof(_id_kp_timeStamping)},
{.oid = _id_kp_OCSPSigning, .oid_len = (u8)sizeof(_id_kp_OCSPSigning)},
{.oid = _id_kp_ns_SGC, .oid_len = (u8)sizeof(_id_kp_ns_SGC)},
{.oid = _id_kp_ms_SGC, .oid_len = (u8)sizeof(_id_kp_ms_SGC)}};

u16 const num_known_kp_oids =
(u16)(sizeof(known_kp_oids) / sizeof(known_kp_oids[0]));

/*@
	requires buf == \null || \valid_read(buf + (0 .. len-1));
	requires \valid_read(&known_kp_oids[0 .. num_known_kp_oids-1]);
	requires \forall integer i; 0 <= i < num_known_kp_oids ==>\valid_read(&known_kp_oids[i].oid_len) &&\valid_read(&known_kp_oids[i].oid) &&(known_kp_oids[i].oid_len == 0 ||\valid_read(known_kp_oids[i].oid + (0 .. known_kp_oids[i].oid_len - 1)));
	assigns \nothing;
	ensures buf == \null ==> \result == \null;
	ensures len == 0 ==> \result == \null;
	ensures \result == \null ||(\exists integer k; 0 <= k < num_known_kp_oids &&\result == &known_kp_oids[k]);
	ensures \result != \null ==>(u32)\result->oid_len == len &&(\forall integer j; 0 <= j < len ==> \result->oid[j] == buf[j]);
	ensures \result == \null && buf != \null && len != 0 ==>(\forall integer k; 0 <= k < num_known_kp_oids ==>((u32)known_kp_oids[k].oid_len != len ||(\exists integer j; 0 <= j < (u32)known_kp_oids[k].oid_len &&known_kp_oids[k].oid[j] != buf[j])));
*/
static _kp_oid const *find_kp_by_oid(u8 const *buf, u32 len)
{
	u8 k;
	_kp_oid const *found = (_kp_oid const *)0;
	_kp_oid const *cur = (_kp_oid const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u8)0;
	/*@
		loop invariant 0 <= k <= num_known_kp_oids;
		loop invariant found == (_kp_oid const *)0;
		loop invariant \forall integer j; 0 <= j < k ==>((u32)known_kp_oids[j].oid_len != len ||bufs_differ(known_kp_oids[j].oid, buf, (u32)known_kp_oids[j].oid_len) != 0);
		loop assigns k, cur;
		loop variant (int)num_known_kp_oids - (int)k;
	*/
	while ((int)k < (int)num_known_kp_oids) {
		{
			int ret;
			int tmp_0;
			int tmp;
			cur = & known_kp_oids[k];
		/*@ assert rte_484: mem_access: \valid_read(&cur->oid_len); */
			if ((u32)cur->oid_len != len) goto __Cont;
		/*@ assert rte_485: mem_access: \valid_read(&cur->oid); */
		/*@ assert rte_486: mem_access: \valid_read(&cur->oid_len); */
			tmp = bufs_differ(cur->oid,buf,(u32)cur->oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_487: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u8)((int)k + 1);
	}
	out: ;
	return found;
}

static int parse_ext_EKU(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	int ret;
	u32 remain = (u32)0;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 oid_len = (u32)0;
	u8 const *buf = cert + off;
	_kp_oid const *kp = (_kp_oid const *)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5597);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto _LOR_0;
	else
	if (data_len == (u32)0) {
		_LOR_0: {
			ret = - (2 * 100000 + 5607);
			goto out;
		}
	}
	buf += hdr_len;
	off += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 5617);
		goto out;
	}
	while (remain) {
		ret = parse_OID(buf,remain,& oid_len);
		if (ret) goto out;
		kp = find_kp_by_oid(buf,oid_len);
		if (kp == (_kp_oid const *)0) {
			ret = - (2 * 100000 + 5638);
			goto out;
		}
	/*@ assert rte_488: mem_access: \valid_read(&kp->oid); */
		if (kp->oid == _id_kp_anyEKU)
		if (critical) {
			ret = - (2 * 100000 + 5649);
			goto out;
		}
		buf += oid_len;
		off += oid_len;
		remain -= oid_len;
	}
/*@ assert rte_489: mem_access: \valid(&ctx->has_eku); */
	ctx->has_eku = 1;
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_CRLDP(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	u32 remain;
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	u8 const *buf = cert + off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5703);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	remain = data_len;
	if (len != hdr_len + data_len) {
		ret = - (2 * 100000 + 5721);
		goto out;
	}
/*@ assert rte_490: mem_access: \valid(&ctx->has_crldp); */
	ctx->has_crldp = 1;
/*@ assert rte_491: mem_access: \valid(&ctx->one_crldp_has_all_reasons); */
	ctx->one_crldp_has_all_reasons = 0;
	while (remain) {
		int crldp_has_all_reasons = 0;
		u32 eaten = (u32)0;
		ret = parse_DistributionPoint(buf,remain,& crldp_has_all_reasons,& eaten);
		if (ret) goto out;
		if (crldp_has_all_reasons)
	/*@ assert rte_492: mem_access: \valid(&ctx->one_crldp_has_all_reasons); */
		ctx->one_crldp_has_all_reasons = 1;
		remain -= eaten;
		buf += eaten;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_ext_inhibitAnyPolicy(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, int critical)
{
	int ret;
	u8 const *buf = cert + off;
	u32 eaten = (u32)0;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5790);
			goto out;
		}
	}
	if (! critical) {
		ret = - (2 * 100000 + 5802);
		goto out;
	}
	ret = parse_integer(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_INTEGER,
	& hdr_len,& data_len);
	if (ret) {
		ret = - (2 * 100000 + 5810);
		goto out;
	}
	eaten = hdr_len + data_len;
	if (eaten != (u32)3) {
		ret = - (2 * 100000 + 5821);
		goto out;
	}
/*@ assert rte_493: mem_access: \valid_read(buf + 2); */
	if ((int)*(buf + 2) & 0x80) goto _LOR_0;
	else
/*@ assert rte_494: mem_access: \valid_read(buf + 2); */
	if ((int)*(buf + 2) > 64) {
		_LOR_0: {
			ret = - (2 * 100000 + 5827);
			goto out;
		}
	}
	if (eaten != len) {
		ret = - (2 * 100000 + 5833);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static u8 const _ext_oid_AIA[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x01,
(u8)0x01};

static u8 const _ext_oid_subjectDirAttr[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x09};

static u8 const _ext_oid_SKI[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x0e};

static u8 const _ext_oid_keyUsage[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x0f};

static u8 const _ext_oid_SAN[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x11};

static u8 const _ext_oid_IAN[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x12};

static u8 const _ext_oid_basicConstraints[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x13};

static u8 const _ext_oid_nameConstraints[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x1e};

static u8 const _ext_oid_CRLDP[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x1f};

static u8 const _ext_oid_certPolicies[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x20};

static u8 const _ext_oid_policyMapping[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x21};

static u8 const _ext_oid_AKI[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x23};

static u8 const _ext_oid_policyConstraints[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x24};

static u8 const _ext_oid_EKU[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x25};

static u8 const _ext_oid_FreshestCRL[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x2e};

static u8 const _ext_oid_inhibitAnyPolicy[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x36};

/*@
	requires (cert != \null && len > 0) ==> \valid_read(cert + (0 .. len - 1));
	requires (ctx != \null) ==> \valid(ctx);
	assigns \nothing;
*/
static int parse_ext_bad_oid(cert_parsing_ctx *ctx, u8 const *cert, u32 off,
u32 len, int critical)
{
	int ret;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 5883);
			goto out;
		}
	}
	ret = 0;
	out: ;
	return ret;
}

static u8 const _ext_oid_bad_ct1[10] =
{(u8)0x06,
	(u8)0x08,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x05,
	(u8)0x05,
	(u8)0x07,
	(u8)0x01,
(u8)0x01};

static u8 const _ext_oid_bad_ct_poison[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0xd6,
	(u8)0x79,
	(u8)0x02,
	(u8)0x04,
(u8)0x03};

static u8 const _ext_oid_bad_ct_enabled[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0xd6,
	(u8)0x79,
	(u8)0x02,
	(u8)0x04,
(u8)0x02};

static u8 const _ext_oid_bad_ns_cert_type[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x86,
	(u8)0xf8,
	(u8)0x42,
	(u8)0x01,
(u8)0x01};

static u8 const _ext_oid_bad_szOID_ENROLL[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x14,
(u8)0x02};

static u8 const _ext_oid_bad_smime_cap[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2a,
	(u8)0x86,
	(u8)0x48,
	(u8)0x86,
	(u8)0xf7,
	(u8)0x0d,
	(u8)0x01,
	(u8)0x09,
(u8)0x0f};

static u8 const _ext_oid_bad_ns_comment[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x86,
	(u8)0xf8,
	(u8)0x42,
	(u8)0x01,
(u8)0x0d};

static u8 const _ext_oid_bad_deprecated_AKI[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x01};

static u8 const _ext_oid_bad_szOID_CERT_TEMPLATE[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x07};

static u8 const _ext_oid_bad_pkixFixes[12] =
{(u8)0x06,
	(u8)0x0a,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x97,
	(u8)0x55,
	(u8)0x03,
	(u8)0x01,
(u8)0x05};

static u8 const _ext_oid_bad_ns_ca_policy_url[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x60,
	(u8)0x86,
	(u8)0x48,
	(u8)0x01,
	(u8)0x86,
	(u8)0xf8,
	(u8)0x42,
	(u8)0x01,
(u8)0x08};

static u8 const _ext_oid_bad_szOID_CERTSRV_CA_VERS[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x01};

static u8 const _ext_oid_bad_szOID_APP_CERT_POL[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x0a};

static u8 const _ext_oid_bad_priv_key_usage_period[5] =
{(u8)0x06, (u8)0x03, (u8)0x55, (u8)0x1d, (u8)0x10};

static u8 const _ext_oid_bad_subject_signing_tool[7] =
{(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x6f};

static u8 const _ext_oid_bad_issuer_signing_tool[7] =
{(u8)0x06, (u8)0x05, (u8)0x2a, (u8)0x85, (u8)0x03, (u8)0x64, (u8)0x70};

static u8 const _ext_oid_bad_szOID_CERTSRV_PREVIOUS_CERT_HASH[11] =
{(u8)0x06,
	(u8)0x09,
	(u8)0x2b,
	(u8)0x06,
	(u8)0x01,
	(u8)0x04,
	(u8)0x01,
	(u8)0x82,
	(u8)0x37,
	(u8)0x15,
(u8)0x02};

static _ext_oid const generic_unsupported_ext_oid =
{.oid = (u8 const *)0,
	.oid_len = (u32)0,
.parse_ext_params = & parse_ext_bad_oid};

static _ext_oid const known_ext_oids[33] =
{{.oid = _ext_oid_AIA,
		.oid_len = (u32)sizeof(_ext_oid_AIA),
	.parse_ext_params = & parse_ext_AIA},
	{.oid = _ext_oid_AKI,
		.oid_len = (u32)sizeof(_ext_oid_AKI),
	.parse_ext_params = & parse_ext_AKI},
	{.oid = _ext_oid_SKI,
		.oid_len = (u32)sizeof(_ext_oid_SKI),
	.parse_ext_params = & parse_ext_SKI},
	{.oid = _ext_oid_keyUsage,
		.oid_len = (u32)sizeof(_ext_oid_keyUsage),
	.parse_ext_params = & parse_ext_keyUsage},
	{.oid = _ext_oid_certPolicies,
		.oid_len = (u32)sizeof(_ext_oid_certPolicies),
	.parse_ext_params = & parse_ext_certPolicies},
	{.oid = _ext_oid_policyMapping,
		.oid_len = (u32)sizeof(_ext_oid_policyMapping),
	.parse_ext_params = & parse_ext_policyMapping},
	{.oid = _ext_oid_SAN,
		.oid_len = (u32)sizeof(_ext_oid_SAN),
	.parse_ext_params = & parse_ext_SAN},
	{.oid = _ext_oid_IAN,
		.oid_len = (u32)sizeof(_ext_oid_IAN),
	.parse_ext_params = & parse_ext_IAN},
	{.oid = _ext_oid_subjectDirAttr,
		.oid_len = (u32)sizeof(_ext_oid_subjectDirAttr),
	.parse_ext_params = & parse_ext_subjectDirAttr},
	{.oid = _ext_oid_basicConstraints,
		.oid_len = (u32)sizeof(_ext_oid_basicConstraints),
	.parse_ext_params = & parse_ext_basicConstraints},
	{.oid = _ext_oid_nameConstraints,
		.oid_len = (u32)sizeof(_ext_oid_nameConstraints),
	.parse_ext_params = & parse_ext_nameConstraints},
	{.oid = _ext_oid_policyConstraints,
		.oid_len = (u32)sizeof(_ext_oid_policyConstraints),
	.parse_ext_params = & parse_ext_policyConstraints},
	{.oid = _ext_oid_EKU,
		.oid_len = (u32)sizeof(_ext_oid_EKU),
	.parse_ext_params = & parse_ext_EKU},
	{.oid = _ext_oid_CRLDP,
		.oid_len = (u32)sizeof(_ext_oid_CRLDP),
	.parse_ext_params = & parse_ext_CRLDP},
	{.oid = _ext_oid_inhibitAnyPolicy,
		.oid_len = (u32)sizeof(_ext_oid_inhibitAnyPolicy),
	.parse_ext_params = & parse_ext_inhibitAnyPolicy},
	{.oid = _ext_oid_FreshestCRL,
		.oid_len = (u32)sizeof(_ext_oid_FreshestCRL),
	.parse_ext_params = & parse_ext_CRLDP},
	{.oid = _ext_oid_bad_ct1,
		.oid_len = (u32)sizeof(_ext_oid_bad_ct1),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_ct_poison,
		.oid_len = (u32)sizeof(_ext_oid_bad_ct_poison),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_ct_enabled,
		.oid_len = (u32)sizeof(_ext_oid_bad_ct_enabled),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_ns_cert_type,
		.oid_len = (u32)sizeof(_ext_oid_bad_ns_cert_type),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_szOID_ENROLL,
		.oid_len = (u32)sizeof(_ext_oid_bad_szOID_ENROLL),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_smime_cap,
		.oid_len = (u32)sizeof(_ext_oid_bad_smime_cap),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_ns_comment,
		.oid_len = (u32)sizeof(_ext_oid_bad_ns_comment),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_deprecated_AKI,
		.oid_len = (u32)sizeof(_ext_oid_bad_deprecated_AKI),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_szOID_CERT_TEMPLATE,
		.oid_len = (u32)sizeof(_ext_oid_bad_szOID_CERT_TEMPLATE),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_pkixFixes,
		.oid_len = (u32)sizeof(_ext_oid_bad_pkixFixes),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_ns_ca_policy_url,
		.oid_len = (u32)sizeof(_ext_oid_bad_ns_ca_policy_url),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_szOID_CERTSRV_CA_VERS,
		.oid_len = (u32)sizeof(_ext_oid_bad_szOID_CERTSRV_CA_VERS),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_szOID_APP_CERT_POL,
		.oid_len = (u32)sizeof(_ext_oid_bad_szOID_APP_CERT_POL),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_priv_key_usage_period,
		.oid_len = (u32)sizeof(_ext_oid_bad_priv_key_usage_period),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_subject_signing_tool,
		.oid_len = (u32)sizeof(_ext_oid_bad_subject_signing_tool),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_issuer_signing_tool,
		.oid_len = (u32)sizeof(_ext_oid_bad_issuer_signing_tool),
	.parse_ext_params = & parse_ext_bad_oid},
	{.oid = _ext_oid_bad_szOID_CERTSRV_PREVIOUS_CERT_HASH,
		.oid_len = (u32)sizeof(_ext_oid_bad_szOID_CERTSRV_PREVIOUS_CERT_HASH),
.parse_ext_params = & parse_ext_bad_oid}};

/*@
	requires buf == \null || len == 0 || \valid_read(buf + (0 .. len-1));
	assigns \nothing;
	ensures \result == \null ||(\exists integer i; 0 <= i < sizeof(known_ext_oids)/sizeof(known_ext_oids[0]) &&\result == &known_ext_oids[i]);
	ensures (buf == \null || len == 0) ==> \result == \null;
	ensures \result != \null ==>(\result->oid_len == len &&(\forall integer j; 0 <= j < len ==> \result->oid[j] == buf[j]));
	ensures (\result == \null && buf != \null && len != 0) ==>(\forall integer i; 0 <= i < sizeof(known_ext_oids)/sizeof(known_ext_oids[0]) ==>known_ext_oids[i].oid_len != len ||(\exists integer j; 0 <= j < len && known_ext_oids[i].oid[j] != buf[j]));
*/
static _ext_oid const *find_ext_by_oid(u8 const *buf, u32 len)
{
	u16 k;
	_ext_oid const *found = (_ext_oid const *)0;
	_ext_oid const *cur = (_ext_oid const *)0;
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) _LOR: goto out;
	k = (u16)0;
	/*@
		loop invariant 0 <= k <= sizeof(known_ext_oids) / sizeof(known_ext_oids[0]);
		loop invariant found == (_ext_oid const *)0 ==>(\forall integer i; 0 <= i < k ==>known_ext_oids[i].oid_len != len ||bufs_differ(known_ext_oids[i].oid, buf, known_ext_oids[i].oid_len) != 0);
		loop invariant found != (_ext_oid const *)0 ==>(\exists integer i; 0 <= i < sizeof(known_ext_oids) / sizeof(known_ext_oids[0]) &&found == &known_ext_oids[i] &&found->oid_len == len &&bufs_differ(found->oid, buf, found->oid_len) == 0);
		loop invariant cur == (_ext_oid const *)0 ||(\exists integer i; 0 <= i < sizeof(known_ext_oids) / sizeof(known_ext_oids[0]) &&cur == &known_ext_oids[i]);
		loop assigns k, cur, found;
		loop variant sizeof(known_ext_oids) / sizeof(known_ext_oids[0]) - k;
	*/
	while ((unsigned long)k < sizeof(known_ext_oids) / sizeof(known_ext_oids[0])) {
		{
			int ret;
			int tmp_0;
			int tmp;
			cur = & known_ext_oids[k];
		/*@ assert rte_445: mem_access: \valid_read(&cur->oid_len); */
			if (cur->oid_len != len) goto __Cont;
		/*@ assert rte_446: mem_access: \valid_read(&cur->oid); */
		/*@ assert rte_447: mem_access: \valid_read(&cur->oid_len); */
			tmp = bufs_differ(cur->oid,buf,cur->oid_len);
			if (tmp) tmp_0 = 0; else tmp_0 = 1;
			ret = tmp_0;
			if (ret) {
				found = cur;
				break;
			}
		}
		__Cont:
	/*@ assert rte_448: signed_overflow: (int)k + 1 <= 2147483647; */
		k = (u16)((int)k + 1);
	}
	out: ;
	return found;
}

/*@
	requires \valid_read(ext);
	requires \valid(parsed_oid_list + (0 .. 32));
	assigns parsed_oid_list[0 .. 32];
	ensures \result == 0 || \result == -(2 * 100000 + 6203) || \result == -(2 * 100000 + 6215);
	ensures \result == 0 ==>(\exists integer k; 0 <= k < 33 &&parsed_oid_list[k] == ext &&(\forall integer j; 0 <= j < k ==>\at(parsed_oid_list[j], Pre) != (_ext_oid const *)0 &&\at(parsed_oid_list[j], Pre) != ext) &&\at(parsed_oid_list[k], Pre) == (_ext_oid const *)0);
	ensures \result == -(2 * 100000 + 6203) ==>(\exists integer k; 0 <= k < 33 &&\at(parsed_oid_list[k], Pre) == ext &&(\forall integer j; 0 <= j < k ==>\at(parsed_oid_list[j], Pre) != (_ext_oid const *)0));
	ensures \result == -(2 * 100000 + 6215) ==>(\forall integer k; 0 <= k < 33 ==>\at(parsed_oid_list[k], Pre) != (_ext_oid const *)0 &&\at(parsed_oid_list[k], Pre) != ext);
*/
static int check_record_ext_unknown(_ext_oid const *ext,
_ext_oid const **parsed_oid_list)
{
	int ret;
	u16 pos = (u16)0;
	/*@
		loop invariant 0 <= pos <= 33;
		loop invariant \forall integer j; 0 <= j < pos ==>parsed_oid_list[j] != (_ext_oid const *)0 &&parsed_oid_list[j] != ext;
		loop assigns pos;
		loop variant 33 - pos;
	*/
	while ((unsigned long)pos < sizeof(known_ext_oids) / sizeof(known_ext_oids[0])) {
	/*@ assert rte_449: mem_access: \valid_read(parsed_oid_list + (int)pos); */
		if (*(parsed_oid_list + (int)pos) == (_ext_oid const *)0) {
		/*@ assert rte_450: mem_access: \valid(parsed_oid_list + (int)pos); */
			*(parsed_oid_list + (int)pos) = ext;
			break;
		}
	/*@ assert rte_451: mem_access: \valid_read(parsed_oid_list + (int)pos); */
		if (ext == *(parsed_oid_list + (int)pos)) {
			ret = - (2 * 100000 + 6203);
			goto out;
		}
	/*@ assert rte_452: signed_overflow: (int)pos + 1 <= 2147483647; */
		pos = (u16)((int)pos + 1);
	}
	if ((unsigned long)pos >= sizeof(known_ext_oids) / sizeof(known_ext_oids[0])) {
		ret = - (2 * 100000 + 6215);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_cert_Extension(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len,
_ext_oid const **parsed_oid_list,
u32 *eaten)
{
	int ret;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u32 ext_hdr_len = (u32)0;
	u32 ext_data_len = (u32)0;
	u32 oid_len = (u32)0;
	u32 saved_ext_len = (u32)0;
	u32 parsed = (u32)0;
	u8 const *buf = cert + off;
	_ext_oid const *ext = (_ext_oid const *)0;
	int critical = 0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 6269);
			goto out;
		}
	}
	remain = len;
	ret = parse_id_len(buf,remain,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& ext_hdr_len,& ext_data_len);
	if (ret) goto out;
	buf += ext_hdr_len;
	off += ext_hdr_len;
	remain -= ext_hdr_len;
	saved_ext_len = ext_hdr_len + ext_data_len;
	ret = parse_OID(buf,ext_data_len,& oid_len);
	if (ret) goto out;
	ext = find_ext_by_oid(buf,oid_len);
	if (ext == (_ext_oid const *)0) ext = & generic_unsupported_ext_oid;
	if (ext != & generic_unsupported_ext_oid) {
		ret = check_record_ext_unknown(ext,parsed_oid_list);
		if (ret) goto out;
	}
	buf += oid_len;
	off += oid_len;
	ext_data_len -= oid_len;
	ret = parse_boolean(buf,ext_data_len,& parsed);
	if (ret) {
		if (ext_data_len)
	/*@ assert rte_495: mem_access: \valid_read(buf + 0); */
		if ((int)*(buf + 0) == ASN1_TYPE_BOOLEAN) {
			ret = - (2 * 100000 + 6375);
			goto out;
		}
	}
	else {
		if (parsed != (u32)3) {
			ret = - (2 * 100000 + 6387);
			goto out;
		}
		critical = 1;
		buf += parsed;
		off += parsed;
		ext_data_len -= parsed;
	}
	ret = parse_id_len(buf,ext_data_len,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_OCTET_STRING,& hdr_len,& data_len);
	if (ret) goto out;
	buf += hdr_len;
	off += hdr_len;
	ext_data_len -= hdr_len;
	if (data_len != ext_data_len) {
		ret = - (2 * 100000 + 6429);
		goto out;
	}
/*@ assert rte_496: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_AIA) ret = parse_ext_AIA(ctx,cert,
	off,
	ext_data_len,
	critical);
	else
/*@ assert rte_497: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_AKI) ret = parse_ext_AKI
	(ctx,cert,off,ext_data_len,
	critical);
	else
/*@ assert rte_498: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_SKI) ret = parse_ext_SKI
	(ctx,cert,off,
	ext_data_len,critical);
	else
/*@ assert rte_499: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_keyUsage) ret = parse_ext_keyUsage
	(ctx,cert,off,
	ext_data_len,
	critical);
	else
/*@ assert rte_500: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_certPolicies) ret = parse_ext_certPolicies
	(ctx,cert,
	off,
	ext_data_len,
	critical);
	else
/*@ assert rte_501: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_policyMapping) ret = parse_ext_policyMapping
	(ctx,
	cert,off,
	ext_data_len,
	critical);
	else
/*@ assert rte_502: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_SAN) ret = parse_ext_SAN
	(ctx,cert,off,
	ext_data_len,
	critical);
	else
/*@ assert rte_503: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_IAN) ret = parse_ext_IAN
	(ctx,cert,off,
	ext_data_len,
	critical);
	else
/*@ assert rte_504: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_subjectDirAttr)
	ret = parse_ext_subjectDirAttr(ctx,cert,off,ext_data_len,
	critical);
	else
/*@ assert rte_505: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_basicConstraints)
	ret = parse_ext_basicConstraints(ctx,cert,off,
	ext_data_len,critical);
	else
/*@ assert rte_506: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_nameConstraints)
	ret = parse_ext_nameConstraints(ctx,cert,off,
	ext_data_len,
	critical);
	else
/*@ assert rte_507: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_policyConstraints)
	ret = parse_ext_policyConstraints(ctx,cert,off,
	ext_data_len,
	critical);
	else
/*@ assert rte_508: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_EKU)
	ret = parse_ext_EKU(ctx,cert,off,ext_data_len,
	critical);
	else
/*@ assert rte_509: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_CRLDP)
	ret = parse_ext_CRLDP(ctx,cert,off,
	ext_data_len,critical);
	else
/*@ assert rte_510: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_inhibitAnyPolicy)
	ret = parse_ext_inhibitAnyPolicy(ctx,cert,
	off,
	ext_data_len,
	critical);
	else
/*@ assert rte_511: mem_access: \valid_read(&ext->parse_ext_params); */
	if (ext->parse_ext_params == & parse_ext_bad_oid)
	ret = parse_ext_bad_oid(ctx,cert,off,
	ext_data_len,
	critical);
	if (ret) goto out;
/*@ assert rte_512: mem_access: \valid(eaten); */
	*eaten = saved_ext_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_cert_Extensions(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len, u32 *eaten)
{
	_ext_oid const *parsed_oid_list[sizeof(known_ext_oids) / sizeof(known_ext_oids[0])];
	int ret;
	u16 i;
	u32 data_len = (u32)0;
	u32 hdr_len = (u32)0;
	u32 remain = (u32)0;
	u8 const *buf = cert + off;
	u32 saved_len = (u32)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 6509);
			goto out;
		}
	}
	ret = parse_explicit_id_len(buf,len,(u32)3,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_SEQUENCE,& hdr_len,& data_len);
	if (ret) goto out;
	remain = data_len;
	buf += hdr_len;
	off += hdr_len;
	saved_len = hdr_len + data_len;
	if (data_len == (u32)0) {
		ret = - (2 * 100000 + 6539);
		goto out;
	}
	i = (u16)0;
	while ((unsigned long)i < sizeof(known_ext_oids) / sizeof(known_ext_oids[0])) {
	/*@ assert rte_513: index_bound: i < (unsigned long)(sizeof(_ext_oid const [33]) / sizeof(_ext_oid)); */
		parsed_oid_list[i] = (_ext_oid const *)0;
	/*@ assert rte_514: signed_overflow: (int)i + 1 <= 2147483647; */
		i = (u16)((int)i + 1);
	}
	while (remain) {
		u32 ext_len = (u32)0;
		ret = parse_x509_cert_Extension(ctx,cert,off,remain,parsed_oid_list,
		& ext_len);
		if (ret) goto out;
		remain -= ext_len;
		buf += ext_len;
		off += ext_len;
	}
/*@ assert rte_515: mem_access: \valid_read(&ctx->empty_subject); */
	if (ctx->empty_subject) {
	/*@ assert rte_516: mem_access: \valid_read(&ctx->has_san); */
		if (! ctx->has_san) {
			ret = - (2 * 100000 + 6584);
			goto out;
		}
	/*@ assert rte_517: mem_access: \valid_read(&ctx->san_critical); */
		if (! ctx->san_critical) {
			ret = - (2 * 100000 + 6589);
			goto out;
		}
	}
/*@ assert rte_518: mem_access: \valid(eaten); */
	*eaten = saved_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_tbsCertificate(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len,
_sig_alg const **sig_alg, u32 *eaten)
{
	u8 const *subject_ptr;
	u8 const *issuer_ptr;
	u32 subject_len;
	u32 issuer_len;
	int ret;
	u32 tbs_data_len = (u32)0;
	u32 tbs_hdr_len = (u32)0;
	u32 tbs_cert_len = (u32)0;
	u32 remain = (u32)0;
	u32 parsed = (u32)0;
	u32 cur_off = off;
	u8 const *buf = cert + cur_off;
	_sig_alg const *alg = (_sig_alg const *)0;
	int empty_issuer = 1;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 6666);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& tbs_hdr_len,& tbs_data_len);
	if (ret) goto out;
	tbs_cert_len = tbs_hdr_len + tbs_data_len;
	buf += tbs_hdr_len;
	cur_off += tbs_hdr_len;
	remain = tbs_data_len;
	ret = parse_x509_cert_Version(cert,cur_off,remain,& ctx->version,& parsed);
	if (ret) goto out;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_CertSerialNumber(ctx,cert,cur_off,remain,CLASS_UNIVERSAL,
	(u32)ASN1_TYPE_INTEGER,& parsed);
	if (ret) goto out;
/*@ assert rte_519: mem_access: \valid_read(&ctx->version); */
	if ((int)ctx->version != 0x02) {
		ret = X509_PARSER_ERROR_VERSION_NOT_3;
		goto out;
	}
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_x509_tbsCert_sig_AlgorithmIdentifier(ctx,cert,cur_off,remain,
	& alg,& parsed);
	if (ret) goto out;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_x509_Name(buf,remain,& parsed,& empty_issuer);
	if (ret) goto out;
/*@ assert rte_520: mem_access: \valid(&ctx->issuer_start); */
	ctx->issuer_start = cur_off;
/*@ assert rte_521: mem_access: \valid(&ctx->issuer_len); */
	ctx->issuer_len = parsed;
	if (empty_issuer) {
		ret = - (2 * 100000 + 6756);
		goto out;
	}
	issuer_ptr = buf;
	issuer_len = parsed;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_x509_Validity(ctx,cert,cur_off,remain,& parsed);
	if (ret) goto out;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	ret = parse_x509_Name(buf,remain,& parsed,& ctx->empty_subject);
	if (ret) goto out;
/*@ assert rte_522: mem_access: \valid(&ctx->subject_start); */
	ctx->subject_start = cur_off;
/*@ assert rte_523: mem_access: \valid(&ctx->subject_len); */
	ctx->subject_len = parsed;
	subject_ptr = buf;
	subject_len = parsed;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
/*@ assert rte_524: mem_access: \valid(&ctx->subject_issuer_identical); */
	ctx->subject_issuer_identical = 0;
	if (subject_len == issuer_len) {
		int tmp_0;
		int tmp;
		tmp = bufs_differ(subject_ptr,issuer_ptr,issuer_len);
		if (tmp) tmp_0 = 0; else tmp_0 = 1;
	/*@ assert rte_525: mem_access: \valid(&ctx->subject_issuer_identical); */
		ctx->subject_issuer_identical = tmp_0;
	}
	ret = parse_x509_subjectPublicKeyInfo(ctx,cert,cur_off,remain,& parsed);
	if (ret) goto out;
	buf += parsed;
	cur_off += parsed;
	remain -= parsed;
	if (remain) {
		ret = parse_x509_cert_Extensions(ctx,cert,cur_off,remain,& parsed);
		if (ret) goto out;
		buf += parsed;
		cur_off += parsed;
		remain -= parsed;
	}
	if (remain != (u32)0) {
		ret = - (2 * 100000 + 6864);
		goto out;
	}
/*@ assert rte_526: mem_access: \valid_read(&ctx->keyCertSign_set); */
	if (ctx->keyCertSign_set)
/*@ assert rte_527: mem_access: \valid_read(&ctx->ca_true); */
	if (! ctx->ca_true) goto _LOR_0;
	else
/*@ assert rte_528: mem_access: \valid_read(&ctx->bc_critical); */
	if (! ctx->bc_critical) {
		_LOR_0: {
			ret = - (2 * 100000 + 6902);
			goto out;
		}
	}
/*@ assert rte_529: mem_access: \valid_read(&ctx->cRLSign_set); */
	if (ctx->cRLSign_set)
/*@ assert rte_530: mem_access: \valid_read(&ctx->empty_subject); */
	if (ctx->empty_subject) {
		ret = - (2 * 100000 + 6915);
		goto out;
	}
/*@ assert rte_531: mem_access: \valid_read(&ctx->pathLenConstraint_set); */
	if (ctx->pathLenConstraint_set)
/*@ assert rte_532: mem_access: \valid_read(&ctx->ca_true); */
	if (! ctx->ca_true) goto _LOR_1;
	else
/*@ assert rte_533: mem_access: \valid_read(&ctx->keyCertSign_set); */
	if (! ctx->keyCertSign_set) {
		_LOR_1: {
			ret = - (2 * 100000 + 6927);
			goto out;
		}
	}
/*@ assert rte_534: mem_access: \valid_read(&ctx->has_name_constraints); */
	if (ctx->has_name_constraints)
/*@ assert rte_535: mem_access: \valid_read(&ctx->ca_true); */
	if (! ctx->ca_true) {
		ret = - (2 * 100000 + 6937);
		goto out;
	}
/*@ assert rte_536: mem_access: \valid_read(&ctx->ca_true); */
	if (ctx->ca_true)
/*@ assert rte_537: mem_access: \valid_read(&ctx->has_crldp); */
	if (ctx->has_crldp)
/*@ assert rte_538: mem_access: \valid_read(&ctx->one_crldp_has_all_reasons); */
	if (! ctx->one_crldp_has_all_reasons) {
		ret = - (2 * 100000 + 6949);
		goto out;
	}
/*@ assert rte_539: mem_access: \valid(eaten); */
	*eaten = tbs_cert_len;
/*@ assert rte_540: mem_access: \valid(sig_alg); */
	*sig_alg = alg;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \valid(ctx) || ctx == \null;
	requires \valid(eaten) || eaten == \null;
	requires (cert != \null && len > 0) ==> \valid_read(cert + (0 .. off + len - 1));
	requires (cert != \null && ctx != \null && len > 0) ==>(ctx->tbs_sig_alg_len == 0 ||\valid_read(cert + (ctx->tbs_sig_alg_start .. ctx->tbs_sig_alg_start + ctx->tbs_sig_alg_len - 1)));
	requires off <= UINT32_MAX - len;
	assigns ctx->sig_alg_start, ctx->sig_alg_len, *eaten;
	ensures (cert == \null || len == 0 || ctx == \null || eaten == \null) ==> (\result == -207003);
	ensures (cert != \null && len != 0 && ctx != \null && eaten != \null) ==> (\result == -207010);
	ensures (ctx->tbs_sig_alg_len > len) ==> (\result == -207010);
	ensures (cert != \null && len != 0 && ctx != \null && eaten != \null) ==> (\result == -207017);
	ensures (ctx->tbs_sig_alg_len <= len) ==> (\result == -207017);
	ensures (\exists integer k; 0 <= k < ctx->tbs_sig_alg_len &&cert[ctx->tbs_sig_alg_start + k] != cert[off + k]) ==> (\result == -207017);
	ensures (cert != \null && len != 0 && ctx != \null && eaten != \null) ==> (ctx->sig_alg_start == off);
	ensures (ctx->tbs_sig_alg_len <= len) ==> (ctx->sig_alg_start == off);
	ensures (\forall integer k; 0 <= k < ctx->tbs_sig_alg_len ==>cert[ctx->tbs_sig_alg_start + k] == cert[off + k]) ==> (ctx->sig_alg_start == off);
	ensures (cert != \null && len != 0 && ctx != \null && eaten != \null) ==> (ctx->sig_alg_len == \old(ctx->tbs_sig_alg_len));
	ensures (ctx->tbs_sig_alg_len <= len) ==> (ctx->sig_alg_len == \old(ctx->tbs_sig_alg_len));
	ensures (\forall integer k; 0 <= k < ctx->tbs_sig_alg_len ==>cert[ctx->tbs_sig_alg_start + k] == cert[off + k]) ==> (ctx->sig_alg_len == \old(ctx->tbs_sig_alg_len));
	ensures (cert != \null && len != 0 && ctx != \null && eaten != \null) ==> (*eaten == \old(ctx->tbs_sig_alg_len));
	ensures (ctx->tbs_sig_alg_len <= len) ==> (*eaten == \old(ctx->tbs_sig_alg_len));
	ensures (\forall integer k; 0 <= k < ctx->tbs_sig_alg_len ==>cert[ctx->tbs_sig_alg_start + k] == cert[off + k]) ==> (*eaten == \old(ctx->tbs_sig_alg_len));
	ensures (cert != \null && len != 0 && ctx != \null && eaten != \null) ==> (\result == 0);
	ensures (ctx->tbs_sig_alg_len <= len) ==> (\result == 0);
	ensures (\forall integer k; 0 <= k < ctx->tbs_sig_alg_len ==>cert[ctx->tbs_sig_alg_start + k] == cert[off + k]) ==> (\result == 0);
	ensures \result == 0 || \result == -207003 || \result == -207010 || \result == -207017;
*/
static int parse_x509_signatureAlgorithm(cert_parsing_ctx *ctx,
u8 const *cert, u32 off, u32 len,
u32 *eaten)
{
	u32 prev_len;
	int ret;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 7003);
			goto out;
		}
	}
/*@ assert rte_541: mem_access: \valid_read(&ctx->tbs_sig_alg_len); */
	prev_len = ctx->tbs_sig_alg_len;
	if (prev_len > len) {
		ret = - (2 * 100000 + 7010);
		goto out;
	}
/*@ assert rte_542: mem_access: \valid_read(&ctx->tbs_sig_alg_start); */
	ret = bufs_differ(cert + ctx->tbs_sig_alg_start,cert + off,prev_len);
	if (ret) {
		ret = - (2 * 100000 + 7017);
		goto out;
	}
/*@ assert rte_543: mem_access: \valid(&ctx->sig_alg_start); */
	ctx->sig_alg_start = off;
/*@ assert rte_544: mem_access: \valid(&ctx->sig_alg_len); */
	ctx->sig_alg_len = prev_len;
/*@ assert rte_545: mem_access: \valid(eaten); */
	*eaten = prev_len;
	ret = 0;
	out: ;
	return ret;
}

static int parse_x509_signatureValue(cert_parsing_ctx *ctx, u8 const *cert,
u32 off, u32 len,
_sig_alg const *sig_alg, u32 *eaten)
{
	sig_params *params;
	int ret;
	u32 saved_off = off;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (sig_alg == (_sig_alg const *)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (2 * 100000 + 7062);
			goto out;
		}
	}
/*@ assert rte_614: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == (int (*)(sig_params *params, u8 const *cert,
	u32 off, u32 len, u32 *eaten))0) {
		ret = - (2 * 100000 + 7068);
		goto out;
	}
	params = & ctx->sig_alg_params;
/*@ assert rte_615: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_ed448) ret = parse_sig_ed448(params,
	cert,off,
	len,
	eaten);
	else
/*@ assert rte_616: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_ed25519) ret = parse_sig_ed25519
	(params,cert,off,len,
	eaten);
	else
/*@ assert rte_617: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_ecdsa) ret = parse_sig_ecdsa
	(params,cert,off,len,
	eaten);
	else
/*@ assert rte_618: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_sm2) ret = parse_sig_sm2
	(params,cert,off,len,
	eaten);
	else
/*@ assert rte_619: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_dsa) ret = parse_sig_dsa
	(params,cert,off,len,
	eaten);
	else
/*@ assert rte_620: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_pkcs1_v15) ret = parse_sig_rsa_pkcs1_v15
	(params,
	cert,off,
	len,eaten);
	else
/*@ assert rte_621: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_ssa_pss) ret = parse_sig_rsa_ssa_pss
	(params,
	cert,off,
	len,eaten);
	else
/*@ assert rte_622: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_9796_2_pad)
	ret = parse_sig_rsa_9796_2_pad(params,cert,off,len,eaten);
	else
/*@ assert rte_623: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_rsa_belgian)
	ret = parse_sig_rsa_belgian(params,cert,off,len,eaten);
	else
/*@ assert rte_624: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost94) ret = parse_sig_gost94
	(params,
	cert,off,
	len,eaten);
	else
/*@ assert rte_625: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost2001)
	ret = parse_sig_gost2001(params,cert,off,len,eaten);
	else
/*@ assert rte_626: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost2012_512)
	ret = parse_sig_gost2012_512(params,cert,off,len,
	eaten);
	else
/*@ assert rte_627: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_gost2012_256)
	ret = parse_sig_gost2012_256(params,cert,off,len,
	eaten);
	else
/*@ assert rte_628: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_bign)
	ret = parse_sig_bign(params,cert,off,len,eaten);
	else
/*@ assert rte_629: mem_access: \valid_read(&sig_alg->parse_sig); */
	if (sig_alg->parse_sig == & parse_sig_monkey)
	ret = parse_sig_monkey(params,cert,off,len,
	eaten);
	if (ret) goto out;
/*@ assert rte_630: mem_access: \valid(&ctx->sig_start); */
	ctx->sig_start = saved_off;
/*@ assert rte_631: mem_access: \valid(&ctx->sig_len); */
/*@ assert rte_632: mem_access: \valid_read(eaten); */
	ctx->sig_len = *eaten;
	ret = 0;
	out: ;
	return ret;
}

/*@
	requires \true;
	assigns \nothing;
	ensures \result.tbs_start == 0;
	ensures \result.tbs_len == 0;
	ensures \result.version == 0;
	ensures \result.serial_start == 0;
	ensures \result.serial_len == 0;
	ensures \result.tbs_sig_alg_start == 0;
	ensures \result.tbs_sig_alg_len == 0;
	ensures \result.tbs_sig_alg_oid_start == 0;
	ensures \result.tbs_sig_alg_oid_len == 0;
	ensures \result.tbs_sig_alg_oid_params_start == 0;
	ensures \result.tbs_sig_alg_oid_params_len == 0;
	ensures \result.issuer_start == 0;
	ensures \result.issuer_len == 0;
	ensures \result.not_before == 0;
	ensures \result.not_after == 0;
	ensures \result.subject_start == 0;
	ensures \result.subject_len == 0;
	ensures \result.empty_subject == 0;
	ensures \result.subject_issuer_identical == 0;
	ensures \result.spki_start == 0;
	ensures \result.spki_len == 0;
	ensures \result.spki_alg_oid_start == 0;
	ensures \result.spki_alg_oid_len == 0;
	ensures \result.spki_alg_oid_params_start == 0;
	ensures \result.spki_alg_oid_params_len == 0;
	ensures \result.spki_pub_key_start == 0;
	ensures \result.spki_pub_key_len == 0;
	ensures \result.spki_alg == (spki_alg_id)0;
	ensures \result.spki_alg_params.ecpubkey.curve == (curve_id)0;
	ensures \result.spki_alg_params.ecpubkey.curve_order_bit_len == 0;
	ensures \result.spki_alg_params.ecpubkey.compression == 0;
	ensures \result.spki_alg_params.ecpubkey.ecc_raw_x_off == 0;
	ensures \result.spki_alg_params.ecpubkey.ecc_raw_x_len == 0;
	ensures \result.spki_alg_params.ecpubkey.ecc_raw_y_off == 0;
	ensures \result.spki_alg_params.ecpubkey.ecc_raw_y_len == 0;
	ensures \result.has_ski == 0;
	ensures \result.ski_start == 0;
	ensures \result.ski_len == 0;
	ensures \result.has_aki == 0;
	ensures \result.aki_has_keyIdentifier == 0;
	ensures \result.aki_keyIdentifier_start == 0;
	ensures \result.aki_keyIdentifier_len == 0;
	ensures \result.aki_has_generalNames_and_serial == 0;
	ensures \result.aki_generalNames_start == 0;
	ensures \result.aki_generalNames_len == 0;
	ensures \result.aki_serial_start == 0;
	ensures \result.aki_serial_len == 0;
	ensures \result.has_san == 0;
	ensures \result.san_critical == 0;
	ensures \result.bc_critical == 0;
	ensures \result.ca_true == 0;
	ensures \result.pathLenConstraint_set == 0;
	ensures \result.has_keyUsage == 0;
	ensures \result.keyCertSign_set == 0;
	ensures \result.cRLSign_set == 0;
	ensures \result.has_eku == 0;
	ensures \result.has_crldp == 0;
	ensures \result.one_crldp_has_all_reasons == 0;
	ensures \result.has_name_constraints == 0;
	ensures \result.sig_alg_start == 0;
	ensures \result.sig_alg_len == 0;
	ensures \result.sig_alg == (sig_alg_id)0;
	ensures \result.hash_alg == (hash_alg_id)0;
	ensures \result.sig_alg_params.dsa.r_raw_off == 0;
	ensures \result.sig_alg_params.dsa.r_raw_len == 0;
	ensures \result.sig_alg_params.dsa.s_raw_off == 0;
	ensures \result.sig_alg_params.dsa.s_raw_len == 0;
	ensures \result.sig_start == 0;
	ensures \result.sig_len == 0;
*/
static cert_parsing_ctx get_zeroized_cert_ctx_val(void)
{
	cert_parsing_ctx zeroized_ctx =
	{.tbs_start = (u32)0,
		.tbs_len = 0U,
		.version = (unsigned char)0,
		.serial_start = 0U,
		.serial_len = 0U,
		.tbs_sig_alg_start = 0U,
		.tbs_sig_alg_len = 0U,
		.tbs_sig_alg_oid_start = 0U,
		.tbs_sig_alg_oid_len = 0U,
		.tbs_sig_alg_oid_params_start = 0U,
		.tbs_sig_alg_oid_params_len = 0U,
		.issuer_start = 0U,
		.issuer_len = 0U,
		.not_before = 0UL,
		.not_after = 0UL,
		.subject_start = 0U,
		.subject_len = 0U,
		.empty_subject = 0,
		.subject_issuer_identical = 0,
		.spki_start = 0U,
		.spki_len = 0U,
		.spki_alg_oid_start = 0U,
		.spki_alg_oid_len = 0U,
		.spki_alg_oid_params_start = 0U,
		.spki_alg_oid_params_len = 0U,
		.spki_pub_key_start = 0U,
		.spki_pub_key_len = 0U,
		.spki_alg = (spki_alg_id)0,
		.spki_alg_params = {.ecpubkey = {.curve = (curve_id)0,
				.curve_order_bit_len = 0U,
				.compression = 0,
				.ecc_raw_x_off = 0U,
				.ecc_raw_x_len = 0U,
				.ecc_raw_y_off = 0U,
		.ecc_raw_y_len = 0U}},
		.has_ski = 0,
		.ski_start = 0U,
		.ski_len = 0U,
		.has_aki = 0,
		.aki_has_keyIdentifier = 0,
		.aki_keyIdentifier_start = 0U,
		.aki_keyIdentifier_len = 0U,
		.aki_has_generalNames_and_serial = 0,
		.aki_generalNames_start = 0U,
		.aki_generalNames_len = 0U,
		.aki_serial_start = 0U,
		.aki_serial_len = 0U,
		.has_san = 0,
		.san_critical = 0,
		.bc_critical = 0,
		.ca_true = 0,
		.pathLenConstraint_set = 0,
		.has_keyUsage = 0,
		.keyCertSign_set = 0,
		.cRLSign_set = 0,
		.has_eku = 0,
		.has_crldp = 0,
		.one_crldp_has_all_reasons = 0,
		.has_name_constraints = 0,
		.sig_alg_start = 0U,
		.sig_alg_len = 0U,
		.sig_alg = (sig_alg_id)0,
		.hash_alg = (hash_alg_id)0,
		.sig_alg_params = {.dsa = {.r_raw_off = 0U,
				.r_raw_len = 0U,
				.s_raw_off = 0U,
		.s_raw_len = 0U}},
		.sig_start = 0U,
	.sig_len = 0U};
	return zeroized_ctx;
}

int parse_x509_cert(cert_parsing_ctx *ctx, u8 const *cert, u32 len)
{
	int ret;
	u32 seq_data_len = (u32)0;
	u32 eaten = (u32)0;
	u32 off = (u32)0;
	_sig_alg const *sig_alg = (_sig_alg const *)0;
	if (cert == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (ctx == (cert_parsing_ctx *)0) {
		_LOR: {
			ret = - (2 * 100000 + 7168);
			goto out;
		}
	}
/*@ assert rte_633: mem_access: \valid(ctx); */
	*ctx = get_zeroized_cert_ctx_val();
	ret = parse_id_len(cert,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& eaten,& seq_data_len);
	if (ret) goto out;
	len -= eaten;
	off += eaten;
	if (seq_data_len != len) {
		ret = - (2 * 100000 + 7195);
		goto out;
	}
	ret = parse_x509_tbsCertificate(ctx,cert,off,len,& sig_alg,& eaten);
	if (ret) goto out;
/*@ assert rte_634: mem_access: \valid(&ctx->tbs_start); */
	ctx->tbs_start = off;
/*@ assert rte_635: mem_access: \valid(&ctx->tbs_len); */
	ctx->tbs_len = eaten;
	len -= eaten;
	off += eaten;
	ret = parse_x509_signatureAlgorithm(ctx,cert,off,len,& eaten);
	if (ret) goto out;
	len -= eaten;
	off += eaten;
	ret = parse_x509_signatureValue(ctx,cert,off,len,sig_alg,& eaten);
	if (ret) goto out;
	if (len != eaten) {
		ret = - (2 * 100000 + 7239);
		goto out;
	}
	ret = 0;
	out: ;
	return ret;
}

int parse_x509_cert_relaxed(cert_parsing_ctx *ctx, u8 const *buf, u32 len,
u32 *eaten);

int parse_x509_crl_relaxed(crl_parsing_ctx *ctx, u8 const *buf, u32 len,
u32 *eaten);

int parse_x509_cert_relaxed(cert_parsing_ctx *ctx, u8 const *buf, u32 len,
u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (ctx == (cert_parsing_ctx *)0) goto _LOR;
	else
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (4 * 100000 + 35);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) {
		ret = 1;
		goto out;
	}
/*@ assert rte_761: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = parse_x509_cert(ctx,buf,hdr_len + data_len);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}

int parse_x509_crl_relaxed(crl_parsing_ctx *ctx, u8 const *buf, u32 len,
u32 *eaten)
{
	int ret;
	u32 hdr_len = (u32)0;
	u32 data_len = (u32)0;
	if (ctx == (crl_parsing_ctx *)0) goto _LOR;
	else
	if (buf == (u8 const *)0) goto _LOR;
	else
	if (len == (u32)0) goto _LOR;
	else
	if (eaten == (u32 *)0) {
		_LOR: {
			ret = - (4 * 100000 + 87);
			goto out;
		}
	}
	ret = parse_id_len(buf,len,CLASS_UNIVERSAL,(u32)ASN1_TYPE_SEQUENCE,
	& hdr_len,& data_len);
	if (ret) {
		ret = 1;
		goto out;
	}
/*@ assert rte_762: mem_access: \valid(eaten); */
	*eaten = hdr_len + data_len;
	ret = parse_x509_crl(ctx,buf,hdr_len + data_len);
	if (ret) goto out;
	ret = 0;
	out: ;
	return ret;
}


