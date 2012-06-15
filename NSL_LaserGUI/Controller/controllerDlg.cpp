
// controllerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "controller.h"
#include "controllerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*
*/
t_M05_set_pwm2_frq				M05_set_pwm2_frq;
t_M05_mc_change_multi_voltage	M05_mc_change_multi_voltage;
t_M05_set_power_compensate		M05_set_power_compensate;

t_M05_SetCanUseTime	M05_SetCanUseTime;
t_M05_GetCardNumber	M05_GetCardNumber;
t_M05_GetHasUseTime	M05_GetHasUseTime;
t_M05_SetHasUseTime	M05_SetHasUseTime;

t_M05_change_voltage2		M05_change_voltage2;
t_M05_c_set_multi_power		M05_c_set_multi_power;
t_M05_change_multi_voltage	M05_change_multi_voltage;

t_M05_ReadDogNumber			M05_ReadDogNumber;
t_M05_WriteDogNumber		M05_WriteDogNumber;

t_M05_init_board			M05_init_board;
t_M05_compile_work_file		M05_compile_work_file;
t_M05_GetCanUseTime			M05_GetCanUseTime;
t_M05_c_set_power			M05_c_set_power;
t_M05_send_cmd				M05_send_cmd;
t_M05_send_data	M05_send_data;
t_M05_c_set_vector_profile	M05_c_set_vector_profile;
t_M05_call_sub	M05_call_sub;
t_M05_change_voltage	M05_change_voltage;
t_M05_check_EncA	M05_check_EncA;
t_M05_check_EncB	M05_check_EncB;
t_M05_check_EncZ	M05_check_EncZ;
t_M05_check_SD	M05_check_SD;
t_M05_check_alarm	M05_check_alarm;
t_M05_check_batch_done	M05_check_batch_done;
t_M05_check_buffer	M05_check_buffer;
t_M05_check_done	M05_check_done;
t_M05_check_ecp_num	M05_check_ecp_num;
t_M05_check_home	M05_check_home;
t_M05_check_limit	M05_check_limit;
t_M05_check_status	M05_check_status;
t_M05_check_status32	M05_check_status32;
t_M05_checkin_bit	M05_checkin_bit;
t_M05_checkin_byte	M05_checkin_byte;
t_M05_close_file	M05_close_file;
t_M05_close_link	M05_close_link;
t_M05_comm_read_data	M05_comm_read_data;
t_M05_con_hmove	M05_con_hmove;
t_M05_con_hmove2	M05_con_hmove2;
t_M05_con_hmove3	M05_con_hmove3;
t_M05_con_line2	M05_con_line2;
t_M05_con_line3	M05_con_line3;
t_M05_con_pmove	M05_con_pmove;
t_M05_con_pmove2	M05_con_pmove2;
t_M05_con_pmove3	M05_con_pmove3;
t_M05_con_vmove	M05_con_vmove;
t_M05_con_vmove2	M05_con_vmove2;
t_M05_con_vmove3	M05_con_vmove3;
t_M05_control_func	M05_control_func;
t_M05_create_file	M05_create_file;
t_M05_decel_stop	M05_decel_stop;
t_M05_decel_stop2	M05_decel_stop2;
t_M05_decel_stop3	M05_decel_stop3;
t_M05_del_all_file	M05_del_all_file;
t_M05_del_file	M05_del_file;
t_M05_delay_time	M05_delay_time;
t_M05_download_work_file	M05_download_work_file;
t_M05_end_backlash	M05_end_backlash;
t_M05_fast_arc_center	M05_fast_arc_center;
t_M05_fast_hmove	M05_fast_hmove;
t_M05_fast_hmove2	M05_fast_hmove2;
t_M05_fast_hmove3	M05_fast_hmove3;
t_M05_fast_line2	M05_fast_line2;
t_M05_fast_line2_to	M05_fast_line2_to;
t_M05_fast_pmove	M05_fast_pmove;
t_M05_fast_pmove2	M05_fast_pmove2;
t_M05_fast_pmove3	M05_fast_pmove3;
t_M05_fast_vmove	M05_fast_vmove;
t_M05_fast_vmove2	M05_fast_vmove2;
t_M05_fast_vmove3	M05_fast_vmove3;
t_M05_get_abs_pos	M05_get_abs_pos;
t_M05_get_board_info	M05_get_board_info;
t_M05_get_board_ver	M05_get_board_ver;
t_M05_get_cmd_counter	M05_get_cmd_counter;
t_M05_get_debug_info	M05_get_debug_info;
t_M05_get_dll_info	M05_get_dll_info;
t_M05_get_encoder	M05_get_encoder;
t_M05_get_err	M05_get_err;
t_M05_get_file_num	M05_get_file_num;
t_M05_get_hdw_ver	M05_get_hdw_ver;
t_M05_get_last_err	M05_get_last_err;
t_M05_get_lib_ver	M05_get_lib_ver;
t_M05_get_rate	M05_get_rate;
t_M05_get_rel_pos	M05_get_rel_pos;
t_M05_get_sn	M05_get_sn;
t_M05_get_status	M05_get_status;
t_M05_m_change_voltage	M05_m_change_voltage;
t_M05_m_curve_begin	M05_m_curve_begin;
t_M05_m_curve_end	M05_m_curve_end;
t_M05_m_curve_vertex	M05_m_curve_vertex;
t_M05_m_delay_time	M05_m_delay_time;
t_M05_m_fast_line2	M05_m_fast_line2;
t_M05_m_move_pause	M05_m_move_pause;
t_M05_m_move_resume	M05_m_move_resume;
t_M05_m_move_start	M05_m_move_start;
t_M05_m_move_stop	M05_m_move_stop;
t_M05_m_set_blow_mode	M05_m_set_blow_mode;
t_M05_m_set_commonIO_3	M05_m_set_commonIO_3;
t_M05_m_set_common_IO	M05_m_set_common_IO;
t_M05_m_set_laser_mode	M05_m_set_laser_mode;
t_M05_m_set_period	M05_m_set_period;
t_M05_m_set_power	M05_m_set_power;
t_M05_m_set_vector_profile	M05_m_set_vector_profile;
t_M05_m_set_vector_profile2	M05_m_set_vector_profile2;
t_M05_main_begin	M05_main_begin;
t_M05_main_end	M05_main_end;
t_M05_mc_begin_cut	M05_mc_begin_cut;
t_M05_mc_change_voltage	M05_mc_change_voltage;
t_M05_mc_con_line2	M05_mc_con_line2;
t_M05_mc_delay_time	M05_mc_delay_time;
t_M05_mc_end_cut	M05_mc_end_cut;
t_M05_mc_fast_line2	M05_mc_fast_line2;
t_M05_mc_set_blow_mode	M05_mc_set_blow_mode;
t_M05_mc_set_commonIO_3	M05_mc_set_commonIO_3;
t_M05_mc_set_common_IO	M05_mc_set_common_IO;
t_M05_mc_set_laser_mode	M05_mc_set_laser_mode;
t_M05_mc_set_vector_conspeed	M05_mc_set_vector_conspeed;
t_M05_mc_set_vector_profile	M05_mc_set_vector_profile;
t_M05_mi_checkin_bit	M05_mi_checkin_bit;
t_M05_mi_checkin_byte	M05_mi_checkin_byte;
t_M05_move_pause	M05_move_pause;
t_M05_move_resume	M05_move_resume;
t_M05_on_comm	M05_on_comm;
t_M05_open_file	M05_open_file;
t_M05_open_link	M05_open_link;
t_M05_outport_bit	M05_outport_bit;
t_M05_outport_byte	M05_outport_byte;
t_M05_outport_data	M05_outport_data;
t_M05_read_data	M05_read_data;
t_M05_reset_cmd_counter	M05_reset_cmd_counter;
t_M05_reset_err	M05_reset_err;
t_M05_reset_pos	M05_reset_pos;
t_M05_set_abs_pos	M05_set_abs_pos;
t_M05_set_backlash	M05_set_backlash;
t_M05_set_batch_flag	M05_set_batch_flag;
t_M05_set_blow_mode	M05_set_blow_mode;
t_M05_set_cmd_counter	M05_set_cmd_counter;
t_M05_set_comm	M05_set_comm;
t_M05_set_commonIO_3	M05_set_commonIO_3;
t_M05_set_common_IO	M05_set_common_IO;
t_M05_set_conspeed	M05_set_conspeed;
t_M05_set_ellipse_ratio	M05_set_ellipse_ratio;
t_M05_set_encoder_mode	M05_set_encoder_mode;
t_M05_set_getpos_mode	M05_set_getpos_mode;
t_M05_set_home_mode	M05_set_home_mode;
t_M05_set_laser_mode	M05_set_laser_mode;
t_M05_set_level_in	M05_set_level_in;
t_M05_set_level_sprt	M05_set_level_sprt;
t_M05_set_maxspeed	M05_set_maxspeed;
t_M05_set_outmode	M05_set_outmode;
t_M05_set_power_mode	M05_set_power_mode;
t_M05_set_profile	M05_set_profile;
t_M05_set_pulse_per_milimter	M05_set_pulse_per_milimter;
t_M05_set_pwm_frq	M05_set_pwm_frq;
t_M05_set_ramp_flag	M05_set_ramp_flag;
t_M05_set_s_curve	M05_set_s_curve;
t_M05_set_s_section	M05_set_s_section;
t_M05_set_scan_backlash	M05_set_scan_backlash;
t_M05_set_scan_startpos	M05_set_scan_startpos;
t_M05_set_scan_step	M05_set_scan_step;
t_M05_set_senddata_flag	M05_set_senddata_flag;
t_M05_set_signal_in	M05_set_signal_in;
t_M05_set_vector_conspeed	M05_set_vector_conspeed;
t_M05_set_vector_profile	M05_set_vector_profile;
t_M05_set_work_file	M05_set_work_file;
t_M05_set_work_mode	M05_set_work_mode;
t_M05_set_xy_ratio	M05_set_xy_ratio;
t_M05_show_err	M05_show_err;
t_M05_start_backlash	M05_start_backlash;
t_M05_start_run	M05_start_run;
t_M05_stop_run	M05_stop_run;
t_M05_sub_begin	M05_sub_begin;
t_M05_sub_end	M05_sub_end;
t_M05_sudden_stop	M05_sudden_stop;
t_M05_sudden_stop2	M05_sudden_stop2;
t_M05_sudden_stop3	M05_sudden_stop3;
t_M05_sys_set	M05_sys_set;
t_M05_sysset_end	M05_sysset_end;
t_M05_system_test	M05_system_test;
t_M05_trig_position	M05_trig_position;
t_M05_verify_ecp_num	M05_verify_ecp_num;
t_M05_work_begin	M05_work_begin;
t_M05_work_end	M05_work_end;
t_M05_write_data	M05_write_data;
t_M05_write_data_block	M05_write_data_block;
t_M05_write_ecp_num	M05_write_ecp_num;
t_M05_write_file	M05_write_file;
t_M05_write_power_data	M05_write_power_data;
t_M05_write_power_tab	M05_write_power_tab;
t_M05_access_flash	M05_access_flash;
t_M05_arc_center	M05_arc_center;
t_M05_arc_final	M05_arc_final;
t_M05_c_change_voltage	M05_c_change_voltage;
t_M05_c_curve_begin	M05_c_curve_begin;
t_M05_c_curve_end	M05_c_curve_end;
t_M05_c_curve_vertex	M05_c_curve_vertex;
t_M05_c_delay_time	M05_c_delay_time;
t_M05_c_fast_line2	M05_c_fast_line2;
t_M05_c_move_pause	M05_c_move_pause;
t_M05_c_move_resume	M05_c_move_resume;
t_M05_c_move_start	M05_c_move_start;
t_M05_c_move_stop	M05_c_move_stop;
t_M05_c_set_blow_mode	M05_c_set_blow_mode;
t_M05_c_set_max_accel	M05_c_set_max_accel;
t_M05_c_set_move_profile	M05_c_set_move_profile;
t_M05_c_set_commonIO_3	M05_c_set_commonIO_3;
t_M05_c_set_common_IO	M05_c_set_common_IO;
t_M05_c_set_laser_mode	M05_c_set_laser_mode;

