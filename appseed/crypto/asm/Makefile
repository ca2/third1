PERL	?= perl
PERL	+= -I../openssl/crypto/perlasm -I../openssl/crypto/bn/asm

OUTPUTS	= \
	x86-elf-gas/aes/aes-586.s \
	x86-elf-gas/aes/aesni-x86.s \
	x86-elf-gas/bf/bf-686.s \
	x86-elf-gas/bn/x86-mont.s \
	x86-elf-gas/bn/x86.s \
	x86-elf-gas/camellia/cmll-x86.s \
	x86-elf-gas/cast/cast-586.s \
	x86-elf-gas/des/crypt586.s \
	x86-elf-gas/des/des-586.s \
	x86-elf-gas/md5/md5-586.s \
	x86-elf-gas/rc4/rc4-586.s \
	x86-elf-gas/rc5/rc5-586.s \
	x86-elf-gas/ripemd/rmd-586.s \
	x86-elf-gas/sha/sha1-586.s \
	x86-elf-gas/sha/sha256-586.s \
	x86-elf-gas/sha/sha512-586.s \
	x86-elf-gas/whrlpool/wp-mmx.s \
	x86-elf-gas/x86cpuid.s \
	x64-elf-gas/aes/aes-x86_64.s \
	x64-elf-gas/aes/aesni-x86_64.s \
	x64-elf-gas/aes/aesni-sha1-x86_64.s \
	x64-elf-gas/bn/modexp512-x86_64.s \
	x64-elf-gas/bn/x86_64-mont.s \
	x64-elf-gas/camellia/cmll-x86_64.s \
	x64-elf-gas/md5/md5-x86_64.s \
	x64-elf-gas/rc4/rc4-x86_64.s \
	x64-elf-gas/rc4/rc4-md5-x86_64.s \
	x64-elf-gas/sha/sha1-x86_64.s \
	x64-elf-gas/sha/sha512-x86_64.s \
	x64-elf-gas/whrlpool/wp-x86_64.s \
	x64-elf-gas/x86_64cpuid.s \
	x86-macosx-gas/aes/aes-586.s \
	x86-macosx-gas/aes/aesni-x86.s \
	x86-macosx-gas/bf/bf-686.s \
	x86-macosx-gas/bn/x86-mont.s \
	x86-macosx-gas/bn/x86.s \
	x86-macosx-gas/camellia/cmll-x86.s \
	x86-macosx-gas/cast/cast-586.s \
	x86-macosx-gas/des/crypt586.s \
	x86-macosx-gas/des/des-586.s \
	x86-macosx-gas/md5/md5-586.s \
	x86-macosx-gas/rc4/rc4-586.s \
	x86-macosx-gas/rc5/rc5-586.s \
	x86-macosx-gas/ripemd/rmd-586.s \
	x86-macosx-gas/sha/sha1-586.s \
	x86-macosx-gas/sha/sha256-586.s \
	x86-macosx-gas/sha/sha512-586.s \
	x86-macosx-gas/whrlpool/wp-mmx.s \
	x86-macosx-gas/x86cpuid.s \
	x64-macosx-gas/aes/aes-x86_64.s \
	x64-macosx-gas/aes/aesni-x86_64.s \
	x64-macosx-gas/aes/aesni-sha1-x86_64.s \
	x64-macosx-gas/bn/modexp512-x86_64.s \
	x64-macosx-gas/bn/x86_64-mont.s \
	x64-macosx-gas/camellia/cmll-x86_64.s \
	x64-macosx-gas/md5/md5-x86_64.s \
	x64-macosx-gas/rc4/rc4-x86_64.s \
	x64-macosx-gas/rc4/rc4-md5-x86_64.s \
	x64-macosx-gas/sha/sha1-x86_64.s \
	x64-macosx-gas/sha/sha512-x86_64.s \
	x64-macosx-gas/whrlpool/wp-x86_64.s \
	x64-macosx-gas/x86_64cpuid.s \
	x86-win32-masm/aes/aes-586.asm \
	x86-win32-masm/aes/aesni-x86.asm \
	x86-win32-masm/bf/bf-686.asm \
	x86-win32-masm/bn/x86-mont.asm \
	x86-win32-masm/bn/x86.asm \
	x86-win32-masm/camellia/cmll-x86.asm \
	x86-win32-masm/cast/cast-586.asm \
	x86-win32-masm/des/crypt586.asm \
	x86-win32-masm/des/des-586.asm \
	x86-win32-masm/md5/md5-586.asm \
	x86-win32-masm/rc4/rc4-586.asm \
	x86-win32-masm/rc5/rc5-586.asm \
	x86-win32-masm/ripemd/rmd-586.asm \
	x86-win32-masm/sha/sha1-586.asm \
	x86-win32-masm/sha/sha256-586.asm \
	x86-win32-masm/sha/sha512-586.asm \
	x86-win32-masm/whrlpool/wp-mmx.asm \
	x86-win32-masm/x86cpuid.asm \
	x64-win32-masm/aes/aes-x86_64.asm \
	x64-win32-masm/aes/aesni-x86_64.asm \
	x64-win32-masm/aes/aesni-sha1-x86_64.asm \
	x64-win32-masm/bn/modexp512-x86_64.asm \
	x64-win32-masm/bn/x86_64-mont.asm \
	x64-win32-masm/camellia/cmll-x86_64.asm \
	x64-win32-masm/md5/md5-x86_64.asm \
	x64-win32-masm/rc4/rc4-x86_64.asm \
	x64-win32-masm/rc4/rc4-md5-x86_64.asm \
	x64-win32-masm/sha/sha1-x86_64.asm \
	x64-win32-masm/sha/sha512-x86_64.asm \
	x64-win32-masm/whrlpool/wp-x86_64.asm \
	x64-win32-masm/x86_64cpuid.asm \

