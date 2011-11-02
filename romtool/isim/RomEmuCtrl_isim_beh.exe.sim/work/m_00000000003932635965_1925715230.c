/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x1cce1bb2 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "E:/032/romtool/RomEmulatorCtrl.v";
static unsigned int ng1[] = {0U, 0U};
static unsigned int ng2[] = {1U, 0U};
static int ng3[] = {0, 0};
static int ng4[] = {3, 0};
static int ng5[] = {1, 0};
static int ng6[] = {7, 0};
static int ng7[] = {4, 0};
static int ng8[] = {2, 0};
static int ng9[] = {11, 0};
static int ng10[] = {8, 0};
static int ng11[] = {15, 0};
static int ng12[] = {12, 0};
static unsigned int ng13[] = {2U, 0U};
static unsigned int ng14[] = {3U, 0U};
static unsigned int ng15[] = {4U, 0U};
static unsigned int ng16[] = {8U, 0U};
static unsigned int ng17[] = {5U, 0U};
static unsigned int ng18[] = {0U, 255U};



static void Always_84_0(char *t0)
{
    char t12[8];
    char t13[8];
    char t14[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    int t6;
    char *t7;
    char *t8;
    int t9;
    char *t10;
    char *t11;
    char *t15;
    char *t16;
    char *t17;
    char *t18;
    char *t19;
    char *t20;
    unsigned int t21;
    int t22;
    char *t23;
    unsigned int t24;
    int t25;
    int t26;
    char *t27;
    unsigned int t28;
    int t29;
    int t30;
    unsigned int t31;
    int t32;
    unsigned int t33;
    unsigned int t34;
    int t35;
    int t36;
    char *t37;
    unsigned int t38;
    unsigned int t39;
    unsigned int t40;
    unsigned int t41;
    unsigned int t42;
    unsigned int t43;
    unsigned int t44;
    unsigned int t45;
    unsigned int t46;
    unsigned int t47;
    unsigned int t48;

LAB0:    t1 = (t0 + 5248U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(84, ng0);
    t2 = (t0 + 7304);
    *((int *)t2) = 1;
    t3 = (t0 + 5280);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(85, ng0);

LAB5:    xsi_set_current_line(86, ng0);
    t4 = (t0 + 1048U);
    t5 = *((char **)t4);

LAB6:    t4 = ((char*)((ng1)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 3, t4, 3);
    if (t6 == 1)
        goto LAB7;

LAB8:    t2 = ((char*)((ng2)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 3, t2, 3);
    if (t6 == 1)
        goto LAB9;

LAB10:    t2 = ((char*)((ng13)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 3, t2, 3);
    if (t6 == 1)
        goto LAB11;

LAB12:    t2 = ((char*)((ng14)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 3, t2, 3);
    if (t6 == 1)
        goto LAB13;

LAB14:    t2 = ((char*)((ng15)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 3, t2, 3);
    if (t6 == 1)
        goto LAB15;

LAB16:    t2 = ((char*)((ng17)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 3, t2, 3);
    if (t6 == 1)
        goto LAB17;

LAB18:
LAB20:
LAB19:
LAB21:    goto LAB2;

LAB7:    xsi_set_current_line(88, ng0);

LAB22:    xsi_set_current_line(89, ng0);
    t7 = ((char*)((ng1)));
    t8 = (t0 + 3368);
    xsi_vlogvar_wait_assign_value(t8, t7, 0, 0, 16, 0LL);
    xsi_set_current_line(90, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 3528);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 8, 0LL);
    xsi_set_current_line(91, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 4008);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 1, 0LL);
    xsi_set_current_line(93, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 4168);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 2, 0LL);
    xsi_set_current_line(94, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 4328);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 4, 0LL);
    xsi_set_current_line(96, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 3688);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 1, 0LL);
    xsi_set_current_line(97, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 3848);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 1, 0LL);
    goto LAB21;

LAB9:    xsi_set_current_line(101, ng0);

LAB23:    xsi_set_current_line(102, ng0);
    t3 = (t0 + 4168);
    t4 = (t3 + 56U);
    t7 = *((char **)t4);

LAB24:    t8 = ((char*)((ng3)));
    t9 = xsi_vlog_unsigned_case_compare(t7, 2, t8, 32);
    if (t9 == 1)
        goto LAB25;

LAB26:    t2 = ((char*)((ng5)));
    t6 = xsi_vlog_unsigned_case_compare(t7, 2, t2, 32);
    if (t6 == 1)
        goto LAB27;

LAB28:    t2 = ((char*)((ng8)));
    t6 = xsi_vlog_unsigned_case_compare(t7, 2, t2, 32);
    if (t6 == 1)
        goto LAB29;

LAB30:    t2 = ((char*)((ng4)));
    t6 = xsi_vlog_unsigned_case_compare(t7, 2, t2, 32);
    if (t6 == 1)
        goto LAB31;

LAB32:
LAB33:    xsi_set_current_line(108, ng0);
    t2 = (t0 + 4168);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t8 = ((char*)((ng2)));
    memset(t12, 0, 8);
    xsi_vlog_unsigned_add(t12, 2, t4, 2, t8, 2);
    t10 = (t0 + 4168);
    xsi_vlogvar_wait_assign_value(t10, t12, 0, 0, 2, 0LL);
    goto LAB21;

LAB11:    xsi_set_current_line(112, ng0);

LAB42:    xsi_set_current_line(113, ng0);
    t3 = (t0 + 3368);
    t4 = (t3 + 56U);
    t8 = *((char **)t4);
    t10 = ((char*)((ng2)));
    memset(t12, 0, 8);
    xsi_vlog_unsigned_add(t12, 16, t8, 16, t10, 16);
    t11 = (t0 + 3368);
    xsi_vlogvar_wait_assign_value(t11, t12, 0, 0, 16, 0LL);
    goto LAB21;

LAB13:    xsi_set_current_line(117, ng0);

LAB43:    xsi_set_current_line(118, ng0);
    t3 = (t0 + 4328);
    t4 = (t3 + 56U);
    t8 = *((char **)t4);

LAB44:    t10 = ((char*)((ng3)));
    t9 = xsi_vlog_unsigned_case_compare(t8, 4, t10, 32);
    if (t9 == 1)
        goto LAB45;

LAB46:    t2 = ((char*)((ng5)));
    t6 = xsi_vlog_unsigned_case_compare(t8, 4, t2, 32);
    if (t6 == 1)
        goto LAB47;

LAB48:    t2 = ((char*)((ng8)));
    t6 = xsi_vlog_unsigned_case_compare(t8, 4, t2, 32);
    if (t6 == 1)
        goto LAB49;

LAB50:
LAB52:
LAB51:
LAB53:    xsi_set_current_line(129, ng0);
    t2 = (t0 + 4328);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t10 = ((char*)((ng13)));
    memset(t12, 0, 8);
    t11 = (t4 + 4);
    t15 = (t10 + 4);
    t21 = *((unsigned int *)t4);
    t24 = *((unsigned int *)t10);
    t28 = (t21 ^ t24);
    t31 = *((unsigned int *)t11);
    t33 = *((unsigned int *)t15);
    t34 = (t31 ^ t33);
    t38 = (t28 | t34);
    t39 = *((unsigned int *)t11);
    t40 = *((unsigned int *)t15);
    t41 = (t39 | t40);
    t42 = (~(t41));
    t43 = (t38 & t42);
    if (t43 != 0)
        goto LAB62;

LAB59:    if (t41 != 0)
        goto LAB61;

LAB60:    *((unsigned int *)t12) = 1;

LAB62:    t17 = (t12 + 4);
    t44 = *((unsigned int *)t17);
    t45 = (~(t44));
    t46 = *((unsigned int *)t12);
    t47 = (t46 & t45);
    t48 = (t47 != 0);
    if (t48 > 0)
        goto LAB63;

LAB64:    xsi_set_current_line(132, ng0);
    t2 = (t0 + 4328);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t10 = ((char*)((ng2)));
    memset(t12, 0, 8);
    xsi_vlog_unsigned_add(t12, 4, t4, 4, t10, 4);
    t11 = (t0 + 4328);
    xsi_vlogvar_wait_assign_value(t11, t12, 0, 0, 4, 0LL);

LAB65:    goto LAB21;

LAB15:    xsi_set_current_line(136, ng0);

LAB66:    xsi_set_current_line(137, ng0);
    t3 = (t0 + 4328);
    t4 = (t3 + 56U);
    t10 = *((char **)t4);

LAB67:    t11 = ((char*)((ng3)));
    t9 = xsi_vlog_unsigned_case_compare(t10, 4, t11, 32);
    if (t9 == 1)
        goto LAB68;

LAB69:    t2 = ((char*)((ng5)));
    t6 = xsi_vlog_unsigned_case_compare(t10, 4, t2, 32);
    if (t6 == 1)
        goto LAB70;

LAB71:
LAB73:
LAB72:    xsi_set_current_line(144, ng0);
    t2 = (t0 + 3528);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    memset(t13, 0, 8);
    t11 = (t13 + 4);
    t15 = (t4 + 4);
    t21 = *((unsigned int *)t4);
    t24 = (t21 >> 1);
    *((unsigned int *)t13) = t24;
    t28 = *((unsigned int *)t15);
    t31 = (t28 >> 1);
    *((unsigned int *)t11) = t31;
    t33 = *((unsigned int *)t13);
    *((unsigned int *)t13) = (t33 & 127U);
    t34 = *((unsigned int *)t11);
    *((unsigned int *)t11) = (t34 & 127U);
    t16 = ((char*)((ng1)));
    xsi_vlogtype_concat(t12, 8, 8, 2U, t16, 1, t13, 7);
    t17 = (t0 + 3528);
    xsi_vlogvar_wait_assign_value(t17, t12, 0, 0, 8, 0LL);

LAB74:    xsi_set_current_line(147, ng0);
    t2 = (t0 + 4328);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t11 = ((char*)((ng16)));
    memset(t12, 0, 8);
    t15 = (t4 + 4);
    t16 = (t11 + 4);
    t21 = *((unsigned int *)t4);
    t24 = *((unsigned int *)t11);
    t28 = (t21 ^ t24);
    t31 = *((unsigned int *)t15);
    t33 = *((unsigned int *)t16);
    t34 = (t31 ^ t33);
    t38 = (t28 | t34);
    t39 = *((unsigned int *)t15);
    t40 = *((unsigned int *)t16);
    t41 = (t39 | t40);
    t42 = (~(t41));
    t43 = (t38 & t42);
    if (t43 != 0)
        goto LAB79;

LAB76:    if (t41 != 0)
        goto LAB78;

LAB77:    *((unsigned int *)t12) = 1;

LAB79:    t18 = (t12 + 4);
    t44 = *((unsigned int *)t18);
    t45 = (~(t44));
    t46 = *((unsigned int *)t12);
    t47 = (t46 & t45);
    t48 = (t47 != 0);
    if (t48 > 0)
        goto LAB80;

LAB81:    xsi_set_current_line(150, ng0);
    t2 = (t0 + 4328);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t11 = ((char*)((ng2)));
    memset(t12, 0, 8);
    xsi_vlog_unsigned_add(t12, 4, t4, 4, t11, 4);
    t15 = (t0 + 4328);
    xsi_vlogvar_wait_assign_value(t15, t12, 0, 0, 4, 0LL);

LAB82:    goto LAB21;

LAB17:    xsi_set_current_line(154, ng0);

LAB83:    xsi_set_current_line(155, ng0);
    t3 = (t0 + 1368U);
    t4 = *((char **)t3);
    memset(t12, 0, 8);
    t3 = (t12 + 4);
    t11 = (t4 + 4);
    t21 = *((unsigned int *)t4);
    t24 = (t21 >> 0);
    t28 = (t24 & 1);
    *((unsigned int *)t12) = t28;
    t31 = *((unsigned int *)t11);
    t33 = (t31 >> 0);
    t34 = (t33 & 1);
    *((unsigned int *)t3) = t34;
    t15 = (t0 + 4008);
    xsi_vlogvar_wait_assign_value(t15, t12, 0, 0, 1, 0LL);
    goto LAB21;

LAB25:    xsi_set_current_line(103, ng0);
    t10 = (t0 + 1368U);
    t11 = *((char **)t10);
    t10 = (t0 + 3368);
    t15 = (t0 + 3368);
    t16 = (t15 + 72U);
    t17 = *((char **)t16);
    t18 = ((char*)((ng4)));
    t19 = ((char*)((ng3)));
    xsi_vlog_convert_partindices(t12, t13, t14, ((int*)(t17)), 2, t18, 32, 1, t19, 32, 1);
    t20 = (t12 + 4);
    t21 = *((unsigned int *)t20);
    t22 = (!(t21));
    t23 = (t13 + 4);
    t24 = *((unsigned int *)t23);
    t25 = (!(t24));
    t26 = (t22 && t25);
    t27 = (t14 + 4);
    t28 = *((unsigned int *)t27);
    t29 = (!(t28));
    t30 = (t26 && t29);
    if (t30 == 1)
        goto LAB34;

LAB35:    goto LAB33;

LAB27:    xsi_set_current_line(104, ng0);
    t3 = (t0 + 1368U);
    t4 = *((char **)t3);
    t3 = (t0 + 3368);
    t8 = (t0 + 3368);
    t10 = (t8 + 72U);
    t11 = *((char **)t10);
    t15 = ((char*)((ng6)));
    t16 = ((char*)((ng7)));
    xsi_vlog_convert_partindices(t12, t13, t14, ((int*)(t11)), 2, t15, 32, 1, t16, 32, 1);
    t17 = (t12 + 4);
    t21 = *((unsigned int *)t17);
    t9 = (!(t21));
    t18 = (t13 + 4);
    t24 = *((unsigned int *)t18);
    t22 = (!(t24));
    t25 = (t9 && t22);
    t19 = (t14 + 4);
    t28 = *((unsigned int *)t19);
    t26 = (!(t28));
    t29 = (t25 && t26);
    if (t29 == 1)
        goto LAB36;

LAB37:    goto LAB33;

LAB29:    xsi_set_current_line(105, ng0);
    t3 = (t0 + 1368U);
    t4 = *((char **)t3);
    t3 = (t0 + 3368);
    t8 = (t0 + 3368);
    t10 = (t8 + 72U);
    t11 = *((char **)t10);
    t15 = ((char*)((ng9)));
    t16 = ((char*)((ng10)));
    xsi_vlog_convert_partindices(t12, t13, t14, ((int*)(t11)), 2, t15, 32, 1, t16, 32, 1);
    t17 = (t12 + 4);
    t21 = *((unsigned int *)t17);
    t9 = (!(t21));
    t18 = (t13 + 4);
    t24 = *((unsigned int *)t18);
    t22 = (!(t24));
    t25 = (t9 && t22);
    t19 = (t14 + 4);
    t28 = *((unsigned int *)t19);
    t26 = (!(t28));
    t29 = (t25 && t26);
    if (t29 == 1)
        goto LAB38;

LAB39:    goto LAB33;

LAB31:    xsi_set_current_line(106, ng0);
    t3 = (t0 + 1368U);
    t4 = *((char **)t3);
    t3 = (t0 + 3368);
    t8 = (t0 + 3368);
    t10 = (t8 + 72U);
    t11 = *((char **)t10);
    t15 = ((char*)((ng11)));
    t16 = ((char*)((ng12)));
    xsi_vlog_convert_partindices(t12, t13, t14, ((int*)(t11)), 2, t15, 32, 1, t16, 32, 1);
    t17 = (t12 + 4);
    t21 = *((unsigned int *)t17);
    t9 = (!(t21));
    t18 = (t13 + 4);
    t24 = *((unsigned int *)t18);
    t22 = (!(t24));
    t25 = (t9 && t22);
    t19 = (t14 + 4);
    t28 = *((unsigned int *)t19);
    t26 = (!(t28));
    t29 = (t25 && t26);
    if (t29 == 1)
        goto LAB40;

LAB41:    goto LAB33;

LAB34:    t31 = *((unsigned int *)t14);
    t32 = (t31 + 0);
    t33 = *((unsigned int *)t12);
    t34 = *((unsigned int *)t13);
    t35 = (t33 - t34);
    t36 = (t35 + 1);
    xsi_vlogvar_wait_assign_value(t10, t11, t32, *((unsigned int *)t13), t36, 0LL);
    goto LAB35;

LAB36:    t31 = *((unsigned int *)t14);
    t30 = (t31 + 0);
    t33 = *((unsigned int *)t12);
    t34 = *((unsigned int *)t13);
    t32 = (t33 - t34);
    t35 = (t32 + 1);
    xsi_vlogvar_wait_assign_value(t3, t4, t30, *((unsigned int *)t13), t35, 0LL);
    goto LAB37;

LAB38:    t31 = *((unsigned int *)t14);
    t30 = (t31 + 0);
    t33 = *((unsigned int *)t12);
    t34 = *((unsigned int *)t13);
    t32 = (t33 - t34);
    t35 = (t32 + 1);
    xsi_vlogvar_wait_assign_value(t3, t4, t30, *((unsigned int *)t13), t35, 0LL);
    goto LAB39;

LAB40:    t31 = *((unsigned int *)t14);
    t30 = (t31 + 0);
    t33 = *((unsigned int *)t12);
    t34 = *((unsigned int *)t13);
    t32 = (t33 - t34);
    t35 = (t32 + 1);
    xsi_vlogvar_wait_assign_value(t3, t4, t30, *((unsigned int *)t13), t35, 0LL);
    goto LAB41;

LAB45:    xsi_set_current_line(119, ng0);
    t11 = (t0 + 1368U);
    t15 = *((char **)t11);
    t11 = (t0 + 3528);
    t16 = (t0 + 3528);
    t17 = (t16 + 72U);
    t18 = *((char **)t17);
    t19 = ((char*)((ng4)));
    t20 = ((char*)((ng3)));
    xsi_vlog_convert_partindices(t12, t13, t14, ((int*)(t18)), 2, t19, 32, 1, t20, 32, 1);
    t23 = (t12 + 4);
    t21 = *((unsigned int *)t23);
    t22 = (!(t21));
    t27 = (t13 + 4);
    t24 = *((unsigned int *)t27);
    t25 = (!(t24));
    t26 = (t22 && t25);
    t37 = (t14 + 4);
    t28 = *((unsigned int *)t37);
    t29 = (!(t28));
    t30 = (t26 && t29);
    if (t30 == 1)
        goto LAB54;

LAB55:    goto LAB53;

LAB47:    xsi_set_current_line(121, ng0);

LAB56:    xsi_set_current_line(122, ng0);
    t3 = (t0 + 1368U);
    t4 = *((char **)t3);
    t3 = (t0 + 3528);
    t10 = (t0 + 3528);
    t11 = (t10 + 72U);
    t15 = *((char **)t11);
    t16 = ((char*)((ng6)));
    t17 = ((char*)((ng7)));
    xsi_vlog_convert_partindices(t12, t13, t14, ((int*)(t15)), 2, t16, 32, 1, t17, 32, 1);
    t18 = (t12 + 4);
    t21 = *((unsigned int *)t18);
    t9 = (!(t21));
    t19 = (t13 + 4);
    t24 = *((unsigned int *)t19);
    t22 = (!(t24));
    t25 = (t9 && t22);
    t20 = (t14 + 4);
    t28 = *((unsigned int *)t20);
    t26 = (!(t28));
    t29 = (t25 && t26);
    if (t29 == 1)
        goto LAB57;

LAB58:    xsi_set_current_line(123, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 3848);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 1, 0LL);
    goto LAB53;

LAB49:    xsi_set_current_line(125, ng0);
    t3 = ((char*)((ng2)));
    t4 = (t0 + 3848);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 1, 0LL);
    goto LAB53;

LAB54:    t31 = *((unsigned int *)t14);
    t32 = (t31 + 0);
    t33 = *((unsigned int *)t12);
    t34 = *((unsigned int *)t13);
    t35 = (t33 - t34);
    t36 = (t35 + 1);
    xsi_vlogvar_wait_assign_value(t11, t15, t32, *((unsigned int *)t13), t36, 0LL);
    goto LAB55;

LAB57:    t31 = *((unsigned int *)t14);
    t30 = (t31 + 0);
    t33 = *((unsigned int *)t12);
    t34 = *((unsigned int *)t13);
    t32 = (t33 - t34);
    t35 = (t32 + 1);
    xsi_vlogvar_wait_assign_value(t3, t4, t30, *((unsigned int *)t13), t35, 0LL);
    goto LAB58;

LAB61:    t16 = (t12 + 4);
    *((unsigned int *)t12) = 1;
    *((unsigned int *)t16) = 1;
    goto LAB62;

LAB63:    xsi_set_current_line(130, ng0);
    t18 = ((char*)((ng1)));
    t19 = (t0 + 4328);
    xsi_vlogvar_wait_assign_value(t19, t18, 0, 0, 4, 0LL);
    goto LAB65;

LAB68:    xsi_set_current_line(138, ng0);
    t15 = ((char*)((ng1)));
    t16 = (t0 + 3688);
    xsi_vlogvar_wait_assign_value(t16, t15, 0, 0, 1, 0LL);
    goto LAB74;

LAB70:    xsi_set_current_line(140, ng0);

LAB75:    xsi_set_current_line(141, ng0);
    t3 = (t0 + 2488U);
    t4 = *((char **)t3);
    t3 = (t0 + 3528);
    xsi_vlogvar_wait_assign_value(t3, t4, 0, 0, 8, 0LL);
    xsi_set_current_line(142, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 3688);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 1, 0LL);
    goto LAB74;

