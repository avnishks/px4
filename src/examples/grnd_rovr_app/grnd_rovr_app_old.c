/****************************************************************************
 *
 *   Copyright (c) 2012, 2013 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file px4_daemon_app.c
 * daemon application example for led sync with mavlink heartbeat
 * 
 * @author Example User <mail@example.com>
 */

#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <unistd.h>
#include <stdio.h>

#include <systemlib/err.h>
#include <systemlib/systemlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <math.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <drivers/drv_hrt.h>

#include <uORB/uORB.h>
#include <drivers/drv_gpio.h>
#include <drivers/drv_rc_input.h>
 #include <drivers/drv_pwm_output.h>

#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_attitude_setpoint.h>
#include <uORB/topics/actuator_outputs.h>
#include <uORB/topics/actuator_controls.h>
#include <uORB/topics/actuator_controls_effective.h>
#include <uORB/topics/vehicle_command.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/vehicle_local_position_setpoint.h>
#include <uORB/topics/vehicle_global_position.h>
#include <uORB/topics/vehicle_gps_position.h>
#include <uORB/topics/vehicle_vicon_position.h>
#include <uORB/topics/optical_flow.h>
#include <uORB/topics/battery_status.h>
#include <uORB/topics/differential_pressure.h>
#include <uORB/topics/airspeed.h>

#include <uORB/topics/debug_key_value.h>

#include <nuttx/wqueue.h>
#include <nuttx/clock.h>


static bool thread_should_exit = false;		/**< daemon exit flag */
static bool thread_running = false;		/**< daemon status flag */
static int daemon_task;				/**< Handle of daemon task / thread */


static int topic_handle;

/**
 * daemon management function.
 */
__EXPORT int grnd_rovr_app_main(int argc, char *argv[]);

/**
 * Mainloop of daemon.
 */
int grnd_rovr_thread_main(int argc, char *argv[]);

/**
 * Print the correct usage.
 */
static void usage(const char *reason);

static void
usage(const char *reason)
{
	if (reason)
		warnx("%s\n", reason);
	errx(1, "usage: daemon {start|stop|status} [-p <additional params>]\n\n");
}


void arm_it()
{
	struct actuator_armed_s armed;
	armed.armed = true;

	/* XXX allow arming by external components on multicopters only if not yet armed by RC */
	/* XXX allow arming only if core sensors are ok */
	armed.ready_to_arm = true;

	/* lock down actuators if required, only in HIL */
	armed.lockdown = false;
	orb_advert_t armed_pub = orb_advertise(ORB_ID(actuator_armed), &armed);
	orb_publish(ORB_ID(actuator_armed), armed_pub, &armed);
}


float my_map(float angle, float fromSource, float toSource, float fromTarget, float toTarget)
{
	float ans = 0.0f;
	ans = (angle - fromSource) / (toSource - fromSource) * (toTarget - fromTarget) + fromTarget;
	return ans;
}

//tripathy
void your_fn(struct vehicle_gps_position_s * gps_pos, struct vehicle_attitude_s * att , struct actuator_controls_s * actuators, struct rc_input_values * rc_input)
{
	actuators->control[0] = my_map(att->roll,-3.14f,3.14f,-1.0f,1.0f);
	actuators->control[1] = my_map(att->pitch,-3.14f,3.14f,-1.0f,1.0f);
	actuators->control[2] = my_map(att->yaw,-3.14f,3.14f,-1.0f,1.0f);
	actuators->control[3] = 1.0f;
	
	//reads pwm input 1 value
	uint16_t pwm_input_1 = rc_input.value[0];

}


void update_my_ppm(struct vehicle_attitude_s * att, struct actuator_controls_s * actuators)
{
/*
	printf("R P Y:\t%8.4f\t%8.4f\t%8.4f\n",
						(double)att->roll,
						(double)att->pitch,
						(double)att->yaw);
*/
	actuators->control[0] = my_map(att->roll,-3.14f,3.14f,-1.0f,1.0f);
	actuators->control[1] = my_map(att->pitch,-3.14f,3.14f,-1.0f,1.0f);
	actuators->control[2] = my_map(att->yaw,-3.14f,3.14f,-1.0f,1.0f);
	actuators->control[3] = 1.0f;
}

/**
 * The daemon app only briefly exists to start
 * the background job. The stack size assigned in the
 * Makefile does only apply to this management task.
 * 
 * The actual stack size should be set in the call
 * to task_create().
 */
