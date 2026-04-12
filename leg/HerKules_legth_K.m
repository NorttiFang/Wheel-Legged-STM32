function K = HerKules_legth_K(legth)

%   K = HerKules_legth_K(0.18);

    arguments
        legth (1,1) double {mustBeReal, mustBePositive}
    end

    %% ===================== 符号与模型 =====================
    syms R_w R_l l_l l_r l_wl l_wr l_bl l_br l_c m_w m_l m_b I_w I_ll I_lr I_b I_z g
    syms theta_wl theta_wr dtheta_wl dtheta_wr ddtheta_wl ddtheta_wr
    syms theta_ll theta_lr dtheta_ll dtheta_lr ddtheta_ll ddtheta_lr
    syms theta_b dtheta_b ddtheta_b
    syms T_wl T_wr T_bl T_br

    % 原文方程组(3.11)-(3.15)
    eqn1 = (I_w*l_l/R_w + m_w*R_w*l_l + m_l*R_w*l_bl) * ddtheta_wl ...
         + (m_l*l_wl*l_bl - I_ll) * ddtheta_ll ...
         + (m_l*l_wl + m_b*l_l/2) * g * theta_ll ...
         + T_bl - T_wl*(1 + l_l/R_w) == 0;

    eqn2 = (I_w*l_r/R_w + m_w*R_w*l_r + m_l*R_w*l_br) * ddtheta_wr ...
         + (m_l*l_wr*l_br - I_lr) * ddtheta_lr ...
         + (m_l*l_wr + m_b*l_r/2) * g * theta_lr ...
         + T_br - T_wr*(1 + l_r/R_w) == 0;

    eqn3 = -(m_w*R_w^2 + I_w + m_l*R_w^2 + m_b*R_w^2/2) * ddtheta_wl ...
         - (m_w*R_w^2 + I_w + m_l*R_w^2 + m_b*R_w^2/2) * ddtheta_wr ...
         - (m_l*R_w*l_wl + m_b*R_w*l_l/2) * ddtheta_ll ...
         - (m_l*R_w*l_wr + m_b*R_w*l_r/2) * ddtheta_lr ...
         + T_wl + T_wr == 0;

    eqn4 = (m_w*R_w*l_c + I_w*l_c/R_w + m_l*R_w*l_c) * ddtheta_wl ...
         + (m_w*R_w*l_c + I_w*l_c/R_w + m_l*R_w*l_c) * ddtheta_wr ...
         + m_l*l_wl*l_c * ddtheta_ll ...
         + m_l*l_wr*l_c * ddtheta_lr ...
         - I_b*ddtheta_b + m_b*g*l_c*theta_b ...
         - (T_wl + T_wr)*l_c/R_w - (T_bl + T_br) == 0;

    eqn5 = (I_z*R_w/(2*R_l) + I_w*R_l/R_w) * ddtheta_wl ...
         - (I_z*R_w/(2*R_l) + I_w*R_l/R_w) * ddtheta_wr ...
         + (I_z*l_l/(2*R_l)) * ddtheta_ll ...
         - (I_z*l_r/(2*R_l)) * ddtheta_lr ...
         - T_wl*R_l/R_w + T_wr*R_l/R_w == 0;

    [ddtheta_wl_sol, ddtheta_wr_sol, ddtheta_ll_sol, ddtheta_lr_sol, ddtheta_b_sol] = solve( ...
        eqn1, eqn2, eqn3, eqn4, eqn5, ddtheta_wl, ddtheta_wr, ddtheta_ll, ddtheta_lr, ddtheta_b);

    % 偏导（与原脚本一致）
    J_A = jacobian([ddtheta_wl_sol, ddtheta_wr_sol, ddtheta_ll_sol, ddtheta_lr_sol, ddtheta_b_sol], ...
                   [theta_ll, theta_lr, theta_b]);
    J_B = jacobian([ddtheta_wl_sol, ddtheta_wr_sol, ddtheta_ll_sol, ddtheta_lr_sol, ddtheta_b_sol], ...
                   [T_wl, T_wr, T_bl, T_br]);

    % 构建 A(10x10), B(10x4)
    A = sym('A', [10 10]);
    B = sym('B', [10 4]);

    for p = 5:2:9
        A_index = (p - 3) / 2;
        A(2,p) = R_w * (J_A(1,A_index) + J_A(2,A_index)) / 2;
        A(4,p) = R_w * (-J_A(1,A_index) + J_A(2,A_index)) / (2*R_l) ...
               - l_l * J_A(3,A_index) / (2*R_l) + l_r * J_A(4,A_index) / (2*R_l);
        for q = 6:2:10
            A(q,p) = J_A(q/2, A_index);
        end
    end

    for r = 1:10
        if rem(r,2) == 0
            A(r,1)=0; A(r,2)=0; A(r,3)=0; A(r,4)=0; A(r,6)=0; A(r,8)=0; A(r,10)=0;
        else
            A(r,:) = zeros(1,10);
            A(r,r+1) = 1;
        end
    end

    for h = 1:4
        B(2,h) = R_w * (J_B(1,h) + J_B(2,h)) / 2;
        B(4,h) = R_w * (-J_B(1,h) + J_B(2,h)) / (2*R_l) ...
               - l_l * J_B(3,h) / (2*R_l) + l_r * J_B(4,h) / (2*R_l);
        for f = 6:2:10
            B(f,h) = J_B(f/2, h);
        end
    end
    for e = 1:2:9
        B(e,:) = zeros(1,4);
    end

    %% ===================== 数值参数（保持原 HerKules 参数） =====================
    g_ac   = 9.81;
    R_w_ac = 0.06;
    R_l_ac = 0.25;
    l_c_ac = 0.037;

    m_w_ac = 0.8;
    m_l_ac = 1.6183599;
    m_b_ac = 11.542;

    I_w_ac = 3510000e-7;
    I_b_ac = 0.260;
    I_z_ac = 0.226;

    % 使用原脚本腿部数据集，并对单腿长做插值（左右对称）
    Leg_data = [0.11,  0.0480,  0.0620,  0.01819599;
                0.12,  0.0470,  0.0730,  0.01862845;
                0.13,  0.0476,  0.0824,  0.01898641;
                0.14,  0.0480,  0.0920,  0.01931342;
                0.15,  0.0490,  0.1010,  0.01962521;
                0.16,  0.0500,  0.1100,  0.01993092;
                0.17,  0.0510,  0.1190,  0.02023626;
                0.18,  0.0525,  0.1275,  0.02054500;
                0.19,  0.0539,  0.1361,  0.02085969;
                0.20,  0.0554,  0.1446,  0.02118212;
                0.21,  0.0570,  0.1530,  0.02151357;
                0.22,  0.0586,  0.1614,  0.02185496;
                0.23,  0.0600,  0.1700,  0.02220695;
                0.24,  0.0621,  0.1779,  0.02256999;
                0.25,  0.0639,  0.1861,  0.02294442;
                0.26,  0.0657,  0.1943,  0.02333041;
                0.27,  0.0676,  0.2024,  0.02372806;
                0.28,  0.0700,  0.2100,  0.02413735;
                0.29,  0.0713,  0.2187,  0.02455817;
                0.30,  0.0733,  0.2267,  0.02499030];

    L = Leg_data(:,1);
    if legth < min(L) || legth > max(L)
        error('HerKules_legth_K:OutOfRange', ...
              'legth=%.4f 超出数据集范围 [%.2f, %.2f] m。', legth, min(L), max(L));
    end

    l_l_ac  = legth;
    l_r_ac  = legth;
    l_wl_ac = interp1(L, Leg_data(:,2), legth, 'pchip');
    l_bl_ac = interp1(L, Leg_data(:,3), legth, 'pchip');
    I_ll_ac = interp1(L, Leg_data(:,4), legth, 'pchip');

    l_wr_ac = l_wl_ac;
    l_br_ac = l_bl_ac;
    I_lr_ac = I_ll_ac;

    %% ===================== LQR（保持原维度与权重） =====================
    lqr_Q = [20, 0,    0,    0,    0,     0,    0,     0,    0,     0;
             0,  5,    0,    0,    0,     0,    0,     0,    0,     0;
             0,  0, 8000,    0,    0,     0,    0,     0,    0,     0;
             0,  0,    0,   30,    0,     0,    0,     0,    0,     0;
             0,  0,    0,    0, 16000,    0,    0,     0,    0,     0;
             0,  0,    0,    0,    0,    60,    0,     0,    0,     0;
             0,  0,    0,    0,    0,     0, 16000,    0,    0,     0;
             0,  0,    0,    0,    0,     0,    0,    60,    0,     0;
             0,  0,    0,    0,    0,     0,    0,     0, 24000,    0;
             0,  0,    0,    0,    0,     0,    0,     0,    0,   120];

    lqr_R = [0.6, 0,   0,   0;
             0,  0.6, 0,   0;
             0,   0,  0.2, 0;
             0,   0,  0,  0.2];

    %% ===================== 代入求解 K =====================
    A_ac = subs(A, [R_w R_l l_l l_r l_wl l_wr l_bl l_br l_c m_w m_l m_b I_w I_ll I_lr I_b I_z g], ...
                   [R_w_ac R_l_ac l_l_ac l_r_ac l_wl_ac l_wr_ac l_bl_ac l_br_ac l_c_ac m_w_ac m_l_ac m_b_ac I_w_ac I_ll_ac I_lr_ac I_b_ac I_z_ac g_ac]);
    B_ac = subs(B, [R_w R_l l_l l_r l_wl l_wr l_bl l_br l_c m_w m_l m_b I_w I_ll I_lr I_b I_z g], ...
                   [R_w_ac R_l_ac l_l_ac l_r_ac l_wl_ac l_wr_ac l_bl_ac l_br_ac l_c_ac m_w_ac m_l_ac m_b_ac I_w_ac I_ll_ac I_lr_ac I_b_ac I_z_ac g_ac]);

    A_num = double(A_ac);
    B_num = double(B_ac);

    try
        [~, K, ~] = icare(A_num, B_num, lqr_Q, lqr_R, [], [], []);
    catch
        % 若环境不支持 icare，则回退到 lqr
        K = lqr(A_num, B_num, lqr_Q, lqr_R);
    end
end
