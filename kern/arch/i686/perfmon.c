#include <arch/perfmon.h> 


static void setup_counter(int index, uint8_t mask, uint8_t event) {
 	//just a magic number for now to indicate the most common case
	uint32_t os_user_enabled = 0x43;
	write_msr(IA32_PERFEVTSEL_BASE + index, (os_user_enabled<<16 | mask<<8 | event));
}

void perfmon_init() {
#ifdef __CONFIG_OSDI__
	//setting up to collect cache miss behavior specifically for OSDI
	setup_counter(0, LLCACHE_REF_MASK, LLCACHE_EVENT);
	setup_counter(1, LLCACHE_MISS_MASK, LLCACHE_EVENT);

  //enable user level access to the performance counters
  lcr4(rcr4() | CR4_PCE);
#endif
}
