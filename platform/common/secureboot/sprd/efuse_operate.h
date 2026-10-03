#ifndef _EFUSE_OPERATE_H_
#define _EFUSE_OPERATE_H_

int arm7_key_write( char *key, u32 count);
int arm7_key_read( char *key, u32 count);
int efuse_hash_write( char *hash, u32 count);
int efuse_hash_read(char *hash, u32 count);
int efuse_uid_read(char *uid, int count);
int secure_efuse_program_sprd(void);
int efuse_rollback_version_read(int type);

#endif/*_EFUSE_OPERATE_H_*/
