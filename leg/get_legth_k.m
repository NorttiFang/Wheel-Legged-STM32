function K = get_legth_k(leg_length)
   
%theta : 摆杆与竖直方向夹角             R   ：驱动轮半径
%x     : 驱动轮位移                    L   : 摆杆重心到驱动轮轴距离
%phi   : 机体与水平夹角                LM  : 摆杆重心到其转轴距离
%T     ：驱动轮输出力矩                 l   : 机体重心到其转轴距离
%Tp    : 髋关节输出力矩                 mw  : 驱动轮转子质量
%N     ：驱动轮对摆杆力的水平分量        mp  : 摆杆质量
%P     ：驱动轮对摆杆力的竖直分量        M   : 机体质量
%Nm    ：摆杆对机体力水平方向分量        Iw  : 驱动轮转子转动惯量
%Pm    ：摆杆对机体力竖直方向分量        Ip  : 摆杆绕质心转动惯量
%Nf    : 地面对驱动轮摩擦力             Im  : 机体绕质心转动惯量

syms x(t) theta(t) phi(t) T R Iw mw M L LM  l mp g Tp Ip IM
syms dd_theta dd_x dd_phi d_theta d_x d_phi theta0 x0 phi0 F1 F2 F3
%leg_length = 0.18;
R1=0.077;                         %驱动轮半径
L1=leg_length/2;                  %摆杆重心到驱动轮轴距离
LM1=leg_length/2;                 %摆杆重心到其转轴距离
l1=0.03;                          %机体质心距离转轴距离 
mw1=0.499;                         %驱动轮质量
mp1=1.1;                         %杆质量
M1= 7.32;      
Iw1=mw1*R1^2;                     %驱动轮转动惯量
Ip1=mp1*((L1+LM1)^2+0.05^2)/12.0;
%Ip1=mp1*L1^2/12.0;             %摆杆转动惯量
IM1=M1*(0.284^2+0.183^2)/12.0;       %机体绕质心转动惯量

    
NM = M*diff(x + (L + LM )*sin(theta)-l*sin(phi),t,2); %diffx + (L + LM )*sin(theta)-l*sin(phi)对t求导
N = NM + mp*diff(x + L*sin(theta),t,2);%diff(x + L*sin(theta),t,2)对t求导
PM = M*g + M*diff((L+LM)*cos(theta)+l*cos(phi),t,2);
P = PM +mp*g+mp*diff(L*cos(theta),t,2);

eqn1 = diff(x,t,2) == (T -N*R)/(Iw/R + mw*R);
eqn2 = Ip*diff(theta,t,2) == (P*L + PM*LM)*sin(theta)-(N*L+NM*LM)*cos(theta)-T+Tp;
eqn3 = IM*diff(phi,t,2) == Tp +NM*l*cos(phi)+PM*l*sin(phi);

f1 = subs(eqn1, [diff(theta(t), t, t), diff(x(t), t, t), diff(phi(t), t, t), diff(theta(t), t), diff(x(t), t), diff(phi(t), t), theta(t), x(t), phi(t)], [dd_theta, dd_x, dd_phi, d_theta, d_x, d_phi, theta0, x0, phi0]);
f2 = subs(eqn2, [diff(theta(t), t, t), diff(x(t), t, t), diff(phi(t), t, t), diff(theta(t), t), diff(x(t), t), diff(phi(t), t), theta(t), x(t), phi(t)], [dd_theta, dd_x, dd_phi, d_theta, d_x, d_phi, theta0, x0, phi0]);
f3 = subs(eqn3, [diff(theta(t), t, t), diff(x(t), t, t), diff(phi(t), t, t), diff(theta(t), t), diff(x(t), t), diff(phi(t), t), theta(t), x(t), phi(t)], [dd_theta, dd_x, dd_phi, d_theta, d_x, d_phi, theta0, x0, phi0]);
    
[F1, F2, F3] = solve([f1,f2,f3],[dd_theta,dd_x,dd_phi]);

JA = jacobian([d_theta,F1,d_x,F2,d_phi,F3],[theta0,d_theta,x0,d_x,phi0,d_phi]);
JA = subs(JA, [theta0,d_theta,d_x,phi0,d_phi,T,Tp],[0,0,0,0,0,0,0]);
JA=subs(JA,[R,L,LM,l,mw,mp,M,Iw,Ip,IM,g],[R1,L1,LM1,l1,mw1,mp1,M1,Iw1,Ip1,IM1,9.8]);
JA=double(JA);

JB = jacobian([d_theta,F1,d_x,F2,d_phi,F3],[T,Tp]);
JB = subs(JB, [theta0,d_theta,d_x,phi0,d_phi,T,Tp],[0,0,0,0,0,0,0]);
JB=subs(JB,[R,L,LM,l,mw,mp,M,Iw,Ip,IM,g],[R1,L1,LM1,l1,mw1,mp1,M1,Iw1,Ip1,IM1,9.8]);
JB=double(JB);    

Q=diag([10 10 500 100 0.001 1]);%theta d_theta x d_x phi d_phi
R=[1 0;0 2.5];                %T Tp
K=lqr(JA,JB,Q,R);
end


    
   