x64-elf-gas/%.s x86-elf-gas/%.s:
	$(PERL) $< elf > $@

x64-macosx-gas/%.s x86-macosx-gas/%.s:
	$(PERL) $< macosx > $@

x64-win32-masm/%.asm:
	$(PERL) $< masm > $@

x86-win32-masm/%.asm:
	$(PERL) $< win32 > $@

.PHONY:	all
all:	$(OUTPUTS)
	# strip trailing whitespace and final blank newline
	sed -sri -e 's/\s+$$/\n/' -e '$$ { /^$$/d }' $^

clean:
	find . -iname '*.asm' -exec rm "{}" \;
	find . -iname '*.s' -exec rm "{}" \;

x64-elf-gas/aes/aes-x86_64.s: ../openssl/crypto/aes/asm/aes-x86_64.pl
x64-elf-gas/aes/aesni-x86_64.s: ../openssl/crypto/aes/asm/aesni-x86_64.pl
x64-elf-gas/aes/aesni-sha1-x86_64.s: ../openssl/crypto/aes/asm/aesni-sha1-x86_64.pl
x64-elf-gas/bn/modexp512-x86_64.s: ../openssl/crypto/bn/asm/modexp512-x86_64.pl
x64-elf-gas/bn/x86_64-mont.s: ../openssl/crypto/bn/asm/x86_64-mont.pl
x64-elf-gas/camellia/cmll-x86_64.s: ../openssl/crypto/camellia/asm/cmll-x86_64.pl
x64-elf-gas/md5/md5-x86_64.s: ../openssl/crypto/md5/asm/md5-x86_64.pl
x64-elf-gas/rc4/rc4-x86_64.s: ../openssl/crypto/rc4/asm/rc4-x86_64.pl
x64-elf-gas/rc4/rc4-md5-x86_64.s: ../openssl/crypto/rc4/asm/rc4-md5-x86_64.pl
x64-elf-gas/sha/sha1-x86_64.s: ../openssl/crypto/sha/asm/sha1-x86_64.pl
x64-elf-gas/sha/sha512-x86_64.s: ../openssl/crypto/sha/asm/sha512-x86_64.pl
x64-elf-gas/whrlpool/wp-x86_64.s: ../openssl/crypto/whrlpool/asm/wp-x86_64.pl
x64-elf-gas/x86_64cpuid.s: ../openssl/crypto/x86_64cpuid.pl
x64-macosx-gas/aes/aes-x86_64.s: ../openssl/crypto/aes/asm/aes-x86_64.pl
x64-macosx-gas/aes/aesni-x86_64.s: ../openssl/crypto/aes/asm/aesni-x86_64.pl
x64-macosx-gas/aes/aesni-sha1-x86_64.s: ../openssl/crypto/aes/asm/aesni-sha1-x86_64.pl
x64-macosx-gas/bn/modexp512-x86_64.s: ../openssl/crypto/bn/asm/modexp512-x86_64.pl
x64-macosx-gas/bn/x86_64-mont.s: ../openssl/crypto/bn/asm/x86_64-mont.pl
x64-macosx-gas/camellia/cmll-x86_64.s: ../openssl/crypto/camellia/asm/cmll-x86_64.pl
x64-macosx-gas/md5/md5-x86_64.s: ../openssl/crypto/md5/asm/md5-x86_64.pl
x64-macosx-gas/rc4/rc4-x86_64.s: ../openssl/crypto/rc4/asm/rc4-x86_64.pl
x64-macosx-gas/rc4/rc4-md5-x86_64.s: ../openssl/crypto/rc4/asm/rc4-md5-x86_64.pl
x64-macosx-gas/sha/sha1-x86_64.s: ../openssl/crypto/sha/asm/sha1-x86_64.pl
x64-macosx-gas/sha/sha512-x86_64.s: ../openssl/crypto/sha/asm/sha512-x86_64.pl
x64-macosx-gas/whrlpool/wp-x86_64.s: ../openssl/crypto/whrlpool/asm/wp-x86_64.pl
x64-macosx-gas/x86_64cpuid.s: ../openssl/crypto/x86_64cpuid.pl
x64-win32-masm/aes/aes-x86_64.asm: ../openssl/crypto/aes/asm/aes-x86_64.pl
x64-win32-masm/aes/aesni-x86_64.asm: ../openssl/crypto/aes/asm/aesni-x86_64.pl
x64-win32-masm/aes/aesni-sha1-x86_64.asm: ../openssl/crypto/aes/asm/aesni-sha1-x86_64.pl
x64-win32-masm/bn/modexp512-x86_64.asm: ../openssl/crypto/bn/asm/modexp512-x86_64.pl
x64-win32-masm/bn/x86_64-mont.asm: ../openssl/crypto/bn/asm/x86_64-mont.pl
x64-win32-masm/camellia/cmll-x86_64.asm: ../openssl/crypto/camellia/asm/cmll-x86_64.pl
x64-win32-masm/md5/md5-x86_64.asm: ../openssl/crypto/md5/asm/md5-x86_64.pl
x64-win32-masm/rc4/rc4-x86_64.asm: ../openssl/crypto/rc4/asm/rc4-x86_64.pl
x64-win32-masm/rc4/rc4-md5-x86_64.asm: ../openssl/crypto/rc4/asm/rc4-md5-x86_64.pl
x64-win32-masm/sha/sha1-x86_64.asm: ../openssl/crypto/sha/asm/sha1-x86_64.pl
x64-win32-masm/sha/sha512-x86_64.asm: ../openssl/crypto/sha/asm/sha512-x86_64.pl
x64-win32-masm/whrlpool/wp-x86_64.asm: ../openssl/crypto/whrlpool/asm/wp-x86_64.pl
x64-win32-masm/x86_64cpuid.asm: ../openssl/crypto/x86_64cpuid.pl
x86-elf-gas/aes/aes-586.s: ../openssl/crypto/aes/asm/aes-586.pl
x86-elf-gas/aes/aesni-x86.s: ../openssl/crypto/aes/asm/aesni-x86.pl
x86-elf-gas/bf/bf-686.s: ../openssl/crypto/bf/asm/bf-686.pl
x86-elf-gas/bn/x86-mont.s: ../openssl/crypto/bn/asm/x86-mont.pl
x86-elf-gas/bn/x86.s: ../openssl/crypto/bn/asm/x86.pl
x86-elf-gas/camellia/cmll-x86.s: ../openssl/crypto/camellia/asm/cmll-x86.pl
x86-elf-gas/cast/cast-586.s: ../openssl/crypto/cast/asm/cast-586.pl
x86-elf-gas/des/crypt586.s: ../openssl/crypto/des/asm/crypt586.pl
x86-elf-gas/des/des-586.s: ../openssl/crypto/des/asm/des-586.pl
x86-elf-gas/md5/md5-586.s: ../openssl/crypto/md5/asm/md5-586.pl
x86-elf-gas/rc4/rc4-586.s: ../openssl/crypto/rc4/asm/rc4-586.pl
x86-elf-gas/rc5/rc5-586.s: ../openssl/crypto/rc5/asm/rc5-586.pl
x86-elf-gas/ripemd/rmd-586.s: ../openssl/crypto/ripemd/asm/rmd-586.pl
x86-elf-gas/sha/sha1-586.s: ../openssl/crypto/sha/asm/sha1-586.pl
x86-elf-gas/sha/sha256-586.s: ../openssl/crypto/sha/asm/sha256-586.pl
x86-elf-gas/sha/sha512-586.s: ../openssl/crypto/sha/asm/sha512-586.pl
x86-elf-gas/whrlpool/wp-mmx.s: ../openssl/crypto/whrlpool/asm/wp-mmx.pl
x86-elf-gas/x86cpuid.s: ../openssl/crypto/x86cpuid.pl
x86-macosx-gas/aes/aes-586.s: ../openssl/crypto/aes/asm/aes-586.pl
x86-macosx-gas/aes/aesni-x86.s: ../openssl/crypto/aes/asm/aesni-x86.pl
x86-macosx-gas/bf/bf-686.s: ../openssl/crypto/bf/asm/bf-686.pl
x86-macosx-gas/bn/x86-mont.s: ../openssl/crypto/bn/asm/x86-mont.pl
x86-macosx-gas/bn/x86.s: ../openssl/crypto/bn/asm/x86.pl
x86-macosx-gas/camellia/cmll-x86.s: ../openssl/crypto/camellia/asm/cmll-x86.pl
x86-macosx-gas/cast/cast-586.s: ../openssl/crypto/cast/asm/cast-586.pl
x86-macosx-gas/des/crypt586.s: ../openssl/crypto/des/asm/crypt586.pl
x86-macosx-gas/des/des-586.s: ../openssl/crypto/des/asm/des-586.pl
x86-macosx-gas/md5/md5-586.s: ../openssl/crypto/md5/asm/md5-586.pl
x86-macosx-gas/rc4/rc4-586.s: ../openssl/crypto/rc4/asm/rc4-586.pl
x86-macosx-gas/rc5/rc5-586.s: ../openssl/crypto/rc5/asm/rc5-586.pl
x86-macosx-gas/ripemd/rmd-586.s: ../openssl/crypto/ripemd/asm/rmd-586.pl
x86-macosx-gas/sha/sha1-586.s: ../openssl/crypto/sha/asm/sha1-586.pl
x86-macosx-gas/sha/sha256-586.s: ../openssl/crypto/sha/asm/sha256-586.pl
x86-macosx-gas/sha/sha512-586.s: ../openssl/crypto/sha/asm/sha512-586.pl
x86-macosx-gas/whrlpool/wp-mmx.s: ../openssl/crypto/whrlpool/asm/wp-mmx.pl
x86-macosx-gas/x86cpuid.s: ../openssl/crypto/x86cpuid.pl
x86-win32-masm/aes/aes-586.asm: ../openssl/crypto/aes/asm/aes-586.pl
x86-win32-masm/aes/aesni-x86.asm: ../openssl/crypto/aes/asm/aesni-x86.pl
x86-win32-masm/bf/bf-686.asm: ../openssl/crypto/bf/asm/bf-686.pl
x86-win32-masm/bn/x86.asm: ../openssl/crypto/bn/asm/x86.pl
x86-win32-masm/bn/x86-mont.asm: ../openssl/crypto/bn/asm/x86-mont.pl
x86-win32-masm/camellia/cmll-x86.asm: ../openssl/crypto/camellia/asm/cmll-x86.pl
x86-win32-masm/cast/cast-586.asm: ../openssl/crypto/cast/asm/cast-586.pl
x86-win32-masm/des/crypt586.asm: ../openssl/crypto/des/asm/crypt586.pl
x86-win32-masm/des/des-586.asm: ../openssl/crypto/des/asm/des-586.pl
x86-win32-masm/md5/md5-586.asm: ../openssl/crypto/md5/asm/md5-586.pl
x86-win32-masm/rc4/rc4-586.asm: ../openssl/crypto/rc4/asm/rc4-586.pl
x86-win32-masm/rc5/rc5-586.asm: ../openssl/crypto/rc5/asm/rc5-586.pl
x86-win32-masm/ripemd/rmd-586.asm: ../openssl/crypto/ripemd/asm/rmd-586.pl
x86-win32-masm/sha/sha1-586.asm: ../openssl/crypto/sha/asm/sha1-586.pl
x86-win32-masm/sha/sha256-586.asm: ../openssl/crypto/sha/asm/sha256-586.pl
x86-win32-masm/sha/sha512-586.asm: ../openssl/crypto/sha/asm/sha512-586.pl
x86-win32-masm/whrlpool/wp-mmx.asm: ../openssl/crypto/whrlpool/asm/wp-mmx.pl
x86-win32-masm/x86cpuid.asm: ../openssl/crypto/x86cpuid.pl
