/* builtin command list - automatically generated, do not edit */
#include <nuttx/config.h>
#include <nuttx/binfmt/builtin.h>
extern int sercon_main(int argc, char *argv[]);
extern int serdis_main(int argc, char *argv[]);
extern int perf_main(int argc, char *argv[]);
extern int reboot_main(int argc, char *argv[]);
extern int uorb_main(int argc, char *argv[]);
const struct builtin_s g_builtins[] = {
    {"sercon", SCHED_PRIORITY_DEFAULT, 2048, sercon_main},
    {"serdis", SCHED_PRIORITY_DEFAULT, 2048, serdis_main},
    {"perf", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, perf_main},
    {"reboot", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, reboot_main},
    {"uorb", SCHED_PRIORITY_DEFAULT, 4096, uorb_main},
    {NULL, 0, 0, NULL}
};
const int g_builtin_count = 5;
