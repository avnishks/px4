/* builtin command list - automatically generated, do not edit */
#include <nuttx/config.h>
#include <nuttx/binfmt/builtin.h>
extern int sercon_main(int argc, char *argv[]);
extern int serdis_main(int argc, char *argv[]);
extern int adc_main(int argc, char *argv[]);
extern int attitude_estimator_ekf_main(int argc, char *argv[]);
extern int att_pos_estimator_ekf_main(int argc, char *argv[]);
extern int blinkm_main(int argc, char *argv[]);
extern int bl_update_main(int argc, char *argv[]);
extern int boardinfo_main(int argc, char *argv[]);
extern int commander_main(int argc, char *argv[]);
extern int commander_tests_main(int argc, char *argv[]);
extern int config_main(int argc, char *argv[]);
extern int ets_airspeed_main(int argc, char *argv[]);
extern int flow_position_estimator_main(int argc, char *argv[]);
extern int fmu_main(int argc, char *argv[]);
extern int gps_main(int argc, char *argv[]);
extern int hil_main(int argc, char *argv[]);
extern int hmc5883_main(int argc, char *argv[]);
extern int hott_sensors_main(int argc, char *argv[]);
extern int hott_telemetry_main(int argc, char *argv[]);
extern int l3gd20_main(int argc, char *argv[]);
extern int lsm303d_main(int argc, char *argv[]);
extern int mavlink_onboard_main(int argc, char *argv[]);
extern int mavlink_main(int argc, char *argv[]);
extern int mb12xx_main(int argc, char *argv[]);
extern int meas_airspeed_main(int argc, char *argv[]);
extern int mixer_main(int argc, char *argv[]);
extern int ms5611_main(int argc, char *argv[]);
extern int multirotor_att_control_main(int argc, char *argv[]);
extern int multirotor_pos_control_main(int argc, char *argv[]);
extern int nshterm_main(int argc, char *argv[]);
extern int param_main(int argc, char *argv[]);
extern int perf_main(int argc, char *argv[]);
extern int position_estimator_inav_main(int argc, char *argv[]);
extern int preflight_check_main(int argc, char *argv[]);
extern int pwm_main(int argc, char *argv[]);
extern int px4io_main(int argc, char *argv[]);
extern int ramtron_main(int argc, char *argv[]);
extern int reboot_main(int argc, char *argv[]);
extern int rgbled_main(int argc, char *argv[]);
extern int sdlog2_main(int argc, char *argv[]);
extern int sensors_main(int argc, char *argv[]);
extern int tests_main(int argc, char *argv[]);
extern int tone_alarm_main(int argc, char *argv[]);
extern int top_main(int argc, char *argv[]);
extern int uorb_main(int argc, char *argv[]);
const struct builtin_s g_builtins[] = {
    {"sercon", SCHED_PRIORITY_DEFAULT, 2048, sercon_main},
    {"serdis", SCHED_PRIORITY_DEFAULT, 2048, serdis_main},
    {"adc", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, adc_main},
    {"attitude_estimator_ekf", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, attitude_estimator_ekf_main},
    {"att_pos_estimator_ekf", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, att_pos_estimator_ekf_main},
    {"blinkm", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, blinkm_main},
    {"bl_update", SCHED_PRIORITY_DEFAULT, 4096, bl_update_main},
    {"boardinfo", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, boardinfo_main},
    {"commander", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, commander_main},
    {"commander_tests", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, commander_tests_main},
    {"config", SCHED_PRIORITY_DEFAULT, 4096, config_main},
    {"ets_airspeed", SCHED_PRIORITY_DEFAULT, 2048, ets_airspeed_main},
    {"flow_position_estimator", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, flow_position_estimator_main},
    {"fmu", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, fmu_main},
    {"gps", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, gps_main},
    {"hil", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, hil_main},
    {"hmc5883", SCHED_PRIORITY_DEFAULT, 4096, hmc5883_main},
    {"hott_sensors", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, hott_sensors_main},
    {"hott_telemetry", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, hott_telemetry_main},
    {"l3gd20", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, l3gd20_main},
    {"lsm303d", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, lsm303d_main},
    {"mavlink_onboard", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, mavlink_onboard_main},
    {"mavlink", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, mavlink_main},
    {"mb12xx", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, mb12xx_main},
    {"meas_airspeed", SCHED_PRIORITY_DEFAULT, 2048, meas_airspeed_main},
    {"mixer", SCHED_PRIORITY_DEFAULT, 4096, mixer_main},
    {"ms5611", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, ms5611_main},
    {"multirotor_att_control", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, multirotor_att_control_main},
    {"multirotor_pos_control", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, multirotor_pos_control_main},
    {"nshterm", SCHED_PRIORITY_DEFAULT, 3000, nshterm_main},
    {"param", SCHED_PRIORITY_DEFAULT, 4096, param_main},
    {"perf", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, perf_main},
    {"position_estimator_inav", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, position_estimator_inav_main},
    {"preflight_check", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, preflight_check_main},
    {"pwm", SCHED_PRIORITY_DEFAULT, 4096, pwm_main},
    {"px4io", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, px4io_main},
    {"ramtron", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, ramtron_main},
    {"reboot", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, reboot_main},
    {"rgbled", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, rgbled_main},
    {"sdlog2", SCHED_PRIORITY_MAX-30, CONFIG_PTHREAD_STACK_DEFAULT, sdlog2_main},
    {"sensors", SCHED_PRIORITY_MAX-5, CONFIG_PTHREAD_STACK_DEFAULT, sensors_main},
    {"tests", SCHED_PRIORITY_DEFAULT, 12000, tests_main},
    {"tone_alarm", SCHED_PRIORITY_DEFAULT, CONFIG_PTHREAD_STACK_DEFAULT, tone_alarm_main},
    {"top", SCHED_PRIORITY_DEFAULT, 3000, top_main},
    {"uorb", SCHED_PRIORITY_DEFAULT, 4096, uorb_main},
    {NULL, 0, 0, NULL}
};
const int g_builtin_count = 45;
