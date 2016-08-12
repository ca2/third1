 //O volume na unidade Z ‚ eco
 //O N£mero de S‚rie do Volume ‚ 4A91-94E4

 //Pasta de Z:\ca2\app\axis\openssl\crypto\evp

#include "bio_b64.c"
#include "bio_enc.c"
#include "bio_md.c"
#include "bio_ok.c"
#include "c_all.c"
#include "c_allc.c"
#include "c_alld.c"
#include "digest.c"
#include "encode.c"
#include "evp_acnf.c"
#include "evp_cnf.c"
#include "evp_enc.c"
#include "evp_err.c"
#include "evp_fips.c"
#include "evp_key.c"
#include "evp_lib.c"
#include "evp_pbe.c"
#include "evp_pkey.c"
//#include "evp_test.c"
#include "e_aes.c"
#include "e_aes_cbc_hmac_sha1.c"
#include "e_bf.c"
#include "e_camellia.c"
#include "e_cast.c"
#include "e_des.c"
#include "e_des3.c"
//#include "e_dsa.c"
#include "e_idea.c"
#include "e_null.c"
#include "e_old.c"
#include "e_rc2.c"
#include "e_rc4.c"
#include "e_rc4_hmac_md5.c"
#include "e_rc5.c"
#include "e_seed.c"
#include "e_xcbc_d.c"
#define init init_dss
#define update update_dss
#define final final_dss
#include "m_dss.c"
#undef init
#undef update
#undef final
#define init init_dss1
#define update update_dss1
#define final final_dss1
#include "m_dss1.c"
#undef init
#undef update
#undef final
#define init init_ecdsa
#define update update_ecdsa
#define final final_ecdsa
#include "m_ecdsa.c"
#undef init
#undef update
#undef final
#define init init_md2
#define update update_md2
#define final final_md2
#include "m_md2.c"
#undef init
#undef update
#undef final
#define init init_md4
#define update update_md4
#define final final_md4
#include "m_md4.c"
#undef init
#undef update
#undef final
#define init init_md5
#define update update_md5
#define final final_md5
#include "m_md5.c"
#undef init
#undef update
#undef final
#define init init_mdc2
#define update update_mdc2
#define final final_mdc2
#include "m_mdc2.c"
#undef init
#undef update
#undef final
#define init init_null
#define update update_null
#define final final_null
#include "m_null.c"
#undef init
#undef update
#undef final
#define init init_ripemd2
#define update update_ripemd2
#define final final_ripemd2
#include "m_ripemd.c"
#undef init
#undef update
#undef final
#define init init_sha
#define update update_sha
#define final final_sha
#include "m_sha.c"
#undef init
#undef update
#undef final
#define init init_sha1
#define update update_sha1
#define final final_sha1
#include "m_sha1.c"
#undef init
#undef update
#undef final
//#define init init_sigver
//#define update update_sigver
//#define final final_sigver
#include "m_sigver.c"
#include "m_wp.c"
#include "names.c"
#include "openbsd_hw.c"
#include "p5_crpt.c"
#include "p5_crpt2.c"
#include "pmeth_fn.c"
#include "pmeth_gn.c"
#include "pmeth_lib.c"
#include "p_dec.c"
#include "p_enc.c"
#include "p_lib.c"
#include "p_open.c"
#include "p_seal.c"
#include "p_sign.c"
#include "p_verify.c"
//2015-01-03  21:49                 0 _evp.c"
//              64 arquivo(s)        477.765 bytes
//               0 pasta(s)   36.919.476.224 bytes dispon¡veis