int grnd_rovr_app_main(int argc, char *argv[])
{
	if (argc < 1)
		usage("missing command");

	if (!strcmp(argv[1], "start")) {

		if (thread_running) {
			warnx("daemon already running\n");
			/* this is not an error */
			exit(0);
		}

		thread_should_exit = false;
		daemon_task = task_spawn("daemon",
					 SCHED_DEFAULT,
					 SCHED_PRIORITY_DEFAULT,
					 4096,
					 grnd_rovr_thread_main,
					 (argv) ? (const char **)&argv[2] : (const char **)NULL);
		exit(0);
	}

	if (!strcmp(argv[1], "stop")) {
		thread_should_exit = true;
		exit(0);
	}

	if (!strcmp(argv[1], "status")) {
		if (thread_running) {
			warnx("\trunning\n");
		} else {
			warnx("\tnot started\n");
		}
		exit(0);
	}

	usage("unrecognized command");
	exit(1);
}


int grnd_rovr_thread_main(int argc, char *argv[]) {

	warnx("[daemon] starting\n");

	int result, fd;

	/* output structs */
	struct actuator_controls_s actuators;
	memset(&actuators, 0, sizeof(actuators));

	/* publish actuator controls */
	for (unsigned i = 0; i < NUM_ACTUATOR_CONTROLS; i++) {
		actuators.control[i] = 0.0f;
	}
	
	orb_advert_t actuator_pub = orb_advertise(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, &actuators);
	

	fd = open(PWM_OUTPUT_DEVICE_PATH, O_RDWR);
	result = ioctl(fd, PWM_SERVO_ARM, 0);

	const ssize_t fdsc = 13;
	/* Sanity check variable and index */
	ssize_t fdsc_count = 0;
	/* file descriptors to wait for */
	struct pollfd fds[fdsc];

	union {
		struct vehicle_command_s cmd;
		struct vehicle_status_s status;
		struct sensor_combined_s sensor;
		struct vehicle_attitude_s att;
		struct vehicle_attitude_setpoint_s att_sp;
		struct actuator_outputs_s act_outputs;
		struct actuator_controls_s act_controls;
		struct actuator_controls_effective_s act_controls_effective;
		struct vehicle_local_position_s local_pos;
		struct vehicle_local_position_setpoint_s local_pos_sp;
		struct vehicle_global_position_s global_pos;
		struct vehicle_gps_position_s gps_pos;
		struct rc_input_values rc_input;
		} buf;
	memset(&buf, 0, sizeof(buf));


	struct {
		int cmd_sub;
		int status_sub;
		int sensor_sub;
		int att_sub;
		int att_sp_sub;
		int act_outputs_sub;
		int act_controls_sub;
		int act_controls_effective_sub;
		int local_pos_sub;
		int local_pos_sp_sub;
		int global_pos_sub;
		int gps_pos_sub;
		int rc_input_sub;
	} subs;

	/* --- VEHICLE COMMAND --- */
	subs.cmd_sub = orb_subscribe(ORB_ID(vehicle_command));
	fds[fdsc_count].fd = subs.cmd_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- VEHICLE STATUS --- */
	subs.status_sub = orb_subscribe(ORB_ID(vehicle_status));
	fds[fdsc_count].fd = subs.status_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- GPS POSITION --- */
	subs.gps_pos_sub = orb_subscribe(ORB_ID(vehicle_gps_position));
	fds[fdsc_count].fd = subs.gps_pos_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- SENSORS COMBINED --- */
	subs.sensor_sub = orb_subscribe(ORB_ID(sensor_combined));
	fds[fdsc_count].fd = subs.sensor_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- ATTITUDE --- */
	subs.att_sub = orb_subscribe(ORB_ID(vehicle_attitude));
	fds[fdsc_count].fd = subs.att_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- ATTITUDE SETPOINT --- */
	subs.att_sp_sub = orb_subscribe(ORB_ID(vehicle_attitude_setpoint));
	fds[fdsc_count].fd = subs.att_sp_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- ACTUATOR OUTPUTS --- */
	subs.act_outputs_sub = orb_subscribe(ORB_ID(actuator_outputs_0));
	fds[fdsc_count].fd = subs.act_outputs_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- ACTUATOR CONTROL --- */
	subs.act_controls_sub = orb_subscribe(ORB_ID_VEHICLE_ATTITUDE_CONTROLS);
	fds[fdsc_count].fd = subs.act_controls_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- ACTUATOR CONTROL EFFECTIVE --- */
	subs.act_controls_effective_sub = orb_subscribe(ORB_ID_VEHICLE_ATTITUDE_CONTROLS_EFFECTIVE);
	fds[fdsc_count].fd = subs.act_controls_effective_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- LOCAL POSITION --- */
	subs.local_pos_sub = orb_subscribe(ORB_ID(vehicle_local_position));
	fds[fdsc_count].fd = subs.local_pos_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- LOCAL POSITION SETPOINT --- */
	subs.local_pos_sp_sub = orb_subscribe(ORB_ID(vehicle_local_position_setpoint));
	fds[fdsc_count].fd = subs.local_pos_sp_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- GLOBAL POSITION --- */
	subs.global_pos_sub = orb_subscribe(ORB_ID(vehicle_global_position));
	fds[fdsc_count].fd = subs.global_pos_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;

	/* --- RC INPUT --- */
	subs.rc_input_sub = orb_subscribe(ORB_ID(input_rc));
	fds[fdsc_count].fd = subs.rc_input_sub;
	fds[fdsc_count].events = POLLIN;
	fdsc_count++;




	if (fdsc_count > fdsc) {
		warn("WARNING: Not enough space for poll fds allocated. Check %s:%d.", __FILE__, __LINE__);
		fdsc_count = fdsc;
	}

	/*
	 * set up poll to block for new data,
	 * wait for a maximum of 1000 ms
	 */
	const int poll_timeout = 1000;

	thread_running = true;


	/* track changes in sensor_combined topic */
	uint16_t gyro_counter = 0;
	uint16_t accelerometer_counter = 0;
	uint16_t magnetometer_counter = 0;
	uint16_t baro_counter = 0;
	uint16_t differential_pressure_counter = 0;


	while (!thread_should_exit) {


		/* poll all topics */
		int poll_ret = poll(fds, fdsc_count, 100);

		/* handle the poll result */
		if (poll_ret < 0) {
			warnx("ERROR: poll error, stop.");
			thread_should_exit = true;

		} else if (poll_ret > 0) {

			int ifds = 0;

			/* --- VEHICLE COMMAND --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_command), subs.cmd_sub, &buf.cmd);
			}

			/* --- VEHICLE STATUS --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_status), subs.status_sub, &buf.status);
			}


			/* --- GPS POSITION --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_gps_position), subs.gps_pos_sub, &buf.gps_pos,&buf.rc_input );
				//your_fn(&buf.gps_pos, &buf.att , &actuators);
			}

			/* --- SENSOR COMBINED --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(sensor_combined), subs.sensor_sub, &buf.sensor);
			}

			/* --- ATTITUDE --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_attitude), subs.att_sub, &buf.att);
				update_my_ppm(&buf.att,&actuators);//modifies actuators value accourding to attitude value
				/* sanity check and publish actuator outputs */
				if (isfinite(actuators.control[0]) &&
				    isfinite(actuators.control[1]) &&
				    isfinite(actuators.control[2]) &&
				    isfinite(actuators.control[3])) {
				orb_publish(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, actuator_pub, &actuators);
				}
			}

			/* --- ATTITUDE SETPOINT --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_attitude_setpoint), subs.att_sp_sub, &buf.att_sp);
			}

			/* --- ACTUATOR OUTPUTS --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(actuator_outputs_0), subs.act_outputs_sub, &buf.act_outputs);
				/*				
				printf("Actuator Output:\t%8.4f\t%8.4f\t%8.4f\t%8.4f\n",
						(float)buf.act_outputs.output[0],
						(double)buf.act_outputs.output[1],
						(float)buf.act_outputs.output[2],(float)buf.act_outputs.output[3]);
				*/
			}

			/* --- ACTUATOR CONTROL --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, subs.act_controls_sub, &buf.act_controls);
				/*				
				printf("Actuator Control:\t%8.4f\t%8.4f\t%8.4f\t%8.4f\n",
						(float)buf.act_controls.control[0],
						(double)buf.act_controls.control[1],
						(float)buf.act_controls.control[2],(float)buf.act_controls.control[3]);
				*/
				// TODO not implemented yet
			}

			/* --- ACTUATOR CONTROL EFFECTIVE --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID_VEHICLE_ATTITUDE_CONTROLS_EFFECTIVE, subs.act_controls_effective_sub, &buf.act_controls_effective);
				/*printf("Actuator Control eff:\t%8.4f\t%8.4f\t%8.4f\t%8.4f\n",
						(float)buf.act_controls_effective.control_effective[0],
						(double)buf.act_controls_effective.control_effective[1],
						(float)buf.act_controls_effective.control_effective[2],(float)buf.act_controls_effective.control_effective[3]);
				*/
				// TODO not implemented yet
			}

			/* --- LOCAL POSITION --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_local_position), subs.local_pos_sub, &buf.local_pos);
			}

			/* --- LOCAL POSITION SETPOINT --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_local_position_setpoint), subs.local_pos_sp_sub, &buf.local_pos_sp);
			}

			/* --- GLOBAL POSITION --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(vehicle_global_position), subs.global_pos_sub, &buf.global_pos);
				// TODO not implemented yet
			}
			/* --- RC INPUT --- */
			if (fds[ifds++].revents & POLLIN) {
				orb_copy(ORB_ID(input_rc), subs.rc_input_sub, &buf.rc_input);
				
				printf("[RC INPUT]: \t%8.4f\t%8.4f\t%8.4f\t%8.4f\n",
						(uint16_t)buf.rc_input.values[0],
						(uint16_t)buf.rc_input.values[1],
						(uint16_t)buf.rc_input.values[2],(float)buf.rc_input.values[3]);
			
			}
			

		}

	}

	warnx("[daemon] exiting.\n");

	thread_running = false;

	return 0;
}
