#include "fractol_common.h"
#include <sys/sysctl.h>

int hw_get_logicalcpu(void) {
	size_t cores;
	size_t cores_max;
	size_t size;

	size = sizeof(size_t);
	sysctlbyname("hw.logicalcpu", &cores, &size, NULL, 0);
	sysctlbyname("hw.logicalcpu_max", &cores_max, &size, NULL, 0);
	if (cores_max != cores) {
		ft_dprintf(2, "fractol: warning: hello\n");
	}
	return (cores);
}

int hw_is_avx_available(void) {

}
