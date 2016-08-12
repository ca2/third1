// O volume na unidade Z ‚ eco
// O N£mero de S‚rie do Volume ‚ 4A91-94E4
//
// Pasta de Z:\ca2\app\axis\crypto\crypto
//
//#include "armcap.c"
#include "cpt_err.c"
#include "cryptlib.c"
#if !defined(LINUX) && !defined(__APPLE__)
#include "cversion.c"
#endif
#include "ebcdic.c"
#include "ex_data.c"
#include "fips_ers.c"
//#include "LPdir_nyi.c"
//#include "LPdir_unix.c"
//#include "LPdir_vms.c"
//#include "LPdir_win.c"
//#include "LPdir_win32.c"
//#include "LPdir_wince.c"

//#include "mem.c"
//#include "mem_aura.c"



#if !defined(LINUX) && !defined(__APPLE__)
#include "mem_clr.c"
#endif
#if defined(__APPLE__) && defined(OPENSSL_NO_ASM)
#include "mem_clr.c"
#endif
#include "mem_dbg.c"


//#ifdef METROWIN
//int _getpid()
//{
//   return 0;
//}
//#else
#include "o_dir.c"
//#endif
//#include "o_dir_test.c"
#include "o_fips.c"
#include "o_init.c"
#include "o_str.c"
#include "o_time.c"
//#include "ppccap.c"
//#include "s390xcap.c"
//#include "sparcv9cap.c"
#include "uid.c"
//#include "_crypto.c"
//#include "_crypto_all.c"
//#include "_crypto_vsnord.c"
//#include "_crypto_vsnordl.c"
#include "aes/_aes.c"
#include "asn1/_a.c"
#include "bf/_bf.c"
#include "bio/_bio.c"
#include "buffer/_buffer.c"
#include "camellia/_cmll.c"
#include "cast/_cast.c"
#include "cms/_cms.c"
#include "conf/_conf.c"
#include "des/_des.c"
#include "dh/_dh.c"
#include "dsa/_dsa.c"
#include "dso/_dso.c"
#include "ec/_ec.c"
#include "ecdh/_ech.c"
#include "ecdsa/_ecs.c"
#include "engine/_engine.c"
#include "evp/_evp.c"
#include "idea/_i.c"
#include "modes/_modes.c"
#include "objects/_obj.c"
#include "ocsp/_ocsp.c"
#include "pem/_pem.c"
#include "pkcs12/_p12.c"
#include "pkcs7/_p7.c"
#include "rand/_rand.c"
#include "rc2/_rc2.c"
#if defined(OPENSSL_NO_ASM)
#include "rc4/rc4_enc.c"
#include "rc4/rc4_skey.c"
#endif
#include "rsa/_rsa.c"
//#include "ts/_ts.c"
#include "ui/_ui.c"
//#include "x509/_x509.c"
//#include "x509v3/_x509v3.c"
#include "seed/_seed.c"
#include "comp/c_rle.c"
#include "comp/c_zlib.c"
#include "comp/comp_lib.c"
#include "comp/comp_err.c"
#include "err/err.c"
#include "err/err_all.c"
#include "err/err_prn.c"
#include "_digest.c"


              //31 arquivo(s)        250.682 bytes
              // 0 pasta(s)   29.041.623.040 bytes dispon¡veis