LAB78:    t17 = (t12 + 4);
    *((unsigned int *)t12) = 1;
    *((unsigned int *)t17) = 1;
    goto LAB79;

LAB80:    xsi_set_current_line(148, ng0);
    t19 = ((char*)((ng1)));
    t20 = (t0 + 4328);
    xsi_vlogvar_wait_assign_value(t20, t19, 0, 0, 4, 0LL);
    goto LAB82;

}

static void Cont_162_1(char *t0)
{
    char t3[8];
    char t4[8];
    char *t1;
    char *t2;
    char *t5;
    char *t6;
    char *t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    char *t13;
    char *t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    char *t18;
    char *t19;
    unsigned int t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    char *t24;
    char *t25;
    char *t26;
    char *t27;
    char *t28;
    char *t29;
    char *t30;
    unsigned int t31;
    unsigned int t32;
    char *t33;
    unsigned int t34;
    unsigned int t35;
    char *t36;
    unsigned int t37;
    unsigned int t38;
    char *t39;

LAB0:    t1 = (t0 + 5496U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(162, ng0);
    t2 = (t0 + 4008);
    t5 = (t2 + 56U);
    t6 = *((char **)t5);
    memset(t4, 0, 8);
    t7 = (t6 + 4);
    t8 = *((unsigned int *)t7);
    t9 = (~(t8));
    t10 = *((unsigned int *)t6);
    t11 = (t10 & t9);
    t12 = (t11 & 1U);
    if (t12 != 0)
        goto LAB4;

LAB5:    if (*((unsigned int *)t7) != 0)
        goto LAB6;

LAB7:    t14 = (t4 + 4);
    t15 = *((unsigned int *)t4);
    t16 = *((unsigned int *)t14);
    t17 = (t15 || t16);
    if (t17 > 0)
        goto LAB8;

LAB9:    t20 = *((unsigned int *)t4);
    t21 = (~(t20));
    t22 = *((unsigned int *)t14);
    t23 = (t21 || t22);
    if (t23 > 0)
        goto LAB10;

LAB11:    if (*((unsigned int *)t14) > 0)
        goto LAB12;

LAB13:    if (*((unsigned int *)t4) > 0)
        goto LAB14;

LAB15:    memcpy(t3, t25, 8);

LAB16:    t26 = (t0 + 7496);
    t27 = (t26 + 56U);
    t28 = *((char **)t27);
    t29 = (t28 + 56U);
    t30 = *((char **)t29);
    memset(t30, 0, 8);
    t31 = 65535U;
    t32 = t31;
    t33 = (t3 + 4);
    t34 = *((unsigned int *)t3);
    t31 = (t31 & t34);
    t35 = *((unsigned int *)t33);
    t32 = (t32 & t35);
    t36 = (t30 + 4);
    t37 = *((unsigned int *)t30);
    *((unsigned int *)t30) = (t37 | t31);
    t38 = *((unsigned int *)t36);
    *((unsigned int *)t36) = (t38 | t32);
    xsi_driver_vfirst_trans(t26, 0, 15);
    t39 = (t0 + 7320);
    *((int *)t39) = 1;

LAB1:    return;
LAB4:    *((unsigned int *)t4) = 1;
    goto LAB7;

LAB6:    t13 = (t4 + 4);
    *((unsigned int *)t4) = 1;
    *((unsigned int *)t13) = 1;
    goto LAB7;

LAB8:    t18 = (t0 + 1688U);
    t19 = *((char **)t18);
    goto LAB9;

LAB10:    t18 = (t0 + 3368);
    t24 = (t18 + 56U);
    t25 = *((char **)t24);
    goto LAB11;

LAB12:    xsi_vlog_unsigned_bit_combine(t3, 16, t19, 16, t25, 16);
    goto LAB16;

LAB14:    memcpy(t3, t19, 8);
    goto LAB16;

}

static void Cont_163_2(char *t0)
{
    char t3[8];
    char t4[8];
    char *t1;
    char *t2;
    char *t5;
    char *t6;
    char *t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    char *t13;
    char *t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    char *t18;
    char *t19;
    unsigned int t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    char *t24;
    char *t25;
    char *t26;
    char *t27;
    char *t28;
    char *t29;
    char *t30;
    unsigned int t31;
    unsigned int t32;
    char *t33;
    unsigned int t34;
    unsigned int t35;
    char *t36;
    unsigned int t37;
    unsigned int t38;
    char *t39;

LAB0:    t1 = (t0 + 5744U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(163, ng0);
    t2 = (t0 + 4008);
    t5 = (t2 + 56U);
    t6 = *((char **)t5);
    memset(t4, 0, 8);
    t7 = (t6 + 4);
    t8 = *((unsigned int *)t7);
    t9 = (~(t8));
    t10 = *((unsigned int *)t6);
    t11 = (t10 & t9);
    t12 = (t11 & 1U);
    if (t12 != 0)
        goto LAB4;

LAB5:    if (*((unsigned int *)t7) != 0)
        goto LAB6;

LAB7:    t14 = (t4 + 4);
    t15 = *((unsigned int *)t4);
    t16 = *((unsigned int *)t14);
    t17 = (t15 || t16);
    if (t17 > 0)
        goto LAB8;

LAB9:    t20 = *((unsigned int *)t4);
    t21 = (~(t20));
    t22 = *((unsigned int *)t14);
    t23 = (t21 || t22);
    if (t23 > 0)
        goto LAB10;

LAB11:    if (*((unsigned int *)t14) > 0)
        goto LAB12;

LAB13:    if (*((unsigned int *)t4) > 0)
        goto LAB14;

LAB15:    memcpy(t3, t25, 8);

LAB16:    t26 = (t0 + 7560);
    t27 = (t26 + 56U);
    t28 = *((char **)t27);
    t29 = (t28 + 56U);
    t30 = *((char **)t29);
    memset(t30, 0, 8);
    t31 = 1U;
    t32 = t31;
    t33 = (t3 + 4);
    t34 = *((unsigned int *)t3);
    t31 = (t31 & t34);
    t35 = *((unsigned int *)t33);
    t32 = (t32 & t35);
    t36 = (t30 + 4);
    t37 = *((unsigned int *)t30);
    *((unsigned int *)t30) = (t37 | t31);
    t38 = *((unsigned int *)t36);
    *((unsigned int *)t36) = (t38 | t32);
    xsi_driver_vfirst_trans(t26, 0, 0);
    t39 = (t0 + 7336);
    *((int *)t39) = 1;

LAB1:    return;
LAB4:    *((unsigned int *)t4) = 1;
    goto LAB7;

LAB6:    t13 = (t4 + 4);
    *((unsigned int *)t4) = 1;
    *((unsigned int *)t13) = 1;
    goto LAB7;

LAB8:    t18 = (t0 + 2008U);
    t19 = *((char **)t18);
    goto LAB9;

LAB10:    t18 = (t0 + 3688);
    t24 = (t18 + 56U);
    t25 = *((char **)t24);
    goto LAB11;

LAB12:    xsi_vlog_unsigned_bit_combine(t3, 1, t19, 1, t25, 1);
    goto LAB16;

LAB14:    memcpy(t3, t19, 8);
    goto LAB16;

}

static void Cont_164_3(char *t0)
{
    char t3[8];
    char t4[8];
    char *t1;
    char *t2;
    char *t5;
    char *t6;
    char *t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    char *t13;
    char *t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    char *t18;
    char *t19;
    unsigned int t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    char *t24;
    char *t25;
    char *t26;
    char *t27;
    char *t28;
    char *t29;
    char *t30;
    unsigned int t31;
    unsigned int t32;
    char *t33;
    unsigned int t34;
    unsigned int t35;
    char *t36;
    unsigned int t37;
    unsigned int t38;
    char *t39;

LAB0:    t1 = (t0 + 5992U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(164, ng0);
    t2 = (t0 + 4008);
    t5 = (t2 + 56U);
    t6 = *((char **)t5);
    memset(t4, 0, 8);
    t7 = (t6 + 4);
    t8 = *((unsigned int *)t7);
    t9 = (~(t8));
    t10 = *((unsigned int *)t6);
    t11 = (t10 & t9);
    t12 = (t11 & 1U);
    if (t12 != 0)
        goto LAB4;

LAB5:    if (*((unsigned int *)t7) != 0)
        goto LAB6;

LAB7:    t14 = (t4 + 4);
    t15 = *((unsigned int *)t4);
    t16 = *((unsigned int *)t14);
    t17 = (t15 || t16);
    if (t17 > 0)
        goto LAB8;

LAB9:    t20 = *((unsigned int *)t4);
    t21 = (~(t20));
    t22 = *((unsigned int *)t14);
    t23 = (t21 || t22);
    if (t23 > 0)
        goto LAB10;

LAB11:    if (*((unsigned int *)t14) > 0)
        goto LAB12;

LAB13:    if (*((unsigned int *)t4) > 0)
        goto LAB14;

LAB15:    memcpy(t3, t25, 8);

LAB16:    t26 = (t0 + 7624);
    t27 = (t26 + 56U);
    t28 = *((char **)t27);
    t29 = (t28 + 56U);
    t30 = *((char **)t29);
    memset(t30, 0, 8);
    t31 = 1U;
    t32 = t31;
    t33 = (t3 + 4);
    t34 = *((unsigned int *)t3);
    t31 = (t31 & t34);
    t35 = *((unsigned int *)t33);
    t32 = (t32 & t35);
    t36 = (t30 + 4);
    t37 = *((unsigned int *)t30);
    *((unsigned int *)t30) = (t37 | t31);
    t38 = *((unsigned int *)t36);
    *((unsigned int *)t36) = (t38 | t32);
    xsi_driver_vfirst_trans(t26, 0, 0);
    t39 = (t0 + 7352);
    *((int *)t39) = 1;

LAB1:    return;
LAB4:    *((unsigned int *)t4) = 1;
    goto LAB7;

LAB6:    t13 = (t4 + 4);
    *((unsigned int *)t4) = 1;
    *((unsigned int *)t13) = 1;
    goto LAB7;

LAB8:    t18 = (t0 + 2168U);
    t19 = *((char **)t18);
    goto LAB9;

LAB10:    t18 = (t0 + 3848);
    t24 = (t18 + 56U);
    t25 = *((char **)t24);
    goto LAB11;

LAB12:    xsi_vlog_unsigned_bit_combine(t3, 1, t19, 1, t25, 1);
    goto LAB16;

LAB14:    memcpy(t3, t19, 8);
    goto LAB16;

}

static void Cont_165_4(char *t0)
{
    char t3[8];
    char t4[8];
    char t5[8];
    char t15[8];
    char t57[8];
    char t58[8];
    char *t1;
    char *t2;
    char *t6;
    unsigned int t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    char *t12;
    char *t13;
    char *t14;
    unsigned int t16;
    unsigned int t17;
    unsigned int t18;
    char *t19;
    char *t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    unsigned int t24;
    unsigned int t25;
    unsigned int t26;
    unsigned int t27;
    char *t28;
    char *t29;
    unsigned int t30;
    unsigned int t31;
    unsigned int t32;
    unsigned int t33;
    unsigned int t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    int t38;
    int t39;
    unsigned int t40;
    unsigned int t41;
    unsigned int t42;
    unsigned int t43;
    unsigned int t44;
    unsigned int t45;
    char *t46;
    unsigned int t47;
    unsigned int t48;
    unsigned int t49;
    unsigned int t50;
    unsigned int t51;
    char *t52;
    char *t53;
    unsigned int t54;
    unsigned int t55;
    unsigned int t56;
    char *t59;
    char *t60;
    char *t61;
    char *t62;
    unsigned int t63;
    unsigned int t64;
    unsigned int t65;
    unsigned int t66;
    unsigned int t67;
    char *t68;
    char *t69;
    unsigned int t70;
    unsigned int t71;
    unsigned int t72;
    char *t73;
    char *t74;
    unsigned int t75;
    unsigned int t76;
    unsigned int t77;
    unsigned int t78;
    char *t79;
    char *t80;
    unsigned int t81;
    unsigned int t82;
    unsigned int t83;
    unsigned int t84;
    char *t85;
    char *t86;
    char *t87;
    char *t88;
    char *t89;
    char *t90;
    unsigned int t91;
    unsigned int t92;
    char *t93;
    unsigned int t94;
    unsigned int t95;
    char *t96;
    unsigned int t97;
    unsigned int t98;
    char *t99;

LAB0:    t1 = (t0 + 6240U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(165, ng0);
    t2 = (t0 + 2808U);
    t6 = *((char **)t2);
    memset(t5, 0, 8);
    t2 = (t6 + 4);
    t7 = *((unsigned int *)t2);
    t8 = (~(t7));
    t9 = *((unsigned int *)t6);
    t10 = (t9 & t8);
    t11 = (t10 & 1U);
    if (t11 != 0)
        goto LAB7;

LAB5:    if (*((unsigned int *)t2) == 0)
        goto LAB4;

LAB6:    t12 = (t5 + 4);
    *((unsigned int *)t5) = 1;
    *((unsigned int *)t12) = 1;

LAB7:    t13 = (t0 + 2648U);
    t14 = *((char **)t13);
    t16 = *((unsigned int *)t5);
    t17 = *((unsigned int *)t14);
    t18 = (t16 & t17);
    *((unsigned int *)t15) = t18;
    t13 = (t5 + 4);
    t19 = (t14 + 4);
    t20 = (t15 + 4);
    t21 = *((unsigned int *)t13);
    t22 = *((unsigned int *)t19);
    t23 = (t21 | t22);
    *((unsigned int *)t20) = t23;
    t24 = *((unsigned int *)t20);
    t25 = (t24 != 0);
    if (t25 == 1)
        goto LAB8;

LAB9:
LAB10:    memset(t4, 0, 8);
    t46 = (t15 + 4);
    t47 = *((unsigned int *)t46);
    t48 = (~(t47));
    t49 = *((unsigned int *)t15);
    t50 = (t49 & t48);
    t51 = (t50 & 1U);
    if (t51 != 0)
        goto LAB11;

LAB12:    if (*((unsigned int *)t46) != 0)
        goto LAB13;

LAB14:    t53 = (t4 + 4);
    t54 = *((unsigned int *)t4);
    t55 = *((unsigned int *)t53);
    t56 = (t54 || t55);
    if (t56 > 0)
        goto LAB15;

LAB16:    t81 = *((unsigned int *)t4);
    t82 = (~(t81));
    t83 = *((unsigned int *)t53);
    t84 = (t82 || t83);
    if (t84 > 0)
        goto LAB17;

LAB18:    if (*((unsigned int *)t53) > 0)
        goto LAB19;

LAB20:    if (*((unsigned int *)t4) > 0)
        goto LAB21;

LAB22:    memcpy(t3, t85, 8);

LAB23:    t86 = (t0 + 7688);
    t87 = (t86 + 56U);
    t88 = *((char **)t87);
    t89 = (t88 + 56U);
    t90 = *((char **)t89);
    memset(t90, 0, 8);
    t91 = 255U;
    t92 = t91;
    t93 = (t3 + 4);
    t94 = *((unsigned int *)t3);
    t91 = (t91 & t94);
    t95 = *((unsigned int *)t93);
    t92 = (t92 & t95);
    t96 = (t90 + 4);
    t97 = *((unsigned int *)t90);
    *((unsigned int *)t90) = (t97 | t91);
    t98 = *((unsigned int *)t96);
    *((unsigned int *)t96) = (t98 | t92);
    xsi_driver_vfirst_trans(t86, 0, 7);
    t99 = (t0 + 7368);
    *((int *)t99) = 1;

LAB1:    return;
LAB4:    *((unsigned int *)t5) = 1;
    goto LAB7;

LAB8:    t26 = *((unsigned int *)t15);
    t27 = *((unsigned int *)t20);
    *((unsigned int *)t15) = (t26 | t27);
    t28 = (t5 + 4);
    t29 = (t14 + 4);
    t30 = *((unsigned int *)t5);
    t31 = (~(t30));
    t32 = *((unsigned int *)t28);
    t33 = (~(t32));
    t34 = *((unsigned int *)t14);
    t35 = (~(t34));
    t36 = *((unsigned int *)t29);
    t37 = (~(t36));
    t38 = (t31 & t33);
    t39 = (t35 & t37);
    t40 = (~(t38));
    t41 = (~(t39));
    t42 = *((unsigned int *)t20);
    *((unsigned int *)t20) = (t42 & t40);
    t43 = *((unsigned int *)t20);
    *((unsigned int *)t20) = (t43 & t41);
    t44 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t44 & t40);
    t45 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t45 & t41);
    goto LAB10;

LAB11:    *((unsigned int *)t4) = 1;
    goto LAB14;

LAB13:    t52 = (t4 + 4);
    *((unsigned int *)t4) = 1;
    *((unsigned int *)t52) = 1;
    goto LAB14;

LAB15:    t59 = (t0 + 4008);
    t60 = (t59 + 56U);
    t61 = *((char **)t60);
    memset(t58, 0, 8);
    t62 = (t61 + 4);
    t63 = *((unsigned int *)t62);
    t64 = (~(t63));
    t65 = *((unsigned int *)t61);
    t66 = (t65 & t64);
    t67 = (t66 & 1U);
    if (t67 != 0)
        goto LAB24;

LAB25:    if (*((unsigned int *)t62) != 0)
        goto LAB26;

LAB27:    t69 = (t58 + 4);
    t70 = *((unsigned int *)t58);
    t71 = *((unsigned int *)t69);
    t72 = (t70 || t71);
    if (t72 > 0)
        goto LAB28;

LAB29:    t75 = *((unsigned int *)t58);
    t76 = (~(t75));
    t77 = *((unsigned int *)t69);
    t78 = (t76 || t77);
    if (t78 > 0)
        goto LAB30;

LAB31:    if (*((unsigned int *)t69) > 0)
        goto LAB32;

LAB33:    if (*((unsigned int *)t58) > 0)
        goto LAB34;

LAB35:    memcpy(t57, t80, 8);

LAB36:    goto LAB16;

LAB17:    t85 = ((char*)((ng18)));
    goto LAB18;

LAB19:    xsi_vlog_unsigned_bit_combine(t3, 8, t57, 8, t85, 8);
    goto LAB23;

LAB21:    memcpy(t3, t57, 8);
    goto LAB23;

LAB24:    *((unsigned int *)t58) = 1;
    goto LAB27;

LAB26:    t68 = (t58 + 4);
    *((unsigned int *)t58) = 1;
    *((unsigned int *)t68) = 1;
    goto LAB27;

LAB28:    t73 = (t0 + 1848U);
    t74 = *((char **)t73);
    goto LAB29;

LAB30:    t73 = (t0 + 3528);
    t79 = (t73 + 56U);
    t80 = *((char **)t79);
    goto LAB31;

LAB32:    xsi_vlog_unsigned_bit_combine(t57, 8, t74, 8, t80, 8);
    goto LAB36;

LAB34:    memcpy(t57, t74, 8);
    goto LAB36;

}

static void Cont_166_5(char *t0)
{
    char t3[8];
    char t4[8];
    char t5[8];
    char t15[8];
    char t49[8];
    char *t1;
    char *t2;
    char *t6;
    unsigned int t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    char *t12;
    char *t13;
    char *t14;
    unsigned int t16;
    unsigned int t17;
    unsigned int t18;
    char *t19;
    char *t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    unsigned int t24;
    unsigned int t25;
    unsigned int t26;
    unsigned int t27;
    char *t28;
    char *t29;
    unsigned int t30;
    unsigned int t31;
    unsigned int t32;
    unsigned int t33;
    unsigned int t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    int t38;
    int t39;
    unsigned int t40;
    unsigned int t41;
    unsigned int t42;
    unsigned int t43;
    unsigned int t44;
    unsigned int t45;
    char *t46;
    char *t47;
    char *t48;
    unsigned int t50;
    unsigned int t51;
    unsigned int t52;
    char *t53;
    char *t54;
    char *t55;
    unsigned int t56;
    unsigned int t57;
    unsigned int t58;
    unsigned int t59;
    unsigned int t60;
    unsigned int t61;
    unsigned int t62;
    char *t63;
    char *t64;
    unsigned int t65;
    unsigned int t66;
    unsigned int t67;
    unsigned int t68;
    unsigned int t69;
    unsigned int t70;
    unsigned int t71;
    unsigned int t72;
    int t73;
    int t74;
    unsigned int t75;
    unsigned int t76;
    unsigned int t77;
    unsigned int t78;
    unsigned int t79;
    unsigned int t80;
    char *t81;
    unsigned int t82;
    unsigned int t83;
    unsigned int t84;
    unsigned int t85;
    unsigned int t86;
    char *t87;
    char *t88;
    unsigned int t89;
    unsigned int t90;
    unsigned int t91;
    char *t92;
    char *t93;
    unsigned int t94;
    unsigned int t95;
    unsigned int t96;
    unsigned int t97;
    char *t98;
    char *t99;
    char *t100;
    char *t101;
    char *t102;
    unsigned int t103;
    unsigned int t104;
    char *t105;
    unsigned int t106;
    unsigned int t107;
    char *t108;
    unsigned int t109;
    unsigned int t110;
    char *t111;

LAB0:    t1 = (t0 + 6488U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(166, ng0);
    t2 = (t0 + 2008U);
    t6 = *((char **)t2);
    memset(t5, 0, 8);
    t2 = (t6 + 4);
    t7 = *((unsigned int *)t2);
    t8 = (~(t7));
    t9 = *((unsigned int *)t6);
    t10 = (t9 & t8);
    t11 = (t10 & 1U);
    if (t11 != 0)
        goto LAB7;

LAB5:    if (*((unsigned int *)t2) == 0)
        goto LAB4;

LAB6:    t12 = (t5 + 4);
    *((unsigned int *)t5) = 1;
    *((unsigned int *)t12) = 1;

LAB7:    t13 = (t0 + 2168U);
    t14 = *((char **)t13);
    t16 = *((unsigned int *)t5);
    t17 = *((unsigned int *)t14);
    t18 = (t16 & t17);
    *((unsigned int *)t15) = t18;
    t13 = (t5 + 4);
    t19 = (t14 + 4);
    t20 = (t15 + 4);
    t21 = *((unsigned int *)t13);
    t22 = *((unsigned int *)t19);
    t23 = (t21 | t22);
    *((unsigned int *)t20) = t23;
    t24 = *((unsigned int *)t20);
    t25 = (t24 != 0);
    if (t25 == 1)
        goto LAB8;

LAB9:
LAB10:    t46 = (t0 + 4008);
    t47 = (t46 + 56U);
    t48 = *((char **)t47);
    t50 = *((unsigned int *)t15);
    t51 = *((unsigned int *)t48);
    t52 = (t50 & t51);
    *((unsigned int *)t49) = t52;
    t53 = (t15 + 4);
    t54 = (t48 + 4);
    t55 = (t49 + 4);
    t56 = *((unsigned int *)t53);
    t57 = *((unsigned int *)t54);
    t58 = (t56 | t57);
    *((unsigned int *)t55) = t58;
    t59 = *((unsigned int *)t55);
    t60 = (t59 != 0);
    if (t60 == 1)
        goto LAB11;

LAB12:
LAB13:    memset(t4, 0, 8);
    t81 = (t49 + 4);
    t82 = *((unsigned int *)t81);
    t83 = (~(t82));
    t84 = *((unsigned int *)t49);
    t85 = (t84 & t83);
    t86 = (t85 & 1U);
    if (t86 != 0)
        goto LAB14;

LAB15:    if (*((unsigned int *)t81) != 0)
        goto LAB16;

LAB17:    t88 = (t4 + 4);
    t89 = *((unsigned int *)t4);
    t90 = *((unsigned int *)t88);
    t91 = (t89 || t90);
    if (t91 > 0)
        goto LAB18;

LAB19:    t94 = *((unsigned int *)t4);
    t95 = (~(t94));
    t96 = *((unsigned int *)t88);
    t97 = (t95 || t96);
    if (t97 > 0)
        goto LAB20;

LAB21:    if (*((unsigned int *)t88) > 0)
        goto LAB22;

LAB23:    if (*((unsigned int *)t4) > 0)
        goto LAB24;

LAB25:    memcpy(t3, t92, 8);

LAB26:    t98 = (t0 + 7752);
    t99 = (t98 + 56U);
    t100 = *((char **)t99);
    t101 = (t100 + 56U);
    t102 = *((char **)t101);
    memset(t102, 0, 8);
    t103 = 255U;
    t104 = t103;
    t105 = (t3 + 4);
    t106 = *((unsigned int *)t3);
    t103 = (t103 & t106);
    t107 = *((unsigned int *)t105);
    t104 = (t104 & t107);
    t108 = (t102 + 4);
    t109 = *((unsigned int *)t102);
    *((unsigned int *)t102) = (t109 | t103);
    t110 = *((unsigned int *)t108);
    *((unsigned int *)t108) = (t110 | t104);
    xsi_driver_vfirst_trans(t98, 0, 7);
    t111 = (t0 + 7384);
    *((int *)t111) = 1;

LAB1:    return;
LAB4:    *((unsigned int *)t5) = 1;
    goto LAB7;

LAB8:    t26 = *((unsigned int *)t15);
    t27 = *((unsigned int *)t20);
    *((unsigned int *)t15) = (t26 | t27);
    t28 = (t5 + 4);
    t29 = (t14 + 4);
    t30 = *((unsigned int *)t5);
    t31 = (~(t30));
    t32 = *((unsigned int *)t28);
    t33 = (~(t32));
    t34 = *((unsigned int *)t14);
    t35 = (~(t34));
    t36 = *((unsigned int *)t29);
    t37 = (~(t36));
    t38 = (t31 & t33);
    t39 = (t35 & t37);
    t40 = (~(t38));
    t41 = (~(t39));
    t42 = *((unsigned int *)t20);
    *((unsigned int *)t20) = (t42 & t40);
    t43 = *((unsigned int *)t20);
    *((unsigned int *)t20) = (t43 & t41);
    t44 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t44 & t40);
    t45 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t45 & t41);
    goto LAB10;

LAB11:    t61 = *((unsigned int *)t49);
    t62 = *((unsigned int *)t55);
    *((unsigned int *)t49) = (t61 | t62);
    t63 = (t15 + 4);
    t64 = (t48 + 4);
    t65 = *((unsigned int *)t15);
    t66 = (~(t65));
    t67 = *((unsigned int *)t63);
    t68 = (~(t67));
    t69 = *((unsigned int *)t48);
    t70 = (~(t69));
    t71 = *((unsigned int *)t64);
    t72 = (~(t71));
    t73 = (t66 & t68);
    t74 = (t70 & t72);
    t75 = (~(t73));
    t76 = (~(t74));
    t77 = *((unsigned int *)t55);
    *((unsigned int *)t55) = (t77 & t75);
    t78 = *((unsigned int *)t55);
    *((unsigned int *)t55) = (t78 & t76);
    t79 = *((unsigned int *)t49);
    *((unsigned int *)t49) = (t79 & t75);
    t80 = *((unsigned int *)t49);
    *((unsigned int *)t49) = (t80 & t76);
    goto LAB13;

LAB14:    *((unsigned int *)t4) = 1;
    goto LAB17;

LAB16:    t87 = (t4 + 4);
    *((unsigned int *)t4) = 1;
    *((unsigned int *)t87) = 1;
    goto LAB17;

LAB18:    t92 = (t0 + 2488U);
    t93 = *((char **)t92);
    goto LAB19;

LAB20:    t92 = ((char*)((ng18)));
    goto LAB21;

LAB22:    xsi_vlog_unsigned_bit_combine(t3, 8, t93, 8, t92, 8);
    goto LAB26;

LAB24:    memcpy(t3, t93, 8);
    goto LAB26;

}

static void Cont_167_6(char *t0)
{
    char t5[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t6;
    char *t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    unsigned int t11;
    unsigned int t12;
    unsigned int t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;
    char *t18;
    unsigned int t19;
    unsigned int t20;
    char *t21;
    unsigned int t22;
    unsigned int t23;
    char *t24;
    unsigned int t25;
    unsigned int t26;
    char *t27;

LAB0:    t1 = (t0 + 6736U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(167, ng0);
    t2 = (t0 + 3528);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    memset(t5, 0, 8);
    t6 = (t5 + 4);
    t7 = (t4 + 4);
    t8 = *((unsigned int *)t4);
    t9 = (t8 >> 0);
    t10 = (t9 & 1);
    *((unsigned int *)t5) = t10;
    t11 = *((unsigned int *)t7);
    t12 = (t11 >> 0);
    t13 = (t12 & 1);
    *((unsigned int *)t6) = t13;
    t14 = (t0 + 7816);
    t15 = (t14 + 56U);
    t16 = *((char **)t15);
    t17 = (t16 + 56U);
    t18 = *((char **)t17);
    memset(t18, 0, 8);
    t19 = 1U;
    t20 = t19;
    t21 = (t5 + 4);
    t22 = *((unsigned int *)t5);
    t19 = (t19 & t22);
    t23 = *((unsigned int *)t21);
    t20 = (t20 & t23);
    t24 = (t18 + 4);
    t25 = *((unsigned int *)t18);
    *((unsigned int *)t18) = (t25 | t19);
    t26 = *((unsigned int *)t24);
    *((unsigned int *)t24) = (t26 | t20);
    xsi_driver_vfirst_trans(t14, 0, 0);
    t27 = (t0 + 7400);
    *((int *)t27) = 1;

LAB1:    return;
}

static void Cont_168_7(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    unsigned int t10;
    unsigned int t11;
    char *t12;
    unsigned int t13;
    unsigned int t14;
    char *t15;
    unsigned int t16;
    unsigned int t17;
    char *t18;

LAB0:    t1 = (t0 + 6984U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(168, ng0);
    t2 = (t0 + 4008);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 7880);
    t6 = (t5 + 56U);
    t7 = *((char **)t6);
    t8 = (t7 + 56U);
    t9 = *((char **)t8);
    memset(t9, 0, 8);
    t10 = 1U;
    t11 = t10;
    t12 = (t4 + 4);
    t13 = *((unsigned int *)t4);
    t10 = (t10 & t13);
    t14 = *((unsigned int *)t12);
    t11 = (t11 & t14);
    t15 = (t9 + 4);
    t16 = *((unsigned int *)t9);
    *((unsigned int *)t9) = (t16 | t10);
    t17 = *((unsigned int *)t15);
    *((unsigned int *)t15) = (t17 | t11);
    xsi_driver_vfirst_trans(t5, 0, 0);
    t18 = (t0 + 7416);
    *((int *)t18) = 1;

LAB1:    return;
}


extern void work_m_00000000003932635965_1925715230_init()
{
	static char *pe[] = {(void *)Always_84_0,(void *)Cont_162_1,(void *)Cont_163_2,(void *)Cont_164_3,(void *)Cont_165_4,(void *)Cont_166_5,(void *)Cont_167_6,(void *)Cont_168_7};
	xsi_register_didat("work_m_00000000003932635965_1925715230", "isim/RomEmuCtrl_isim_beh.exe.sim/work/m_00000000003932635965_1925715230.didat");
	xsi_register_executes(pe);
}
