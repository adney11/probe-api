set(CPPSOURCES "")

FILE(GLOB CPPSOURCES_TMP
	${DEPS_DIR}/openssl/*.h
	${DEPS_DIR}/openssl/ssl/*.c
	${DEPS_DIR}/openssl/ssl/*.h
	${DEPS_DIR}/openssl/ssl/record/*.c
	${DEPS_DIR}/openssl/ssl/record/*.h
	)
set(CPPSOURCES ${CPPSOURCES} ${CPPSOURCES_TMP})

# Include recursively:
FILE(GLOB_RECURSE CPPSOURCES_TMP
	${DEPS_DIR}/openssl/crypto/*.c
	${DEPS_DIR}/openssl/crypto/*.h
	)
set(CPPSOURCES ${CPPSOURCES} ${CPPSOURCES_TMP})

if(WIN32 OR CYGWIN)
	FILE(GLOB CPPSOURCES_TMP
		${DEPS_DIR}/openssl/ms/*.c
		${DEPS_DIR}/openssl/ms/*.h
		)
set(CPPSOURCES ${CPPSOURCES} ${CPPSOURCES_TMP})
endif()

# Exclude some files (could not be compiled for me):
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/conf/cnf_save.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/conf/test.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/ec/ecp_nistz256_table.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/evp/e_dsa.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/jpake/jpake.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/jpake/jpake_err.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/lhash/lh_test.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/LPdir_nyi.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/LPdir_unix.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/LPdir_vms.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/LPdir_win.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/LPdir_win32.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/LPdir_wince.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/md2/md2_dgst.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/md2/md2_one.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/o_dir_test.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/ppccap.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/pqueue/pq_test.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/rc5/rc5cfb64.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/rc5/rc5ofb64.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/rc5/rc5_ecb.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/rc5/rc5_enc.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/rc5/rc5_skey.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/s390xcap.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/sparcv9cap.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/store/str_err.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/store/str_lib.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/store/str_mem.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/store/str_meth.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/threads/mttest.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/x509v3/v3conf.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/x509v3/v3prin.c")

# Exclude some files (duplicated functions defined):
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/des/ncbc_enc.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/bn/asm/x86_64-gcc.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/bf/bf_cbc.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/armcap.c")
list(REMOVE_ITEM CPPSOURCES "${DEPS_DIR}/openssl/crypto/aes/aes_x86core.c")

add_library(openssl STATIC ${CPPSOURCES} ${CPPSOURCES_2} ${CPPSOURCES_3})

target_include_directories(openssl PRIVATE ${DEPS_DIR}/openssl)
target_include_directories(openssl PUBLIC ${DEPS_DIR}/openssl/include)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/openssl/crypto)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/openssl/crypto/asn1)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/openssl/crypto/evp)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/openssl/crypto/modes)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/openssl/crypto/include)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/build.projects)
target_include_directories(openssl PRIVATE ${DEPS_DIR}/build.projects/openssl)


target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_DEPRECATED)
target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_ASM)
target_compile_definitions(openssl PRIVATE -DOPENSSL_THREADS)
target_compile_definitions(openssl PRIVATE -DOPENSSL_USE_APPLINK)
target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_RC5)
target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_MD2)
target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_KRB5)
target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_JPAKE)
target_compile_definitions(openssl PRIVATE -DOPENSSL_NO_STATIC_ENGINE)

if(WIN32 OR CYGWIN)
	target_compile_definitions(openssl PRIVATE -DOPENSSL_SYSNAME_WIN32)
	target_compile_definitions(openssl PRIVATE -DNO_WINDOWS_BRAINDEATH)
	target_compile_definitions(openssl PRIVATE -DWIN32_LEAN_AND_MEAN)
endif()

if(MSVC)
	target_compile_definitions(openssl PRIVATE -D_CRT_SECURE_NO_DEPRECATE)
	target_compile_definitions(openssl PRIVATE -D_CRT_NONSTDC_NO_WARNINGS)
	target_compile_definitions(openssl PRIVATE -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()