//Shortcut macro
#define INIT_PROC_ADDR( name )	name = (t_##name)GetProcAddress(h_M05, #name);	\
								if( name == NULL ) {							\
									MessageBoxA(NULL, #name ,"Error",  0 );		\
									goto dll_error;								\
								}

// Library handle
static HMODULE h_M05;

int InitM05( void ) 
{
	// load the library
	h_M05 = LoadLibraryA("MPC05LS.DLL");
	if ( !h_M05 ) {

		return 0;
	}

	INIT_PROC_ADDR( M05_init_board );
	INIT_PROC_ADDR( M05_GetCanUseTime );
    INIT_PROC_ADDR( M05_access_flash);
    INIT_PROC_ADDR( M05_arc_center);
    INIT_PROC_ADDR( M05_arc_final);
    INIT_PROC_ADDR( M05_c_change_voltage);
    INIT_PROC_ADDR( M05_c_curve_begin);
    INIT_PROC_ADDR( M05_c_curve_end);
    INIT_PROC_ADDR( M05_c_curve_vertex);
    INIT_PROC_ADDR( M05_c_delay_time);
    INIT_PROC_ADDR( M05_c_fast_line2);
    INIT_PROC_ADDR( M05_c_move_pause);
    INIT_PROC_ADDR( M05_c_move_resume);
    INIT_PROC_ADDR( M05_c_move_start);
    INIT_PROC_ADDR( M05_c_move_stop);
    INIT_PROC_ADDR( M05_c_set_blow_mode);
    INIT_PROC_ADDR( M05_c_set_commonIO_3);
    INIT_PROC_ADDR( M05_c_set_common_IO);
    INIT_PROC_ADDR( M05_c_set_laser_mode);
    INIT_PROC_ADDR( M05_c_set_max_accel);
    INIT_PROC_ADDR( M05_c_set_move_profile);
    INIT_PROC_ADDR( M05_c_set_power);
    INIT_PROC_ADDR( M05_c_set_vector_profile);
    INIT_PROC_ADDR( M05_call_sub);
    INIT_PROC_ADDR( M05_change_voltage);
    INIT_PROC_ADDR( M05_check_EncA);
    INIT_PROC_ADDR( M05_check_EncB);
    INIT_PROC_ADDR( M05_check_EncZ);
    INIT_PROC_ADDR( M05_check_SD);
    INIT_PROC_ADDR( M05_check_alarm);
    INIT_PROC_ADDR( M05_check_batch_done);
    INIT_PROC_ADDR( M05_check_buffer);
    INIT_PROC_ADDR( M05_check_done);
    INIT_PROC_ADDR( M05_check_ecp_num);
    INIT_PROC_ADDR( M05_check_home);
    INIT_PROC_ADDR( M05_check_limit);
    INIT_PROC_ADDR( M05_check_status);
    INIT_PROC_ADDR( M05_check_status32);
    INIT_PROC_ADDR( M05_checkin_bit);
    INIT_PROC_ADDR( M05_checkin_byte);
    INIT_PROC_ADDR( M05_close_file);
    INIT_PROC_ADDR( M05_close_link);
    INIT_PROC_ADDR( M05_comm_read_data);
    INIT_PROC_ADDR( M05_con_hmove);
    INIT_PROC_ADDR( M05_con_hmove2);
    INIT_PROC_ADDR( M05_con_hmove3);
    INIT_PROC_ADDR( M05_con_line2);
    INIT_PROC_ADDR( M05_con_line3);
    INIT_PROC_ADDR( M05_con_pmove);
    INIT_PROC_ADDR( M05_con_pmove2);
    INIT_PROC_ADDR( M05_con_pmove3);
    INIT_PROC_ADDR( M05_con_vmove);
    INIT_PROC_ADDR( M05_con_vmove2);
    INIT_PROC_ADDR( M05_con_vmove3);
    INIT_PROC_ADDR( M05_control_func);
    INIT_PROC_ADDR( M05_create_file);
    INIT_PROC_ADDR( M05_decel_stop);
    INIT_PROC_ADDR( M05_decel_stop2);
    INIT_PROC_ADDR( M05_decel_stop3);
    INIT_PROC_ADDR( M05_del_all_file);
    INIT_PROC_ADDR( M05_del_file);
    INIT_PROC_ADDR( M05_delay_time);
    INIT_PROC_ADDR( M05_download_work_file);
    INIT_PROC_ADDR( M05_end_backlash);
    INIT_PROC_ADDR( M05_fast_arc_center);
    INIT_PROC_ADDR( M05_fast_hmove);
    INIT_PROC_ADDR( M05_fast_hmove2);
    INIT_PROC_ADDR( M05_fast_hmove3);
    INIT_PROC_ADDR( M05_fast_line2);
    INIT_PROC_ADDR( M05_fast_line2_to);
    INIT_PROC_ADDR( M05_fast_pmove);
    INIT_PROC_ADDR( M05_fast_pmove2);
    INIT_PROC_ADDR( M05_fast_pmove3);
    INIT_PROC_ADDR( M05_fast_vmove);
    INIT_PROC_ADDR( M05_fast_vmove2);
    INIT_PROC_ADDR( M05_fast_vmove3);
    INIT_PROC_ADDR( M05_get_abs_pos);
    INIT_PROC_ADDR( M05_get_board_info);
    INIT_PROC_ADDR( M05_get_board_ver);
    INIT_PROC_ADDR( M05_get_cmd_counter);
    INIT_PROC_ADDR( M05_get_debug_info);
    INIT_PROC_ADDR( M05_get_dll_info);
    INIT_PROC_ADDR( M05_get_encoder);
    INIT_PROC_ADDR( M05_get_err);
    INIT_PROC_ADDR( M05_get_file_num);
    INIT_PROC_ADDR( M05_get_hdw_ver);
    INIT_PROC_ADDR( M05_get_last_err);
    INIT_PROC_ADDR( M05_get_lib_ver);
    INIT_PROC_ADDR( M05_get_rate);
    INIT_PROC_ADDR( M05_get_rel_pos);
    INIT_PROC_ADDR( M05_get_sn);
    INIT_PROC_ADDR( M05_get_status);
    INIT_PROC_ADDR( M05_init_board);
    INIT_PROC_ADDR( M05_m_change_voltage);
    INIT_PROC_ADDR( M05_m_curve_begin);
    INIT_PROC_ADDR( M05_m_curve_end);
    INIT_PROC_ADDR( M05_m_curve_vertex);
    INIT_PROC_ADDR( M05_m_delay_time);
    INIT_PROC_ADDR( M05_m_fast_line2);
    INIT_PROC_ADDR( M05_m_move_pause);
    INIT_PROC_ADDR( M05_m_move_resume);
    INIT_PROC_ADDR( M05_m_move_start);
    INIT_PROC_ADDR( M05_m_move_stop);
    INIT_PROC_ADDR( M05_m_set_blow_mode);
    INIT_PROC_ADDR( M05_m_set_commonIO_3);
    INIT_PROC_ADDR( M05_m_set_common_IO);
    INIT_PROC_ADDR( M05_m_set_laser_mode);
    INIT_PROC_ADDR( M05_m_set_period);
    INIT_PROC_ADDR( M05_m_set_power);
    INIT_PROC_ADDR( M05_m_set_vector_profile);
    INIT_PROC_ADDR( M05_m_set_vector_profile2);
    INIT_PROC_ADDR( M05_main_begin);
    INIT_PROC_ADDR( M05_main_end);
    INIT_PROC_ADDR( M05_mc_begin_cut);
    INIT_PROC_ADDR( M05_mc_change_voltage);
    INIT_PROC_ADDR( M05_mc_con_line2);
    INIT_PROC_ADDR( M05_mc_delay_time);
    INIT_PROC_ADDR( M05_mc_end_cut);
    INIT_PROC_ADDR( M05_mc_fast_line2);
    INIT_PROC_ADDR( M05_mc_set_blow_mode);
    INIT_PROC_ADDR( M05_mc_set_commonIO_3);
    INIT_PROC_ADDR( M05_mc_set_common_IO);
    INIT_PROC_ADDR( M05_mc_set_laser_mode);
    INIT_PROC_ADDR( M05_mc_set_vector_conspeed);
    INIT_PROC_ADDR( M05_mc_set_vector_profile);
    INIT_PROC_ADDR( M05_mi_checkin_bit);
    INIT_PROC_ADDR( M05_mi_checkin_byte);
    INIT_PROC_ADDR( M05_move_pause);
    INIT_PROC_ADDR( M05_move_resume);
    INIT_PROC_ADDR( M05_on_comm);
    INIT_PROC_ADDR( M05_open_file);
    INIT_PROC_ADDR( M05_open_link);
    INIT_PROC_ADDR( M05_outport_bit);
    INIT_PROC_ADDR( M05_outport_byte);
    INIT_PROC_ADDR( M05_outport_data);
    INIT_PROC_ADDR( M05_read_data);
    INIT_PROC_ADDR( M05_reset_cmd_counter);
    INIT_PROC_ADDR( M05_reset_err);
    INIT_PROC_ADDR( M05_reset_pos);
    INIT_PROC_ADDR( M05_send_cmd);
    INIT_PROC_ADDR( M05_send_data);
    INIT_PROC_ADDR( M05_set_abs_pos);
    INIT_PROC_ADDR( M05_set_backlash);
    INIT_PROC_ADDR( M05_set_batch_flag);
    INIT_PROC_ADDR( M05_set_blow_mode);
    INIT_PROC_ADDR( M05_set_cmd_counter);
    INIT_PROC_ADDR( M05_set_comm);
    INIT_PROC_ADDR( M05_set_commonIO_3);
    INIT_PROC_ADDR( M05_set_common_IO);
    INIT_PROC_ADDR( M05_set_conspeed);
    INIT_PROC_ADDR( M05_set_ellipse_ratio);
    INIT_PROC_ADDR( M05_set_encoder_mode);
    INIT_PROC_ADDR( M05_set_getpos_mode);
    INIT_PROC_ADDR( M05_set_home_mode);
    INIT_PROC_ADDR( M05_set_laser_mode);
    INIT_PROC_ADDR( M05_set_level_in);
    INIT_PROC_ADDR( M05_set_level_sprt);
    INIT_PROC_ADDR( M05_set_maxspeed);
    INIT_PROC_ADDR( M05_set_outmode);
    INIT_PROC_ADDR( M05_set_power_mode);
    INIT_PROC_ADDR( M05_set_profile);
    INIT_PROC_ADDR( M05_set_pulse_per_milimter);
    INIT_PROC_ADDR( M05_set_pwm_frq);
    INIT_PROC_ADDR( M05_set_ramp_flag);
    INIT_PROC_ADDR( M05_set_s_curve);
    INIT_PROC_ADDR( M05_set_s_section);
    INIT_PROC_ADDR( M05_set_scan_backlash);
    INIT_PROC_ADDR( M05_set_scan_startpos);
    INIT_PROC_ADDR( M05_set_scan_step);
    INIT_PROC_ADDR( M05_set_senddata_flag);
    INIT_PROC_ADDR( M05_set_signal_in);
    INIT_PROC_ADDR( M05_set_vector_conspeed);
    INIT_PROC_ADDR( M05_set_vector_profile);
    INIT_PROC_ADDR( M05_set_work_file);
    INIT_PROC_ADDR( M05_set_work_mode);
    INIT_PROC_ADDR( M05_set_xy_ratio);
    INIT_PROC_ADDR( M05_show_err);
    INIT_PROC_ADDR( M05_start_backlash);
    INIT_PROC_ADDR( M05_start_run);
    INIT_PROC_ADDR( M05_stop_run);
    INIT_PROC_ADDR( M05_sub_begin);
    INIT_PROC_ADDR( M05_sub_end);
    INIT_PROC_ADDR( M05_sudden_stop);
    INIT_PROC_ADDR( M05_sudden_stop2);
    INIT_PROC_ADDR( M05_sudden_stop3);
    INIT_PROC_ADDR( M05_sys_set);
    INIT_PROC_ADDR( M05_sysset_end);
    INIT_PROC_ADDR( M05_system_test);
    INIT_PROC_ADDR( M05_trig_position);
    INIT_PROC_ADDR( M05_verify_ecp_num);
    INIT_PROC_ADDR( M05_work_begin);
    INIT_PROC_ADDR( M05_work_end);
    INIT_PROC_ADDR( M05_write_data);
    INIT_PROC_ADDR( M05_write_data_block);
    INIT_PROC_ADDR( M05_write_ecp_num);
    INIT_PROC_ADDR( M05_write_file);
    INIT_PROC_ADDR( M05_write_power_data);
    INIT_PROC_ADDR( M05_write_power_tab);
	INIT_PROC_ADDR( M05_compile_work_file );	
	

    INIT_PROC_ADDR( M05_set_pwm2_frq);

	INIT_PROC_ADDR( M05_change_multi_voltage);
    INIT_PROC_ADDR( M05_c_set_multi_power);

    INIT_PROC_ADDR( M05_ReadDogNumber);
    INIT_PROC_ADDR( M05_WriteDogNumber);

	INIT_PROC_ADDR( M05_change_voltage2);
    INIT_PROC_ADDR( M05_GetCardNumber);
    INIT_PROC_ADDR( M05_GetHasUseTime);
    INIT_PROC_ADDR( M05_SetCanUseTime);
    INIT_PROC_ADDR( M05_SetHasUseTime);
    INIT_PROC_ADDR( M05_set_power_compensate);


//	INIT_PROC_ADDR( M05_mc_change_multi_voltage ); // @8

	return 1;

dll_error:;

	if( h_M05 ) {
		FreeLibrary( h_M05 ) ;
	}

	h_M05 = NULL ;
	
	return 0;

}

void FreeM05(void)
{
	if( h_M05 ) {
		FreeLibrary( h_M05 );
	}

	h_M05 = NULL ;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcontrollerDlg dialog




CcontrollerDlg::CcontrollerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcontrollerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcontrollerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcontrollerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CcontrollerDlg::OnBnClickedConnect)
END_MESSAGE_MAP()


// CcontrollerDlg message handlers

BOOL CcontrollerDlg::OnInitDialog()
{
	InitM05();

	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcontrollerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcontrollerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcontrollerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CcontrollerDlg::OnBnClickedConnect()
{
	if (M05_init_board()) 
	{
		MessageBox(L"Failed to connect");
		return ;
	}

	M05_compile_work_file("test.txt");
}

