#include <chipram_env.h>

chipram_env_t  local_chipram_env;

void setup_chipram_env(void)
{
	chipram_env_t * env = (chipram_env_t *)CHIPRAM_ENV_LOCATION;

	if (CHIPRAM_ENV_MAGIC != env->magic) {
		errorf("Chipram magic wrong ,mode data may be broken\n");
	}

	local_chipram_env = * env;
}

chipram_env_t* get_chipram_env(void)
{
	return &local_chipram_env;
}

boot_mode_t get_boot_role(void)
{
	boot_mode_t boot_role;
	chipram_env_t* cr_env = get_chipram_env();
	boot_role = cr_env->mode;
	return boot_role;
}