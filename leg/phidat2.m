syms phi1(t)  phi4(t)  d_phi1 d_phi4 phi_1 phi_4 l1 l2 l3 l4 l5

YD = l4*sin(phi4);
YB = l1*sin(phi1);
XD = l5 + l4*cos(phi4);
XB = l1*cos(phi1); 
lBD = sqrt((XD - XB)*(XD - XB) + (YD - YB)*(YD - YB));
A0 = 2*l2*(XD - XB);
B0 = 2*l2*(YD - YB);
C0 = l2*l2 + lBD*lBD - l3*l3;
phi2 = 2*atan2((B0 + sqrt(A0*A0 + B0*B0 - C0*C0)),A0 + C0);
phi3 = atan2(YB-YD+l2*sin(phi2),XB-XD+l2*cos(phi2));
XC = l1*cos(phi1) + l2*cos(phi2);
YC = l1*sin(phi1) + l2*sin(phi2);
L0 = sqrt((XC - l5/2)*(XC - l5/2) + YC*YC);
phi0 = atan2(YC,XC - l5/2);
eqn1 = diff(L0,t);
eqn2 = diff(L0,t,2);
eqn3 = diff(phi0,t);
eqn4 = diff(phi0,t,2);
L0_dot = subs(eqn1,[diff(phi1(t),t),diff(phi4(t),t),phi1(t),phi4(t)],[d_phi1,d_phi4,phi_1,phi_4]);
L0_dot_dot = subs(eqn2,[diff(phi1(t),t),diff(phi4(t),t),phi1(t),phi4(t)],[d_phi1,d_phi4,phi_1,phi_4]);
phi0_dot = subs(eqn3,[diff(phi1(t),t),diff(phi4(t),t),phi1(t),phi4(t)],[d_phi1,d_phi4,phi_1,phi_4]);
phi0_dot_dot = subs(eqn4,[diff(phi1(t),t),diff(phi4(t),t),phi1(t),phi4(t)],[d_phi1,d_phi4,phi_1,phi_4]);

