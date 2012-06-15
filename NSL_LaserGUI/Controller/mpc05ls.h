#ifndef _H_INTERFACE_M05_
#define _H_INTERFACE_M05_

// fixed up by charliex AT nullspacelabs DOT com

#ifdef __cplusplus
extern "C" {
#endif

#if __DLL_EXPORT

#ifdef _EXPORTING
   #define CLASS_DECLSPEC   __declspec(dllexport)
#else
   #define CLASS_DECLSPEC	__declspec( dllimport )
#endif

#define APICALL __stdcall 

CLASS_DECLSPEC int APICALL M05_access_flash(DWORD type,PDWORD data);
CLASS_DECLSPEC int APICALL M05_arc_center(int ch1,int ch2,long cenx,long ceny,double angle);
CLASS_DECLSPEC int APICALL M05_arc_final(int ch1,int ch2,int dir,long fx,long fy,long r);

CLASS_DECLSPEC int APICALL M05_call_sub(int subid, double* param, int paramnum);
CLASS_DECLSPEC int APICALL M05_change_voltage(int v);
CLASS_DECLSPEC int APICALL M05_check_alarm(int ch);
CLASS_DECLSPEC int APICALL M05_check_batch_done();
CLASS_DECLSPEC int APICALL M05_check_buffer(int BufferType);
CLASS_DECLSPEC int APICALL M05_check_done(int ch);
CLASS_DECLSPEC int APICALL M05_check_ecp_num();
CLASS_DECLSPEC int APICALL M05_check_EncA(int ch);
CLASS_DECLSPEC int APICALL M05_check_EncB(int ch);
CLASS_DECLSPEC int APICALL M05_check_EncZ(int ch);
CLASS_DECLSPEC int APICALL M05_check_home(int ch);
CLASS_DECLSPEC int APICALL M05_check_limit(int ch);
CLASS_DECLSPEC int APICALL M05_check_SD(int ch);
CLASS_DECLSPEC int APICALL M05_check_status(int ch);
CLASS_DECLSPEC unsigned int APICALL M05_check_status32(int ch);
CLASS_DECLSPEC int APICALL M05_checkin_bit(int cardno,int bitno);
CLASS_DECLSPEC int APICALL M05_checkin_byte(int cardno);
CLASS_DECLSPEC int APICALL M05_close_file();
CLASS_DECLSPEC int APICALL M05_close_link(int ch);
CLASS_DECLSPEC int APICALL M05_compile_work_file(char* filename);
CLASS_DECLSPEC int APICALL M05_comm_read_data(DWORD dwDataLenBytes, PBYTE pchDataIn, PBYTE pchDataOut);
CLASS_DECLSPEC int APICALL M05_control_func(int FuncID);
CLASS_DECLSPEC int APICALL M05_con_hmove(int ch , int dir);
CLASS_DECLSPEC int APICALL M05_con_hmove2(int chx , int dirx , int chy , int diry);
CLASS_DECLSPEC int APICALL M05_con_hmove3(int chx , int dirx , int chy , int diry , int chz , int dirz);
CLASS_DECLSPEC int APICALL M05_con_line2(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_con_line3(int chx , long disx ,int chy , long disy ,int chz , long disz);
CLASS_DECLSPEC int APICALL M05_con_pmove(int ch,long step);
CLASS_DECLSPEC int APICALL M05_con_pmove2(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_con_pmove3(int chx , long disx ,int chy , long disy ,int chz , long disz);
CLASS_DECLSPEC int APICALL M05_con_vmove(int ch , int dir);
CLASS_DECLSPEC int APICALL M05_con_vmove2(int chx , int dirx , int chy , int diry);
CLASS_DECLSPEC int APICALL M05_con_vmove3(int chx , int dirx , int chy , int diry , int chz , int dirz);
CLASS_DECLSPEC int APICALL M05_create_file(PBYTE filename, DWORD filelenbytes);
CLASS_DECLSPEC int APICALL M05_c_change_voltage(int v);
CLASS_DECLSPEC int APICALL M05_c_curve_begin();
CLASS_DECLSPEC int APICALL M05_c_curve_end();
CLASS_DECLSPEC int APICALL M05_c_curve_vertex(int IsMove);
CLASS_DECLSPEC int APICALL M05_c_delay_time(int ms);
CLASS_DECLSPEC int APICALL M05_c_fast_line2(int chx,long disx,int chy,long disy);
CLASS_DECLSPEC int APICALL M05_c_move_start();
CLASS_DECLSPEC int APICALL M05_c_move_pause();
CLASS_DECLSPEC int APICALL M05_c_move_resume();
CLASS_DECLSPEC int APICALL M05_c_move_stop();
CLASS_DECLSPEC int APICALL M05_c_set_laser_mode(int mode);
CLASS_DECLSPEC int APICALL M05_c_set_blow_mode(int mode);/*Add on 21.09.2005*/
CLASS_DECLSPEC int APICALL M05_c_set_commonIO_3(int mode);/*Add on 25.02.2006*/
CLASS_DECLSPEC int APICALL M05_c_set_common_IO(int PortID,int mode);/*Add on 02.03.2006*/
CLASS_DECLSPEC int APICALL M05_c_set_max_accel(double ac);
CLASS_DECLSPEC int APICALL M05_c_set_move_profile(double ls, double hs, double ac);
CLASS_DECLSPEC int APICALL M05_c_set_power(int LowPower,int HighPower, double LowSpeed, double HighSpeed, int Mode);
CLASS_DECLSPEC int APICALL M05_c_set_vector_profile(double ls,double hs,double ac);

CLASS_DECLSPEC int APICALL M05_decel_stop(int ch);
CLASS_DECLSPEC int APICALL M05_decel_stop2(int ch1,int ch2);
CLASS_DECLSPEC int APICALL M05_decel_stop3(int ch1,int ch2,int ch3);
CLASS_DECLSPEC int APICALL M05_delay_time(int ms);
CLASS_DECLSPEC int APICALL M05_del_file(int FileID);
CLASS_DECLSPEC int APICALL M05_del_all_file();
CLASS_DECLSPEC int APICALL M05_download_work_file(char* filename);

CLASS_DECLSPEC int APICALL M05_fast_arc_center(int ch1,int ch2,long cenx,long ceny,double angle);
CLASS_DECLSPEC int APICALL M05_fast_hmove(int ch , int dir);
CLASS_DECLSPEC int APICALL M05_fast_hmove2(int chx , int dirx , int chy , int diry);
CLASS_DECLSPEC int APICALL M05_fast_hmove3(int chx , int dirx , int chy , int diry , int chz , int dirz);
CLASS_DECLSPEC int APICALL M05_fast_line2(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_fast_line2_to(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_fast_pmove(int ch,long step);
CLASS_DECLSPEC int APICALL M05_fast_pmove2(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_fast_pmove3(int chx , long disx ,int chy , long disy ,int chz , long disz);
CLASS_DECLSPEC int APICALL M05_fast_vmove(int ch , int dir);
CLASS_DECLSPEC int APICALL M05_fast_vmove2(int chx , int dirx , int chy , int diry);
CLASS_DECLSPEC int APICALL M05_fast_vmove3(int chx , int dirx , int chy , int diry , int chz , int dirz);

CLASS_DECLSPEC int APICALL M05_get_abs_pos(int ch,long *pos);
CLASS_DECLSPEC int APICALL M05_get_board_info(int* info);
CLASS_DECLSPEC DWORD  APICALL M05_get_board_ver();
CLASS_DECLSPEC int APICALL M05_get_cmd_counter();
CLASS_DECLSPEC int  APICALL M05_get_debug_info(LPCTSTR info);
CLASS_DECLSPEC int APICALL M05_get_dll_info(double* info);
CLASS_DECLSPEC int APICALL M05_get_encoder(int ch,long *enc);
CLASS_DECLSPEC DWORD APICALL M05_get_err(int index);
CLASS_DECLSPEC int APICALL M05_get_file_num(int* CurFileID, int* FileNum);
CLASS_DECLSPEC DWORD  APICALL M05_get_hdw_ver();
CLASS_DECLSPEC DWORD APICALL M05_get_last_err();
CLASS_DECLSPEC DWORD  APICALL M05_get_lib_ver();
CLASS_DECLSPEC int APICALL M05_get_rel_pos(int ch,long *pos);
CLASS_DECLSPEC double APICALL M05_get_rate(int ch);
CLASS_DECLSPEC int  APICALL M05_get_sn(unsigned char* sn);
CLASS_DECLSPEC unsigned int APICALL M05_get_status(int ch);

CLASS_DECLSPEC int APICALL M05_init_board(void);

CLASS_DECLSPEC int APICALL M05_main_begin();
CLASS_DECLSPEC int APICALL M05_main_end();
CLASS_DECLSPEC int APICALL M05_mi_checkin_bit(int bitno);
CLASS_DECLSPEC int APICALL M05_mi_checkin_byte();
CLASS_DECLSPEC int APICALL M05_move_pause();
CLASS_DECLSPEC int APICALL M05_move_resume();
CLASS_DECLSPEC int APICALL M05_m_move_pause();
CLASS_DECLSPEC int APICALL M05_m_move_resume();
CLASS_DECLSPEC int APICALL M05_m_move_stop();
CLASS_DECLSPEC int APICALL M05_m_move_start();
CLASS_DECLSPEC int APICALL M05_m_fast_line2(int chx,long disx,int chy,long disy);
CLASS_DECLSPEC int APICALL M05_m_set_vector_profile(double ls,double hs,double ac);
CLASS_DECLSPEC int APICALL M05_m_set_vector_profile2(double start_ls, double hs, double end_ls, double ac, double dc);
CLASS_DECLSPEC int APICALL M05_m_curve_vertex();
CLASS_DECLSPEC int APICALL M05_m_curve_begin();
CLASS_DECLSPEC int APICALL M05_m_curve_end();
CLASS_DECLSPEC int APICALL M05_m_set_period(double period);
CLASS_DECLSPEC int APICALL M05_m_set_power(int LowPower,int HighPower);
CLASS_DECLSPEC int APICALL M05_m_set_laser_mode(int mode);
CLASS_DECLSPEC int APICALL M05_m_set_blow_mode(int mode);/*Add on 21.09.2005*/
CLASS_DECLSPEC int APICALL M05_m_set_commonIO_3(int mode);/*Add on 25.02.2006*/
CLASS_DECLSPEC int APICALL M05_m_set_common_IO(int PortID,int mode);/*Add on 02.03.2006*/
CLASS_DECLSPEC int APICALL M05_m_delay_time(int ms);
CLASS_DECLSPEC int APICALL M05_m_change_voltage(int v);
CLASS_DECLSPEC int APICALL M05_mc_set_vector_conspeed(double conspeed);
CLASS_DECLSPEC int APICALL M05_mc_con_line2(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_mc_delay_time(int ms);
CLASS_DECLSPEC int APICALL M05_mc_change_voltage(int v);
CLASS_DECLSPEC int APICALL M05_mc_set_laser_mode(int mode);
CLASS_DECLSPEC int APICALL M05_mc_set_blow_mode(int mode);/*Add on 21.09.2005*/
CLASS_DECLSPEC int APICALL M05_mc_set_commonIO_3(int mode);/*Add on 25.02.2006*/
CLASS_DECLSPEC int APICALL M05_mc_set_common_IO(int mode);/*Add on 02.03.2006*/
CLASS_DECLSPEC int APICALL M05_mc_set_vector_profile(double vl , double vh , double ad);
CLASS_DECLSPEC int APICALL M05_mc_fast_line2(int chx , long disx ,int chy , long disy);
CLASS_DECLSPEC int APICALL M05_mc_begin_cut();
CLASS_DECLSPEC int APICALL M05_mc_end_cut();

CLASS_DECLSPEC int APICALL M05_on_comm(int ch);
CLASS_DECLSPEC int APICALL M05_outport_bit(int cardno,int bitno,int status);
CLASS_DECLSPEC int APICALL M05_outport_byte(int cardno,int byte);
CLASS_DECLSPEC int APICALL M05_outport_data(DWORD Addr,DWORD Data);
CLASS_DECLSPEC int APICALL M05_open_file(BYTE fileID, PDWORD filelen, PBYTE filename);
CLASS_DECLSPEC int APICALL M05_open_link(int ch, int rate);

CLASS_DECLSPEC DWORD APICALL M05_read_data(DWORD addr);
CLASS_DECLSPEC int APICALL M05_reset_cmd_counter();
CLASS_DECLSPEC int APICALL M05_reset_err();
CLASS_DECLSPEC int APICALL M05_reset_pos(int ch);

CLASS_DECLSPEC int APICALL M05_end_backlash(int ch);

CLASS_DECLSPEC int APICALL M05_send_cmd(DWORD dwDataLenDword, PDWORD pdwData);
CLASS_DECLSPEC int APICALL M05_send_data(DWORD dwDataLenBytes, PBYTE pchData);
CLASS_DECLSPEC int APICALL M05_set_abs_pos(int ch,long pos);
CLASS_DECLSPEC int APICALL M05_set_backlash(int ch,int blash);
CLASS_DECLSPEC int APICALL M05_set_batch_flag(int flag);
CLASS_DECLSPEC int APICALL M05_set_cmd_counter(int type,int counter);
CLASS_DECLSPEC int APICALL M05_set_comm(LPVOID pcMSComm);
CLASS_DECLSPEC int APICALL M05_set_conspeed(int ch , double conspeed);
CLASS_DECLSPEC int APICALL M05_set_ellipse_ratio(float ratio);
CLASS_DECLSPEC int APICALL M05_set_encoder_mode(int ch,int mode,int multip,int count_unit);
CLASS_DECLSPEC int APICALL M05_set_getpos_mode(int ch,int mode);
CLASS_DECLSPEC int APICALL M05_set_home_mode(int ch,int origin_mode);
CLASS_DECLSPEC int APICALL M05_set_laser_mode(int mode);
CLASS_DECLSPEC int APICALL M05_set_blow_mode(int mode);/*Add on 21.09.2005*/
CLASS_DECLSPEC int APICALL M05_set_commonIO_3(int mode);/*Add on 25.02.2006*/
CLASS_DECLSPEC int APICALL M05_set_common_IO(int PortID,int mode);/*Add on 02.03.2006*/
CLASS_DECLSPEC int APICALL M05_set_pulse_per_milimter(double pulseX,double pulseY,double pulseZ);
CLASS_DECLSPEC int APICALL M05_set_level_in(int ch,int status);
CLASS_DECLSPEC int APICALL M05_set_level_sprt(int ch,int status);
CLASS_DECLSPEC int APICALL M05_set_maxspeed(int ch , double maxpeed);
CLASS_DECLSPEC int APICALL M05_set_outmode(int ch,int mode,int logic);
CLASS_DECLSPEC int APICALL M05_set_power_mode(int mode);
CLASS_DECLSPEC int APICALL M05_set_profile(int ch , double vl , double vh , double ad);
CLASS_DECLSPEC int APICALL M05_set_pwm_frq(double frq);
CLASS_DECLSPEC int APICALL M05_set_ramp_flag(int flag);
CLASS_DECLSPEC int APICALL M05_set_scan_backlash(int p);
CLASS_DECLSPEC int APICALL M05_set_scan_step(int step);
CLASS_DECLSPEC int APICALL M05_set_scan_startpos(int pos);
CLASS_DECLSPEC int APICALL M05_set_senddata_flag(int flag);
CLASS_DECLSPEC int APICALL M05_set_s_curve(int ch,int mode);
CLASS_DECLSPEC int APICALL M05_set_s_section(int ch,double rate1,double rate2);
CLASS_DECLSPEC int APICALL M05_set_signal_in(int ch,int status);
CLASS_DECLSPEC int APICALL M05_set_vector_conspeed(double conspeed);
CLASS_DECLSPEC int APICALL M05_set_vector_profile(double vl , double vh , double ad);
CLASS_DECLSPEC int APICALL M05_set_work_mode(int mode);
CLASS_DECLSPEC int APICALL M05_set_work_file(char* file);
CLASS_DECLSPEC int APICALL M05_set_xy_ratio(double r);
CLASS_DECLSPEC void APICALL M05_show_err(int flag);
CLASS_DECLSPEC int APICALL M05_start_backlash(int ch);
CLASS_DECLSPEC int APICALL M05_start_run();
CLASS_DECLSPEC int APICALL M05_stop_run();
CLASS_DECLSPEC int APICALL M05_sub_begin(int subid);
CLASS_DECLSPEC int APICALL M05_sub_end(int subid);
CLASS_DECLSPEC int APICALL M05_sudden_stop(int ch);
CLASS_DECLSPEC int APICALL M05_sudden_stop2(int ch1,int ch2);
CLASS_DECLSPEC int APICALL M05_sudden_stop3(int ch1,int ch2,int ch3);
CLASS_DECLSPEC int APICALL M05_system_test(int type,int* pdata);
CLASS_DECLSPEC int APICALL M05_sys_set(int id, double* param, int paramnum);
CLASS_DECLSPEC int APICALL M05_sysset_end();

CLASS_DECLSPEC int APICALL M05_trig_position(int ch,int destpos,int condition);

CLASS_DECLSPEC int APICALL M05_verify_ecp_num(unsigned char* pnum,int len); 

CLASS_DECLSPEC int APICALL M05_work_begin();
CLASS_DECLSPEC int APICALL M05_work_end();
CLASS_DECLSPEC int APICALL M05_write_data(DWORD Addr,DWORD Data);
CLASS_DECLSPEC int APICALL M05_write_data_block(DWORD DataLen,PDWORD Data);
CLASS_DECLSPEC int APICALL M05_write_ecp_num(unsigned char* pnum,int len); 
CLASS_DECLSPEC int APICALL M05_write_file(DWORD dwDataLenBytes, PBYTE pchFileData);
CLASS_DECLSPEC int APICALL M05_write_power_data(DWORD DataLen,PUCHAR Data);
CLASS_DECLSPEC int APICALL M05_write_power_tab(int TabType,DWORD DataLen,PDWORD Data);

CLASS_DECLSPEC int APICALL M05_GetCanUseTime(void);
CLASS_DECLSPEC int APICALL M05_c_set_multi_power( int a1,  int a2,  int a3,  int a4, double a5, double a6, int a7);
CLASS_DECLSPEC int APICALL M05_get_board_ver(void);
CLASS_DECLSPEC int APICALL M05_change_multi_voltage( int a1,  int a2);
CLASS_DECLSPEC int APICALL M05_ReadDogNumber(char *a1);
CLASS_DECLSPEC int APICALL M05_WriteDogNumber(char *a1);
CLASS_DECLSPEC int APICALL M05_change_voltage2(int a1);
CLASS_DECLSPEC int APICALL M05_SetHasUseTime(int a1);
CLASS_DECLSPEC int APICALL M05_SetCanUseTime(int a1);
CLASS_DECLSPEC int APICALL M05_GetCardNumber(int a1);
CLASS_DECLSPEC int APICALL M05_GetHasUseTime(void);
CLASS_DECLSPEC int APICALL M05_set_power_compensate(int a1, int a2, int a3);
CLASS_DECLSPEC int APICALL M05_set_pwm2_frq(double a1);
CLASS_DECLSPEC int APICALL M05_mc_change_multi_voltage(int a1, int a2);// @8

#else

typedef int (WINAPI *t_M05_access_flash)(DWORD type,PDWORD data);
typedef int (WINAPI *t_M05_arc_center)(int ch1,int ch2,long cenx,long ceny,double angle);
typedef int (WINAPI *t_M05_arc_final)(int ch1,int ch2,int dir,long fx,long fy,long r);

typedef int (WINAPI *t_M05_call_sub)(int subid, double* param, int paramnum);
typedef int (WINAPI *t_M05_change_voltage)(int v);
typedef int (WINAPI *t_M05_check_alarm)(int ch);
typedef int (WINAPI *t_M05_check_batch_done)();
typedef int (WINAPI *t_M05_check_buffer)(int BufferType);
typedef int (WINAPI *t_M05_check_done)(int ch);
typedef int (WINAPI *t_M05_check_ecp_num)();
typedef int (WINAPI *t_M05_check_EncA)(int ch);
typedef int (WINAPI *t_M05_check_EncB)(int ch);
typedef int (WINAPI *t_M05_check_EncZ)(int ch);
typedef int (WINAPI *t_M05_check_home)(int ch);
typedef int (WINAPI *t_M05_check_limit)(int ch);
typedef int (WINAPI *t_M05_check_SD)(int ch);
typedef int (WINAPI *t_M05_check_status)(int ch);
typedef unsigned int (WINAPI *t_M05_check_status32)(int ch);
typedef int (WINAPI *t_M05_checkin_bit)(int cardno,int bitno);
typedef int (WINAPI *t_M05_checkin_byte)(int cardno);
typedef int (WINAPI *t_M05_close_file)();
typedef int (WINAPI *t_M05_close_link)(int ch);
typedef int (WINAPI *t_M05_compile_work_file)(char* filename);
typedef int (WINAPI *t_M05_comm_read_data)(DWORD dwDataLenBytes, PBYTE pchDataIn, PBYTE pchDataOut);
typedef int (WINAPI *t_M05_control_func)(int FuncID);
typedef int (WINAPI *t_M05_con_hmove)(int ch , int dir);
typedef int (WINAPI *t_M05_con_hmove2)(int chx , int dirx , int chy , int diry);
typedef int (WINAPI *t_M05_con_hmove3)(int chx , int dirx , int chy , int diry , int chz , int dirz);
typedef int (WINAPI *t_M05_con_line2)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_con_line3)(int chx , long disx ,int chy , long disy ,int chz , long disz);
typedef int (WINAPI *t_M05_con_pmove)(int ch,long step);
typedef int (WINAPI *t_M05_con_pmove2)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_con_pmove3)(int chx , long disx ,int chy , long disy ,int chz , long disz);
typedef int (WINAPI *t_M05_con_vmove)(int ch , int dir);
typedef int (WINAPI *t_M05_con_vmove2)(int chx , int dirx , int chy , int diry);
typedef int (WINAPI *t_M05_con_vmove3)(int chx , int dirx , int chy , int diry , int chz , int dirz);
typedef int (WINAPI *t_M05_create_file)(PBYTE filename, DWORD filelenbytes);
typedef int (WINAPI *t_M05_c_change_voltage)(int v);
typedef int (WINAPI *t_M05_c_curve_begin)();
typedef int (WINAPI *t_M05_c_curve_end)();
typedef int (WINAPI *t_M05_c_curve_vertex)(int IsMove);
typedef int (WINAPI *t_M05_c_delay_time)(int ms);
typedef int (WINAPI *t_M05_c_fast_line2)(int chx,long disx,int chy,long disy);
typedef int (WINAPI *t_M05_c_move_start)();
typedef int (WINAPI *t_M05_c_move_pause)();
typedef int (WINAPI *t_M05_c_move_resume)();
typedef int (WINAPI *t_M05_c_move_stop)();
typedef int (WINAPI *t_M05_c_set_laser_mode)(int mode);
typedef int (WINAPI *t_M05_c_set_blow_mode)(int mode);/*Add on 21.09.2005*/
typedef int (WINAPI *t_M05_c_set_commonIO_3)(int mode);/*Add on 25.02.2006*/
typedef int (WINAPI *t_M05_c_set_common_IO)(int PortID,int mode);/*Add on 02.03.2006*/
typedef int (WINAPI *t_M05_c_set_max_accel)(double ac);
typedef int (WINAPI *t_M05_c_set_move_profile)(double ls, double hs, double ac);
typedef int (WINAPI *t_M05_c_set_power)(int LowPower,int HighPower, double LowSpeed, double HighSpeed, int Mode);
typedef int (WINAPI *t_M05_c_set_vector_profile)(double ls,double hs,double ac);

typedef int (WINAPI *t_M05_decel_stop)(int ch);
typedef int (WINAPI *t_M05_decel_stop2)(int ch1,int ch2);
typedef int (WINAPI *t_M05_decel_stop3)(int ch1,int ch2,int ch3);
typedef int (WINAPI *t_M05_delay_time)(int ms);
typedef int (WINAPI *t_M05_del_file)(int FileID);
typedef int (WINAPI *t_M05_del_all_file)(void);
typedef int (WINAPI *t_M05_download_work_file)(char* filename);

typedef int (WINAPI *t_M05_fast_arc_center)(int ch1,int ch2,long cenx,long ceny,double angle);
typedef int (WINAPI *t_M05_fast_hmove)(int ch , int dir);
typedef int (WINAPI *t_M05_fast_hmove2)(int chx , int dirx , int chy , int diry);
typedef int (WINAPI *t_M05_fast_hmove3)(int chx , int dirx , int chy , int diry , int chz , int dirz);
typedef int (WINAPI *t_M05_fast_line2)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_fast_line2_to)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_fast_pmove)(int ch,long step);
typedef int (WINAPI *t_M05_fast_pmove2)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_fast_pmove3)(int chx , long disx ,int chy , long disy ,int chz , long disz);
typedef int (WINAPI *t_M05_fast_vmove)(int ch , int dir);
typedef int (WINAPI *t_M05_fast_vmove2)(int chx , int dirx , int chy , int diry);
typedef int (WINAPI *t_M05_fast_vmove3)(int chx , int dirx , int chy , int diry , int chz , int dirz);

typedef int (WINAPI *t_M05_get_abs_pos)(int ch,long *pos);
typedef int (WINAPI *t_M05_get_board_info)(int* info);
typedef DWORD  (WINAPI *t_M05_get_board_ver)();
typedef int (WINAPI *t_M05_get_cmd_counter)();
typedef int  (WINAPI *t_M05_get_debug_info)(LPCTSTR info);
typedef int (WINAPI *t_M05_get_dll_info)(double* info);
typedef int (WINAPI *t_M05_get_encoder)(int ch,long *enc);
typedef DWORD (WINAPI *t_M05_get_err)(int index);
typedef int (WINAPI *t_M05_get_file_num)(int* CurFileID, int* FileNum);
typedef DWORD  (WINAPI *t_M05_get_hdw_ver)();
typedef DWORD (WINAPI *t_M05_get_last_err)();
typedef DWORD  (WINAPI *t_M05_get_lib_ver)();
typedef int (WINAPI *t_M05_get_rel_pos)(int ch,long *pos);
typedef double (WINAPI *t_M05_get_rate)(int ch);
typedef int  (WINAPI *t_M05_get_sn)(unsigned char* sn);
typedef unsigned int (WINAPI *t_M05_get_status)(int ch);

typedef int (WINAPI *t_M05_init_board)(void);

typedef int (WINAPI *t_M05_main_begin)();
typedef int (WINAPI *t_M05_main_end)();
typedef int (WINAPI *t_M05_mi_checkin_bit)(int bitno);
typedef int (WINAPI *t_M05_mi_checkin_byte)();
typedef int (WINAPI *t_M05_move_pause)();
typedef int (WINAPI *t_M05_move_resume)();
typedef int (WINAPI *t_M05_m_move_pause)();
typedef int (WINAPI *t_M05_m_move_resume)();
typedef int (WINAPI *t_M05_m_move_stop)();
typedef int (WINAPI *t_M05_m_move_start)();
typedef int (WINAPI *t_M05_m_fast_line2)(int chx,long disx,int chy,long disy);
typedef int (WINAPI *t_M05_m_set_vector_profile)(double ls,double hs,double ac);
typedef int (WINAPI *t_M05_m_set_vector_profile2)(double start_ls, double hs, double end_ls, double ac, double dc);
typedef int (WINAPI *t_M05_m_curve_vertex)();
typedef int (WINAPI *t_M05_m_curve_begin)();
typedef int (WINAPI *t_M05_m_curve_end)();
typedef int (WINAPI *t_M05_m_set_period)(double period);
typedef int (WINAPI *t_M05_m_set_power)(int LowPower,int HighPower);
typedef int (WINAPI *t_M05_m_set_laser_mode)(int mode);
typedef int (WINAPI *t_M05_m_set_blow_mode)(int mode);/*Add on 21.09.2005*/
typedef int (WINAPI *t_M05_m_set_commonIO_3)(int mode);/*Add on 25.02.2006*/
typedef int (WINAPI *t_M05_m_set_common_IO)(int PortID,int mode);/*Add on 02.03.2006*/
typedef int (WINAPI *t_M05_m_delay_time)(int ms);
typedef int (WINAPI *t_M05_m_change_voltage)(int v);
typedef int (WINAPI *t_M05_mc_set_vector_conspeed)(double conspeed);
typedef int (WINAPI *t_M05_mc_con_line2)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_mc_delay_time)(int ms);
typedef int (WINAPI *t_M05_mc_change_voltage)(int v);
typedef int (WINAPI *t_M05_mc_set_laser_mode)(int mode);
typedef int (WINAPI *t_M05_mc_set_blow_mode)(int mode);/*Add on 21.09.2005*/
typedef int (WINAPI *t_M05_mc_set_commonIO_3)(int mode);/*Add on 25.02.2006*/
typedef int (WINAPI *t_M05_mc_set_common_IO)(int mode);/*Add on 02.03.2006*/
typedef int (WINAPI *t_M05_mc_set_vector_profile)(double vl , double vh , double ad);
typedef int (WINAPI *t_M05_mc_fast_line2)(int chx , long disx ,int chy , long disy);
typedef int (WINAPI *t_M05_mc_begin_cut)();
typedef int (WINAPI *t_M05_mc_end_cut)();

typedef int (WINAPI *t_M05_on_comm)(int ch);
typedef int (WINAPI *t_M05_outport_bit)(int cardno,int bitno,int status);
typedef int (WINAPI *t_M05_outport_byte)(int cardno,int byte);
typedef int (WINAPI *t_M05_outport_data)(DWORD Addr,DWORD Data);
typedef int (WINAPI *t_M05_open_file)(BYTE fileID, PDWORD filelen, PBYTE filename);
typedef int (WINAPI *t_M05_open_link)(int ch, int rate);

typedef DWORD (WINAPI *t_M05_read_data)(DWORD addr);
typedef int (WINAPI *t_M05_reset_cmd_counter)();
typedef int (WINAPI *t_M05_reset_err)();
typedef int (WINAPI *t_M05_reset_pos)(int ch);

typedef int (WINAPI *t_M05_end_backlash)(int ch);

typedef int (WINAPI *t_M05_send_cmd)(DWORD dwDataLenDword, PDWORD pdwData);
typedef int (WINAPI *t_M05_send_data)(DWORD dwDataLenBytes, PBYTE pchData);
typedef int (WINAPI *t_M05_set_abs_pos)(int ch,long pos);
typedef int (WINAPI *t_M05_set_backlash)(int ch,int blash);
typedef int (WINAPI *t_M05_set_batch_flag)(int flag);
typedef int (WINAPI *t_M05_set_cmd_counter)(int type,int counter);
typedef int (WINAPI *t_M05_set_comm)(LPVOID pcMSComm);
typedef int (WINAPI *t_M05_set_conspeed)(int ch , double conspeed);
typedef int (WINAPI *t_M05_set_ellipse_ratio)(float ratio);
typedef int (WINAPI *t_M05_set_encoder_mode)(int ch,int mode,int multip,int count_unit);
typedef int (WINAPI *t_M05_set_getpos_mode)(int ch,int mode);
typedef int (WINAPI *t_M05_set_home_mode)(int ch,int origin_mode);
typedef int (WINAPI *t_M05_set_laser_mode)(int mode);
typedef int (WINAPI *t_M05_set_blow_mode)(int mode);/*Add on 21.09.2005*/
typedef int (WINAPI *t_M05_set_commonIO_3)(int mode);/*Add on 25.02.2006*/
typedef int (WINAPI *t_M05_set_common_IO)(int PortID,int mode);/*Add on 02.03.2006*/
typedef int (WINAPI *t_M05_set_pulse_per_milimter)(double pulseX,double pulseY,double pulseZ);
typedef int (WINAPI *t_M05_set_level_in)(int ch,int status);
typedef int (WINAPI *t_M05_set_level_sprt)(int ch,int status);
typedef int (WINAPI *t_M05_set_maxspeed)(int ch , double maxpeed);
typedef int (WINAPI *t_M05_set_outmode)(int ch,int mode,int logic);
typedef int (WINAPI *t_M05_set_power_mode)(int mode);
typedef int (WINAPI *t_M05_set_profile)(int ch , double vl , double vh , double ad);
typedef int (WINAPI *t_M05_set_pwm_frq)(double frq);
typedef int (WINAPI *t_M05_set_ramp_flag)(int flag);
typedef int (WINAPI *t_M05_set_scan_backlash)(int p);
typedef int (WINAPI *t_M05_set_scan_step)(int step);
typedef int (WINAPI *t_M05_set_scan_startpos)(int pos);
typedef int (WINAPI *t_M05_set_senddata_flag)(int flag);
typedef int (WINAPI *t_M05_set_s_curve)(int ch,int mode);
typedef int (WINAPI *t_M05_set_s_section)(int ch,double rate1,double rate2);
typedef int (WINAPI *t_M05_set_signal_in)(int ch,int status);
typedef int (WINAPI *t_M05_set_vector_conspeed)(double conspeed);
typedef int (WINAPI *t_M05_set_vector_profile)(double vl , double vh , double ad);
typedef int (WINAPI *t_M05_set_work_mode)(int mode);
typedef int (WINAPI *t_M05_set_work_file)(char* file);
typedef int (WINAPI *t_M05_set_xy_ratio)(double r);
typedef void (WINAPI *t_M05_show_err)(int flag);
typedef int (WINAPI *t_M05_start_backlash)(int ch);
typedef int (WINAPI *t_M05_start_run)();
typedef int (WINAPI *t_M05_stop_run)();
typedef int (WINAPI *t_M05_sub_begin)(int subid);
typedef int (WINAPI *t_M05_sub_end)(int subid);
typedef int (WINAPI *t_M05_sudden_stop)(int ch);
typedef int (WINAPI *t_M05_sudden_stop2)(int ch1,int ch2);
typedef int (WINAPI *t_M05_sudden_stop3)(int ch1,int ch2,int ch3);
typedef int (WINAPI *t_M05_system_test)(int type,int* pdata);
typedef int (WINAPI *t_M05_sys_set)(int id, double* param, int paramnum);
typedef int (WINAPI *t_M05_sysset_end)();

typedef int (WINAPI *t_M05_trig_position)(int ch,int destpos,int condition);

typedef int (WINAPI *t_M05_verify_ecp_num)(unsigned char* pnum,int len); 

typedef int (WINAPI *t_M05_work_begin)();
typedef int (WINAPI *t_M05_work_end)();
typedef int (WINAPI *t_M05_write_data)(DWORD Addr,DWORD Data);
typedef int (WINAPI *t_M05_write_data_block)(DWORD DataLen,PDWORD Data);
typedef int (WINAPI *t_M05_write_ecp_num)(unsigned char* pnum,int len); 
typedef int (WINAPI *t_M05_write_file)(DWORD dwDataLenBytes, PBYTE pchFileData);
typedef int (WINAPI *t_M05_write_power_data)(DWORD DataLen,PUCHAR Data);
typedef int (WINAPI *t_M05_write_power_tab)(int TabType,DWORD DataLen,PDWORD Data);

typedef int (WINAPI *t_M05_GetCanUseTime)(void);

// working on (parameters not confirmed)
typedef int (WINAPI *t_M05_c_set_multi_power)( int a1,  int a2,  int a3,  int a4, double a5, double a6, int a7);
typedef int (WINAPI *t_M05_change_multi_voltage)( int a1,  int a2);
typedef int (WINAPI *t_M05_ReadDogNumber)(char *a1);
typedef int (WINAPI *t_M05_WriteDogNumber)(char *a1);
typedef int (WINAPI *t_M05_change_voltage2)(int a1);
typedef int (WINAPI *t_M05_SetHasUseTime)(int a1);
typedef int (WINAPI *t_M05_SetCanUseTime)(int a1);
typedef int (WINAPI *t_M05_GetCardNumber)(int a1);
typedef int (WINAPI *t_M05_GetHasUseTime)(void);
typedef int (WINAPI *t_M05_set_power_compensate)(int a1, int a2, int a3);
typedef int (WINAPI *t_M05_set_pwm2_frq)(double a1);
typedef int (WINAPI *t_M05_mc_change_multi_voltage)(int a1, int a2); // @8

#endif

#ifdef __cplusplus
}
#endif

#endif